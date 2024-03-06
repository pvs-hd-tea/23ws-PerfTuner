from PerfTuner import PerfTuner
import matplotlib.pyplot as plt

import subprocess
import time
#
#
# def measure_execution_time(main_file, function_file):
#     # Run the g++ command to compile the C++ code
#     command = f"g++ {main_file} {function_file} -o output_cc"
#     result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, text=True)
#
#     # Check if the compilation was successful
#     if result.returncode != 0:
#         print("Compilation failed.")
#         return None
#
#     # Measure the runtime of the C++ code
#     start_time_cc = time.time()
#     run_command_cc = "./output_cc"
#     result_run_cc = subprocess.run(run_command_cc, shell=True, stdout=subprocess.PIPE, text=True)
#     end_time_cc = time.time()
#     runtime_cc = end_time_cc - start_time_cc
#
#     return runtime_cc
# def main():
#     print("----------------------------------------------------------------------------------------")
#     print("Welcome to the automatic evaluation loop of PerfTuner!")
#     print("This evaluation loop will try to make different programs faster using AVX2 by calling ChatGPT via meta-strategies.")
#     print("Let's see what happens.")
#     print("----------------------------------------------------------------------------------------")
#     print("")
#
#     STATISTICS = []
#     RESULT = []
#
#     classes = {
#         "Class1": [
#             "LinearCombination",
#             "MatrixMultiplication",
#             "MatrixVectorMultiplication",
#             "ScalarMultiplication",
#             "Scalarproduct",
#             "VectorAddition"
#         ],
#         "Class2": [
#             "LU1",
#             "Transpose"
#         ],
#         "Class3": [
#             "LU3"
#         ]
#     }
#
#     for class_name, problems in classes.items():
#
#         print(class_name)
#         print("")
#
#         numberOfProblemsInClass = len(problems)
#         solved = 0
#
#         # Lists to store problem names and corresponding runtimes
#         problem_names_for_plot = []
#         runtimes_cc_for_plot = []
#
#         for problem in problems:
#
#             print("----------------------------------------------------------------------------------------")
#             print(str(problem))
#             print("----------------------------------------------------------------------------------------")
#             print("")
#
#             # Initializing perfTuner
#             perfTuner = PerfTuner("Problems/" + class_name + "/" + problem)
#
#             statistics = perfTuner.do()
#             STATISTICS.append([problem + " (" + class_name + ")", statistics])
#             if statistics[0] == 0:
#                 solved += 1
#                 print("SOLVED")
#                 print("")
#                 # Measure execution time
#                 main_file_path = f"Problems/{class_name}/{problem}/main.cc"
#                 function_file_path = f"Problems/{class_name}/{problem}/function.cc"
#                 runtime_cc = measure_execution_time(main_file_path, function_file_path)
#
#                 # Display and save execution time
#                 print(f"Execution time for {problem}: {runtime_cc} seconds")
#                 #STATISTICS[-1][1].append(runtime_cc)  # Append execution time to the existing statistics
#
#                 # Store data for plotting
#                 problem_names_for_plot.append(problem)
#                 runtimes_cc_for_plot.append(runtime_cc)
#
#             else:
#                 print("FAIL")
#                 print("")
#
#         RESULT.append([class_name, str(solved) + " / " + str(numberOfProblemsInClass)])
#
#     ## Output ##
#     print("RESULT")
#     for x in RESULT:
#         print(x)
#     print("")
#     print("STATISTICS")
#     for x in STATISTICS:
#         print(x)
#
#     # table 1
#     data = [["Class", "Solved / Total"]]
#
#     for result in RESULT:
#         data.append([result[0], result[1]])
#
#     plt.figure(figsize=(10, 6))
#     table = plt.table(cellText=data, loc='center', cellLoc='center')
#     table.auto_set_font_size(False)
#     table.set_fontsize(12)
#     table.scale(1, 1.5)
#     plt.axis('off')
#     plt.savefig('table1.png', bbox_inches='tight', pad_inches=0.05)
#
#     # table 2
#     data = [["Problem", "Statistics"]]
#
#     for statistics in STATISTICS:
#         data.append([statistics[0], statistics[1]])
#
#     plt.figure(figsize=(10, 6))
#     table = plt.table(cellText=data, loc='center', cellLoc='left')
#     table.auto_set_font_size(False)
#     table.set_fontsize(12)
#     table.scale(1, 1.5)
#     plt.axis('off')
#     plt.savefig('table2.png', bbox_inches='tight', pad_inches=0.05)
#
#
#
#     # Assuming RESULT is a list of lists where each inner list contains [class_name, solved/total]
#     # and STATISTICS is a list of lists where each inner list contains [problem_name, statistics]
#     # Class names and problem names should be unique for this approach to work.
#
#     # Extracting data
#     class_names = [result[0] for result in RESULT]
#     solved_status = [result[1] for result in RESULT]
#     problem_names = [statistics[0] for statistics in STATISTICS]
#     problem_metrics = [statistics[1][0] for statistics in
#                        STATISTICS]  # Assuming the first number indicates success (0 for 'SOLVED', -1 for 'FAIL')
#
#     # Assigning unique colors to each class
#     class_color_mapping = {class_name: plt.cm.jet(i / len(class_names)) for i, class_name in enumerate(class_names)}
#
#     # Creating the plot
#     fig, ax = plt.subplots(figsize=(10, 6))
#
#     # Plotting each problem
#     for i, problem_name in enumerate(problem_names):
#         class_name = problem_name.split()[0]  # Assuming the class name is the first word in the problem name
#         color = class_color_mapping.get(class_name, 'gray')  # Use gray for unknown classes
#         solved = 1 if problem_metrics[i] == 0 else 0  # Assuming 0 indicates success (SOLVED)
#
#         ax.scatter(problem_name, solved, color=color, label=class_name)
#
#     # Setting labels and title
#     ax.set_xlabel('Problem Names')
#     ax.set_ylabel('Solved Status (1: Solved, 0: Fail)')
#     ax.set_title('Solved Status of Problems')
#
#     # Adding legend
#     ax.legend()
#
#     # Rotating x-axis labels for better visibility
#     plt.xticks(rotation=45, ha='right')
#
#     # Save the plot to a file
#     plt.tight_layout()
#     plt.savefig('solved_status_graph.png')
#
#     """
#     # Diagram 1
#     x_values = []
#     y_values = []
#     text_labels = []
#
#     i = 1
#     for data in STATISTICS:
#         x_values.append(i)
#         if(data[1][0]==0):
#             y_values.append(data[1][1])
#         else:
#             y_values.append(-1)
#         text_labels.append(data[0])
#         i += 1
#
#     plt.figure(figsize=(10, 6))
#     plt.scatter(x_values, y_values)
#
#     for i, txt in enumerate(text_labels):
#         plt.annotate(txt, (x_values[i], 0), textcoords="offset points", xytext=(0, -20), ha='center', va='top', rotation=90)
#
#     plt.xlabel('Problem')
#     plt.ylabel('Solving Approach')
#     plt.title('Which transformation approach was successful?')
#     plt.grid(True)
#     plt.savefig('scatter_plot_1.png')
#
#     # Diagram 2
#     x_values = []
#     y_values = []
#     for data in STATISTICS:
#         if((data[1][0]==0) & (data[1][1] == 2)):
#             y_values.append(data[1][2]) # snippet
#             x_values.append(data[1][3]) # which build
#     plt.figure(figsize=(10, 6))
#     plt.scatter(x_values, y_values)
#     plt.xlabel('runs_buildSnippet')
#     plt.ylabel('runs_useSnippet')
#     plt.title('Which build was successful when transformBySnippet() was?')
#     plt.grid(True)
#     plt.savefig('scatter_plot_2.png')
#     """
#     # Plotting and saving the graph
#     plt.figure(figsize=(10, 6))
#     plt.bar(problem_names_for_plot, runtimes_cc_for_plot, color='blue')
#     plt.xlabel('Problem Names')
#     plt.ylabel('Runtime (seconds) for code')
#     plt.title('Runtime for Optimal code')
#     plt.xticks(rotation=45, ha='right')
#     plt.tight_layout()
#     plt.savefig('runtime_cc_graph.png')
#
#
# if __name__ == "__main__":
#     main()
#
#
#
#
#

