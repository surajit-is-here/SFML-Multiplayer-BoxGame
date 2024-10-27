#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"
#include <vector>
#include <limits.h>
#include <unistd.h>
#include <chrono>

using namespace std;

void displayScores(const vector<int> &v, const vector<string> &names)
{
    // Create a new SFML window for displaying scores

    sf::RenderWindow scoreWindow(sf::VideoMode(400, 300), "Player Scores");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("/home/surajit/Documents/game/fonts/Dosis-Light.ttf"))
    {
        cout << "ERROR::INITFONTS::Failed to load font!" << "\n";
        return;
    }

    vector<sf::Text> scoreTexts;
    for (size_t i = 0; i < v.size(); ++i)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(names[i] + ": " + std::to_string(v[i]));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(10.f, 30.f * i);
        scoreTexts.push_back(text);
    }

    int index = 0, max = INT_MIN, flag = 0;
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] > max)
        {
            max = v[i];
            index = i;
        }
        else if (v[i] == max)
        {
            flag = 1; // Indicates a draw
        }
    }

    sf::Text winnerText;
    winnerText.setFont(font);
    if (flag == 0)
    {
        winnerText.setString("Winner: " + names[index]);
    }
    else
    {
        winnerText.setString("DRAW");
    }
    winnerText.setCharacterSize(24);
    winnerText.setFillColor(sf::Color::Green);
    winnerText.setPosition(10.f, 30.f * v.size() + 10.f);

    while (scoreWindow.isOpen())
    {
        sf::Event event;
        while (scoreWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                scoreWindow.close();
        }

        // Clear window
        scoreWindow.clear(sf::Color::Black);

        for (const auto &text : scoreTexts)
        {
            scoreWindow.draw(text);
        }

        scoreWindow.draw(winnerText);

        // Display the scores
        scoreWindow.display();
    }
}

int main()
{
    std::srand(static_cast<unsigned>(time(NULL)));

    vector<int> healthResults;
    int numPlayers;
    cout << "Enter number of players: \t";
    cin >> numPlayers;
    vector<string> names;
    string s;

    // Input player names
    cout << "Enter " << numPlayers << " player names: " << endl;
    for (int i = 0; i < numPlayers; i++)
    {
        cin >> s;
        names.push_back(s);
    }

    int difficulty = 2; // Default is medium
    cout << "Choose difficulty level:\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
    cout << "Enter your choice: ";
    cin >> difficulty;

    for (int i = 1; i <= numPlayers; i++)
    {
        cout << names[i - 1] << "'s turn starting in: " << endl;
        int count = 3;
        while (count > 0)
        {
            cout << count<<endl;
            sleep(1);
            count--;
        }

        Game game(difficulty);

        while (game.running() && !game.getEndGame())
        {
            game.update();

            game.render();
        }

        // Store the player's health at the end of their game
        healthResults.push_back(game.get_points());
    }

    displayScores(healthResults, names);
    return 0;
}
