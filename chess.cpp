#include "chess.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include "ImgUtils.h"
#include <algorithm>
#include <random>
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
                if(v==0){
                    board.get(i,j) = false;
                    goto L1;
                }
                values.push_back(v);
            }
            sort(values.begin(),values.end());
            if(*(values.end()-1)- *(values.begin()) > 700000)
                board.at(i * 8 + j) = false;
            else
                board.at(i * 8 + j) = true;
            L1:;
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
std::vector<PieceEffectRange> get_piecetype_effect_ranges(PieceType piecetype){
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
std::vector<PieceEffectRange> get_enemy_piecetype_effect_ranges(PieceType piecetype){
    typedef PieceEffectRange P;
    if(piecetype == PieceType::pawn){
        std::vector<P> ranges={PieceEffectRange{1,-1,1},PieceEffectRange{1,1,1}};
        return ranges;
    }else{
        return get_piecetype_effect_ranges(piecetype);
    }
}

size_t get_piece_effect_score(PieceType piecetype){
    typedef PieceType P;
    switch (piecetype){
    case P::bishop:
        return 13;
    case P::rook:
        return 14;
    case P::king:
        return 8;
    case P::queen:
        return 27;
    case P::knight:
        return 8;
    case P::pawn:
        return 2;
    case P::unknow:
        return 0;
    }
}

Board set_to_board(Board const b,std::vector<Piece> enemy_pieces, std::vector<Piece> pieces){
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
    for(Piece & piece : enemy_pieces){
        points_map[piece.point] = true;
    }
    Board no_effect_point{};
    for(Piece & piece : enemy_pieces){
        auto x_ = piece.point.x;
        auto y_ = piece.point.y;
        for(auto & effect_range : get_enemy_piecetype_effect_ranges(piece.piece_type)){
            for(auto i=1;;++i){
                auto dx = effect_range.dx * i;
                auto dy = effect_range.dy * i;
                auto x = x_ + dx;
                auto y = y_ + dy;
                if(x>=0 && x < 8&&y>=0&&y<8){
                    no_effect_point.get(x,y) = true;
                    if(points_map[Point{x,y}])
                        break;
                }else{
                    break;
                }
                if(i==effect_range.n)
                    break;
            }
        }
    }
//    std::cout << static_cast<std::string>(no_effect_point) << "\n";
    for(Piece & piece : pieces){
        auto x_ = piece.point.x;
        auto y_ = piece.point.y;
        if(no_effect_point.get(x_,y_))
            continue;
        for(auto & effect_range : get_piecetype_effect_ranges(piece.piece_type)){
            for(int i =1;;++i){
                auto dx = effect_range.dx * i;
                auto dy = effect_range.dy * i;
                auto x = x_ + dx;
                auto y = y_ + dy;
                if (x>= 0 && x <8 && y >=0 && y <8){
                    rv.at(x*8+y) = false;
                    if(points_map[Point{x,y}])
                        break;
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
inline size_t get_left_score(Board board){
    return std::count(board.begin(), board.end(), true);
}
std::vector<Point> get_points(Board board){
    std::vector<Point> rv;
    for(int i =0;i<8;++i){
        for(int j=0;j<8;++j){
            if(board.get(i,j))
                rv.push_back({i,j});
        }
    }
    return rv;
}
int times{0};
bool solve(std::vector<std::vector<Piece>> & answers, Board b,
           std::vector<Piece> enemy_pieces, std::vector<PieceType> piecetypes,std::vector<Piece> current_choice={}){
//    for(auto x : current_choice){
//        std::cout << static_cast<std::string>(x) << " ";
//    }
//    std::cout << get_left_score(set_to_board(b, enemy_pieces, current_choice)) << "\n";
    if(piecetypes.size() == 0 && get_left_score(set_to_board(b, enemy_pieces, current_choice)) == 0){
        answers.push_back(current_choice);
        return true;
    }
    if(piecetypes.size() == 0)
        return false;
    struct Choice{
        Piece piece;
        size_t score;
    };

    std::vector<Choice> choices{};
    PieceType piecetype = piecetypes[piecetypes.size() - 1];
    piecetypes.pop_back();
    Board b_place{b};
    for(auto p : current_choice){
        b_place.get(p.point.x,p.point.y) = false;
    }
    for(auto point : get_points(b_place)){
        times++;
        Piece piece{piecetype, point};
        current_choice.push_back(piece);
        size_t n = get_left_score(set_to_board(b,enemy_pieces, current_choice));
        Choice c{piece, n};
        choices.push_back(c);
        current_choice.pop_back();
    }
    std::sort(choices.begin(),choices.end(), [](Choice a,Choice b){return a.score < b.score;});
    for(auto x : choices){
//        std::cout << static_cast<std::string>(x.piece) << " " << x.score<<"\n";
        size_t sum = 0;
        for(auto x : piecetypes){
            sum += get_piece_effect_score(x);
        }
        if(sum < x.score)
            break;
        current_choice.push_back(x.piece);
        if (solve(answers, b, enemy_pieces, piecetypes, current_choice)){
            return true;
        }
        current_choice.pop_back();
    }
    return false;
}
std::vector<std::vector<Piece>> solve(Board board, std::vector<Piece> enemy_pieces, PieceCount piece_count){
    std::random_device rd;
    std::mt19937 g(rd());
    std::srand(std::time(nullptr));
    std::vector<std::vector<Piece>> answers;
    std::vector<PieceType> piecetypes;
    for(auto pair : piece_count){
        for(int i=0;i<std::get<1>(pair);++i){
            piecetypes.push_back(std::get<0>(pair));
        }
    }
    std::sort(piecetypes.begin(), piecetypes.end(),std::greater<PieceType>());
    for(auto x : piecetypes){
        std::cout << get_PieceType_name(x) << " ";
    }
    std::cout << "\n";
    solve(answers, board, enemy_pieces, piecetypes);
    for(int i = 0;answers.size() == 0 && i < 42 ;++i){
        std::shuffle(piecetypes.begin(), piecetypes.end(),g);
        for(auto x : piecetypes){
            std::cout << get_PieceType_name(x) << " ";
        }
        std::cout << "\n";
        solve(answers, board, enemy_pieces, piecetypes);
    }
    std::cout << times << "\n";
    return answers;
}
std::string pprint(Piece p){
    return get_PieceType_name(p.piece_type) + "," + static_cast<char>('8' - p.point.x) + static_cast<char>('a' + p.point.y);
}
std::string pprint(std::vector<Piece> ps){
    std::string rv;
    for(auto x : ps){
        rv += pprint(x) + " ";
    }
    return rv;
}
}
