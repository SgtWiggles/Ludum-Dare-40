#include "KeyBindingManager.h"
#include <Thor/Input.hpp>
#include <fmt/printf.h>

#include "json_loader.h"
#include "Conductor.h"


KeyBindingManager::KeyBindingManager()
{
	m_key_binds.resize(TOTAL_BINDS);
}

void KeyBindingManager::load_key_bindings()
{
	auto js = load_json_from_file("./settings.json");
	auto keybinds = js["key_bindings"];
	set_key_bind(U , keybinds["up"]);
	set_key_bind(D , keybinds["down"]);
	set_key_bind(L , keybinds["left"]);
	set_key_bind(R , keybinds["right"]);
	set_key_bind(UL, keybinds["up_left"]);
	set_key_bind(UR, keybinds["up_right"]);
	set_key_bind(DL, keybinds["down_left"]);
	set_key_bind(DR, keybinds["down_right"]);
}

void KeyBindingManager::update(const Conductor & conductor)
{
	for (size_t i = 0; i < TOTAL_BINDS; ++i)
	{
		m_key_binds[i].update(conductor);
	}
}

KeyState KeyBindingManager::get_key(KeyBindings key) const
{	
	return m_key_binds[key].get_key_state();
}

void KeyBindingManager::set_key_bind(KeyBindings key, std::string keyStr)
{
	m_key_binds[key].set_key(thor::toKeyboardKey(keyStr));
	fmt::print("key {} | keystr {} | bind {} | {}\n", key, keyStr, m_key_binds[key].get_key(), sf::Keyboard::Num8);
} 

