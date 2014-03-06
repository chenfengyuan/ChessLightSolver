//time (cd tmp;f=`idevicescreenshot|sed -e 's/ /\n/g'|tail -1|tr -d '\n'`;cd ../;./ChessLightSolver tmp/$f)
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "ImgUtils.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "chess.h"


int main(int,char *argv[])
{
    cv::Mat ref = cv::imread(argv[1]);
//    std::cout << ref.rows << " " << ref.cols << std::endl;
//    std::cout << ref.at<cv::Vec3b>(262,216) << std::endl;
//    std::cout << static_cast<int>(ref.at<cv::Vec3b>(262,216)[0]) << std::endl;
    auto board =  Chess::get_board(ref);
    for(int i=0;i<8;++i){
        for(int j=0;j<8;++j){
            if(board[i*8+j])
                std::cout << '*';
            else
                std::cout << ".";
        }
        std::cout << "\n";
    }
//    using Chess::PieceType;
    Chess::PieceImgs piece_imgs = Chess::get_PieceImgs_from_dir("chess pieces");
    for(auto pair : Chess::get_piece_count(ref,piece_imgs)){
        std::cout << get_PieceType_name(std::get<0>(pair)) << "," << std::get<1>(pair) << "\n";
    }
//    for(auto x : Chess::get_positions_in_board(ref,cv::imread("enemy_pieces/rook.png"))){
//        std::cout << static_cast<std::string>(x) << "\n";
//    }
//    for(auto piecetype : Chess::get_all_piece_types()){
//        std::cout << get_PieceType_name(piecetype) << "\n";
//        for(auto x : Chess::get_piece_effect_ranges(piecetype)){
//            std::cout << static_cast<std::string>(x) << " ";
//        }
//        std::cout << "\n";
//    }
//    for(auto piecetype : Chess::get_all_piece_types())
//    {
//        Chess::Board board{};
//        Chess::Piece p,p1;
//        std::cout << Chess::get_PieceType_name(piecetype) << "\n";
//        p.piece_type = piecetype;
//        p.point={3,3};
//        p1.point={3,1};
//        std::vector<Chess::Piece> list={p};
//        std::vector<Chess::Piece> list1={p1};
//        board = Chess::set_to_board(board,list1,list,true);
//        for(int i=0;i<8;++i){
//            for(int j=0;j<8;++j){
//                if(board[i*8+j])
//                    std::cout << '*';
//                else
//                    std::cout << ".";
//            }
//            std::cout << "\n";
//        }
//    }
//    std::cout << "\n";
    std::vector<Chess::Piece> tmp;
    typedef Chess::PieceType PT;
    for(auto pt : {PT::bishop, PT::rook, PT::pawn, PT::knight, PT::queen}){
        std::string filename{std::string("enemy_pieces/") + Chess::get_PieceType_name(pt) + ".png"};
        for(auto x : Chess::get_positions_in_board(ref, cv::imread(filename))){
            Chess::Piece p;
            p.piece_type = pt;
            p.point = x;
            tmp.push_back(p);
            std::cout << Chess::get_PieceType_name(p.piece_type) << "," << static_cast<std::string>(x) <<" ";
        }
    }
    std::cout << "\n";
    for (auto answer : Chess::solve(board, tmp, Chess::get_piece_count(ref, piece_imgs))){
        std::cout << Chess::pprint(answer);
        for(auto p : answer){
            auto img = piece_imgs[p.piece_type];
            auto x = 127 + 65 * p.point.x;
            auto y = 65 + 65 * p.point.y;
            std::cout << x << "," << y<< "\n";
            cv::Rect roi(cv::Point(y,x), img.size());
            img.copyTo(ref(roi));
        }
        std::cout << "\n";
    }
    cv::imshow("SLOVED",ref);
}
