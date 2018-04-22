from ij import IJ, ImagePlus
from ij.process import ByteProcessor
from ij.gui import RotatedRectRoi
from ij.plugin.frame import RoiManager
from array import zeros
import math
import copy
import time

# Last activated image
imp = IJ.getImage()  

# Print image details  
print "title:", imp.title  

def newImage(title, pixels, width, height):
	bp = ByteProcessor(width, height, pixels)
	return ImagePlus(title, bp)

def getBeamPixel(r,R):
	return 1 - 0.5 * ( 1 + math.cos((math.pi*r)/R))

def getBeamPixelAbsolute(x,y,R,a,b,theta):
	tantheta = math.tan(theta)
	r = math.fabs(tantheta*(x - a) - y + b)/math.sqrt(1 + tantheta*tantheta)
	return getBeamPixel(r,R)

def getSquaredError(imageProc, virtualImageProc, a, b, width, length, theta):
	
	roi = RotatedRectRoi(a, 
	b, 
	a + length * math.cos(theta), 
	b -length * math.sin(theta), 
	width)
	
	points = roi.getContainedPoints()
	sumError = 0;
	
	for i in xrange(len(points)):
		x = points[i].x
		y = points[i].y
		bp = getBeamPixelAbsolute(x,y,width/2,a,b, math.pi - theta)
		imageVal = imageProc.getInterpolatedValue(x/3.0,y/3.0)
		err = bp*255 - imageVal
		sumError += err*err
		#virtualImageProc.putPixelValue(x, y, bp*255)

	virtualImageProc.setRoi(roi)
	virtualImageProc.setValue(255*sumError/500000000)
	virtualImageProc.fill(roi.getMask())
	
	return sumError

def step(vi, im, imageProc, virtualImageProc, a, b, width, length, lastTheta, angleStep, i):


	imageProc.putPixelValue(int(math.floor(a/3)), int(math.floor(b/3)), 255)
	imageProc.putPixelValue(int(math.floor(a/3)) + 1, int(math.floor(b/3)), 255)
	imageProc.putPixelValue(int(math.floor(a/3)), int(math.floor(b/3)) + 1, 255)
	imageProc.putPixelValue(int(math.floor(a/3)) -1, int(math.floor(b/3)), 255)
	imageProc.putPixelValue(int(math.floor(a/3)), int(math.floor(b/3)) -1, 255)
	imageProc.putPixelValue(int(math.floor(a/3)) -1, int(math.floor(b/3)) -1, 255)
	imageProc.putPixelValue(int(math.floor(a/3)) +1, int(math.floor(b/3)) +1, 255)
	imageProc.putPixelValue(int(math.floor(a/3)) -1, int(math.floor(b/3)) +1, 255)
	imageProc.putPixelValue(int(math.floor(a/3)) -1, int(math.floor(b/3)) +1, 255)
	
	numSteps = int(math.floor(math.pi/angleStep))

	minErr = 0
	newTheta = lastTheta
	
	for x in xrange(numSteps):
	
		alpha = 0
	
		if x%2 == 0:
			alpha = lastTheta + angleStep*(x/2)
		else:
			alpha = lastTheta - angleStep*(x/2)

		sqerr = getSquaredError(imageProc, virtualImageProc, a, b, width, length, alpha)

		if x == 0 or sqerr < minErr:
			minErr = sqerr
			newTheta = alpha
		
	newA = a +0.5*length * math.cos(newTheta)
	newB = b -0.5*length * math.sin(newTheta)

	vi.updateAndDraw()
	im.updateAndDraw()

	if i < 18:
		step(vi,im, imageProc, virtualImageProc, newA, newB, width, length, newTheta, angleStep, i + 1)
	

pixels = imp.getProcessor().convertToByteProcessor().getPixels()
imageScratch = newImage("ImageScratch", copy.copy(pixels), imp.width, imp.height)
# Fine mesh 3x resolution of original image.
virtualImage = newImage("VirtualImage", zeros('b', len(pixels*9)), imp.width*3, imp.height*3)

manager = RoiManager.getRoiManager()
point = manager.getRoi(0).getContainedPoints()[0]
akx = point.x*3
aky = point.y*3
width = 50*3
length = 35*3

angleStep = 0.2


virtualImageProc = virtualImage.getProcessor()
imageProc = imageScratch.getProcessor()


imageScratch.show()
virtualImage.show()

step(virtualImage, imageScratch, imageProc, virtualImageProc, akx, aky, width, length,0, angleStep, 0)