#try1
#
# import matplotlib.pyplot as plt
# import subprocess
# import time
#
# def measure_execution_time(main_file, function_file):
#     # Run the g++ command to compile the C++ code
#     command = f"g++ {main_file} {function_file} -o output_cc"
#     result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, text=True)
#
#     # Check if the compilation was successful
#     if result.returncode != 0:
#         print("Compilation failed.")
#         return None
#
#     # Measure the runtime of the C++ code
#     start_time_cc = time.time()
#     run_command_cc = "./output_cc"
#     result_run_cc = subprocess.run(run_command_cc, shell=True, stdout=subprocess.PIPE, text=True)
#     end_time_cc = time.time()
#     runtime_cc = end_time_cc - start_time_cc
#
#     return runtime_cc
#
# def main():
#     print("----------------------------------------------------------------------------------------")
#     print("Welcome to the automatic evaluation loop of PerfTuner!")
#     print("This evaluation loop will try to make different programs faster using AVX2 by calling ChatGPT via meta-strategies.")
#     print("Let's see what happens.")
#     print("----------------------------------------------------------------------------------------")
#     print("")
#
#     STATISTICS = []
#     RESULT = []
#
#     # Lists to store problem names and corresponding runtimes for plotting
#     problem_names_for_plot = []
#     runtimes_cc_for_plot = []
#
#     classes = {
#         "Class1": [
#             "LinearCombination",
#             "MatrixMultiplication",
#             "MatrixVectorMultiplication",
#             "ScalarMultiplication",
#             "Scalarproduct",
#             "VectorAddition"
#         ],
#         "Class2": [
#             "LU1",
#             "Transpose"
#         ],
#         "Class3": [
#             "LU3"
#         ]
#     }
#
#     for class_name, problems in classes.items():
#
#         print(class_name)
#         print("")
#
#         numberOfProblemsInClass = len(problems)
#         solved = 0
#
#         for problem in problems:
#
#             print("----------------------------------------------------------------------------------------")
#             print(str(problem))
#             print("----------------------------------------------------------------------------------------")
#             print("")
#
#             # Initializing perfTuner
#             perfTuner = PerfTuner("Problems/" + class_name + "/" + problem)
#
#             statistics = perfTuner.do()
#             STATISTICS.append([problem + " (" + class_name + ")", statistics])
#             if statistics[0] == 0:
#                 solved += 1
#                 print("SOLVED")
#                 print("")
#                 # Measure execution time
#                 main_file_path = f"Problems/{class_name}/{problem}/main.cc"
#                 function_file_path = f"Problems/{class_name}/{problem}/function.cc"
#                 runtime_cc = measure_execution_time(main_file_path, function_file_path)
#
#                 # Display and save execution time
#                 print(f"Execution time for {problem}: {runtime_cc} seconds")
#                 # Store data for plotting
#                 problem_names_for_plot.append(problem)
#                 runtimes_cc_for_plot.append(runtime_cc)
#
#             else:
#                 print("FAIL")
#                 print("")
#
#         RESULT.append([class_name, str(solved) + " / " + str(numberOfProblemsInClass)])
#
#     ## Output ##
#     print("RESULT")
#     for x in RESULT:
#         print(x)
#     print("")
#     print("STATISTICS")
#     for x in STATISTICS:
#         print(x)
#
#     # table 1
#     data = [["Class", "Solved / Total"]]
#
#     for result in RESULT:
#         data.append([result[0], result[1]])
#
#     plt.figure(figsize=(10, 6))
#     table = plt.table(cellText=data, loc='center', cellLoc='center')
#     table.auto_set_font_size(False)
#     table.set_fontsize(12)
#     table.scale(1, 1.5)
#     plt.axis('off')
#     plt.savefig('table1.png', bbox_inches='tight', pad_inches=0.05)
#
#     # table 2
#     data = [["Problem", "Statistics"]]
#
#     for statistics in STATISTICS:
#         data.append([statistics[0], statistics[1]])
#
#     plt.figure(figsize=(10, 6))
#     table = plt.table(cellText=data, loc='center', cellLoc='left')
#     table.auto_set_font_size(False)
#     table.set_fontsize(12)
#     table.scale(1, 1.5)
#     plt.axis('off')
#     plt.savefig('table2.png', bbox_inches='tight', pad_inches=0.05)
#
#     # Plotting execution time graph
#     plt.figure(figsize=(10, 6))
#     plt.bar(problem_names_for_plot, runtimes_cc_for_plot, color='blue')
#     plt.xlabel('Problem Names')
#     plt.ylabel('Execution Time (seconds)')
#     plt.title('Execution Time of Solved Problems')
#     plt.xticks(rotation=45, ha='right')
#     plt.tight_layout()
#     plt.savefig('execution_time_graph.png')
#
#     # Assuming RESULT is a list of lists where each inner list contains [class_name, solved/total]
#     # and STATISTICS is a list of lists where each inner list contains [problem_name, statistics]
#     # Class names and problem names should be unique for this approach to work.
#
#     # Extracting data
#     class_names = [result[0] for result in RESULT]
#     solved_status = [result[1] for result in RESULT]
#     problem_names = [statistics[0] for statistics in STATISTICS]
#     problem_metrics = [statistics[1][0] for statistics in
#                        STATISTICS]  # Assuming the first number indicates success (0 for 'SOLVED', -1 for 'FAIL')
#
#     # Assigning unique colors to each class
#     class_color_mapping = {class_name: plt.cm.jet(i / len(class_names)) for i, class_name in enumerate(class_names)}
#
#     # Creating the plot
#     fig, ax = plt.subplots(figsize=(10, 6))
#
#     # Plotting each problem
#     for i, problem_name in enumerate(problem_names):
#         class_name = problem_name.split()[0]  # Assuming the class name is the first word in the problem name
#         color = class_color_mapping.get(class_name, 'gray')  # Use gray for unknown classes
#         solved = 1 if problem_metrics[i] == 0 else 0  # Assuming 0 indicates success (SOLVED)
#
#         ax.scatter(problem_name, solved, color=color, label=class_name)
#
#     # Setting labels and title
#     ax.set_xlabel('Problem Names')
#     ax.set_ylabel('Solved Status (1: Solved, 0: Fail)')
#     ax.set_title('Solved Status of Problems')
#
#     # Adding legend
#     ax.legend()
#
#     # Rotating x-axis labels for better visibility
#     plt.xticks(rotation=45, ha='right')
#     plt.yticks([0, 1])  # Setting y-axis ticks to only 0 and 1
#
#     # Save the plot to a file
#     plt.tight_layout()
#     plt.savefig('solved_status_graph.png')
#
#     plt.show()
#
# if __name__ == "__main__":
#     main()


