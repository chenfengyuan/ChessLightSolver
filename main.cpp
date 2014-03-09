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
    Chess::PieceImgs piece_imgs = Chess::get_PieceImgs_from_dir("chess pieces");
    for(auto pair : Chess::get_piece_count(ref,piece_imgs)){
        std::cout << get_PieceType_name(std::get<0>(pair)) << "," << std::get<1>(pair) << "\n";
    }
    auto tmp = Chess::get_enemy_pieces(ref);
    auto answers = Chess::solve(board, tmp, Chess::get_piece_count(ref, piece_imgs));
    for (auto answer : answers){
        Chess::draw_answers(answer, ref, piece_imgs);
        break;
    }
    std::cout << "\n";
    cv::imshow("SLOVED",ref);
    cv::moveWindow("SLOVED", 0, 0);
//    cv::waitKey(0);
    std::string c;
    std::getline(std::cin,c);
}
