CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Maintainers


INTRODUCTION
------------
Quiz voice answer module provides a new content type to use with quiz module as
a question type. It allows users to record and save voice message as a response
to quiz question.


REQUIREMENTS
------------
Drupal 7.x
PHP 5.1 (for OOP code introduced in Quiz 4.0)
Quiz 7.x-4.x
File entity 7.x-2.x
Libraries API 2.x


INSTALLATION
------------
1. Copy record question folder to modules directory (usually sites/all/modules).
2. Download the library https://github.com/Ruslan03492/Recordmp3js-Drupal
3. Insert the contents of the archive here
sites/all/libraries/Recordmp3js-Drupal
4. At admin/build/modules enable the Quiz voice answer module in the Quiz
Question package.
5. Hit "Save configuration" button.

For detailed instructions on installing contributed modules see:
http://drupal.org/documentation/install/modules-themes/modules-7


MAINTAINERS
-----------
Current maintainers:
 * Ruslan_03492 - https://www.drupal.org/user/2804867
