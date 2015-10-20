Drupal Members Page module

This module makes use of the php curl library if it is installed and enabled.
The curl library offers better url validation for reconfigure requests than the
standard Drupal library.

A 'members_page' directory is created when you install this module.

The file names and the module functions in this directory are prefixed with
'members' instead of 'members_page'. There are historical reason for this. They
are not important.

  http://drupal.org/node/1237070#comment-5660122
  http://drupal.org/project/members



**Using drush commands**

This version of the Members Page module can be enabled, disabled, and
uninstalled with the following drush commands. This assumes the current dir
is the contributed modules directory. Adjust as needed.

drush en members -y
drush dis members -y
drush pm-uninstall members -y


**Novel Features**

There are two novel features in this module. Each feature is implemented by a
an API type function.  The code is found in the members.inc file.

1) Configurable URLs - members_check_url_free(string $url)
2) Block generation by function call -- members_render_magic_block($fields)


*Configurable URLs*

members_check_url_free()

The Members page is located at '/members' when it is first installed. It can
be changed to any other available url.

A check is performed to determine if a requested url is valid. If the curl 
library is installed, then it is used to make an aditional check. If this curl 
request returns a 404, then the url is considered available.


*Block Generation by Function Call*

members_render_magic_block()

A standard Drupal block typically appears on any given page according to a
configuration that is recorded in the database. The database entries also have
the module names to point to the code to generate the blocks. This combination
allows for placement of any block in any region on a given page.

The members page module generates 3 drupal blocks using a function call...

  members_render_magic_block($fields).

These blocks are displayed in only one region (content) on only one page -- the
Members Page.

It is possible and probable that you might see two login blocks on the
Members Page. One login block is the standard Drupal Login block.  It can be
configured to not appear. The Members Pages is intended to replace it.


http://sontag.ca/drupal/members-page-module

