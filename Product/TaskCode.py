from transformBySnippet import transformBySnippet
from test import test

def TaskCode(i,j, snippet_filepath, snippet_opt_filepath, function_opt_filepath, main_filepath, function_filepath, output_filepath, output_avx_filepath, jobsStatusArray, runs_buildSnippet):

    # do a transformation by using transformBySnippet()
    output = transformBySnippet(str(function_filepath), snippet_filepath, snippet_opt_filepath)

    # write result in the process specific file
    function_opt_filepath.parent.mkdir(parents=True, exist_ok=True)
    with open(function_opt_filepath, "w") as file_out:
        file_out.write(output)

    # test function_opt
    test_result = test(main_filepath, function_filepath, output_filepath, function_opt_filepath, output_avx_filepath)

    # communicate results over the shared memory jobsStatusArray
    jobsStatusArray[i*runs_buildSnippet+j][0] = test_result[0]  # status
    jobsStatusArray[i*runs_buildSnippet+j][1] = test_result[1]  # runtime_cc
    jobsStatusArray[i*runs_buildSnippet+j][2] = test_result[2]  # runtime_avx
            

