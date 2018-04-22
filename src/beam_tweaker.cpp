#include "beam_tweaker.h"
#define SQRT_2_PI 2.50662827463
#define SQRT_3 1.7320508076

template<typename T>
std::vector<T>
conv(std::vector<T> const &f, std::vector<T> const &g) {
    int const nf = f.size();
    int const ng = g.size();
    int const n  = nf + ng - 1;
    std::vector<T> out(n, T());
    for(auto i(0); i < n; ++i) {
        int const jmn = (i >= ng - 1)? i - (ng - 1) : 0;
        int const jmx = (i <  nf - 1)? i            : nf - 1;
        for(auto j(jmn); j <= jmx; ++j) {
            out[i] += (f[j] * g[i - j]);
        }
    }
    return out;
}

template<typename T>
std::vector<T>
conv_valid(std::vector<T> const &f, std::vector<T> const &g) {
    int const nf = f.size();
    int const ng = g.size();
    std::vector<T> const &min_v = (nf < ng)? f : g;
    std::vector<T> const &max_v = (nf < ng)? g : f;
    int const n  = std::max(nf, ng) - std::min(nf, ng) + 1;
    std::vector<T> out(n, T());
    for(auto i(0); i < n; ++i) {
        for(int j(min_v.size() - 1), k(i); j >= 0; --j) {
            out[i] += min_v[j] * max_v[k];
            ++k;
        }
    }
    return out;
}

template<typename T>
std::vector<T>
conv_same(std::vector<T> const &f, std::vector<T> const &g) {

    std::vector<T> result = conv_valid(f, g);

    long requiredPadding = f.size() - result.size();

    T startPadding = result[0];
    T endPadding =  result[result.size() - 1];

    for(long i = 0; i < requiredPadding/2; i++) {

        result.push_back(startPadding);
        //TODO deque instead.
        result.insert(result.begin(),endPadding);

    }

    return result;

}

float gauss(float x, float sigma) {

    float exponent = (float)(  -pow(x, 2)/(2*pow(sigma, 2))  );
    return  (float)(  exp(exponent)/(sigma*SQRT_2_PI)  );
}

float gaussp(float x, float sigma) {

    float exponent = (float)(  -pow(x, 2)/(2*pow(sigma, 2))  );
    return (float)(  -x*exp(exponent)/(pow(sigma,3)*SQRT_2_PI)  );
}

float gausspp(float x, float sigma) {

    float exponent = (float)(  -pow(x, 2)/(2*pow(sigma, 2))  );
    return (float)( (pow(x,2) - pow(sigma,2)) * exp(exponent)/(pow(sigma,5)*SQRT_2_PI)  );

}



void gaussianFilter(int size, float sigma, std::vector<float>& out) {


    for(int j = 0; j < size; j++)
    {
        float x =  2  * ( j - floor(size/2.0f) ) / ( 0.5f*(size - 1) );

        float filterValue = gauss(x, sigma);

        out.push_back(filterValue);

    }

}

void gaussianFilterPrime(int size, float sigma, std::vector<float>& out) {

    const float scale = sigma*SQRT_3;

    for(int j = 0; j < size; j++)
    {
        float x = 2 * scale *  ( j - floor(size/2.0f) ) / ( 0.5f*(size - 1) );

        float filterValue = gaussp(x, sigma);

        out.push_back(filterValue);

    }

}

void gaussianFilterDoublePrime(int size, float sigma, std::vector<float>& out) {

    const float scale = sigma*SQRT_3;

    for(int j = 0; j < size; j++)
    {
        float x = 2 * scale * ( j - floor(size/2.0f) ) / ( 0.5f*(size - 1) );

        float filterValue = gausspp(x, sigma);

        out.push_back(filterValue);

    }


}


template<typename T>
T max_index (vector<T> v) {

    return std::distance(v.begin(), std::max_element(v.begin(), v.end()));
}

template<typename T>
T max_val (vector<T> v) {

    return v[max_index(v)];
}


struct PotentialEdge {
    ofVec2f point;
    float measuredIntensity;
    float derivative;
    int index;
};


bool compareByDerivative(const PotentialEdge &a, const PotentialEdge &b)
{
    return a.derivative > b.derivative;
}

bool compareByIndex(const PotentialEdge &a, const PotentialEdge &b)
{
    return a.index < b.index;
}


