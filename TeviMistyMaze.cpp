// TeviMistyMaze.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <future>

typedef char c8;
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

// WARNING: Including jetpack causes insane delay as it increases the number of accessible locations exponentially.
// H12L is 17 maps away from A4 without it and seemingly 10 maps away with it.
// However, it takes my monster $12000 PC some 30-45 MINUTES and nearly 30 GIGABYTES of RAM to find the shortest available path using the jetpack.
// Proceed with extreme caution.

// #define JETPACK

#ifndef JETPACK
#define JETPACK 0
#else
#undef JETPACK
#define JETPACK 1
#endif


enum MapID : s32
{
    NA = -1,
    A6, A5, A4, A3, A2, A1,
    B6L, B6R, B5, B4, B3L, B3R, B2L, B2R, B1,
    C9, C8, C7, C6, C5, C4, C3, C2, C1,
    D11, D10, D9, D8, D7, D6, D5L, D5R, D4, D3, D2, D1,
    E11, E10, E9, E8, E7, E6, E5, E4, E3, E2, E1,
    F9, F8L, F8R, F7, F6, F5, F4, F3, F2L, F2R, F1,
    G11, G10, G9, G8, G7, G6L, G6B, G6R, G5, G4, G3, G2, G1,
    H12L, H12R, H11, H10, H9, H8, H7, H6, H5, H4, H3, H2, H1,
    COUNT
};

std::string GetMapName(MapID id)
{
    switch (id)
    {
    case -1: return "NA";
    case A6: return "A6";
    case A5: return "A5";
    case A4: return "A4";
    case A3: return "A3";
    case A2: return "A2";
    case A1: return "A1";
    case B6L: return "B6L";
    case B6R: return "B6R";
    case B5: return "B5";
    case B4: return "B4";
    case B3L: return "B3L";
    case B3R: return "B3R";
    case B2L: return "B2L";
    case B2R: return "B2R";
    case B1: return "B1";
    case C9: return "C9";
    case C8: return "C8";
    case C7: return "C7";
    case C6: return "C6";
    case C5: return "C5";
    case C4: return "C4";
    case C3: return "C3";
    case C2: return "C2";
    case C1: return "C1";
    case D11: return "D11";
    case D10: return "D10";
    case D9: return "D9";
    case D8: return "D8";
    case D7: return "D7";
    case D6: return "D6";
    case D5L: return "D5L";
    case D5R: return "D5R";
    case D4: return "D4";
    case D3: return "D3";
    case D2: return "D2";
    case D1: return "D1";
    case E11: return "E11";
    case E10: return "E10";
    case E9: return "E9";
    case E8: return "E8";
    case E7: return "E7";
    case E6: return "E6";
    case E5: return "E5";
    case E4: return "E4";
    case E3: return "E3";
    case E2: return "E2";
    case E1: return "E1";
    case F9: return "F9";
    case F8L: return "F8L";
    case F8R: return "F8R";
    case F7: return "F7";
    case F6: return "F6";
    case F5: return "F5";
    case F4: return "F4";
    case F3: return "F3";
    case F2L: return "F2L";
    case F2R: return "F2R";
    case F1: return "F1";
    case G11: return "G11";
    case G10: return "G10";
    case G9: return "G9";
    case G8: return "G8";
    case G7: return "G7";
    case G6L: return "G6L";
    case G6B: return "G6B";
    case G6R: return "G6R";
    case G5: return "G5";
    case G4: return "G4";
    case G3: return "G3";
    case G2: return "G2";
    case G1: return "G1";
    case H12L: return "H12L";
    case H12R: return "H12R";
    case H11: return "H11";
    case H10: return "H10";
    case H9: return "H9";
    case H8: return "H8";
    case H7: return "H7";
    case H6: return "H6";
    case H5: return "H5";
    case H4: return "H4";
    case H3: return "H3";
    case H2: return "H2";
    case H1: return "H1";
    default: return "UNKNOWN";
    }
}

struct Map
{
    MapID connections[4];
};

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

constexpr auto TR = true;
// constexpr auto FA = false;

static Map MAPS[MapID::COUNT];
constexpr MapID START = A4;
constexpr MapID END = H12L;

