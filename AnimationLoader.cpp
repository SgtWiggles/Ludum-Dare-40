#include "AnimationLoader.h"
#include "json_loader.h"

AnimationLoader::AnimationLoader()
{
}


AnimationLoader::~AnimationLoader()
{
}

std::vector<thor::FrameAnimation> AnimationLoader::loadAnimations(std::string path)
{
	std::vector<thor::FrameAnimation> ret;
	ret.push_back(thor::FrameAnimation());

	auto js = load_json_from_file(path);
	auto frames = js["frames"];
	for (auto &i : frames)
	{
		auto& current = ret.back();
		auto frame = i["frame"];
		current.addFrame(1.f, sf::IntRect(int(frame["x"]), int(frame["y"])
										, int(frame["w"]), int(frame["h"])));
	}

	return ret;
}


