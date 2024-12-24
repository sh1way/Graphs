#ifndef RENDERING_H
#define RENDERING_H

#include <SFML/Graphics.hpp>
#include <vector>

sf::Color getEdgeColor(int weight);
void drawArrow(sf::RenderWindow& window, float x1, float y1, float x2, float y2, sf::Color color, float offsetX = 0, float offsetY = 0);
void createNodes(std::vector<sf::CircleShape>& smallCircles, std::vector<sf::Text>& labels, const sf::Vector2f& center, int count, float radius, float smallCircleRadius, const sf::Font& font);
void drawMenu(sf::RenderWindow& window, const sf::Font& font);
void runGraphicsWindow(int** G, int nG);

#endif
