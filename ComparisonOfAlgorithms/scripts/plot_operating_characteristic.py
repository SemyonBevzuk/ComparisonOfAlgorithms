import matplotlib.pyplot as plt
import numpy as np
import os, argparse

from work_with_info_about_series import load_info_about_series

def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--path', type=str, default=os.path.join('..', '_results', 'log'), help='Path to the directory.')
    parser.add_argument('--save_path', type=str, default=os.path.join('..', '_results', 'img'), help='Path to save img')
    parser.add_argument('--x_lim', type=int, default='300000', help='x lim for plot')
    parser.add_argument('--files', nargs='+', default=[], help='List of files to add to the image.')
    parser.add_argument('--logScale', action='store_true', help='Plot with logarithmic scale')
    parser.add_argument('--show', action='store_true', help='Show img')

    return parser

def plot_add_characteristic(serial_info, _label, style=''):
    tasks_trials_status = []
    for task_number in serial_info['tasks_info']:
        trials = serial_info['tasks_info'][str(task_number)]['trials']
        status = serial_info['tasks_info'][str(task_number)]['status'] 
        tasks_trials_status.append((trials, status))		
		
    tasks_trials_status.sort(key=lambda x: x[0])
    x = []
    y = []
    done_tasks_count = 0;
    correct_done_tasks_count = 0
    for i in tasks_trials_status:
        if i[1]==True:
            correct_done_tasks_count += 1
            done_tasks_count += 1
            x.append(i[0])
            y.append(done_tasks_count)
    y = [elem/1000 for elem in y]
	
    print('In %(method_name)s correct tasks: %(correct_task_count)d' %{'method_name': _label, 'correct_task_count': correct_done_tasks_count})
	
    d = {}
    for i, j in zip(x, y):
        d[i] = j
    x = []
    y = []
    for i in d:
        x.append(i)
        y.append(d[i])
    
    
    if done_tasks_count == 0:
        x.append(0)
        y.append(0)
    if done_tasks_count != 1000:
        x.append(20000000)
        y.append(done_tasks_count/1000)
    
    plt.plot(x, y, style, label = _label)

    
def main(args):
    file_folder = args.path;
    plt.figure(dpi=200, figsize=(8, 6))

    is_r_loc_been = False
    
    for file in args.files:
        file_name = str(file)+'.json'
        file_path = os.path.join(file_folder, file_name)
        serial_info = load_info_about_series(file_path)

        label = ''
        style = ''
        if 'r1' in serial_info['general_info']:
            label = r'$r_{1}$ = ' + str(serial_info['general_info']['r1']) + ', '
            label += r'$r_{2}$ = ' + str(serial_info['general_info']['r2'])
            if str(serial_info['general_info']['method_name'])=='MethodWithTwoRLambda':
                label += r', $\lambda = $' + str((serial_info['general_info']['lambda']))        
        else:
            label = r'$r$ = ' + str(serial_info['general_info']['r'])
            if 'M_Piyavsky' in serial_info['general_info']:
                label += r', $M$ = ' + str(serial_info['general_info']['M_Piyavsky'])
                label += r', $\lambda = $' + str((serial_info['general_info']['lambda']))

        plot_add_characteristic(serial_info, label)

    if args.logScale == True:
        plt.xscale('log')
    else:
        plt.xlim(0, args.x_lim)

    plt.ylim(0, 1)
    plt.legend(loc='lower right', fontsize=12)
    plt.xlabel('Number of trials', fontsize=12)
    plt.ylabel('Percentage of solved problems', fontsize=12)
    plt.grid(True)
    plt.title(str(serial_info['general_info']['problem_class']))

    class_folder = str(serial_info['general_info']['problem_class'])
    #class_folder = ''
    save_folder = os.path.join(args.save_path, class_folder)
    if not os.path.exists(save_folder):
        os.makedirs(save_folder)
    img_name = 'Operating_characteristic_'+str(serial_info['general_info']['problem_class'])
    if args.logScale == True:
        img_name = img_name + '_log'
    img_name = img_name + '' + '.png'
    save_path = os.path.join(save_folder, img_name)
    plt.savefig(save_path, dpi=300, bbox_inches='tight')
    if args.show == True:
        plt.show()
    
if __name__ == '__main__':
    parser = create_parser()
    main(parser.parse_args())
