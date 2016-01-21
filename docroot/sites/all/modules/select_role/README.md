CONTENTS OF THIS FILE
---------------------
 * Overview
 * Introduction
 * Installation
 * Steps to test this module
 * Configuration
 * Troubleshooting
 * FAQ


Overview
--------
Forcing the user to choose one role after login, and led to other roles
does not work. User can switch role anytime just visit select_role/landing.


Introduction
------------
This module is useful if your site have differences significantly related
layout and menu item on each role.

For example, you have a website of journal system, which has roles: author,
editor, and reviewer. Sometimes there is one user who has three roles at once.
If that user login, you want that user has to choose one role only. As soon as
the user login, the user must choose one role. After one role is selected,
then other roles will be removed for a while. So, if he/she selects a role as
an author, then the permission and access at the time he role as a reviewer
will not work.

This module uses hook_boot to remove unused roles from $GLOBALS['user']->roles.


Installation
------------
Install as you would normally install a contributed drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.


Steps to test this module
-------------------------
 1. Login as user#1, then make roles: author, editor, reviewer, lecturer.
 2. Visit: admin/config/people/select_role, then check for author, editor, and
    reviewer, then save.
 3. Create one user called desi. give her role of author and reviewer. Save.
 4. Give access to the role author which can create article.
 5. Give access to the role reviewer which can create basic page.
 6. Logout from user#1, and login as desi. Now you will see the effect of this
    module. Visit node/add


Configuration
-------------
Visit admin/config/people/select_role, there two fields
 * Global Select Role
   User who have the role(s), need to select single role only. This
   configuration effected for all user.
 * Excluded Pages
   Indicates which pages will be ignored (no select role checks). Example: Your
   custom logout page. By default, path js/* is used by admin_menu module to
   load admin_menu_toolbar.
Visit user/[user:uid]/edit, there are a new field "Select Role", if there are
special people who choose different roles than others, (example: your boss
don't need to select role: author, editor, and reviewer, he only need
select role: editor, and lecturer) you can set here.


FAQ
----
 * Q: Are user who has chosen the role can switch to other role, without logout?
   A: Yes, just visit select_role/landing
