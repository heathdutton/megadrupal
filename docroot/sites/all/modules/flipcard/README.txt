CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation

INTRODUCTION
------------
Maintainer: drupalshrek 

Documentation: http://drupal.org/project/Flipcard

This project allows you to create flashcards based on complete nodes. It 
automatically makes flashcards shown less-and-less often as you get them right,
by implemeting spaced-repetition with the Leitner box algorithm.

INSTALLATION
------------
1. Copy flipcard folder to modules directory, sites/all/modules.

2. At admin/modules enable the flipcard module.

3. Enable permissions at admin/people/permissions.

4. For the content type which you wish to use for questions and answers, define
   different view modes for rendering the question and answer types wanted. The
   idea here is that you have, say a node type where you define which fields
   are displayed for the question, and which fields are displayed for the answer.
   For example, if you want to use the standard node type which has the name of 
   a country for the node title, and a field called capital for the content, then
   you might define four view modes:
     - question_country (for asking "What is the capital of [e.g.] France")
     - answer_country (for replying "The capital is [e.g.] Paris")
     - question_capital (for asking "Where is [e.g.] London the capital of?")
     - answer_capital (for replying "England")
   See http://drupal.org/node/1965196 for discussion of this topic.
   
5. You will need to make the following changes to the code for your quiz types. 
   Different quiz types are done using view modes.
   
   A summary of the places which will need change are shown below:
   - flipcard_entity_info_alter() for creation of the view mode names
   - flipcard_start_quiz() for the checkbox for user to select quiz type
   - flipcard_quiz_question() for selection of view mode for a question
   - flipcard_quiz_answer() for selection of view mode for an answer
   
   For example, define constants for each quiz type that you want, e.g.
   the default contains:
      define('QUIZ_THAI_TO_ENGLISH', 1);
      define('QUIZ_ENGLISH_TO_THAI', 2);
   or if, say, you want a quiz for town names you might change to:
      define('QUIZ_CAPITALS', 1);
      define('QUIZ_COUNTRIES', 2); 
 






