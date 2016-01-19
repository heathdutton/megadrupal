The Mechanical Tutor module provides means for building, presenting and
evaluating script-generated questions.


# What does the module do?

## For end users (learners)

Mechanical Tutor allows you to practice exercises of a certain type, such as
adding fractions or learning German words for days and months. The questions may
vary slightly (different numbers or week days/months) but they only vary within
a set pattern. The questions will never end, so you can practice until you feel
that you have mastered this type of question. Both you and your teacher can
track your progress.

## For end users (teachers)

Mechanical Tutor is a tool for making it easier and hopefully more
fun/interesting to work with repetitive exercises. Teachers select questions
they want students to work on, and can follow their progress. The project is
only intended as a tool for questions that can be easily standardized to a
pattern, which is far from everything, but in these cases Mechanical Tutor
allows combining a never-ending list of exercises with continuous evaluation.

## For site builders

Mechanical Tutor provides script-generated questions, which can be embedded as a
field on nodes and other entities. The questions are submitted and automatically
evaluated. You can use modules like Views and Rules to display statistics for
individual questions, students or classes. The actual questions are plugins
written in code, not built by configuration.

## For coders/developers

Mechanical tutor is a framework for script-generated questions. Each question is
varied by using a set of parameters, for example "A + B = ?" would use the
parameters A and B. Each question is represented by its own class, used for
generating the parameters, building the question and evaluating the submitted
answer. The project is built with math questions in mind, and provides tools
for drawing graphs and evaluating equation patterns.


# Similar projects

This module is inspired by Khan Academy (http://www.khanacademy.org/), and is
intended to complement (but not replace!) Khan Academy as it is used in some
classrooms. (The project maintainer is a maths teacher.)

There is a somewhat simular module called Closed Question, allowing editors/site
builders to set up scripted questions. The Mechanical Tutor module allows more
flexibility for coders, but is not suitable if you want site users to be able to
extend the library of questions by on-site configuration.


# How to use

* Download this project.
* If you want to use the included example questions, download the "matheval"
  library and place it in the folder sites/all/libraries/matheval. This should
  result in a file at sites/all/libraries/matheval/matheval.class.php. You can
  find the library at https://github.com/Itangalo/evalmath.class.php (note the
  name flip).
* Enable the Tutor examples module and the Tutor field module, and all their
  dependencies.
* Add a "Tutor question" field to an entity of your choice (say, node).
* Create a new entity of the relevant bundle. Select which question type you
  want to display. (A few example question types are provided by the Tutor
  example module.)
* View the node: You get a question and an answer box. Question is repeated
  until you answer correctly. Then a new question is generated.

About the Tutor statistics module/feature
* This module provides a new content type for Tutor questions, and also a new
  entity type used for keeping track of streaks of correct answers. There is a
  view attached as a field to the exercise nodes showing these statistics to the
  end user.


# How to create new scripted questions

Best way to get started is probably to have a look at one of the example
questions. Below is a walkthrough of how tutor_example_addition works, included
in the tutor_example module.

* In tutor_example.module, all the questions provided by this module are
  declared by implementing hook_tutor_question(). The tutor_example_addition
  question is declared using its ID as key, and since it follows all the default
  settings no further data is speficied for it. (See tutor.api.php for some
  more information on how to provide more complex settings, or how to declare
  all the plugin files in a specified folder in one go.)
* In tutor_example.info, there is a declaration of the file containing the
  plugin for tutor_example_addition: "files[] = tutor_example_addition.inc"
* The file itself contains a class, describing the question:
  TutorExampleAddition. The class name is by default derived from the question
  ID, and the class must extend TutorQuestion (or one of its sub classes).
* The class overrides some methods from the base class:
  - labelGet() provides the label for the question.
  - parametersGenerate() is called when new question parameters should be
    generated. It returns an array with the parameters.
  - buildQuestion(&$form) is passed a form array by reference, and inserts a
    description of the question into it. There is an answer textfield provided
    by default, which can be overridden if more complex answers are needed.
  - extractAnswer($form) is passed the submitted form, and uses that to populate
    $this->answer with the submitted answer.
  - evaluateAnswer() compares the question parameters with the submitted answer
    and populates $this->result accordingly. The content must be an object of
    the class TutorQuestionResponse (which basically contains both a right/wrong
    value and a message to display to the user).

The full documentation for the TutorQuestion class is found in tutor.inc.


# Contribute
Ideas, questions, code, bug reports and what not are welcome! Check out the
project page over at http://www.drupal.org/project/tutor.
