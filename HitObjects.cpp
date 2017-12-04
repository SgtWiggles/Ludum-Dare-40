#include "HitObjects.h"
#include "Conductor.h"

#include <algorithm>
#include <fmt/printf.h>
#include <Thor\Math.hpp>
#include <math.h>

HitObjects::HitObjects(size_t read_head, const sf::Vector2f& screen_size)
	: m_read_head(float(read_head))
	, m_inv_read_head(1.f / float(m_read_head))
	, m_spawn_cap(0)
	, rand()
	, m_open_lanes(0)
	, m_timing_window(sf::milliseconds(100))
	, judge_radius(std::min(screen_size.x, screen_size.y) / 10.f) //14
	, judge_width(judge_radius / 10.f) //10
	, m_judge(judge_radius, judge_width, judge_radius / 25.f, sf::Color::Red, sf::Color::White, screen_size / 2.f)
	, object_radius(std::min(screen_size.x, screen_size.y) / 20.f)
	, object_width(object_radius / 6.f)
	, object_outline(object_width / 2.f)
	, game_center(screen_size / 2.f)
{
	float minaxis = std::min(screen_size.x, screen_size.y);

	m_particles_texture.loadFromFile("resources/particle.png");
	particles.setTexture(m_particles_texture);
	thor::ScaleAffector expand(sf::Vector2f(6.f,6.f));
	FadeAffector fade;
	particles.addAffector(expand);
	particles.addAffector(fade);

	if (!m_hitSoundBuffer.loadFromFile("resources/click/hitsound.wav"))
	{
		fmt::print("failed to load hitsound.wav\n");
	}

	m_hitSound.setBuffer(m_hitSoundBuffer);

	m_key_cache.resize(KeyBindingManager::TOTAL_BINDS);
	//TODO maybe replace lanes with color gradient rect then draw center circle!
	
	m_lanes.resize(size_t(LaneName::TOTAL_LANES));

	const float hx = screen_size.x / 2.f;
	const float hy = screen_size.y / 2.f;
	const float fx = screen_size.x;
	const float fy = screen_size.y;

	const float laneLength = minaxis * 0.75f;
	m_lanes[LaneName::R]  = Lane({ fx, hx }	, {hx, hx},laneLength, judge_radius);
	m_lanes[LaneName::L] =  Lane({ 0.f, hx },  {hx,hx},laneLength, judge_radius);
	m_lanes[LaneName::U]  = Lane({ hx, 0.f }, { hx,hx},laneLength, judge_radius);
	m_lanes[LaneName::D]  = Lane({ hx, fx }	, { hx,hx},laneLength, judge_radius);
	m_lanes[LaneName::UR] = Lane({ fx, 0.f }, { hx,hx},laneLength, judge_radius);
	m_lanes[LaneName::UL] = Lane({ 0.f, 0.f },{ hx,hx},laneLength, judge_radius);
	m_lanes[LaneName::DR] = Lane({ fx, fx }	, { hx,hx},laneLength, judge_radius);
	m_lanes[LaneName::DL] = Lane({ 0.f, fx }, { hx,hx},laneLength, judge_radius);

	m_laneHitPoints.push_back(std::make_tuple(judge_width * 1.5f, m_lanes[LaneName::R].start + m_lanes[LaneName::R].delta));
	m_laneHitPoints.push_back(std::make_tuple(judge_width * 1.5f, m_lanes[LaneName::L].start + m_lanes[LaneName::L].delta));
	m_laneHitPoints.push_back(std::make_tuple(judge_width * 1.5f, m_lanes[LaneName::U].start + m_lanes[LaneName::U].delta));
	m_laneHitPoints.push_back(std::make_tuple(judge_width * 1.5f, m_lanes[LaneName::D].start  + m_lanes[LaneName::D].delta));
	m_laneHitPoints.push_back(std::make_tuple(judge_width * 1.5f, m_lanes[LaneName::UR].start + m_lanes[LaneName::UR].delta));
	m_laneHitPoints.push_back(std::make_tuple(judge_width * 1.5f, m_lanes[LaneName::UL].start + m_lanes[LaneName::UL].delta));
	m_laneHitPoints.push_back(std::make_tuple(judge_width * 1.5f, m_lanes[LaneName::DR].start + m_lanes[LaneName::DR].delta));
	m_laneHitPoints.push_back(std::make_tuple(judge_width * 1.5f, m_lanes[LaneName::DL].start + m_lanes[LaneName::DL].delta));
}

