#pragma once
#include <vector>
#include "KeyStateManager.h"

class Conductor;

class KeyBindingManager
{
public:
	enum KeyBindings
	{
		  R
		, L
		, U
		, D
		, UR
		, UL
		, DR
		, DL
		, TOTAL_BINDS
	};

	KeyBindingManager();
	void load_key_bindings();
	void update(const Conductor& conductor);

	KeyState get_key(KeyBindings key) const;



private:
	void set_key_bind(KeyBindings key, std::string keyStr);

	std::vector<KeyStateManager> m_key_binds;
};

