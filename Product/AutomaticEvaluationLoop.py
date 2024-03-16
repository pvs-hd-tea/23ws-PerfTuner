from PerfTunerClass import PerfTuner
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

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
            "BitwiseLogicCombined/version1",

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
                

                # Store data for plotting
                problem_names_for_plot.append(problem)
                runtimes_cc_for_plot.append(statistics[4])

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

    script_dir = Path(__file__).resolve().parent
    files_path = script_dir / "Statistics/table1.png"
    plt.savefig(files_path, bbox_inches='tight', pad_inches=0.05)

    # table 2
    data = [["Problem", "Statistics"]]

    for statistics in STATISTICS:
        data.append([statistics[0], statistics[1]])

    plt.figure(figsize=(10, 6))
    table = plt.table(cellText=data, loc='center', cellLoc='left', colWidths=[0.4, 1.0])
    table.auto_set_font_size(False)
    table.set_fontsize(12)
    table.scale(1, 1.5)

    plt.axis('off')

    files_path = script_dir / "Statistics/table2.png"
    plt.savefig(files_path, bbox_inches='tight', pad_inches=0.05)

    #GRAPH 1

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
    files_path = script_dir / "Statistics/solved_status_graph.png"
    plt.savefig(files_path)

    plt.show()

    # GRAPH 2
    problem_names_for_plot = []
    original_runtimes_for_plot = []
    optimized_runtimes_for_plot = []

    snippet_used_for_plot = []

    # Extracting data for speed-up graph
    for problem_info in STATISTICS:
        problem_name, class_name = problem_info[0].split(" (")
        original_runtime = problem_info[1][5]
        optimized_runtime = problem_info[1][4]
        snippet_used = problem_info[1][2]

        # Check if runtime is non-negative
        if original_runtime >= 0 and optimized_runtime >= 0:
            # Append data to lists
            problem_names_for_plot.append(problem_name)
            original_runtimes_for_plot.append(original_runtime)
            optimized_runtimes_for_plot.append(optimized_runtime)

    # Plotting speed-up graph
    plt.figure(figsize=(10, 6))
    index = range(len(problem_names_for_plot))
    bar_width = 0.15

    max_runtime = max(max(original_runtimes_for_plot), max(optimized_runtimes_for_plot))
    if np.isfinite(max_runtime):
        plt.ylim(0, max_runtime * 1.1)  # Add some padding


    plt.bar(index, original_runtimes_for_plot, bar_width, alpha=1.0, color='yellow', label='Original Code Time')
    plt.bar(index, optimized_runtimes_for_plot, bar_width, alpha=0.5, color='g',
            label='Optimized Code Time')

    plt.xlabel('Problem Names')
    plt.ylabel('Execution Time (seconds)')
    plt.title('Speed-up Comparison: Original vs Optimized Code')
    plt.xticks(index, problem_names_for_plot, rotation=45, ha='right')
    plt.legend()

    # Place legend outside the plot
    plt.legend(loc='upper left', bbox_to_anchor=(1, 1))

    plt.tight_layout()

    files_path = script_dir / "Statistics/speedup_graph.png"
    plt.savefig(files_path)

    plt.show()

    # GRAPH 3

    # Lists to store problem names and corresponding snippets chosen for plotting
    problem_names_for_snippet_plot = []
    snippets_chosen_for_plot = []

    # Extracting data for snippet chosen graph
    for problem_info in STATISTICS:
        problem_name, class_name = problem_info[0].split(" (")
        snippet_chosen = problem_info[1][2]

        # Append data to lists
        if len(snippet_chosen)>0:
            problem_names_for_snippet_plot.append(problem_name)
            snippets_chosen_for_plot.append(snippet_chosen)

    # Plotting snippet chosen graph
    plt.figure(figsize=(10, 6))

    # Plotting markers for snippet chosen
    plt.scatter(problem_names_for_snippet_plot, snippets_chosen_for_plot, color='blue', label='Snippet Chosen')

    plt.xlabel('Problem Names')
    plt.ylabel('Snippets Chosen')
    plt.title('Snippets Chosen for Each Problem')
    plt.xticks(rotation=45, ha='right')
    plt.legend()

    plt.tight_layout()
    plt.savefig('Statistics/snippet_chosen_graph.png')
    plt.show()


if __name__ == "__main__":
    main()
