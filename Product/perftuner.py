from pathlib import Path
from PerfTunerClass import PerfTuner

# this file is run by the user
# the user has to input the path to the file that he wants to transform
# he can choose how many snipptes should be tried in how many trials

print("----------------------------------------------------------------------------------------")
print("Welcome to PerfTuner!")
print("Your AVX transformation journey begins here.")
print("----------------------------------------------------------------------------------------")
print("")
print("You need to prepare two files in the same directory:")
print("function.cc - containing the C++ function you want to optimize.")
print("     The function needs to be called function(...) and should have no return value.")
print("main.cc - containing a test environment for function(...).")
print("     This will be used to check wether the transformation was successfulll.")
print("")

# get the path to the directory of the used files
path = input("Enter the path to the files function.cc and main.cc starting from this directory: ")
script_dir = Path(__file__).resolve().parent
files_path = script_dir / path
print("")

# get the number of snippets 
runs_useSnippet = input("How many snippets should be tested: ")
print("")

# get the number of trials
runs_buildSnippet = input("How many trials should be tested: ")
print("")    

# get the method for finding the snippet list
while(True):
    choice = input("Which method should be used to find the snippets? Type 1 for tournament and 2 for voting: ")
    if (choice == "1"):
        method = "tournament"
        break
    if (choice == "2"):
        method = "voting"
        break
    print("Incorrect choice for method. Please try again.")

print("")

# initialize the PerfTuner
perfTuner = PerfTuner(files_path,runs_useSnippet,runs_buildSnippet,method)

# do the transformation
perfTuner.do()