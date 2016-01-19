CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Configuration


INTRODUCTION
------------

Current Maintainer: Viswanath Polaki <polaki_2005@yahoo.com>

Cmood module helps site admin to know the mood of node 
posted by the user. Using this module algorithm site admin can calculate a
rough mood of a user's node. This module considers the title and body
and finds if it contains any mood words preceded with rank words, if found
then it calculates the mood.

Site admin must add words for which he wants the mood of content
can be calculated. He must also add rank words for increasing the
mode by rank number of times.

For example: If site admin adds a mood word "good" with a weigth of +2 and
a rank word "very" with weight of +3, Then if a node title or body contains
a word or phrase with "very good" then a mood is calculated which equals
to 6 (3 * 2 = 6). If the title and body contains only word "good" then a mood
will be calculated which equals to 2.

Sample: If a user creates a node which contains a text
of "Hi, I am a very good boy. And I have a good smile.",
then taking into the above word and rank weights the mood is calculated which
equals to (3 * 2) + 2 = 8.

There are no modules available for evaluating the mood of content posted in
the drupal site. By the help of this module site admins can know and analyse
what kind of content exists or posted in the site.


INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
Site admins can see the mood of nodes via admin panel
For viewing node mood navigate to 
URL: <yourdomain.com>/admin/cmood

Site admins can manage mood words via admin panel
For adding mood words navigate to 
URL: <yourdomain.com>/admin/cmood/mood_word

Site admins can manage rank words via admin panel
For adding mood words navigate to 
URL: <yourdomain.com>/admin/cmood/rank_word
