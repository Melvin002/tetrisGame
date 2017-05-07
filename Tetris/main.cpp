#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include "Piece.h"
#include <ctime>  
#include <stdlib.h> 

const int BOARD_TOP_OFFSIDE = -50;
const int BOARD_LEFT_OFFSIDE = 150;
const int BOARD_RIGHT_OFFSIDE = 150;
const int FPS_NUMBER = 60;
const int MULTIPLER = 300;
const int WIDTH = 1 * MULTIPLER + BOARD_LEFT_OFFSIDE + BOARD_RIGHT_OFFSIDE;
const int RIGHT_WINDOW_SIDE = 1 * MULTIPLER + BOARD_LEFT_OFFSIDE;
const int HEIGTH = 2.0333333 * MULTIPLER;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGTH = 22;
const int LOCK_DELAY = 120;
const int SHAPE_ARRAY_SIZE = 4;
const int POSITION_OF_PIECE_CODE = 100; // yxx / 100 = y
const int DAS_DELAY = 20; // DAS - delayed auto shift
const int MOVE_AUTOREPEAT_DELAY = 3; //time between each repeat
const int SQUARE_SIDE = 30;
const Point HOLD_POSITION(5,200);
const Point NEXT_POSITION(RIGHT_WINDOW_SIDE + 5, 200);

int board[BOARD_HEIGTH][BOARD_WIDTH] = { 0 };
Piece piece = Piece(Piece::empty);
Piece pieceInHold = Piece(Piece::empty);
sf::RectangleShape rectangle(sf::Vector2f(SQUARE_SIDE, SQUARE_SIDE));
sf::Font font;
int ghostOffset;
int lockDelayCounter;
int softDownDAS;
int leftDAS;
int rightDAS;
bool holdHappened = false;


enum Directions {
	Left, Right, Top, Bottom, DropDown, SoftDown
};

void processInput(sf::RenderWindow &window);
void update(sf::RenderWindow &window);
void render(sf::RenderWindow &window);
void move(Directions direction);
void rotate(Directions direction);
void checkIfLanded(sf::RenderWindow &window);
bool isSpaceOccupied(Directions direction, Point shape[], int distance = 1);
void clearLines();
void setPen(sf::RenderWindow &window, Piece::Pieces pieceType, int x, int y);
bool checkIfGameLost();

