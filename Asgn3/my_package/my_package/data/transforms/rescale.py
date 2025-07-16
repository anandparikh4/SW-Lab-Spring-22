
class RescaleImage(object):
    '''
        Rescales the image to a given size.
    '''

    def __init__(self, output_size):
        '''
            Arguments:
            output_size (tuple or int): Desired output size. If tuple, output is
            matched to output_size. If int, smaller of image edges is matched
            to output_size keeping aspect ratio the same.
        '''

        # Write your code here

        if type(output_size) is int:        # set the data according to class
            self.size = output_size

        else :
            self.size = -1
            self.dimensions = output_size

    def __call__(self, image):
        '''
            Arguments:
            image (numpy array or PIL image)

            Returns:
            image (numpy array or PIL image)

            Note: You do not need to resize the bounding boxes. ONLY RESIZE THE IMAGE.
        '''

        # Write your code here
        if self.size == -1 :
            int_dimensions = (int(self.dimensions[0]) , int(self.dimensions[1]))
            return image.resize(int_dimensions)

        else :

            if(image.width < image.height) :                # set the smaller value to input size and scale the other one maintaining the ratio
                width = int(self.size)
                height = int(width / image.width * image.height)
                return image.resize((width , height))

            else :
                height = int(self.size)
                width =  int(height / image.height * image.width)
                return image.resize((width , height))


'''
im = Image.open("tree.jpg")
Rescale = RescaleImage((600,600))        # driver code
print(im.height)
print(im.width)
new_im = Rescale.__call__(im)
im.show()
new_im.show()
'''



