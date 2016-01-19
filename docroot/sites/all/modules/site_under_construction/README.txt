Module Purpose:
 Use third party templates for drupal sites while your site is under 
construction.

Installations:
 1- Download the third party templates.
 2- Move template directory under sites/all/libraries/site_under_construction_templates
 or some other place you want.
 3- Enable module and go to configuration page.
 4- Enable the check box and choose the template which you want to implement for
 your site.
 

 Limitations:
 1- Main template directory should be under your mentioned templates path.
 2- Module only will include main template directory html files in option list.
sites/all/libraries/site_under_construction_templates/template_dir/index.html
sites/all/libraries/site_under_construction_templates/template_dir/world.html
sites/all/libraries/site_under_construction_templates/template_dir/world.html

 3- Module will exclude all html files if these are placed under template sub 
 directory e.g
sites/all/libraries/site_under_construction_templates/template_dir/sub_dir/index.html
 4- Html template will be implemented all pages except /user pages.
 5- Html template will be implemented for end user only.