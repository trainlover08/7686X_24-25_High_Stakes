#pragma once
#include <vector>
#include <cmath>

namespace dimensions {
    namespace field {
        namespace long_headings {
            /** Heading to face referee. This side of the field is furthest from the climb
             * stake. */
            constexpr float REFEREE = 0;
            /** Heading to face audience. This side of the field is closest to the climb
             * stake. */
            constexpr float AUDIENCE = 180;
            /** Heading to face red station */
            constexpr float RED_STATION = 270;
            /** Heading to face blue station */
            constexpr float BLUE_STATION = 90;
        }; // namespace long_headings

        using namespace long_headings;

        namespace short_headings {
            /** Heading to face referee. This side of the field is furthest from the climb
             * stake. */
            constexpr float REF = long_headings::REFEREE;
            /** Heading to face referee (stands). This side of the field is furthest from
             * the climb stake. */
            constexpr float STANDS = long_headings::AUDIENCE;
            /** Heading to face red driver station */
            constexpr float RED = long_headings::RED_STATION;
            /** Heading to face blue driver station */
            constexpr float BLUE = long_headings::BLUE_STATION;
        }; // namespace short_headings
        namespace DIR = short_headings;

        constexpr float FIELD_LENGTH = 140.25;

        constexpr float TILE = FIELD_LENGTH / 6;

        /** x axis goes from red driver station to the blue driver station */
        constexpr float MAX_X = TILE * 3;
        constexpr float MIN_X = -MAX_X;
        /** x axis goes from red driver station to the blue driver station */
        constexpr float MAX_Y = TILE * 3;
        constexpr float MIN_Y = -MAX_Y;

        /** Diameter of the rings */
        constexpr float RING_DIAM = 7;

        /** Mogo dimentions */
        constexpr float MOGO_FLATEDGE = TILE - 18.5;
        const float MOGO_CORNER = std::sqrt(std::pow(TILE - 20.6, 2.0f) + std::pow(MOGO_FLATEDGE, 2.0f));

        namespace statics {
            /** Corners */
            constexpr std::pair<float, float> RED_POSITIVE_CORNER = {-TILE * 3, -TILE * 3};
            constexpr std::pair<float, float> RED_NEGATIVE_CORNER = {-TILE * 3, TILE * 3};
            constexpr std::pair<float, float> BLUE_POSITIVE_CORNER = {TILE * 3, -TILE * 3};
            constexpr std::pair<float, float> BLUE_NEGATIVE_CORNER = {TILE * 3, TILE * 3};

            /** Alliance and wall stake */
            constexpr std::pair<float, float> BLUE_ALLIANCE_STAKE = {TILE * 3, 0};
            constexpr std::pair<float, float> RED_ALLIANCE_STAKE = {-TILE * 3, 0};
            constexpr std::pair<float, float> AUDIENCE_STAKE = {0, -TILE * 3};
            constexpr std::pair<float, float> REFEREE_STAKE = {0, TILE * 3};

            /** Ladder dimentions */
            constexpr std::pair<float, float> AUDIENCE_LADDER_POST = {0, -TILE};
            constexpr std::pair<float, float> REFEREE_LADDER_POST = {0, TILE};
            constexpr std::pair<float, float> RED_LADDER_POST = {-TILE, 0};
            constexpr std::pair<float, float> BLUE_LADDER_POST = {TILE, 0};
            constexpr float LADDER_BASE = 3.5f;

            namespace match {
                /** Mogo poses */
                constexpr std::pair<float, float> MOGO_LEFT_RED = {-TILE, TILE};
                constexpr std::pair<float, float> MOGO_RIGHT_RED = {-TILE, -TILE};
                constexpr std::pair<float, float> MOGO_LEFT_BLUE = {TILE, -TILE};
                constexpr std::pair<float, float> MOGO_RIGHT_BLUE = {TILE, TILE};
                constexpr std::pair<float, float> MOGO_GOAL_RUSH = {0, -2 * TILE};

                /** Im not going to add in ring poses cause that would get silly */
            };

            namespace skills {
                /** Mogo poses */
                /** @todo add in relavent skills positions */
            };
        };
    } // namespace field

    namespace robot {
        /** @todo change to correct track width */
        constexpr float TRACK_WIDTH = 16.5 - 1. / 8;
        constexpr float DRIVE_WIDTH = 29.0 / 2 + 1. / 8;
        constexpr float DRIVE_LENGTH = 32.0 / 2;
    } // namespace robot

    namespace all {
        using namespace field;
        using namespace robot;
    } // namespace all
} // namespace dimensions