/*
 Created on 13. April 2022
 Last modified on 15. April 2022
 Author: bondoki
 
 Purpose: Rendering of the hexagon tiles in various layout to emphasize pseudo 2.5 view.
 This program provides routine for selecting and creation of hex tiles
 heavly based on the tutorial http://www.redblobgames.com/grids/hexagons/base
 Inspired by the "Coding Quickie: Isometric Tiles" example of
 Javidx9 ( https://www.youtube.com/watch?v=ukkbNKTgf5U ) gave the idea for this.
 It's just for fun and educational purpose. Feel free to modify and use it :)
 
 Note1: the hex tiles are indexed in the axial coordinate (q,r,s) system with
 boundary condition: q+r+s=0 . Solely the third coordinate is given by s=-q-r 
 and the selected tile has the coordinate (q,r).
 
    ___
   / q \
  /     \
 |       |
  \s   r/
   \___/ 
 
 Note2: The tiles have been original equally size hexagon and scaled in the next
 by 50% in height direction.
 Note3: For a more isometric 2.5 view you should first rotate your hexagon by 45°
 and scale it by 50% in height direction.
 
 License: This piece of code is licensed to Creative Commons Zero v1.0 Universal according to
 https://github.com/Bondoki/HexagonGrid/blob/main/LICENSE
 also attributing Javidx9 for the unique and easy-to-use olc::PixelGameEngine and the underlying OLC-3 license see
 https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/Licencing
 For more information about Javidx9 and OneLoneCoder please see https://github.com/OneLoneCoder
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unordered_set>
#include <set>
#include <list>
#include <unordered_map>

#include "Hex.h"

class IsometricTilesHexagon : public olc::PixelGameEngine
{
public:
  IsometricTilesHexagon()
  {
    sAppName = "Isometric Hexagon Tiles";
  }
  
private:
  // Total passed time
  float fTotalElapsedTime;
  
  // Number of tiles in world
  olc::vi2d vWorldSize = { 2, 2 };
  
  // Size of single tile graphic
  olc::vi2d vTileSize = { 72, 46 };
  
  // Size of single tile graphic and offset to align them
  olc::vi2d vTileSizeOffset = { 0, 10 };
  
  // Where to place tile (0,0) on screen (in screen pixel steps)
  olc::vi2d vOriginScreen = { 72*3, 46*3 };
  
  // Sprite that holds all imagery
  olc::Renderable  sprIsom;
  
  // List to order for drawing
  std::list<Hex> hexlist;
  
  // HexTile to Value
  using HexMap=std::unordered_map<Hex, int,decltype(hexhash), decltype(hexequal)>;
  HexMap heights{HexMap(10,hexhash,hexequal)};
  
  // Star-Sprite and shadow
  olc::Renderable  sprStar;
  
  // Board-style name
  std::string sBoardName;
  
  // Animated elements sprite
  olc::Renderable sprWater;
  olc::Renderable sprAir;
  olc::Renderable sprFire;
  olc::Renderable sprEarth;
  
public:
  bool OnUserCreate() override
  {
    fTotalElapsedTime=0.0f;
    /* initialize random seed: */
    srand (time(NULL));
    
    // Load sprites used in demonstration
    sprIsom.Load("assets/hextiles.png");
    
    sprStar.Load("assets/star.png");
    
    sprWater.Load("assets/hex_water_anim.png");
    sprAir.Load("assets/hex_air_anim.png");
    sprFire.Load("assets/hex_fire_anim.png");
    sprEarth.Load("assets/hex_earth_anim.png");
    
    // we start with hexagonal board
    sBoardName="";
    createBoard(2);
    
    return true;
  }
  
  // create the board layout and sort for drawing
  void createBoard(int nr)
  {
    //clear the list and the map
    hexlist.clear();
    heights.clear();
    
    int dist = 4;
    
    switch(nr)
    {
      case 0: //Rectangle 1
        sBoardName="Rectangle 1";
        for (int q = -3; q <= 3; q++)
        { // flat top
          int q_offset = floor((q-1)/2.0); // or q>>1
          for (int r = -5 - q_offset; r <= 4 - q_offset; r++)
          {
            if(hex_distance(Hex(q, r, -q-r),Hex(0,0,0)) != 1)
            {
              hexlist.push_back(Hex(q, r, -q-r));
              heights[Hex(q, r, -q-r)] = std::abs(r)%9+1;
            }
          }
        }
        
        break;
        
      case 1: //Rectangle 2
        sBoardName="Rectangle 2";
        for (int q = -3; q <= 3; q++)
        { // flat top
          int q_offset = floor((q+1)/2.0); 
          for (int r = -4 - q_offset; r <= 4 - q_offset; r++)
          {
            if(hex_distance(Hex(q, r, -q-r),Hex(0,0,0)) != 1)
            {
              hexlist.push_back(Hex(q, r, -q-r));
              heights[Hex(q, r, -q-r)] = std::abs(r)%9+1;
            }
          }
        }
        for (int q = -3; q <= 3; q+=2)
        { // flat top
          int r = 4 - floor((q)/2.0); 
          hexlist.push_back(Hex(q, r, -q-r));
          heights[Hex(q, r, -q-r)] = std::abs(r)%9+1;
          //for (int r = -4 - q_offset; r <= 4 - q_offset; r+=2)
        }
        
        break;
        
      case 2: //Hexagon
        sBoardName="Hexagon";
        dist = 4;
        for (int q = -dist; q <= dist; q++)
        {
          int r1 = std::max(-dist, -q - dist);
          int r2 = std::min(dist, -q + dist);
          for (int r = r1; r <= r2; r++)
          {
            hexlist.push_back(Hex(q, r, -q-r));
            heights[Hex(q, r, -q-r)] = (hex_distance(Hex(q, r, -q-r),Hex(0,0,0))+4)%9+1;
          }
        }
        break;
        
      case 3: //Parallelograms 1
        sBoardName="Parallelogram 1";
        dist = 3;
        for (int q = -dist; q <= dist; q++) 
        {
          for (int r = -3; r <= 3; r++)
          {
            if(!((r==0) && (q==0)))
            {
              hexlist.push_back(Hex(q, r, -q-r));
              heights[Hex(q, r, -q-r)] = (std::abs(r))%9+1;
            }
          }
        }
        break;
        
      case 4: //Parallelograms 2
        sBoardName="Parallelogram 2";
        dist = 2;
        for (int r = -dist; r <= dist; r++)
        {
          for (int s = -2; s <= 2; s++) 
          {
            if(!((r==0) && (s==0)))
            {
              hexlist.push_back(Hex(-r-s, r, s));
              heights[Hex(-r-s, r, s)] = (std::abs(r))%9+1;
            }
          }
        }
        break;
        
      case 5: // Parallelograms 3
        sBoardName="Parallelogram 3";
        dist = 2;
        for (int s = -dist; s <= dist; s++)
        {
          for (int q = -3; q <= 3; q++) 
          {
            if(!((s==0) && (q==0)))
            {
              hexlist.push_back(Hex(q, -q-s, s));
              heights[Hex(q, -q-s, s)] = (std::abs(q))%9+1;
            }
          }
        }
        break;
        
      case 6: // Triangle 1
        sBoardName="Triangle 1";
        dist = 6;
        for (int q = 0; q <= dist; q++)
        {
          for (int r = dist - q; r <= dist; r++)
          {
            hexlist.push_back(Hex(q-2, r-dist+1, -q+2-r+dist-1));
            heights[Hex(q-2, r-dist+1, -q+2-r+dist-1)] = (std::abs(q))%9+1;
          }
        }
        break;
        
      case 7: // Triangle 2
        sBoardName="Triangle 2";
        dist = 6;
        for (int q = 0; q <= dist; q++)
        {
          for (int r = 0; r <= dist-q; r++)
          {
            hexlist.push_back(Hex(q-2, (r-dist/2+1), -(q-2)+(-r+dist/2-1)));
            heights[Hex(q-2, (r-dist/2+1), -(q-2)+(-r+dist/2-1))] = (std::abs(q))%9+1;
          }
        }
        break;
        
      default: break;
    }
    
    // center tile 0,0 to screen center
    vOriginScreen.x= ScreenWidth()/2-vTileSize.x/2;
    vOriginScreen.y= ScreenHeight()/2-vTileSize.y/2;
    
    // sort the list to draw from top to bottom
    hexlist.sort(hexcomp_q);
    hexlist.sort(hexcomp_r);
    
  }
  
  bool OnUserUpdate(float fElapsedTime) override
  {
    
    fTotalElapsedTime += fElapsedTime;
    Clear(olc::GREY);
    
    if (GetKey(olc::Key::K1).bPressed)
      createBoard(0);
    if (GetKey(olc::Key::K2).bPressed)
      createBoard(1);
    if (GetKey(olc::Key::K3).bPressed)
      createBoard(2);
    if (GetKey(olc::Key::K4).bPressed)
      createBoard(3);
    if (GetKey(olc::Key::K5).bPressed)
      createBoard(4);
    if (GetKey(olc::Key::K6).bPressed)
      createBoard(5);
    if (GetKey(olc::Key::K7).bPressed)
      createBoard(6);
    if (GetKey(olc::Key::K8).bPressed)
      createBoard(7);
    
    
    // Get Mouse in world
    olc::vi2d vMouse = { GetMouseX(), GetMouseY() };
    
    // this piece of converts screen pixels to axial tile coordinates
    double qfrac = 4.0 / 3.0 * ((1.0*vMouse.x-vOriginScreen.x - 0.5*(vTileSize.x-vTileSizeOffset.x)) / (vTileSize.x-vTileSizeOffset.x));
    double rfrac = -2.0 / 3.0 * ((1.0*vMouse.x-vOriginScreen.x - 0.5*(vTileSize.x-vTileSizeOffset.x)) / (vTileSize.x-vTileSizeOffset.x)) + 2.0*1.7320508075688/3.0 * ((1.0*vMouse.y-vOriginScreen.y-0.5*(vTileSize.y-vTileSizeOffset.y)) / (vTileSize.y-vTileSizeOffset.y));
    double sfrac = -qfrac-rfrac;
    int qi = int(std::round(qfrac));
    int ri = int(std::round(rfrac));
    int si = int(std::round(sfrac));
    double q_diff = std::fabs(qi - qfrac);
    double r_diff = std::fabs(ri - rfrac);
    double s_diff = std::fabs(si - sfrac);
    
    if (q_diff > r_diff && q_diff > s_diff)
    {
      qi = -ri - si;
    }
    else
      if (r_diff > s_diff)
      {
        ri = -qi - si;
      }
      else
      {
        si = -qi - ri;
      }
    
    olc::vi2d vSelected = 
      {         
        qi, ri
      };
      
      
      // Handle mouse left click to switch tile
      if (GetMouse(0).bPressed)
      {
        // Guard array boundary
        auto got = heights.find (Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y));
        
        if ( got != heights.end() )
        {
          int value = heights.at(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y));
          
          value = (value%9)+1;
          heights.at(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y)) = value;
        }
        
      }
      
      // Handle mouse right click to toggle tile - creation/destruction
      if (GetMouse(1).bPressed)
      {
        // Guard array boundary
        auto got = heights.find (Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y));
        
        // tile doesn't exist -> create 
        if (( got == heights.end() ))
        {
          hexlist.push_back(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y));
          heights[Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y)] = 5;
          
          // and sort again
          hexlist.sort(hexcomp_q);
          hexlist.sort(hexcomp_r);
        }
        else // tile exists -> delete
        {
          heights.erase(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y));
          hexlist.remove(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y));
          
          // and sort again
          hexlist.sort(hexcomp_q);
          hexlist.sort(hexcomp_r);
        }
        
      }
      
      // Labmda function to convert "world" (offset-array) coordinate into screen space
      auto ToScreen = [&](int x, int y)
      {			
        return olc::vi2d
        {
          0.5*(vTileSize.x -vTileSizeOffset.x) * 3.0/2.0 *         (x)+vOriginScreen.x,
          0.5*((vTileSize.y-vTileSizeOffset.y) * 1.7320508075688 *((y) + 0.5 * ((x)&1)))+vOriginScreen.y 
        };
      };
      
      // Labmda function to convert "world" (axial) coordinate into screen space
      auto AxialToScreen = [&](int q, int r)
      {			
        
        return olc::vi2d
        {
          0.5*(vTileSize.x -vTileSizeOffset.x) * 3.0/2.0 *         (q)+vOriginScreen.x,
          0.5*((vTileSize.y-vTileSizeOffset.y) * (1.7320508075688/2.0 * (q) + 1.7320508075688*r))+vOriginScreen.y
        };
      };
      
      
      // draw tiles
      for ( auto it=hexlist.cbegin(); it!=hexlist.cend(); ++it )
      {
        int q=(*it).get_q(); int r=(*it).get_r(); int s=(*it).get_s();
        
        auto got = heights.find (Hex(q,r,s));
        
        
        if ( got != heights.end() )
        {
          olc::vi2d vWorld = AxialToScreen(q, r);
          
          int moveXSelected=0;
          int moveYSelected=0;
          
          // highlight selected hexagon
          if ( hexequal(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y), Hex(Hex(q,r,s))) )
          {
            moveXSelected=0.0;
            moveYSelected=5*std::sin(fTotalElapsedTime*5);
          }
          
          switch(got->second)
          {
            case 0:
              // Selected Tile
              DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {0 * vTileSize.x, 0}, {vTileSize.x, vTileSize.y});
              break;
            case 1:
              // Visible Tile
              DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {1 * vTileSize.x, 0}, {vTileSize.x, vTileSize.y});
              break;
            case 2:
              DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {2 * vTileSize.x, 0}, {vTileSize.x, vTileSize.y});
              break;
            case 3:
              DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {3 * vTileSize.x, 0}, {vTileSize.x, vTileSize.y});
              break;
            case 4:
              DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {4 * vTileSize.x, 0}, {vTileSize.x, vTileSize.y});
              break;
              
            case 5:
              DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {0 * vTileSize.x, 1 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              break;
            case 6:
              if ( hexequal(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y), Hex(Hex(q,r,s))) )
              {
                DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprFire.Decal(), {int(fmodf(fTotalElapsedTime*10,7.0f)) * vTileSize.x, 0 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              }
              else
              {
                DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {1 * vTileSize.x, 1 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              }
              break;
            case 7:
              if ( hexequal(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y), Hex(Hex(q,r,s))) )
              {
                DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprEarth.Decal(), {int(fmodf(fTotalElapsedTime*9,18.0f)) * vTileSize.x, 0 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              }
              else
              {
                DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {2 * vTileSize.x, 1 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              }
              break;
            case 8:
              if ( hexequal(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y), Hex(Hex(q,r,s))) )
              {
                DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprWater.Decal(), {int(fmodf(fTotalElapsedTime*9,18.0f)) * vTileSize.x, 0 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              }
              else
              {
                DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {3 * vTileSize.x, 1 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              }
              break;
              
            case 9:
              if ( hexequal(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y), Hex(Hex(q,r,s))) )
              {
                DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprAir.Decal(), {int(fmodf(fTotalElapsedTime*9,6.0f)) * vTileSize.x, 0 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              }
              else
              {
                DrawPartialDecal({vWorld.x+moveXSelected, vWorld.y+moveYSelected}, sprIsom.Decal(), {4 * vTileSize.x, 1 * vTileSize.y}, {vTileSize.x, vTileSize.y});
              }
              break;
              
          }
          
          
        }
        
        // Draw selected "yellow" frame
        {
          
          if ( hexequal(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y), Hex(Hex(q,r,s))) )
          {
            olc::vi2d vWorld = AxialToScreen(vSelected.x,vSelected.y);
            
            DrawPartialDecal({vWorld.x, vWorld.y+5*std::sin(fTotalElapsedTime*5)}, sprIsom.Decal(), {0 * vTileSize.x, 0 * vTileSize.y}, {vTileSize.x, vTileSize.y});
          }
        }
        
        // Draw star and it's shadow
        {
          int moveXSelected=0;
          int moveYSelected=0;
          
          // highlight selected hexagon
          if ( hexequal(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y), Hex(0,0,0)) )
          {
            moveXSelected=0.0;
            moveYSelected=5*std::sin(fTotalElapsedTime*5);
          }
          olc::vi2d vWorld = AxialToScreen(0,0);
          
          DrawPartialDecal({vWorld.x+vTileSize.x/6-3*std::sin(fTotalElapsedTime*5), vWorld.y-0.1*vTileSize.y+moveYSelected}, sprStar.Decal(), {1 * 46, 0}, {46, 44},  { 1.0f+0.15*std::sin(fTotalElapsedTime*5),1.0f }, olc::WHITE);
          
          DrawPartialDecal({vWorld.x+vTileSize.x/6, vWorld.y+5*std::sin(fTotalElapsedTime*5)-0.75*vTileSize.y}, sprStar.Decal(), {0 * 40, 0}, {46, 44},  { 1.0f,1.0f }, olc::WHITE);
          
        }
        
      }
      
      // Draw Debug Info
      DrawString(4, 4, "Mouse   : " + std::to_string(vMouse.x) + ", " + std::to_string(vMouse.y), olc::BLACK);
      DrawString(4, 14, "Selected: " + std::to_string(vSelected.x) + ", " + std::to_string(vSelected.y), olc::BLACK);
      DrawString(4, 24, "Style: " + sBoardName, olc::BLACK);
      
      DrawString(4, ScreenHeight()-20, "Use left and right mouse button to toggle tiles.", olc::BLACK);
      DrawString(4, ScreenHeight()-10, "Use number keys 1-7 to toggle layout.", olc::BLACK);
      
      // Graceful exit if user is in full screen mode
      return !GetKey(olc::Key::ESCAPE).bPressed;
      
  }
};


int main()
{
  IsometricTilesHexagon demo;
  if (demo.Construct(600, 400, 1, 1))
    demo.Start();
  return 0;
}
