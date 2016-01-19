-- SUMMARY --
This module provides two scoreboards to see the results of Closed Questions.
There's a Closed Question Student Scoreboard that shows the results of given
set of questions and which questions in that set aren't answered yet. The 
Closed Question Teacher Scoreboard shows all the results for a given set of 
questions.

-- REQUIREMENTS --
ClosedQuestion Scoreboard depends on the ClosedQuestion module.

-- INSTALLATION --
Install as usual, see http://drupal.org/node/70151 for further information.

-- CONFIGURATION --
None needed

-- USAGE --
Create one of the new node types. Enter a list of node-ids of closedquestion
nodes as body. Seperate them by ";", for example: "3;5;64;7".
