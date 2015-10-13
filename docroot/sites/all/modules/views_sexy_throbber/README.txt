----------------------------------
Installing Views Sexy Throbber:
----------------------------------
Place the entirety of this directory in sites/all/modules/views_sexy_throbber
or install it using the drupal installer.
You must also install the Views module (http://www.drupal.org/project/views).

1) Navigate to Administer -> Build -> Modules. Enable Views Sexy Throbber.
2) Create or edit an existing view and make sure you enable Ajax
from the Advanced Settings
3) Go to your site frontend and visit your ajax view. Now you will have
a nice new loading animation screen.

----------------------------------
Configuring Views Sexy Throbber:
----------------------------------
1) Navigate to Structure -> Views -> Settings -> Views Sexy Throbber
or visit admin/structure/views/settings/trobber
2) Fill the settings form as you wish
3) Click the save button.

----------------------------------
Performance tips
----------------------------------
This module is built with performance in mind.
The flexibility and the style configuration do NOT add any extra load
to your server. The only thing that this module will add is a cached css file.
For better frontend performance enable the "css/js aggregation and compression"
in your production site by visiting "admin/config/development/performance"
so you can merge and minimize the file requests.

----------------------------------
Check out these two sites that offer customized throbber icons:
http://preloaders.net
http://loadinfo.net
http://ajaxload.info
----------------------------------
