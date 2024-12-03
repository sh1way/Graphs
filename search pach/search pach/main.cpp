#include <SFML/Graphics.hpp>
#include <locale>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
using namespace sf;

int** createG(int size) {
    int** G = new int* [size];
    for (int i = 0; i < size; i++) {
        G[i] = new int[size];
        for (int j = 0; j < size; j++) {
            G[i][j] = (i != j && rand() % 2 == 0) ? rand() % 7 + 1 : 0;
        }
    }
    return G;
}

void printG(int** G, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << G[i][j] << " ";
        }
        cout << endl;
    }
}

Color getEdgeColor(int weight) {
    switch (weight) {
    case 1: return Color(255, 0, 0);
    case 2: return Color(255, 69, 0);
    case 3: return Color(255, 255, 0);
    case 4: return Color(0, 255, 0);
    case 5: return Color(0, 255, 255);
    case 6: return Color(0, 0, 255);
    case 7: return Color(128, 0, 128);
    default: return Color::White;
    }
}

void drawArrow(RenderWindow& window, float x1, float y1, float x2, float y2, Color color, float offsetX = 0, float offsetY = 0) {
    x1 += offsetX;
    y1 += offsetY;
    x2 += offsetX;
    y2 += offsetY;

    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx);

    Vertex edge[] = {
        Vertex(Vector2f(x1, y1), color),
        Vertex(Vector2f(x2, y2), color)
    };
    window.draw(edge, 2, Lines);

    float arrowSize = 10.0f;
    ConvexShape arrow;
    arrow.setPointCount(3);
    arrow.setPoint(0, Vector2f(0, 0));
    arrow.setPoint(1, Vector2f(-arrowSize, -arrowSize / 2));
    arrow.setPoint(2, Vector2f(-arrowSize, arrowSize / 2));
    arrow.setFillColor(color);
    arrow.setPosition(x2 - dx / length * 10, y2 - dy / length * 10);
    arrow.setRotation(angle * 180.0f / M_PI);

    window.draw(arrow);
}

void createNodes(vector<CircleShape>& smallCircles, vector<Text>& labels, const Vector2f& center, int count, float radius, float smallCircleRadius, const Font& font) {
    for (int i = 0; i < count; ++i) {
        CircleShape smallCircle(smallCircleRadius);
        smallCircle.setFillColor(Color(127, 255, 212));
        float angle = i * (2 * M_PI / count);
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        smallCircle.setOrigin(smallCircleRadius, smallCircleRadius);
        smallCircle.setPosition(x, y);
        smallCircles.push_back(smallCircle);

        Text label;
        label.setFont(font);
        label.setString(to_string(i + 1));
        label.setCharacterSize(20);
        label.setFillColor(Color::Red);
        FloatRect textBounds = label.getLocalBounds();
        label.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
        label.setPosition(x - 2, y - 6);
        labels.push_back(label);
    }
}

void drawMenu(RenderWindow& window, const Font& font) {
    float menuX = 32;
    float menuY = 70;
    float spacing = 40;

    Text colorInf;
    colorInf.setFont(font);
    colorInf.setString(L"–ебра и их вес");
    colorInf.setCharacterSize(16);
    colorInf.setPosition(28, 32);
    colorInf.setFillColor(Color::White);

    window.draw(colorInf);

    for (int weight = 1; weight <= 7; ++weight) {
        CircleShape colorCircle(8);
        colorCircle.setFillColor(getEdgeColor(weight));
        colorCircle.setPosition(menuX, menuY + (weight - 1) * spacing);

        Text dashText;
        dashText.setFont(font);
        dashText.setString(" - ");
        dashText.setCharacterSize(16);
        dashText.setFillColor(Color::White);
        dashText.setPosition(menuX + 20, menuY + (weight - 1) * spacing - 4);

        Text weightText;
        weightText.setFont(font);
        weightText.setString(to_string(weight));
        weightText.setCharacterSize(16);
        weightText.setFillColor(Color::White);
        weightText.setPosition(menuX + 50, menuY + (weight - 1) * spacing - 4);

        window.draw(colorCircle);
        window.draw(weightText);
        window.draw(dashText);
    }
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    const int nG = 7;
    int** G = createG(nG);
    printG(G, nG);

    RenderWindow window(VideoMode(1400, 800), "Graf");
    Font font;
    if (!font.loadFromFile("menlo.ttf")) return -1;

    const float mainRadius = 300.0f;
    const float smallCircleRadius = 10.0f;
    Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    vector<CircleShape> smallCircles;
    vector<Text> labels;
    createNodes(smallCircles, labels, center, nG, mainRadius, smallCircleRadius, font);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }

        window.clear();

        for (int i = 0; i < nG; ++i) {
            for (int j = 0; j < nG; ++j) {
                if (G[i][j] > 0) {
                    float angle1 = i * (2 * M_PI / nG);
                    float x1 = center.x + mainRadius * cos(angle1);
                    float y1 = center.y + mainRadius * sin(angle1);
                    float angle2 = j * (2 * M_PI / nG);
                    float x2 = center.x + mainRadius * cos(angle2);
                    float y2 = center.y + mainRadius * sin(angle2);
                    float offsetX = (i < j) ? 5.0f : (i > j ? -5.0f : 0);
                    drawArrow(window, x1, y1, x2, y2, getEdgeColor(G[i][j]), offsetX, 0);
                }
            }
        }

        for (size_t i = 0; i < smallCircles.size(); ++i) {
            window.draw(smallCircles[i]);
            window.draw(labels[i]);
        }

        drawMenu(window, font);

        window.display();
    }

    return 0;
}
