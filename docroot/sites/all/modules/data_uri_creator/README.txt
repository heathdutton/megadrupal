===============================================================================
The Data URI Creator module for Drupal
===============================================================================

*******************************************************************************
1.  DESCRIPTION
-------------------------------------------------------------------------------

The Data URI Creator is a lightweight standalone module that implements a
utility page for manual generation of Data URIs (see RFC 2397: The data URL
scheme).  The tool can be particularly useful when you craft HTML pages, CSS
or JSON data by hand and want to embed images directly in the content, as
opposed to indirectly through a link to a separate image file.

The module also provides a lean API that can be used programmatically to
detect, encode and decode Data URIs (through the DataUriCreator class).
Therefore, other modules that work with Data URIs can reference the Data URI
Creator module as a common API to perform encoding and decoding of such URIs.

*******************************************************************************
2.  EVALUATION
-------------------------------------------------------------------------------

To evaluate this Drupal project online, go to the simplytest.me website at
  http://simplytest.me/project/2057243

Once there, with this project selected, click on "Launch sandbox" and wait a
few seconds for setup to complete.  Then, click on the new Drupal website's
"Log in" button, and from the "Navigation" menu select "Data URI Creator".
This will get you to the main Data URI Creator page from where you can create
a Data URI manually by following the instructions in the Operation section
below.

*******************************************************************************
3.  PREREQUISITES
-------------------------------------------------------------------------------

This module has no extra requirements, except for a functional Drupal website.
However, one may optionally choose to enable Drupal's built-in Testing module
(a.k.a. simpletest) to be able to run the tests provided by this module.

*******************************************************************************
4.  INSTALLATION
-------------------------------------------------------------------------------

The module's installation procedure is the standard one.  Refer to the Drupal
Installation Guide about Installing Contributed Modules that is available at
  https://drupal.org/documentation/install/modules-themes/modules-7

In summary, one simply needs to:
 a) Download the module to the Drupal server,
 b) Extract the module's files into a folder for contributed modules, and
 c) Enable the Data URI Creator module through the administrator interface.

*******************************************************************************
5.  CONFIGURATION
-------------------------------------------------------------------------------

No further configuration is required after the module is installed and enabled,
unless the utility page should be made available also to anonymous or other
specific users.  To achieve this, one may optionally tweak the default settings
through the Configuration > Data URI Creator settings page (for example,
available at http://example.com/admin/config/content/data-uri-creator).
Here one can specify the maximum allowed byte-size for data uploads and decide
whether to make the Data URI Creator page publicly available to all users or
privately only to administrators.  For more granular access control, use the
Data URI Creator permissions, for example, available at
  http://example.com/admin/people/permissions#module-data_uri_creator

Also refer to the module's built-in help that can be found, for example, at
  http://example.com/admin/help/data_uri_creator#data-uri-creator-configuration

*******************************************************************************
6.  OPERATION
-------------------------------------------------------------------------------

The Data URI Creator page is used to create a Data URI from an uploaded file,
such as from a small PNG image. This utility page is, for example, available at
  http://example.com/data-uri-creator

From this utility page, select the Data File to be converted to a Data URI.
Then, click on the Create Data URI button.  Upon success, the Data URI will
be displayed in a text box.  One can now Select All text from the box, and
then Copy and Paste the data where needed.  Repeat the process, if desired.

Also refer to the module's built-in help that can be found, for example, at
  http://example.com/admin/help/data_uri_creator#data-uri-creator-operation

===============================================================================
