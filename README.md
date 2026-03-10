# Undercover Most Wanted Physics

Experiment for Need for Speed: Undercover that converts the physics model from Most Wanted and Carbon

Massive thanks to everyone involved with [dbalatoni's MW reverse engineering project](https://github.com/dbalatoni13/nfsmw), this mod was heavily based off of it.

## Installation

- Make sure you have v1.0.0.1 of the game, as this is the only version this plugin is compatible with. (exe size of 10584064 or 10589456 bytes)
- Plop the files into your game folder.
- Enjoy, nya~ :3

## Known issues

- The clutch might occasionally get stuck and leave you unable to drive, release and re-press the gas pedal to fix it
- Launching at race starts might not work if you're fully revved as the counter hits 0

## Building

Building is done on an Arch Linux system with CLion and vcpkg being used for the build process. 

Before you begin, clone [nya-common](https://github.com/gaycoderprincess/nya-common), [nya-common-nfsuc](https://github.com/gaycoderprincess/nya-common-nfsuc) and [CwoeeMenuLib](https://github.com/gaycoderprincess/CwoeeMenuLib) to folders next to this one, so they can be found.

Required packages: `mingw-w64-gcc`

You should be able to build the project now in CLion.
