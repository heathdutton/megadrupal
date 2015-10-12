********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Views Linker Module
Author: Robert Castelo <www.codepositive.com>
Drupal: 7.x
********************************************************************
DESCRIPTION:

The user has searched or filtered on a Views generated page,
clicked on one of the found results, and navigated through to the
page for that result.

After reading the info on the result page, how does the site lead
the user back to the page listing all the other found results?

This module provides a link back to the Views listing page,
which preserves the filters the user applied.

The major advantage of this module over Views built in "Remember
the last selection" is that this module takes a more cache friendly
approach and is more customisable.

Features include:

* Link includes all keywords and filters the user set on the search
  page
* Link is provided in a block so can be added anywhere on a result
  page
* Link can display different label if there was no search and the
  page was reached directly
* Option to not show link if there was no search
* Link labels can be configured
* Link path can be configured
* Page can be cached - link is changed in browser with JavaScript



********************************************************************
PREREQUISITES:

  Views module

  CTools module


********************************************************************
INSTALLATION:

Note: It is assumed that you have Drupal up and running.  Be sure to
check the Drupal web site if you need assistance.

1. Place the entire module directory into your Drupal directory:
   sites/all/modules/


2. Enable the module by navigating to:

   administer > modules

  Click the 'Save configuration' button at the bottom to commit your
  changes.



********************************************************************
USAGE

On a View which is set to display as a Page, click on the Other field group, and then click on 
‘Views linker’ link.

The options are documented on the form that pops up.

Once a View has a Views Link enabled and configured a block will become available which 
displays the link back to search results.

The block admin title begins with ‘Views Linker:’.

Add the block to the result pages using the administer block config, or Panels, or Context.


********************************************************************
AUTHOR CONTACT

- Report Bugs/Request Features:
   https://www.drupal.org/project/views_linker

- Comission New Features:
   https://www.drupal.org/user/3555/contact

- Want To Say Thank You:
   http://www.amazon.com/gp/registry/O6JKRQEQ774F


********************************************************************
ACKNOWLEDGEMENT




********************************************************************
SPONSORS

