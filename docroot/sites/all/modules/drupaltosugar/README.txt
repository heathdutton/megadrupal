MODULE
------
drupaltosugar


DESCRIPTION/FEATURES
--------------------
This module enables you to create integration between your Drupal website
and SugarCRM system. Drupal SugarCRM Integration gives multiple capabilities 
that are not available in earlier modules, such as creating entries
in related modules of sugarCRM. This makes integration faster, 
more comprehensive and better aligned with your business needs. 


REQUIREMENTS
------------
Drupal 7.0


INSTALLATION
------------


drupaltosugar module is dependent on the webform module, you need to
have webform module enabled prior to using drupaltosugar module.

drupaltosugar module is dependent on the nusoap Libraries, you need to
have this libraries in your drupaltosugar module. 
(usually sites/all/modules/drupaltosugar)

Decompress the drupaltosugar.tar.gz file into your Drupal modules
directory (usually sites/all/modules).

Enable the drupaltosugar module: Administration > Modules (admin/modules)

LIBRARY DETAILS:
  * Download library from http://sourceforge.net/projects/nusoap/ 
    and rename it to nusoap and move into drupaltosugar module directory.
  * libraries directory (sites/all/modules/drupaltosugar/nusoap)



CONFIGURATION
-------------

- There are several settings that can be configured in the following places:

  System Configuration > (admin/drupaltosugar/system_configuration)
    Configuration setting form for SugarCRM credentials.

  Module Configuration > (admin/drupaltosugar/module_configuration)
    Map webform fields with SugarCRM module/related module fields. 
    
  Cron Configuration > (admin/drupaltosugar/cron_configuration)
    Set cron interval for each mapped webform.

  Queue Manager > (admin/drupaltosugar/queue_manager)
    Detailed reporting section for webform submission status.
