#pragma once
#include <cstdlib>
#include "SFML/System/Vector2.hpp"
using namespace std;

//https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_segment_de_Bresenham 

std::vector<sf::Vector2i> bresenhamLine(int x1, int y1, int x2, int y2)
{
    std::vector<sf::Vector2i> results = std::vector<sf::Vector2i>();
    int dx = x2 - x1, dy = 0;
    
    if(dx != 0){
        if(dx > 0){
            dy = y2 - y1;
            if(dy != 0){
                if(dy > 0){
                    // vecteur oblique dans le 1er quadran
                    if(dx >= dy){
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 1er octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        // e est positif
                        for(;;){
                            results.push_back({ x1, y1 });
                            ++x1;
                            if(x1 == x2){
                                break;
                            }
                            e -= dy;
                            if(e < 0){
                                ++y1; // déplacement diagonal
                                e += dx;
                            }
                        }
                    }else{
                        // vecteur oblique proche de la verticale, dans le 2d octant
                        int e = dy;
                        dy = e * 2 ;
                        dx = dx * 2 ;
                        // e est positif
                        for(;;){  // déplacements verticaux
                            results.push_back({ x1, y1 });
                            ++y1;
                            if(y1 == y2){
                                break;
                            }
                            e -= dx;
                            if(e < 0){
                                ++x1;  // déplacement diagonal
                                e += dy;
                            }
                        }
                    }
                }else{// dy < 0 (et dx > 0)
                    // vecteur oblique dans le 4e cadran
                    if( dx >= -dy){
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 8e octant
                        int e = dx;
                        dx = e * 2 ;
                        dy = dy * 2 ;
                        // e est positif
                        for(;;){  // déplacements horizontaux
                            results.push_back({ x1, y1 });
                            ++x1;
                            if(x1 == x2){
                                break;
                            }
                            //int e = e + dy;
                            int e = dy;
                            
                            if(e < 0){
                                --y1;  // déplacement diagonal
                                e = e + dx ;
                            }
                        }
                    }else{
                        // vecteur oblique proche de la verticale, dans le 7e octant
                        int e = dy;
                        dy = e * 2 ;
                        dx = dx * 2 ;  // e est négatif
                        for(;;){  // déplacements verticaux
                            results.push_back({ x1, y1 });
                            --y1;
                            if(y1 == y2){
                                break;
                            }
                            e += dx;
                            if(e > 0){
                                ++x1;  // déplacement diagonal
                                e += dy ;
                            }
                        }
                    }
                }
            }else{  // dy = 0 (et dx > 0)
                // vecteur horizontal vers la droite
                do{
                    results.push_back({ x1, y1 });
                    ++x1;
                }while(x1 != x2);
            }
        }else{ // dx < 0
            dy = y2 - y1;
            if(dy != 0){
                if(dy > 0){
                    // vecteur oblique dans le 2d quadran
                    if(-dx >= dy){
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 4e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        // e est négatif
                        for(;;){ // déplacements horizontaux
                            results.push_back({ x1, y1 });
                            --x1;
                            if(x1 == x2){
                                break;
                            }
                            e += dy;
                            if(e >= 0){
                                ++y1;  // déplacement diagonal
                                e += dx;
                            }
                        }
                    }else{
                        // vecteur oblique proche de la verticale, dans le 3e octant
                        int e = dy;
                        dy = e * 2 ;
                        dx = dx * 2 ; 
                        // e est positif
                        for(;;){  // déplacements verticaux
                            results.push_back({ x1, y1 });
                            ++y1;
                            if(y1 == y2){
                                break;
                            }
                            e += dx;
                            if(e <= 0) {
                                --x1;  // déplacement diagonal
                                e += dy ;
                            }
                        }
                    }
                }else{  // dy < 0 (et dx < 0)
                    // vecteur oblique dans le 3e cadran
                    if(dx <= dy){
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 5e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2; 
                        // e est négatif
                        for(;;){  // déplacements horizontaux
                            results.push_back({ x1, y1 });
                            --x1;
                            if(x1 == x2){
                                break;
                            }
                            e -= dy;
                            if(e >= 0){
                                --y1;  // déplacement diagonal
                                e += dx ;
                            }
                        }
                    }else{  // vecteur oblique proche de la verticale, dans le 6e octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        // e est négatif
                        for(;;){ // déplacements verticaux
                            results.push_back({ x1, y1 });
                            --y1;
                            if(y1==y2){
                                break;
                            }
                            e -= dx;
                            if(e >= 0){
                                --x1;  // déplacement diagonal
                                e += dy ;
                            }
                        } 
                    }
                }
            }else{  // dy = 0 (et dx < 0)
                // vecteur horizontal vers la gauche
                do{
                    results.push_back({ x1, y1 });
                    --x1;
                }while(x1 != x2);
            }
        }
    }else{
        // dx = 0
        dy = y2 - y1;
        if(dy != 0){
            if(dy > 0){
                // vecteur vertical croissant
                do{
                    results.push_back({ x1, y1 });
                    ++y1;
                }while(y1 != y2);
            }else{ // dy < 0 (et dx = 0)
                // vecteur vertical décroissant
                do{
                    results.push_back({ x1, y1 });
                    --y1;
                }while(y1 != y2);
            }
        }
    }

    results.push_back({ x2, y2 });
    return results;
}