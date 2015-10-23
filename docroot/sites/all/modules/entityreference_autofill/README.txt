The Entity reference autofill module gives Entity reference fields
support to populate fields in their form with values from the
referenced entities.


Installation
------------

This module is dependant on the Entity reference module,
which can be downloaded from https://drupal.org/node/2140229


Configuration
-------------

- First, you need to add a field you want to be auto-filled to your entity.

- If you plan to load the referenced values from another entity type or bundle,
  you will need to add the same to the referenced entity.

- When you have set up both a source and destination field, add an
  entityreference field to the source entity. Currently supported widgets are
  autocomplete, select list and radio buttons. Note that the field can only have
  one value, ie multi-value fields wont work.
  
- Now, in the reference field's instance settings, enable "Entity reference autofill"
  under "Additional behaviors". Select the fields you wish to fetch from the
  referenced entity.

- Optionally, unselect "Overwrite existing data" if you want to preserve
  field data already entered before selecting referenced entity.
  
- Go to the entity form and try it out by loading a value into the
  reference field.
