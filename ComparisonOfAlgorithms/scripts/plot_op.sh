echo "Start..."

python plot_operating_characteristic.py --path ..\\..\\log\\serial --files Hill_BaseMethodAndPiyavsky_serial_3.6_4.1_0.5 Hill_BaseMethod_serial_3.6 --x_lim 200 --save_path ..\\..\\img

python plot_operating_characteristic.py --path ..\\..\\log\\serial --files Shekel_BaseMethodAndPiyavsky_serial_3.6_4.1_0.5 Shekel_BaseMethod_serial_3.6 --x_lim 200 --save_path ..\\..\\img

echo "Done!"
read item
