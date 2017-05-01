#!/usr/bin/python
# http://slhck.info/articles/crf

'''
Usage:
! Removes audio by default
-s is starting second
-f is Fps
-c is Crf ( 23 is default quality, higher is worse, 45 is bad)
-t is cropped max time ( t=5, the file will be 5 sec long)
python compress.py -i short.mov -f -c 42 -s 20 -t 5


'''

import sys, getopt, subprocess,os

def main(argv):
   inputfile = ''
   outputfile = ''
   fps=''
   crf=''
   ss='0'
   # domysnie usun dzwiek
   avconvArgs=''
   time=''
   try:
      opts, args = getopt.getopt(argv,"hi:f:c:o:t:s:",["ifile=","ofile="])
   except getopt.GetoptError:
      print '-i <input> -fps <fps> -crf <crf> -o <[optional]output>'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print '-i <input> -f <fps> -c <crf> -o <[optional]output>'
         sys.exit()
      elif opt in ("-i", "--ifile"):
         inputfile = arg
      elif opt in ("-o", "--ofile"):
         outputfile = arg
      elif opt  == '-f':
         fps = arg
      elif opt == '-c':
      	 crf = arg
      elif opt=='-t':
          time=arg
      elif opt=='-s':
          ss=arg
   avconvArgs+="-i "+inputfile+" -c:v libx264 -an -ss "+ss
   if outputfile == '':
      outputFile=inputfile.split(".")[0]
      if time!='':
         koniec=int(ss)+int(time)
         outputFile+="-"+ss+"-"+str(koniec)+"s"
         avconvArgs+=" -t "+time
      if fps != '':
         outputFile+="-"+fps+"fps"
         avconvArgs+=" -r "+fps
      if crf!='':
         outputFile+="-"+crf+"crf"
         avconvArgs+=" -crf "+crf
      outputFile+='.'+inputfile.split(".")[1]
   avconvArgs+=" "+outputFile
   print 'Input: ', inputfile
   print 'Fps: ', fps
   print 'Crf: ', crf
   print 'StartTime: ',ss
   print 'Duration: ', time
   print 'Output: ', outputFile
   print 'Running: avconv',avconvArgs

   os.system('avconv '+avconvArgs)
if __name__ == "__main__":
   main(sys.argv[1:])




