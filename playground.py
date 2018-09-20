import numpy as np 
import scipy.misc as spm

a = np.array([[ ( 0, 0, 0, 255) for i in range(0,2000)] for j in range(0,1000)])
spm.imsave('out_img.png', a)
