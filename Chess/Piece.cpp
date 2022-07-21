#include "Piece.h"

bool Piece::s_anyPiecesSelected = false;
bool Piece::s_isTurnOver = true;

// Sequence of calls:
// 0) adds itself into the common map (update)
// if there's an event (a piece is clicked):
// 1) selects a piece (processEvents)
// * does nothing wholesome (update)
// 2) draws available squares (display) 
// 3) moves (process events) if the selection is undone, no longer displays the available squares and gets back to 1)
// 4) checks if it's hiden and computes the TurIsOver part (update)
// 5) displays new position and no available squares (display)
// gets back to 1)

void Piece::processEvents(sf::Event& event, Window& window)
{
	if (event.type == sf::Event::MouseButtonPressed && !m_hiden)
	{
		sf::Vector2<float> v(sf::Mouse::getPosition(window.getWindow()));

		if (m_sprite.getGlobalBounds().contains(v) && !s_anyPiecesSelected) // selects a piece
		{
			if (!m_type->s_checkedKing.first && m_type->m_possibleMoves.empty())
					m_type->calculatePossibleMoves();

			m_type->checkIfIsPinned();

			if (!m_type->m_possibleMoves.empty()) // if there are moves left it gets selected
			{
				m_selected = true;
				s_anyPiecesSelected = true;

				for (auto&& a : m_type->m_possibleMoves) // fills in available moves
				{
					sf::RectangleShape cell({ SQUARE_WIDTH, SQUARE_HEIGHT });
					cell.setPosition(m_type->m_coordinates.convertToPosition({ a.x, a.y }));
					cell.setFillColor({ 200, 0, 128, 100 });
					m_availableCells.push_back(cell);
				}
			}
		}
		else if (m_selected && s_anyPiecesSelected) // moves
		{
			for (auto&& a : m_availableCells)
			{
				if (a.getGlobalBounds().contains(v)) // if it's an available square then moves to it
				{
					m_selected = false;
					s_anyPiecesSelected = false;
					s_isTurnOver = true; // finish the turn (happens only in this branch)

					m_type->s_dangerousPieces.clear();
					m_type->s_checkedKing.first = false;

					if(m_type->isFirstTime())
						m_type->setFirstTime(false);

					m_availableCells.clear(); // deletes all the suggested squares

					m_type->s_positionInfo.erase(m_type->m_coordinates); // erases its old position in the common map
					m_sprite.setPosition(a.getPosition()); // sets new coordinates
					m_type->m_coordinates.convertToBoard(m_sprite.getPosition()); // converts them into the borad coordinates

					if (m_type->s_positionInfo.contains(m_type->m_coordinates)) // if the position it's trying to move to is already occupied, it frees the square
						m_type->s_positionInfo.erase(m_type->m_coordinates);

					m_type->s_positionInfo.emplace(m_type->m_coordinates, m_type); // puts itself at the new position 

					break;
				}
				else if (m_sprite.getGlobalBounds().contains(v)) // if it's the piece itself then it undoes its selection
				{
					m_selected = false;
					s_anyPiecesSelected = false;

					m_availableCells.clear();

					break;
				}
			}
		}
	}
}

void Piece::update()
{
	if (!m_hiden)
	{
		m_type->m_coordinates.convertToBoard(m_sprite.getPosition()); // update coordinates

		if (!m_type->s_positionInfo.contains(m_type->m_coordinates)) // there's no one at its coordinates in the map, it inserts itself (needed for the first turn)
		{
			m_type->s_positionInfo.emplace(m_type->m_coordinates, m_type);
		}
		else if (m_type->getColour() != m_type->s_positionInfo.at(m_type->m_coordinates)->getColour()) 
		{
			// if it's missing from the map at its coordinates and its place is taken, it hides itself in order not to be processed further in the game 
			m_hiden = true;
			m_type->m_possibleMoves.clear(); // importatnt part for determining when it's a mate
			return;
		}

		if (s_isTurnOver) // recaclculate its moves according to the new board setup and check if it can reach the opponent's king
		{
			m_type->calculatePossibleMoves();
			m_type->checkTheKing();
		}

		m_type->checkIfMyKingIsChecked(); // checks if its king is safe or checked
	}
}

void Piece::display(Window& window)
{
	if (!m_hiden)
	{
		window.draw(m_sprite);

		for (auto&& a : m_availableCells)
			window.draw(a);
	}
}


PieceType* Piece::getType() const
{
	return m_type;
}

void Piece::setType(PieceType* type)
{
	m_type = type;
}