<?php
/**
 * @file defacto.admin.inc
 * Admin page callbacks for Defacto module.
 */

/**
 * Implements hook_form().
 */
function defacto_admin_settings() {

  // Get list of taxonomy_term_reference fields to build select list options
  $fields = field_read_fields();
  $checklist_field_array = array();
  foreach ($fields as $item) {
    if ($item['type'] == 'taxonomy_term_reference') {
      $key = $item['field_name'];
      $value = $item['field_name'];
      $checklist_field_array[$key] = $value;
    }
  }

  $form['defacto_bias'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Bias'),
    '#default_value' => variable_get('defacto_bias', 42),
    '#required' => TRUE,
    '#element_validate' => array('_defacto_admin_validate'),
    '#description' => t('The amount of bias to apply to items marked as canonical responses to search keywords.')
  );

  
  $form['defacto_field'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#options' => $checklist_field_array,
    '#default_value' => variable_get('defacto_field'),
    '#required' => TRUE,
    '#description' => t('The taxonomy reference field used to set/select canonical terms associated with a node.'),
  );
  if ("foo" == "bar") {
    form_error($form['defacto_field'], t('No taxonomy term reference fields are available. !link'), 
      array('!link' => l(check_plain('Please configure a taxonomy term reference field on a content type at', 'admin/structure/types'))));
  }

  return system_settings_form($form);
}

/**
 * Admin form validation.
 */
function _defacto_admin_validate($element, &$form_state, $form) {
  switch ($element['#name']) {
    case 'bias':
      if (!is_numeric($element['#value'])) {
        form_error($element, t('Search Bias field must be expressed as a positive decimal number.'));
      }
      break;
  }
}