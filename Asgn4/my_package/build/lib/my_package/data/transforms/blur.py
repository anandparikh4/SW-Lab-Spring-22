from PIL import ImageFilter

class BlurImage(object):
    '''
        Applies Gaussian Blur on the image.
    '''

    def __init__(self, radius):
        '''
            Arguments:
            radius (int): radius to blur
        '''

        self.radius = int(radius)
        if(self.radius < 0) :   # safeguard
            self.radius = 0

    def __call__(self, image):
        '''
            Arguments:
            image (numpy array or PIL Image)

            Returns:
            image (numpy array or PIL Image)
        '''

        # Write your code here
        return image.filter(ImageFilter.GaussianBlur(int(self.radius)))      # simply return the blurred image


'''
im = Image.open("tree.jpg")
r = 20
Blur = BlurImage(r)           #driver code 
new_im = Blur.__call__(im)
im.show()
new_im.show()
'''