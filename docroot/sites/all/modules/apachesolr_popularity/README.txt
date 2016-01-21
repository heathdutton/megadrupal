APACHE SOLR NODE POPULARITY MODULE
====================================

SUMMARY -----------------------------------------------------

Apache Solr Node Popularity extends Apache Solr Search Integration to include
the popularity of a node into Solr’s search ranking.  This can greatly improve
the relevancy of the search results by pulling frequently visited pages towards
the top and pushing infrequently visited pages towards the bottom, which allows
users to find what they are likely looking for easier and quicker.

For a link to the technical details, see the Documentation Section of the
project page.


REQUIREMENTS ------------------------------------------------

- Apache Solr server
- Apache Solr Search Integration module (apachesolr)
- Apache Solr Config Generation module (apachesolr_confgen)


INSTALLATION & CONFIGURATION --------------------------------

- Install an Apache Solr server (3.6.x)

- Install and enable the Apache Solr Search Integration module
  (dev, or greater than 7.x-1.1)

- Install and enable the Apache Solr Config Generator module
  (dev, or greater than alpha2)

- Install and enable the Apache Solr Node Popularity module

- Configure the Apache Solr Search Integration module as documented
  "admin/config/search/apachesolr/settings"
  (see installation docs for more info)

- (Optional) Install any other Apache Solr related modules

- Goto Configurations >> Search and metadata >> Apache Solr Search 
  >> Config File Generation
  (admin/config/search/apachesolr/confgen)

- Click "Download all files for solr 3.5.x and 3.6.x as zip archive" 
  and extract files

- Copy extracted files in the confgen_FILES directory to the Solr 
  config directory (e.g., "../tomcat/solr/conf/")

- Restart Solr (by restarting the java app server.  E.g., Tomcat)
  (For example: /opt/tomcat/bin/shutdown.sh; /opt/tomcat/bin/startup.sh)

- Make the Solr data directory (e.g., "../tomcat/solr/data/") 
  is writable by the web server.

- Goto Configurations >> Search and metadata >> Apache Solr Search 
  >> Node Popularity
  "admin/config/search/apachesolr/apachesolr_popularity"

- Enable Apache Solr Node Popularity and enter the path of the Solr 
  data directory


Note: the Popular Pages report can be viewed at "admin/reports/popularpages"


CUSTOMIZATION -----------------------------------------------

- Configure Basic Settings  

- Optional: Enable and configure advanced settings 
  ("config/search/apachesolr/apachesolr_popularity/advanced_settings")

- For detailed information, see the Configuration and Customization Details 
  document under the Documentation section on the module's Drupal page.


LIMITATIONS -------------------------------------------------

- May not work if page caching is enabled and a reverse proxy sits in front
  of Drupal.  To work with this configuration, development work would be
  required to integrate with JavaScript callbacks (suggested by klausi)
  See jstats module for an example: http://drupal.org/project/jstats

- The function _apachesolr_popularity_track_new_nodes() may or may not scale on 
  sites with more than 1 million nodes.  If scaling becomes problematic for 
  large sites, this function will require modification.


TROUBLESHOOTING ---------------------------------------------

Problem:

  "Apache Solr not configured correctly" error.  
   Also, "Warning: file_get_contents(...): failed..."

Causes:

  - The Apache Solr server is not running

  - The default Apache Solr server URL is not correct under Apache Solr
    search settings

  - The proper configuration files were created using Config File Generation
    tab (see installation above), are not in the Solr data directory
    (e.g., "../tomcat/solr/data/")

  - The Solr server was not restarted after configuration changes


CONTACT -----------------------------------------------------

Developer: 
  Jonathan Gagne (jongagne) - http://drupal.org/user/2409764

This project was funded by:
  OPIN Software - http://www.opin.ca/
