import xlrd

# 打开xls文件
workbook = xlrd.open_workbook('DATA/data.xls')
worksheet = workbook.sheet_by_index(0)
num_rows = worksheet.nrows

# 遍历每一行，读取K_DIST值并写入data.txt文件
with open('DATA/data1.txt', 'w') as file:
    for row_idx in range(1, num_rows):
        k_dist = worksheet.cell_value(row_idx, 3)
        file.write(str(k_dist) + '\n')

with open('DATA/data2.txt', 'w') as file:
    for row_idx in range(1, num_rows):
        ENODEBID, LONGITUDE, LATITUDE,= worksheet.cell_value(row_idx, 0), worksheet.cell_value(row_idx, 1), worksheet.cell_value(row_idx, 2)
        file.write(str(ENODEBID) + ' ' + str(LONGITUDE) + ' ' + str(LATITUDE) + '\n')

print('Finished writing data to file.')
