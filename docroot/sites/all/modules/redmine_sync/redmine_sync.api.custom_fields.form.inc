<?php

/**
 * Main builder of FAPI elements for Redmine custom fields
 * @param  string $rdm_entity Redmine entity name
 * @return array FAPI elements or FALSE if no custom fields available
 */
function _redmine_sync_attach_custom_fields(&$form, &$form_state, $customized_type) {
  // @todo: cache it
  $rdm_custom_fields = redmine_sync_rest_get_custom_fields($customized_type, REDMINE_SYNC_RET_VAL_TYPE_DEFAULT, $form['#rdm_user_login']);
  if ($rdm_custom_fields !== null) {
    foreach ($rdm_custom_fields as $c_field) {
      $form['custom_fields']['rdm_custom_' . $c_field['id']] = _redmine_sync_build_fapi_element($form, $form_state, $c_field);
    }
  }
}

/**
 * Single FAPI element builder
 * @param  array $rdm_custom_field Redmine custom field returned by REST API
 * @return array                   Single FAPI element
 */
function _redmine_sync_build_fapi_element(&$form, &$form_state, $rdm_custom_field) {
  $element = array(
    '#title'         => t('!name', array('!name' => $rdm_custom_field['name'])),
    '#required'      => $rdm_custom_field['is_required'] == 'true',
    '#rdm_id'        => (string)$rdm_custom_field['id'],
    '#rdm_el_type'   => (string)$rdm_custom_field['field_format'],
    '#default_value' => (string)$rdm_custom_field['default_value'],
    '#description'   => '',
  );

  // Define element type.
  // p.s. Valid types: bool, date, float, int, link, list, text, string, user, version.
  switch ($rdm_custom_field['field_format']) {
    case 'bool':
      $element['#type'] = 'select';
      $element['#options'] = array('' => '', '0' => t('No'), '1' => t('Yes'));
      $element['#default_value'] = $element['#default_value'] instanceof SimpleXMLElement ? '' : (int)$element['#default_value'];
      break;
    case 'date':
      $element['#type'] = 'textfield';
      $element['#attributes'] = array('class' => array('datepicker'));
      $element['#element_validate'] = array('_redmine_sync_form_date_field_validate');
      $element['#maxlength'] = 10;
      $element['#size'] = 11;
      break;
    case 'float':
      $element['#type'] = 'textfield';
      // @todo: Add checking by length (Not required for this type but this is the Redmine requirements).
      // @todo: Add checking by regular expression.
      break;
    case 'int':
      $element['#type'] = 'textfield';
      // @todo: Add checking by length (Not required for this type but this is the Redmine requirements).
      // @todo: Add checking by regular expression.
      break;
    case 'list':
      $element['#type'] = 'select';
      foreach ($rdm_custom_field['possible_values']->possible_value as $c_value) {
        $element['#options'][(string)$c_value->value] = t('@value', array('@value' => (string)$c_value->value));
      }
      break;
    case 'text':
      $element['#type'] = 'textarea';
      // @todo: Add checking by length.
      // @todo: Add checking by regular expression.
      break;
    case 'string':
      $element['#type'] = 'textfield';
      // @todo: Add checking by length.
      // @todo: Add checking by regular expression.
      break;
    case 'user':
      $list_users = array();
      if (isset($form_state['values']['project_id'])) {
        $memberships = redmine_sync_rest_get_project_memberships($form_state['values']['project_id'], null, REDMINE_SYNC_RET_VAL_TYPE_DEFAULT);
        foreach ($memberships as $c_membership) {
          $list_users[(int)$c_membership['user']['id']] = (string)$c_membership['user']['name'];
        }
      }
      $element['#type'] = 'select';
      $element['#options'] = array('' => t('- Select -')) + $list_users;
      break;
    case 'version':
      $list_versions = array();
      if (isset($form_state['values']['project_id'])) {
        $list_versions = redmine_sync_rest_get_project_versions($form_state['values']['project_id']);
      }
      $element['#type'] = 'select';
      $element['#options'] = array('' => t('- Select -')) + $list_versions;
      break;
    default:
      $element['#type'] = 'textfield';
  }

  // Set multiple status.
  if ($rdm_custom_field['multiple'] == 'true') {
    $element['#multiple'] = true;
  }

  // Add description.
  if (!($rdm_custom_field['min_length'] instanceof SimpleXMLElement)) {
    $element['#description'].= ' ' . t('Minimal lenght: !lenght.', array('!lenght' => $rdm_custom_field['min_length']));
  }
  if (!($rdm_custom_field['max_length'] instanceof SimpleXMLElement)) {
    $element['#description'].= ' ' . t('Maximal lenght: !lenght.', array('!lenght' => $rdm_custom_field['max_length']));
  }

  // Add properties.
  $element = _redmine_sync_fapi_element_add_properties($rdm_custom_field, $element);

  return $element;
}

/**
 * Helper to add additional FAPI element properties
 * @param  array $rdm_custom_field Redmine custom field returned by REST API
 * @param  array $element          FAPI Element
 * @return array                   FAPI Element with additional properties set
 */
function _redmine_sync_fapi_element_add_properties($rdm_custom_field, $element) {
  // Add validators
  if ($rdm_custom_field['field_format'] == 'int') {
    $element['#element_validate'] = array('element_validate_integer_positive');
    $element['#size'] = 10;
  }
  if ($rdm_custom_field['field_format'] == 'float') {
    $element['#element_validate'] = array('_redmine_sync_validate_float');
    $element['#size'] = 10;
  }
  return $element;
}

/**
 * Validation for numeric values
 */
function _redmine_sync_validate_float($element, &$form_state) {
  if (!is_numeric($element['#value'])) {
    form_error($element, t('%name must be a decimal number.', array('%name' => $element['#title'])));
  }
}
