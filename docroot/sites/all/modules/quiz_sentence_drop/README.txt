
/**
 * @file
 * README file for Quiz Sentence Drop.
 */

Quiz Sentence Drop
Allows quiz creators to make a sentence drag drop kind of question type. where 
there are images in a sentence and student need to drag and drop correct words 
to its suitable image.

CONTENTS
--------

1.  Introduction
2.  Installation

----
1. Introduction

This module is an attempt to make it easy to create sentence drop questions 
using the quiz module.

The image drop module lets the user create question having some images in a 
sentence. Later user need to place correct words on the correct image to get 
points.

How to create Questions:

Your options in a sentence should start and end with @ like mentioned below: 

Example question : @Lemon@ and @Oranges@ are citrus fruits.

Now you need to upload images for lemon and oranges, Don't forget to assign 
image description same as @Lemon@ and @Oranges@ for respective images.
----
2. Installation

To install, unpack the module to your modules folder or under question_types 
folder in quiz module, and simply enable the module at Admin > Build > Modules.

This module requires couple of below mentioned module :

http://drupal.org/project/imce
http://drupal.org/project/filefield_sources

A database table prefixed with quiz_sentence_drop is installed to have a separate 
storage for this module.
