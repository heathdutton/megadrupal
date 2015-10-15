-- Overview --
This module provides local storage caching of user's data that is entered into form elements, preventing its loss after accidental page refresh or its closing.

There are two possible states for fields that use local storage caching:

  * default value: default value of a field, the one that really is saved in the database
  * restored value: value restored from local storage

The module plugins to control local storage caching - one can toggle form element value between <strong>restored</strong> or <strong>default</strong> state.


-- Features --
  * Integration with WYSIWYG module: provides Local Storage plugins for WYSIWYG editors.
  * Integration with CKEditor module: provides Local Storage plugin.
  * It is possible to configure for each field, which value (restored or default) to show by default when page is loaded.
  * Provides "#local_storage" render API property that allows developers to turn on local storage caching for any form element.
  * Flexible hook system used in JS allows developers to interact with every event of the module and implement custom reactions.


-- Supported WYSIWYG editors: --
  * CKEditor
  * TinyMCE
  * EpicEditor (not completely)


-- Supported widgets --
  * Textfield
  * Textarea
  * Textarea with summary
  * Number
  * Select


-- Supported elements --
  * textfield
  * textarea
  * text_format
  * select


-- Installation --
  * Install this module.
  * Enable caching for needed fields in their settings' forms. Look at the <a href="https://www.drupal.org/files/project-images/screen1_0.png">screenshot</a>.


-- Recommended modules --
  * Wysiwyg


-- CONTACT --
Current maintainers:
  * WebEvt - http://drupal.org/user/856734
