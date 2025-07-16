import numpy as np
from PIL import Image,ImageDraw,ImageFont

def plot_visualization(pred, np_arr, r, g, b,show,idx,save_file=None):
    pred_boxes = pred[0]
    pred_masks = pred[1]                # take the required items
    pred_class = pred[2]
    pred_score = pred[3]

    full_mask = np.zeros(np_arr.shape)    # the mask/segments array
    final_image = np_arr                  # the final image array

    if len(pred[3]) < 3:
        for i in range(len(pred[3])):
            full_mask[i] = full_mask[i] + pred_masks[i]    # there are at most 3 images to highlight, so naturally, the colours chosen are R,G,B
                                                           # adding the arrays basically highlights that portion, as pixels become more of R/G/B as applied
    else:
        for i in range(3):
            full_mask[i] = full_mask[i] + pred_masks[i]

    final_image = final_image * 255
    final_image = np.transpose(final_image, (1, 2, 0))   # converting the numpy arrays to PIL images by scaling back to X 255 and switching the dimensions

    full_mask = full_mask * 255
    full_mask = np.transpose(full_mask, (1, 2, 0))

    final_image_2 = final_image
    final_image = final_image + full_mask * [r, g, b]    # this lets us regulate the intensity of R/G/B segment highlights

    PIL_image = Image.fromarray(final_image.astype('uint8'), 'RGB')    # convert to PIL image
    PIL_image_2 = Image.fromarray(final_image_2.astype('uint8'), 'RGB')    # convert to PIL image

    draw = ImageDraw.Draw(PIL_image_2)            # use this to draw on the images
    font = ImageFont.truetype("arial.ttf",16)

    if len(pred[3]) < 3:
        for i in range(len(pred[3])):
            x1 = int(pred_boxes[i][0][0])
            y1 = int(pred_boxes[i][0][1])
            x2 = int(pred_boxes[i][1][0])
            y2 = int(pred_boxes[i][1][1])

            t = pred_class[i] + ", " + str(pred_score[i])

            draw.rectangle((x1,y1,x2,y2),fill = None,outline=(200,0,120),width=1)    # make bounding boxes and add text
            draw.text((x1,y1),t,font=font,fill = (0,0,255))

    else:
        for i in range(3):
            x1 = int(pred_boxes[i][0][0])
            y1 = int(pred_boxes[i][0][1])
            x2 = int(pred_boxes[i][1][0])
            y2 = int(pred_boxes[i][1][1])

            t = pred_class[i] + ", " + str(pred_score[i])

            draw.rectangle((x1, y1, x2, y2), fill=None, outline=(200, 0, 120), width=1)
            draw.text((x1, y1), t, font=font, fill=(0,0,255))

    if show :
         PIL_image.show()        # choose whether to show image or not
         PIL_image_2.show()

    file = str(save_file)
    PIL_image.save(file+"/{}_seg.jpg".format(idx))    # save the image in the required folder with correct extension
    PIL_image_2.save(file+"/{}_bbox.jpg".format(idx))    # save the image in the required folder with correct extension

    return PIL_image,PIL_image_2











