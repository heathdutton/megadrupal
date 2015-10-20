
-- Summary -- 

This module allows admins to create rules. Each of these rules will prevent
a user from adding one particular content type unless the user has finished
adding a specified number of another content type. The number can be
specified by the admin.

-- Features -- 

The following are some examples of this module features.

* You want a user to add a company page for the user to be ableto start adding
  a products page.
* Your user needs to add his profile before he can start posting blogs.
  
-- Requirements --

node module.

-- Installation --

* The instalation of this module is like other Drupal modules.

1. Install This module (unpacking it to your Drupal
/sites/all/modules directory if you're installing by hand).

2. Enable this module in Admin menu > Site building > Modules.

3. Set permitions admin > people > permissions #module-content_type_dependency

-- Configuration --

* Configure settings at admin/config/content/content_type_dependency/list
