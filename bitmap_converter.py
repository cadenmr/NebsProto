# convert bitmap for use w/ neopixels for protogen head

# looking from inside the visor, NP chain is:
# LEFTHAND SCREEN -> CENTER SCREEN -> RIGHTHAND SCREEN

# PIXEL INDEX STARTS AT BOTTOM LEFT CORNER, MOVES UPWARD IN Y DIR

import sys
from PIL import Image

filename = sys.argv[1]

try:
    texture = Image.open(filename)
except FileNotFoundError:
    print('file not found')
    quit(1)

if texture.size != (32, 16):
    print(f'image size incorrect: was {texture.size}, must be (32, 16)')
    quit(1)

lefthand_eye_pixels = []
righthand_eye_pixels = []
center_pixels = []

# LEFTHAND EYE
for x in range(24, 32):
    for y in range(7, -1, -1):
        lefthand_eye_pixels.append(texture.getpixel((x,y))[:3])



# RIGHTHAND EYE
for x in range(0, 8):
    for y in range(7, -1, -1):
        righthand_eye_pixels.append(texture.getpixel((x,y))[:3])

# CENTER
for x in range(0, 32):
    for y in range(15, 6, -1):
        #print(x,y)
        pass

# print(lefthand_eye_pixels)

screen_str = '{'

i_ct = 0

print(f'{len(lefthand_eye_pixels)}')
print(f'{len(righthand_eye_pixels)}')
print(f'{len(center_pixels)}')

for i in lefthand_eye_pixels:

    pixel_str = '{'
    j_ct = 0

    for j in i:
        pixel_str += str(j)
        j_ct += 1
        
        if j_ct < 3:
            pixel_str += ','

    pixel_str += '}'

    screen_str += pixel_str
    i_ct += 1
    if i_ct < 64:
        screen_str += ','

screen_str += '}'
print(screen_str)