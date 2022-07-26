#include "PieceType.h"

std::map<PieceType::Coordinates, PieceType*> PieceType::s_positionInfo{};
std::pair<bool, TeamColour> PieceType::s_checkedKing(false, TeamColour::BLACK);
std::vector<PieceType::Coordinates> PieceType::s_dangerousPieces(0);

TeamColour PieceType::getColour() const
{
	return m_teamColour;
}

const char* PieceType::getDirectory()const
{
	return m_textureDirectory;
}

const char* PieceType::getName() const
{
	return m_name;
}

bool PieceType::isFirstTime() const
{
	return m_movesFirstTime;
}

void PieceType::setFirstTime(bool b)
{
	m_movesFirstTime = b;
}

void PieceType::checkTheKing()
{
	PieceType::Coordinates kingPos = [this]() // finds the opponents's king position on the board
	{
		for (auto&& a : this->s_positionInfo)
			if (a.second->getName() == "King" && a.second->getColour() != m_teamColour)
				return a.first;
	}();

	// looks through its moves and if one of them is at opponent's king square then it checks the king
	if (!s_checkedKing.first || s_checkedKing.second != m_teamColour)
	{
		for (auto&& move : m_possibleMoves)
			if (move == kingPos)
			{
				s_checkedKing.first = true;
				s_checkedKing.second = s_positionInfo.at(kingPos)->getColour();
				s_dangerousPieces.push_back(m_coordinates);
			}
	}
}

void PieceType::tryToPreventCheck()
{
	// if the king of this piece's team is checked
	if(s_checkedKing.first && s_checkedKing.second == m_teamColour && m_name != "King")
	{
		// if it's a double check then erases all moves, else tries preventing the check
		(s_dangerousPieces.size() > 1) ? m_possibleMoves.clear()
			: s_positionInfo.at(s_dangerousPieces[0])->eraseDangerousCells(m_possibleMoves, findMyKingPosition(), false);		
	}
}

void PieceType::checkIfIsPinned()
{
	// to check if the piece is pinned it deletes itself from the common map (positionInfo), lets the other pieces recalculate their moves according to new condotions,
	// and at last if the pieces can check the king, it gets pinned and removes all the illegal moves. After that it puts itself back into the common map.
	// The moves are eventually updated in the end of the turn in the Piece::update()

	if (!m_possibleMoves.empty())
	{
		PieceType::Coordinates kingPos = findMyKingPosition();

		// no need to emplace the piece back because it's done in update()
		s_positionInfo.erase(m_coordinates);

		for (auto&& positionInfo : s_positionInfo) // looks through the pieces
			if (positionInfo.second->getColour() != m_teamColour) // finds the opposite team's
			{
				positionInfo.second->calculatePossibleMoves(); // calculates the new set of moves without our concrete piece

				if (kingPos != m_coordinates) // not a king
				{
					// if the king is checked these operations are useless, so it quits
					if (!s_checkedKing.first)
					{
						// checks if king is reachable without our piece
						for (auto&& enemyMove : positionInfo.second->m_possibleMoves)
							if (enemyMove == kingPos)  
								return positionInfo.second->eraseDangerousCells(m_possibleMoves, kingPos, false);
					}
					else
						break;
				}
				else // a king (unlike the others) needs to look through all the opponent's pieces (regardless of being checked)
				{
					positionInfo.second->eraseDangerousCells(m_possibleMoves, m_coordinates, true);
				}
			}

		// removes all the moves containing pieces of the same colour.
		// I put it here because eraseDangerousCells() needs these moves to be present
		// in order not to let the king attack protected enemy piececs.
		std::erase_if(m_possibleMoves, [this](const auto& move) {return (this->s_positionInfo.contains(move) && this->s_positionInfo.at(move)->getColour() == this->m_teamColour); });
	}	
}

PieceType::Coordinates PieceType::findMyKingPosition()
{
	for (auto&& a : s_positionInfo)
		if (a.second->getName() == "King" && a.second->getColour() == m_teamColour)
				return a.first;
}

void PieceType::Coordinates::convertToBoard(sf::Vector2f position)
{
	x = static_cast<int>(position.x / SQUARE_WIDTH);
	y = 7 - static_cast<int>(position.y / SQUARE_HEIGHT);
}

sf::Vector2f PieceType::Coordinates::convertToPosition(int x, int y)
{
	return { x * SQUARE_WIDTH, (7 - y) * SQUARE_HEIGHT };
}

bool PieceType::Coordinates::operator<(const Coordinates c) const
{
	return (y < c.y || (y == c.y && x < c.x));
}

bool PieceType::Coordinates::operator==(const Coordinates c) const
{
	return (x == c.x && y == c.y);
}