void LoadMapData()
{
    MAPS[A6] = { B6R, B6R, NA, NA };
    MAPS[A5] = { NA, B4, NA, B4 };
    MAPS[A4] = { NA, NA, D6, G5 };
    MAPS[A3] = { NA, F4, D7, A2 };
    MAPS[A2] = { NA, F2L, A3, A1 };
    MAPS[A1] = { C1, NA, C3, A6 }; // jetpack
    MAPS[B6L] = { NA, NA, F2R, NA };
    MAPS[B6R] = { B5, B5, NA, NA };
    MAPS[B5] = { NA, C6, B6R, D5L }; // down impossible?
    MAPS[B4] = { A5, C5, A5, NA };
    MAPS[B3L] = { NA, NA, H10, NA };
    MAPS[B3R] = { NA, B6L, NA, C4 };
    MAPS[B2L] = { NA, NA, C8, B2R };
    MAPS[B2R] = { NA, NA, B2L, E1 };
    MAPS[B1] = { NA, C1, B2R, G3 };
    MAPS[C9] = { NA, NA, NA, C8 };
    MAPS[C8] = { NA, F2L, E6, B2L };
    MAPS[C7] = { NA, D7, F2R, G9 };
    MAPS[C6] = { B5, D6, A4, D11 }; // jetpack
    MAPS[C5] = { B4, NA, F1, E10 };
    MAPS[C4] = { B3R, G5, A1, A1 };
    MAPS[C3] = { NA, B3R, C4, A1 };
    MAPS[C2] = { NA, NA, C3, NA };
    MAPS[C1] = { B1, A1, NA, NA };
    MAPS[D11] = { NA, NA, C6, D10 };
    MAPS[D10] = { NA, NA, D11, A1 };
    MAPS[D9] = { C9, E9, D10, D8 };
    MAPS[D8] = { C8, B2L, D9, B1 };
    MAPS[D7] = { C7, E7, A4, D6 };
    MAPS[D6] = { C6, C6, G5, A4 }; // jetpack
    MAPS[D5L] = { NA, NA, G6R, NA };
    MAPS[D5R] = { NA, E5, NA, NA };
    MAPS[D4] = { D4, E4, NA, D3 };
    MAPS[D3] = { B3R, B2L, E8, H1 };
    MAPS[D2] = { NA, NA, H3, NA };
    MAPS[D1] = { NA, E1, NA, NA };
    MAPS[E11] = { NA, A5, B4, E10 };
    MAPS[E10] = { NA, A5, E11, E9 }; // down impossible?
    MAPS[E9] = { D9, E1, E10, G8 }; // jetpack
    MAPS[E8] = { D8, F7, E9, E7 };
    MAPS[E7] = { D7, H4, A3, E6 }; // jetpack
    MAPS[E6] = { D6, B6L, E7, B6L}; // jetpack
    MAPS[E5] = { D5R, F4, C8, A4 }; // jetpack
    MAPS[E4] = { D4, NA, E5, E3 }; // jetpack, left impossible?
    MAPS[E3] = { D3, F8L, E4, NA }; // jetpack
    MAPS[E2] = { NA, F2R, NA, E1 };
    MAPS[E1] = { D1, E9, E2, D8 };
    MAPS[F9] = { E11, NA, H11, H10 }; // jetpack
    MAPS[F8L] = { E10, NA, H6, F8R };
    MAPS[F8R] = { NA, NA, F8L, NA };
    MAPS[F7] = { E8, G8, NA, NA };
    MAPS[F6] = { E7, NA, NA, B3L };
    MAPS[F5] = { NA, NA, F6, NA };
    MAPS[F4] = { E5, G5, NA, NA }; // up impossible?
    MAPS[F3] = { NA, G4, NA, F2L };
    MAPS[F2L] = { C8, NA, G4, F2R };
    MAPS[F2R] = { E2, B3R, F2L, C2 }; // jetpack
    MAPS[F1] = { E1, D4, F2R, E10 };
    MAPS[G11] = { NA, NA, G10, G10 };
    MAPS[G10] = { NA, F2L, G11, G11 };
    MAPS[G9] = { NA, E5, G10, C3 };
    MAPS[G8] = { F7, H8, E1, G6L };
    MAPS[G7] = { NA, NA, G8, G6L };
    MAPS[G6L] = { NA, NA, G7, NA };
    MAPS[G6B] = { NA, H6, NA, NA };
    MAPS[G6R] = { F5, NA, NA, E9 };
    MAPS[G5] = { F4, NA, A4, A4 }; // jetpack
    MAPS[G4] = { F3, H4, H9, F2L }; // jetpack
    MAPS[G3] = { NA, H3, G4, G2 };
    MAPS[G2] = { F2R, NA, B2R, G1 }; // jetpack
    MAPS[G1] = { F1, H1, F9, F9 }; // jetpack, down impossible?
    MAPS[H12L] = { NA, NA, H11, H12R };
    MAPS[H12R] = { NA, NA, H12L, H11 };
    MAPS[H11] = { G11, NA, H12R, H10 }; // jetpack
    MAPS[H10] = { G10, NA, G1, H9 }; // jetpack
    MAPS[H9] = { G9, NA, H10, G4 }; // jetpack
    MAPS[H8] = { G8, NA, H1, H2 };
    MAPS[H7] = { NA, NA, H8, NA };
    MAPS[H6] = { G6B, NA, NA, H5 };
    MAPS[H5] = { NA, NA, H6, E9 };
    MAPS[H4] = { E7, G5, A4, A4 };
    MAPS[H3] = { G3, NA, H4, H2 };
    MAPS[H2] = { NA, NA, H3, H7 };
    MAPS[H1] = { C9, NA, D3, D2 };
}

