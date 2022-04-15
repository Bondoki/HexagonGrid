# Hexagonal Grid Visualization
Rendering of the hexagon tiles in various layout to emphasize pseudo 2.5 view. This program provides routine for selecting and creation of hex tiles.
It's just for fun and educational purpose. Feel free to modify, improve, and use it - also the assets :)
 
![HexBoard](https://user-images.githubusercontent.com/16195016/163599250-94d6505c-51d0-438e-847d-4195a85acc37.gif)
![Hexagon](https://user-images.githubusercontent.com/16195016/163599305-242df584-6004-4bd5-907b-3847e55550b3.gif)
 
## Installation
Just do for standard compilation (tested with gcc 8.2.1):

````sh
    # generates the application
    g++ -o IsometricTilesHexagon IsometricTilesHexagon.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17 -Wno-narrowing
````

## Usage
Simply use the mouse to hover on the board.
Use the left mouse button to toggle the tile style.
Use the right mouse button to toggle the tile on/off.
Use the number keys 1-7 to select board style.
ESC : quit


## References, License, Credit
The program acknowledge the libraries and refer to the appropriate licenses
* [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) by [Javidx9](https://github.com/OneLoneCoder)
* [Hex](https://www.redblobgames.com/grids/hexagons/codegen/output/lib.cpp) by [redblobgames](https://www.redblobgames.com)
* Other pieces of codes are released under [Creative Commons Zero v1.0 Universal](https://github.com/Bondoki/HexagonGrid/blob/main/LICENSE) for free usage.

## Acknowledgment
Inspired by the [Coding Quickie: Isometric Tiles](https://www.youtube.com/watch?v=ukkbNKTgf5U) of [Javidx9](https://github.com/OneLoneCoder) gave the idea for this.
The code for the implementing hex tiles is heavly based on the the very good [hexagonal grid tutorial](https://www.redblobgames.com/grids/hexagons) by [redblobgames](https://www.redblobgames.com).
