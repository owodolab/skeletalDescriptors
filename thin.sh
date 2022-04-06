 #!/bin/bash

PATHTO=$PWD
THINEXEC=$PATHTO/src/graspi
RESULTSDIRECTORY=$PATHTO/examples/result/
DATA=$PATHTO/examples/two-phase_morphology/
cd $DATA
for i in *.txt; do
    echo "skeletonizing file..."
    FILENAME=$i
    BASEFILENAME=`echo ${i} | sed 's/.txt//'` #remove txt-file extension
    echo ${i}
    $THINEXEC $FILENAME $RESULTSDIRECTORY/$BASEFILENAME.txt
done
