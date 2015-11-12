********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: Exclusive Value Module 
Author: Robert Castelo
Sponsor: Code Positive [www.codepositive.com]
Drupal: 7.0.x
********************************************************************
DESCRIPTION:

    Set a CCK field to only have one value on one node across whole site.
    All other nodes have no value for that field.

    User case: Tick a checkbox on a node to make it featured somewhere, 
    when the checkbox is ticked on another node it will unset the value on the first node.

********************************************************************
INSTALLATION:

    Note: It is assumed that you have Drupal up and running.  Be sure to
    check the Drupal web site if you need assistance.  If you run into
    problems, you should always read the INSTALL.txt that comes with the
    Drupal package and read the online documentation.

      Dependencies: CCK Module  

	1. Place the entire module directory into your Drupal
        modules/directory.

	2. Enable the module by navigating to:

	   Administer > Site building > Modules

	Click the 'Save configuration' button at the bottom to commit your
    changes.
    
********************************************************************
CONFIGURATION:

    On the configuration form of the CCK field which you would like to have an exclusive value, 
    set Exclusive Value checkbox.
