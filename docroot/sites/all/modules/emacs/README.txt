-*- coding:utf-8; mode:rst -*-

Tools and libs for Drupal development under GNU/Emacs (http://www.gnu.org/software/emacs/).


Features
========

Version 1.x offers basic set of features:

* syntax highlighting (with drupal-mode)
* code formatting mostly compatible with Drupal standards (with drupal-mode)
* basic set of expandable code templates for Drupal hooks and PHP functions (with yasnippet)
* realtime syntax checks (with flymake)
* call help on API functions in an external browser

From Emacs point of view features are:

* drupal-mode (major) based on php-mode v1.6
* snippets with Yasnippet
* flymake integration
* additional helper functions for API browsing etc.

Of course all Emacs features like context and regexp search, highlighting of JS and HTML etc. are available in Drupal IDE. And you are free to extend Emacs more with other extensions to support projects, navigate through code and files etc.


Short description
=================

Short explain of project directory listing:

drupal		 		 — PHP & Drupal modes for Emacs
snippets 			 — Drupal and common PHP snippets for Yasnippet
							 (http://github.com/capitaomorte/yasnippet)
extras 				 — collects scripts and so on you may find useful for further extending you
							 Emacs for Drupal works. You may find it not useful. Not any gaurantee.


Usage of code snippets
======================

Example of snippet usage:

1. Type h-perm, then press TAB
2. Typed string expanded into hook_permission() call with default comment and allows you to fill permission definition.
3. Use TAB/Shift-TAB to move between macro' arguments.

For listing of available snippets browse `snippets` directory and subdirs:
* php-mode — snippets available in PHP-mode
* drupal-mode — snippets available in PHP-mode and Drupal-mode


Can I help to the project?
==========================

Yep. We need:

1. Bugfixes and code improvements of Elisp code.
2. More snippets for Yasnippet.
3. You ideas about integrating beautiful XXX feature.

Send it to project bugtracker.

Rules for developers
====================

1. Let snippets short memorizable names separated with dashes. Dashes slightly
more convenient for typing than underscores (lispers know it). Sample:
var-get -> variable_get

2. Avoid long names.

3. Hook names start with `h`. Sample: h-node-view -> <modulename>_node_view

4. Form items start with `f`. Sample: f-fieldset -> $form[…] = array('#type' => 'fieldset…

5. DB API functions started with `db`. Sample: db-sel -> db_select(…)

6. Use lisp function (drupal-module-filename) to get module name in snippets.

7. Add doxygen-style comments to you templates.


iinks to contributed and used code and ideas
============================================

* Code for drupal-mode based on code from Xen http://drupal.org/node/59868#comment-3356582
* Flymake recipe taken from Sacha Chua' blog http://sachachua.com/blog/2008/07/emacs-and-php-on-the-fly-syntax-checking-with-flymake/
* API browsing function initially based on hint by Sean Charles http://drupal.org/node/249296
* PHP snippets initially based on "Minimal php-mode scripts" by Nishimura git://github.com/nishimura/minimal-yasnippet-php-mode.git
* Drupal snippets initally based on work by Sir Squall http://www.cestfait.ch/content/yasnippet-drupal
* Used smart-dash mode by Dennis Patrick Lambe Jr. http://malsyned.net/smart-dash.html
* Used improved version of php-mode v1.6 by Eric James Michael Ritz https://github.com/ejmr/php-mode
* Function for generation of templates for hooks contributed by jweowu http://drupal.org/node/1443418#comment-5619448
* Included improved Flymake version by Sam Graham https://github.com/illusori/emacs-flymake and https://github.com/illusori/emacs-flymake-phpcs

