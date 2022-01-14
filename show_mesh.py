#!/usr/bin/python3

import os
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
from stl import mesh
import numpy as np

def show_stl():
    file_name = './meshes/mesh.stl'
    print(f'checking {file_name}')
    if os.path.exists(file_name):
        print('exists')
        figure = plt.figure()
        axes = mplot3d.Axes3D(figure)
        your_mesh = mesh.Mesh.from_file(file_name)
        axes.add_collection3d(mplot3d.art3d.Poly3DCollection(your_mesh.vectors))

        scale = your_mesh.points.flatten('F')
        axes.auto_scale_xyz(scale, scale, scale)

        plt.show()



def main():
    show_stl()

if __name__ == "__main__":
    main()

