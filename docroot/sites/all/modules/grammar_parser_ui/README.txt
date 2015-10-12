
CONTENTS OF THIS FILE
---------------------

 * Author
 * Description
 * Installation
 * Dependencies
 * Developers

AUTHOR
------
Jim Berry ("solotandem", http://drupal.org/user/240748)

DESCRIPTION
-----------
This module provides a user interface to the Grammar Parser library code
available at http://drupal.org/project/grammar_parser. This interface allows
you to specify the source code to be parsed as individual files, entire
directories, or from that entered directly in a text box. The Drush Make file
included with this project will install this code library for use with the
Libraries API.

INSTALLATION
------------
To use this module, install it in a modules directory. See
http://drupal.org/node/895232 for further information.

The included Drush Make file provides a convenient method of downloading and
installing the correct version of the Grammar Parser Library (>=1) dependency.
This project has a short name of "grammar_parser_lib" while the module name is
"gplib." The latter name is included in the .info file. From a command line,
simply invoke:

  drush make --yes gpui.make

DEPENDENCIES
------------
While the Grammar Parser Library is the only dependency for this module, the
latter has two dependencies: the Libraries API module and the Grammar Parser
"library." All of these dependencies may be easily downloaded and installed
using the Drush Make file included with this project. Otherwise please refer to
the README files of those projects for installation instructions.

DEVELOPERS
----------
In the event of issues with the grammar parser, debug output may be enabled on
the settings page of this module. It is recommended to enable this only with
smaller files that include the code causing an issue.
