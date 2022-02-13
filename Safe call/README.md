Program check the function to ensure it does not crash and returns safely
  - safecall(n) calls unsafe(n) and returns values as follows.
  - if the call to unsafe() crashes, safecall() will return -2
  - if unsafe() does not return within 1s, safecall() will return -1
  - safecall() must always return within 1s

  - if the call to unsafe() returns within 1s, safecall() will return
    the value that unsafe() returned

unsafe() is a misbehaved function that safecall() function will
call.
unsafe() will sometimes run correctly, an will quickly return
an integer (in under 0.5 seconds).

however, sometimes it will misbehave:
  - occasionally, it will run for too long, e.g. 5 seconds
  - other times it will never return
  - and sometimes, it will crash
