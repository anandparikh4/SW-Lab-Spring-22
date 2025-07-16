from PIL import Image

class RotateImage(object):
    '''
        Rotates the image about the centre of the image.
    '''

    def __init__(self, degrees):
        '''
            Arguments:
            degrees: rotation degree.
        '''

        # Write your code here
        self.degrees = int(degrees)

    def __call__(self, image):
        '''
            Arguments:
            image (numpy array or PIL image)

            Returns:
            image (numpy array or PIL image)
        '''

        # Write your code here
        return image.rotate(self.degrees,Image.NEAREST,expand = 1)  # expansion is done so that after rotating the image does not go out of the array and
                                                                    # so that we do not lose any pixels


'''
im = Image.open("tree.jpg")
Rotate = RotateImage(-90)  # driver code
new_im = Rotate.__call__(im)
im.show()
new_im.show()
'''



