#ifndef CHESS_H
#define CHESS_H
#include <array>
#include <opencv2/opencv.hpp>
namespace Chess{
struct Board:std::array<bool, 8 *8>{
    bool operator[](int x){
        return this->at(x);
    }
};
enum struct PieceType{
    unknow,king,rook,bishop,queen,knight,pawn
};

struct Piece{
    PieceType piece_type;
    std::array<int, 2> point;
};
Chess::Board get_board(cv::Mat img);
}
#endif // CHESS_H
