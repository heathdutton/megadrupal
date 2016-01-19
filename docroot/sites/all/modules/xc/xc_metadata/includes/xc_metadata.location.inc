<?php
/**
 * @file
 * XC Metadata location functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Get xc location object
 *
 * @see xc_location_get()
 *
 * @param $location_id (int)
 *    location identifier
 * @param $update (Boolean)
 *    TRUE to refresh location cache
 *
 * @return (Object)
 *    location object. Its properties:
 *    - location_id (int)
 *    - name (string)
 *    - description (string)
 *    - type (Array)
 */
function xc_location_load($location_id, $update = FALSE) {
  return xc_location_get($location_id, $update);
}

/**
 * Load location object
 *
 * @param $location_id (int)
 *   location identifier
 *
 * @return (object)
 *   location object. Its properties are:
 *   - location_id (int): the identifier of location
 *   - name (string): the name of location
 *   - description (string)
 *   - types (array): the list of location types (usually: 'sql' and 'solr')
 *   - has_sql_type (boolean): whether the SQL storage is supported
 *   - has_solr_type (boolean): whether the Solr storage is supported
 */
function xc_location_get($location_id, $update = FALSE) {
  static $locations;

  if (!is_object($locations[$location_id]) || $update) {
    $result = db_query('SELECT location_id, name, description, types FROM {xc_location} WHERE location_id = :location_id', array(':location_id' => $location_id));
    $location = $result->fetchObject();
    $location->types = unserialize($location->types);

    if (!isset($location->has_sql_type)) {
      $location->has_sql_type = is_array($location->types) && in_array('sql', $location->types);
    }

    if (!isset($location->has_solr_type)) {
      $location->has_solr_type = is_array($location->types) && in_array('solr', $location->types);
    }

    $locations[$location_id] = $location;
  }

  return $locations[$location_id];
}

/**
 * Get all storage locations
 *
 * @param $update
 *    TRUE to update the storage location cache
 *
 * @return
 *    Array of storage location objects with 'location_id', 'name', 'description' and 'types' fields.
 */
function xc_location_get_all($update = FALSE) {
  static $locations;
  if (!is_array($locations) || $update) {
    $sql = 'SELECT location_id, name, description, types FROM {xc_location}';
    $result = db_query($sql);

    $locations = array();
    foreach ($result as $location) {
      $location->types = unserialize($location->types);
      if (!isset($location->has_sql_type)) {
        $location->has_sql_type = is_array($location->types) && in_array('sql', $location->types);
      }
      if (!isset($location->has_solr_type)) {
        $location->has_solr_type = is_array($location->types) && in_array('solr', $location->types);
      }
      $locations[$location->location_id] = $location;
    }
  }
  return $locations;
}

/**
 * Get all storage location types, such as 'sql' or 'solr'
 *
 * @return
 *    Array of all storage location types
 */
function xc_location_get_types($location = NULL) {
  return module_invoke_all('xc_location', 'types', $location);
}

/**
 * Get all sources that store in a storage location
 *
 * @param $location_id
 *    Location identifier
 * @return
 *    An array of source objects for a storage location
 */
function xc_location_get_sources($location_id) {
  $sources = array();
  $result = db_query('SELECT source_id FROM {xc_source_locations} WHERE location_id = :location_id', array(':location_id' => $location_id));
  foreach ($result as $_source) {
    $sources[$_source->source_id] = xc_source_get($_source->source_id);
  }
  return $sources;
}

/**
 * Get the location which belongs to a source
 *
 * @param $source_id (int)
 *   The source identifier
 *
 * @return (Object)
 *   The location record
 */
