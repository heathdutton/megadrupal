********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Webform Node Value Module
Author: Robert Castelo <www.codepositive.com>
Drupal: 7.x
********************************************************************
DESCRIPTION:

Fill a Webform field with the value of a node field.


********************************************************************
USER CASE:

Use an email address from a node as the destination for the webform to be sent to, based on
what the visitor has selected on one of the webform fields.

More detailed explanation…

There is a Sector content type that includes an Email field, which provides the email address for
the contact person of each sector.

Visitor chooses a sector on a 'Sectors' webform select field. This module uses the value of this field,
and optionally a prefix, to determine the path to the Sector node, and then gets the value of the 
email field, which it then puts into a hidden webform field called 'Send To Email Address'.



********************************************************************
PREREQUISITES:

  Webform module


********************************************************************
INSTALLATION:

Note: It is assumed that you have Drupal up and running.  Be sure to
check the Drupal web site if you need assistance.

1. Place the entire module directory into your Drupal directory:
   sites/all/modules/
   

2. Enable the module by navigating to:

   administer > build > modules
     
  Click the 'Save configuration' button at the bottom to commit your
  changes.


********************************************************************
CONFIGURATION:

Go to the Webform >> Form Settings page of your webform

Look for the collapsed fieldgroup Set Value From A Node




