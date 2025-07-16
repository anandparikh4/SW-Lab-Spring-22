from my_package.model import InstanceSegmentationModel
from my_package.data.dataset import Dataset
from my_package.analysis.visualize import plot_visualization
from my_package.data.transforms.flip import FlipImage
from my_package.data.transforms.blur import BlurImage
from my_package.data.transforms.crop import CropImage
from my_package.data.transforms.rotate import RotateImage
from my_package.data.transforms.rescale import RescaleImage
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

def experiment(annotation_file, segmentor, transforms, outputs):
    '''
        Function to perform the desired experiments

        Arguments:
        annotation_file: Path to annotation file
        segmentor: The image segmentor
        transforms: List of transformation classes
        outputs: path of the output folder to store the images
    '''

    #Create the instance of the dataset.
    dataset = Dataset(annotation_file , transforms)

    print("Working...1")   # just a check that the code runs fine upto this moment

    # Iterate over all data items.
    for i in range(len(dataset)) :

        # use getitem dunder method to get the data
        new_dict = dataset[i]

        # Get the predictions from the segmentor
        pred = segmentor(new_dict["image"])

        # Draw the segmentation maps on the image and save them
        plot_visualization(pred, new_dict["image"], 0.45,0.45,0.45,True,i,False,outputs)

    print("Working...2")



def analysis(annotation_file,image_file, segmentor,outputs):  #the analysis experiment on img 7.jpg

    im = Image.open(image_file)

    transforms = [RotateImage(0),FlipImage("horizontal"),BlurImage(2),RescaleImage((2*im.width,2*im.height)),RescaleImage((0.5*im.width,0.5*im.height)),RotateImage(-90),RotateImage(45)]
    titles = ["Original Image" , "Horizontally flipped" , "Blurred" , "Twice rescaled" , "Half rescaled" , "90 degrees clockwise" , "45 degrees anticlockwise" ]

    plt.axis("off")
    for i in range(7) :
        dataset = Dataset(annotation_file , [transforms[i]])        # make new data set object with one transform each time

        new_dict = dataset[7]                   # perform it on image 7

        pred = segmentor(new_dict["image"])     # get the predictions

        im = plot_visualization(pred , new_dict["image"] , 0.45,0.45,0.45,False,i,True,outputs)  # plot the visualisation

        plt.subplot(2,4,i+1).set_title(titles[i])  # using matplotlib subplots, we can plot all images together for comparison
        plt.axis("off")
        plt.imshow(im)

    plt.savefig(outputs+"/together.jpg")  # save the figure and show the output
    plt.show()


def main():
    segmentor = InstanceSegmentationModel()

    outputs = "./outputs/original"      # segmented images for original ones
    experiment('./data/annotations.jsonl', segmentor, [], outputs)     # so transformed list is empty

    outputs = "./outputs/transformed"   # for transformed ones, so you can customise transforms on ALL images
    experiment('./data/annotations.jsonl', segmentor, [FlipImage("vertical"), BlurImage(1) , RotateImage(10) , CropImage((400,800),"center") , RescaleImage((560,800))], outputs) # Sample arguments to call experiment()

    outputs = "./outputs/my_analysis"   # for analysis experiment
    analysis('./data/annotations.jsonl',"./data/imgs/7.jpg",segmentor,outputs)    # roll no. 20CS10007

    # in each of the above, pass the outputs folder location, to save it there

if __name__ == '__main__':
    main()


# jpg_arr = new_dict["image"] * 255
# jpg_arr = np.transpose(jpg_arr , (1,2,0))                                      # convert numpy array to PIL image, test for testing purposes
# PIL_image = Image.fromarray(jpg_arr.astype('uint8'), 'RGB').show()