 SN FB Dynamic Cover Photo Description
====================================
SN FB Dynamic Cover Photo Drupal module.  Please see
the following page for more current information:

http://drupal.org/project/sn_fb_dynamic_cover_photo

 Installation
====================================
Regular Drupal module installation. 

Install and enable Libraries module, which allows 
dependency on shared usage of external libraries.
You can then choose a configure settings here:

admin/settings/sn_dcp

1. Input your Brand Page, as shown in attached screen-shot it should be similar to your facebook page.
2. Input your facebook App Id and App Secret.
3. Create album "DCP" for your facebook page.
3. Choose Node type and Field name from where you want to upload your facebook cover photo.
4. Now upload image from selected "node type", which will be uploaded to your "DCP" album and reflect as your facebook page cover photo after cron run. 

== Requirement ==

The module requires dependency on external library 
which needs to be downloaded from:

https://github.com/facebook/facebook-php-sdk 

and placed in libraries folder : sites\all\libraries with name "facebook-php-sdk-master"



  Credits
====================================
pgautam (sn_fb_dynamic_cover_photo): https://drupal.org/user/884876
