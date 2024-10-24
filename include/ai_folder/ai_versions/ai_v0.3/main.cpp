#include "include/rl_tools/operations/cpu_mux.h"
#include "include/rl_tools/nn/optimizers/adam/instance/operations_generic.h"
#include "include/rl_tools/rl/environments/pendulum/operations_generic.h"
#include "include/rl_tools/nn_models/mlp/operations_generic.h"
#include "include/rl_tools/nn_models/sequential/operations_generic.h"
#include "include/rl_tools/nn/optimizers/adam/operations_generic.h"
#include "include/rl_tools/rl/algorithms/td3/operations_generic.h"
#include "include/rl_tools/rl/components/off_policy_runner/operations_generic.h"
#include "include/rl_tools/rl/utils/evaluation/operations_generic.h"
namespace rlt = rl_tools;

using T = float;
using DEVICE = rlt::devices::DEVICE_FACTORY<rlt::devices::DefaultCPUSpecification>;
using TI = typename DEVICE::index_t;

using ENVIRONMENT_PARAMETERS = rlt::rl::environments::pendulum::DefaultParameters<T>;
using ENVIRONMENT_SPEC = rlt::rl::environments::pendulum::Specification<T, TI, ENVIRONMENT_PARAMETERS>;
using ENVIRONMENT = rlt::rl::environments::Pendulum<ENVIRONMENT_SPEC>;

struct TD3_PENDULUM_PARAMETERS: rlt::rl::algorithms::td3::DefaultParameters<T, TI>{
    constexpr static typename DEVICE::index_t CRITIC_BATCH_SIZE = 100;
    constexpr static typename DEVICE::index_t ACTOR_BATCH_SIZE = 100;
};
constexpr TI STEP_LIMIT = 10000;
constexpr TI REPLAY_BUFFER_CAP = STEP_LIMIT;
constexpr int N_WARMUP_STEPS = TD3_PENDULUM_PARAMETERS::ACTOR_BATCH_SIZE;
constexpr TI EPISODE_STEP_LIMIT = 200;
constexpr TI ACTOR_NUM_LAYERS = 3;
constexpr TI ACTOR_HIDDEN_DIM = 64;
constexpr TI CRITIC_NUM_LAYERS = 3;
constexpr TI CRITIC_HIDDEN_DIM = 64;
constexpr auto ACTOR_ACTIVATION_FUNCTION = rlt::nn::activation_functions::RELU;
constexpr auto CRITIC_ACTIVATION_FUNCTION = rlt::nn::activation_functions::RELU;
constexpr auto ACTOR_ACTIVATION_FUNCTION_OUTPUT = rlt::nn::activation_functions::TANH;
constexpr auto CRITIC_ACTIVATION_FUNCTION_OUTPUT = rlt::nn::activation_functions::IDENTITY;
using TD3_PARAMETERS = TD3_PENDULUM_PARAMETERS;

using ACTOR_SPEC = rlt::nn_models::mlp::Specification<T, TI, ENVIRONMENT::Observation::DIM, ENVIRONMENT::ACTION_DIM, ACTOR_NUM_LAYERS, ACTOR_HIDDEN_DIM, ACTOR_ACTIVATION_FUNCTION, ACTOR_ACTIVATION_FUNCTION_OUTPUT>;
using CRITIC_SPEC = rlt::nn_models::mlp::Specification<T, TI, ENVIRONMENT::Observation::DIM + ENVIRONMENT::ACTION_DIM, 1, CRITIC_NUM_LAYERS, CRITIC_HIDDEN_DIM, CRITIC_ACTIVATION_FUNCTION, CRITIC_ACTIVATION_FUNCTION_OUTPUT>;
using OPTIMIZER_SPEC = typename rlt::nn::optimizers::adam::Specification<T, TI>;
using PARAMETER_TYPE = rlt::nn::parameters::Adam;
using CAPABILITY_ACTOR = rl_tools::nn::layer_capability::Gradient<PARAMETER_TYPE, TD3_PARAMETERS::ACTOR_BATCH_SIZE>;
using CAPABILITY_CRITIC = rl_tools::nn::layer_capability::Gradient<PARAMETER_TYPE, TD3_PARAMETERS::CRITIC_BATCH_SIZE>;
using CAPABILITY_TARGET = rl_tools::nn::layer_capability::Forward;

using OPTIMIZER = rlt::nn::optimizers::Adam<OPTIMIZER_SPEC>;

using ACTOR_TYPE = rlt::nn_models::mlp::NeuralNetwork<CAPABILITY_ACTOR, ACTOR_SPEC>;
using ACTOR_TARGET_TYPE = rl_tools::nn_models::mlp::NeuralNetwork<CAPABILITY_TARGET, ACTOR_SPEC>;

using CRITIC_TYPE = rl_tools::nn_models::mlp::NeuralNetwork<CAPABILITY_CRITIC, CRITIC_SPEC>;
using CRITIC_TARGET_TYPE = rl_tools::nn_models::mlp::NeuralNetwork<CAPABILITY_TARGET, CRITIC_SPEC>;

