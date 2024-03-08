from PerfTunerClass import PerfTuner

def main():
    print("----------------------------------------------------------------------------------------")
    print("Welcome to PerfTuner!")
    print("Your AVX transformation journey begins here.")
    print("----------------------------------------------------------------------------------------")
    print("")

    path = input("Enter path of folder containing the files function.cc and main.cc:")
    print("")
    

    # Initializing perfTuner
    perfTuner = PerfTuner(path)
    perfTuner.do()


if __name__ == "__main__":
    main()