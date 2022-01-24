#!/usr/bin/python3

import os
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
from stl import mesh
import numpy as np

def show_trajectory(figure, ax):
    file_name = './results/result.txt'
    print(f'checking {file_name}')
    N = 10000
    xs = np.zeros(N)
    ys = np.zeros(N)
    zs = np.zeros(N)
    with open(file_name) as f:
        for idx, line in enumerate(f):
            if idx >= xs.shape[0]:
                xs = np.append(xs, np.zeros(N))
                ys = np.append(ys, np.zeros(N))
                zs = np.append(zs, np.zeros(N))
            xs[idx], ys[idx], zs[idx] = [float(i) for i in line.split()]
    #ax = figure.axes[0]
    #ax = mplot3d.Axes3D(figure)
    #ax.plot(xs, ys, zs, color='b')

    ax.scatter(xs,ys,zs, marker='o', s=1, c="b", alpha=0.6)
    for ii in range(0,360,10):
        ax.view_init(elev=0., azim=ii)
        plt.savefig("shots/movie%d.png" % ii)



def show_stl(figure):
    file_name = './meshes/mesh.stl'
    print(f'checking {file_name}')
    if os.path.exists(file_name):
        print('exists')
        axes = mplot3d.Axes3D(figure)
        your_mesh = mesh.Mesh.from_file(file_name)
        #print(dir(your_mesh.points[0]))
        #all_colors = ("tab:orange", "tab:blue", "tab:green", "tab:grey")
        all_colors = ("tab:brown", "tab:grey")
        #all_colors = ("tab:grey",)
        #colors = [np.random.choice(all_colors) for vec in your_mesh.vectors]
        colors = [all_colors[idx%len(all_colors)] for idx, _ in enumerate(your_mesh.vectors)]
        axes.add_collection3d(mplot3d.art3d.Poly3DCollection(your_mesh.vectors, facecolor = colors))
        

        scale = your_mesh.points.flatten('F')
        axes.auto_scale_xyz(scale, scale, scale)
        return axes

        



def main():
    figure = plt.figure()
    ax = show_stl(figure)
    show_trajectory(figure, ax)
    #plt.show()

if __name__ == "__main__":
    main()

