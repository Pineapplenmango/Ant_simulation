doxyfile config:

PROJECT_NAME           = "Ant Simulation"
OUTPUT_DIRECTORY       = docs
RECURSIVE              = YES
GENERATE_LATEX         = NO
EXTRACT_ALL            = YES
INPUT                  = src
FILE_PATTERNS          = *.h *.cpp
GENERATE_HTML          = YES
HAVE_DOT               = YES
CALL_GRAPH             = YES
INCLUDE_GRAPH          = YES
DOT_IMAGE_FORMAT       = svg

path for index file:
"C:\Users\Andre\Documents\vscodeprojects\Ant_simulation\docs\html\index.html"

*run this in browser*


**giv forklaring for hvorfor denne config**

/**
 * @mainpage Ant Colony Simulation
 *
 * @section intro_sec Introduction
 * This project implements an ant colony simulation using
 * Ant Colony Optimization principles.
 *
 * Two architectures are implemented:
 * - Object-Oriented Design (OOP)
 * - Data-Oriented Design (DOD)
 *
 * The DOD version focuses on cache locality and performance.
 */