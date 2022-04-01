# CellCompression
Image / video compression technique using 1bpp 4x4 cells, see https://en.wikipedia.org/wiki/Color_Cell_Compression

Compresses any 4x4 pixel block in 20 bits, with an additional 128 colour table per image

Proposed block format: 
 * 6 bits: the tile
 * 7 bits: the foreground colour for the tile
 * 7 bits: the background colour for the tile

Proposed palette format:
 * 124 colours: the colours for the image
 * 4 colours: reserved for the UI and stuff

## The tiles:
![Tilemap](/blocks-withsurroundings.png)

The current implementation of the encoder does take into account surrounding pixels around the block for cleaner results, hence the 6x6 tiles in the input.
Inside the blockmap, black pixels get mapped to colour 1, white pixels to colour 2, and grey pixels are ignored during generation of the tile

## How to use

### Requirements
* python and pygame for `generate_tiler.py` and `demo.py`
* C compiler for the encoder

### To compile the encoder
* first generate the `tiler.c` file using `generate_tiler.py`
* then compile `main.c` using your favourite compiler

### To run
* convert your image to P6 PPM
* run the encoder on your image

### To see the result
* convert the resulting out.pnm to a format supported by pygame (e.g. PNG)
* run `demo.py`, then check the resulting `demo.png`
