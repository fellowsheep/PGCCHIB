//
//  DiamondView.h
//  ExercSlidemap
//
//  Criado com base em SlideView por Leandro Tonietto
//

#ifndef DiamondView_h
#define DiamondView_h

#include "TilemapView.h"
#include <iostream>
using namespace std;

class DiamondView : public TilemapView {
public:
    void computeDrawPosition(const int col, const int row, const float tw, const float th, float &targetx, float &targety) const {
        targetx = (col - row) * (tw / 2.0f);
        targety = (col + row) * (th / 2.0f);
    }

    void computeMouseMap(int &col, int &row, const float tw, const float th, const float mx, const float my) const {
        float halfTw = tw / 2.0f;
        float halfTh = th / 2.0f;

        float tempCol = (mx / halfTw + my / halfTh) / 2.0f;
        float tempRow = (my / halfTh - mx / halfTw) / 2.0f;

        col = static_cast<int>(floor(tempCol));
        row = static_cast<int>(floor(tempRow));
    }

    void computeTileWalking(int &col, int &row, const int direction) const {
        switch(direction){
            case DIRECTION_NORTH:       row--; break;
            case DIRECTION_EAST:        col++; break;
            case DIRECTION_SOUTH:       row++; break;
            case DIRECTION_WEST:        col--; break;
            case DIRECTION_NORTHEAST:   col++; row--; break;
            case DIRECTION_SOUTHEAST:   col++; row++; break;
            case DIRECTION_SOUTHWEST:   col--; row++; break;
            case DIRECTION_NORTHWEST:   col--; row--; break;
        }
    }
};

#endif /* DiamondView_h */
