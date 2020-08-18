/*
  ==============================================================================

    CoordinatePlot.cpp
    Created: 10 Aug 2020 5:23:41pm
    Author:  Blair

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CoordinatePlot.h"
#include <iomanip>
#include <sstream>

//==============================================================================
CoordinatePlot::CoordinatePlot()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setGridLineCount();
    setRange(); //sets to default
    initCoords(75.0f, 75.0f);
}

CoordinatePlot::~CoordinatePlot() {}
CoordinatePlot::Listener::Listener() {}
CoordinatePlot::Listener::~Listener() {}

void CoordinatePlot::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (juce::Colours::grey);
    drawPlot(g);
    g.setColour(juce::Colours::orange);
    drawMarker(g);
    g.setColour(juce::Colours::white);
    if (markerMoved) { drawText(g); }

    // capture raw range for reference when resizing
    setRangeRaw();
}

void CoordinatePlot::resized()
{
    setSettings();
    updateCoords();
}

void CoordinatePlot::mouseDown(const juce::MouseEvent& event)
{
    DBG("Mouse Clicked on plot at: " << getX() << "," << getY());
    markerMoved = true;
    setMouseCursor(juce::MouseCursor::NoCursor);

    setCoords(float(event.getMouseDownX()), float(event.getMouseDownY()));
    interactWithComponent();
    repaint();
}

void CoordinatePlot::mouseDrag(const juce::MouseEvent& event)
{
    DBG("Mouse dragged to: " << getX() << ", " << getY());
    juce::Point<int> rawPos(event.getPosition());
    float rawX = float(rawPos.getX());
    float rawY = float(rawPos.getY());

    setCoords(rawX, rawY);
    interactWithComponent();
    repaint();
}

void CoordinatePlot::mouseUp(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void CoordinatePlot::interactWithComponent()
{
    listeners.call([this](Listener& l) { l.coordPlotValueChanged(this); });
}

void CoordinatePlot::addListener(Listener* l) { listeners.add(l); }

void CoordinatePlot::removeListener(Listener* l) { listeners.remove(l); }

float CoordinatePlot::getX() { return constrain(coordsRaw['x']); }

float CoordinatePlot::getY()
{
    return invertCoord(constrain(coordsRaw['y']), range['min'], range['max']);
}

void CoordinatePlot::setGridLineCount(int lineCount)
{
    if (lineCount % 2 == 1){--lineCount;}
    gridLineCount = lineCount;
}

void CoordinatePlot::setRange(float min, float max)
{
    range['min'] = min;
    range['max'] = max;
}

void CoordinatePlot::initCoords(float rawX, float rawY)
{
    coordsRaw['x'] = rawX;
    coordsRaw['y'] = rawY;
}

void CoordinatePlot::setCoords(float rawX, float rawY)
{
    if(inRangeRaw(rawX, rawY)) { coordsRaw['x'] = rawX, coordsRaw['y'] = rawY; }
}

void CoordinatePlot::updateCoords()
{
    //get ratios based off initial range
    double xRatio = double(coordsRaw['x'] / (rangeRaw['max'] - rangeRaw['min']));
    double yRatio = double(coordsRaw['y'] / (rangeRaw['max'] - rangeRaw['min']));

    // new x and y based off current size and previous ratio
    float newX = float(right * xRatio);
    float newY = float(bottom * yRatio);

    setCoords(newX, newY);
}

void CoordinatePlot::setRangeRaw()
{
    rangeRaw['min'] = getLocalBounds().getX();
    rangeRaw['max'] = getLocalBounds().getWidth();
}

void CoordinatePlot::drawPlot(juce::Graphics& g)
{
    g.drawRect(getLocalBounds(), 3);// draw an outline around the component
    drawAxis(g);
    drawGrid(g);
}

void CoordinatePlot::drawAxis(juce::Graphics& g)
{
    //draw x and y axis
    g.drawLine(left, midY, right, midY, 2);
    g.drawLine(midX, left, midX, bottom, 2);
}

void CoordinatePlot::drawGrid(juce::Graphics& g)
{
    const float myDashLength[] = { 3, 3 };
    float offset = float(getLocalBounds().getWidth() / (gridLineCount + 2));

    for (int i = 0; i < (gridLineCount/2); ++i)
    {
        int d{ i + 1 }; //degrees away from axis
        //draw to left/right of Y-axis and top/bottom of X-axis
        g.drawDashedLine(juce::Line<float>(midX - offset * d, top, midX - offset * d, bottom),
            &myDashLength[0], 2, 1.0, 0);
        g.drawDashedLine(juce::Line<float>(midX + offset * d, top, midX + offset * d, bottom),
            &myDashLength[0], 2, 1.0, 0);
        g.drawDashedLine(juce::Line<float>(left, midY - offset * d, right, midY - offset * d),
            &myDashLength[0], 2, 1.0, 0);
        g.drawDashedLine(juce::Line<float>(left, midY + offset * d, right, midY + offset * d),
            &myDashLength[0], 2, 1.0, 0);
    }
}

void CoordinatePlot::drawMarker(juce::Graphics& g)
{
    //set length of cursor
    float length = float(getLocalBounds().getWidth() / 15);

    //create lines
    juce::Line<float> lineH(juce::Point<float>(coordsRaw['x'] - length, coordsRaw['y']),
        juce::Point<float>(coordsRaw['x'] + length, coordsRaw['y']));
    juce::Line<float> lineV(juce::Point<float>(coordsRaw['x'], coordsRaw['y'] - length),
        juce::Point<float>(coordsRaw['x'], coordsRaw['y'] + length));
    
    //draw lines
    g.drawLine(lineH, 2.0f);
    g.drawLine(lineV, 2.0f);
}


void CoordinatePlot::drawText(juce::Graphics& g)
{
    g.setFont(float(getWidth()/12));
    int textHeight = int(g.getCurrentFont().getHeight());

    //Draw Y
    std::stringstream streamY;
    streamY << std::fixed << std::setprecision(2) << getY();
    g.drawText(streamY.str(), int(midX), int(top), int(midX), textHeight, 
        juce::Justification::centredLeft, true);

    //Draw X
    std::stringstream streamX;
    streamX << std::fixed << std::setprecision(2) << getX();
    g.drawText(streamX.str(), int(midX), int(midY), int(midX), textHeight, 
        juce::Justification::centredRight, true);
}

void CoordinatePlot::setSettings()
{
    //recalculate all the settings
    midY = float(getLocalBounds().getCentreY());
    midX = float(getLocalBounds().getCentreX());
    left = float(getLocalBounds().getX());
    right = float(getLocalBounds().getRight());
    top = float(getLocalBounds().getY());
    bottom = float(getLocalBounds().getBottom());
}

float CoordinatePlot::constrain(float coord)
{
    float oldRangeMin = float(getLocalBounds().getX());
    float oldRangeMax = float(getLocalBounds().getWidth());
    float oldRange = oldRangeMax - oldRangeMin;
    float newRange = range['max'] - range['min'];

    float newValue = (((coord - oldRangeMin) * newRange) / oldRange) + range['min'];
    return newValue;
}

/**Inverts coord within a range between min and max*/
float CoordinatePlot::invertCoord(float coord, float min, float max)
{
    return (min + max) - coord;
}
bool CoordinatePlot::inRangeRaw(float rawX, float rawY)
{
    return (rawX >= left && rawX <= right && rawY >= top && rawY <= bottom);
}