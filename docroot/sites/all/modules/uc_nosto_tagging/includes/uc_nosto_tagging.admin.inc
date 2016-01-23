<?php

/**
 * @file
 * Administration pages for the module.
 */

/**
 * Creates the form for the admin page.
 *
 * @return array
 *   The form structure.
 */
function _uc_nosto_tagging_admin() {
  $form = array();

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_SERVER_ADDRESS;
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Server address'),
    '#default_value' => variable_get($name, UC_NOSTO_TAGGING_SERVER_ADDRESS),
    '#description' => t('The server address for the Nosto marketing automation service.'),
    '#required' => TRUE,
  );

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_ACCOUNT_NAME;
  $form[$name] = array(
    '#type' => 'textfield',
    '#title' => t('Account name'),
    '#default_value' => variable_get($name, ''),
    '#description' => t('Your Nosto marketing automation service account name.'),
    '#required' => TRUE,
  );

  _uc_nosto_tagging_admin_get_product_tagging_form($form);
  _uc_nosto_tagging_admin_get_category_tagging_form($form);

  return system_settings_form($form);
}

/**
 * Creates the product tagging settings form.
 *
 * @param array $form
 *   The form structure.
 *
 * @see _uc_nosto_tagging_admin()
 */
function _uc_nosto_tagging_admin_get_product_tagging_form(&$form) {
  $form['fieldset_product_tagging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product tagging settings'),
    '#collapsible' => TRUE,
  );

  $options = _uc_nosto_tagging_admin_get_field_options('node', 'image');
  $options[0] = '<' . t('none') . '>';
  ksort($options);
  $name = UC_NOSTO_TAGGING_CONFIG_KEY_CONTENT_TYPE_IMAGE_FIELD_ID;
  $form['fieldset_product_tagging'][$name] = array(
    '#type' => 'select',
    '#title' => t('Product content type image field'),
    '#options' => $options,
    '#default_value' => variable_get($name, 0),
    '#description' => t('The content type field used for product images.'),
  );

  $options = _uc_nosto_tagging_admin_get_field_options('node', 'taxonomy_term_reference');
  $options[0] = '<' . t('none') . '>';
  ksort($options);
  $name = UC_NOSTO_TAGGING_CONFIG_KEY_CATEGORY_FIELD_ID;
  $form['fieldset_product_tagging'][$name] = array(
    '#type' => 'select',
    '#title' => t('Product category field'),
    '#options' => $options,
    '#default_value' => variable_get($name, 0),
    '#description' => t('The content type taxonomy reference field used for product categories.'),
  );

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_BRAND_FIELD_ID;
  $form['fieldset_product_tagging'][$name] = array(
    '#type' => 'select',
    '#title' => t('Product brand field'),
    '#options' => $options,
    '#default_value' => variable_get($name, 0),
    '#description' => t('The content type taxonomy reference field used for product brand.'),
  );
}

/**
 * Creates the category tagging settings form.
 *
 * @param array $form
 *   The form structure.
 *
 * @see _uc_nosto_tagging_admin()
 */
function _uc_nosto_tagging_admin_get_category_tagging_form(&$form) {
  $form['fieldset_category_tagging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Category tagging settings'),
    '#collapsible' => TRUE,
  );

  $options = _uc_nosto_tagging_admin_get_vocabulary_options();
  $options[0] = '<' . t('none') . '>';
  ksort($options);
  $name = UC_NOSTO_TAGGING_CONFIG_KEY_CATEGORY_VOCABULARY_ID;
  $form['fieldset_category_tagging'][$name] = array(
    '#type' => 'select',
    '#title' => t('Category vocabulary'),
    '#options' => $options,
    '#default_value' => variable_get($name, 0),
    '#description' => t('The vocabulary used for product categories.'),
  );
}

/**
 * Validates form data from admin page.
 *
 * @param array $form
 *   Form structure.
 *
 * @param array $form_state
 *   The current form state data.
 */
function _uc_nosto_tagging_admin_validate($form, $form_state) {
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_SERVER_ADDRESS;
  $server_address = isset($values[$name]) ? $values[$name] : 0;
  if (empty($server_address)) {
    form_set_error($name, t('Server address is required.'));
  }
  elseif (!valid_url($server_address)) {
    form_set_error($name, t('Server address is not a valid url.'));
  }
  elseif (preg_match('@^https?://@i', $server_address)) {
    form_set_error($name, t('Server address cannot include the protocol (http:// or https://).'));
  }

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_ACCOUNT_NAME;
  $account_name = isset($values[$name]) ? $values[$name] : 0;
  if (empty($account_name)) {
    form_set_error($name, t('Account name is required.'));
  }

  _uc_nosto_tagging_admin_validate_product_tagging_settings($form, $form_state);
  _uc_nosto_tagging_admin_validate_category_tagging_settings($form, $form_state);
}

/**
 * Validates form data for the product tagging settings.
 *
 * @param array $form
 *   Form structure.
 *
 * @param array $form_state
 *   The current form state data.
 *
 * @see _uc_nosto_tagging_admin_validate()
 */
