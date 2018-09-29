#coding: utf-8
#数独のインスタンスを作成する
#排他制約付きでやってみる（一つのマスを一つのグループとみなす）

import sys

def isBox(i,j,b):
    if(0 <= i < 3):
        if(0 <= j < 3):
            if(b == 0):
                return True
        elif(3 <= j < 6):
            if(b == 1):
                return True
        elif(6 <= j < 9):
            if(b == 2):
                return True

    elif(3 <= i < 6):
        if(0 <= j < 3):
            if(b == 3):
                return True
        elif(3 <= j < 6):
            if(b == 4):
                return True
        elif(6 <= j < 9):
            if(b == 5):
                return True

    elif(6 <= i < 9):
        if(0 <= j < 3):
            if(b == 6):
                return True
        if(3 <= j < 6):
            if(b == 7):
                return True
        if(6 <= j < 9):
            if(b == 8):
                return True

    return False


args = sys.argv
if(len(args) <= 1):
    print("[FileName]")
    exit()

f = open(args[1], 'w')

mat = []
g = 0
for i in range(0,9):
    for j in range(0,9):
        #(i,j)に数字が入ることを考える
        for n in range(0,9):
            row = []
            row.append(g)

            #RmCn
            for i2 in range(0,9):
                for j2 in range(0,9):
                    if( i == i2 and j == j2):
                        row.append(1)
                    else:
                        row.append(0)

            #Rm#p
            for j2 in range(0,9):
                for n2 in range(0,9):
                    if( j == j2 and n == n2):
                        row.append(1)
                    else:
                        row.append(0)

            #Cm#p
            for i2 in range(0,9):
                for n2 in range(0,9):
                    if( i == i2 and n == n2):
                        row.append(1)
                    else:
                        row.append(0)

            #Bx#p
            for b in range(0,9):
                for n2 in range(0,9):
                    if( isBox(i,j,b) and n == n2):
                        row.append(1)
                    else:
                        row.append(0)
            
            mat.append(row)
        g += 1

#行の数,列の数,グループの数
f.write("{},{},80\n".format(len(mat), len(mat[0]) - 1))
for r in range(0, len(mat)):
    for i in range(0, len(mat[r])):
        f.write(str(mat[r][i]))
        if( i == len(mat[r]) - 1 ):
            f.write("\n")
        else:
            f.write(",")

f.close()
print("{} is created.\n".format(args[1]))
