#pragma once

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int HALF_WINDOW_W = WINDOW_WIDTH / 2;
const int HALF_WINDOW_H = WINDOW_HEIGHT / 2;

const int FPS = 60;
const int one_second = 1000;
const int FRAME_TARGET_TIME = one_second / FPS;
int last_frame_time = 0;