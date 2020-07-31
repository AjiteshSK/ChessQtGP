#include "ChessAlgorithm.h"

ChessAlgorithm::ChessAlgorithm(QObject* parent) {
	m_board = nullptr;
	m_result = NoResult;
	m_currentPlayer = Player1;

	//connect(this, &ChessAlgorithm::gameOver, this, &ChessAlgorithm::endGame);
	
}

void ChessAlgorithm::setBoard(ChessBoard *board)
{
	if (board == m_board) {
		return;
	}
	delete m_board;
	m_board = board;
	emit boardChanged(m_board);
}

void ChessAlgorithm::setResult(Result value){
	if (result() == value) {
		return;
	}

	if (result() == NoResult) {
		m_result = value;
		emit gameOver(m_result);
	}
	else {
		m_result = value;
	}
}

void ChessAlgorithm::setCurrentPlayer(Player value){
	if (currentPlayer() == value) {
		return;
	}
	m_currentPlayer = value;
	emit currentPlayerChanged(m_currentPlayer);
}

ChessBoard * ChessAlgorithm::board() const
{
	return m_board;
}

bool ChessAlgorithm::move(int colFrom, int rankFrom, int colTo, int rankTo)
{
	Q_UNUSED(colFrom)
		Q_UNUSED(rankFrom)
		Q_UNUSED(colTo)
		Q_UNUSED(rankTo)
		return false;
}

bool ChessAlgorithm::move(const QPoint & from, const QPoint & to)
{
	return move(from.x(), from.y(), to.x(), to.y());
}


void ChessAlgorithm::setupBoard() {
	setBoard(new ChessBoard(8, 8, this)); //Point of creation of the base data structure
}

void ChessAlgorithm::newGame() {//What happens here is, we construct a new board, but it's not the one linked to ChessView and consequently MainWindow. This function is only useful for the first time a game is ran
	setupBoard();//Sets up an empty board
	board()->setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); //Fills it with FEN-data
}


standardChess::standardChess(QObject * parent){}

//MEAT
bool standardChess::move(int colFrom, int rankFrom, int colTo, int rankTo)
{
	if (currentPlayer() == NoPlayer) {
		return false;
	}

	char source = board()->data(colFrom, rankFrom);


	if (currentPlayer() == Player1 && !(isupper(source))) return false;//White player
	if (currentPlayer() == Player2 && !(islower(source))) return false;//Black player


	//Check whether the move is in bounds|| Kinda redundant now
	if (colTo < 1 || colTo  > board()->columns()) return false;
	if (rankTo < 1 || rankTo > board()->ranks())   return false;

	//Check if the move is legal||should also account for protection removal
	if (!isPieceMovable(source, colFrom, rankFrom, colTo, rankTo)) {
		return false;
	}

	//Move the piece
	board()->movePiece(colFrom, rankFrom, colTo, rankTo);
	
	//ISPIECEMOVABLE IS MY SALVATION!!! Costly though. The solution is, to check ALL the remaining same colored pieces and check isPieceMovable() for all the fields on the board. If all the pieces have nowhere to go, then it is checkmate, because it implies no way out of the check

	
	
	if (isWhiteCheckMate()) {
		setResult(Result::Player2Wins);
	}
	

	
	else if(isBlackCheckMate()) {
		setResult(Result::Payer1Wins);
	}
	
	
	
	else {
		setCurrentPlayer(currentPlayer() == Player1 ? Player2 : Player1);
	}
	//Win Condition
	//if isCheck()
	//is checkMate()
	//checkMate() woul have to enumerate all possible board positions. A possible way out would be to check all the players for possible postions that cause !isCheck. If no such positions exist, then it's checkmate

	return true;
}

