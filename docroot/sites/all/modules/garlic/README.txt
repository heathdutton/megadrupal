Garlic.js
=========

Adds the garlic.js library to Drupal. Garlic lets you use your browser's local storage to save the state of a webform between page loads and accidental window closing.

## Usage

Garlic will apply to any form in Drupal which is loaded via drupal_get_form or hook_form_alter is triggerd upon.
Just add the data attribute to your form

Ex:

function mymodule_form($form, &$form_state) {
  $form['#attributes']['data-persist'] = 'garlic';
  
  ...
  
  return $form;
}

or

function mymodule_form_alter(&$form, &$form_state, $form_id) {
  $form['#attributes']['data-persist'] = 'garlic';
}

## Note on security

Garlic stores the values saved on forms to the local storage in the browser. This means if you're using the library on a login form or on a form with financial data or any other "secret" information that information is stored in plain text in the browser. It's up to the developer to take care of securing/cleaning up that kind of data.


## Known Issues

- If the Garlic library doesn't load for a form which has the data-attribute set, ensure the module adding the form or form_alter is weighted higher than Garlic in the system table.

### Address Field

- Dynamic address widget field values can have odd behaviors with Garlic (i.e. province, postal codes)