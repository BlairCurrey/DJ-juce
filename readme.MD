## Attributions
This application is an extension of an application developed by the University of London. Some of the work is not my own. [Please see here for more details](#more-details-on-attribution)

## Features
[See my tutorial of the application on youtube](https://www.youtube.com/watch?v=iOKKWV8OHlw)
- Directly load a song into each deck or import a library to load from
- Change volume, speed, and position
- Set the reverb attributes using the coordinate plot for each player
  - (Top) x-axis: Damping, y-axis: Room Size
  - (Bottom) x-axis: Dry level, y-axis: Wet Level
- Displays the file name and a waveform of the track

![A view of the application in action](/screenshot.jpg)

#### More Details on Attribution
The University of London made the MainComponent, DeckGUI, DJAudioPlayer, and WaveformDisplay files, all of which I have modified. The nature of these changes mostly affect the GUI or enabled the implementation of my own code. These changes include but are not limited to changing the layout of components, adding labels to sliders, adding the song name to the waveform, adding a reverb audio source, and defining event listener callbacks. The University also started a rough outline of the Playlist Component files which I finished and ultimately wrote the majority of. The CoordinatePlot and Track files are solely my work.