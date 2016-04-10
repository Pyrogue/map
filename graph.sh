#!/bin/bash
#twopi -Tpng -O map.dot
#twopi -x -v -Tpng map.dot > data.png
#twopi -v -Nlabel="" -Tps map.dot -o data.ps
#twopi -v -Nlabel="" -Tsvg map.dot -o data.svg
dot -v -Nlabel="" -Tsvg map.dot -o data.svg
#neato -v -Nlabel="" -Tsvg map.dot -o data.svg
#sfdp -x -v -Goverlap=scale -Tpng map.dot > data.png
xdg-open data.svg