////////PAWN////////////

Pawn::Pawn(TeamColour tc)
{
	if(tc == TeamColour::WHITE)
		m_textureDirectory = "Images/Pieces/wP.png";
	else if (tc == TeamColour::BLACK)
		m_textureDirectory = "Images/Pieces/bP.png";

	m_teamColour = tc;
	m_name = "Pawn";
}

void Pawn::calculatePossibleMoves()
{
	m_possibleMoves.clear();

	int ahead = m_coordinates.y + 2 * (int)m_teamColour - 1; // y ahead of the pawn

	auto addMoves = [this](int x, int y)
	{
		// only if there are pieces of its opponent's colour
		if (s_positionInfo.contains({ x, y }) && s_positionInfo.at({ x, y })->getColour() != this->getColour()) 
			this->m_possibleMoves.push_back({ x, y });
	};

	if (ahead >= 0 && ahead <= 7)
	{
		if (!s_positionInfo.contains({ m_coordinates.x, ahead })) // checks if there are pieces straight ahead of it
		{
			m_possibleMoves.push_back({ m_coordinates.x, ahead });

			// if that's the first move it can walk two squares ahead (if the square is empty per se)
			if (m_movesFirstTime && !s_positionInfo.contains({ m_coordinates.x, ahead + 2 * (int)m_teamColour - 1 })) 
				m_possibleMoves.push_back({ m_coordinates.x, ahead + 2 * (int)m_teamColour - 1 });
		}
	
		addMoves(m_coordinates.x - 1, ahead); // move on the left
		addMoves(m_coordinates.x + 1, ahead); // move on the right
	}
}

void Pawn::eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)
{
	// the method is called when the king is checked by this pawn 
	std::erase_if(possibleMoves, [this, isKing](const auto& item) {
		auto const& [x, y] = item;
		// if it's the king, it removes only the moves where the pawn attacks; if it's not the king, it removes everything except for the position of the pawn
		return ((std::abs(x - this->m_coordinates.x) == 1 && y == (this->m_coordinates.y + 2 * (int)this->m_teamColour - 1) && isKing) || item != this->m_coordinates && !isKing);
	});
}



//////////ROOK/////////////////

Rook::Rook(TeamColour tc)
{
	if (tc == TeamColour::WHITE)
		m_textureDirectory = "Images/Pieces/wR.png";
	else if (tc == TeamColour::BLACK)
		m_textureDirectory = "Images/Pieces/bR.png";

	m_teamColour = tc;
	m_name = "Rook";
}

void Rook::calculatePossibleMoves()
{
	m_possibleMoves.clear();

	auto addMoves = [this](int x, int y)
	{
		this->m_possibleMoves.push_back({ x, y }); // adds regardless of the colour (important when it comes to protected pieces, so that the king could walk legally)

		return !(s_positionInfo.contains({ x, y })); // if it stumbles upon a piece
	};
	
	for (int i = m_coordinates.x - 1; i >= 0 && addMoves(i, m_coordinates.y); --i){}
	for (int i = m_coordinates.x + 1; i <= 7 && addMoves(i, m_coordinates.y); ++i){}
	for (int i = m_coordinates.y - 1; i >= 0 && addMoves(m_coordinates.x, i); --i){}
	for (int i = m_coordinates.y + 1; i <= 7 && addMoves(m_coordinates.x, i); ++i){}
}

void Rook::eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)
{	 // a piece is pinned or tries to prevent a check
	if (!isKing)
	{
		std::erase_if(possibleMoves, [this, kingPos](const auto& item) {
			auto const& [xMove, yMove] = item; 
			// does big-brain computations
			return !((xMove == kingPos.x && this->m_coordinates.x == kingPos.x && (yMove < kingPos.y && yMove >= this->m_coordinates.y || yMove > kingPos.y && yMove <= this->m_coordinates.y))
				  || (yMove == kingPos.y && this->m_coordinates.y == kingPos.y && (xMove < kingPos.x && xMove >= this->m_coordinates.x || xMove > kingPos.x && xMove <= this->m_coordinates.x)));
		});
	} // the king removes illegal cells (both when checked and not) 
	else if(std::abs(kingPos.x - m_coordinates.x) <= 1 || std::abs(kingPos.y - m_coordinates.y) <= 1)
	{
		// removes intercepting moves
		std::erase_if(possibleMoves, [this](const auto& item) {
			for (auto&& move : this->m_possibleMoves)
				if (item == move)
					return true;

			return false;
		});
	}
}



////////KNIGHT////////////

