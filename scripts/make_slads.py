#!/usr/bin/python3
import os
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
from stl import mesh
import numpy as np
from matplotlib import collections as mc
import pylab as pl


def make_fig(lines, colors, name):
    fig, ax = pl.subplots()
    lc = mc.LineCollection(lines, colors=colors, linewidths=1)
    ax.add_collection(lc)
    ax.margins(0.1)
    plt.savefig(f'{name}.png')
    plt.close(fig)



def printtestoneslad():
    polygon = [
            (-4.52,5.7),
            (-5.12,0.7),
            (-0.92,0.72),
            (4.36,3.1),
            (8.14088,8.83728),
            (-2.16,6.8)
            ]
    e = (-6.52,8.4)
    b = (2,-3)
    a = (-6.07707,-0.00374)
    lines = []
    for idx, p in enumerate(polygon):
        lines.append([polygon[idx-1], p])
    lines.append([e, a])
    lines.append([b, a])
    lines.append([b, e])
    colors = np.zeros((len(lines),4))
    np.put_along_axis(colors, np.array([[3]*colors.shape[0]]), 1, axis=1)
    colors[-1] = (0,0,1,1)
    colors[-2] = (1,0,0,1)
    colors[-3] = (1,0,0,1)
    make_fig(lines, colors, 'testoneslad')

def printtestthreeslad():
    polygon = [
            (-4.52,5.7),
            (-5.12,0.7),
            (-0.92,0.72),
            (4.36,3.1),
            (8.14088,8.83728),
            (-2.16,6.8)
            ]
    a_polygon = [
            (-3,-3),
            (0,-2),
            (1.26621,-5.58509)
            ]
    e = (-6.52,8.4)
    b = (12.4081,-7.49046)
    a = (-6.07707,-0.00374)
    aa = (0.35078,-1.0568)
    lines = []
    for idx, p in enumerate(polygon):
        lines.append([polygon[idx-1], p])
    for idx, p in enumerate(a_polygon):
        lines.append([a_polygon[idx-1], p])
    lines.append([e, a])
    lines.append([b, aa])
    lines.append([a, aa])
    lines.append([b, e])
    colors = np.zeros((len(lines),4))
    np.put_along_axis(colors, np.array([[3]*colors.shape[0]]), 1, axis=1)
    colors[-1] = (0,0,1,1)
    colors[-2] = (1,0,0,1)
    colors[-3] = (1,0,0,1)
    colors[-4] = (1,0,0,1)
    make_fig(lines, colors, 'testthreeslad')

def printrealoneslad():
    polygon = [
            (-1.5,-2.0),
            (-1.139067,-2.000215),
            (0.002228,-1.304303),
            (0.375236,-1.014804),
            (0.998773,-0.775411),
            (1.221465,-0.786545),
            (2.0,0),
            (2.006453,1.173142),
            (0.781649,1.77441),
            (0.013362,1.89689),
            (-2.113344,1.880189),
            (-2.564295,1.429238),
            (-2.436247,0.577442),
            (-2.5,0),
            (-2.770285,-1.00367),
            (-2.43068,-1.437918),
            ]
    e = (2.190981,3.990146)
    b = (-2.64537,-3.214475)
    a = (2.314263,-0.536473)
    lines = []
    for idx, p in enumerate(polygon):
        lines.append([polygon[idx-1], p])
    lines.append([e, a])
    lines.append([b, a])
    lines.append([b, e])
    colors = np.zeros((len(lines),4))
    np.put_along_axis(colors, np.array([[3]*colors.shape[0]]), 1, axis=1)
    colors[-1] = (0,0,1,1)
    colors[-2] = (1,0,0,1)
    colors[-3] = (1,0,0,1)
    make_fig(lines, colors, 'realoneslad')


printtestoneslad()
printtestthreeslad()
printrealoneslad()

