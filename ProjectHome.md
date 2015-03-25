This project was inspired by the original Andy's Anagram Solver (http://www.ssynth.co.uk/~gay/anagram.html), created by Andrew Gay.

You give it a word or phrase and a dictionary file and it gives you back all valid anagrams.

Differences to Andy's Anagram Solver:
  * It's open source.
  * You can run it on your machine. It'll probably be faster than waiting for a web page to download. But obviously this isn't as convenient as the web based nature of the original Andy's Anagram Solver.
  * It doesn't produce duplicates. This was on Andrew Gay's future improvements list.
  * You have the dictionary file. It turns out this is quite handy, because often the results are dominated by solutions containing the same extremely rare words. When this happens, I delete the word I don't like from the dictionary and re-run.
  * I suspect it uses a _lot_ more RAM. Like 100 megabytes, or something. Unbelievably wasteful.