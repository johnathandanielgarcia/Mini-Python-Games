import pygame
import random

# 20x16 grid , 32x32 squares 
# mole drawn in top left square 
# when clicked, move to diff square

# creates a 20x26 grid by increasing in increments of 32 to evenly divide screen
def create_20x16_grid(screen):
    for x in range(0, 640, 32):
        pygame.draw.line(screen, (0, 0, 0), (x, 0), (x, 512))
    for y in range(0, 512, 32):
        pygame.draw.line(screen, (0, 0, 0), (0, y), (640, y))

# gets random x and y for the mole when clicked
def new_pos():
    x = random.randrange(0, 20) * 32
    y = random.randrange(0, 16) * 32
    return x,y

def main():
    try:
        pygame.init()           # init game
        mole_image = pygame.image.load("mole.png")          # load mole image
        mole_pos = (0, 0)           # start @ top left
        screen = pygame.display.set_mode((640, 512))            # draw screen 


        clock = pygame.time.Clock()         # start clock
        running = True          # flag
        while running:
            for event in pygame.event.get():            # continuosuly listen for events 
                if event.type == pygame.QUIT:           # if x out
                    running = False         # quit
                elif event.type == pygame.MOUSEBUTTONDOWN:          # if mouse pressed 
                    if pygame.Rect(mole_pos, (32, 32)).collidepoint(event.pos): # and if pressed on mole
                        mole_pos = new_pos()            # then create new pos for mole 

            # game aesthetics (screen color, draw grid, mole, display )
            screen.fill("light green")  
            create_20x16_grid(screen)
            screen.blit(mole_image, mole_image.get_rect(topleft=(mole_pos)))
            pygame.display.flip()
            clock.tick(60)
    finally:
        pygame.quit()


if __name__ == "__main__":
    main()
