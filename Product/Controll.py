import time

def Controll(Jobs, jobsStatusArray):

    while True:
        for a in jobsStatusArray:
            if a[0] == 0: # there is a successful process
                for job in Jobs:
                    if job != None:
                        try:
                            job.kill()
                            print("KILL")
                        except:
                            pass

