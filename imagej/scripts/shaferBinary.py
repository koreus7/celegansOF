from ij import IJ, ImagePlus
from ij.gui import Roi
from ij.process import ByteProcessor
from array import zeros
import copy


# Last activated image
imp = IJ.getImage()

def newImage(title, pixels, width, height):
	bp = ByteProcessor(width, height, pixels)
	return ImagePlus(title, bp)

windowSize = 5

# Print image details  
print "title:", imp.title  

processor = imp.getProcessor().convertToByteProcessor()
pixels = processor.getPixels()
newPixels = copy.copy(pixels)

backgroundValue = 	(processor.get(1,1) 
					+ processor.get(1,processor.height - 1)
					+ processor.get(processor.width - 1, processor.height - 1) 
					+ processor.get(1, processor.height - 1)) / 4

for y in range(processor.height):
    for x in range(processor.width):
		roi = Roi(x - windowSize/2, y - windowSize/2 , windowSize, windowSize)
		processor.setRoi(roi)
		stats = processor.getStatistics()
		standardDeviation = stats.stdDev
		mean = stats.mean
		if mean < 0.7 * backgroundValue or standardDeviation > 0.3 * mean:
			newPixels[y * processor.width + x] =  127
		else:
			newPixels[y * processor.width + x] = 0

resultImage = newImage("Shafer Threshold", newPixels, imp.width, imp.height)
resultImage.updateAndDraw()
resultImage.show()

IJ.setThreshold(resultImage, 1,128);

#This allows imageJ to see it as a binary image.
IJ.run(resultImage, "Convert to Mask", "");

#Make it a white worm on a black background.
#IJ.run(resultImage, "Invert", "");