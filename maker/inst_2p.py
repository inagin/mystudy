#coding:utf-8

import sys
from instmaker import instutil as inst
#頂点被覆のインスタンスを作成する
#途中で分ける

args = sys.argv
if(len(args) <= 1):
  print("[FileName]")
  exit()

filename = args[1]

fieldX = 3
fieldY = 2
#ペントミノINLUXWPFZTVY|
mino =  [[[1,1]],

         [[1],
          [1]],

         [[1,0],
          [0,1]],

         [[0,1],
          [1,0]]
        ]

minoG = [0,1,2,3]

inst.makeMinoFile(filename, fieldX, fieldY, mino, minoG)

print("Created {}.\n".format(filename))
