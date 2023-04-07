#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

const double PI = atan(1.0) * 4;

sf::Color backgroundColor = sf::Color(60, 179, 113);
sf::Color drawingColor = sf::Color::Black;
sf::Color sinColor = sf::Color::Red;
sf::Color cosColor = sf::Color::Blue;
sf::Color pointColor = sf::Color::White;

int WIDTH = 1280;
int HEIGHT = 720;
float unitCircleRadius = HEIGHT / 4.0f;
float unitCircleBorderThickness = 2.0f;
float trigCircleRadius = unitCircleBorderThickness * 5.0f;
float pointCircleRadius = unitCircleBorderThickness * 2.5f;
float angle = 2 * PI;
bool running = false;
float graphAxisLength = HEIGHT * 0.3f;
sf::Time elapsedTime = sf::milliseconds(0);
sf::Time timeOfLastFrame = sf::milliseconds(0);

std::pair<float, float> unitCirclePosition(WIDTH * .1f, HEIGHT / 2.0f - unitCircleRadius);
std::pair<float, float> sinCirclePosition;
std::pair<float, float> cosCirclePosition;
std::pair<float, float> pointCirclePosition;
std::pair<float, float> sinGraphPosition(WIDTH * .55f, (HEIGHT / 10.0f) * 1.75);
std::pair<float, float> cosGraphPosition(WIDTH * .55f, (HEIGHT / 10.0f) * 5.25);
std::pair<float, float> sinGraphPointCirclePosition(WIDTH * .55f, std::sin(angle) * graphAxisLength);
std::pair<float, float> cosGraphPointCirclePosition(WIDTH * .55f, std::cos(angle) * graphAxisLength);

sf::Clock deltaClock;

float deltaTime() {
    return elapsedTime.asSeconds() * .00005;
}

std::pair<float, float> calculateSinCirclePosition() {
    return std::pair<float,float>(
        unitCirclePosition.first + unitCircleRadius - trigCircleRadius,
        unitCirclePosition.second + unitCircleRadius + (std::sin(angle) * unitCircleRadius) - trigCircleRadius
        );
}

std::pair<float, float> calculateCosCirclePosition() {
    return std::pair<float, float>(
        unitCirclePosition.first + unitCircleRadius + (std::cos(angle) * unitCircleRadius) - trigCircleRadius,
        unitCirclePosition.second + unitCircleRadius - trigCircleRadius
        );
}

std::pair<float, float> calculatePointCirclePosition() {
    return std::pair<float, float>(
        unitCirclePosition.first + unitCircleRadius + (std::cos(angle) * unitCircleRadius) - pointCircleRadius,
        unitCirclePosition.second + unitCircleRadius + (std::sin(angle) * unitCircleRadius) - pointCircleRadius
        );
}

std::pair<float, float> calculateSinGraphPointCirclePosition() {
    return std::pair<float, float>(
        sinGraphPointCirclePosition.first >= sinGraphPosition.first + (graphAxisLength * 2) * .75f ? sinGraphPointCirclePosition.first : sinGraphPointCirclePosition.first + deltaTime() * graphAxisLength / 2,
        sinGraphPosition.second + graphAxisLength/2 + (std::sin(angle) * graphAxisLength / 2) - pointCircleRadius
        );
}
std::pair<float, float> calculateCosGraphPointCirclePosition() {
    return std::pair<float, float>(
        cosGraphPointCirclePosition.first >= cosGraphPosition.first + (graphAxisLength * 2) * .75f ? cosGraphPointCirclePosition.first : cosGraphPointCirclePosition.first + deltaTime() * graphAxisLength / 2,
        cosGraphPosition.second + graphAxisLength / 2 - (std::cos(angle) * graphAxisLength / 2) - pointCircleRadius
        );
}



