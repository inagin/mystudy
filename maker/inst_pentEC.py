#coding:utf-8

import sys
from instmaker import instutil as inst
#頂点被覆のインスタンスを作成する

args = sys.argv
if(len(args) <= 1):
  print("[FileName]")
  exit()

filename = args[1]

fieldX = 15
fieldY = 4
#ペントミノINLUXWPFZTVY
mino =  [[[1,1,1,1,1]], #I

         [[1],
          [1],
          [1],
          [1],
          [1]],

         [[1,1,0,0], #N
          [0,1,1,1]],

         [[0,1],
          [1,1],
          [1,0],
          [1,0]],

         [[1,1,1,0],
          [0,0,1,1]],

         [[0,1],
          [0,1],
          [1,1],
          [1,0]],

         [[0,1,1,1],
          [1,1,0,0]],

         [[1,0],
          [1,1],
          [0,1],
          [0,1]],

         [[0,0,1,1],
          [1,1,1,0]],

         [[1,0],
          [1,0],
          [1,1],
          [0,1]],

         [[1,1,1,1], #L
          [1,0,0,0]],

         [[1,1],
          [0,1],
          [0,1],
          [0,1]],

         [[0,0,0,1],
          [1,1,1,1]],

         [[1,0],
          [1,0],
          [1,0],
          [1,1]],

         [[1,1,1,1],
          [0,0,0,1]],

         [[0,1],
          [0,1],
          [0,1],
          [1,1]],

         [[1,0,0,0],
          [1,1,1,1]],

         [[1,1],
          [1,0],
          [1,0],
          [1,0]],

         [[1,0,1], #U
          [1,1,1]],

         [[1,1],
          [1,0],
          [1,1]],

         [[1,1,1],
          [1,0,1]],

         [[1,1],
          [0,1],
          [1,1]],

         [[0,1,0], #X
          [1,1,1],
          [0,1,0]],

         [[1,0,0], #W
          [1,1,0],
          [0,1,1]],

         [[0,1,1],
          [1,1,0],
          [1,0,0]],

         [[1,1,0],
          [0,1,1],
          [0,0,1]],

         [[0,0,1],
          [0,1,1],
          [1,1,0]],

         [[1,1], #P
          [1,1],
          [1,0]],

         [[1,1,1],
          [0,1,1]],

         [[0,1],
          [1,1],
          [1,1]],

         [[1,1,0],
          [1,1,1]],

         [[1,1],
          [1,1],
          [0,1]],

         [[0,1,1],
          [1,1,1]],

         [[1,0],
          [1,1],
          [1,1]],

         [[1,1,1],
          [1,1,0]],

         [[0,1,1], #F
          [1,1,0],
          [0,1,0]],

         [[0,1,0],
          [1,1,1],
          [0,0,1]],

         [[0,1,0],
          [0,1,1],
          [1,1,0]],

         [[1,0,0],
          [1,1,1],
          [0,1,0]],

         [[1,1,0],
          [0,1,1],
          [0,1,0]],

         [[0,0,1],
          [1,1,1],
          [0,1,0]],

         [[0,1,0],
          [1,1,0],
          [0,1,1]],

         [[0,1,0],
          [1,1,1],
          [1,0,0]],

         [[1,1,0], #Z
          [0,1,0],
          [0,1,1]],

         [[0,0,1],
          [1,1,1],
          [1,0,0]],

         [[0,1,1],
          [0,1,0],
          [1,1,0]],

         [[1,0,0],
          [1,1,1],
          [0,0,1]],

         [[1,1,1], #T
          [0,1,0],
          [0,1,0]],

         [[0,0,1],
          [1,1,1],
          [0,0,1]],

         [[0,1,0],
          [0,1,0],
          [1,1,1]],

         [[1,0,0],
          [1,1,1],
          [1,0,0]],

         [[1,0,0], #V
          [1,0,0],
          [1,1,1]],

         [[1,1,1],
          [1,0,0],
          [1,0,0]],

         [[1,1,1],
          [0,0,1],
          [0,0,1]],

         [[0,0,1],
          [0,0,1],
          [1,1,1]],

         [[0,0,1,0], #Y
          [1,1,1,1]],

         [[1,0],
          [1,0],
          [1,1],
          [1,0]],

         [[1,1,1,1],
          [0,1,0,0]],

         [[0,1],
          [1,1],
          [0,1],
          [0,1]],

         [[0,1,0,0],
          [1,1,1,1]],

         [[1,0],
          [1,1],
          [1,0],
          [1,0]],

         [[1,1,1,1],
          [0,0,1,0]],

         [[0,1],
          [0,1],
          [1,1],
          [0,1]]
        ]

minoG = [0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,4,5,5,5,5,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11,11,11,11,11]

inst.makeMinoFileEC(filename, fieldX, fieldY, mino, minoG)

print("Created {}.\n".format(filename))
