Program check the function to ensure it does not crash and returns safely
safecall(n) calls unsafe(n) and returns values as follows.
  - if the call to unsafe() crashes, safecall() will return -2
  - if unsafe() does not return within 1s, safecall() will return -1
  - safecall() must always return within 1s

  - if the call to unsafe() returns within 1s, safecall() will return
    the value that unsafe() returned
