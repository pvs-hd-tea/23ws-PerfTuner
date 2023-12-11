import os

from findSubTasks import findSubTasks
from findImprovableSubTasks import findImprovableSubTasks
from findUseOfAVX2 import findUseOfAVX2
from findImprovementsInSubTasks import findImprovementsInSubTasks



def is_file_not_empty(file_path):
        return os.path.exists(file_path) and os.path.getsize(file_path) > 0

class PerfTuner:
    def __init__(self, file_input_name, file_output_name):
        self.file_input_name = file_input_name
        self.file_output_name = file_output_name
        self.file_SubTasks_name = "SubTasks.cpp"
        self.file_improvableSubTasks_name = "improvableSubTasks.cpp"
        self.file_UseOfAVX2 = "UseOfAVX2.cpp"
        self.file_improvementsInSubTask_name = "improvementsInSubTask.cpp"

    def do(self):
        
        findSubTasks(self.file_input_name, self.file_SubTasks_name)

        findImprovableSubTasks(self.file_SubTasks_name, self.file_improvableSubTasks_name)
        
        #while(is_file_not_empty(self.file_improvableSubtasks_name)):
        #     findImprovementsInSubTasks(self.file_improvableSubtasks_name)

        findUseOfAVX2(self.file_improvableSubTasks_name, self.file_UseOfAVX2)

        findImprovementsInSubTasks(self.file_UseOfAVX2, self.file_improvementsInSubTask_name)



    


        




