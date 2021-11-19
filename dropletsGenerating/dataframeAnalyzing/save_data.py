# ИМПОРТ БИБЛИОТЕК И НАСТРОЙКА ВЫВОДА
import win32con, win32api, os
import pandas as pd
from shutil import copyfile
from datetime import datetime

def save_csv(df, folder_path, save_name):
    print("Are you sure you want to write this DataFrame to main DataBase? Print 'yes'")
    ans = input()
    if ans.lower().strip() == "yes":
        file_path = os.path.join(folder_path, save_name)
        if os.path.exists(file_path):
            copy_folder_path = os.path.join(folder_path, '_archive')
            if not os.path.exists(copy_folder_path): 
                os.makedirs(copy_folder_path)
            now = datetime.now().strftime("%Y-%m-%d#%H%M")
            copy_name = now + " " + save_name
            print(copy_name)
            copy_path = os.path.join(copy_folder_path, copy_name)
            try:
                copyfile(file_path, copy_path)
            except:  
                return "Copy error! Dataframe hadn't been saved."
            win32api.SetFileAttributes(file_path, win32con.FILE_ATTRIBUTE_NORMAL)
            df.to_csv(file_path, index=False)
            win32api.SetFileAttributes(file_path, win32con.FILE_ATTRIBUTE_READONLY)
            return "Done"
        else:
            return "Error! DataBase file not found."
    return "Saving cancelled"
    

def save_xlsx(df, folder_path, save_name):
    print("Are you sure you want to write this DataFrame to Excel-file? Print 'yes'")
    ans = input()
    if ans.lower().strip() == "yes":
        file_path = os.path.join(folder_path, save_name)
        if os.path.exists(file_path):
            win32api.SetFileAttributes(file_path, win32con.FILE_ATTRIBUTE_NORMAL)
            
            writer = pd.ExcelWriter(file_path, engine='xlsxwriter', datetime_format='YYYY-MM-DD')

            df.to_excel(writer, index=False, sheet_name='Main_Data', startrow=1, header=False)

            # Sheet formatting
            workbook = writer.book
            worksheet = writer.sheets['Main_Data']

            # formatting objects
            head_fmt = workbook.add_format({'bold': True, 'text_wrap': True, 'align': 'center_across', 'valign': 'center'})
            test_num_fmt = workbook.add_format({'bold': True})
            test_num_fmt.set_num_format('0000')

            calib_num_fmt = workbook.add_format({'bold': False})
            calib_num_fmt.set_num_format('0000')

            # SAVING COLUMNS
            excel_columns = ['Test №', 'Crop №', 'Test type', 'Voltage, V', 'Pressure, mbar', 'Impulse time, us', 'Needle', 'Nozzle', 'Distance, mm', 'Calibrat test №', 'Record rate, fps', 'Comments', 'Experiment date', 'Tester', 'Video format', 'Pixel size, um', 'Old Flow type', 'Old Droplet size, um', 'Droplet count', 'Flow type', 'Droplet size, um','Standard deviation of droplet size, um', 'Starting velocity, m/s', 'Ending velocity, m/s', 'Falling time, s', 'Vertical path, mm','Horizontal deviation, mm','Impulse count', 'Crop count']

            # Save header
            for i, column_name in enumerate(excel_columns):
                worksheet.write(0, i, column_name, head_fmt)

            worksheet.set_column('A:A', 5.1, test_num_fmt)
            worksheet.set_column('B:B', 5.1)
            worksheet.set_column('C:C', 8.1)
            worksheet.set_column('D:D', 7.1)
            worksheet.set_column('E:E', 8.1)
            worksheet.set_column('F:F', 7.1)
            worksheet.set_column('G:H', 6.1)
            worksheet.set_column('J:J', 7.0, calib_num_fmt)
            worksheet.set_column('K:K', 7.0)
            worksheet.set_column('L:L', 10)
            worksheet.set_column('M:M', 10.1)
            worksheet.set_column('N:N', 8.0)
            worksheet.set_column('O:O', 7.5)
            worksheet.set_column('P:P', 5.8)
            worksheet.set_column('Q:Q', 9.5)
            worksheet.set_column('R:R', 7.1)
            worksheet.set_column('S:S', 7.1)
            worksheet.set_column('T:T', 5.1)
            worksheet.set_column('U:U', 6.5)
            worksheet.set_column('V:V', 9.0)
            worksheet.set_column('W:W', 7.1)
            worksheet.set_column('X:X', 7.1)
            worksheet.set_column('Y:Y', 7.1)
            worksheet.set_column('Z:Z', 7.1)
            worksheet.set_column('AA:AA', 9.0)
            worksheet.set_column('BB:BB', 8.1)
            worksheet.set_column('CC:CC', 8.1)

            writer.save()

            win32api.SetFileAttributes(file_path, win32con.FILE_ATTRIBUTE_READONLY)
            return "Done"
        else:
            return "Error! Excel-file not found."
    return "Saving cancelled"