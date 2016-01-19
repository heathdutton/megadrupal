			_______________________

				 README

			 David Arroyo Menéndez
			_______________________


Table of Contents
_________________

1 Introduction
2 Requirements
3 Recommended modules
4 Installation
5 Configuration
6 Troubleshooting
7 FAQ
8 Current Maintainers


1 Introduction
==============

  The orgmode drupal module is able to import single files, such as
  articles, and transform it in nodes. Orgmode ([http://orgmode.org/]) is
  a popular format for take notes, TODOs, articles, or books. Orgmode is
  to GNU Emacs, than GNU Emacs is to the operating system, although
  there are a version for vim, too
  ([http://www.vim.org/scripts/script.php?script_id=3342])

  It doesn't need dependencies such as gnu/emacs or another drupal
  modules.

  It manages urls, files and headlines.


2 Requirements
==============

  There are not extra requirements for install orgmode drupal module,
  but you must to have written the org files with an external editor.


3 Recommended modules
=====================

  No.


4 Installation
==============

  $ drush dl orgmode
  $ drush en orgmode


5 Configuration
===============

  There are not configuration.

  You can upload org files from [http://drupalurl/orgmode].


6 Troubleshooting
=================

  You can send troubles to [https://www.drupal.org/project/issues/1975296]


7 FAQ
=====

  + Can I use some org file of example?

  Try 

  $ wget -c [http://www.davidam.com/docu/installingdrupal.org]
  $ wget -c [http://www.davidam.com/docu/life.org]

  Or try much more 

  $ git clone git://orgmode.org/worg.git


8 Current Maintainers
=====================

  + David Arroyo Menéndez (davidam) - [https://www.drupal.org/user/539474/]
