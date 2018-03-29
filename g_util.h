#pragma once

const unsigned
    SCREEN_WIDTH  = 640,
    SCREEN_HEIGHT = 480,
    SCREEN_FPS    = 60;

bool initGL();
void update();
void render();
void handleKeys(unsigned char key, int x, int y);
bool loadMedia();
