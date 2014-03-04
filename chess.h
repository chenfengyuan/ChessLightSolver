#ifndef CHESS_H
#define CHESS_H
#include <array>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <algorithm>
namespace Chess{
struct Point{
    int x;
    int y;
    Point(int x_,int y_):x{x_},y{y_}{}
    Point():x{-1},y{-1}{}
    operator std::string (){
        return std::to_string(x) + "," + std::to_string(y);
    }
};
enum struct PieceType{
    unknow,king,rook,bishop,queen,knight,pawn
};
struct Piece{
    PieceType piece_type;
    Point point;
};
inline bool operator==(Piece a,Piece b){
    if(a.piece_type == b.piece_type && a.point.x == b.point.x && a.point.y == b.point.y)
        return true;
    else
        return false;
}
struct Board:std::array<bool, 8*8>{
    bool & operator[](int x){
        return this->at(x);
    }
};
struct PieceEffectRange{
    int dx;
    int dy;
    int n;
    PieceEffectRange(int dx_,int dy_, int n_):dx{dx_},dy{dy_},n{n_}{}
    operator std::string (){
        return std::to_string(dx) + "," + std::to_string(dy) + "," + std::to_string(n);
    }
};
std::vector<PieceEffectRange> get_piece_effect_ranges(PieceType piecetype);
Board set_to_board(Board const b,std::vector<Piece> pieces, std::vector<Piece> effect_pieces,bool boolean);
inline std::string get_PieceType_name(PieceType x){
    switch(x){
    case PieceType::unknow : return "unknow";
    case PieceType::king : return "king";
    case PieceType::bishop : return "bishop";
    case PieceType::queen : return "queen";
    case PieceType::knight : return "knight";
    case PieceType::pawn : return "pawn";
    case PieceType::rook : return "rook";
    }
}
inline std::vector<PieceType> get_all_piece_types(){
    std::vector<PieceType> rv{};
    for(size_t i = 1;i <= static_cast<size_t>(PieceType::pawn);++i){
        rv.push_back(static_cast<PieceType>(i));
    }
    return rv;
}

Chess::Board get_board(cv::Mat img);
struct PieceCountHash{
    typedef PieceType argument_type;
    typedef std::size_t value_type;

    value_type operator()(argument_type const& s) const
    {
        return static_cast<size_t>(s);
    }
};
struct PieceCount:std::unordered_map<PieceType,int,PieceCountHash>{
};
struct PieceImgs:std::unordered_map<PieceType,cv::Mat,PieceCountHash>{
};
PieceImgs get_PieceImgs_from_dir(std::string path);
PieceCount get_piece_count(cv::Mat img, PieceImgs pieces_imgs);
std::vector<Point> get_positions_in_board(cv::Mat origin, cv::Mat tpl);
}
#endif // CHESS_H
