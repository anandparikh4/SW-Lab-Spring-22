from my_package.model import InstanceSegmentationModel
from my_package.data.dataset import Dataset
from my_package.analysis.visualize import plot_visualization
####### ADD THE ADDITIONAL IMPORTS FOR THIS ASSIGNMENT HERE #######
import PIL.Image, PIL.ImageTk
from tkinter import *
from tkinter import filedialog, messagebox
from functools import partial
import os

imgs = [None,None]   # the segmentation and bounding-boxes images respectively
img = None           # the original image
im_og = None         # the usage of im_og and im are explained the function -> process
im = None

# Define the function you want to call when the filebrowser button is clicked.
def fileClick(clicked, dataset, segmentor):

# This function should pop-up a dialog for the user to select an input image file.
# Once the image is selected by the user, it should automatically get the corresponding outputs from the segmentor.
# Hint: Call the segmentor from here, then compute the output images from using the `plot_visualization` function and save it as an image.
# Once the output is computed it should be shown automatically based on choice the dropdown button is at.
# To have a better clarity, please check out the sample video.
    global imgs
    global img

    path = filedialog.askopenfilename(title = "Open")   # get the path and save that image in img
    img = PIL.Image.open(path)

    filename,file_extension = os.path.splitext(path)    # get the index out of the path name to send into dataset
    idx = int(filename[-1])

    e.delete(0,END)
    e.insert(0,str(path))

    new_dict = dataset[idx]                             # call the dataset to get the transformed image
    np_array = new_dict["image"]

    pred = segmentor(np_array)               # pass the numpy array to the segmentor and get the results

    outputs = "./outputs"        # relative path output file

    imgs = plot_visualization(pred,np_array,0.45,0.45,0.45,False,idx,outputs)    # call the plot_visualisation and store results in the global variable imgs

# `process` function definition starts from here.
# will process the output when clicked.

def process(clicked):
                         # this is necessary, because the tkinter images need to be in the same scope as the root Tk()
    global im_og         # if we do not do this, then the stack is returned ti the system and when root tries to display these images, they are gone
    global im            # So, they cannot local variables.
                         # on the other hand, if we pass as parameters, they cannot be the lvalue and cannot be assigned
                         # So the only feasible solution is using Global variables
    if img == None :
        messagebox.showinfo("No image", "Please select an image first!")    # if no image is chosen, output respective error message
        return

    im_og = PIL.ImageTk.PhotoImage(img)                 # add a label of the original image
    Label(image = im_og).grid(row = 1,column = 0)

    choice = clicked.get()

    if choice == "Segmentation" :
        im = PIL.ImageTk.PhotoImage(imgs[0])
        Label(image = im).grid(row = 1,column = 1,columnspan = 2)       # based on the choice, add a label of either segmentation or bounding boxes image

    elif choice == "Bounding-box" :
        im = PIL.ImageTk.PhotoImage(imgs[1])
        Label(image=im).grid(row = 1,column = 1, columnspan = 2)

    return

# Should show the corresponding segmentation or bounding boxes over the input image wrt the choice provided.
# Note: this function will just show the output, which should have been already computed in the `fileClick` function above.
# Note: also you should handle the case if the user clicks on the `Process` button without selecting any image file.


# `main` function definition starts from here.
if __name__ == '__main__':

    # Instantiate the root window.
    root = Tk()
    # Provide a title to the root window.
    root.title("PythonGUI")  # title

    # Setting up the segmentor model.
    annotation_file = './data/annotations.jsonl'
    transforms = []

    # Instantiate the segmentor model.
    segmentor = InstanceSegmentationModel()
    # Instantiate the dataset.
    dataset = Dataset(annotation_file, transforms=transforms)

    # Declare the options.
    options = ["Segmentation", "Bounding-box"]
    clicked = StringVar()
    clicked.set(options[0])

    e = Entry(root, width=70)
    e.grid(row=0, column=0)

    # Declare the file browsing button
    file_browsing_button = Button(root, text=" ... ", command = partial(fileClick,clicked,dataset,segmentor), bg="white", fg="black", font=("Arial", 8))
    file_browsing_button.grid(row = 0,column = 1)

    # Declare the drop-down button
    dropdown = OptionMenu(root, clicked, *options)
    dropdown.grid(row=0, column=2)

    # This is a `Process` button, check out the sample video to know about its functionality
    myButton = Button(root, text="Process", command=partial(process,clicked))
    myButton.grid(row=0, column=3)

    root.mainloop()    # run the main loop