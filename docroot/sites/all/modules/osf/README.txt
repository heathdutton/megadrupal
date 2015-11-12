OSF for Drupal uses the OSF Web Services endpoints to operate.

== Installation ==

You can install the full Open Semantic Framework stack by 
using this OSF installation script:

   http://wiki.opensemanticframework.org/index.php/OSF_Installer

Then once the OSF instance is installed (or one is made accessible to you), you should follow the initial setup configurations steps:

   http://wiki.opensemanticframework.org/index.php/Category:OSF_for_Drupal_User_Manual#Initial_Configuration
   
And then read the OSF for Drupal manual to know what you can accomplish with this module:

   http://wiki.opensemanticframework.org/index.php/Category:OSF_for_Drupal_User_Manual  

== PHP API Library ==

The OSF for Drupal module depends on the OSF WS PHP API to perform all
communications with the configured OSF Web Services instances.  To install this
library you will need to:

 * Enable the Libraries and XAutoload modules
 * Downloaded or clone from GitHub: https://github.com/structureddynamics/OSF-Web-Services-PHP-API
 * Make sure the library is in sites/all/libraries/OSF-WS-PHP-API
