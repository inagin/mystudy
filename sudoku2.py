#coding: utf-8
#数独のインスタンスを作成する

import sys

def isBox(i,j,b):
    if(0 <= i < 2):
        if(0 <= j < 2):
            if(b == 0):
                return True
        elif(2 <= j < 4):
            if(b == 1):
                return True

    elif(2 <= i < 4):
        if(0 <= j < 2):
            if(b == 2):
                return True
        elif(2 <= j < 4):
            if(b == 3):
                return True

    return False


args = sys.argv
if(len(args) <= 1):
    print("[FileName]")
    exit()

f = open(args[1], 'w')

mat = []
for i in range(0,4):
    for j in range(0,4):
        #(i,j)に数字が入ることを考える
        for n in range(0,4):
            row = []
            #RmCn
            for i2 in range(0,4):
                for j2 in range(0,4):
                    if( i == i2 and j == j2):
                        row.append(1)
                    else:
                        row.append(0)

            #Rm#p
            for j2 in range(0,4):
                for n2 in range(0,4):
                    if( j == j2 and n == n2):
                        row.append(1)
                    else:
                        row.append(0)

            #Cm#p
            for i2 in range(0,4):
                for n2 in range(0,4):
                    if( i == i2 and n == n2):
                        row.append(1)
                    else:
                        row.append(0)

            #Bx#p
            for b in range(0,4):
                for n2 in range(0,4):
                    if( isBox(i,j,b) and n == n2):
                        row.append(1)
                    else:
                        row.append(0)
            
            mat.append(row)

#行の数,列の数
f.write("{},{}\n".format(len(mat), len(mat[0])))
for r in range(0, len(mat)):
    for i in range(0, len(mat[r])):
        f.write(str(mat[r][i]))
        if( i == len(mat[r]) - 1 ):
            f.write("\n")
        else:
            f.write(",")

f.close()
print("{} is created.\n".format(args[1]))