void HitObjects::update(const Conductor& conductor)
{
	const auto current_beat = conductor.currentTick();
	if (current_beat != m_last_beat)
	{
		m_last_beat += 1;
		size_t choice = 1;
		
		if (choice == 1)
		{
			//floor(abs(random() - random()) * (1 + max - min) + min)
			auto num_obj = std::floorf((std::abs(rand.get_real(0.f,1.f) - rand.get_real(0.f,1.f))) * (1 + m_spawn_cap));
			if (num_obj == 0)
			{
				num_obj = 1;
			}
			fmt::print("num{}\n", num_obj);
			const auto type = [num_obj](auto olanes) {
				if (num_obj >= 2 && olanes >= 2)
					return MULTI;
				return SINGLE;
			}(m_open_lanes);


			std::vector<bool> occ_lanes(LaneName::TOTAL_LANES, false);

			for (size_t obj = 0; obj < num_obj && obj < m_open_lanes; ++obj)
			{
				auto spawn_lane = rand.get_int<size_t>(m_open_lanes - 1);
				while (occ_lanes[spawn_lane])  spawn_lane = rand.get_int<size_t>(m_open_lanes - 1);
				occ_lanes[spawn_lane] = true;
				float target = float(current_beat) + m_read_head;
				spawn_hit_object(LaneName(spawn_lane), type, current_beat, target);
			}
		}
	}

	const auto progress = float(conductor.currentTick() + conductor.getBeatProgress());
	advance_objects(progress, conductor);
}

void HitObjects::prepare_draw()
{
	const auto end = m_hit_objects.size();
	for (size_t i = 0; i < end; ++i)
	{
		const auto& obj = m_hit_objects[i];
		const auto& lane = m_lanes[size_t(obj.dir)];
		const sf::Vector2f pos = (obj.pos * lane.delta) + lane.start;		
		//std::cout << "Updateing " << pos.x << "," << pos.y << "|obj" << i << "\n";
		m_hit_sprites[i].setPosition(pos);
	}
}

