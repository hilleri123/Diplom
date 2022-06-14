#!/usr/bin/python3

import os
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
from stl import mesh
import numpy as np
import meshio



def show_stl(figure):
    file_name = './meshes/pic_mesh.stl'
    trj_file_name = './results/pic_result.txt'
    color_mesh_file = './meshes/color_mesh.'
    formats = []
    #formats = ['inp','avs','obj','off','vtk','vtu']
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
            def flat():
                for idx, p in enumerate(face):
                    s[idx] += p[2]
                if (s < l).all():
                    return 'tab:blue'
                if (s >= h).any():
                    return 'tab:brown'
                return 'tab:green'
            return flat()
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

        #SAVE
        def color_np(color):
            res = np.array([0,0,0,1])
            if color == 'tab:blue':
                res[2] = 1
            if color == 'tab:brown':
                res[0] = 0.5
                res[1] = 0.5
            if color == 'tab:green':
                res[1] = 1
            return res
        colors_to_save = np.array([color_np(c) for c in colors])
        points_to_save = []
        #for p in your_mesh.points:
            #points_to_save.append(p[:3])
            #points_to_save.append(p[3:6])
            #points_to_save.append(p[6:9])
        #points_to_save = np.array(points_to_save)
        vecs_to_save = []
        for v in vec:
            res = np.zeros(3)
            for pos, p in enumerate(v):
                all_e = False
                for idx, pp in enumerate(points_to_save):
                    all_e = all([v0 == v1 for v0, v1 in zip(p, pp)])
                    if all_e:
                        res[pos] = idx
                        break
                if not all_e:
                    points_to_save.append(p)
            #print(res)
            vecs_to_save.append(res)
        vecs_to_save = np.array(vecs_to_save)
        #vecs_to_save = np.array([np.array([np.where(points_to_save == p) for p in v]) for v in vec])
        cells = [('triangle', vecs_to_save)]

        mesh_to_save = meshio.Mesh(np.array(points_to_save), cells, cell_data={'a':[colors_to_save]})
        for file_format in formats:
            print(f'{color_mesh_file}{file_format}')
            mesh_to_save.write(color_mesh_file+file_format)
        #return

        collection = mplot3d.art3d.Poly3DCollection(vec, facecolor = colors)
        #collection.set_alpha(0.7)
        ax.add_collection3d(collection)
        scale = np.append(maxs, np.zeros(3)).flatten('F')
        ax.auto_scale_xyz(scale, scale, scale)


        #file_name = './results/0pic_res.txt'
        file_name = trj_file_name
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

