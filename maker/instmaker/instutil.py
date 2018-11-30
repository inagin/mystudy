#coding:utf-8

#列順序の入れ替え用
#ぐるぐる

#排他制約をつけない
def makeMultiMinoFile(fileName, fieldX, fieldY, mino):
  f = open(fileName, 'w')

  rowCount = 0
  out = [] #アウトプットする行を入れる

  for minoI in range(0, len(mino)):
    #座標(i,j) にミノの左上が来るようにする
    for i in range(0, fieldY):
      for j in range(0,fieldX):
        #横幅・縦幅が足りているか
        if((j + len(mino[minoI][0])) <= fieldX and
          (i + len(mino[minoI])) <= fieldY):
          #列の作成
          row = []

          for k in range(0, fieldY):
            if(k < i):
              for l in range(0, fieldX):
                row.append(0)

            elif(i <= k  and k-i < len(mino[minoI]) ):
              for l in range(0, fieldX):
                if(l < j):
                  row.append(0)
                elif(j <= l  and l-j < len(mino[minoI][0]) ):
                  row.append(mino[minoI][k-i][l-j])
                else:
                  row.append(0)

            else:
              for l in range(0, fieldX):
                row.append(0)

          out.append(row)
          rowCount += 1

  f.write("{},{}\n".format(rowCount, fieldX * fieldY))

  for r in range(0, len(out)):
    for x in range(0, len(out[r])):
      f.write(str(out[r][x]))

      if(x != len(out[r]) - 1):
        f.write(',')

    if(r != len(out) - 1):
      f.write('\n')

  f.close()

def makeMinoFile(fileName, fieldX, fieldY, mino, minoG, func = None):
  f = open(fileName, 'w')

  numGroup = max(minoG)
  rowCount = 0
  out = [] #アウトプットする行を入れる

  for minoI in range(0, len(mino)):
  	#座標(i,j) にミノの左上が来るようにする
  	for i in range(0, fieldY):
  		for j in range(0,fieldX):
  			#横幅・縦幅が足りているか
  			if((j + len(mino[minoI][0])) <= fieldX and
  				(i + len(mino[minoI])) <= fieldY):
  				#列の作成
  				row = []

  				row.append(minoG[minoI])
  				for k in range(0, fieldY):
  					if(k < i):
  						for l in range(0, fieldX):
  							row.append(0)

  					elif(i <= k  and k-i < len(mino[minoI]) ):
  						for l in range(0, fieldX):
  							if(l < j):
  								row.append(0)
  							elif(j <= l  and l-j < len(mino[minoI][0]) ):
  								row.append(mino[minoI][k-i][l-j])
  							else:
  								row.append(0)

  					else:
  						for l in range(0, fieldX):
  							row.append(0)

  				out.append(row)
  				rowCount += 1

  f.write("{},{},{}\n".format(rowCount, fieldX * fieldY,numGroup))

  #行順序の入れ替え
  if(func is not None):
    func(out)

  for r in range(0, len(out)):
  	for x in range(0, len(out[r])):
  		f.write(str(out[r][x]))

  		if(x != len(out[r]) - 1):
  			f.write(',')

  	if(r != len(out) - 1):
  		f.write('\n')

  f.close()

#普通のExact Cover問題でミノのインスタンスを作った場合
def makeMinoFileEC(fileName, fieldX, fieldY, mino, minoG):
  f = open(fileName, 'w')

  numGroup = max(minoG)
  rowCount = 0
  out = [] #アウトプットする行を入れる

  for minoI in range(0, len(mino)):
    #座標(i,j) にミノの左上が来るようにする
    for i in range(0, fieldY):
      for j in range(0,fieldX):
        #横幅・縦幅が足りているか
        if((j + len(mino[minoI][0])) <= fieldX and
          (i + len(mino[minoI])) <= fieldY):
          #列の作成
          row = []
          #グループ情報の付加
          for k in range(0, numGroup + 1):
            if(k == minoG[minoI]):
              row.append(1)
            else:
              row.append(0)

          out.append(row)
          rowCount += 1
          
          for k in range(0, fieldY):
            if(k < i):
              for l in range(0, fieldX):
                row.append(0)

            elif(i <= k  and k-i < len(mino[minoI]) ):
              for l in range(0, fieldX):
                if(l < j):
                  row.append(0)
                elif(j <= l  and l-j < len(mino[minoI][0]) ):
                  row.append(mino[minoI][k-i][l-j])
                else:
                  row.append(0)

            else:
              for l in range(0, fieldX):
                row.append(0)

  f.write("{},{}\n".format(rowCount, fieldX * fieldY + numGroup + 1))

  for r in range(0, len(out)):
    for x in range(0, len(out[r])):
      f.write(str(out[r][x]))

      if(x != len(out[r]) - 1):
        f.write(',')

    if(r != len(out) - 1):
      f.write('\n')

  f.close()


def makeMinoFile3D(fileName, fieldX, fieldY, fieldZ, mino, minoG, func = None):
  f = open(fileName, 'w')

  numGroup = max(minoG)
  rowCount = 0
  out = [] #アウトプットする行を入れる

  for minoI in range(0, len(mino)):
    #座標(i,j,k) にミノの左上が来るようにする
    for i in range(0, fieldZ):
      for j in range(0,fieldY):
        for k in range(0, fieldX):
        #横幅・縦幅・奥行が足りているか
          if((k + len(mino[minoI][0][0])) <= fieldX and
            (j + len(mino[minoI][0])) <= fieldY and
            (i + len(mino[minoI])) <= fieldZ):
            #列の作成
            row = []

            #余分な要素が追加されてしまう問題
            try:
              row.append(minoG[minoI])
            except IndexError:
              print(minoI)

            for l in range(0, fieldZ):
              if(l < i):
                for m in range(0, fieldY):
                  for n in range(0, fieldX):
                    row.append(0)

              elif(i <= l and l-i < len(mino[minoI]) ):
                for m in range(0, fieldY):
                  if(m < j):
                    for n in range(0, fieldX):
                      row.append(0)

                  elif(j <= m  and m-j < len(mino[minoI][0]) ):
                    for n in range(0, fieldX):
                      if(n < k):
                        row.append(0)
                      elif(k <= n  and n-k < len(mino[minoI][0][0]) ):
                        try:
                          row.append(mino[minoI][l-i][m-j][n-k])
                        except IndexError:
                          print(minoI)
                          print("l-i = {}, m-j = {}, n-k = {}".format(l-i,m-j,n-k))
                          print(mino[minoI])
                      else:
                        row.append(0)

                  else:
                    for n in range(0, fieldX):
                      row.append(0)

              else:
                for m in range(0, fieldY):
                  for n in range(0, fieldX):
                    row.append(0)

            out.append(row)
            rowCount += 1

  f.write("{},{},{}\n".format(rowCount, fieldX * fieldY * fieldZ ,numGroup))

  if(func is not None):
    func(out)

  for r in range(0, len(out)):
    for x in range(0, len(out[r])):
      f.write(str(out[r][x]))

      if(x != len(out[r]) - 1):
        f.write(',')

    if(r != len(out) - 1):
      f.write('\n')

  f.close()