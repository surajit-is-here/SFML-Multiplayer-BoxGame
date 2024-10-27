#include "game.h"
#include <iostream>
#include <vector>
using namespace std;

void Game::initVariables()
{
	this->window = nullptr;

	this->endGame = false;
	this->points = 0;
	this->health = 15;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
}

void Game::initWindow(int difficulty)
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Dominoes Game", sf::Style::Titlebar | sf::Style::Close);

	// Set frame rate limit based on difficulty
	switch (difficulty)
	{
	case 1: // Easy
		this->window->setFramerateLimit(30);
		break;
	case 2: // Medium
		this->window->setFramerateLimit(50);
		break;
	case 3: // Hard
		this->window->setFramerateLimit(70);
		break;
	default:
		this->window->setFramerateLimit(50); // Default to medium
		break;
	}
}

void Game::initFonts()
{
	if (this->font.loadFromFile("/home/surajit/Documents/game/fonts/Dosis-SemiBold.ttf"));
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);			 
	this->uiText.setFillColor(sf::Color::White); 
	this->uiText.setPosition(10.f, 10.f);	
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setFillColor(sf::Color::Cyan);
}

Game::Game(int difficulty)
{
	this->initVariables();
	this->initWindow(difficulty); // Pass difficulty to initWindow
	this->initFonts();
	this->initText();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

void Game::spawnEnemy()
{

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f);

	// Randomizing enemy type
	int type = rand() % 6;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(10.f, 10.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(85.f, 85.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	case 5:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	}

	this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions()
{

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << "\n"
	   << "Health: " << this->health << "\n";

	this->uiText.setString(ss.str()); 
}
int Game::get_points()
{
	return this->points;
}

void Game::updateEnemies()
{
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;

		this->enemies[i].move(0.f, 5.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			if (this->enemies[i].getFillColor() != sf::Color::Green)
			{
				this->health -= 1;
			}

			this->enemies.erase(this->enemies.begin() + i);
		}
	}

	// Check if mouse is clicked
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					// Gain points
					if (this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 10;
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 7;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points += 3;
					else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
						this->points += 1;
					else
						this->points -= 5;

					// Delete enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}
}

void Game::update()
{
	this->pollEvents();

	if (this->endGame == false)
	{
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

	if (this->health <= 0)
	{
		this->endGame = true;
	}
}

void Game::renderText(sf::RenderTarget &target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget &target)
{
	for (auto &e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::render()
{
	this->window->clear(); 

	// Draw game objects
	this->renderEnemies(*this->window); 

	this->renderText(*this->window);

	this->window->display();
}
