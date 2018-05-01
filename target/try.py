import threading


def thread_job():
    pass

add = threading.Thread(target=thread_job)


print(dir(add))