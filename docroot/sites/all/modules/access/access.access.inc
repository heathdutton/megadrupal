<?php
/**
 * @file
 * Implements the access control kit API to integrate with core modules.
 */

/**
 * Implements hook_access_realm_info().
 */
function access_access_realm_info() {
  $info['boolean'] = array(
    'label' => t('a boolean value'),
    'field_type' => 'access_boolean',
  );

  if (module_exists('list')) {
    foreach (array('integer', 'float', 'text') as $type) {
      $info['list_' . $type] = array(
        'label' => t("a list (" . $type . ") field's allowed values"),
        'field_type' => 'access_' . $type,
        'arguments' => array('field' => ''),
      );
    }
  }

  if (module_exists('taxonomy')) {
    $info['taxonomy_term'] = array(
      'label' => t('a taxonomy term'),
      'field_type' => 'access_integer',
      'arguments' => array('vocabulary' => ''),
    );
    // @todo Support cascading hierarchical privileges as an option.
  }

  $info['user'] = array(
    'label' => t('a user account'),
    'field_type' => 'access_integer',
  );
  // @todo Support access to nodes based on authorship, menu items based on the authorship of the node it links to, etc. (ex. "You can edit any nodes written by Bob and any menu links that point to them.").

  return $info;
}

/**
 * Implements hook_access_realm_settings().
 */
function access_access_realm_settings($realm_type, $has_data, $values = array()) {
  $form = array();
  switch ($realm_type) {
    case 'list_integer':
    case 'list_float':
    case 'list_text':
      // Only support single-value list fields for now. This may change in the
      // future, if we can figure out a sane way to handle it on object forms.
      $fields = field_read_fields(array('type' => $realm_type, 'cardinality' => 1));
      $options = drupal_map_assoc(array_keys($fields));
      $form['field'] = array(
        '#type' => 'select',
        '#title' => t('List field'),
        '#description' => t('The allowed values list from this field will become the access realms for the site.'),
        '#default_value' => $values['field'],
        '#options' => $options,
        '#required' => TRUE,
        '#disabled' => $has_data,
      );
      break;

    case 'taxonomy_term':
      $vocabularies = taxonomy_get_vocabularies();
      $options = array();
      foreach ($vocabularies as $vocabulary) {
        $options[$vocabulary->machine_name] = $vocabulary->name;
      }
      $form['vocabulary'] = array(
        '#type' => 'select',
        '#title' => t('Vocabulary'),
        '#description' => t('The terms from this vocabulary will become the access realms for the site.'),
        '#default_value' => $values['vocabulary'],
        '#options' => $options,
        '#required' => TRUE,
        '#disabled' => $has_data,
      );
      break;
  }
  return $form;
}

/**
 * Implements hook_access_realms().
 */
function access_access_realms($realm_type, $arguments = array()) {
  $values = array();
  switch ($realm_type) {
    case 'boolean':
      $values = array(0 => t('False'), 1 => t('True'));
      break;

    case 'list_integer':
    case 'list_float':
    case 'list_text':
      if (!empty($arguments['field']) && $field = field_info_field($arguments['field'])) {
        $values = list_allowed_values($field);
      }
      break;

    case 'taxonomy_term':
      if (!empty($arguments['vocabulary'])) {
        // Re-use the allowed values function for term reference fields.
        $field = array();
        $field['settings']['allowed_values'][] = array('vocabulary' => $arguments['vocabulary'], 'parent' => 0);
        $values = taxonomy_allowed_values($field);
      }
      break;

    case 'user':
      $values = db_query('SELECT uid, name FROM {users} WHERE uid > 0')->fetchAllKeyed();
      break;
  }
  return $values;
}
