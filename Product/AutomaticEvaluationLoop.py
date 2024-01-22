from PerfTuner import PerfTuner

print("----------------------------------------------------------------------------------------")
print("Welcome to the automatic evaluation loop of PerfTuner!")
print("This evaluation loop will try to make different programs faster using AVX2 by calling ChatGPT via meta-strategies.")
print("Let's see what happens.")
print("----------------------------------------------------------------------------------------")
print("")

### Class 1 ###
print("--- Class 1 problems ---")
print("")

problems_Class1 = 0
solved_Class1 = 0

# Scalarproduct
problems_Class1 += 1
print("a. Scalarproduct:")
print("")

perfTuner = PerfTuner("Problems/Class1/Scalarproduct")

# Output (Scalarproduct)
statistics_array = perfTuner.do()
if statistics_array != 1:
    solved_Class1 += 1
    print("SOLVED")
    print("")
else:
    print("FAIL")
    print("")

# MatrixMultiplication
problems_Class1 += 1
print("b. MatrixMultiplication:")
print("")

perfTuner = PerfTuner("Problems/Class1/MatrixMultiplication")

# Output (MatrixMultiplication)
statistics_array = perfTuner.do()
if statistics_array != 1:
    solved_Class1 += 1
    print("SOLVED")
    print("")
else:
    print("FAIL")
    print("")

### Class 2 ###
print("--- Class 2 problems ---")
print("")

problems_Class2 = 0
solved_Class2 = 0

# LU1
problems_Class2 += 1
print("a. LU1:")
print("")

perfTuner = PerfTuner("Problems/Class2/LU1")

# Output (LU1)
statistics_array = perfTuner.do()
if statistics_array != 1:
    solved_Class2 += 1
    print("SOLVED")
    print("")
else:
    print("FAIL")
    print("")

# Transpose
problems_Class2 += 1
print("b. MatrixMultiplication:")
print("")

perfTuner = PerfTuner("Problems/Class2/Transpose")

# Output (Transpose)
statistics_array = perfTuner.do()
if statistics_array != 1:
    solved_Class2 += 1
    print("SOLVED")
    print("")
else:
    print("FAIL")
    print("")

### Class 3 ###
print("--- Class 3 problems ---")
print("")

problems_Class3 = 0
solved_Class3 = 0

# LU3
problems_Class3 += 1
print("a. LU3:")
print("")

perfTuner = PerfTuner("Problems/Class3/LU3")

# Output (LU3)
statistics_array = perfTuner.do()
if statistics_array != 1:
    solved_Class3 += 1
    print("SOLVED")
    print("")
else:
    print("FAIL")
    print("")

# Output
print ("Class 1: " + str(solved_Class1) + " / " + str(problems_Class1))
print ("Class 2: " + str(solved_Class2) + " / " + str(problems_Class2))
print ("Class 3: " + str(solved_Class3) + " / " + str(problems_Class3))