bool CanAccess(MapID position, Direction direction)
{
    switch (position)
    {
        // jetpack required
    case A1:
    case C6:
    case D6:
    case E9:
    case E7:
    case E6:
    case E5:
    case E3:
    case F9:
    case F6:
    case F2R:
    case G8:
    case G5:
    case G4:
    case G2:
    case H11:
    case H10:
    case H9:
    jetpack_check:
        if (direction == UP)
        {
            return JETPACK;
        }
        return true;

    case G1: // down is impossible but up is possible with jetpack
        if (direction == DOWN)
        {
            return false;
        }
        goto jetpack_check;

    case E4: // left impossible but up is possible with jetpack
        if (direction == LEFT)
        {
            return false;
        }
        goto jetpack_check;

    case F4: // up impossible
        if (direction == UP)
        {
            return false;
        }
        return true;

        // down impossible
    case B5:
    case E10:
        if (direction == DOWN)
        {
            return false;
        }
        return true;
    default: return true;
    }
}

static size_t STATIC_PATH_ID;

void CheckPath(std::vector<std::vector<MapID>>& working_paths, std::vector<MapID>& shortest_path, const size_t& path_id, const MapID& position, const MapID& target)
{
    // if the current path contains values, check if the current position already exists in that path
    if (working_paths[path_id].size())
    {
        auto pos_find = std::find(working_paths[path_id].begin(), working_paths[path_id].end(), position);
        if (pos_find != working_paths[path_id].end()
            || (shortest_path.size() && working_paths[path_id].size() + 1 >= shortest_path.size()))
        {
            // we do not want duplicate maps or longer paths so prune this path
            // this path_id should be unique
            working_paths[path_id].clear();
            return;
        }
    }

    // append the current position
    working_paths[path_id].push_back(position);

    // if we've arrived at the desired location, return to the previous stack
    if (position == target && (!shortest_path.size() || working_paths[path_id].size() < shortest_path.size()))
    {
        shortest_path.clear();
        for (const auto& map : working_paths[path_id])
        {
            shortest_path.push_back(map);
        }
        working_paths[path_id].clear();
        return;
    }

    // check all connections in the current position
    for (s8 i = 0; i < 4; i++)
    {
        // validate the path
        if (MAPS[position].connections[i] != NA && CanAccess(position, Direction(i)))
        {
            // add a new path with a unique id
            working_paths.push_back(working_paths[path_id]);
            CheckPath(working_paths, shortest_path, STATIC_PATH_ID++, MAPS[position].connections[i], target);
        }
    }
}

void PrintPath(const std::vector<MapID>& path)
{
    std::stringstream directions;
    std::stringstream map_names;
    map_names << GetMapName(path[0]);
    if (path.size() > 1)
    {
        map_names << ", ";
    }
    for (s32 i = 1; i < path.size(); i++)
    {
        if (path[i] == MAPS[path[i - 1]].connections[RIGHT])
        {
            directions << "right";
        }
        else if (path[i] == MAPS[path[i - 1]].connections[LEFT])
        {
            directions << "left";
        }
        else if (path[i] == MAPS[path[i - 1]].connections[DOWN])
        {
            directions << "down";
        }
        else if (path[i] == MAPS[path[i - 1]].connections[UP])
        {
            directions << "up";
        }
        else
        {
            directions << "ERROR";
        }
        map_names << GetMapName(path[i]);
        if (i != path.size() - 1)
        {
            directions << ", ";
            map_names << ", ";
        }
    }
    directions << std::endl;
    map_names << std::endl;
    printf(directions.str().c_str());
    printf(map_names.str().c_str());
}

constexpr size_t important_map_count = 13;
static MapID important_maps[important_map_count] =
{
    A4,
    C9,
    C4,
    C2,
    D5R,
    D2,
    E6,
    F8R,
    F7,
    F4,
    F2L,
    G11,
    H12L
};

void FindPath(const MapID& from, const MapID& to)
{
    STATIC_PATH_ID = 0;
    std::vector<std::vector<MapID>> working_paths{};
    std::vector<MapID> shortest_path{};
    working_paths.push_back({ });
    CheckPath(working_paths, shortest_path, STATIC_PATH_ID++, from, to);
    working_paths.clear();

    if (!shortest_path.size())
    {
        printf("No path found.");
        return;
    }

    PrintPath(shortest_path);
}

int main()
{
    LoadMapData();

    /*for (size_t sid = 0; sid < important_map_count; sid++)
    {
        for (size_t eid = 0; eid < important_map_count; eid++)
        {
            if (sid == eid)
            {
                continue;
            }

            FindPath(important_maps[sid], important_maps[eid]);
        }
        printf("\n");
    }*/
    FindPath(C8, A4);
    std::cout << '\a';
    return 0;
}
