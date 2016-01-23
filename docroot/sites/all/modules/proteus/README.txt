
-- SUMMARY --

The Proteus module displays closed questions depending on the current "level" of
the student. After answering a question correct, the levels of a student are
updated to a new level, depending on the number of tries the student needed to
answer the question.


-- REQUIREMENTS --

Proteus depends on ClosedQuestion


-- INSTALLATION --

* Install as usual, http://drupal.org/documentation/install/modules-themes/modules-7 for further information.


-- CONFIGURATION --

Default quiz step:
  You can  configure if the student must be given a choice for the next step to make for gaining score points. Default
  a student is given a choice to select a question with small, medium or large impact each time he go's to the next question.
  In the configuration this can be changed to a default using step size. When a step size is set, the student won't be given
  a choice.
Show progress:
  You can  configure if progress and current question score information must be shown to the student during the quiz. Default this
  is only shown to the person that creates the quiz.

-- USAGE --

Proteus is an adaptive system that offers a student questions, based on the
student's current knowledge.

From a teacher's point of view:
 * Make a bunch of Closed Question nodes (using the ClosedQuestion module).
 * For each learning objective make a taxonomy term and link the term to the
   content types Closed Question and Proteus Quiz.
 * For each question, under the heading "Proteus Settings" select the learning
   objectives for the question and assign an entry level and an exit level.
 * Create a Proteus Quiz node.
 * For this Proteus Quiz node, select the learning objectives for the quiz and set the Target Level for each objective.

From a student's point of view:
 * A student starts at a level of 0 for each objective.
 * The student can get questions that have an entry level that is lower than or equal his current level.
 * If a student answers a question correct in 1 try, his new level will be the exit level for each objective linked to that question.
 * If a student needs more tries, his new level will be lower than the exit level.
 * Once a student reaches the target Level for all objectives of the Proteus Quiz, he has finished the quiz.

-- TODO --

See the TODO.txt file.

-- CONTACT --

Wageningen MultiMedia Research Centre: http://wmmrc.wur.nl/

Current maintainers:
* Hylke van der Schaaf - hylke@pkedu.fbt.wur.nl
* Port to Drupal 7, Eelco van Dam - eelco.van.dam@peacs.nl
