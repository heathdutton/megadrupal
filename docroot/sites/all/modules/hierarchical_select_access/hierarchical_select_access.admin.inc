<?php
/**
 * @file
 * Provides the hierarchical select access module admin page.
 */

/**
* Settings form
*/
function hierarchical_select_access_settings_form($form, &$form_state) {
  $roles = user_roles(TRUE);
  $names = node_type_get_names();

  $form['roles_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for all roles'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['roles_fieldset']['hierarchical_select_access_selected_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#description' => t('Select the roles that should not see the Hierarchical Select widget.'),
    '#default_value' => variable_get('hierarchical_select_access_selected_roles', array()),
    '#options' => $roles,
  );
  $form['content_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for all content types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['content_fieldset']['hierarchical_select_access_selected_content'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Types'),
    '#description' => t('Select the content types for which the Hierarchical Select widget will not appear in the node form.'),
    '#default_value' => variable_get('hierarchical_select_access_selected_content', array()),
    '#options' => $names,
  );

  return system_settings_form($form);
}
