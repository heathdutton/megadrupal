DESCRIPTION
----------------
This module provides the ability to "authenticate" content against possible plagiarism. The document authentication process involves splitting up the content 
of the document and performing numerous web searches on the pieces. If possible matches are found, the full page content of these matches is retrieved from the 
net and each page is compared against the full text of this document. Depending on how close a match the document and the matched page are, a comparison score 
is calculated. The URLs with content that are considered a good match (i.e. a score above the limit defined by the site administrator) are then reported along with their comparison score.

Various search options will be supported in the future. At them moment only Yahoo's search API is utilized. Future releases of this module will allow for
modular search API extensions and should include Google and possibly paid authentication services such as those offered by iThenitcate.com.


INSTALL
----------------
Install as usual for Drupal modules.

The PEAR class Text_Diff must be installed for this module to work, see the following:
  http://pear.php.net/manual/en/installation.php.
  http://pear.php.net/package/Text_Diff
  
The actions module is not required; but if installed it allows for templating pre-canned email notifications as well as 
provides a mechanism for adding additional email notifications. At the moment.

USAGE
--------------
Basic Usage is:

- install module
- enable any or all API modules that you want to use
- set up accounts with your API(s) of choice (i.e. get a Google API key or Yahoo api key or an account with iThenitcate)
- add your API info to admin settings page
- also on admin settings page define general settings such as how you want doc split up and comparison rating threshhold (Standard APIs (Google/Yahoo) only)
- set which node types you want to provide Authentication services for
- set which roles should be able to perform authentication of those node types

- now when you are on a node view for that type you should get a tab titled Authenticate with subtabs for the enabled API's

- when you pick a API subtab it will ask if you want to submit for authentication

- once submitted it will refresh page with stats or results once finished (or you can just come back to this page later).

CREDITS
---------------
Module designed by Peter Lindstrom of LiquidCMS.ca under contract by LifeWire.com (a service from The New York Times Company). 

Upgrade to Drupal 6 funded by ConsumerSearch.com (a service from The New York Times Company).

