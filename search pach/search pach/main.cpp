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

bool isSafe(int v, int** G, vector<int>& path, int pos) {
    if (G[path[pos - 1]][v] == 0) return false; // Нет ребра между последней и текущей вершиной
    for (int i = 0; i < pos; i++) {
        if (path[i] == v) return false; // Вершина уже была в пути
    }
    return true;
}

void hamiltonianCycleUtil(int** G, vector<int>& path, int pos, int size, vector<vector<int>>& allPaths) {
    if (pos == size) {
        if (G[path[pos - 1]][path[0]] > 0) {
            allPaths.push_back(path); // Сохраняем найденный цикл
        }
        return;
    }

    for (int v = 1; v < size; v++) {
        if (isSafe(v, G, path, pos)) {
            path[pos] = v;
            hamiltonianCycleUtil(G, path, pos + 1, size, allPaths);
            path[pos] = -1; // Откат
        }
    }
}

// Функция для подсчета веса цикла
int calculateCycleWeight(int** G, const vector<int>& path) {
    int weight = 0;
    for (int i = 0; i < path.size() - 1; i++) {
        weight += G[path[i]][path[i + 1]]; // Суммируем веса рёбер
    }
    weight += G[path[path.size() - 1]][path[0]]; // Добавляем вес ребра между последней и первой вершинами
    return weight;
}

void findAllHamiltonianCycles(int** G, int size, int start) {
    vector<int> path(size, -1);
    path[0] = start; // Стартовая вершина

    vector<vector<int>> allPaths;
    hamiltonianCycleUtil(G, path, 1, size, allPaths);

    if (allPaths.empty()) {
        cout << "Гамильтонов циклов не существует." << endl;
    }
    else {
        cout << "Найдено Гамильтонов циклов: " << allPaths.size() << endl;

        // Перебираем все найденные циклы, выводим их и считаем их веса
        int minWeight = INT_MAX;
        vector<int> shortestCycle;

        for (const auto& cycle : allPaths) {
            // Выводим цикл
            for (int v : cycle) {
                cout << v + 1 << " "; // Вершины выводятся с 1, а не с 0
            }
            cout << cycle[0] + 1 << endl; // Возвращаемся в начало
            int weight = calculateCycleWeight(G, cycle); // Рассчитываем вес этого цикла
            cout << "Вес цикла: " << weight << endl;

            // Находим минимальный вес
            if (weight < minWeight) {
                minWeight = weight;
                shortestCycle = cycle;
            }
        }

        // Выводим цикл с минимальным весом
        cout << "\nСамый короткий Гамильтонов цикл (с минимальным весом " << minWeight << "): ";
        for (int v : shortestCycle) {
            cout << v + 1 << " "; // Вершины выводятся с 1, а не с 0
        }
        cout << shortestCycle[0] + 1 << endl; // Возвращаемся в начало
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
    colorInf.setString(L"Ребра и их вес");
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
    //42 для 1 вершины - 4 пути
    //для ручной проверки
    srand(42);
    setlocale(LC_ALL, "Russian");

    const int nG = 7;
    int** G = createG(nG);
    printG(G, nG);

    int start;
    cout << "Введите номер стартовой вершины (1 - " << nG << "): ";
    cin >> start;
    start -= 1; // Преобразуем в индекс (с 0, а не с 1)

    // Проверяем правильность ввода
    if (start < 0 || start >= nG) {
        cout << "Неверный номер вершины!" << endl;
        return -1;
    }

    findAllHamiltonianCycles(G, nG, start);

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