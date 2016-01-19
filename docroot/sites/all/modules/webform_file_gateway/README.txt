CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation


INTRODUCTION
------------

Current Maintainer: Ian Whitcomb - http://drupal.org/user/771654

Webform Private File Gateway provides umbrella functionality for controlling
access to private files through the use of a webform. Users are forced to fill
out a webform before they are granted access to the private file they are
requesting. Developers can optionally specify whether or not to send an email
the the user with a link to the file download and/or enable cookies for
remembering the user.

The difference between this module and Webform Protected Downloads is that
Webform Private File Gateway controls access to ALL private files whereas
Webform Protected Downloads requires separate webforms to be created for
different files.


INSTALLATION
------------

1. Download module and copy webform_file_gateway folder to sites/all/modules

2. Enable Webform Private File Gateway module.

3. Visit admin/config/media/file-system page to configure module settings.
