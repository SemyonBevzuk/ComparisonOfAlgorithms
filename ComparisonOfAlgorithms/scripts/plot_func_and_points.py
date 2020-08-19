import matplotlib.pyplot as plt
import numpy as np
import argparse, sys, os

from work_with_info_about_series import load_info_about_series


def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--path', type=str, default='', help='Path to info about series')
    parser.add_argument('--save_path', type=str, default=os.path.join('..', 'img'), help='Path to save img')

    return parser
 
 
def get_img_name(stats):
    img_name =  str(stats['task_info']['problem_class'])+ '_' + \
                str(stats['task_info']['task_number'])+ '_' + \
                str(stats['general_info']['method_name'])
    if 'r' in stats['general_info']:
        img_name += '_' + str(stats['general_info']['r'])
    if 'r1' in stats['general_info']:
        img_name += '_' + str(stats['general_info']['r1']) + '_' + str(stats['general_info']['r2'])
    if 'M_Piyavsky' in stats['general_info']:
        img_name += '_' + str(stats['general_info']['M_Piyavsky'])
    if 'lambda' in stats['general_info']:
        img_name += '_' + str(stats['general_info']['lambda'])
    return img_name
 
 
def plot_func_and_iter_points(stats, plt):
    a = float(stats['task_info']['left_border'])
    b = float(stats['task_info']['right_border'])
    y = stats['task_info']['function']
    x=np.linspace(a, b, len(y))
    min_y = float(min(y))
    max_y = float(max(y))
    plt.subplot (2, 1, 1)
    plt.plot(x, y)
    plt.ylabel(r'$f(x)$')
    
    trials_points = stats['task_info']['iters']
    if stats['general_info']['method_name'] == 'BaseMethod' or stats['general_info']['method_name'] == 'BaseMethodAndPiyavsky':
        x_other = []
        for i in range(0, len(trials_points), 2):
            x_other.append(float(trials_points[i]))
        y = [min_y-1.0 for i in range(0, len(x_other))]
        plt.scatter(x_other, y, color='black', label=r'Trial points', s=10.,)
    else:
        x_loc = []
        x_glob = []
        for i in range(0, len(trials_points), 2):
            if trials_points[i+1]== 1:
                x_loc.append(float(trials_points[i]))
            elif trials_points[i+1]== 2:
                x_glob.append(float(trials_points[i]))
        y = [min_y-1.0 for i in range(0, len(x_loc))]
        plt.scatter(x_loc, y, color='red', label=r'$r_{loc}$', s=10.,)
        y = [min_y-1.0 for i in range(0, len(x_glob))]
        plt.scatter(x_glob, y, color='green', label=r'$r_{glob}$', s=10.)

    plt.xlim(a, b)
    plt.ylim(round(min_y-2.0), round(max_y+2.0))
    plt.grid(True)
    plt.title(get_img_name(stats), fontsize=12)
    plt.legend()
    
    # Точки испытаний
    plt.subplot (2, 1, 2)
    plt.xlabel(r'$x$')
    plt.ylabel(r'$k$')
    plt.xlim(a, b)
    plt.ylim(0, len(trials_points)/2)
    # Линия последовательности испытаний
    x_points = [trials_points[i] for i in range(0, len(trials_points), 2)]
    y = [i for i in range(0, len(x_points))]
    plt.plot(x_points, y, color='black', lw=0.5, alpha=0.5, zorder=1)
    # Тип испытаний
    x_loc = []
    x_glob = []
    x_other = []
    y_loc = []
    y_glob = []
    y_other = []
    y_count = 0
    for i in range(0, len(trials_points), 2):
        if trials_points[i+1]== 1:
            x_loc.append(float(trials_points[i]))
            y_loc.append(y_count)
        elif trials_points[i+1]== 2:
            x_glob.append(float(trials_points[i]))
            y_glob.append(y_count)
        else:
            x_other.append(float(trials_points[i]))
            y_other.append(y_count)

        y_count += 1
    plt.scatter(x_other, y_other, color='black', s=10., zorder=2)
    plt.scatter(x_loc, y_loc, color='red', s=10., zorder=2)
    plt.scatter(x_glob, y_glob, color='green', s=10., zorder=2)

    plt.xticks([])
    #plt.yticks([])
 
 
def save_img(stats, plt, save_path):
    class_folder = str(stats['task_info']['problem_class'])
    save_folder = os.path.join(save_path, class_folder)
    if not os.path.exists(save_folder):
        os.makedirs(save_folder)
    img_name = get_img_name(stats)
    img_name += '.png'
    save_path = os.path.join(save_folder, img_name)
    #print(save_path)
    plt.savefig(save_path, dpi=300, bbox_inches='tight')
 
 
def main(args):
    stats = load_info_about_series(args.path)
    plt.figure(dpi=300, figsize=(8, 6))
    plot_func_and_iter_points(stats, plt)
    save_img(stats, plt, args.save_path)

    #plt.show()
    
if __name__ == '__main__':
    parser = create_parser()
    main(parser.parse_args())