#try2

import matplotlib.pyplot as plt
import subprocess
import time

def measure_execution_time(main_file, function_file):
    # Run the g++ command to compile the C++ code
    command = f"g++ {main_file} {function_file} -o output_cc"
    result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, text=True)

    # Check if the compilation was successful
    if result.returncode != 0:
        print("Compilation failed.")
        return None

    # Measure the runtime of the C++ code
    start_time_cc = time.time()
    run_command_cc = "./output_cc"
    result_run_cc = subprocess.run(run_command_cc, shell=True, stdout=subprocess.PIPE, text=True)
    end_time_cc = time.time()
    runtime_cc = end_time_cc - start_time_cc

    return runtime_cc

def main():
    print("----------------------------------------------------------------------------------------")
    print("Welcome to the automatic evaluation loop of PerfTuner!")
    print("This evaluation loop will try to make different programs faster using AVX2 by calling ChatGPT via meta-strategies.")
    print("Let's see what happens.")
    print("----------------------------------------------------------------------------------------")
    print("")

    STATISTICS = []
    RESULT = []

    # Lists to store problem names and corresponding runtimes for plotting
    problem_names_for_plot = []
    runtimes_cc_for_plot = []

    # Lists to store problem names and corresponding snippet choices for plotting
    snippet_names_for_plot = []
    snippet_choices_for_plot = []

    # Lists to store problem names, solved status, and class for plotting
    solved_status_names_for_plot = []
    solved_status_for_plot = []
    class_for_plot = []

    classes = {
        "Class1": [
            "BitwiseAND/version1",

            "LinearCombination/version1",
            "LinearCombination/version2",

            "MatrixMultiplication/version1",
            "MatrixMultiplication/version2",
            "MatrixMultiplication/version3",
            
            "MatrixVectorMultiplication/version1",
            
            "ScalarMultiplication/version1",
            
            "Scalarproduct/version1",
            "Scalarproduct/version2",
            "Scalarproduct/version3",

            "VectorAddition/version1"
        ],
        "Class2": [
            "BitwiseLogicCombined/version1"
            
            "LU1/version1",

            "Transpose/version1"
        ],
        "Class3": [
            "LU3/version1"
        ]
    }

    # Dictionary to store the count of problems using each snippet
    snippet_count = {}

    for class_name, problems in classes.items():

        print(class_name)
        print("")

        numberOfProblemsInClass = len(problems)
        solved = 0

        for problem in problems:

            print("----------------------------------------------------------------------------------------")
            print(str(problem))
            print("----------------------------------------------------------------------------------------")
            print("")

            # Initializing perfTuner
            perfTuner = PerfTuner("Problems/" + class_name + "/" + problem)

            statistics = perfTuner.do()
            STATISTICS.append([problem + " (" + class_name + ")", statistics])
            if statistics[0] == 0:
                solved += 1
                print("SOLVED")
                print("")
                # Measure execution time
                main_file_path = f"Problems/{class_name}/{problem}/main.cc"
                function_file_path = f"Problems/{class_name}/{problem}/function.cc"
                runtime_cc = measure_execution_time(main_file_path, function_file_path)

                # Display and save execution time
                print(f"Execution time for {problem}: {runtime_cc} seconds")
                # Store data for plotting
                problem_names_for_plot.append(problem)
                runtimes_cc_for_plot.append(runtime_cc)

                # Store snippet choice data for plotting
                snippet_name = f"Snippet for {problem}"
                snippet_choice = f"Snippet{statistics[1]}"  # Assuming snippet number is the second element in the statistics
                snippet_names_for_plot.append(snippet_name)
                snippet_choices_for_plot.append(snippet_choice)

                # Store data for solved status graph
                solved_status_names_for_plot.append(problem)
                solved_status_for_plot.append(1)  # 1 indicates solved
                class_for_plot.append(class_name)

                # Update snippet count
                snippet_count[snippet_choice] = snippet_count.get(snippet_choice, 0) + 1


            else:
                print("FAIL")
                print("")
                # Store data for solved status graph
                solved_status_names_for_plot.append(problem)
                solved_status_for_plot.append(0)  # 0 indicates failed
                class_for_plot.append(class_name)

        RESULT.append([class_name, str(solved) + " / " + str(numberOfProblemsInClass)])

    ## Output ##
    print("RESULT")
    for x in RESULT:
        print(x)
    print("")
    print("STATISTICS")
    for x in STATISTICS:
        print(x)

    # table 1
    data = [["Class", "Solved / Total"]]

    for result in RESULT:
        data.append([result[0], result[1]])

    plt.figure(figsize=(10, 6))
    table = plt.table(cellText=data, loc='center', cellLoc='center')
    table.auto_set_font_size(False)
    table.set_fontsize(12)
    table.scale(1, 1.5)
    plt.axis('off')
    plt.savefig('Statistics/table1.png', bbox_inches='tight', pad_inches=0.05)

    # table 2
    data = [["Problem", "Statistics"]]

    for statistics in STATISTICS:
        data.append([statistics[0], statistics[1]])

    plt.figure(figsize=(10, 6))
    table = plt.table(cellText=data, loc='center', cellLoc='left')
    table.auto_set_font_size(False)
    table.set_fontsize(12)
    table.scale(1, 1.5)
    plt.axis('off')
    plt.savefig('Statistics/table2.png', bbox_inches='tight', pad_inches=0.05)

    # Assuming RESULT is a list of lists where each inner list contains [class_name, solved/total]
    # and STATISTICS is a list of lists where each inner list contains [problem_name, statistics]
    # Class names and problem names should be unique for this approach to work.

    # Extracting data for solved status graph
    class_names_for_plot = [class_name for class_name in classes for _ in range(len(classes[class_name]))]

    # Assigning unique colors to each class
    class_color_mapping = {class_name: plt.cm.jet(i / len(classes)) for i, class_name in enumerate(classes)}

    # Creating the plot for solved status graph
    fig, ax = plt.subplots(figsize=(10, 6))

    # Plotting each problem
    for i, problem_name in enumerate(solved_status_names_for_plot):
        class_name = class_for_plot[i]
        color = class_color_mapping.get(class_name, 'gray')  # Use gray for unknown classes
        ax.scatter(problem_name, solved_status_for_plot[i], color=color, label=class_name)

    # Setting labels and title for solved status graph
    ax.set_xlabel('Problem Names')
    ax.set_ylabel('Solved Status (1: Solved, 0: Fail)')
    ax.set_title('Solved Status of Problems by Class')

    # Adding legend for solved status graph
    ax.legend()

    # Rotating x-axis labels for better visibility
    plt.xticks(rotation=45, ha='right')
    plt.yticks([0, 1])  # Setting y-axis ticks to only 0 and 1

    # Save the plot for solved status graph to a file
    plt.tight_layout()
    plt.savefig('Statistics/solved_status_graph.png')

    # Plotting execution time graph
    plt.figure(figsize=(10, 6))
    plt.bar(problem_names_for_plot, runtimes_cc_for_plot, color='blue')
    plt.xlabel('Problem Names')
    plt.ylabel('Execution Time (seconds)')
    plt.title('Execution Time of Problems')
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.savefig('Statistics/execution_time_graph.png')
    plt.show()

    # TODO: Not- working yet- Plotting snippet choices graph
    snippet_numbers = [int(snippet[-1]) for snippet in snippet_choices_for_plot]  # Extracting snippet numbers
    plt.figure(figsize=(10, 6))
    plt.bar(snippet_numbers, [snippet_count.get(f"Snippet{i}", 0) for i in range(1, 12)], color='green')
    plt.xlabel('Snippet Numbers')
    plt.ylabel('Number of Problems')
    plt.title('Number of Problems Using Each Snippet')
    plt.xticks(range(1, 12))  # Setting x-axis ticks to snippet numbers from 1 to 11
    plt.tight_layout()
    plt.savefig('Statistics/snippet_choices_graph.png')
    plt.show()

if __name__ == "__main__":
    main()
