#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>
#include <Thor\Particles.hpp>
#include <Thor\Vectors.hpp>
#include <tuple>

#include "RandomEngine.h"
#include "KeyBindingManager.h"
#include "JudgeBar.h"
#include "HitObjectSprite.h"
#include "HPManager.h"
#include "ScoreCounter.h"

class Conductor;
class KeyBindingManager;

class HitObjects
{
public:
	HitObjects(size_t beats_ahead, const sf::Vector2f& screen_size);
	
	void update(const Conductor& conductor);

	void prepare_draw();
	void handle_inputs(const KeyBindingManager& keys, const Conductor& conductor, HPManager& hp, ScoreCounter& score);
	
	void destroy_old_objects();
	void draw(sf::RenderWindow& window);
	void reset();
	
	void addLane();
	void addMaxObjects();
	void addOdds(size_t idx, float amount);

	enum LaneName
	{
		  R
		, L
		, U
		, D
		, UR
		, UL
		, DR
		, DL
		, TOTAL_LANES
	};

	struct Object 
	{
		float pos;
		LaneName dir;
		float start_beat;
		float target_beat;

		bool hit;
	};

private:

	void advance_objects(float progress,  const Conductor& conductor);
	struct FadeAffector
	{
		void operator()	(thor::Particle & 	particle, sf::Time 	dt)
		{
			auto alpha = (float)particle.color.a - (1.f * dt.asSeconds());
			if (alpha <= 0)
			{
				thor::abandonParticle(particle);
			}
			else
			{
				particle.color.a = alpha;
			}
		}
	};

	struct Lane
	{
		Lane()
			: start()
			, delta()
		{}
		Lane(const sf::Vector2f lstart, const sf::Vector2f lend, float laneLength, float judgebar)
			: delta((laneLength * thor::unitVector(lend - lstart)))
			, start(lend - delta  - (judgebar * thor::unitVector(lend - lstart)))
			//, start(lend)
		{}
		
		sf::Vector2f delta;
		sf::Vector2f start;

		sf::Vertex line[2];
	};
	
	std::array<float, 2> spawning_odds;

	std::vector<Lane> m_lanes;
	std::vector<std::tuple<float, sf::Vector2f>> m_laneHitPoints;

	std::vector<Object>			 m_hit_objects;
	std::vector<HitObjectSprite> m_hit_sprites;

	const float m_read_head;
	const float  m_inv_read_head;
	size_t m_last_beat;
	
	size_t m_spawn_cap;
	size_t m_open_lanes;

	RandomEngine rand;
	std::vector<KeyState> m_key_cache;

	sf::Time m_timing_window;

	sf::SoundBuffer		m_hitSoundBuffer;
	sf::Sound			m_hitSound;
	
	const float judge_radius;
	const float judge_width;
	JudgeBar m_judge;
	
	sf::Vector2f game_center;

	sf::Texture m_particles_texture;
	thor::ParticleSystem  particles;
	sf::Clock particleClock;

	enum ObjectType
	{
		SINGLE
		, MULTI
	};
	void spawn_hit_object(LaneName lane, ObjectType type, float start, float hit_beat);


	const float object_radius;
	const float object_width;
	const float object_outline;
	

	//advances one tile or advances a at rate?
	//one tile seems better
	static constexpr Object invalid_object{-1.f, TOTAL_LANES, -1.f};
};

inline bool operator==(const HitObjects::Object& ls, const HitObjects::Object& rs)
{
	return ls.dir == rs.dir && ls.target_beat == rs.target_beat; 
}