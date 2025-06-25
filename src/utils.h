#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include <bits/stdc++.h>

struct pair_hash {
    std::size_t operator()(const std::pair<int,int>& p) const {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second << 1);
    }
};

int GenerateChunkSeed(int x, int z);
Image GenerateBlendedHeightmap(int position_x, int position_z, int seed);
float GetHeightAtPoint(Image heightmap, int x, int z);
int AbsolutePos2Grid(float l);
void DrawHelpMenu();
std::vector<std::pair<int, int>> GetChunksAroundPlayer(int centerX, int centerZ);

#endif // UTILS_H
