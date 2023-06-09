#include "Tablet.h"

Tablet::Tablet(sf::String text, sf::Vector2f pos) {
	do {
		t_tablet.setSize(t_tablet_size);
		if (!t_tablet_texture.loadFromFile("png/tablet8.png")) { exit(1); }
		t_tablet.setTexture(&t_tablet_texture);
		t_tablet_pos.x = pos.x - t_tablet_size.x / 2;
		t_tablet_pos.y = pos.y - t_tablet_size.y / 2;

		if (!t_font.loadFromFile("font/Oldscriptc.otf")) { exit(1); }
		t_tablet_text.setString(text);
		t_tablet_text.setFont(t_font);
		t_tablet_text.setCharacterSize(t_text_size);
		t_tablet_text.setFillColor(sf::Color(RGB_BEIGE));
		t_tablet_text.setOutlineThickness(2);
		t_tablet_text.setOutlineColor(sf::Color(RGB_DARK_BROWN));
	} while (false);
}

void Tablet::draw(sf::RenderWindow& window) {
	t_tablet.setPosition(t_tablet_pos);
	t_tablet_text.setPosition(t_tablet_pos.x + t_tablet_size.x / 2 - t_tablet_text.getGlobalBounds().width / 2,
		t_tablet_pos.y + t_tablet_size.y / 2 - t_tablet_text.getGlobalBounds().height + 10);

	window.draw(t_tablet);
	window.draw(t_tablet_text);

}

void Tablet::change_color(sf::Color color) {
	t_tablet_text.setFillColor(color);
}


