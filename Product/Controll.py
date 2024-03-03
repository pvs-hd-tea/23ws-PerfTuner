def Controll(Jobs, jobsStatusArray):

    while True:
        for status in jobsStatusArray:
            if status == 1:
                for job in Jobs:
                    job.kill()

