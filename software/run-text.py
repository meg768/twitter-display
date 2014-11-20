# -*- coding: utf-8 -*-


import sys, getopt
import PIL
from PIL import ImageFont
from PIL import Image
from PIL import ImageDraw

def main(argv):
	outputFile = 'a.png'
	fontSize = 16   
	fontName = 'Verdana'
	textColor = 'red'
	text = unicode("ÅÄÖ", "UTF-8")
	
	try:
		opts, args = getopt.getopt(argv,"o:t:p:f:c:")

	except getopt.GetoptError:
		print 'Upps!'
		sys.exit(2)
	
	for opt, arg in opts:
		if opt == '-t':
			text = unicode(arg, "UTF-8")
	
		elif opt == "-o":
			outputFile = arg

		elif opt == "-p":
			fontSize = int(arg)

		elif opt == "-f":
			fontName = arg

		elif opt == "-c":
			textColor = arg

   
	font = ImageFont.truetype("fonts/" + fontName + ".ttf",fontSize)
	
	w, h = font.getsize(text)

	img = Image.new("RGBA", (w,32),(0,0,0))
	draw = ImageDraw.Draw(img)
	draw.text((0, (32-h)/2),text,textColor,font=font)
	draw = ImageDraw.Draw(img)
	img.save(outputFile)
	
	

main(sys.argv[1:])