bool standardChess::isPieceMovable(char piece, int colFrom, int rankFrom, int colTo, int rankTo) {
	std::vector<std::pair<int, int>>all_possible_positions;
	all_possible_positions.push_back(std::pair<int, int>(0, 0));

	switch (piece) {
	case 'r':
	case 'R': {

		auto vertical_l = vertical(piece, colFrom, rankFrom);
		auto horizontal_l = horizontal(piece, colFrom, rankFrom);
		all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
		all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
		for (auto i : all_possible_positions) {
			if (std::pair<int, int>(colTo, rankTo) == i && !isCheck(piece, colFrom, rankFrom, colTo, rankTo)) {//&&!isCheck(piece, from, to);
				return true;
			}
		}
		break;
	}
	case 'n':
	case 'N': {
		auto knight_l = knightPos(piece, colFrom, rankFrom);
		all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
		for (auto i : all_possible_positions) {
			if (std::pair<int, int>(colTo, rankTo) == i&&!isCheck(piece, colFrom, rankFrom, colTo, rankTo)) {
				return true;
			}
		}
		break;
	}
	case 'b':
	case 'B': {
		auto diagonal_l = diagonal(piece, colFrom, rankFrom);
		all_possible_positions.insert(all_possible_positions.end(), diagonal_l.begin(), diagonal_l.end());
		for (auto i : all_possible_positions) {
			if (std::pair<int, int>(colTo, rankTo) == i&&!isCheck(piece, colFrom, rankFrom, colTo, rankTo)) {
				return true;
			}
		}
		break;
	}
	case 'q':
	case 'Q': {
		auto diagonal_l = diagonal(piece, colFrom, rankFrom);
		auto vertical_l = vertical(piece, colFrom, rankFrom);
		auto horizontal_l = horizontal(piece, colFrom, rankFrom);
		all_possible_positions.insert(all_possible_positions.end(), diagonal_l.begin(), diagonal_l.end());
		all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
		all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
		for (auto i : all_possible_positions) {
			if (std::pair<int, int>(colTo, rankTo) == i && !isCheck(piece, colFrom, rankFrom, colTo, rankTo)) {//if removesCheck
				return true;
			}
		}
		break;
	}
	case 'k':
	case 'K': {
		auto king_l = kingPos(piece, colFrom, rankFrom);
		all_possible_positions.insert(all_possible_positions.end(), king_l.begin(), king_l.end());
		for (auto i : all_possible_positions) {
			if (std::pair<int, int>(colTo, rankTo) == i&&!isCheck(piece, colFrom, rankFrom, colTo, rankTo)) {
				return true;
			}
		}
		break;
	}
	case 'p': 
	case 'P': {
		auto pawn_l = pawnPos(piece, colFrom, rankFrom);
		all_possible_positions.insert(all_possible_positions.end(), pawn_l.begin(), pawn_l.end());
		for (auto i : all_possible_positions) {
			if (std::pair<int, int>(colTo, rankTo) == i&&!isCheck(piece, colFrom, rankFrom, colTo, rankTo)) {
				return true;
			}
		}
		break;
	}
 }

	return false;
}

