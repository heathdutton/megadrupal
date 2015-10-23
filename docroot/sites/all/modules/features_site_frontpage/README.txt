********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: Features Site Frontpage 
Author: Robert Castelo
Drupal: 7.x
********************************************************************
DESCRIPTION:

    Enables the site frontpage setting to be exported with Features module.

    Converts node ID into UUID, so that if the frontpage is set to a node that has been exported 
    with a UUID the correct node ID will be set on the target site.



********************************************************************
DEPENDENCIES

Features
https://drupal.org/project/features

UUID
https://drupal.org/project/uuid

Node Export
Not a requirement for Features Site Frontpage module to install or run, but provides a solution
for exporting nodes with a UUID, so if you set your frontage to a node you will probably need this. 
https://drupal.org/project/node_export


********************************************************************
INSTALLATION:

    Note: It is assumed that you have Drupal up and running.  Be sure to
    check the Drupal web site if you need assistance.  If you run into
    problems, you should always read the INSTALL.txt that comes with the
    Drupal package and read the online documentation.

	1. Place the entire module directory into your Drupal
        modules/directory.

	2. Enable the module by navigating to:

	   Administer > Site building > Modules

	Click the 'Save configuration' button at the bottom to commit your
    changes.
    
