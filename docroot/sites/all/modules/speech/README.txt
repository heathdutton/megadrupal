
Speech README

CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Usage


INTRODUCTION
------------
Maintainer: Daniel Braksator (http://drupal.org/user/134005)

Project page: http://drupal.org/project/speech.


INSTALLATION
------------
1. Copy speech folder to modules directory.
   Usually sites/all/modules
2. At admin/modules enable the Speech recognition module.
3. Configure permissions at admin/people/permissions.

USAGE
------------
The admin user interface is at admin/config/user-interface/speech.
Here you can turn on the 'enable speech recognition' widget which allows you to
individually enable the speech feature on compatible form inputs. In order to
use this module you need at least one group of settings configured.  In the
configuration for a group of settings you can also add the speech feature
automatically to all inputs of certain types. The usage for settings groups is
that you can configure different groups of speech inputs in different ways.
Currently the only relevant configuration to this is the 'Submit upon speech
change' option, since you may want this on some inputs but not others.

NOTE: Speech recognition is experimental and not standard in HTML, and will not
work in all browsers, and may function incorrectly in some browsers.