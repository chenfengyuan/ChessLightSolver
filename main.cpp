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
    std::cout << ref.rows << " " << ref.cols << std::endl;
    std::cout << ref.at<cv::Vec3b>(262,216) << std::endl;
    std::cout << static_cast<int>(ref.at<cv::Vec3b>(262,216)[0]) << std::endl;
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
    using Chess::PieceType;
    Chess::PieceImgs piece_imgs = Chess::get_PieceImgs_from_dir("chess pieces");
    for(auto pair : Chess::get_piece_count(ref,piece_imgs)){
        std::cout << get_PieceType_name(std::get<0>(pair)) << "," << std::get<1>(pair) << "\n";
    }
    for(auto x : ImgUtils::get_positions_in_board(ref,cv::imread("enemy_pieces/rook.png"))){
        std::cout << x << "\n";
    }
}
