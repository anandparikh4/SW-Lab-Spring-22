import json
from PIL import Image
import numpy as np

class Dataset(object):
    '''
        A class for the dataset that will return data items as per the given index
    '''

    def __init__(self, annotation_file, transforms=None):
        '''
            Arguments:
            annotation_file: path to the annotation file
            transforms: list of transforms (class instances)
                        For instance, [<class 'RandomCrop'>, <class 'Rotate'>]
        '''

        self.file_address = annotation_file
        self.transforms = transforms

    def __len__(self):
        '''
            return the number of data points in the dataset
        '''

        jsonFile = open(self.file_address, "r")          # open file
        json_list = list(jsonFile)               # convert to list, where each element is a line of the json file

        return len(json_list)       # return length of list

    def __getitem__(self, idx):
        '''
            return the dataset element for the index: "idx"
            Arguments:
                idx: index of the data element.

            Returns: A dictionary with:
                image: image (in the form of a numpy array) (shape: (3, H, W))
                gt_png_ann: the segmentation annotation image (in the form of a numpy array) (shape: (1, H, W))
                gt_bboxes: N X 5 array where N is the number of bounding boxes, each
                            consisting of [class, x1, y1, x2, y2]
                            x1 and x2 lie between 0 and width of the image,
                            y1 and y2 lie between 0 and height of the image.

            You need to do the following,
            1. Extract the correct annotation using the idx provided.
            2. Read the image, png segmentation and convert it into a numpy array (wont be necessary
                with some libraries). The shape of the arrays would be (3, H, W) and (1, H, W), respectively.
            3. Scale the values in the arrays to be with [0, 1].
            4. Perform the desired transformations on the image.
            5. Return the dictionary of the transformed image and annotations as specified.
        '''

        jsonFile = open(self.file_address, "r")          # open file
        json_list = list(jsonFile)               # convert to list

        if idx<0 or idx>9 :       # safeguard, you can set any default value
            idx = 6

        json_obj = json.loads(json_list[idx])    # make a json object to store the individual strings as dictionaries

        jpg_image = Image.open("C:/Users/Admin/Desktop/Sem 4/Software Lab/Assignment 3 (Python)/CS29006_SW_Lab_Spr2022-master/CS29006_SW_Lab_Spr2022-master/Python_DS_Assignment/data/{}".format(json_obj["img_fn"]))
        png_image = Image.open("C:/Users/Admin/Desktop/Sem 4/Software Lab/Assignment 3 (Python)/CS29006_SW_Lab_Spr2022-master/CS29006_SW_Lab_Spr2022-master/Python_DS_Assignment/data/{}".format(json_obj["png_ann_fn"]))

        for class_x in self.transforms :       # perform transformations

            obj_1 = class_x
            jpg_image = obj_1(jpg_image)

            obj_2 = class_x
            png_image = obj_2(png_image)

        jpg_arr = np.array(jpg_image)       # convert both images to numpy arrays as directed
        jpg_np_arr = np.transpose(jpg_arr, (2, 0, 1))  # convert the dimensions of the array to be (3 , H , W)

        png_arr = np.array(png_image)       # already in (H , W) format
        png_np_arr = np.array([png_arr])    # so just add a redundant dimension like this to make it (1 , H , W)

        jpg_np_arr = jpg_np_arr / 255   # scale both their contents down to range [0,1]
        png_np_arr = png_np_arr / 255

        # jpg_image.show()
        # png_image.show()
        # print(jpg_np_arr.shape)
        # print(png_np_arr.shape)

        b_box_list = json_obj["bboxes"]

        b_box_array = np.zeros((len(b_box_list) , 5))    # make an array of N X 5 with all zeros

        i = 0
        for b_box in b_box_list :           # iterate over all bboxes
            x1 = b_box["bbox"][0]           # extract required data
            y1 = b_box["bbox"][1]
            x2 = x1 + b_box["bbox"][2]
            y2 = y1 + b_box["bbox"][3]
            category_id = b_box["category_id"]
            new_row = [category_id,x1,y1,x2,y2]
            b_box_array[i] = new_row        # copy the new row of data to the N X 5 array
            i = i+1

        return { "image" : jpg_np_arr, "gt_png_ann" : png_np_arr , "gt_bboxes" : b_box_array}    # return the dictionary containing the data items


# transforms = [FlipImage()]
# dataset = Dataset("annotations.jsonl" , transforms)             # driver code -> for running as main, copy class Flip() at top of this code
# print(len(dataset))
# dict = dataset[4]
# #print(dict["image"]*255)
# #print(dict["gt_bboxes"])
# jpg_arr = dict["image"] * 255
# jpg_arr = np.transpose(jpg_arr , (1,2,0))
# PIL_image = Image.fromarray(jpg_arr.astype('uint8'), 'RGB').show()

