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
    int** G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (i == j) {
                G[i][j] = 0;
            }
            else {
                G[i][j] = rand() % 2;
                G[j][i] = G[i][j];
            }
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

int sumEdges(int** G, int size, int& edges) {
    int edge = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0;j < size; j++) {
            if (G[i][j] == 1) {
                edge++;
            }
        }
    }
    edges = edge / 2;
    return edges;
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "");

    int nG = 19;
    int edges = 0;
    int** G = createG(nG);

    printG(G, nG);
    sumEdges(G, nG, edges);

    cout << "\n" << edges << endl;

    RenderWindow window(VideoMode(1200, 800), "123");

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }
    
    const float mainRadius = 300.0f;
    const int circleCount = nG;
    const float smallCircleRadius = 20.0f;

    CircleShape mainCircle(mainRadius);
    mainCircle.setFillColor(Color::Transparent);
    mainCircle.setOutlineColor(Color::White);
    mainCircle.setOutlineThickness(2);

    // Центрируем главную окружность
    Vector2u windowSize = window.getSize();
    Vector2f center(windowSize.x / 2.0f, windowSize.y / 2.0f);
    mainCircle.setOrigin(mainRadius, mainRadius);
    mainCircle.setPosition(center);

    // Создаём маленькие круги
    vector<CircleShape> smallCircles;
    vector<Text> labels;

    for (int i = 0; i < circleCount; ++i) {
        CircleShape smallCircle(smallCircleRadius);
        smallCircle.setFillColor(Color::Green);

        float angle = i * (2 * M_PI / circleCount);

        float x = center.x + mainRadius * cos(angle);
        float y = center.y + mainRadius * sin(angle);

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
        label.setPosition(x, y);

        labels.push_back(label);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear();

        for (size_t i = 0; i < smallCircles.size(); ++i) {
            window.draw(smallCircles[i]);
            window.draw(labels[i]);
        }

        window.display();
    }

    return 0;
}