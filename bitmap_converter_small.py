import sys
from PIL import Image

filename_in = sys.argv[1]
filename_out = sys.argv[2]  # future use

try:
    texture = Image.open(filename_in)
except FileNotFoundError:
    print('file not found')
    exit(1)
    
if texture.size != (32, 16):
    print('incorrect image size')
    exit(1)
    
righthand_eye_pixels_arr = []
lefthand_eye_pixels_arr = []
center_pixels_arr = []

# right-hand eye
even_line = True    # run forwards on even lines, reverse on odd lines
for x in range(0, 8):
    
    if even_line:
        for y in range(0, 8):
            righthand_eye_pixels_arr.append(texture.getpixel((x,y))[:3])
        even_line = False
        
    else:
        for y in range(7, -1, -1):
            righthand_eye_pixels_arr.append(texture.getpixel((x,y))[:3])
        even_line = True
        
# left-hand eye
even_line = True    # run forwards on even lines, reverse on odd lines
for x in range(24, 32):
    
    if even_line:
        for y in range(0, 8):
            lefthand_eye_pixels_arr.append(texture.getpixel((x,y))[:3])
        even_line = False
        
    else:
        for y in range(7, -1, -1):
            lefthand_eye_pixels_arr.append(texture.getpixel((x,y))[:3])
        even_line = True
        
# center
even_line = True    # run forwards on even lines, reverse on odd lines
for x in range(0, 32):
    
    if even_line:
        for y in range(8, 16):
            center_pixels_arr.append(texture.getpixel((x,y))[:3])
        even_line = False
        
    else:
        for y in range(15, 7, -1):
            center_pixels_arr.append(texture.getpixel((x,y))[:3])
        even_line = True
        

def form_data_str(pixels_arr): 

    data_str = '{'

    i_counter = 0

    for i in pixels_arr:
        
        pixel_str = '{'
        j_counter = 0
        
        for j in i:
            pixel_str += str(j)
            
            if j_counter < len(i)-1:
                pixel_str += ','

            j_counter += 1
            
        pixel_str += '}'
        data_str += pixel_str
            
        if i_counter < len(pixels_arr)-1:
            data_str += ','
        
        i_counter += 1
            
    data_str += '}'
    
    return data_str

rh_eye_str = form_data_str(righthand_eye_pixels_arr)
lh_eye_str = form_data_str(lefthand_eye_pixels_arr)
center_data_str = form_data_str(center_pixels_arr)

print(rh_eye_str)
print('\n\n', end=None)
print(lh_eye_str)
print('\n\n', end=None)
print(center_data_str)
