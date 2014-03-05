#include "chess.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include "ImgUtils.h"
namespace Chess{
Chess::Board get_board(cv::Mat img){
    Chess::Board board{};
    for(int i = 0;i < 8;++i){
        for(int j = 0;j < 8;++j){
            auto x = 160 + i * 65;
            auto y = 90 + j * 65;
            std::vector<long long> values{};
            for(int di = -10 ;di < 10;++di){
                auto value = img.at<cv::Vec3b>(x + di,y);
                auto v = static_cast<int>(value[0]) * static_cast<int>(value[1]) * static_cast<int>(value[2]);
                values.push_back(v);
            }
            sort(values.begin(),values.end());
            if(*(values.end()-1)- *(values.begin()) > 700000)
                board.at(i * 8 + j) = false;
            else
                board.at(i * 8 + j) = true;
        }
    }
    return board;
}
PieceCount get_piece_count(cv::Mat img, PieceImgs pieces_imgs){
    PieceCount piece_count{};
    for(auto & type : {PieceType::king,PieceType::rook,PieceType::bishop,PieceType::queen,PieceType::knight,PieceType::pawn}){
        piece_count[type] = ImgUtils::get_positions(img,pieces_imgs[type]).size();
    }
    return piece_count;
}
PieceImgs get_PieceImgs_from_dir(std::string path){
    PieceImgs rv{};
    for(auto piece_type : get_all_piece_types()){
        std::string filename = get_PieceType_name(piece_type) + ".png";
        rv[piece_type] = cv::imread(path + "/" + filename);
    }
    return rv;
}
std::vector<Point> get_positions_in_board(cv::Mat origin, cv::Mat tpl){
    auto positions = ImgUtils::get_positions(origin,tpl);
    std::vector<Point> positions_in_board;
    for(auto position : positions){
        for(int i = 0;i<8;++i){
            for(int j = 0;j<8;++j){
                auto x = 192 + i * 65;
                auto y = 128 + j * 65;
                if(x - 10 > position.x && y - 10 > position.y){
                    positions_in_board.push_back(Point{i,j});
                    goto END;
                }
            }
        }
        END:;
    }
    return positions_in_board;
}
std::vector<PieceEffectRange> get_piece_effect_ranges(PieceType piecetype){
    typedef PieceEffectRange P;
    std::vector<P> ranges;
    switch(piecetype){
    case PieceType::bishop:
        for(auto i : {-1,1}){
            for(auto j : {-1, 1}){
                ranges.push_back(P{i,j,-1});
            }
        }
        return ranges;
    case PieceType::king:
        for(int i=-1;i<2;++i){
            for(int j=-1;j<2;++j){
                if(i==0 && j==0)
                    continue;
                ranges.push_back(P{i,j,1});
            }
        }
        return ranges;
    case PieceType::knight:
        for(auto i :{-1,1}){
            for(auto j :{-2,2}){
                ranges.push_back(P{i,j,1});
                ranges.push_back(P{j,i,1});
            }
        }
        return ranges;
    case PieceType::pawn:
        ranges={PieceEffectRange{-1,-1,1},PieceEffectRange{-1,1,1}};
        return ranges;
    case PieceType::rook:
        for(auto i : {-1, 1}){
            ranges.push_back(P{0,i,-1});
            ranges.push_back(P{i,0,-1});
        }
        return ranges;
    case PieceType::queen:
        for(int i=-1;i<2;++i){
            for(int j=-1;j<2;++j){
                if(i==0 && j == 0)
                    continue;
                ranges.push_back(P{i,j,-1});
            }
        }
        return ranges;
    case PieceType::unknow:
        return ranges;
    }
}
Board set_to_board(Board const b,std::vector<Piece> pieces, std::vector<Piece> effect_pieces,bool boolean){
    struct PointHash{
        size_t operator()(Point const& p) const
        {
            return p.x * 8 + p.y;
        }
    };
    Board rv{b};
    std::unordered_map<Point , bool, PointHash> points_map;
    for(Piece & piece : pieces){
        points_map[piece.point] = true;
    }
    for(Piece & piece : effect_pieces){
        for(auto & effect_range : get_piece_effect_ranges(piece.piece_type)){
            auto x_ = piece.point.x;
            auto y_ = piece.point.y;
            for(int i =1;;++i){
                auto dx = effect_range.dx * i;
                auto dy = effect_range.dy * i;
                auto x = x_ + dx;
                auto y = y_ + dy;
                if (x>= 0 && x <8 && y >=0 && y <8){
                    if(points_map[Point{x,y}])
                        break;
                    rv.at(x*8+y) = boolean;
                }else{
                    break;
                }
                if(i==effect_range.n)
                    break;
            }
        }
    }
    return rv;
}
}
