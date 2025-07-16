from PIL import Image

class FlipImage(object):
    '''
        Flips the image.
    '''

    def __init__(self, flip_type="horizontal"):
        '''
            Arguments:
            flip_type: 'horizontal' or 'vertical' Default: 'horizontal'
        '''

        # Write your code here
        self.type = flip_type

    def __call__(self, image):
        '''
            Arguments:
            image (numpy array or PIL image)

            Returns:
            image (numpy array or PIL image)
        '''

        # Write your code here
        if self.type == "vertical":
            return image.transpose(Image.FLIP_TOP_BOTTOM)      # simply return flipped images

        else:
            return image.transpose(Image.FLIP_LEFT_RIGHT)


'''
im = Image.open("tree.jpg")
Flip = FlipImage("vertical")             # driver code
new_im = Flip.__call__(im)
im.show()
new_im.show()
'''

