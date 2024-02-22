import os
from PIL import Image

# filename_in = sys.argv[1]
# filename_out = sys.argv[2]  # future use

filenames_in = sorted(os.listdir('input'))

output_file = open('incl_out.ino', 'w')

pic_counter = 0

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

for filename in filenames_in:
    
    output_file.write('// ' + filename + '\n')
    
    filename_in = 'input/' + filename
    filename_no_extension = filename.split('.')[0]
    
    try:
        texture = Image.open(filename_in)
    except FileNotFoundError:
        print('file not found')
        exit(1)
    
    if texture.size == (32, 16):
            
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
                
        rh_eye_str = form_data_str(righthand_eye_pixels_arr)
        lh_eye_str = form_data_str(lefthand_eye_pixels_arr)
        center_data_str = form_data_str(center_pixels_arr)

        output_file.write('// righthand eye\n' + f'uint8_t {filename_no_extension}_rightEye[64][3] = ' + rh_eye_str)
        output_file.write('\n\n')
        output_file.write('// lefthand eye\n' + f'uint8_t {filename_no_extension}_leftEye[64][3] = ' + lh_eye_str)
        output_file.write('\n\n')
        output_file.write('// center\n' + f'uint8_t {filename_no_extension}_center[256][3] = ' + center_data_str)
        output_file.write('\n\n')

    elif texture.size == (8, 8):
        
        eye_pixels_arr = []
        
        # right-hand eye
        even_line = True    # run forwards on even lines, reverse on odd lines
        for x in range(0, 8):
            
            if even_line:
                for y in range(0, 8):
                    eye_pixels_arr.append(texture.getpixel((x,y))[:3])
                even_line = False
                
            else:
                for y in range(7, -1, -1):
                    eye_pixels_arr.append(texture.getpixel((x,y))[:3])
                even_line = True
                
        eye_str = form_data_str(eye_pixels_arr)
                
        if 'left' in filename:
            output_file.write('// lefthand eye\n' + f'uint8_t {filename_no_extension}_leftEye[64][3] = ' + eye_str)
            output_file.write('\n\n')
            
        elif 'right' in filename:
            output_file.write('// righthand eye\n' + f'uint8_t {filename_no_extension}_rightEye[64][3] = ' + eye_str)
            output_file.write('\n\n')
            
        else:
            print("8x8 with no left/right inidcator, skipping...")      
    
    elif texture.size == (32, 8):
        
        center_pixels_arr = []
        
        even_line = True    # run forwards on even lines, reverse on odd lines
        for x in range(0, 32):
            
            if even_line:
                for y in range(0, 8):
                    center_pixels_arr.append(texture.getpixel((x,y))[:3])
                even_line = False
                
            else:
                for y in range(7, -1, -1):
                    center_pixels_arr.append(texture.getpixel((x,y))[:3])
                even_line = True
                
        center_data_str = form_data_str(center_pixels_arr)
        output_file.write('// center\n' + f'uint8_t {filename_no_extension}_center[256][3] = ' + center_data_str)
        output_file.write('\n\n')
    
    pic_counter += 1
        

print(f'processed {pic_counter} images')

output_file.close()