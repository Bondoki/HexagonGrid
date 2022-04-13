

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unordered_set>
#include <set>
#include <list>
#include <unordered_map>

#include "Hex.h"

// Override base class with your custom functionality
class IsometricTilesHexagon : public olc::PixelGameEngine
{
public:
	IsometricTilesHexagon()
	{
		sAppName = "Isometric Hexagon Tiles";
	}

private:
        float fTotalElapsedTime;
  
	// Number of tiles in world
	olc::vi2d vWorldSize = { 2, 2 };

	// Size of single tile graphic
	olc::vi2d vTileSize = { 72, 46 };
        
        // Size of single tile graphic and offset to align them
	olc::vi2d vTileSizeOffset = { 0, 10 };

	// Where to place tile (0,0) on screen (in tile size steps)
	//olc::vi2d vOrigin = { 1, 2 };
        
        // Where to place tile (0,0) on screen (in screen pixel steps)
        olc::vi2d vOriginScreen = { 72*3, 46*3 };

	// Sprite that holds all imagery
	olc::Sprite *sprIsom = nullptr;

        // list to order for drawing
	std::list<Hex> hexlist;
        
        // HexTile to Value
        using HexMap=std::unordered_map<Hex, int,decltype(hexhash), decltype(hexequal)>;
        HexMap heights{HexMap(10,hexhash,hexequal)};
        
        

public:
	bool OnUserCreate() override
	{
                fTotalElapsedTime=0.0f;
                /* initialize random seed: */
                srand (time(NULL));

		// Load sprites used in demonstration
		sprIsom = new olc::Sprite("assets/hextiles.png");

		createBoard(2);
               
		return true;
	}
	
	void createBoard(int nr)
        {
          //clear the list and the map
          hexlist.clear();
          heights.clear();
          
          int dist = 4;
          
          switch(nr)
          {
            case 0: //Rectangle 1
                    for (int q = -3; q <= 3; q++)
                    { // flat top
                      int q_offset = floor(q/2.0); // or q>>1
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
                    
                    break;
    
            case 2: //Hexagon
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
          vOriginScreen.x= 300-vTileSize.x/2;
          vOriginScreen.y= 200-vTileSize.y/2;
          
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
		
		// Work out active cell
		//olc::vi2d vCell = { vMouse.x / vTileSize.x, vMouse.y / vTileSize.y };

		// Work out mouse offset into cell
		//olc::vi2d vOffset = { vMouse.x % vTileSize.x, vMouse.y % vTileSize.y };

		// Sample into cell offset colour
		//olc::Pixel col = sprIsom->GetPixel(3 * vTileSize.x + vOffset.x, vOffset.y);

		// Work out selected cell by transforming screen cell
		
			//(vCell.y - vOrigin.y) + (vCell.x - vOrigin.x),
			//(vCell.y - vOrigin.y) - (vCell.x - vOrigin.x) 
			//(-1./3 * point.x  +  sqrt(3)/3 * point.y) / size
			//q,r
                         //std::floor(4.0 / 3.0 * ((1.0*vMouse.x-vOriginScreen.x) / (vTileSize.x-vTileSizeOffset.x))) ,
                         //std::floor(-2.0 / 3.0 * ((1.0*vMouse.x-vOriginScreen.x) / (vTileSize.x-vTileSizeOffset.x)) + 2.0*1.7320508075688/3.0 * ((1.0*vMouse.y-vOriginScreen.y+vTileSizeOffset.y) / (vTileSize.y-vTileSizeOffset.y)))
		
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

		// "Bodge" selected cell by sampling corners
		//if (col == olc::RED) vSelected += {-1, +0};
		//if (col == olc::BLUE) vSelected += {+0, -1};
		//if (col == olc::GREEN) vSelected += {+0, +1};
		//if (col == olc::YELLOW) vSelected += {+1, +0};

		// Handle mouse click to toggle if a tile is visible or not
		if (GetMouse(0).bPressed)
		{
			// Guard array boundary
                         auto got = heights.find (Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y));
                  
                        if ( got != heights.end() )
			//if (vSelected.x >= 0 && vSelected.x < vWorldSize.x && vSelected.y >= 0 && vSelected.y < vWorldSize.y)
                        {
				
                                //heights.at(Hex(x,y,-x-y))++
                                //++heights.at(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y)) %=6;
                                int value = heights.at(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y));
                                
                                value = (value%9)+1;
                                heights.at(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y)) = value;
                                
                                //heights.at(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y))++;
                                //heights.at(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y)) %=7;
                                //heights.at(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y))++;
                                //std::cout << "modify" << std::endl;
                        }
                        
		}
                
