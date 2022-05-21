#!/usr/bin/python3

import os
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
from stl import mesh
import numpy as np



def show_stl(figure):
    file_name = './meshes/mesh.stl'
    print(f'checking {file_name}')
    if os.path.exists(file_name):
        print('exists')
        ax = mplot3d.Axes3D(figure)
        your_mesh = mesh.Mesh.from_file(file_name)
        #print(dir(your_mesh.points[0]))
        #all_colors = ("tab:orange", "tab:blue", "tab:green", "tab:grey")
        all_colors = ("tab:brown", "tab:grey")
        #all_colors = ("tab:grey",)
        #colors = [np.random.choice(all_colors) for vec in your_mesh.vectors]
        #print(vec)
        #print('-'*50)
        #print(np.min(np.min(vec, axis=0), axis=0))
        mins = np.min(np.min(your_mesh.vectors, axis=0), axis=0)
        vec = your_mesh.vectors - mins
        maxs = np.max(np.max(vec, axis=0), axis=0)
        L = 0.01
        def chose_color(face):
            a = 6378.137
            f = 1./298.257223263;
            b = a - f * a;
            l = L
            h = 1
            s = np.zeros(3)
            for idx, p in enumerate(face):
                for i in p:
                    s[idx] += i**2
                s[idx] = np.sqrt(s[idx])
                az = np.arccos(np.sqrt(p[0]**2+p[1]**2)/s[idx])
                axy = np.arccos(p[0]/np.sqrt(p[0]**2+p[1]**2))
                x = np.cos(axy) * np.cos(az) * a
                y = np.sin(axy) * np.cos(az) * a
                z = np.sin(az) * b
                r = np.sqrt(x**2+y**2+z**2)
                s[idx] -= r
            if (s < l).all():
                return 'tab:blue'
            if (s >= h).all():
                return 'tab:brown'
            return 'tab:green'

        colors = [chose_color(face) for idx, face in enumerate(your_mesh.vectors)]
        collection = mplot3d.art3d.Poly3DCollection(vec, facecolor = colors)
        #collection.set_alpha(0.7)
        ax.add_collection3d(collection)
        scale = np.append(maxs, np.zeros(3)).flatten('F')
        ax.auto_scale_xyz(scale, scale, scale)
        return


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
                xs[idx], ys[idx], zs[idx] = [float(i)-mins[idx] for idx, i in enumerate(line.split())]
        for i in range(idx, xs.shape[0]):
            xs[i] = xs[idx]
            ys[i] = ys[idx]
            zs[i] = zs[idx]


        scale = np.append(np.append(np.append(np.append(maxs, np.zeros(3)), xs), ys), zs).flatten('F')
        #scale = np.append(maxs, mins).flatten('F')
        ax.scatter(xs,ys,zs, marker='o', s=1, c="r", alpha=1)
        ax.plot(xs,ys,zs, c="black")

        ax.auto_scale_xyz(scale, scale, scale)

        #sup_points = np.array([[2000,2750,420],[3400,2300,850],])
        sup_points = []
        
        for p in zip(xs, ys, zs):
            if 410 < p[2] < 430 and len(sup_points) < 2:
                sup_points.append(p)
            if 840 < p[2] < 860 and len(sup_points) < 1:
                sup_points.append(p)
        for spx, spy, spz in sup_points:
            ax.scatter(spx,spy,spz, marker='o', s=1, c="r", alpha=1)

        #colors = [chose_color(face) for idx, face in enumerate(your_mesh.vectors)]
        #ax.add_collection3d(mplot3d.art3d.Poly3DCollection(vec, facecolor = colors))

        if True:
            elev = 0
            for ii in range(0,360,10):
                ax.view_init(elev=elev, azim=ii)
                plt.savefig("shots/plain_movie%d_%d_l%d.png" % (ii, elev, L))
        


        



def main():
    figure = plt.figure()
    show_stl(figure)
    plt.show()

if __name__ == "__main__":
    main()

