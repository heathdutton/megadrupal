<?php

/**
 * @file
 * Admin callbacks for the Minor Edit module.
 */

/**
 * Form constructor for the minor edit settings form.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function minor_edit_settings() {

  $form = array();
  $form['minor_edit_node_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allow minor edits'),
    '#description' => t('Enables the minor edit checkbox on allowed content types.'),
    '#prefix' => '<div class="minor-edit-toggles">',
    '#suffix' => '</div>',
  );
  $form['minor_edit_node_types']['minor_edit_node_types'] = array(
    '#type' => 'checkboxes',
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('minor_edit_node_types', array('')),
  );
  return system_settings_form($form);
}
