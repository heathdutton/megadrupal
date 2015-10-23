
Member Only Content 7.x-1.x-dev
========================================

Member Only Content is a module that allows you to show a modified version of a node to users based on their role, allowing you to hide content you wouldn't want them to access and present them with a message or forms.

The module utilizes a checkbox field that can be applied to each content type via an admin screen which will add a checkbox to the publishing options that will allow you to enable your content replacements.

This will also add a new display mode where you can set which fields appear, or in what format they appear.

Changing content based on user role
=========================================

For anonymous non-logged in users, you have the option of placing HTML above and below an optional login form.

For logged in non-members, you can place a block of HTML, and/or a Webform which could prompt the user to fill out the form to gain membership.

For both anonymous and logged in members, you can choose the format of the fields, such as a trimmed body as opposed to the full text, hide entire fields, or add an additional field.

Important pages
=========================================

Admin page - %basepath%/admin/config/content/member_only

Members only content list - %basepath%/admin/config/content/member_only/list

Non-member fields display - %basepath%/admin/structure/types/manage/%nodetype%/display/member_only

Webform module - http://drupal.org/project/webform