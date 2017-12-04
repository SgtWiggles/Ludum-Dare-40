#pragma once

#include <string>
#include <vector>
#include <Thor\Animations.hpp>

class AnimationLoader
{
public:
	AnimationLoader();
	~AnimationLoader();

	static std::vector<thor::FrameAnimation> loadAnimations(std::string path);
};

