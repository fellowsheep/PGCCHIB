//
//  SlideView.h
//  ExercSlidemap
//
//  Created by Leandro Tonietto on 09/05/16.
//  Copyright © 2016 Leandro Tonietto. All rights reserved.
//

#ifndef SlideView_h
#define SlideView_h

#include "TilemapView.h"
#include <iostream>
using namespace std;

class SlideView : public TilemapView {
public:
    void computeDrawPosition(const int col, const int row, const float tw, const float th, float &targetx, float &targety) const {
        targetx = col * tw + row * tw/2;
        targety = row * th / 2;
    }
    
    void computeMouseMap(int &col, int &row, const float tw, const float th, const float mx, const float my) const {
        float tw2 = tw / 2.0f;
        float th2 = th / 2.0f;
        
        row = my / th2;
        col = (mx - row * tw2) / tw;
        // cout << "dest: " << col << "," << row << endl;

    }
    
    void computeTileWalking(int &col, int &row, const int direction) const {
        switch(direction){
            case DIRECTION_NORTH: 
                col--; 
                row += 2;
                break;
            case DIRECTION_EAST: 
                col++;
                break;
            case DIRECTION_SOUTH: 
                col++; 
                row -= 2;
                break;
            case DIRECTION_WEST: 
                col--;
                break;
            case DIRECTION_NORTHEAST: 
                row++;
                break;
            case DIRECTION_SOUTHEAST: 
                col++; 
                row--;
                break;
            case DIRECTION_SOUTHWEST: 
                row--;
                break;
            case DIRECTION_NORTHWEST: 
                col--; 
                row++;
                break;       
        }
    } 
    
};
    
#endif /* SlideView_h */