Knight::Knight(TeamColour tc)
{
	if (tc == TeamColour::WHITE)
		m_textureDirectory = "Images/Pieces/wN.png";
	else if (tc == TeamColour::BLACK)
		m_textureDirectory = "Images/Pieces/bN.png";

	m_teamColour = tc;
	m_name = "Knight";
}
void Knight::calculatePossibleMoves()
{
	m_possibleMoves.clear();

	auto addMoves = [this](int x, int y)
	{
		// adds a move only if the square is empty or there's an opponent's piece; and if it's not out of the bounds
		if (x >= 0 && y >= 0 && x <= 7 && y <= 7)
			this->m_possibleMoves.push_back({ x, y });
	};

	addMoves(m_coordinates.x - 2, m_coordinates.y + 1);
	addMoves(m_coordinates.x - 2, m_coordinates.y - 1);
	addMoves(m_coordinates.x - 1, m_coordinates.y + 2);
	addMoves(m_coordinates.x - 1, m_coordinates.y - 2);
	addMoves(m_coordinates.x + 1, m_coordinates.y + 2);
	addMoves(m_coordinates.x + 1, m_coordinates.y - 2);
	addMoves(m_coordinates.x + 2, m_coordinates.y + 1);
	addMoves(m_coordinates.x + 2, m_coordinates.y - 1);
}

void Knight::eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)
{
	// the only condition when the knight can check the king/attack nearby squares
	if (std::abs(kingPos.x - m_coordinates.x) <= 3 && std::abs(kingPos.y - m_coordinates.y) <= 3)
	{
		std::erase_if(possibleMoves, [this, isKing](const auto& item) {
			if(isKing) // removes the intercepting squares (including the protected pieces) for the king
				for (auto&& move : this->m_possibleMoves)
					if (item == move)
						return true;

			//if its not the king it just leaves its own position
			return (!isKing && item != this->m_coordinates);
		});
	}
}



/////////BISHOP//////////////

Bishop::Bishop(TeamColour tc)
{
	if (tc == TeamColour::WHITE)
		m_textureDirectory = "Images/Pieces/wB.png";
	else if (tc == TeamColour::BLACK)
		m_textureDirectory = "Images/Pieces/bB.png";

	m_teamColour = tc;
	m_name = "Bishop";
}

void Bishop::calculatePossibleMoves()
{
	m_possibleMoves.clear();

	auto addMoves = [this](int x, int y)
	{
		this->m_possibleMoves.push_back({ x, y });// adds regardless of the colour (important when it comes to protected pieces, so that the king could walk legally)

		return !(s_positionInfo.contains({ x, y })); // if it stumbles upon a piece
	};

	for (int i = m_coordinates.x - 1, j = m_coordinates.y + 1; i >= 0 && j <= 7 && addMoves(i, j); i--, j++){}
	for (int i = m_coordinates.x + 1, j = m_coordinates.y + 1; i <= 7 && j <= 7 && addMoves(i, j); i++, j++){}
	for (int i = m_coordinates.x - 1, j = m_coordinates.y - 1; i >= 0 && j >= 0 && addMoves(i, j); i--, j--){}
	for (int i = m_coordinates.x + 1, j = m_coordinates.y - 1; i <= 7 && j >= 0 && addMoves(i, j); i++, j--){}
}

void Bishop::eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)
{
	if (!isKing) // if a piece is pinned or tries to prevent a check
	{
		std::erase_if(possibleMoves, [this, kingPos](const auto& move) {
			auto const& [xMove, yMove] = move;
			// ultra big-brain computations
			return !(((this->m_coordinates.x + this->m_coordinates.y == xMove + yMove) && (kingPos.x + kingPos.y == xMove + yMove)
				|| (this->m_coordinates.x - this->m_coordinates.y == xMove - yMove) && (kingPos.x - kingPos.y == xMove - yMove))
				&& (xMove >= this->m_coordinates.x && xMove < kingPos.x || xMove > kingPos.x && xMove <= this->m_coordinates.x));
		});
	}
	else // if the king is checked (or not) and deletes illegal moves
	{
		std::erase_if(possibleMoves, [this](const auto& item) {
			for (auto&& move : this->m_possibleMoves)
				if (item == move)
					return true;

			return false;
		});
	}
}



/////////QUEEN///////////

