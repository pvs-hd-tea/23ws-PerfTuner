from PerfTuner import PerfTuner
import matplotlib.pyplot as plt

def main():
    print("----------------------------------------------------------------------------------------")
    print("Welcome to the automatic evaluation loop of PerfTuner!")
    print("This evaluation loop will try to make different programs faster using AVX2 by calling ChatGPT via meta-strategies.")
    print("Let's see what happens.")
    print("----------------------------------------------------------------------------------------")
    print("")

    STATISTICS = []
    RESULT = []

    classes = {
        "Class1": [
            "LinearCombination",
            "MatrixMultiplication",
            "MatrixVectorMultiplication",
            "ScalarMultiplication",
            "Scalarproduct",
            "VectorAddition"
        ],
        "Class2": [
            "LU1",
            "Transpose"
        ],
        "Class3": [
            "LU3"
        ]
    }

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
            else:
                print("FAIL")
                print("")

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
    plt.savefig('table1.png', bbox_inches='tight', pad_inches=0.05)

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
    plt.savefig('table2.png', bbox_inches='tight', pad_inches=0.05)

    """
    # Diagram 1
    x_values = []
    y_values = []
    text_labels = []

    i = 1
    for data in STATISTICS:
        x_values.append(i)
        if(data[1][0]==0):
            y_values.append(data[1][1])
        else:
            y_values.append(-1)
        text_labels.append(data[0])
        i += 1

    plt.figure(figsize=(10, 6))
    plt.scatter(x_values, y_values)

    for i, txt in enumerate(text_labels):
        plt.annotate(txt, (x_values[i], 0), textcoords="offset points", xytext=(0, -20), ha='center', va='top', rotation=90)

    plt.xlabel('Problem')
    plt.ylabel('Solving Approach')
    plt.title('Which transformation approach was successful?')
    plt.grid(True)
    plt.savefig('scatter_plot_1.png')

    # Diagram 2
    x_values = []
    y_values = []
    for data in STATISTICS:
        if((data[1][0]==0) & (data[1][1] == 2)):
            y_values.append(data[1][2]) # snippet
            x_values.append(data[1][3]) # which build
    plt.figure(figsize=(10, 6))
    plt.scatter(x_values, y_values)
    plt.xlabel('runs_buildSnippet')
    plt.ylabel('runs_useSnippet')
    plt.title('Which build was successful when transformBySnippet() was?')
    plt.grid(True)
    plt.savefig('scatter_plot_2.png')
    """

if __name__ == "__main__":
    main()




