CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Module Details
 * Configuration
 * Troubleshooting
 * FAQ
 * Similar modules
 * Maintainers


INTRODUCTION
------------

Wget Static Module integrates wget application installed on server with drupal.
The module provides you option to generate static HTML of node page, any drupal
internal path or whole website using wget application from drupal itself and
then that static HTML can either be downloaded or can be saved to remote FTP
location.

Wget Static Module provides you handful of options for generation of static HTML
as provided by default wget application also.

New Feature: Save Static html to Webdav Server (With Two factor Authentication
Support).

REQUIREMENTS
------------

Wget Static Module Requires :
* Shell Execution Access for Drupal.
* Wget Application installed on server.
* Commandline access to that wget application.

Without these wget static module is of no use.

IMPORTANT: To use Wget Static Module, understand the module and wget firstly.



MODULE DETAILS
--------------

Wget Static Module comprises of 3 operations in sequential manner.

1- Content Selection :
  Wget Static Module provides 3 options for content to be selected for Static
  HTML generation
  a) NODE
  b) PATH
  c) COMPLETE WEBSITE

  Each of these options are accessible by three different respective urls:
  a) NODE - wget_static/node
  b) PATH - wget_static/path
  c) WEBSITE - wget_static/website

  Basically accessing these URLS would take you to wget static form from where
  user can generate static HTML & then either save to FTP location or download.

  Also users can provide default values alongwith urls using query parameters:
  For node: wget_static/node?nid=3
  For path: wget_static/path?url=<front>, wget_static/path?url=node/3

  Passing the query parameters along with URL directly brings you to the
  download/save options.

2- Wget Options :
  Wget Application provides many options on how to run the query and so the
  Wget Static Module. As of now the module has following configurable options:
  a) Directories Options
  b) HTTP/HTTPS Options
  c) Recursive Retrieval Options
  d) Accept/Reject Options

  These options make wget static module very useful and flexible. Wget options
  are configured by default. Example: Recursive Retrieval is ON.

  IMPORTANT: Wget options which are not included in module are accessible
  directly if "Use wget command options directly" permissions are granted to
  user. But this permission has security implications. So give it to trusted
  roles only.

  Read about Wget options at: https://www.gnu.org/software/wget/manual/wget.html

3- Save/Download Generated Static HTML
  Wget Static Module provides you three options :
  a) Download Zip File
  b) Save files to FTP Server either decompressed or as compressed file.
  c) Save files to Webdav Server either decompressed or as compressed file.
  (Supports Two Factor Authentication).



CONFIGURATION
-------------

Configuration page of Wget Static Module contains basic settings only. Most
Important being "Wget URL on the server", which appears if Wget static module is
not able to find out wget application on server.

All the Wget Options are provided with the form only.


TROUBLESHOOTING
---------------

Wget Debug Mode:
When enabled, wget will write logs to dblog each time it operates at backend.
It basically logs the command generated and output of the command operated.
Used for developer purpose only.

Wget Log file:
Each time wget operates, it writes output to wget.log file in the temporary
directory as used by Drupal. This log file contains last run time logs only.



SIMILAR MODULES
---------------

* Static Generator : https://www.drupal.org/project/static
* Save to FTP : https://www.drupal.org/project/savetoftp

Q: How Wget Static is Different?
A: Wget static module has completely different architecture as it uses Wget
   Command for static html generation and also provides option to either
   download the generated HTML or upload it to Remote FTP/Webdav server.


MAINTAINERS
-----------
Current maintainers:

 * Purushotam Rai (https://drupal.org/user/3193859)


This project has been sponsored by:
 * QED42
  QED42 is a web development agency focussed on helping organisations and
  individuals reach their potential, most of our work is in the space of
  publishing, e-commerce, social and enterprise.