function _uc_nosto_tagging_admin_validate_product_tagging_settings($form, $form_state) {
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_CONTENT_TYPE_IMAGE_FIELD_ID;
  $field_id = isset($values[$name]) ? $values[$name] : 0;
  if (!_uc_nosto_tagging_admin_validate_field_id($field_id, 'image')) {
    form_set_error($name, t('Invalid product content type image field selection.'));
  }
  elseif ((int) $field_id === 0) {
    drupal_set_message(t('Product image url will not be included in the product tagging.'), 'warning');
  }

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_CATEGORY_FIELD_ID;
  $field_id = isset($values[$name]) ? $values[$name] : 0;
  if (!_uc_nosto_tagging_admin_validate_field_id($field_id, 'taxonomy_term_reference')) {
    form_set_error($name, t('Invalid product category field selection.'));
  }
  elseif ((int) $field_id === 0) {
    drupal_set_message(t('Product categories will not be included in the product tagging.'), 'warning');
  }

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_BRAND_FIELD_ID;
  $field_id = isset($values[$name]) ? $values[$name] : 0;
  if (!_uc_nosto_tagging_admin_validate_field_id($field_id, 'taxonomy_term_reference')) {
    form_set_error($name, t('Invalid product brand field selection.'));
  }
  elseif ((int) $field_id === 0) {
    drupal_set_message(t('Product brand name will not be included in the product tagging.'), 'warning');
  }
}

/**
 * Validates form data for the category tagging settings.
 *
 * @param array $form
 *   Form structure.
 *
 * @param array $form_state
 *   The current form state data.
 *
 * @see _uc_nosto_tagging_admin_validate()
 */
function _uc_nosto_tagging_admin_validate_category_tagging_settings($form, $form_state) {
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  $name = UC_NOSTO_TAGGING_CONFIG_KEY_CATEGORY_VOCABULARY_ID;
  $vid = isset($values[$name]) ? $values[$name] : 0;
  if (!ctype_digit((string) $vid)) {
    form_set_error($name, t('Invalid category vocabulary.'));
  }
  elseif ($vid > 0) {
    $vocabulary = taxonomy_vocabulary_load($vid);
    if (!$vocabulary) {
      form_set_error($name, t('Invalid category vocabulary.'));
    }
  }
  else {
    drupal_set_message(t('Categories will not be tagged. It is highly discouraged to leave the category vocabulary blank.'), 'warning');
  }
}

/**
 * Validation for field ids.
 *
 * Checks that the id is number and that the field info
 * for the id can be loaded. If a type is provided, then
 * the loaded field info must be of that type.
 *
 * @param int $field_id
 *   The field id to validate.
 *
 * @param null|string $field_type
 *   The type of the field the id should reference.
 *
 * @return bool
 *   If validation passed or not.
 *
 * @see _uc_nosto_tagging_admin_validate()
 */
function _uc_nosto_tagging_admin_validate_field_id($field_id, $field_type = NULL) {
  if (!ctype_digit((string) $field_id)) {
    return FALSE;
  }
  elseif ($field_id > 0) {
    $field_info = field_info_field_by_id($field_id);
    if ($field_type !== NULL && (!isset($field_info['type']) || $field_info['type'] !== $field_type)) {
      return FALSE;
    }
    elseif (!isset($field_info['id'])) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Returns list of field options for entity.
 *
 * @param string $entity_type
 *   The entity type to fetch the fields for.
 *
 * @param string $field_type
 *   The type of fields to include, defaults to all.
 *
 * @return array
 *   Associative array of field id to name.
 *
 * @see _uc_nosto_tagging_admin()
 */
function _uc_nosto_tagging_admin_get_field_options($entity_type, $field_type = NULL) {
  $options = array();

  switch ($entity_type) {
    case 'node':
      $node_types = node_type_get_types();
      $types = array_keys($node_types);
      break;

    default:
      $types = array();
      break;
  }

  if (!empty($types)) {
    foreach ($types as $type) {
      foreach (field_info_instances($entity_type, $type) as $field_name => $value) {
        $field_info = field_info_field($field_name);
        if ($field_type === NULL || $field_info['type'] === $field_type) {
          if (!isset($options[$field_info['id']])) {
            $options[$field_info['id']] = array(
              'name' => $field_name,
              'labels' => array(),
            );
          }
          $options[$field_info['id']]['labels'][] = $value['label'];
        }
      }
    }
  }

  foreach ($options as $id => $data) {
    $options[(int) $id] = $data['name'] . ' (' . implode(', ', array_unique($data['labels'])) . ')';
  }

  return $options;
}

/**
 * Returns a list of vocabulary options.
 *
 * @return array
 *   Associative array of vocabulary vid to name.
 *
 * @see _uc_nosto_tagging_admin()
 */
function _uc_nosto_tagging_admin_get_vocabulary_options() {
  $options = array();

  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    $options[(int) $vocabulary->vid] = $vocabulary->machine_name . ' (' . $vocabulary->name . ')';
  }

  return $options;
}
