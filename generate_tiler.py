import pygame

tilemap = pygame.image.load("blocks-withsurroundings.png")

with open("tiler.c", "w") as output:
 output.write("""#include <stdlib.h>
 
/*
  tiler.c - From Caroline's color cell compression
  
  This piece of software finds the optimal tile to represent a 6x6 RGB image, together with both the first and the second colors inside that tile.
  The 183 possible tiles are generated from the "blocks-withsurroundings.png" by the "generate_tiler.py"
  
  This code was generated from "generate_tiler.py" - please don't modify this directly!
*/

""")

 for skipverti in (-1,0,1):
  for skiphori in (-1,0,1):
   func_name={
    (-1,-1): "_top_left",
    ( 0,-1): "_top",
    ( 1,-1): "_top_right",
    (-1, 0): "_right",
    ( 0, 0): "",
    ( 1, 0): "_left",
    (-1, 1): "_bottom_left",
    ( 0, 1): "_bottom",
    ( 1, 1): "_bottom_right",
    }[(skiphori,skipverti)]
   yr= skipverti!=0
   xr= skiphori!=0
   tilew=6-xr
   tileh=6-yr
   yt=skipverti==-1
   xt=skiphori==-1

   output.write("unsigned char find_tile%s(\n\tunsigned char* redplane, unsigned char* greenplane, unsigned char* blueplane, /* The input planes, as %dx%d byte buffers */\n\tunsigned char* bred, unsigned char* bgreen, unsigned char* bblue, /* The output red, green and blue, for when the tile's pixel is 0 */\n\tunsigned char* wred, unsigned char* wgreen, unsigned char* wblue /* The output red, green and blue, for when the tile's pixel is 1 */\n\t){\n\tunsigned int score;\n\tunsigned int bestscore=0xfffffff;\n"%(func_name,tilew,tileh))
   for variable in ("besttile", "tbred", "tbgreen", "tbblue", "twred", "twgreen", "twblue"):
    output.write("\tunsigned char %s;\n"%variable)
   output.write("\n")
   #for tilenum in range(0x39,255):
   for tilenum in range(0,62):
    black_pos=[]
    white_pos=[]
    for y in range(6-yr):
     for x in range(6-xr):
      color = tilemap.get_at(((tilenum&15)*6+x+xt, (tilenum>>4)*6+y+yt))
      if color.g<64:
       black_pos.append((x,y))
      elif color.g>192:
       white_pos.append((x,y))
    output.write("\t/* Tile 0x%02x */\n\n"%tilenum)
    output.write("\ttbred   = (" + "+".join(map(lambda pos: "redplane[%s]"%(pos[1]*tilew+pos[0]), black_pos)) + ") /%d;\n"%len(black_pos))
    output.write("\ttbgreen = (" + "+".join(map(lambda pos: "greenplane[%s]"%(pos[1]*tilew+pos[0]), black_pos)) + ") /%d;\n"%len(black_pos))
    output.write("\ttbblue  = (" + "+".join(map(lambda pos: "blueplane[%s]"%(pos[1]*tilew+pos[0]), black_pos)) + ") /%d;\n"%len(black_pos))
    output.write("\tscore   = " + "+".join(map(lambda pos: "abs(redplane[%s]-tbred)+abs(greenplane[%s]-tbgreen)+abs(blueplane[%s]-tbblue)"%(pos[1]*6+pos[0],pos[1]*6+pos[0],pos[1]*6+pos[0]), black_pos)) + "/%d;\n\n"%len(black_pos))
    if len(white_pos)>0:
        output.write("\ttwred   = (" + "+".join(map(lambda pos: "redplane[%s]"%(pos[1]*tilew+pos[0]), white_pos)) + ") /%d;\n"%len(white_pos))
        output.write("\ttwgreen = (" + "+".join(map(lambda pos: "greenplane[%s]"%(pos[1]*tilew+pos[0]), white_pos)) + ") /%d;\n"%len(white_pos))
        output.write("\ttwblue  = (" + "+".join(map(lambda pos: "blueplane[%s]"%(pos[1]*tilew+pos[0]), white_pos)) + ") /%d;\n"%len(white_pos))
        output.write("\tscore  += " + "+".join(map(lambda pos: "abs(redplane[%s]-twred)+abs(greenplane[%s]-twgreen)+abs(blueplane[%s]-twblue)"%(pos[1]*6+pos[0],pos[1]*6+pos[0],pos[1]*6+pos[0]), white_pos)) + "/%d;\n\n"%len(white_pos))
    else:
        output.write("\ttwred=twgreen=twblue=0;\n")
    output.write("\tif(score<bestscore){\n")
    output.write("\t\tbesttile  = 0x%02x;                       bestscore = score;\n"%tilenum)
    output.write("\t\t*bred     = tbred; *bgreen   = tbgreen; *bblue    = tbblue;\n")
    output.write("\t\t*wred     = twred; *wgreen   = twgreen; *wblue    = twblue;\n")
    output.write("\t}\n\n")
   output.write("\treturn besttile;\n}\n")
 
