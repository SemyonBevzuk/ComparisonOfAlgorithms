echo "Start..."

python plot_func_and_points.py --path ..\\..\\log\\tasks\\Shekel_10_BaseMethod_single_3.6.json --save_path ..\\..\\img
python plot_func_and_points.py --path ..\\..\\log\\tasks\\Shekel_10_BaseMethodAndPiyavsky_single_3.6_4.1_0.5.json --save_path ..\\..\\img
python plot_func_and_points.py --path ..\\..\\log\\tasks\\Shekel_10_MethodWithTwoR_single_2.1_4.1.json --save_path ..\\..\\img
python plot_func_and_points.py --path ..\\..\\log\\tasks\\Shekel_10_MethodWithTwoRLambda_single_2.1_4.1_0.5.json --save_path ..\\..\\img

python plot_func_and_points.py --path ..\\..\\log\\tasks\\Hill_10_BaseMethod_single_3.6.json --save_path ..\\..\\img
python plot_func_and_points.py --path ..\\..\\log\\tasks\\Hill_10_BaseMethodAndPiyavsky_single_3.6_4.1_0.5.json --save_path ..\\..\\img
python plot_func_and_points.py --path ..\\..\\log\\tasks\\Hill_10_MethodWithTwoR_single_2.1_4.1.json --save_path ..\\..\\img
python plot_func_and_points.py --path ..\\..\\log\\tasks\\Hill_10_MethodWithTwoRLambda_single_2.1_4.1_0.5.json --save_path ..\\..\\img

echo "Done!"
read item