                // Handle mouse click to toggle tile - right click
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
                /*3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0)
                Point hex_to_pixel(Layout layout, Hex h) {
    const Orientation& M = layout.orientation;
    double x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x;
    double y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y;
    return Point(x + layout.origin.x, y + layout.origin.y);*/


		
		// Draw World - has binary transparancy so enable masking
		//SetPixelMode(olc::Pixel::MASK);
                SetPixelMode(olc::Pixel::ALPHA);
		// (0,0) is at top, defined by vOrigin, so draw from top to bottom
		// to ensure tiles closest to camera are drawn last
		/*for (int y = 0; y < vWorldSize.y; y++)
		{
			for (int x = 0; x < vWorldSize.x; x+=2)
			{
				// Convert cell coordinate to world space
				olc::vi2d vWorld = ToScreen(x, y);
				
				switch(pWorld[y*vWorldSize.x + x])
				{
				case 0:
					// Invisble Tile
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 1:
					// Visible Tile
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 2:
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 2 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 3:
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 0 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
				case 4:
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
				case 5:
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 2 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
				}			
			}
			
			for (int x = 1; x < vWorldSize.x; x+=2)
			{
				// Convert cell coordinate to world space
				olc::vi2d vWorld = ToScreen(x, y);
				
				switch(pWorld[y*vWorldSize.x + x])
				{
				case 0:
					// Invisble Tile
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 1:
					// Visible Tile
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 2:
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 2 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 3:
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 0 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
				case 4:
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
				case 5:
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 2 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
				}			
			}
		}
		*/
                /*
                for (int r = 0; r < vWorldSize.y; r++) {
                   for (int q = 0; q < vWorldSize.x; q++) {
                    olc::vi2d vWorld = AxialToScreen(q, r);
                    DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					
                    
                  }
                }
                */
                
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
					// Invisble Tile
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 1:
					// Visible Tile
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 2:
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 2 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
                                case 3:
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 3 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
                                case 4:
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 4 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
                                        
				case 5:
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 0 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
				case 6:
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 1 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
				case 7:
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 2 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
                                case 8:
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 3 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
                                        
                                 case 9:
					DrawPartialSprite(vWorld.x+moveXSelected, vWorld.y+moveYSelected, sprIsom, 4 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y);
					break;
                                
				}
                  }
                  
                  // Draw selected
                {
                  
                  if ( hexequal(Hex(vSelected.x,vSelected.y,-vSelected.x-vSelected.y), Hex(Hex(q,r,s))) )
                  {
                      olc::vi2d vWorld = AxialToScreen(vSelected.x,vSelected.y);
                      
                      
                      // Invisble Tile
                      //DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
                      DrawPartialSprite(vWorld.x, vWorld.y+5*std::sin(fTotalElapsedTime*5), sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
                      
                    
                  }
                }
                }
                
                
                /*
                for ( auto it=hexmap.cbegin(); it!=hexmap.cend(); ++it )
                {
                  olc::vi2d vWorld = AxialToScreen((*it).get_q(), (*it).get_r());
                  DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);	
                }
                
                */
                /*
                
                for ( auto it=hexset.cbegin(); it!=hexset.cend(); ++it )
                {
                  olc::vi2d vWorld = AxialToScreen((*it).get_q(), (*it).get_r());
                  DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);	
                }
                */
		// Draw Selected Cell - Has varying alpha components
		//SetPixelMode(olc::Pixel::ALPHA);

		// Convert selected cell coordinate to world space
		//olc::vi2d vSelectedWorld = ToScreen(vSelected.x, vSelected.y);

		// Draw "highlight" tile
		//DrawPartialSprite(vSelectedWorld.x, vSelectedWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);

		// Go back to normal drawing with no expected transparency
		SetPixelMode(olc::Pixel::NORMAL);

		// Draw Hovered Cell Boundary
		//DrawRect(vCell.x * vTileSize.x, vCell.y * vTileSize.y, vTileSize.x, vTileSize.y, olc::RED);
				
		// Draw Debug Info
		DrawString(4, 4, "Mouse   : " + std::to_string(vMouse.x) + ", " + std::to_string(vMouse.y), olc::BLACK);
		DrawString(4, 24, "Selected: " + std::to_string(vSelected.x) + ", " + std::to_string(vSelected.y), olc::BLACK);
		
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
