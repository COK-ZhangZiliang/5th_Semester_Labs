import xlrd

# 打开xls文件
workbook = xlrd.open_workbook('DATA/dijkstra.xls')

# 读入22个基站的数据
worksheet = workbook.sheet_by_index(0)
with open('DATA/dijkstra_22.txt', 'w') as file:
    for i in range(2, 24):
        for j in range(2, 24):
            file.write(str(i - 1) + ' ' + str(j - 1) + ' ' + str(worksheet.cell_value(i, 1)) + ' ' 
                       + str(worksheet.cell_value(1, j)) + ' ' + str(worksheet.cell_value(i, j)) + '\n')

# 读入42个基站的数据
worksheet = workbook.sheet_by_index(1)
with open('DATA/dijkstra_42.txt', 'w') as file:
    for i in range(2, 44):
        for j in range(2, 44):
            file.write(str(i - 1) + ' ' + str(j - 1) + ' ' + str(worksheet.cell_value(i, 1)) + ' ' 
                       + str(worksheet.cell_value(1, j)) + ' ' + str(worksheet.cell_value(i, j)) + '\n')

print('Finished writing data to file.')