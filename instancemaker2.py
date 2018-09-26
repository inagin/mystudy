#coding:utf-8

import sys
#頂点被覆のインスタンスを作成する

args = sys.argv
if(len(args) <= 1):
	print("[FileName]")
	exit()

f = open(args[1], 'w')

fieldX = 4
fieldY = 4

numGroup = 5
minoA = [
		 [[1,1,1],
			[1,1,1]],

		 [[1,1,1]]
		]

minoB = [[[0,1,0],
					[1,1,1]],

				 [[1,0],
					[1,1],
					[1,0]],

				 [[1,1,1],
					[0,1,0]],

				 [[0,1],
					[1,1],
					[0,1]],

				 [[1,1,1,1]],

				 [[1],
					[1],
					[1],
					[1]],

				 [[1,1,0],
					[0,1,1]],

				 [[0,1],
					[1,1],
					[1,0]],

				 [[1,1],
					[1,1]],

				 [[1,0,0],
					[1,1,1]],

				 [[1,1],
					[1,0],
					[1,0]],

				 [[1,1,1],
					[0,0,1]],

				 [[0,1],
					[0,1],
					[1,1]],

				 [[1,1],
					[0,1],
					[0,1]],

				 [[1,1,1],
					[1,0,0]],

				 [[1,0],
					[1,0],
					[1,1]],

				 [[0,0,1],
					[1,1,1]]
			 ]

mino = minoB
minoG = [0,0,0,0,1,1,2,2,3,4,4,4,4,5,5,5,5]

rowCount = 0

out = [] #アウトプットする行を入れる

for minoI in range(0, len(mino)):
	#座標(i,j) にミノの左上が来るようにする
	minoW = len(mino[minoI][0]) #参照中のミノの横幅
	minoH = len(mino[minoI])

	#print("minoW: {}, minoH: {}".format(minoW, minoH))
	for i in range(0, fieldY):
		for j in range(0,fieldX):
			#横幅・縦幅が足りているか
			if((j + minoW <= fieldX) and
				(i + minoH <= fieldY)):
				#列の作成
				row = []

				for k in range(0, fieldY):
					#ミノの左上のマスにたどり着くまで0で埋める
					if(k < i):
						for l in range(0, fieldX):
							row.append(0)

					#iで参照しているY座標の中にミノを配置する
					elif(i <= k  and k < i + minoH ):
						for l in range(0, fieldX):
							if(l < j):
								row.append(0)
							#jで参照しているX座標の中にミノを配置する
							elif(j <= l  and l-j < minoW ):
								row.append(mino[minoI][k-i][l-j])
							else:
								row.append(0)

					else:
						for l in range(0, fieldX):
							row.append(0)

				out.append(row)
				
				rowCount += 1

print("{}".format(out))
f.write("{},{},{}\n".format(rowCount, fieldX * fieldY,numGroup))

for r in range(0, len(out)):
	for x in range(0, len(out[r])):
		f.write(str(out[r][x]))

		if(x != len(out[r]) - 1):
			f.write(',')

	if(r != len(out) - 1):
		f.write('\n')

f.close()

print("{} is created.\n".format(args[1]))