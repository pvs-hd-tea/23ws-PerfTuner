from PerfTuner import PerfTuner

input = "example.cpp"
output = "exampleOpt.cpp"

perfTuner = PerfTuner(input, output)
perfTuner.do()
