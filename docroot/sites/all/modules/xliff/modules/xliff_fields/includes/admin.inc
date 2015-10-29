<?php

/**
 * @file
 * The administration interface.
 */

/**
 * Menu callback; Admin form for xliff fields.
 */
function xliff_fields_settings(&$form_state) {
  // @todo: we need to set which fields are translatable, to know which fields
  // have to be exported/imported.
  drupal_set_message(t('Not yet implemented.'), 'error');
  $form['dummy'] = NULL;
  return $form;
}
