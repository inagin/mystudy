#coding:utf-8

import sys
from instmaker import instutil as inst
#頂点被覆のインスタンスを作成する

args = sys.argv
if(len(args) <= 1):
  print("[FileName]")
  exit()

filename = args[1]

fieldX = 4
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

inst.makeMultiMinoFile(filename, fieldX, fieldY, mino)

print("Created {}.\n".format(filename))
