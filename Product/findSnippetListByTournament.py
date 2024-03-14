from pathlib import Path
from askChatGPT import askChatGPT
import re
from Functionality import split
from dotenv import load_dotenv
from openai import OpenAI
import random

# the following function finds the best four snippets to transforming the given function
def findSnippetListByTournament(function, library):

    numberOfSnippets = 11 # needs to be updated if the number of snippets is changed, needs to be >= 4

    print("# A snippet list is under construction for:")
    print("- " + str(function))
    print("")

    contenders = list() # list of all snippts still in competition
    newContenders = list() # winners of current competition
    for i in range(1,numberOfSnippets+1):
        contenders.append(i)

    # find next smaller power of 2:
    n = 4
    while (n <= numberOfSnippets):
        n*= 2
    n/= 2

    # initial battles to reduce the number of contenders to power of 2:
    numberOfInitialBattles = int(numberOfSnippets - n)

    for i in range(0,numberOfInitialBattles):
        # pick first contender
        contender1 = random.choice(contenders) 
        contenders.remove(contender1)
        # pick second contender
        contender2 = random.choice(contenders)
        contenders.remove(contender2)
        # push winner to new list
        newContenders.append(tournament(function,library,contender1,contender2))
    
    contenders += newContenders  # push winners to the remaining contenders
    newContenders = list() # clear winner list

    # do tournament until final 4:
    while (len(contenders) > 4):
        while (len(contenders) > 1):
            # pick first contender
            contender1 = random.choice(contenders)
            contenders.remove(contender1)
            # pick second contender
            contender2 = random.choice(contenders)
            contenders.remove(contender2)
            # push winner to new list
            newContenders.append(tournament(function,library,contender1,contender2))
        
        contenders = list(newContenders) # winners become the new contenders
        newContenders = list() # clear winner list
    
    # do final 4:
    final4 = list()  # list for place 3 and 4
    while (len(contenders) > 1):
        # pick first contender
        contender1 = random.choice(contenders)
        contenders.remove(contender1)
        # pick second contender
        contender2 = random.choice(contenders)
        contenders.remove(contender2)
        # push participants to final4 list
        final4.append(contender1)
        final4.append(contender2)
        # push winner to new list
        winner = tournament(function,library,contender1,contender2)
        newContenders.append(winner)
        # remove winner from final4 list (because he advances)
        final4.remove(winner)
            
    contenders = list(newContenders)
    newContenders = list()

    # do final battle:
    runnerUp = list()  # second place
    contender1 = contenders[0]
    contender2 = contenders[1]
    runnerUp.append(contender1)
    runnerUp.append(contender2)
    # push winner to new list
    winner = tournament(function,library,contender1,contender2)
    # remove winner from runnerUp list
    runnerUp.remove(winner)
    

    # wrap up result:
    solution = []
    solution.append(["snippet" + str(winner) + ".cc", "snippet" + str(winner) + "_opt.cc"])
    solution.append(["snippet" + str(runnerUp[0]) + ".cc", "snippet" + str(runnerUp[0]) + "_opt.cc"])
    solution.append(["snippet" + str(final4[0]) + ".cc", "snippet" + str(final4[0]) + "_opt.cc"])
    solution.append(["snippet" + str(final4[1]) + ".cc", "snippet" + str(final4[1]) + "_opt.cc"])

    if solution == []:
        return -1  # no snippet won the (should not happen)

    print("")        
    print("# The snippet list has been successfully constructed:")
    print("")
    for x in solution:
        print(x)
        print("")
            
    return solution

        
# function to do battle between two snippets
def tournament(function,library,snippet1, snippet2):

    # set file paths
    core_path = library.resolve().parent
    path1 = "snippet" + str(snippet1) + ".cc"
    snippet1_path = core_path / path1
    path2 = "snippet" + str(snippet2) + ".cc"
    snippet2_path = core_path / path2

    # load token form .env
    load_dotenv()
    
    while(True):
        try:
            # prepare prompt with function and snippets:
            prompt = "function: \n"  
            file_in = open(function,"r")
            prompt += file_in.read()
            prompt += "\n snippet1: \n"
            file_in = open(snippet1_path,"r")
            prompt += file_in.read()
            prompt += "\n snippet2: \n"
            file_in = open(snippet2_path,"r")
            prompt += file_in.read()

            client = OpenAI()

            # do battle of snippets with chatGPT:
            while(True):       
                response = client.chat.completions.create(
                    model="gpt-3.5-turbo",#-4",
                    messages=[
                    {"role": "system", "content": "You will be given a C++-function and two different snippets.\
                        Which snippet performs a taks, that is more closely related to the taks of the C++-function? \
                        <snippet 1> or <snippet 2>?"},
                    {"role": "user", "content": prompt}])
                choice = response.choices[0].message.content

                # determine the winner from answer by chatGPT
                found1 = re.search("1",choice)
                found2 = re.search("2",choice)
                if (found1 != None or found2 != None):
                    break
                print("The (snippet) battle was not decided. I try again.")
            
            # find winner and looser:
            if (found1 == None):
                result = 2
            elif (found2 == None):
                result = 1
            elif (found1.start() < found2.start()):
                result = 1
            else:
                result = 2

            if(result == 1):
                winner = snippet1
                looser = snippet2
            else:
                winner = snippet2
                looser = snippet1

            print("Snippet" + str(winner) + " won the battle against Snippet" + str(looser) + ".")
            
            return winner
        
        except Exception as e:
            print(f"error: {e}")



    
