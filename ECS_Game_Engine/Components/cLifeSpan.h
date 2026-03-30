#pragma once

struct cLifeSpan {
	//the life span will be the number of frames that this entity will live
	long long total_frames;

	cLifeSpan(long long initial_frames) : total_frames(initial_frames) {}
};
