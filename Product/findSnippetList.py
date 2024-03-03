from askChatGPT import askChatGPT
import re
from Functionality import split
from dotenv import load_dotenv

def findSnippetList(function, library):

    load_dotenv()
    
    print("# A snippet list is under construction using:")
    print("- " + str(function))
    print("- " + str(library))
    print("")

    while(True):
        try:
            list = askChatGPT([function, library], None, 
                    message={"role": "system", "content": "You will be given a C++-function and a library with different snippet pairs.\
                            The first snippet of a snippet pair is the orginal basic C++-version, the second one is an optimization of that using AVX.\
                            Make a hierarchical numbered list of all the snippet pairs in terms of usefulness in using them to optimize the C++-function without additional information (e.g. 1. <snippet to be replaced> 2. <snippet snippet to be replaced> ...)"})

            print("# ChatGPT found the following list:")
            print("")
            print(list)
            print("")
            
            # Given list of snippet pairs
            snippet_pairs = split(list)
            
            print("# The list has been split:")
            print("")
            print(snippet_pairs)
            print("")

            solution = []
            patterns = [[r'Snippet 1 ', r'Snippet1 ', r'snippet1:', r'snippet1_opt ', r'Snippet pair 1 '], 
                        [r'Snippet 2 ', r'Snippet2 ', r'snippet2:', r'snippet2_opt ', r'Snippet pair 2 '],
                        [r'Snippet 3 ', r'Snippet3 ', r'snippet3:', r'snippet3_opt ', r'Snippet pair 3 '],
                        [r'Snippet 4 ', r'Snippet4 ', r'snippet4:', r'snippet4_opt ', r'Snippet pair 4 '],
                        [r'Snippet 5 ', r'Snippet5 ', r'snippet5:', r'snippet5_opt ', r'Snippet pair 5 '],
                        [r'Snippet 6 ', r'Snippet6 ', r'snippet6:', r'snippet6_opt ', r'Snippet pair 6 '],
                        [r'Snippet 7 ', r'Snippet7 ', r'snippet7:', r'snippet7_opt ', r'Snippet pair 7 '],
                        [r'Snippet 8 ', r'Snippet8 ', r'snippet8:', r'snippet8_opt ', r'Snippet pair 8 '],
                        [r'Snippet 9 ', r'Snippet9 ', r'snippet9:', r'snippet9_opt ', r'Snippet pair 9 '],
                        [r'Snippet 10 ', r'Snippet10 ', r'snippet10:', r'snippet10_opt ', r'Snippet pair 10 '],
                        [r'Snippet 11 ', r'Snippet11 ', r'snippet11:', r'snippet11_opt ', r'Snippet pair 11 ']]
            
            patterns_len = len(patterns)
            for string in snippet_pairs:
                # i_active = False
                for i in range(0, patterns_len):
                    for j in range (0, len(patterns[i])):
                        if re.search(patterns[i][j], string):
                            # if i_active == True:
                                # print("DOUBLED because of talkative list: rebuild list")
                                #return -1
                            #i_active = True
                            solution.append(["snippet" + str(i+1) + ".cc", "snippet" + str(i+1) + "_opt.cc"])
                            break

            if solution == []:
                return -1
            
            print("# The snippet list has been successfully constructed:")
            print("")
            for x in solution:
                print(x)
            print("")
            
            return solution

        except Exception as e:
            print(f"error: {e}")

    
