#!/bin/bash

echo "========================================"; \
echo "             GAME RULES:               "; \
echo " 1. Click on enemies to destroy them.   "; \
echo " 2. Red : 10 points   "; \
echo " 3. Cyan : 7 points         "; \
echo " 4. Blue : 5 points  "; \
echo " 5. Magenta : 3 points"; \
echo " 5. Yellow : 1 point"; \
echo " 5. Green : -5 points"; \
echo "========================================";


cd /home/surajit/Documents/game

# Execute the corresponding Makefile

make game           # Build the game
./game              # Run the game
make clean          # Clean the build files