int main() {
	/*
	piece codes:
	o - 1,	i - 2,	s - 3,	z - 4
	l - 5,	j - 6,	t - 7

	XYYY
	X - type of piece code
	YYY - number of element

	0 - empty
	*/
	std::srand(unsigned(std::time(0)));

	
	if (!font.loadFromFile("arial.ttf")) {

	}

	sf::RenderWindow window;
	window.create(sf::VideoMode(WIDTH, HEIGTH), "Tetris!");
	window.setFramerateLimit(FPS_NUMBER);
	window.setKeyRepeatEnabled(false);

	sf::Clock clock;
	sf::Time programTime;

	piece = Piece(Piece::random);//piece is global for now

	while (window.isOpen()) {

		processInput(window);
		update(window);
		render(window);

	}

}
void processInput(sf::RenderWindow &window) {
	//sfml window event capture
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		//keyboard input
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::X)) {
			rotate(Right);
		}
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::Z)) {
			rotate(Left);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
			move(DropDown);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
			//pause in future
		}
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::C) && !holdHappened) {
			if (pieceInHold.pieceType == Piece::empty){
				pieceInHold = Piece(piece.pieceType);
				piece = Piece(Piece::random);
			}
			else {
				Piece swapTemp = Piece(piece.pieceType);
				piece = pieceInHold;
				piece.positionReset();
				pieceInHold = swapTemp;
			}
			holdHappened = true;
		}
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left) {
			leftDAS = 0;
		}
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right) {
			rightDAS = 0;
		}
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down) {
			softDownDAS = 0;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		if (leftDAS%DAS_DELAY == 0 && leftDAS <= DAS_DELAY) {
			move(Left);
		}
		else if (leftDAS%MOVE_AUTOREPEAT_DELAY == 0 && leftDAS > DAS_DELAY) {
			move(Left);
			leftDAS = DAS_DELAY;
		}
		leftDAS++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if (rightDAS%DAS_DELAY == 0 && rightDAS <= DAS_DELAY) {
			move(Right);
		}
		else if (rightDAS%MOVE_AUTOREPEAT_DELAY == 0 && rightDAS > DAS_DELAY) {
			move(Right);
			rightDAS = DAS_DELAY;
		}
		rightDAS++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		if (softDownDAS%DAS_DELAY == 0 && softDownDAS <= DAS_DELAY) {
			move(SoftDown);
		}
		else if (softDownDAS%MOVE_AUTOREPEAT_DELAY == 0 && softDownDAS > DAS_DELAY) {
			move(SoftDown);
			softDownDAS = DAS_DELAY;
		}
		softDownDAS++;
	}

}
int counter = 0;
void update(sf::RenderWindow &window) {
	//piece fall speed
	counter++;
	if (counter > 60) {//60 speed fall limiter
		move(SoftDown);
		counter = 0;
	}
	//find ghost bottom offset
	ghostOffset = 0;
	while (!isSpaceOccupied(Bottom, piece.shape, ghostOffset + 1)) {
		ghostOffset++;
	}
	if (isSpaceOccupied(Bottom, piece.shape))
		lockDelayCounter++;
	else
		lockDelayCounter = 0;
	checkIfLanded(window);



}
void render(sf::RenderWindow &window) {
	window.clear(sf::Color::Black);
	
	sf::RectangleShape outline(sf::Vector2f(BOARD_WIDTH * SQUARE_SIDE, BOARD_HEIGTH * SQUARE_SIDE));
	outline.setOutlineColor(sf::Color::White);
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(1);
	outline.setPosition(BOARD_LEFT_OFFSIDE,0);
	window.draw(outline);


	for (int i = BOARD_HEIGTH - 1; i >= 0; i--) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (i % 2 == 0 && j % 2 == 0 || i % 2 == 1 && j % 2 == 1) {
				rectangle.setFillColor(sf::Color(32, 32, 32, 50));
				rectangle.setPosition((j * SQUARE_SIDE) + BOARD_LEFT_OFFSIDE, (i * SQUARE_SIDE) + BOARD_TOP_OFFSIDE);
				window.draw(rectangle);
			}
			else {
				rectangle.setFillColor(sf::Color(128, 128, 128, 50));
				rectangle.setPosition((j * SQUARE_SIDE) + BOARD_LEFT_OFFSIDE, (i * SQUARE_SIDE) + BOARD_TOP_OFFSIDE);
				window.draw(rectangle);
			}
		}
	}
	//check all board and draws tetrominos
	for (int i = BOARD_HEIGTH - 1; i >= 0; i--) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if(board[i][j] != 0)
				setPen(window, static_cast<Piece::Pieces>((board[i][j] / POSITION_OF_PIECE_CODE) - 1), (j * SQUARE_SIDE) + BOARD_LEFT_OFFSIDE , (i * SQUARE_SIDE) + BOARD_TOP_OFFSIDE);//60 is two squares, 
		}
	}
	//render ghost
	for (int i = 0; i < 4; i++) {
		setPen(window, Piece::shadow, ((piece.shape[i].x + piece.position.x) * SQUARE_SIDE) + BOARD_LEFT_OFFSIDE, ((piece.shape[i].y + piece.position.y + ghostOffset) * SQUARE_SIDE) + BOARD_TOP_OFFSIDE);
	}
	//render moving piece
	for (int i = 0; i < 4; i++){
		setPen(window, piece.pieceType, ((piece.shape[i].x + piece.position.x) * SQUARE_SIDE) + BOARD_LEFT_OFFSIDE, ((piece.shape[i].y + piece.position.y) * SQUARE_SIDE) + BOARD_TOP_OFFSIDE);
	}
	//hold
	sf::Text text;
	text.setFont(font);
	text.setString("HOLD");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(HOLD_POSITION.x + 20,HOLD_POSITION.y - 50);
	window.draw(text);
	for (int i = 0; i < 4; i++) {
		setPen(window, pieceInHold.pieceType, (pieceInHold.shape[i].x * SQUARE_SIDE + HOLD_POSITION.x), (pieceInHold.shape[i].y * SQUARE_SIDE + HOLD_POSITION.y));
	}
	//next
	for (int k = 0, indexVector = piece.randomPermutationOfPieces.size() - 1; k < 3 && indexVector >= 0; k++, indexVector--) {
		Piece tempNextPiece = Piece(piece.randomPermutationOfPieces[indexVector]);
		for (int i = 0; i < 4; i++) {
			setPen(window, tempNextPiece.pieceType, (tempNextPiece.shape[i].x * SQUARE_SIDE + NEXT_POSITION.x), (tempNextPiece.shape[i].y * SQUARE_SIDE + NEXT_POSITION.y + 3 * SQUARE_SIDE* k));
		}
	}
	text.setString("NEXT");
	text.setPosition(NEXT_POSITION.x + 20, NEXT_POSITION.y - 50);
	window.draw(text);

	window.display();
}
void setPen(sf::RenderWindow &window, Piece::Pieces pieceType, int x, int y) {
	switch (pieceType) {
	case Piece::pieceO:
		rectangle.setFillColor(sf::Color::Yellow);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceI:
		rectangle.setFillColor(sf::Color::Cyan);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceS:
		rectangle.setFillColor(sf::Color::Green);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceZ:
		rectangle.setFillColor(sf::Color::Red);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceL:
		rectangle.setFillColor(sf::Color(255, 165, 0));
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceJ:
		rectangle.setFillColor(sf::Color::Blue);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::pieceT:
		rectangle.setFillColor(sf::Color::Magenta);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	case Piece::shadow:
		rectangle.setFillColor(sf::Color::White);
		rectangle.setPosition(x, y);
		window.draw(rectangle);
		break;
	}
}
void move(Directions direction){

	switch (direction) {
	case Left:
		if (!isSpaceOccupied(Left, piece.shape)) {
			piece.position.x--;
		}
		break;
	case Right:
		if (!isSpaceOccupied(Right, piece.shape)) {
			piece.position.x++;
		}
		break;
	case SoftDown:
		if (!isSpaceOccupied(Bottom, piece.shape)) {
			piece.position.y++;
		}
		break;
	case Top:
		if (!isSpaceOccupied(Top, piece.shape)) {
			piece.position.y--;
		}
		break;
	case DropDown:
		while (!isSpaceOccupied(Bottom, piece.shape)) {
			piece.position.y++;
		}
		lockDelayCounter = LOCK_DELAY;
		break;
	}
	
}
//function checks for other pieces blocking the way or end of the board BLOCKED = TRUE, EMPTY = FALSE
bool isSpaceOccupied(Directions direction, Point shape[], int distance) {
	switch (direction) {
	case Left:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].y + piece.position.y < BOARD_HEIGTH && shape[i].x + piece.position.x - distance >= 0 && shape[i].x + piece.position.x - distance < BOARD_WIDTH && board[shape[i].y + piece.position.y][shape[i].x + piece.position.x - distance] == 0))
				return true;
		}
		return false;
		break;
	case Right:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].y + piece.position.y < BOARD_HEIGTH && shape[i].x + piece.position.x + distance >= 0 && shape[i].x + piece.position.x + distance < BOARD_WIDTH && board[shape[i].y + piece.position.y][shape[i].x + piece.position.x + distance] == 0))
				return true;
		}
		return false;
		break;
	case Bottom:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].x + piece.position.x >= 0 && shape[i].x + piece.position.x < BOARD_WIDTH && shape[i].y + piece.position.y + distance < BOARD_HEIGTH && board[shape[i].y + piece.position.y + distance][shape[i].x + piece.position.x] == 0))
				return true;
		}
		return false;
		break;
	case Top:
		for (int i = 0; i < 4; i++) {
			if (!(shape[i].x + piece.position.x >= 0 && shape[i].x + piece.position.x < BOARD_WIDTH && shape[i].y + piece.position.y - distance >= 0 && shape[i].y + piece.position.y - distance < BOARD_HEIGTH && board[shape[i].y + piece.position.y - distance][shape[i].x + piece.position.x] == 0))
				return true;
		}
		return false;
		break;
	}
	return false;
}
void rotate(Directions direction) {

	//ghost for cheking if rotation is possible
	Point tempShape[4];
	bool isRotationPossible = true;
	bool isThereAnObstacle = false;
	switch (direction) {
	case Right:
		if (piece.pieceType != piece.pieceI && piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				tempShape[i].y = piece.shape[i].x;
				tempShape[i].x = 2 - piece.shape[i].y;
			}
		}
		else if(piece.pieceType != piece.pieceO){
			for (int i = 0; i < 4; i++) {
				tempShape[i].y = piece.shape[i].x;
				tempShape[i].x = 3 - piece.shape[i].y;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (tempShape[i].x + piece.position.x < 0 || tempShape[i].x + piece.position.x >= BOARD_WIDTH || board[tempShape[i].y + piece.position.y][tempShape[i].x + piece.position.x] != 0) {
				isRotationPossible = false;
			}
		}
		if (!isRotationPossible) {
			if (!isSpaceOccupied(Top, tempShape)) {
				piece.position.y--;
				isRotationPossible = true;
			}
			else if(!isSpaceOccupied(Right, tempShape)) {
				piece.position.x++;
				isRotationPossible = true;
			}
			else if(!isSpaceOccupied(Left, tempShape)){
				piece.position.x--;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Bottom, tempShape)) {
				piece.position.y++;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Top, tempShape, 2)) {
				piece.position.y -= 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Right, tempShape, 2)) {
				piece.position.x += 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Left, tempShape, 2)) {
				piece.position.x -= 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Bottom, tempShape, 2)) {
				piece.position.y += 2;
				isRotationPossible = true;
			}
		}
		if (isRotationPossible && piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				piece.shape[i].x = tempShape[i].x;
				piece.shape[i].y = tempShape[i].y;
			}
		}
		
		break;
	case Left:
		if (piece.pieceType != piece.pieceI && piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				tempShape[i].x = piece.shape[i].y;
				tempShape[i].y = 2 - piece.shape[i].x;
			}
		}
		else if (piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				tempShape[i].x = piece.shape[i].y;
				tempShape[i].y = 3 - piece.shape[i].x;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (tempShape[i].x + piece.position.x < 0 || tempShape[i].x + piece.position.x >= BOARD_WIDTH || board[tempShape[i].y + piece.position.y][tempShape[i].x + piece.position.x] != 0) {
				isRotationPossible = false;
			}
		}
		if (!isRotationPossible) {
			if (!isSpaceOccupied(Top, tempShape)) {
				piece.position.y--;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Right, tempShape)) {
				piece.position.x++;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Left, tempShape)) {
				piece.position.x--;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Bottom, tempShape)) {
				piece.position.y++;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Top, tempShape, 2)) {
				piece.position.y -= 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Right, tempShape, 2)) {
				piece.position.x += 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Left, tempShape, 2)) {
				piece.position.x -= 2;
				isRotationPossible = true;
			}
			else if (!isSpaceOccupied(Bottom, tempShape, 2)) {
				piece.position.y += 2;
				isRotationPossible = true;
			}
		}
		if (isRotationPossible && piece.pieceType != piece.pieceO) {
			for (int i = 0; i < 4; i++) {
				piece.shape[i].x = tempShape[i].x;
				piece.shape[i].y = tempShape[i].y;
			}
		}
		break;
	}
	lockDelayCounter = 0;
}
void checkIfLanded(sf::RenderWindow &window) {
	if (lockDelayCounter > LOCK_DELAY) {
		//write piece to board
		for (int i = 0; i < 4; i++) {
			board[piece.shape[i].y + piece.position.y][piece.shape[i].x + piece.position.x] = piece.pieceCode;
		}

		clearLines();
		if (checkIfGameLost())
			window.close();
		piece = Piece(Piece::random);
		if (isSpaceOccupied(Bottom, piece.shape, 0))
			window.close();
		holdHappened = false;
		lockDelayCounter = 0;
	}
}
void clearLines() {
	std::vector<int> indexesOfClearedLines; //store indexes
	int pieceInRowCounter = 0;
	for (int i = 0; i < BOARD_HEIGTH; i++) {
		pieceInRowCounter = 0;
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (board[i][j] != 0)
				pieceInRowCounter++;
		}
		//find from which indexes lines should be removed
		if (pieceInRowCounter == 10) {
			indexesOfClearedLines.push_back(i);
			pieceInRowCounter = 0;
		}
	}
	if (!indexesOfClearedLines.empty()) {
		int temp = indexesOfClearedLines.size();
		for (int iOld = BOARD_HEIGTH - 1, iNew = BOARD_HEIGTH - 1; iOld >= 0; iOld--, iNew--) {
			//skip indexes that should be romved
			while (!indexesOfClearedLines.empty() && iOld == indexesOfClearedLines.back()) {
				iOld--;
				indexesOfClearedLines.pop_back();
			}
			for (int j = 0; j < BOARD_WIDTH; j++) {
				board[iNew][j] = board[iOld][j];
			}
		}
		for (int i = temp - 1; i >= 0; i--) {
			//clear the rest of board
			for (int j = 0; j < BOARD_WIDTH; j++) {
				board[i][j] = 0;
			}
		}
	}
}
bool checkIfGameLost() {
	int rowCounter = 0;
	for (int j = 0; j < BOARD_WIDTH; j++) {
		if (board[0][j] != 0)
			rowCounter++;
	}
	if (rowCounter > 0)
		return true;
	else 
		return false;
}
