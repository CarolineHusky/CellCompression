import pygame

image = pygame.image.load("out.png")
tiles = pygame.image.load("blocks-withsurroundings.png")

with open("tiles.bin","rb") as data:
    d=data.read()
    output=pygame.Surface((image.get_width()*4,image.get_height()*2))

    for y in range(output.get_height()//4):
        for x in range(output.get_width()//4):
            try:
                tile = d[x+y*(output.get_width()//4)-1]
                kx = tile&15
                ky = tile>>4
                for ry in range(4):
                    for rx in range(4):
                        color = tiles.get_at((1+rx+kx*6,1+ry+ky*6))
                        if color.g>128:
                            output.set_at((x*4+rx,y*4+ry),image.get_at((x,y+image.get_height()//2)))
                        else:
                            output.set_at((x*4+rx,y*4+ry),image.get_at((x,y)))
            except IndexError:
                pass
                        
pygame.image.save(output,"demo.png")
