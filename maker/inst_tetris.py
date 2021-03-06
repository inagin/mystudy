#coding:utf-8

import sys
from instmaker import instutil as inst
#頂点被覆のインスタンスを作成する

args = sys.argv
if(len(args) <= 1):
  print("[FileName]")
  exit()

filename = args[1]

fieldX = 7
fieldY = 4
#テトリミノ
mino =  [
		 [[1,1,1,1]],

		 [[1],
		  [1],
		  [1],
		  [1]],

		 [[1,1],
		  [1,1]],

		 [[0,1,1],
		  [1,1,0]],

		 [[1,0],
		  [1,1],
		  [0,1]],

		 [[1,1,0],
		  [0,1,1]],

		 [[0,1],
		  [1,1],
		  [1,0]],

		 [[1,1,1],
		  [0,0,1]],

		 [[0,1],
		  [0,1],
		  [1,1]],

		 [[1,0,0],
		  [1,1,1]],

		 [[1,1],
		  [1,0],
		  [1,0]],

		 [[1,1,1],
		  [1,0,0]],

		 [[1,1],
		  [0,1],
		  [0,1]],

		 [[0,0,1],
		  [1,1,1]],

		 [[1,0],
		  [1,0],
		  [1,1]],

		 [[1,1,1],
		  [0,1,0]],

		 [[0,1],
		  [1,1],
		  [0,1]],

		 [[0,1,0],
		  [1,1,1]],

		 [[1,0],
		  [1,1],
		  [1,0]]
		]

minoG = [0,0,1,2,2,3,3,4,4,4,4,5,5,5,5,6,6,6,6]

#inst.makeMultiMinoFile(filename, fieldX, fieldY, mino)
#inst.makeMinoFile(filename, fieldX, fieldY, mino, minoG)
inst.makeMinoFileEC(filename, fieldX, fieldY, mino, minoG)

print("Created {}.\n".format(filename))
