********************************************************************
D R U P A L    M O D U L E
********************************************************************
Name: ApacheSolr View Modes Module
Author: Robert Castelo
Sponsor: Code Positive [www.codepositive.com]
Drupal: 7.0.x
********************************************************************
DESCRIPTION:

    Create ApacheSolr index fields for each view mode of a node.



********************************************************************
PREREQUISITES:

  Apache Solr module must be installed.
  https://drupal.org/project/apachesolr



********************************************************************
INSTALLATION:

    Note: It is assumed that you have Drupal up and running.  Be sure to
    check the Drupal web site if you need assistance.  If you run into
    problems, you should always read the INSTALL.txt that comes with the
    Drupal package and read the online documentation.


	1. Place the entire directory into your Drupal
        modules/directory.

	2. Enable the module by navigating to:

	   Administer > Site building > Modules

	Click the 'Save configuration' button at the bottom to commit your
    changes.

  IMPORTANT:

    The Apache Solr index needs to be rebuilt after this module has
    been enabled, so that an index field can be created for each view
    mode.


********************************************************************
CONFIGURATION:

  Once this module has been enabled and the Apache Solr index re-built,
  a field should be available for each view mode.

  The fields have this naming convention:

  Apache Solr: ss_view_mode_[view mode machine name]

  Example:

  Apache Solr: ss_view_mode_teaser


********************************************************************
LIMITATIONS:

  Each time a view mode is changed, the Apache Solr search index must
  be reuilt.



********************************************************************
AUTHOR CONTACT

- Report Bugs/Request Features:
   http://drupal.org/project/apachesolr_view_modes

- Comission New Features:
   http://drupal.org/user/3555/contact

- Want To Say Thank You:
  Europe:
  http://www.amazon.co.uk/registry/wishlist/4KP50DVHBY62

  Rest of the World:
  http://amzn.com/w/O6JKRQEQ774F
