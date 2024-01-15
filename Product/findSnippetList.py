from askChatGPT import askChatGPT
import re
from Functionality import split

def findSnippetList(function, library):
    while(True):
        try:
            list = askChatGPT([function, library], None, 
                    message={"role": "system", "content": "You will be given a C++-function and a library with different snippet pairs.\
                            The first snippet of a snippet pair is the orginal basic C++-version, the second one is an optimization of that using AVX.\
                            Give me a hierarchical list of the snippet pairs that you think could be used to optimize the C++-function."})

            # Given list of snippet pairs
            snippet_pairs = split(list)

            print(snippet_pairs)

            solution = []
            patterns = [[r'Snippet 1', r'Snippet1', r'snippet1', r'snippet1_opt', r'Snippet pair 1'], [r'Snippet 2', r'Snippet2', r'snippet2', r'snippet2_opt', r'Snippet pair 2']]
            patterns_len = len(patterns)
            for string in snippet_pairs:
                i_active = False
                for i in range(0, patterns_len):
                    for j in range (0, len(patterns[i])):
                        if re.search(patterns[i][j], string):
                            print(i+1)
                            if i_active == True:
                                print("DOUBLED because of talkative list: rebuild list")
                                return -1
                            i_active = True
                            solution.append("snippet" + str(i+1) + ".cc")
                            break
            return solution

        except Exception as e:
            print(f"error: {e}")

    
