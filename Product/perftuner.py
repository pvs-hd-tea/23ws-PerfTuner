from pathlib import Path
from PerfTunerClass import PerfTuner

print("----------------------------------------------------------------------------------------")
print("Welcome to PerfTuner!")
print("Your AVX transformation journey begins here.")
print("----------------------------------------------------------------------------------------")
print("")
print("You need to prepare two files in the same directory:")
print("function.cc - containing the C++ function you want to optimize. \
The function needs to be called function(...) and should have no return value.")
print("main.cc - containing a test environment for function(...). \
This will be used to check wether the transformation was successfulll.")
print("")

path = input("Enter the path to the files function.cc and main.cc starting from this directory: ")
script_dir = Path(__file__).resolve().parent
files_path = script_dir / path
print("")
runs_useSnippet = input("How many snippets should be tested: ")
print("")
runs_buildSnippet = input("How many trials should be tested: ")
print("")    

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



# Initializing perfTuner
perfTuner = PerfTuner(files_path,runs_useSnippet,runs_buildSnippet,method)
perfTuner.do()