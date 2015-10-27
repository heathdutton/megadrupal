INSTRUCTIONS:


== OAuth lib install ==
- Download OAuth module http://drupal.org/project/oauth
- Enable oauth_common module



== Register your application with 500px ==
- To enable OAuth based access for 500px, you must register your application with 500px 
  and note down the keys: 
  http://developers.500px.com/settings/applications?from=developers



== Install 500px Integration module for Drupal ==

- Enable the 500px API module here:
  admin/modules
- Configure 500px API and add the Consumer keys here: 
  admin/config/services/d500px/settings  
- Additionally, enable the 500px Blocks module 
