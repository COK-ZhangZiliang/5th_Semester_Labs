import xlrd

# 打开xls文件
workbook = xlrd.open_workbook('DATA/data.xls')

# 读入15个基站的数据
worksheet = workbook.sheet_by_index(0)
with open('DATA/tsp_15.txt', 'w') as file:
    for i in range(2, 17):
        for j in range(2, 17):
            file.write(str(worksheet.cell_value(i, 0)) + ' ' + str(worksheet.cell_value(0, j)) + ' ' + str(worksheet.cell_value(i, j)) + '\n')

# 读入20个基站的数据
worksheet = workbook.sheet_by_index(1)
with open('DATA/tsp_20.txt', 'w') as file:
    for i in range(2, 22):
        for j in range(2, 22):
            file.write(str(worksheet.cell_value(i, 0)) + ' ' + str(worksheet.cell_value(0, j)) + ' ' + str(worksheet.cell_value(i, j)) + '\n')

# 读入22个基站的数据
worksheet = workbook.sheet_by_index(2)
with open('DATA/tsp_22.txt', 'w') as file:
    for i in range(2, 24):
        for j in range(2, 24):
            file.write(str(worksheet.cell_value(i, 0)) + ' ' + str(worksheet.cell_value(0, j)) + ' ' + str(worksheet.cell_value(i, j)) + '\n')

# 读入30个基站的数据
worksheet = workbook.sheet_by_index(3)
with open('DATA/tsp_30.txt', 'w') as file:
    for i in range(2, 32):
        for j in range(2, 32):
            file.write(str(worksheet.cell_value(i, 0)) + ' ' + str(worksheet.cell_value(0, j)) + ' ' + str(worksheet.cell_value(i, j)) + '\n')

print('Finished writing data to file.')