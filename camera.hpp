#pragma once;


enum CAMERA_MODE {
	LOCKED,
	CHASE
};

class Camera {
public:
	double relative_x_pos = 0;
	double relative_y_pos = 0;

	int screen_width = 50;
	int screen_height = 50;

	CAMERA_MODE mode = LOCKED;
};