const void BeamTweaker::tweak(const BeamParameters &parameters, ofImage *originalImage, ofPolyline &fittedLine)
{
    for(int i = 1; i < fittedLine.size(); i++)
    {
        ofPoint currentLinePoint = fittedLine[i];
        ofPoint previous = fittedLine[i - 1];

        float backgroundIntensity = (float)originalImage->getPixels().getColor(0,0).r/255.0;

        ofVec2f diff  = (currentLinePoint - previous);
        diff.normalize();

        vector <float> measured;
        vector <ofVec2f> samplePoints;

        const int numSamples = (int)floor(parameters.beamWidth*2.0f);


        std::cout << "measured  {";
        for(int j = 0; j < numSamples; j++)
        {
            ofVec2f samplePoint = currentLinePoint + ofVec2f(diff.y, -diff.x)*parameters.beamWidth
                                  + ofVec2f(-diff.y, diff.x)*j;

            samplePoints.push_back(samplePoint);

            int x = (int)floor(samplePoint.x);
            int y = (int)floor(samplePoint.y);

            float sample = 0.0f;

            if(x >= 0 && x < originalImage->getWidth() && y >= 0 && y < originalImage->getHeight()) {

                sample = originalImage->getPixels().getColor(x,y).r/255.0f;

            }

            std::cout << sample << " , ";
            measured.push_back(sample);

        }

        std::cout << "}" << endl;



        vector <float> filter;

        float sigma = parameters.beamWidth/SQRT_3;

        //const int filterSize = (int)floor(parameters.beamWidth);

        // Smoothing and lowpass
        gaussianFilter(21, 0.5, filter);
        vector<float> smoothed = conv_same(measured,  filter);

        vector <float> filter1;
        gaussianFilterPrime(25, sigma, filter1);
        vector<float> firstDerivatives = conv_same(smoothed, filter1);

        vector <float> filter2;
        gaussianFilterDoublePrime(25, sigma, filter2);
        vector<float> secondDerivatives = conv_same(smoothed, filter2);

        std::cout << "d2filter {";
        for(long i = 0; i < filter2.size(); i++) {
            std::cout << filter2[i]*10000 << " , ";
        }
        std::cout << "}" << endl;




        float maxsmooth = max_val(smoothed);
        std::cout << "smoothed {";
        for(int j = 0; j < smoothed.size(); j++) {

            ofVec2f point = samplePoints[j];

            std::cout << smoothed[j] << " , ";

            //originalImage->setColor((int)floor(point.x), (int)floor(point.y), ofColor(255.0*smoothed[j]/maxsmooth,0,0));

        }
        std::cout << "}" << endl;

        std::cout << "firstd {";
        float maxfirstd = max_val(firstDerivatives);
        for(int j = 0; j < firstDerivatives.size(); j++) {

            ofVec2f point = samplePoints[j];

            firstDerivatives[j] = fabs(firstDerivatives[j]);

            std::cout << firstDerivatives[j] << " , ";

            originalImage->setColor((int)floor(point.x), (int)floor(point.y), ofColor(255.0f*firstDerivatives[j]/maxfirstd,0,0));

        }

        std::cout << "}" << endl;
        float maxsecondd = max_val(secondDerivatives);
        std::cout << "secondd {";
        for(int j = 0; j < secondDerivatives.size(); j++) {

            ofVec2f point = samplePoints[j];

            //secondDerivatives[j] = fabs(secondDerivatives[j]);

            std::cout << 1000.0*secondDerivatives[j] << " , ";

            originalImage->setColor((int)floor(point.x), (int)floor(point.y), ofColor(0,255.0f*secondDerivatives[j]/maxsecondd,0));

        }
        std::cout << "}" << endl;

        std::cout << endl << endl;


        ofVec2f adjusted =  samplePoints[max_index(secondDerivatives)];
        originalImage->setColor((int)floor(adjusted.x), (int)floor(adjusted.y), ofColor(255,0,255));


        int max1stIndex = max_index(firstDerivatives);
        ofVec2f max1st =  samplePoints[max1stIndex];

        // Non maximum suppression.
        for(int j = 0; j < (int)floor(parameters.beamWidth*0.6f); j++) {

            if(max1stIndex + j < firstDerivatives.size()) {

                firstDerivatives[max1stIndex + j] = 0.0f;

            }

            if(max1stIndex - j >= 0) {

                firstDerivatives[max1stIndex - j] = 0.0f;

            }

        }


        ofVec2f max2nd =  samplePoints[max_index(firstDerivatives)];

        originalImage->setColor((int)floor(max1st.x), (int)floor(max1st.y), ofColor(255,255,0));
        originalImage->setColor((int)floor(max2nd.x), (int)floor(max2nd.y), ofColor(255,255,0));


        ofVec2f direction =  max1st - max2nd;
        float len = direction.length();
        direction.normalize();

        fittedLine[i] = max2nd + direction*len*0.5;


    }

    originalImage->update();
}
