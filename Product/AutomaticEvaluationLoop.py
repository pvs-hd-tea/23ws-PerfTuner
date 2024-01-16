from PerfTuner import PerfTuner

## Class 1 ##
problems = 1
solved = 0

# Scalarproduct
perfTuner = PerfTuner("Problems/Class1/Scalarproduct/function.cc", "Problems/Class1/Scalarproduct/function_opt.cc", "Problems/Class1/Scalarproduct/main.cc")
if perfTuner.do() != 1:
    solved += 1

# Output
print ("Class 1: " + str(solved)/str(problems))


## Class 2 ##
problems = 2
solved = 0

# LU1
perfTuner = PerfTuner("Problems/Class1/LU1/function.cc", "Problems/Class1/LU1/function_opt.cc", "Problems/Class1/LU1/main.cc")
if perfTuner.do() != 1:
    solved += 1

# Transpose
perfTuner = PerfTuner("Problems/Class1/Transpose/function.cc", "Problems/Class1/Transpose/function_opt.cc", "Problems/Class1/Transpose/main.cc")
if perfTuner.do() != 1:
    solved += 1

# Output
print ("Class 2: " + str(solved)/str(problems))


## Class 3 ##
problems = 0
solved = 0

# Output
print ("Class 3: " + str(solved)/str(problems))