void HitObjects::handle_inputs(const KeyBindingManager& keys, const Conductor& conductor, HPManager& hp, ScoreCounter& score)
{
	m_key_cache.clear();
	for (size_t i = 0; i < KeyBindingManager::TOTAL_BINDS; ++i)
	{
		m_key_cache.push_back(keys.get_key(KeyBindingManager::KeyBindings(i)));
	}
	bool printed = false;
	const auto now = conductor.now();
	const auto end = m_hit_objects.size();
	for(size_t i = 0; i < end; ++i)
	{
		auto& cobj = m_hit_objects[i];
		const sf::Time beat = conductor.beat_time(cobj.target_beat);
		const sf::Time lowerWindow = beat - m_timing_window;
		const sf::Time upperWindow = beat + m_timing_window;
		const auto& ckey = m_key_cache[cobj.dir];
		
		if (lowerWindow > now)
		{
			break;
		}

		if (upperWindow < now)
		{
			cobj.dir = invalid_object.dir;
			cobj.target_beat = invalid_object.target_beat;
			hp.damage(10);
		}
		
		if (ckey.time >= lowerWindow && ckey.time <= upperWindow)
		{
			if (ckey.state == KeyState::DOWN)
			{
				m_hitSound.stop();
				m_hitSound.play();
				hp.damage(-2.f);
				cobj.dir = invalid_object.dir;
				cobj.target_beat = invalid_object.target_beat;
				cobj.hit = true;
				score.addScore(1);
			}
		}

		//if(printed)
		//	fmt::print("\n");
	}
	//if (printed)
	//	fmt::print("----------------------------\n");
}
void HitObjects::destroy_old_objects()
{

	for (int i = m_hit_objects.size() - 1; i >= 0; --i)
	{
		if (m_hit_objects[i] == invalid_object)
		{
			auto spriteIter = std::begin(m_hit_sprites) + i;
			auto direction = sf::Vector2f(rand.get_real(-1.f, 1.f), rand.get_real(-1.f, 1.f) ) * 50.f;
			thor::UniversalEmitter emitter;
			emitter.setEmissionRate(25);
			if (m_hit_objects[i].hit)
			{
				emitter.setParticleColor(spriteIter->getColor());
			}
			else
			{
				emitter.setParticleColor(sf::Color(244, 66, 66));
			}

			
			emitter.setParticlePosition(spriteIter->getPosition());
			emitter.setParticlePosition(thor::Distributions::circle(spriteIter->getPosition(), object_radius));   // Emit particles in given circle
			emitter.setParticleVelocity(thor::Distributions::deflect(direction, 360.f)); // Emit towards direction with deviation of 15Åã
			emitter.setParticleScale(sf::Vector2f(1.f, 1.f));
			particles.addEmitter(emitter, sf::seconds(0.2f));

			m_hit_objects.erase(std::begin(m_hit_objects) + i);
			m_hit_sprites.erase(spriteIter);
		}
	}
	particles.update(particleClock.restart());
}
void HitObjects::draw(sf::RenderWindow & window)
{
	m_judge.draw(window);

	for (auto & i : m_hit_sprites)
	{
		i.draw(window);
	}

	window.draw(particles);

}

void HitObjects::reset()
{
	m_hit_objects.clear();
	m_hit_sprites.clear();
	m_judge.reset();
	m_last_beat = 0;
	m_spawn_cap = 1;
	m_open_lanes = 0;
	for (auto& i : spawning_odds)
	{
		i = 0;
	}
	
	spawning_odds[0] = 1;
	spawning_odds[1] = 1;

	addLane();
}

void HitObjects::addLane()
{
	if (m_open_lanes < m_lanes.size())
	{
		m_open_lanes += 1;
	}

	auto cPoint = m_laneHitPoints[m_open_lanes - 1];
	m_judge.add_hit_point(std::get<0>(cPoint), std::get<1>(cPoint));

}

void HitObjects::addMaxObjects()
{
	m_spawn_cap += 1;
}



void HitObjects::addOdds(size_t idx, float amount)
{
	spawning_odds[idx] += amount;
}

void HitObjects::spawn_hit_object(LaneName lane, ObjectType type,  float start, float hit_beat)
{
	//std::cout << "spawned object in line " << lane <<  "|beat" << hit_beat << "\n";
	sf::Color inColor = [type]() {
		if (type == SINGLE)
			return sf::Color(140, 32, 229, 255);
		if (type == MULTI)
			return sf::Color(249, 166, 57, 255);
		return sf::Color(140, 32, 229, 255);
		}();
	
	m_hit_sprites.push_back(HitObjectSprite(object_radius, object_width, object_outline, inColor, sf::Color::White));
	if (type == MULTI)
	{
		m_hit_sprites.back().addMulti(object_outline, game_center);
	}
	m_hit_objects.emplace_back(Object{ 0.f, lane , start, hit_beat, false});
}

void HitObjects::advance_objects(float progress, const Conductor& conductor)
{	
	const auto now = conductor.now();
	for (auto & i : m_hit_objects)
	{
		const auto beat = conductor.beat_time(i.target_beat);
		const auto start = conductor.beat_time(i.start_beat);

		i.pos = (now - start) / (beat - start);
	}
}