std::vector<std::pair<int, int>> standardChess::horizontal(char piece, int curr_column, int curr_rank) {
	std::vector < std::pair<int, int>> hor_pos;

	for (int i = curr_column+1, j = curr_rank; i <= board()->columns(); i++) {//Positions To the right
		if (board()->data(i, j) == ' ') {
			hor_pos.push_back(std::pair<int, int>(i, j));
		}
		else {
			if (isupper(piece)) {
				if (isupper(board()->data(i, j))) {//Both pieces are same color
					break;
				}
				else {//Pieces are different color
					hor_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
			else {
				if (islower(board()->data(i, j))) {
					break;
				}
				else {
					hor_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
		}
	}

	for (int i = curr_column-1, j = curr_rank; i >= 1; i--) {//Positions to the left
		if (board()->data(i, j) == ' ') {//if(!isCheck(piece, i,j))
			hor_pos.push_back(std::pair<int, int>(i, j));
		}
		else {
			if (isupper(piece)) {
				if (isupper(board()->data(i, j))) {//Both pieces are same color
					break;
				}
				else {//Pieces are different color
					hor_pos.push_back(std::pair<int, int>(i, j));
					//Check for King. Set bool isCheck to TRUE in that case.
					break;
				}
			}
			else {
				if (islower(board()->data(i, j))) {
					break;
				}
				else {
					hor_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
		}
	}

	return hor_pos;
}

std::vector<std::pair<int, int>> standardChess::vertical(char piece, int curr_column, int curr_rank) {
	std::vector < std::pair<int, int>> ver_pos;

	for (int i = curr_column, j = curr_rank+1; j <= board()->columns(); j++) {//Positions to the top
		if (board()->data(i, j) == ' ') {
			ver_pos.push_back(std::pair<int, int>(i, j));
		}
		else {
			if (isupper(piece)) {
				if (isupper(board()->data(i, j))) {//Both pieces are same color
					break;
				}
				else {//Pieces are different color
					ver_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
			else {
				if (islower(board()->data(i, j))) {
					break;
				}
				else {
					ver_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
		}
	}

	for (int i = curr_column, j = curr_rank-1; j >= 1; j--) {//Positions to the bottom
		if (board()->data(i, j) == ' ') {
			ver_pos.push_back(std::pair<int, int>(i, j));
		}
		else {
			if (isupper(piece)) {
				if (isupper(board()->data(i, j))) {//Both pieces are same color
					break;
				}
				else {//Pieces are different color
					ver_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
			else {
				if (islower(board()->data(i, j))) {
					break;
				}
				else {
					ver_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
		}
	}

	return ver_pos;
}

std::vector<std::pair<int, int>> standardChess::diagonal(char piece, int curr_column, int curr_rank) {
	std::vector<std::pair<int, int>> dia_pos;

	//Top-Left(Column--. Rank++)
	for (int i = curr_column-1, j = curr_rank+1; i >= 1&& j <= board()->ranks(); i--, j++) {
		if (board()->data(i, j) == ' ') {
			dia_pos.push_back(std::pair<int, int>(i, j));
		}
		else {
			if (isupper(piece)) {
				if (isupper(board()->data(i, j))) {//Both pieces are same color
					break;
				}
				else {//Pieces are different color
					dia_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
			else {
				if (islower(board()->data(i, j))) {
					break;
				}
				else {
					dia_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
		}
	}

	//Top-Right(Column++,Rank++)
	for (int i = curr_column+1, j = curr_rank+1; i <=board()->columns()&& j <= board()->ranks(); i++, j++) {
		if (board()->data(i, j) == ' ') {
			dia_pos.push_back(std::pair<int, int>(i, j));
		}
		else {
			if (isupper(piece)) {
				if (isupper(board()->data(i, j))) {//Both pieces are same color
					break;
				}
				else {//Pieces are different color
					dia_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
			else {
				if (islower(board()->data(i, j))) {
					break;
				}
				else {
					dia_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
		}
	}

	//Bottom-Left(Columns--,Ranks--)
	for (int i = curr_column-1, j = curr_rank-1; i >= 1&& j >=1; i--, j--) {
		if (board()->data(i, j) == ' ') {
			dia_pos.push_back(std::pair<int, int>(i, j));
		}
		else {
			if (isupper(piece)) {
				if (isupper(board()->data(i, j))) {//Both pieces are same color
					break;
				}
				else {//Pieces are different color
					dia_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
			else {
				if (islower(board()->data(i, j))) {
					break;
				}
				else {
					dia_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
		}
	}

	//Bottom-Right(Columns++,Ranks--)
	for (int i = curr_column+1, j = curr_rank-1; i <=board()->columns()&& j >=1; i++, j--) {
		if  (board()->data(i, j) == ' ') {
			dia_pos.push_back(std::pair<int, int>(i, j));
		}
		else {
			if (isupper(piece)) {
				if (isupper(board()->data(i, j))) {//Both pieces are same color
					break;
				}
				else {//Pieces are different color
					dia_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
			else {
				if (islower(board()->data(i, j))) {
					break;
				}
				else {
					dia_pos.push_back(std::pair<int, int>(i, j));
					break;
				}
			}
		}
	}

	return dia_pos;
}

std::vector<std::pair<int, int>> standardChess::knightPos(char piece, int curr_column, int curr_rank)
{
	std::vector<std::pair<int, int>> knight_pos;

	//Top-Left
	curr_column--;
	curr_rank += 2;
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}

	//Top-Right
	curr_column+=2;
	//Rank stays same
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}
	
	curr_column++;
	curr_rank--;
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}
	
	//Column stays same
	curr_rank -=2;
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}

	curr_column--;
	curr_rank--;
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}

	curr_column-=2;
	//Rank stays same
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}

	curr_column--;
	curr_rank ++;
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}

	//Column stays same
	curr_rank+=2;
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					knight_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}

	return knight_pos;
}

std::vector<std::pair<int, int>> standardChess::kingPos(char piece, int curr_column, int curr_rank) {
	std::vector<std::pair<int, int>> king_pos;

	for (int i = curr_column - 1, j = curr_rank + 1;i<=curr_column+1; i++) {
		if (1 <= i && i <= board()->columns() && 1 <= j && j <= board()->ranks()) {
			if (board()->data(i, j) == ' ') {
				king_pos.push_back(std::pair<int, int>(i, j));
			}//Else- check for enemy pieces
			else {
				if (isupper(piece)) {
					if (isupper(board()->data(i, j))) {
						break;
					}
					else {
						king_pos.push_back(std::pair<int, int>(i, j));
						break;
					}
				}
				else {
					if (islower(board()->data(i, j))) {
						king_pos.push_back(std::pair<int, int>(i, j));
						break;
					}
					else {//redundant?
						break;
					}
				}
			}
		}
	}

	for (int i = curr_column - 1, j = curr_rank - 1, increment = 0; i<=curr_column+1;i++) {
		if (1 <= i && i <= board()->columns() && 1 <= j && j <= board()->ranks()) {
			if (board()->data(i, j) == ' ') {
				king_pos.push_back(std::pair<int, int>(i, j));
			}
			else {
				if (isupper(piece)) {
					if (islower(board()->data(i, j))) {
						king_pos.push_back(std::pair<int, int>(i, j));
						break;
					}
					
				}
				else {
					if (isupper(board()->data(i, j))) {
						king_pos.push_back(std::pair<int, int>(i, j));
						break;
					}
					
				}
			}
			}
	}

	curr_column--;
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			king_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					king_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					king_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}

	curr_column += 2;
	if (1 <= curr_column && curr_column <= board()->columns() && 1 <= curr_rank && curr_rank <=board()->ranks()) {
		if (board()->data(curr_column, curr_rank) == ' ') {
			king_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
		}
		else {
			if (isupper(piece)) {
				if (islower(board()->data(curr_column, curr_rank))) {//Pieces are different color
					king_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
			else {
				if (isupper(board()->data(curr_column, curr_rank))) {
					king_pos.push_back(std::pair<int, int>(curr_column, curr_rank));
				}
			}
		}
	}
	

	return king_pos;
}

std::vector<std::pair<int, int>> standardChess::pawnPos(char piece, int curr_column, int curr_rank)
{
	std::vector<std::pair<int, int>> pawn_pos;
	
	if (islower(piece)) {
		if (curr_rank == 7) {

			if (board()->data(curr_column, curr_rank - 2) == ' ') {
				pawn_pos.push_back(std::pair<int, int>(curr_column, curr_rank - 2));
			}
		}
		if (board()->data(curr_column, curr_rank - 1) == ' ') {
			pawn_pos.push_back(std::pair<int, int>(curr_column, curr_rank - 1));
		}

		if (isupper(board()->data(curr_column - 1, curr_rank - 1)) && curr_column - 1 >= 1) {
			pawn_pos.push_back(std::pair<int, int>(curr_column - 1, curr_rank - 1));
		}
		if (isupper(board()->data(curr_column + 1, curr_rank - 1)) && curr_column + 1 <= board()->columns()) {
			pawn_pos.push_back(std::pair<int, int>(curr_column + 1, curr_rank - 1));
		}
		
	}
	else {
		if (curr_rank == 2) {
			if (board()->data(curr_column, curr_rank + 2) == ' ') {
				pawn_pos.push_back(std::pair<int, int>(curr_column, curr_rank + 2));
			}
		}
		if (board()->data(curr_column, curr_rank + 1) == ' ') {
			pawn_pos.push_back(std::pair<int, int>(curr_column, curr_rank + 1));
		}

		if (islower(board()->data(curr_column - 1, curr_rank + 1)) && curr_column - 1 >= 1) {
			pawn_pos.push_back(std::pair<int, int>(curr_column - 1, curr_rank + 1));
		}
		if (islower(board()->data(curr_column + 1, curr_rank + 1)) && curr_column + 1 <= board()->columns()) {
			pawn_pos.push_back(std::pair<int, int>(curr_column + 1, curr_rank + 1));
		}
		
	}

	return pawn_pos;
}


/*
  First, a piece shouldn't be allowed to move to a place that causes a self-check. This has to be checked in all_possible_positions.
  Second, after a piece DOES move, we have to check the isCheck() flag. This can be done in the Win Condition block
  If isChecked, then only those moves are allowed that toggle the flag. This has to be checkd in all_possible_moves too
  If no such moves exist, then its checkmate. Win Condition.
*/

/*
bool standardChess::isCheck() {//Diff versions for colors?
	for (int i = 1; i <= board()->columns(); i++) {
		for (int j = 1; j <= board()->columns(); j++) {
			char piece = board()->data(i, j);
			switch (piece) {
			case 'r': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto vertical_l = vertical(piece, i, j);
				auto horizontal_l = horizontal(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
				all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'K') {
						return true;
					}
				}
			}
			case 'R':{
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto vertical_l = vertical(piece, i, j);
				auto horizontal_l = horizontal(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
				all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'k') {
						return true;
					}
				}
			}
			case 'n': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto knight_l = knightPos(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'K') {
						return true;
					}
				}
			}
			case 'N': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto knight_l = knightPos(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'k') {
						return true;
					}
				}
			}
			case 'b':{
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto knight_l = diagonal(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'K') {
						return true;
					}
				}
			}
			case 'B': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto knight_l = diagonal(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'k') {
						return true;
					}
				}
			}
			case 'q': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto diagonal_l = diagonal(piece, i, j);
				auto vertical_l = vertical(piece, i, j);
				auto horizontal_l = horizontal(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), diagonal_l.begin(), diagonal_l.end());
				all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
				all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'k') {
						return true;
					}
				}
			}
			case 'Q': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto diagonal_l = diagonal(piece, i, j);
				auto vertical_l = vertical(piece, i, j);
				auto horizontal_l = horizontal(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), diagonal_l.begin(), diagonal_l.end());
				all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
				all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'k') {
						return true;
					}
				}
			}
			case 'k': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto knight_l = kingPos(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'K') {
						return true;
					}
				}
			}
			case 'K': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto knight_l = kingPos(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'k') {
						return true;
					}
				}
			}
			case 'p': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto knight_l = pawnPos(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'K') {
						return true;
					}
				}
			}
			case 'P': {
				std::vector<std::pair<int, int>>all_possible_positions;
				all_possible_positions.push_back(std::pair<int, int>(0, 0));
				auto knight_l = pawnPos(piece, i, j);
				all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
				for (auto x : all_possible_positions) {
					if (board()->data(x.first, x.second) == 'K') {
						return true;
					}
				}
			}
			}
		}
	}
	return false;
}
*/

//Heavy-ass function call

bool standardChess::isCheck(char source, int colFrom, int rankFrom, int colTo, int rankTo) {
	char pseudo_piece = board()->data(colTo, rankTo);
	board()->movePiece(colFrom, rankFrom, colTo, rankTo);
	//A way around this is move and undo the move on the same board
	if (isupper(source)) {
		//test->movePiece(colFrom, rankFrom, colTo, rankTo);
		//This has to be changed to 
		for (int i = 1; i <= board()->columns(); i++) {
			for (int j = 1; j <= board()->columns(); j++) {
				char piece = board()->data(i, j);
				switch (piece) {
				case 'r': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto vertical_l = vertical(piece, i, j);
					auto horizontal_l = horizontal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
					all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'K') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom);
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'n': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto knight_l = knightPos(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'K') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom);
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'b': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto knight_l = diagonal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'K') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom);
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'q': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto diagonal_l = diagonal(piece, i, j);
					auto vertical_l = vertical(piece, i, j);
					auto horizontal_l = horizontal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), diagonal_l.begin(), diagonal_l.end());
					all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
					all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'K') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom); 
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'k': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto knight_l = kingPos(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'K') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom);
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'p': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto knight_l = pawnPos(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'K') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom); 
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				}
			}
		}
	}
	else {
		for (int i = 1; i <= board()->columns(); i++) {
			for (int j = 1; j <= board()->columns(); j++) {
				char piece = board()->data(i, j);
				switch (piece) {
				case 'R': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto vertical_l = vertical(piece, i, j);
					auto horizontal_l = horizontal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
					all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {//Ohh Lord
							if (board()->data(x.first, x.second) == 'k') {//x.first =0, x.second = 0. Hence, out of range. board()->data(all_possible_positions[i].first, all_possible_positions[i].second)
								board()->movePiece(colTo, rankTo, colFrom, rankFrom);
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'N': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto knight_l = knightPos(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'k') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom);
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'B': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto knight_l = diagonal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'k') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom); 
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'Q': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto diagonal_l = diagonal(piece, i, j);
					auto vertical_l = vertical(piece, i, j);
					auto horizontal_l = horizontal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), diagonal_l.begin(), diagonal_l.end());
					all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
					all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'k') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom); 
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'K': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto knight_l = kingPos(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'k') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom);
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				case 'P': {
					std::vector<std::pair<int, int>>all_possible_positions;
					
					auto knight_l = pawnPos(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), knight_l.begin(), knight_l.end());
					if (all_possible_positions.size() >= 1) {
						for (auto x : all_possible_positions) {
							if (board()->data(x.first, x.second) == 'K') {
								board()->movePiece(colTo, rankTo, colFrom, rankFrom);
								board()->setData(colTo, rankTo, pseudo_piece);
								return true;
							}
						}
					}
					break;
				}
				}
			}
		}
	}

	board()->movePiece(colTo, rankTo, colFrom, rankFrom);
	board()->setData(colTo, rankTo, pseudo_piece);
	return false;
}

//Heavier-ass function call

bool standardChess::isBlackCheckMate(){
	//std::vector<std::pair<int, int>> possible_escape;
	for (int i = 1; i <= board()->columns(); i++) {
		for (int j = 1; j <= board()->ranks(); j++) {
			if (islower(board()->data(i, j))) {
				char piece = board()->data(i, j); 
				switch (piece) {
				case'r': {
					std::vector<std::pair<int, int>> all_possible_positions;
					//all_possible_positions.push_back(std::pair<int, int>(0, 0));
					auto vertical_l = vertical(piece, i, j);
					auto horizontal_l = horizontal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
					all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
						for (auto x:all_possible_positions) {
							if (isPieceMovable(piece, i, j, x.first, x.second)) {
								return false;
							}
						}
					
					break;
				}
				case'n': {
					std::vector<std::pair<int, int>> all_possible_positions{ knightPos(piece,i,j) };
					for (auto x: all_possible_positions) {  //Instead of these two fucking loops, I could use knightPos to significantly 
							   //decrease the number of checks I have to do
							if (isPieceMovable(piece, i, j, x.first, x.second)) {
								return false;
							}
					}
					break; 
				}
				case'b': {
					std::vector<std::pair<int, int>> all_possible_positions{ diagonal(piece,i,j) };
						for (auto x:all_possible_positions) {
							if (isPieceMovable(piece, i, j, x.first, x.second)) {
								return false;
							}
						}
					
						 break; 
				}
				case'q': {
					std::vector<std::pair<int, int>> all_possible_positions;
					auto vertical_l = vertical(piece, i, j);
					auto horizontal_l = horizontal(piece, i, j);
					auto diagonal_l = diagonal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
					all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
					all_possible_positions.insert(all_possible_positions.end(), diagonal_l.begin(), diagonal_l.end());
					for (auto x: all_possible_positions) {
							if (isPieceMovable(piece, i, j, x.first, x.second)) {
								return false;
							}
						}
					
					break; 
				}
				case'k': {
					std::vector<std::pair<int, int>> all_possible_positions{ kingPos(piece,i,j) };
						for (auto x: all_possible_positions) {
							if (isPieceMovable(piece, i, j, x.first, x.second)) {
								return false;
							}
						}
					
					break; 
				}
				case'p': {
					std::vector<std::pair<int, int>> all_possible_positions{ pawnPos(piece,i,j) };
						for (auto x : all_possible_positions) {
							if (isPieceMovable(piece, i, j, x.first, x.second)) {
								return false;
							}
						
					}
						break; 
				}

				}
			}
		}
	}
	return true;
}

bool standardChess::isWhiteCheckMate() {
	
	for (int i = 1; i <= board()->columns(); i++) {
		for (int j = 1; j <= board()->ranks(); j++) {
			if (isupper(board()->data(i, j))) {
				char piece = board()->data(i, j);
				switch (piece) {
				case'R': {
					std::vector<std::pair<int, int>> all_possible_positions;
					//all_possible_positions.push_back(std::pair<int, int>(0, 0));
					auto vertical_l = vertical(piece, i, j);
					auto horizontal_l = horizontal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
					all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
					for (auto x : all_possible_positions) {
						if (isPieceMovable(piece, i, j, x.first, x.second)) {
							return false;
						}
					}

					break;
				}
				case'N': {
					std::vector<std::pair<int, int>> all_possible_positions{ knightPos(piece,i,j) };
					for (auto x : all_possible_positions) {  //Instead of these two fucking loops, I could use knightPos to significantly 
							   //decrease the number of checks I have to do
						if (isPieceMovable(piece, i, j, x.first, x.second)) {
							return false;
						}
					}
					break;
				}
				case'B': {
					std::vector<std::pair<int, int>> all_possible_positions{ diagonal(piece,i,j) };
					for (auto x : all_possible_positions) {
						if (isPieceMovable(piece, i, j, x.first, x.second)) {
							return false;
						}
					}

					break;
				}
				case'Q': {
					std::vector<std::pair<int, int>> all_possible_positions;
					auto vertical_l = vertical(piece, i, j);
					auto horizontal_l = horizontal(piece, i, j);
					auto diagonal_l = diagonal(piece, i, j);
					all_possible_positions.insert(all_possible_positions.end(), vertical_l.begin(), vertical_l.end());
					all_possible_positions.insert(all_possible_positions.end(), horizontal_l.begin(), horizontal_l.end());
					all_possible_positions.insert(all_possible_positions.end(), diagonal_l.begin(), diagonal_l.end());
					for (auto x : all_possible_positions) {
						if (isPieceMovable(piece, i, j, x.first, x.second)) {
							return false;
						}
					}

					break;
				}
				case'K': {
					std::vector<std::pair<int, int>> all_possible_positions{ kingPos(piece,i,j) };
					for (auto x : all_possible_positions) {
						if (isPieceMovable(piece, i, j, x.first, x.second)) {
							return false;
						}
					}

					break;
				}
				case'P': {
					std::vector<std::pair<int, int>> all_possible_positions{ pawnPos(piece,i,j) };
					for (auto x : all_possible_positions) {
						if (isPieceMovable(piece, i, j, x.first, x.second)) {
							return false;
						}

					}
					break;
				}

				}
			}
		}
	}
	return true;
}