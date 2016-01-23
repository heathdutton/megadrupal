
* Introduction

The orgmode drupal module is able to import single files, such as
articles, and transform it in nodes. Orgmode (http://orgmode.org/) is
a popular format for take notes, TODOs, articles, or books. Orgmode is
to GNU Emacs, than GNU Emacs is to the operating system, although
there are a version for vim, too
(http://www.vim.org/scripts/script.php?script_id=3342)

It doesn't need dependencies such as gnu/emacs or another drupal
modules.

It manages urls, files and headlines.

* Requirements

There are not extra requirements for install orgmode drupal module,
but you must to have written the org files with an external editor.

* Recommended modules

No.

* Installation

$ drush dl orgmode
$ drush en orgmode

* Configuration

There are not configuration.

You can upload org files from http://drupalurl/orgmode.

* Troubleshooting

You can send troubles to https://www.drupal.org/project/issues/1975296

* FAQ

+ Can I use some org file of example?

Try 

$ wget -c http://www.davidam.com/docu/installingdrupal.org
$ wget -c http://www.davidam.com/docu/life.org

Or try much more 

$ git clone git://orgmode.org/worg.git

* Current Maintainers

+ David Arroyo Menéndez (davidam) - https://www.drupal.org/user/539474/

