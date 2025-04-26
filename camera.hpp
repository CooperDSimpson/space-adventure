#pragma once;


enum CAMERA_MODE {
	LOCKED,
	CHASE
};

class Camera {
public:
	double relative_x_pos = 0;
	double relative_y_pos = 0;

	int scale = 4;

	int screen_width = 59 * scale;
	int screen_height = 20 * scale;

	CAMERA_MODE mode = LOCKED;
};