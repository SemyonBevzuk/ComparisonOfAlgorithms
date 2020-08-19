import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D


def convert_line_to_floats(line):
    line_split = line.split()
    list_numbers = [float(item) for item in line_split]
    return list_numbers


def get_XYZ(data):
    x = []
    y = []
    z = []
    for item in data:
        x.append(item[0])
        y.append(item[1])
        z.append(item[3])
    return (x, y, z)


def plot_2d(x_solved, y_solved, x_unsolved, y_unsolved):
    plt.clf()
    plt.scatter(x_solved, y_solved, marker='o', color='g', label='Solved (= 1000)')
    plt.scatter(x_unsolved, y_unsolved, marker='o', color='r', label='Unsolved (< 1000)')
    
    plt.legend()
    plt.title(r'Серия на задачах Хилла, Accuracy, Eps = 0.0001, MaxNP = 500', fontsize=12)
    plt.xlabel(r'$r_{local}$', fontsize=12)
    plt.ylabel(r'$r_{global}$', fontsize=12)
    #plt.show()
    
    save_folder = os.path.join('..', 'img')
    img_name = 'Hill_DualR_parameters_2d' + '.png'
    save_path = os.path.join(save_folder, img_name)
    plt.savefig(save_path, dpi=300, bbox_inches='tight')


def plot_3d_points(x_solved, y_solved, z_solved, x_unsolved, y_unsolved, z_unsolved):
    plt.clf()
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    ax.scatter(x_solved, y_solved, z_solved, color='g')
    ax.scatter(x_unsolved, y_unsolved, z_unsolved, color='r')
    
    ax.set_title(r'Серия на задачах Хилла, Accuracy, Eps = 0.0001, MaxNP = 500', fontsize=12)
    ax.set_xlabel(r'$r_{local}$', fontsize=12)
    ax.set_ylabel(r'$r_{global}$', fontsize=12)
    ax.set_zlabel(r'$MaxIters$', fontsize=12)
    ax.view_init(25, 225)
    #plt.show()
    save_folder = os.path.join('..', 'img')
    img_name = 'Hill_DualR_parameters_3d_points' + '.png'
    save_path = os.path.join(save_folder, img_name)
    plt.savefig(save_path, dpi=300, bbox_inches='tight')


def plot_3d_surface(x_solved, y_solved, z_solved, x_unsolved, y_unsolved, z_unsolved):
    plt.clf()
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    ax.plot_trisurf(x_solved, y_solved, z_solved, cmap='viridis')
    
    ax.set_title(r'Серия на задачах Хилла, Accuracy, Eps = 0.0001, MaxNP = 500', fontsize=12)
    ax.set_xlabel(r'$r_{local}$', fontsize=12)
    ax.set_ylabel(r'$r_{global}$', fontsize=12)
    ax.set_zlabel(r'$MaxIters$', fontsize=12)
    ax.view_init(25, 225)
    save_folder = os.path.join('..', 'img')
    img_name = 'Hill_DualR_parameters_3d_surface' + '.png'
    save_path = os.path.join(save_folder, img_name)
    plt.savefig(save_path, dpi=300, bbox_inches='tight')
    
    

fileName = os.path.join('..', 'hill_grid_DualR_params.txt')
file = open(fileName, 'r')
line = file.readline()

solved_tasks = []
unsolved_tasks = []

for line in file.readlines():
    result = convert_line_to_floats(line)
    if result[2] == 0:
        solved_tasks.append(result)
    else:
        unsolved_tasks.append(result)

x_solved, y_solved, z_solved = get_XYZ(solved_tasks)
x_unsolved, y_unsolved, z_unsolved = get_XYZ(unsolved_tasks)

plot_2d(x_solved, y_solved, x_unsolved, y_unsolved)
plot_3d_points(x_solved, y_solved, z_solved, x_unsolved, y_unsolved, z_unsolved)
plot_3d_surface(x_solved, y_solved, z_solved, x_unsolved, y_unsolved, z_unsolved)
