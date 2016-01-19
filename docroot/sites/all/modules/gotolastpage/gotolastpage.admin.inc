<?php
/**
 * @file
 * Administration functions for gotolastpage module.
 */

/**
 * Implements hook_admin_settings().
 */
function gotolastpage_admin_settings() {

  $form['gotolastpage_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Redirect to last page of comments for these node types'),
    '#default_value' => variable_get('gotolastpage_nodetypes', array()),
    '#options' => drupal_map_assoc(array_keys(node_type_get_types())),
    '#description' => t('(no selection means all node types)'),
  );

  $form['gotolastpage_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('Add this fragment to rendered pager links'),
    '#default_value' => variable_get('gotolastpage_fragment', GOTOLASTPAGE_FRAGMENT),
    '#description' => t('Content of this field will be added as a fragment (#:fragment) to rendered pager links.', array(
      ':fragment' => variable_get('gotolastpage_fragment', GOTOLASTPAGE_FRAGMENT),
    )),
  );

  return system_settings_form($form);
}


/**
 * Validation of admin form.
 */
function gotolastpage_admin_settings_validate($form, &$form_state) {
  // Filter empty values in node types field.
  $form_state['values']['gotolastpage_nodetypes'] = array_values(array_filter(
    $form_state['values']['gotolastpage_nodetypes']
  ));

}
