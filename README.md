# httpQuest

**still under development**. httpQuest is a simple program for a linux terminal that allows the user to test http headers, or get the program to generate one off the users inputs and to be able to see the servers response. Though this program is aimed towards http request it is also possable to send other text based tcp messages to a server.

Usage
-----

```httpquest -[s|i] [-p port] [-f file] [-t type] [-a headerArg] [URL]```
#### Arguments
`-s` silences the output and only sends the header to the specified url.

`-i` makes the inputed parameters imutable (i.e. the program wont try and fix a https request by sending it to the correct port).

`-p [port]` specify what port to use to send the header over.

`-f [file]` send a preconfigured header in a text file. If a file is used the type and url arguments will be ignored.

`-t [type]` is used to specify what type of request is to be sent (`GET|POST|PUT` ect).

`[URL]` is the url the request is going to be sent to.

Install
-------
Todo
