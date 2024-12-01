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
                if (rand() % 2 == 0) {
                    G[i][j] = rand() % 7 + 1;
                    G[j][i] = G[i][j];
                }
                else {
                    G[i][j] = 0;
                    G[j][i] = 0;
                }
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

Color getEdgeColor(int weight) {
    switch (weight) {
    case 1: return sf::Color(255, 0, 0);
    case 2: return sf::Color(255, 69, 0);
    case 3: return sf::Color(255, 255, 0);
    case 4: return sf::Color(0, 255, 0);
    case 5: return sf::Color(0, 255, 255);
    case 6: return sf::Color(0, 0, 255);
    case 7: return sf::Color(128, 0, 128);
    default: return sf::Color::White;
    }
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    int nG = 10;
    int edges = 0;
    int** G = createG(nG);

    printG(G, nG);
    sumEdges(G, nG, edges);

    cout << "\n" << edges << endl;

    RenderWindow window(VideoMode(1400, 800), "123");

    Font font;
    if (!font.loadFromFile("menlo.ttf")) {
        return -1;
    }
    
    const float mainRadius = 300.0f;
    const int circleCount = nG;
    const float smallCircleRadius = 20.0f;

    CircleShape mainCircle(mainRadius);
    mainCircle.setFillColor(Color::Transparent);
    mainCircle.setOutlineColor(Color::White);
    mainCircle.setOutlineThickness(2);

    Vector2u windowSize = window.getSize();
    Vector2f center(windowSize.x / 2.0f, windowSize.y / 2.0f);
    mainCircle.setOrigin(mainRadius, mainRadius);
    mainCircle.setPosition(center);

    vector<CircleShape> smallCircles;
    vector<Text> labels;

    //left panel
    Text colorInf;
    colorInf.setFont(font);
    colorInf.setString(L"Ребра и их вес");
    colorInf.setCharacterSize(16);
    colorInf.setPosition(32, 32);
    colorInf.setFillColor(Color::White);

    for (int i = 0; i < circleCount; ++i) {
        CircleShape smallCircle(smallCircleRadius);
        smallCircle.setFillColor(Color(127, 255, 212));

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
        label.setPosition(x - 2, y - 6);

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

        for (int i = 0; i < nG; ++i) {
            for (int j = i + 1; j < nG; ++j) {
                if (G[i][j] > 0) {
                    float angle1 = i * (2 * M_PI / nG);
                    float x1 = center.x + mainRadius * cos(angle1);
                    float y1 = center.y + mainRadius * sin(angle1);

                    // Координаты второй вершины
                    float angle2 = j * (2 * M_PI / nG);
                    float x2 = center.x + mainRadius * cos(angle2);
                    float y2 = center.y + mainRadius * sin(angle2);

                    // Вычисляем длину и угол ребра
                    float dx = x2 - x1;
                    float dy = y2 - y1;
                    float length = sqrt(dx * dx + dy * dy);
                    float angle = atan2(dy, dx) * 180 / M_PI; // Угол в градусах

                    // Создаём ребро в виде прямоугольника
                    sf::RectangleShape edge(sf::Vector2f(length, 3.0f)); // Толщина = 3
                    edge.setPosition(x1, y1); // Начальная точка ребра
                    edge.setRotation(angle); // Угол наклона ребра

                    // Устанавливаем цвет ребра
                    edge.setFillColor(getEdgeColor(G[i][j]));

                    // Отрисовываем ребро
                    window.draw(edge);
                }
            }
        }

        for (size_t i = 0; i < smallCircles.size(); ++i) {
            window.draw(smallCircles[i]);
            window.draw(labels[i]);
        }

        window.draw(colorInf);

        // Отрисовка меню слева
        float menuX = 32;  // Положение меню по X
        float menuY = 70;  // Начальная Y-позиция для первого элемента
        float spacing = 40; // Отступ между элементами меню
        for (int weight = 1; weight <= 7; ++weight) {
            // Создаём круг
            CircleShape colorCircle(8); // Радиус = 8 для маленьких кругов
            colorCircle.setFillColor(getEdgeColor(weight)); // Устанавливаем цвет в зависимости от веса
            colorCircle.setPosition(menuX, menuY + (weight - 1) * spacing); // Расположение по Y с отступом

            Text dashText;
            dashText.setFont(font);
            dashText.setString(" - ");
            dashText.setCharacterSize(16);
            dashText.setFillColor(Color::White);
            dashText.setPosition(menuX + 20, menuY + (weight - 1) * spacing - 4);

            // Создаём текст веса
            Text weightText;
            weightText.setFont(font);
            weightText.setString(to_string(weight));
            weightText.setCharacterSize(16);
            weightText.setFillColor(Color::White);
            weightText.setPosition(menuX + 50, menuY + (weight - 1) * spacing - 4); // Расположение текста рядом с кругом

            // Отрисовываем элементы меню
            window.draw(colorCircle);
            window.draw(weightText);
            window.draw(dashText);
        }

        window.display();
    }


    return 0;
}