using TD3_SPEC = rlt::rl::algorithms::td3::Specification<T, DEVICE::index_t, ENVIRONMENT, ACTOR_TYPE, ACTOR_TARGET_TYPE, CRITIC_TYPE, CRITIC_TARGET_TYPE, OPTIMIZER, TD3_PARAMETERS>;
using ACTOR_CRITIC_TYPE = rlt::rl::algorithms::td3::ActorCritic<TD3_SPEC>;

struct OFF_POLICY_RUNNER_PARAMETERS: rlt::rl::components::off_policy_runner::ParametersDefault<T, TI>{
    // you can selectively overwrite defaults here
};

static_assert(ACTOR_CRITIC_TYPE::SPEC::PARAMETERS::ACTOR_BATCH_SIZE == ACTOR_CRITIC_TYPE::SPEC::PARAMETERS::CRITIC_BATCH_SIZE);

DEVICE device;
OPTIMIZER optimizer;
auto rng = rlt::random::default_engine(typename DEVICE::SPEC::RANDOM{}, 1);
//rlt::rl::environments::DummyUI ui; // this is used later to signal the rlt::evaluate to not use a UI

ACTOR_CRITIC_TYPE actor_critic;
rlt::malloc(device, actor_critic);
rlt::init(device, actor_critic, rng);

OFF_POLICY_RUNNER_TYPE off_policy_runner;
rlt::malloc(device, off_policy_runner);
ENVIRONMENT envs[decltype(off_policy_runner)::N_ENVIRONMENTS];
ENVIRONMENT::Parameters env_parameters[decltype(off_policy_runner)::N_ENVIRONMENTS];
rlt::init(device, off_policy_runner, envs, env_parameters);

OFF_POLICY_RUNNER_TYPE::Batch<TD3_PARAMETERS::CRITIC_BATCH_SIZE> critic_batch;
rlt::rl::algorithms::td3::CriticTrainingBuffers<ACTOR_CRITIC_TYPE::SPEC> critic_training_buffers;
CRITIC_TYPE::Buffer<ACTOR_CRITIC_TYPE::SPEC::PARAMETERS::CRITIC_BATCH_SIZE> critic_buffer;
rlt::malloc(device, critic_batch);
rlt::malloc(device, critic_training_buffers);
rlt::malloc(device, critic_buffer);

OFF_POLICY_RUNNER_TYPE::Batch<TD3_PARAMETERS::ACTOR_BATCH_SIZE> actor_batch;
rlt::rl::algorithms::td3::ActorTrainingBuffers<ACTOR_CRITIC_TYPE::SPEC> actor_training_buffers;
ACTOR_TYPE::Buffer<ACTOR_CRITIC_TYPE::SPEC::PARAMETERS::ACTOR_BATCH_SIZE> actor_buffer;
ACTOR_TYPE::Buffer<OFF_POLICY_RUNNER_SPEC::PARAMETERS::N_ENVIRONMENTS> actor_buffer_eval;
rlt::malloc(device, actor_batch);
rlt::malloc(device, actor_training_buffers);
rlt::malloc(device, actor_buffer_eval);
rlt::malloc(device, actor_buffer);

auto start_time = std::chrono::high_resolution_clock::now();
for(int step_i = 0; step_i < STEP_LIMIT; step_i+=OFF_POLICY_RUNNER_SPEC::PARAMETERS::N_ENVIRONMENTS){
    // Taking the training time and evaluating the agent
    if(step_i % 1000 == 0 || step_i == STEP_LIMIT - 1){
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = current_time - start_time;
        rlt::rl::utils::evaluation::Result<rlt::rl::utils::evaluation::Specification<T, TI, ENVIRONMENT, 10, EPISODE_STEP_LIMIT>> result;
        rlt::evaluate(device, envs[0], ui, actor_critic.actor, result, actor_buffer_eval, rng);
        std::cout << "Step: " << step_i << "/" << (STEP_LIMIT-1) << " mean return: " << result.returns_mean << " (" << elapsed_seconds.count() << "s)" << std::endl;
    }
    // One environment step (saved in the replay buffer)
    rlt::step(device, off_policy_runner, actor_critic.actor, actor_buffer_eval, rng);

    // TD3 training using the replay buffer
    if(step_i > N_WARMUP_STEPS){
        // Critic training
        for(int critic_i = 0; critic_i < 2; critic_i++){
            rlt::target_action_noise(device, actor_critic, critic_training_buffers.target_next_action_noise, rng);
            rlt::gather_batch(device, off_policy_runner, critic_batch, rng);
            rlt::train_critic(device, actor_critic, critic_i == 0 ? actor_critic.critic_1 : actor_critic.critic_2, critic_batch, optimizer, actor_buffer, critic_buffer, critic_training_buffers, rng);
        }
        // Actor training
        if(step_i % 2 == 0){
            {
                rlt::gather_batch(device, off_policy_runner, actor_batch, rng);
                rlt::train_actor(device, actor_critic, actor_batch, optimizer, actor_buffer, critic_buffer, actor_training_buffers, rng);
            }

            rlt::update_critic_targets(device, actor_critic);
            rlt::update_actor_target(device, actor_critic);
        }
    }
}