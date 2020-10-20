import matplotlib.pyplot as plt
import os
import numpy as np

image_output_dir = "image_processed"
os.makedirs(image_output_dir, exist_ok=True)


def save_im(imname, im, cmap=None):
    impath = os.path.join(image_output_dir, imname)
    plt.imsave(impath, im, cmap=cmap)


def greyscale(im):
    """ Converts an RGB image to greyscale
    
    Args:
        im ([type]): [np.array of shape [H, W, 3]]
    
    Returns:
        im ([type]): [np.array of shape [H, W]]
    """
    # YOUR CODE HERE

    out = np.zeros((len(im), len(im[0])))
    for i in range(len(out)):
        for j in range(len(out[i])):
            out[i, j] = (0.212 * im[i][j][0]) + (0.7152 * im[i][j][1]) + (0.0722 * im[i][j][2])

    return out


def inverse(im):
    """ Finds the inverse of the greyscale image
    
    Args:
        im ([type]): [np.array of shape [H, W]]
    
    Returns:
        im ([type]): [np.array of shape [H, W]]
    """    
     # YOUR CODE HERE

    out = np.zeros((len(im), len(im[0])))
    for i in range(len(out)):
        for j in range(len(out[i])):
            out[i, j] = 1.0 -  im[i, j]

    return out


if __name__ == "__main__":
    im = plt.imread("images/lake.jpg")
    im = greyscale(im)
    inverse_im = inverse(im)
    save_im("lake_greyscale.jpg", im, cmap="gray")
    save_im("lake_inverse.jpg", inverse_im, cmap="gray")
