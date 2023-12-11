import subprocess

# Compile the C++ code
subprocess.run(["g++", "/home/agoebel/Documents/Uni/Fortgeschrittenenpraktikum_AI_Tools/Programs/23ws-PerfTuner/LLM_APIs/main.cpp", "-mavx","-o", "test"])

# Execute the compiled C++ program
subprocess.run(["./test"])