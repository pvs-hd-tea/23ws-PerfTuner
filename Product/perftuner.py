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
    choice = input("Which method should be used to find the snippets? Type 1 for tournament, 2 for voting or 3 for a default snippet: ")
    if (choice == "1"):
        method = "tournament"
        break
    if (choice == "2"):
        method = "voting"
        break
    if (choice == "3"):
        method = "default"
        break
    print("Incorrect choice for method. Please try again.")

print("")

# get the number of the desired default snippet
defaultSnippet = "snippet"
if (method == "default"):
    if (int(runs_useSnippet) > 1):
        runs_useSnippet = 1
        print("By choosing the default method, only one snippet is used!")
    while(True):
        choice = int(input("Give the number of the default snippet to be used (1-11): "))
        if (choice > 0 and choice < 12):
            break
        print("Incorrect choice for number of snippet. Please try again.")
else:
    choice = 10 # default snippet, should not be needed.

defaultSnippet += str(choice)

print(defaultSnippet)



# initialize the PerfTuner
perfTuner = PerfTuner(files_path,runs_useSnippet,runs_buildSnippet,method,defaultSnippet)

# do the transformation
perfTuner.do()