function xc_location_get_by_source($source_id) {
  static $locations;
  if (!isset($locations[$source_id])) {
    $result = db_query('SELECT l.* FROM {xc_location} AS l
      LEFT JOIN {xc_source_locations} AS sl ON (l.location_id = sl.location_id)
      WHERE sl.source_id = :sl.source_id', array(':sl.source_id' => (int) $source_id));
    $data = $result->fetchObject();
    $data->types = unserialize($data->types);
    $locations[$source_id] = $data;
  }
  return $locations[$source_id];
}

/**
 * Set a location object to the database
 *
 * It calls hook_xc_location, which have these known implementations:
 * - xc_solr_xc_location
 * - xc_sql_xc_location
 *
 * @param $location
 *    location object
 * @return
 *    New location identifier
 */
function xc_location_set($location) {
  $record = db_query('SELECT * FROM {xc_location} WHERE location_id = :location_id', array(':location_id' => $location->location_id))->fetchObject();
  $record->name = $location->name;
  $record->description = $location->description;
  $record->types = serialize($location->types);
  if ($record) {
    $result = drupal_write_record('xc_location', $record, 'location_id');
    if ($result) {
      module_invoke_all('xc_location', 'update', $location);
    }
  }
  else {
    $result = drupal_write_record('xc_location', $record);
    if ($result) {
      $location->location_id = $record->location_id;
      module_invoke_all('xc_location', 'create', $location);
    }
  }

  if (!$result) {
    drupal_set_message(t('Unexpected error. Unable to save metadata storage location'), 'error');
  }
}

/**
 * Empty a storage location
 *
 * @param $location
 *    Location object
 */
function xc_location_empty($location) {
  module_invoke_all('xc_location', 'purge', $location);
}

/**
 * Unset a storage location from the database, removing it completely
 *
 * @param $location
 *    Location object
 */
function xc_location_unset($location) {
  module_invoke_all('xc_location', 'delete', $location);
  $result = db_delete('xc_location')
              ->condition('location_id', $location->location_id)
              ->execute();
  if (!$result) {
    drupal_set_message(t('Unexpected error. Unable to remove metadata storage location'), 'error');
  }
}

/**
 * List all storage locations
 */
function xc_location_list() {
  $output = '';
  $header = array(
    t('Name'),
    t('Description'),
    array(
      'data' => t('Operations'),
      'colspan' => 4
    )
  );
  $rows = array();

  $pager_num = 0;
  //$sql = 'SELECT location_id, name, description FROM {xc_location}';
  //$result = pager_query($sql, 10, $pager_num, NULL);
  //$query = $query->extend('PagerDefault')->limit(10);
  $result = db_select('xc_location', 'xl')
              ->fields('xl', array('location_id', 'name', 'description'));
              //->execute();
  $result = $result->extend('PagerDefault')->limit(10)->execute();
  if ($result) {
    foreach ($result as $location) {
      $rows[] = array(
        $location->name,
        $location->description,
        l(t('view'), 'admin/xc/metadata/location/' . $location->location_id),
        l(t('edit'), 'admin/xc/metadata/location/' . $location->location_id . '/edit'),
        l(t('purge'), 'admin/xc/metadata/location/' . $location->location_id . '/purge'),
        l(t('delete'), 'admin/xc/metadata/location/' . $location->location_id . '/delete'),
      );
    }
    $output = theme('table', array('header' => $header, 'rows' => $rows));
    $output .= theme('pager', array('tags' => NULL, 'element' => $pager_num));
  }
  else {
    drupal_set_message(t('No metadata storage locations exist.'), 'warning');
  }
  return $output;
}

/**
 * View a single storage location
 */
function xc_location_view($location) {
  $header = array(t('Property'), t('Value'));
  $rows = array();
  $output = '';

  $rows[] = array(t('Name'), $location->name);
  if (!empty($location->description)) {
    $rows[] = array(t('Description'), $location->description);
  }

  $types_output = array();
  $types = xc_location_get_types($location);
  if (isset($location->types) && !empty($location->types)) {
    foreach ($location->types as $type) {
      $types_output[] = $types[$type]['name'];
    }
  }
  $rows[] = array(t('Storage types'), theme('item_list', array('items' => $types_output)));

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Display storage location name for page title
 */
function xc_location_title($location) {
  if (!is_object($location)) {
    $location = xc_location_get($location);
  }
  return $location->name;
}

/**
 * Storage location form
 */
function xc_location_form($form, &$form_state, $location = NULL) {
  $location_types = xc_location_get_types();
  if (!isset($form_state['storage'])) {
    $form_state['storage'] = array();
  }

  $location = (isset($form_state['storage']['location']) && is_object($form_state['storage']['location']))
    ? $form_state['storage']['location']
    : (is_object($location) ? $location : new stdClass());

  $step = isset($form_state['storage']['step'])
    ? (int) $form_state['storage']['step']
    : 1;

  $form_state['storage']['step'] = $step;
  $form_state['storage']['location'] = $location;

  $label = t('Add storage location');
  if (isset($location->location_id) && is_numeric($location->location_id)) {
    $label = t('Save');
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $label,
    '#weight' => 1000,
  );

  switch ($step) {
    case 1:
      $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => isset($location->name) ? $location->name : '',
        '#required' => TRUE,
      );

      $form['description'] = array(
        '#type' => 'textarea',
        '#title' => t('Description'),
        '#default_value' => isset($location->description) ? $location->description : '',
      );

      $form['submit']['#value'] = t('Continue');

      $options = array();
      $default_values = array();
      foreach ($location_types as $key => $value) {
        $options[$key] = t($value['name']);
        // the logic appears backwards, but that's because the checkboxes are to DISABLE the storage type
        if (isset($location->types) && is_array($location->types)) {
          // necessary to be in its own IF clause to ensure the final ELSE clause
          if (!in_array($key, $location->types)) {
            $default_values[] = $key;
          }
        }
        elseif (!$value['default']) {
          $default_values[] = $key;
        }
      }

      $form['advanced'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t('Advanced options'),
      );

      $form['advanced']['disabled_types'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Disable storage types'),
        '#description' => t('Check box to prevent certain storage types from being used with this metadata storage location.'),
        '#default_value' => $default_values,
        '#options' => $options,
      );
      break;

    case 2:
//      print "CASE 2";
//      $form['name'] = array(
//        '#type' => 'hidden',
//        '#value' => $form_state['values']['name']
//      );
      //
//      $form['description'] = array(
//        '#type' => 'hidden',
//        '#value' => $form_state['values']['description']
//      );
      //
//      $form['disabled_types'] = array(
//        '#type' => 'hidden',
//        '#value' => $form_state['values']['disabled_types']
//      );
      foreach ($location->types as $type) {
        $form[$type] = array(
          '#type' => 'fieldset',
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
          '#title' => check_plain($location_types[$type]['name']),
          '#description' => filter_xss($location_types[$type]['description']),
        );
      }
//    $form = array_merge_recursive($form, module_invoke_all('xc_location', 'form', $location));
      break;
  }

  return $form;
}

/**
 * Storage location validation handler
 */
function xc_location_form_validate($form, &$form_state) {
  $step     = &$form_state['storage']['step'];
  $location = &$form_state['storage']['location'];

  switch ($step) {
    case 1:
      $form_state['#rebuild'] = TRUE;
      $location->name        = $form_state['values']['name'];
      $location->description = $form_state['values']['description'];
      $location->types       = array_diff(
        array_keys(xc_location_get_types()),
        $form_state['values']['disabled_types']
      );
      if (empty($location->types)) {
        form_set_error('types', t('You cannot disable all storage types.'));
      }
      break;
    case 2:
      $form_state['#rebuild'] = FALSE;
      break;
  }
}

/**
 * Storage location submission handler
 */
function xc_location_form_submit($form, &$form_state) {
  $step     = &$form_state['storage']['step'];
  $location = &$form_state['storage']['location'];

  switch ($step) {
    case 1:
      $form_state['rebuild'] = TRUE;
      break;
    case 2:
      xc_location_set($location);
      $form_state['redirect'] = 'admin/xc/metadata/location';
      unset($form_state['storage']);
      break;
  }
  $step++;
}

/**
 * Purge (empty all metadata) storage location form
 */
function xc_location_purge_form(&$form_state, $location) {
  $form['location'] = array(
    '#type' => 'value',
    '#value' => $location,
  );
  return confirm_form(
    $form,                                // the form
    t('Are you sure you want to purge the %name storage location of all metadata?', array('%name' => $location->name)), // question
    'admin/xc/metadata/location',          // path
    '',                                    // description
    t('Purge'),                            // yes
    t('Cancel')                           // no
  );
}

/**
 * Purge (empty all metadata) storage location form submission handler
 */
function xc_location_purge_form_submit($form, &$form_state) {
  $location = $form_state['values']['location'];
  xc_location_empty($location);

  $form_state['redirect'] = 'admin/xc/metadata/location';
  drupal_set_message(t('%name storage location purged.',
    array('%name' => $location->name)));
}

/**
 * Delete storage location form
 */
function xc_location_delete_form(&$form_state, $location) {
  $form['location'] = array(
    '#type' => 'value',
    '#value' => $location,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the %name storage location?',
      array('%name' => $location->name)),
    'admin/xc/metadata/location', '', t('Delete'), t('Cancel'));
}

/**
 * Delete storage location form submission handler
 */
function xc_location_delete_form_submit($form, &$form_state) {
  $location = $form_state['values']['location'];
  xc_location_unset($location);

  $form_state['redirect'] = 'admin/xc/metadata/location';
  drupal_set_message(t('%name storage location deleted.', array('%name' => $location->name)));
}

/**
 * Storage location autocomplete
 *
 * @param $string
 *    Autocomplete string
 */
function xc_location_autocomplete($string = '') {
  $locations = array();
  if ($string) {
    $result = db_query_range("SELECT name FROM {xc_location} WHERE LOWER(name) LIKE LOWER('%s%%')", $string);
    foreach ($result as $location) {
      $locations[$location->name] = check_plain($location->location_id);
    }
  }

  drupal_json_output($locations);
}
