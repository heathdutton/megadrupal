DESCRIPTION
-----------
This module allows flagging an answer to a question as being the correct/best
answer similar to how stackoverflow works.

The default configuration of this module is to allow the user that posted the
question to select which answer is the correct answer.

Only one answer per question can be flagged as the correct answer.


REQUIREMENTS
------------
Commons 3.x with the the following modules enabled:
* Flag
* Commons Q&A


INSTALLATION
------------
* Copy this module to your sites/all/modules folder
* Enable the module


USE
---
Once the module is enabled, a "Correct Answer" link will appear on the answer
nodes of questions for users with the appropriate permissions.  The user can
then click the "Correct Answer" link to flag that answer as the correct
answer to their question.


THEMING
-------
This module adds the following CSS classes to the rendered node elements after
an answer has been flagged as the correct answer.

Question Node: flag--commons-correct-answer-question
Answer Node: flag--commons-correct-answer-answer
