********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Select Remix Module
Author: Robert Castelo <www.codepositive.com>
Drupal: 7.x
********************************************************************
DESCRIPTION:

Hide specific options of a CCK select field for particular content type(s).


********************************************************************
INSTALLATION:

Note: It is assumed that you have Drupal up and running.  Be sure to
check the Drupal web site if you need assistance.

1. Place the entire directory into your Drupal directory:
   sites/all/modules/
   

2. Enable the module by navigating to:

   administer > build > modules
     
  Click the 'Save configuration' button at the bottom to commit your
  changes.


********************************************************************
CONFIGURATION:

Configuration for this module is on the edit form of the field that you want to affect.

For example if you have a Resource Type field on the Documents content type, go to:

Structure -> Content Types -> Documents -> manage fields -> edit [on the row of field to affect]

On the edit form of the field whose options you want to hide, use the Hide Options checkboxes
to select which options to hide for this content type.

When creating a field the Hide Options checkboxes will not appear until you have saved the 
field and come back to edit the field.



********************************************************************
EXPORT:

Select Remix configuration is stored in the Variables table, so can be exported and included in 
Features using the Strongarm module.

The pattern is: select_remix_[content type machine name]_[field machine name]



********************************************************************
AUTHOR CONTACT

- Bug Reports
http://drupal.org/project/select_remix
   
- Commission New Features:
   http://drupal.org/user/3555/contact
   
- Want To Say Thank You:
   http://www.amazon.com/gp/registry/O6JKRQEQ774F

        
********************************************************************
ACKNOWLEDGEMENT

Developed by Code Positive http://www.codepositive.com

Funded by SIANI http://www.siani.se
