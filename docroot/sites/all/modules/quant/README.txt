INTRODUCTION
---------------------------------------------

Quant provides an engine for producing quantitative, time-based analytics 
for virtually any Drupal component. Quant takes raw data about normal Drupal 
actions, such as node creation, and plots the activity over time, with the 
selected time being configurable. 


RECOMMENDED MODULES
---------------------------------------------

Chart API (http://drupal.org/project/chart)
-- This provides the ability to output charts as images using the
   Google Chart API


INSTALLATION
---------------------------------------------

1. Add both quant to your site's modules directory
2. Enable the module at admin/modules
3. Visit quant's settings page and configure the module to your
   liking: /admin/config/media/quant
4. Visit the analytics report: /analytics


CHARTS PROVIDED
---------------------------------------------

Content creation
Comment creation
Content creation by type
Aggregate content creation
User creation
User shouts (requires shoutbox)
User point transactions (requires userpoints)
Group creation (requires organic groups)
Group joins (requires organic groups)
Invites sent (requires invite)
Invites accepted (requires invite)


API
---------------------------------------------
Want to add your own charts or chart plugins? 
See quant.api.php.
