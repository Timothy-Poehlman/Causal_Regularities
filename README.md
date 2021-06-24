# Causal_Regularities - Python Algorithm
You can also get this tutorial and run it on your laptop:

git clone https://github.com/ipython/ipython-in-depth
Install IPython and Jupyter:

with conda:

conda install ipython jupyter
with pip:

# first, always upgrade pip!
pip install --upgrade pip
pip install --upgrade ipython jupyter
Start the notebook in the tutorial directory:

cd ipython-in-depth
jupyter notebook

# Causal_Regularities - C Algorithm
How to run:
$ make
$ ./causal

Files:
Makefile - Used to compile the programs
Main.c - Holds the steps from baumgartner
conditionlist.c/.h - Helper functions and struct for ConditionList
pairlist.c/.h - Helper functions and struct for PairList
helper.c/.h - General helper functions for Main.c
thread.c/.h - Helper function for multithreading
queue.c/.h - Helper functions and struct for Queue