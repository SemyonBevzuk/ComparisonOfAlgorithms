import json

def save_info_about_series(series_info, filename):
    if not series_info:
        print('Nothing to save.')
    else:
        with open(filename, 'w') as outfile:
            json.dump(series_info, outfile)

def load_info_about_series(filename):
    series_info = {}
    with open(filename, 'r') as f:
        series_info = json.load(f)
    return series_info
    
def main():
    serial_info = load_info_about_series("log\\Hill_MethodWithTwoR_serial_1.3_3.6.json")
    print(serial_info)

if __name__ == '__main__':
    main()
