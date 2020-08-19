import matplotlib.pyplot as plt
import numpy as np
import argparse, sys, os

from work_with_info_about_series import load_info_about_series

def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--path', type=str, default='', help='Path to info about series')
    parser.add_argument('--save_path', type=str, default=os.path.join('..', 'img'), help='Path to save img')

    return parser

def plot_rules_1(serial_info):
    r1 = [0]*11
    r2 = [0]*11
    task_count = 0
    for task_number in serial_info['tasks_info']:
        r_local = serial_info['tasks_info'][str(task_number)]['r_types'][0]
        r_global = serial_info['tasks_info'][str(task_number)]['r_types'][1]
        for i in range(0, 11):
            r1[i] += r_local[i]
            r2[i] += r_global[i]
        task_count += 1
    for i in range(0, 11):
        r1[i] = r1[i]/task_count
        r2[i] = r2[i]/task_count
    x = [i for i in range(0, 101, 10)]
    plt.xlim(10, 100)
    plt.plot(x, r1, 'ro-', label = r'$r_{1}$ = ' + str(serial_info['general_info']['r1']))
    plt.plot(x, r2, 'go-', label = r'$r_{2}$ = ' + str(serial_info['general_info']['r2']))

    plt.legend(loc='lower right', fontsize=12)

def plot_rules_2(serial_info):
    r1 = [0]*11
    r2 = [0]*11
    task_count = 0
    for task_number in serial_info['tasks_info']:
        r_local = serial_info['tasks_info'][str(task_number)]['r_types'][0]
        r_global = serial_info['tasks_info'][str(task_number)]['r_types'][1]
        r1_count = 0
        r2_count = 0
        for i in range(0, 11):
            r1_count += r_local[i]
            r2_count += r_global[i]
            r1[i] += r1_count
            r2[i] += r2_count
        task_count += 1
    for i in range(0, 11):
        r1[i] = r1[i]/task_count
        r2[i] = r2[i]/task_count
    x = [i for i in range(0, 101, 10)]
    plt.xlim(10, 100)
    plt.plot(x, r1, 'ro-', label = r'$r_{1}$ = ' + str(serial_info['general_info']['r1']))
    plt.plot(x, r2, 'go-', label = r'$r_{2}$ = ' + str(serial_info['general_info']['r2']))
    
def main(args):
    serial_info = load_info_about_series(args.path)
    plot_rules_1(serial_info)
    plt.ylim(0)
    plt.legend()
    plt.xlabel('Progress in solving the problem', fontsize=12)
    plt.ylabel('Frequency', fontsize=12)
    plt.title(str(serial_info['general_info']['method_name']), fontsize=12)
    plt.grid(True)

    class_folder = str(serial_info['general_info']['problem_class'])
    save_folder = os.path.join(args.save_path, class_folder) #'..', '_results', 'img', ''
    if not os.path.exists(save_folder):
        os.makedirs(save_folder)
    img_name = 'Decision_rule_'+str(serial_info['general_info']['problem_class'])+ '_' + \
                str(serial_info['general_info']['r1']) + '_' + \
                str(serial_info['general_info']['r2'])
    if 'lambda' in serial_info['general_info']:
        img_name += '_' + str(serial_info['general_info']['lambda'])
    img_name += '.png'
    save_path = os.path.join(save_folder, img_name)
    #print(save_path)
    plt.savefig(save_path, dpi=300, bbox_inches='tight')
    #plt.show()
    
if __name__ == '__main__':
    parser = create_parser()
    main(parser.parse_args())
