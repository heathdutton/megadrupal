Description
===========
Allows the contents of an "Entity Reference" field to be pre-populated by
taking a parameter from the current Entity as return by the function menu_get_object().

Install
=======
1. Download and enable the module.
2. Visit admin/structure/types/manage/[ENTITY-TYPE]/fields/[FIELD-NAME]
3. Enable "Entity reference current" under the instance settings.


Configuration
=============
Enable Entity reference current:
  Check this to enable Entity reference current on this field.
Action
  Using the select box choose the action to take if the entity reference
  field is pre-populated.
Fallback behaviour
  Select what to do if the current Entity  does NOT exist or is not correct to
  pre-populate the field.
Skip access permission
  This is a fallback overide, the fallback behaviour will not be followed
  for users with the specified permission.

Usage
=====
In order to pre-populate an entity reference field you have to make sure that entity add/edit form is only shown
on pages where the menu object is the correct type of Entity.

This module was originally made for Entityforms: http://drupal.org/project/entityform
But should with any entity type as the Entity containing the Entity Reference field

See http://api.drupal.org/api/drupal/includes!menu.inc/function/menu_get_object/7