int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Sine and Cosine");

    sf::CircleShape unitCircle(unitCircleRadius);
    unitCircle.setFillColor(backgroundColor);
    unitCircle.setOutlineThickness(unitCircleBorderThickness);
    unitCircle.setOutlineColor(drawingColor);
    unitCircle.setPosition(unitCirclePosition.first, unitCirclePosition.second);

    sf::CircleShape sinCircle(trigCircleRadius);
    sinCircle.setFillColor(sinColor);

    sf::CircleShape cosCircle(trigCircleRadius);
    cosCircle.setFillColor(cosColor);

    sf::CircleShape pointCircle(pointCircleRadius);
    pointCircle.setFillColor(pointColor);

    sf::Vertex sinLine[2];
    sf::Vertex cosLine[2];

    sf::Vertex sinGraphHorizontalAxis[2] =
    {
        sf::Vector2f(sinGraphPosition.first, sinGraphPosition.second + graphAxisLength / 2),
        sf::Vector2f(sinGraphPosition.first + graphAxisLength * 2, sinGraphPosition.second + graphAxisLength / 2)
    };
    sf::Vertex cosGraphHorizontalAxis[2] =
    {
        sf::Vector2f(cosGraphPosition.first, cosGraphPosition.second + graphAxisLength / 2),
        sf::Vector2f(cosGraphPosition.first + graphAxisLength * 2, cosGraphPosition.second + graphAxisLength / 2)
    };

    sinGraphHorizontalAxis[0].color = sf::Color::Black;
    sinGraphHorizontalAxis[1].color = sf::Color::Black;
    cosGraphHorizontalAxis[0].color = sf::Color::Black;
    cosGraphHorizontalAxis[1].color = sf::Color::Black;

    std::vector<sf::Vertex> sinCurve;
    std::vector<sf::Vertex> cosCurve;

    // for the graphs and their points/curves... start the point at graph origin, move it forward until
    // about 3/4 of horizontal axis length, then just move the curve, with the point only moving up and down.
    sf::CircleShape sinGraphPointCircle(pointCircleRadius);
    sf::CircleShape cosGraphPointCircle(pointCircleRadius);
    sinGraphPointCircle.setFillColor(pointColor);
    cosGraphPointCircle.setFillColor(pointColor);

    sinCirclePosition = calculateSinCirclePosition();
    cosCirclePosition = calculateCosCirclePosition();
    pointCirclePosition = calculatePointCirclePosition();
    sinGraphPointCirclePosition = calculateSinGraphPointCirclePosition();
    cosGraphPointCirclePosition = calculateCosGraphPointCirclePosition();
    sinCircle.setPosition(sinCirclePosition.first, sinCirclePosition.second);
    cosCircle.setPosition(cosCirclePosition.first, cosCirclePosition.second);
    pointCircle.setPosition(pointCirclePosition.first, pointCirclePosition.second);
    sinGraphPointCircle.setPosition(sinGraphPointCirclePosition.first, sinGraphPointCirclePosition.second);
    cosGraphPointCircle.setPosition(cosGraphPointCirclePosition.first, cosGraphPointCirclePosition.second);

    sf::Transform curveTransform;

    bool firstFrame = true;
    deltaClock.restart();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    running = !running;
                }
                if (event.key.code == sf::Keyboard::BackSpace) {
                    angle = 0;
                    // updating circle positions
                    sinCirclePosition = calculateSinCirclePosition();
                    cosCirclePosition = calculateCosCirclePosition();
                    pointCirclePosition = calculatePointCirclePosition();
                    sinGraphPointCirclePosition = calculateSinGraphPointCirclePosition();
                    cosGraphPointCirclePosition = calculateCosGraphPointCirclePosition();
                    sinCircle.setPosition(sinCirclePosition.first, sinCirclePosition.second);
                    cosCircle.setPosition(cosCirclePosition.first, cosCirclePosition.second);
                    pointCircle.setPosition(pointCirclePosition.first, pointCirclePosition.second);
                    sinGraphPointCircle.setPosition(sinGraphPointCirclePosition.first, sinGraphPointCirclePosition.second);
                    cosGraphPointCircle.setPosition(cosGraphPointCirclePosition.first, cosGraphPointCirclePosition.second);

                    // updating line positions
                    sinLine[0] = sf::Vertex(sf::Vector2f(sinCircle.getPosition().x + trigCircleRadius, sinCircle.getPosition().y + trigCircleRadius));
                    sinLine[1] = sf::Vertex(sf::Vector2f(pointCircle.getPosition().x + pointCircleRadius, pointCircle.getPosition().y + pointCircleRadius));
                    sinLine[0].color = sinColor;
                    sinLine[1].color = sinColor;

                    cosLine[0] = sf::Vertex(sf::Vector2f(cosCircle.getPosition().x + trigCircleRadius, cosCircle.getPosition().y + trigCircleRadius));
                    cosLine[1] = sf::Vertex(sf::Vector2f(pointCircle.getPosition().x + pointCircleRadius, pointCircle.getPosition().y + pointCircleRadius));
                    cosLine[0].color = cosColor;
                    cosLine[1].color = cosColor;
                }
            }
        }

        if (running) {
            if (firstFrame) {
                timeOfLastFrame = deltaClock.restart();
                firstFrame = false;
            } else {
                elapsedTime = timeOfLastFrame - deltaClock.restart();
            }
            // update angle
            angle -= deltaTime();
            if (angle < 0) angle = 2 * PI;

            // updating circle positions
            sinCirclePosition = calculateSinCirclePosition();
            cosCirclePosition = calculateCosCirclePosition();
            pointCirclePosition = calculatePointCirclePosition();
            sinGraphPointCirclePosition = calculateSinGraphPointCirclePosition();
            cosGraphPointCirclePosition = calculateCosGraphPointCirclePosition();
            sinCircle.setPosition(sinCirclePosition.first, sinCirclePosition.second);
            cosCircle.setPosition(cosCirclePosition.first, cosCirclePosition.second);
            pointCircle.setPosition(pointCirclePosition.first, pointCirclePosition.second);
            sinGraphPointCircle.setPosition(sinGraphPointCirclePosition.first, sinGraphPointCirclePosition.second);
            cosGraphPointCircle.setPosition(cosGraphPointCirclePosition.first, cosGraphPointCirclePosition.second);

            // updating curves
            if (cosGraphPointCirclePosition.first < cosGraphPosition.first + (graphAxisLength * 2) * .75f) {
                sinCurve.push_back(sf::Vector2f(sinGraphPointCirclePosition.first + pointCircleRadius, sinGraphPointCirclePosition.second + pointCircleRadius));
                sinCurve[sinCurve.size() - 1].color = sinColor;
                cosCurve.push_back(sf::Vector2f(cosGraphPointCirclePosition.first + pointCircleRadius, cosGraphPointCirclePosition.second + pointCircleRadius));
                cosCurve[sinCurve.size() - 1].color = cosColor;
            } else {
                sinCurve.push_back(curveTransform.getInverse().transformPoint(sf::Vector2f(sinGraphPointCirclePosition.first + pointCircleRadius, sinGraphPointCirclePosition.second + pointCircleRadius)));
                sinCurve[sinCurve.size() - 1].color = sinColor;
                cosCurve.push_back(curveTransform.getInverse().transformPoint(sf::Vector2f(cosGraphPointCirclePosition.first + pointCircleRadius, cosGraphPointCirclePosition.second + pointCircleRadius)));
                cosCurve[sinCurve.size() - 1].color = cosColor;
                sinCurve.erase(sinCurve.begin());
                cosCurve.erase(cosCurve.begin());
                curveTransform.translate(sf::Vector2f(-deltaTime() * graphAxisLength / 2, 0));
            }

            // updating line positions
            sinLine[0] = sf::Vertex(sf::Vector2f(sinCircle.getPosition().x + trigCircleRadius, sinCircle.getPosition().y + trigCircleRadius));
            sinLine[1] = sf::Vertex(sf::Vector2f(pointCircle.getPosition().x + pointCircleRadius, pointCircle.getPosition().y + pointCircleRadius));
            sinLine[0].color = sinColor;
            sinLine[1].color = sinColor;

            cosLine[0] = sf::Vertex(sf::Vector2f(cosCircle.getPosition().x + trigCircleRadius, cosCircle.getPosition().y + trigCircleRadius));
            cosLine[1] = sf::Vertex(sf::Vector2f(pointCircle.getPosition().x + pointCircleRadius, pointCircle.getPosition().y + pointCircleRadius));
            cosLine[0].color = cosColor;
            cosLine[1].color = cosColor;
        }


        // updating display
        window.clear(backgroundColor);
        window.draw(unitCircle);
        window.draw(sinLine, 2, sf::Lines);
        window.draw(cosLine, 2, sf::Lines);
        window.draw(sinCircle);
        window.draw(cosCircle);
        window.draw(pointCircle);
        window.draw(sinGraphHorizontalAxis, 2, sf::Lines);
        window.draw(cosGraphHorizontalAxis, 2, sf::Lines);

        
        

        if (sinCurve.size() > 1) {
            window.draw(&sinCurve[0], sinCurve.size(), sf::LinesStrip, curveTransform);
        }
        if (cosCurve.size() > 1) {
            window.draw(&cosCurve[0], cosCurve.size(), sf::LinesStrip, curveTransform);
        }

        

        window.draw(sinGraphPointCircle);
        window.draw(cosGraphPointCircle);
        window.display();
    }

    return 0;
}