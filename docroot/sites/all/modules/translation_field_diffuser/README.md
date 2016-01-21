
-- SUMMARY --

Translation Field Diffuser provides the ability to users to diffuse the data of
an entity's field from its initial language to any translation.
With this module, the user will avoid having to fill in the field's data for
all of the content's translations.
It is especially useful for quickly contribute media.

Translation Field Diffuser work with entity type node and taxonomy term.
To add the administration of another entity type, you must determine whether
or not the given field of the entity is translatable.
To do so you need to use the drupal_alter, which is found in
the function translation_field_diffuser_field_widget_form_alter().
You must also add a function in the format
"MODULENAME_data_propagation_ENTITYTYPE" to backup the entity
when using the modules "Content Translation" or "i18n".


-- REQUIREMENTS --

At least one of the following modules need to be enable :
 - Content Translation
 - Internationalization (i18n)
 - Entity Translation


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.


-- CONFIGURATION --

To be able to diffuse a field's data, the entity in which it is incorporated
must be translatable.
Likewise, depending on the translation module being used, the field
must also be translatable.
Once the conditions are fulfilled the field can be defined as diffusable
from its configuration page, linked to the entity.


-- USAGE --

On the editing page of a translatable entity in its source language,
a checkbox titled "Can this field be propagate ?" is made available
under each field, or only those that are translatable
if you use the "Entity Translation" module.
If at least one field box is checked, during the backup of the entity
the data of these fields will be diffused to its existing translations.
Many boxes can be checked for a single backup.


-- CONTACT --

Current maintainers:
* Antoine Forgue (anfor) - https://www.drupal.org/user/2577354
* Alan Moreau (dDoak)    - https://www.drupal.org/user/626534
