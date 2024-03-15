import re
from dotenv import load_dotenv

from askChatGPT import askChatGPT
from Functionality import split


def findSnippetList(function, library):

    load_dotenv() # access the OPENAI_API_KEY from the .env file
    
    print("# A snippet list is under construction using:")
    print("- " + str(function))
    print("- " + str(library))
    print("")

    # errors can occur to non-constant output format of ChatGPT => try until it works
    while(True):
        try:
            ## get a list (one string) from ChatGPT ranking the snippets accordingly to its relevance for the function ##
            list = askChatGPT([function, library], None, 
                    message={"role": "system", "content": "You will be given a C++-function and a library with different snippet pairs.\
                            The first snippet of a snippet pair is the original basic C++-version, the second one is an optimization of that using AVX.\
                            Make a hierarchical numbered list of all the snippet pairs in terms of usefulness in using them to optimize the C++-function without additional information (e.g. 1. <snippet to be replaced> 2. <snippet snippet to be replaced> ...)"})

            print("# ChatGPT found the following list:\n")
            print(list, "\n")
            
            ## split the string into an array of strings ("1. x 2. y" -> ["1. x", "2. y"]) ##
            snippetsArray = split(list)
            
            print("# The list has been split:\n")
            print(snippetsArray, "\n")

            ## generate a snippet list in an usable format via pattern matching ##
            patterns = [
                [r'Snippet 1 ', r'Snippet1 ', r'snippet1:', r'snippet1_opt ',  r'snippet1_opt\n', r'Snippet pair 1 '], # note the needed empty space: 'Snippet 1' matches Snippet10, 'Snippet 1 ' doesn't
                [r'Snippet 2 ', r'Snippet2 ', r'snippet2:', r'snippet2_opt ',  r'snippet2_opt\n', r'Snippet pair 2 '],
                [r'Snippet 3 ', r'Snippet3 ', r'snippet3:', r'snippet3_opt ',  r'snippet3_opt\n', r'Snippet pair 3 '],
                [r'Snippet 4 ', r'Snippet4 ', r'snippet4:', r'snippet4_opt ',  r'snippet4_opt\n', r'Snippet pair 4 '],
                [r'Snippet 5 ', r'Snippet5 ', r'snippet5:', r'snippet5_opt ',  r'snippet5_opt\n', r'Snippet pair 5 '],
                [r'Snippet 6 ', r'Snippet6 ', r'snippet6:', r'snippet6_opt ',  r'snippet6_opt\n', r'Snippet pair 6 '],
                [r'Snippet 7 ', r'Snippet7 ', r'snippet7:', r'snippet7_opt ',  r'snippet7_opt\n', r'Snippet pair 7 '],
                [r'Snippet 8 ', r'Snippet8 ', r'snippet8:', r'snippet8_opt ',  r'snippet8_opt\n', r'Snippet pair 8 '],
                [r'Snippet 9 ', r'Snippet9 ', r'snippet9:', r'snippet9_opt ',  r'snippet9_opt\n', r'Snippet pair 9 '],
                [r'Snippet 10 ', r'Snippet10 ', r'snippet10:', r'snippet10_opt ',  r'snippet10_opt\n', r'Snippet pair 10 '],
                [r'Snippet 11 ', r'Snippet11 ', r'snippet11:', r'snippet11_opt ',  r'snippet11_opt\n', r'Snippet pair 11 ']]
            
            solution = []
            for string in snippetsArray: # search each snippet string
                for i in range(0, len(patterns)): # for a matching pattern
                    for j in range (0, len(patterns[i])):
                        if re.search(patterns[i][j], string):
                            solution.append(["snippet" + str(i+1) + ".cc", "snippet" + str(i+1) + "_opt.cc"]) # append solution in needed format
                            break
            
            if solution == []:
                return -1
            
            print("# The snippet list has been successfully constructed:\n")
            for x in solution:
                print(x)
            print("")

            return solution

        except Exception as e:
            print(f"error: {e}")

    
