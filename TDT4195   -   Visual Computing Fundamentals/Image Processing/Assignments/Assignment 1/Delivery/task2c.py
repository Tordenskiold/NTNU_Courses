import os

import matplotlib.pyplot as plt
import numpy as np


def convolve_im(im, kernel):
    """ A function that convolves im with kernel
    
    Args:
        im ([type]): [np.array of shape [H, W, 3]]
        kernel ([type]): [np.array of shape [K, K]]
    
    Returns:
        [type]: [np.array of shape [H, W, 3]. should be same as im]
    """
    # YOUR CODE HERE

    np.flip(kernel, 0)
    np.flip(kernel, 1)
    offset = len(kernel) // 2
    out = np.zeros((len(im), len(im[0]), len(im[0][0])))
    for i in range(len(im)):
        for j in range(len(im[i])):
            new_value = np.array([0.0, 0.0, 0.0])
            for a in range(len(kernel)):
                for b in range(len(kernel)):
                    if (i + (a - offset) >= 0) and (i + (a - offset) < len(im)) and (j + (b - offset) >= 0) and (j + (b - offset) < len(im[i])):
                        new_value += (im[i + (a - offset), j + (b - offset)] * kernel[a, b] / 255)
            out[i, j] = new_value

    return out


if __name__ == "__main__":
    # Read image
    impath = os.path.join("images", "lake.jpg")
    im = plt.imread(impath)

    # Define the convolutional kernels
    h_a = np.ones((3, 3)) / 9
    h_b = np.array([
        [1, 4, 6, 4, 1],
        [4, 16, 24, 16, 4],
        [6, 24, 36, 24, 6],
        [4, 16, 24, 16, 4],
        [1, 4, 6, 4, 1],
    ]) / 256
    # Convolve images
    smoothed_im1 = convolve_im(im.copy(), h_a)
    smoothed_im2 = convolve_im(im, h_b)

    # DO NOT CHANGE
    assert isinstance(smoothed_im1, np.ndarray), \
        f"Your convolve function has to return a np.array. " +\
        f"Was: {type(smoothed_im1)}"
    assert smoothed_im1.shape == im.shape, \
        f"Expected smoothed im ({smoothed_im1.shape}" + \
        f"to have same shape as im ({im.shape})"
    assert smoothed_im2.shape == im.shape, \
        f"Expected smoothed im ({smoothed_im1.shape}" + \
        f"to have same shape as im ({im.shape})"

    ("convolved_im_h_a.jpg", smoothed_im1)
    ("convolved_im_h_b.jpg", smoothed_im2)
