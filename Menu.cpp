#include "Menu.h"


Menu::Menu(sf::RenderWindow& window, std::initializer_list <sf::String> list) {
	m_pos.x = window.getSize().x / 2;
	m_pos.y = window.getSize().y / 2.5;
	m_menu.reserve(list.size());
	for (int i = 0; i < list.size(); i++) {
		std::shared_ptr<Tablet> t{std::make_shared<Tablet>(*(list.begin() + i), sf::Vector2f{m_pos.x, m_pos.y + m_step * i}) };
		m_menu.push_back(t);
		if (i == selected_point) {
			m_menu[i]->change_color(sf::Color(RGB_LIGHT_BROWN));
		}
		t = nullptr;
	}

}

void Menu::draw(sf::RenderWindow& window) {
	for (auto& item : m_menu) {
		(*item).draw(window);
	}
}

void Menu::move_up() {
	m_menu[selected_point]->change_color(sf::Color(RGB_BEIGE));

	if (selected_point == 0) {
		selected_point = m_menu.size() - 1;
	}
	else
	{
		--selected_point;
	}

	m_menu[selected_point]->change_color(sf::Color(RGB_LIGHT_BROWN));

}

void Menu::move_down() {
	m_menu[selected_point]->change_color(sf::Color(RGB_BEIGE));

	if (selected_point == m_menu.size() - 1) {
		selected_point = 0;
	}
	else
	{
		++selected_point;
	}

	m_menu[selected_point]->change_color(sf::Color(RGB_LIGHT_BROWN));

}

int Menu::selected() {
	return selected_point;
}

void Menu::menu_navigating(sf::Vector2i click_pos) {

	if (click_pos.x < m_menu[0]->get_pos().x || click_pos.x > m_menu[0]->get_pos().x + m_menu[0]->get_size().x ||
		click_pos.y < m_menu[0]->get_pos().y || click_pos.y > m_menu[m_menu.size() - 1]->get_pos().y + m_menu[0]->get_size().y) {
		return;
	}

	for (int i = 0; i < m_menu.size(); i++) {
		if (click_pos.y >= m_menu[i]->get_pos().y && click_pos.y <= m_menu[i]->get_pos().y + m_menu[i]->get_size().y) {
			m_menu[i]->change_color(sf::Color(RGB_LIGHT_BROWN));
			selected_point = i;
			for (int j = 0; j < m_menu.size(); j++) {
				if (j != i) {
					m_menu[j]->change_color(sf::Color(RGB_BEIGE));
				}
			}
			return;
		}
	}

}

int Menu::click_point(sf::Vector2i click_pos) {
	if (click_pos.x >= m_menu[selected_point]->get_size().x && click_pos.x <= m_menu[selected_point]->get_pos().x + m_menu[0]->get_size().x &&
		click_pos.y >= m_menu[selected_point]->get_pos().y || click_pos.y <= m_menu[selected_point]->get_pos().y + m_menu[0]->get_size().y) {
		return selected_point;
	}

	return -1;
}