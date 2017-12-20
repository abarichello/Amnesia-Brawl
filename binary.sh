#!/bin/bash

# Used to package the necessary files for releases
zip -r $1.zip game/media/
zip $1.zip game/amnesia
