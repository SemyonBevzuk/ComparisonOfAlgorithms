echo "Start..."

python plot_decision_rule.py --path ..\\..\\log\\serial\\Hill_MethodWithTwoR_serial_2.1_4.1.json --save_path ..\\..\\img

python plot_decision_rule.py --path ..\\..\\log\\serial\\Shekel_MethodWithTwoR_serial_2.1_4.1.json --save_path ..\\..\\img

echo "Done!"
read item
