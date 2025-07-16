import random

class CropImage(object):
    '''
        Performs either random cropping or center cropping.
    '''

    def __init__(self, shape, crop_type="center"):
        '''
            Arguments:
            shape: output shape of the crop (h, w)
            crop_type: center crop or random crop. Default: center
        '''

        # Write your code here
        self.height = shape[0]
        self.width = shape[1]
        self.type = crop_type

    def __call__(self, image):
        '''
            Arguments:
            image (numpy array or PIL image)

            Returns:
            image (numpy array or PIL image)
        '''

        # Write your code here

        if self.width > image.width:             # safeguards
            self.width = image.width

        if self.height > image.height:
            self.height = image.height

        if self.type == "random":
            x1 = random.random() * (image.width - self.width)     # randomly pick x1 and y1 in the possible range and then calculate x2 and y2 accordingly
            x2 = x1 + self.width
            y1 = random.random() * (image.height - self.height)
            y2 = y1 + self.height
            return image.crop((int(x1),int(y1), int(x2), int(y2)))

        else :
            xcenter = image.width / 2                  # find center and get the desired co-ordinates
            ycenter = image.height / 2
            x1 = xcenter - self.width / 2
            y1 = ycenter - self.height/ 2
            x2 = xcenter + self.width / 2
            y2 = ycenter + self.height / 2
            return image.crop((int(x1), int(y1), int(x2), int(y2)))        # in both cases just return the cropped PIL image

'''
im = Image.open("tree.jpg")
Crop = CropImage((im.height/2 , im.width/2) , "random")              #driver code 
new_im = Crop.__call__(im)
im.show()
new_im.show()
'''