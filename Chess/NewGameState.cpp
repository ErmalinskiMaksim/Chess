#include "NewGameState.h"

NewGameState::NewGameState() : m_teamThatMoves(TeamColour::WHITE), m_isGameOver(false)
{
	initializeTeams();
}

void NewGameState::processInput(sf::Event& event, Window& window)
{
	if(!m_isGameOver)
		processTeamInput(event, window);
}

void NewGameState::update()
{
	if (!m_isGameOver)
		updateTeams();
}

void NewGameState::display(Window& window)
{
	displayBackground(window);
	displayTeams(window);
}

void NewGameState::initializeTeams()
{
	auto initialization = [](std::array<Piece, 16>& team, TeamColour colour)
	{
		team[0].setType(new Rook(colour));
		team[1].setType(new Knight(colour));
		team[2].setType(new Bishop(colour));
		team[3].setType(new Queen(colour));
		team[4].setType(new King(colour));
		team[5].setType(new Bishop(colour));
		team[6].setType(new Knight(colour));
		team[7].setType(new Rook(colour));

		for (int i = 0; i < 8; ++i)
		{
			team[i].setPosition({ i * SQUARE_WIDTH, (WINDOW_HEIGHT - SQUARE_HEIGHT) * (int)colour });
			team[i].setTexture(team[i].getType()->getDirectory());
		}

		for (int i = 8; i < 16; ++i)
		{
			team[i].setType(new Pawn(colour));
			team[i].setPosition({ (i - 8) * SQUARE_WIDTH, (int)colour * (WINDOW_HEIGHT - 3 * SQUARE_HEIGHT) + SQUARE_HEIGHT });
			team[i].setTexture(team[i].getType()->getDirectory());
		}
	};

	initialization(m_teamBlack, TeamColour::BLACK);
	initialization(m_teamWhite, TeamColour::WHITE);
}

void NewGameState::processTeamInput(sf::Event& event, Window& window)
{
	Piece::s_isTurnOver = false;
	auto noMoves = [](std::array<Piece, 16>& team)
	{
		for (auto&& a : team)
			if (!a.getType()->m_possibleMoves.empty() || !PieceType::s_checkedKing.first || PieceType::s_checkedKing.second != a.getType()->getColour())
				return false;

		return true;
	};


	if (m_teamThatMoves == TeamColour::BLACK)
	{
		for (auto&& a : m_teamBlack)
			a.processEvents(event, window);

		if(Piece::s_isTurnOver)
			m_teamThatMoves = TeamColour::WHITE;
		else if (noMoves(m_teamBlack))
		{
			m_isGameOver = true;
			std::cout << "WHITE WINS\n";
		}

	}
	else
	{
		for (auto&& a : m_teamWhite)
			a.processEvents(event, window);

		if(Piece::s_isTurnOver)
			m_teamThatMoves = TeamColour::BLACK;
		else if (noMoves(m_teamWhite))
		{
			m_isGameOver = true;
			std::cout << "BLACK WINS\n";
		}
	}
}

void NewGameState::updateTeams()
{
	for (auto&& a : m_teamBlack)
		a.update();

	for (auto&& a : m_teamWhite)
		a.update();
}

void NewGameState::displayTeams(Window& window)
{
	for (auto&& a : m_teamBlack)
		a.display(window);

	for (auto&& a : m_teamWhite)
		a.display(window);
}

void NewGameState::displayBackground(Window& window)
{
	m_background.display(window);
}