Queen::Queen(TeamColour tc)
{
	m_possibleMoves.clear();

	if (tc == TeamColour::WHITE)
		m_textureDirectory = "Images/Pieces/wQ.png";
	else if (tc == TeamColour::BLACK)
		m_textureDirectory = "Images/Pieces/bQ.png";

	m_teamColour = tc;
	m_name = "Queen";
}
void Queen::calculatePossibleMoves()
{
	m_possibleMoves.clear();

	auto addMoves = [this](int x, int y)
	{
		this->m_possibleMoves.push_back({ x, y }); // adds regardless of the colour (important when it comes to protected pieces, so that the king could walk legally)

		return !(s_positionInfo.contains({ x, y }));
	};

	for (int i = m_coordinates.x - 1, j = m_coordinates.y + 1; i >= 0 && j <= 7 && addMoves(i, j); i--, j++){}
	for (int i = m_coordinates.x + 1, j = m_coordinates.y + 1; i <= 7 && j <= 7 && addMoves(i, j); i++, j++){}
	for (int i = m_coordinates.x - 1, j = m_coordinates.y - 1; i >= 0 && j >= 0 && addMoves(i, j); i--, j--){}
	for (int i = m_coordinates.x + 1, j = m_coordinates.y - 1; i <= 7 && j >= 0 && addMoves(i, j); i++, j--){}
	for (int i = m_coordinates.x - 1; i >= 0 && addMoves(i, m_coordinates.y); --i){}
	for (int i = m_coordinates.x + 1; i <= 7 && addMoves(i, m_coordinates.y); ++i){}
	for (int i = m_coordinates.y - 1; i >= 0 && addMoves(m_coordinates.x, i); --i){}
	for (int i = m_coordinates.y + 1; i <= 7 && addMoves(m_coordinates.x, i); ++i){}
}

void Queen::eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)
{
	if (!isKing) // if a piece is pinned or tries to prevent a check
	{
		std::erase_if(possibleMoves, [this, kingPos](const auto& move) {
			auto const& [xMove, yMove] = move;
			// a combination of bishop's and rook's big-brain computations
			if (kingPos.x == m_coordinates.x || kingPos.y == m_coordinates.y)
				return !((xMove == kingPos.x && this->m_coordinates.x == kingPos.x && (yMove < kingPos.y&& yMove >= this->m_coordinates.y || yMove > kingPos.y && yMove <= this->m_coordinates.y))
					  || (yMove == kingPos.y && this->m_coordinates.y == kingPos.y && (xMove < kingPos.x&& xMove >= this->m_coordinates.x || xMove > kingPos.x && xMove <= this->m_coordinates.x)));
			else 
				return !(((this->m_coordinates.x + this->m_coordinates.y == xMove + yMove) && (kingPos.x + kingPos.y == xMove + yMove)
					|| (this->m_coordinates.x - this->m_coordinates.y == xMove - yMove) && (kingPos.x - kingPos.y == xMove - yMove))
					&& (xMove >= this->m_coordinates.x && xMove < kingPos.x || xMove > kingPos.x && xMove <= this->m_coordinates.x));
		});
	}
	else  // if the king is checked (or not) and deletes illegal moves
	{
		std::erase_if(possibleMoves, [this](const auto& item) {
			for (auto&& move : this->m_possibleMoves)
				if (item == move)
					return true;

			return false;
		});
	}
}



//////KING////////////

King::King(TeamColour tc)
{
	if (tc == TeamColour::WHITE)
		m_textureDirectory = "Images/Pieces/wK.png";
	else if (tc == TeamColour::BLACK)
		m_textureDirectory = "Images/Pieces/bk.png";

	m_teamColour = tc;
	m_name = "King";
}

void King::calculatePossibleMoves()
{
	m_possibleMoves.clear();

	auto addMoves = [this](int x, int y) {
		if ((!s_positionInfo.contains({ x, y }) || s_positionInfo.at({ x, y })->getColour() != this->getColour()) && x >= 0 && x <= 7 && y >= 0 && y <= 7)
			this->m_possibleMoves.push_back({ x, y });
	};

	for (int i = -1; i <= 1; ++i)
	{
		addMoves(m_coordinates.x + i, m_coordinates.y + 2 * (int)m_teamColour - 1); // cells ahead
		if (i) addMoves(m_coordinates.x + i, m_coordinates.y); // cells at its y position
		addMoves(m_coordinates.x + i, m_coordinates.y + 1 - 2 * (int)m_teamColour); // cells behind
	}
}

void King::eraseDangerousCells(std::vector<PieceType::Coordinates>& possibleMoves, const PieceType::Coordinates& kingPos, bool isKing)
{
	// needs to be executed only if two kings stand one next to another
	if (std::abs(kingPos.x - m_coordinates.x) <= 2 && std::abs(kingPos.y - m_coordinates.y) <= 2)
	{	
		std::erase_if(possibleMoves, [this](const auto& item) {
			auto const& [x, y] = item;
			return (std::abs(x - this->m_coordinates.x) <= 1 && std::abs(y - this->m_coordinates.y) <= 1); 
		});
	}
}
