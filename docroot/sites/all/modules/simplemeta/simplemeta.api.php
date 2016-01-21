<?php

/**
 * @file
 * Documents available hooks of the SimpleMeta module.
 */

/**
 * Return information about SimpleMeta property.
 * 
 * See simplemeta_simplemeta_info() for example of SimpleMeta properties
 * definition.
 * 
 * @return array
 *   An array whose keys are SimpleMeta properties' machine names 
 *   and whose values are arrays containing the keys:
 *   - title: Human-readable name of the property.
 *   - form: SimpleMeta edition form element callback for defined property
 *   - theme: SimpleMeta property theming callback used to build output content,
 *     must be defined via hook_theme().
 *   - validate: (optional) additional SimpleMeta form validation callback.
 *   - submit: (optional) additional SimpleMeta form submission callback.
 * 
 * @see simplemeta_simplemeta_info()
 */
function hook_simplemeta_info() {
  $info = array();
  $info['description'] = array(
    'title' => t('Description'),
    'form' => 'simplemeta_form_description',
    'validate' => NULL,
    'submit' => NULL,
    'theme' => 'simplemeta_meta_description',
  );
  return $info;
}

/**
 * Respond to performed on SimpleMeta data actions.
 * 
 * @param object $meta 
 *   meta object operation is performed on.
 * @param string $op 
 *   performed operation, possible values are:
 *   - insert: SimpleMeta data has just been inserted to DB
 *   - update: SimpleMeta data has just been updated in DB
 *   - delete: SimpleMeta data has just been removed from DB
 */
function hook_simplemeta($meta, $op) {
  // Do something.
}
