#!/bin/bash
#twopi -Tpng -O map.dot
#twopi -x -v -Tpng map.dot > data.png
#twopi -v -Tsvg map.dot -o data.svg
#tred map.dot | sfdp -v -Tsvg -o data.svg
#tred map.dot | dot -v -Tsvg -o data.svg	#unable to get results. :(
#tred map.dot | twopi -v -Tsvg -o data.svg
#tred map.dot | fdp -v -Tsvg -o data.svg		#nothing... well a dot.
tred map.dot | neato -v -Tsvg -o data.svg	#amazing! but takes a good 5 minutes
#patchwork
#circo
#dot -v -Tsvg map.dot -o data.svg			#nothing
#twopi -v map.dot | 
#neato -v -Nlabel="" -Tsvg map.dot -o data.svg
#sfdp -v -Tsvg map.dot > data.svg
xdg-open data.svg
#xdg-open map.pdf