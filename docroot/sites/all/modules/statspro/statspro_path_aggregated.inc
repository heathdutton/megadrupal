<?php

/**
 * @file
 *   Path aggregated page.
 *
 */

require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/statspro_settings.inc';

define('STATSPRO_PATH_AGGREGATED_EXECUTION_SECONDS_PER_DAY', 2);

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_path_aggregator_list() {
  if (!statspro_path_aggregated_mandatory()) {
    return "\n<p>" . t('Dependencies not met.') . "</p>\n";
  }

  // No path aggregators defined.
  $row_count = db_query('SELECT COUNT(spaid) FROM {statspro_path_aggregator}')->fetchField();
  if ($row_count == 0) {
    $output = "\n<p class='statspro_warning'>" . t('No path aggregators defined.') . "</p>\n";
  }
  else {
    $output = drupal_get_form('statspro_path_aggregator_list_form');
  }

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_path_aggregator_list_form($form) {
  $result = db_query('SELECT
        n.spaid, n.name, n.weight
      FROM {statspro_path_aggregator} n
      ORDER BY n.weight,
        n.name,
        n.spaid');
  $form = array('#theme' => 'statspro_path_aggregator_list_form');

  $edit = t('Edit');
  $delete = t('Delete');
  while ($row = db_fetch_array($result)) {
    $spaid = (int) $row['spaid'];
    $element = _statspro_form_field_name('aggregator', $spaid);
    $form[$element] = array('#type' => 'fieldset');
    $subelement = _statspro_form_field_name('spaid', $spaid);
    $form[$element][$subelement] = array(
      '#value' => (int) $row['spaid'],
      '#type' => 'value',
    );
    $subelement = _statspro_form_field_name('name', $spaid);
    $form[$element][$subelement] = array(
      '#value' => $row['name'],
      '#type' => 'item',
    );
    $subelement = _statspro_form_field_name('weight', $spaid);
    $form[$element][$subelement] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => (int) $row['weight'],
    );
    $form[$element]['edit'] = array(
      '#type' => 'markup',
      '#value' => sprintf(
        "<a href='/%s/%u'>%s</a>",
        'admin/config/statspro/path_aggregator/edit',
        $spaid,
        $edit
      ),
    );
    $form[$element]['delete'] = array(
      '#type' => 'markup',
      '#value' => sprintf(
        "<a href='/%s/%u'>%s</a>",
        'admin/config/statspro/path_aggregator/delete',
        $spaid,
        $delete
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['cancel'] = array(
    '#value' => sprintf(
      "<a href='/%s'>%s</a>",
      'admin/config/statspro/path_aggregator/list',
      t('Cancel')
    ),
  );

  return $form;
}

/**
 * Returns the form field name to be used for this specific field.
 *
 * @param string $name
 * @param int $id
 * @return string The form field name
 */
function _statspro_form_field_name($name, $id) {
  return sprintf('%s-%u', $name, $id);
}

/**
 * Implementation of theme_form() for statspro_path_aggregator_list_form.
 */
function theme_statspro_path_aggregator_list_form($variables) {
  $form = $variables['form'];
  $headers = array('', t('Name'), t('Weight'), '', '');
  $rows = array();
  $content = '';
  foreach ($form as $name => $elements) {
    if (strpos($name, '#') === 0) {
      continue;
    }
    if (strpos($name, 'aggregator') === 0) {
      $row = array();
      foreach ($elements as $element_name => $element) {
        if (strpos($element_name, '#') === 0) {
          continue;
        }
        unset($element['#title']);
        // TODO Please change this theme call to use an associative array for the $variables parameter.
        $row[] = theme($element['#type'], $element);
      }
      $rows[] = $row;
    }
    else {
      $content .= drupal_render_children($elements);
    }
  }
  $content = theme('table', array('header' => $headers, 'rows' => $rows)) . $content;
  return $content;
}

/**
 * Implemnetation of submit() for the statspro_path_aggregator_list_form form.
 */
function statspro_path_aggregator_list_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  foreach ($values as $name => $value) {
    if (!preg_match('/spaid-(\d+)/', $name, $result)) {
      continue;
    }
    $spaid = $result[1];
    $weight_name = _statspro_form_field_name('weight', $spaid);
    $weight = $values[$weight_name];
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query("UPDATE {statspro_path_aggregator}
     SET weight = %d
     WHERE spaid = %u", $weight, $spaid) */
    db_update('statspro_path_aggregator')
  ->fields(array(
    'weight' => $weight,
  ))
  ->condition('spaid', '%u')
  ->execute();
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_path_aggregator_add() {
  if (!statspro_path_aggregated_mandatory()) {
    return "\n<p>" . t('Dependencies not met.') . "</p>\n";
  }

  $output = drupal_get_form('statspro_path_aggregator_add_form');

  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_path_aggregator_add_form($form) {
  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 54,
    '#required' => TRUE,
  );

  $form['paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => t('Include one path per line. Wildcards are accepted.'),
    '#required' => TRUE,
    '#size' => 54,
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#required' => TRUE,
    '#description' => t('In the report, the items with greater weight will sink and the ones width smaller weight will be positioned nearer the top.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  $form['cancel'] = array(
    '#value' => sprintf("<a href='/admin/settings/statspro/path_aggregator/list'>%s</a>", t('Cancel')),
  );

  return $form;
}

/**
 * Implementation of submit() for the statspro_path_aggregator_add_form form.
 */
function statspro_path_aggregator_add_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $clean_paths = _statspro_path_cleaner($values['paths']);
  $name = trim($values['name']);
  $object = array(
    'name' => $name,
    'paths' => implode("\n", $clean_paths),
    'weight' => $values['weight'],
  );
  if (drupal_write_record('statspro_path_aggregator', $object)) {
    drupal_set_message(t('New path aggregator %path_aggregator_name created.', array('%path_aggregator_name' => $name)));
    drupal_goto('admin/config/statspro/path_aggregator/list');
  }
  else {
    drupal_set_message(t('Error creating new path aggregator %path_aggregator_name.', array('%path_aggregator_name' => $name)) . 'error');
    drupal_goto('admin/config/statspro/path_aggregator/list');
  }
}

/**
 * Returns a string array with cleaned paths.
 *
 * @param string $original_paths
 * @return array String array with cleaned paths.
 */
function _statspro_path_cleaner($original_paths) {
  $paths = explode("\n", $original_paths);
  $clean_paths = array();
  foreach ($paths as $path) {
    $path = _statspro_clean_path($path);
    if ($path) {
      $clean_paths[] = $path;
    }
  }
  return $clean_paths;
}

/**
 * Returns the cleaned version of the provided path.
 *
 * @param string $path
 * @return string Returns the cleaned path
 */
function _statspro_clean_path($path) {
  $path = trim($path);
  if (strpos($path, '/') === 0) {
    $path = drupal_substr($path, 1);
  }
  return $path;
}

/**
 * Edit path aggregator.
 */
function statspro_path_aggregator_edit($spaid) {
  if (!statspro_path_aggregated_mandatory()) {
    return "\n<p>" . t('Dependencies not met.') . "</p>\n";
  }

  $result = db_query('SELECT
    n.spaid, n.name, n.paths, n.weight
    FROM {statspro_path_aggregator} n
    WHERE n.spaid = :n.spaid', array(':n.spaid' => $spaid));
  $spa = db_fetch_array($result);
  if (!is_array($spa)) {
    drupal_set_message(t('Aggregator spaid %path_aggregator_id not found.', array('%path_aggregator_id' => $spaid)));
    drupal_goto('admin/config/statspro/path_aggregator/list');
    return;
  }
  $output = drupal_get_form('statspro_path_aggregator_edit_form', $spa);
  return $output;
}

/**
 * The statspro_path_aggregator_edit_form form.
 */
function statspro_path_aggregator_edit_form($form, $spa) {
  $form = array();

  $form['spaid'] = array(
    '#type' => 'value',
    '#value' => (int) $spa['spaid'],
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 54,
    '#required' => TRUE,
    '#default_value' => $spa['name'],
  );

  $form['paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => t('Include one path per line. Wildcards are accepted.'),
    '#required' => TRUE,
    '#size' => 54,
    '#default_value' => $spa['paths'],
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#required' => TRUE,
    '#description' => t('In the report, the items with greater weight will sink and the ones width smaller weight will be positioned nearer the top.'),
    '#default_value' => (int) $spa['weight'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['cancel'] = array(
    '#value' => sprintf("<a href='/admin/settings/statspro/path_aggregator/list'>%s</a>", t('Cancel')),
  );

  return $form;
}

/**
 * Implementation of submit() for the statspro_path_aggregator_edit_form form.
 */
function statspro_path_aggregator_edit_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $clean_paths = _statspro_path_cleaner($values['paths']);
  $name = trim($values['name']);
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query("UPDATE
   {statspro_path_aggregator}
   SET name = '%s', paths = '%s', weight = %d
   WHERE spaid = %d", $name, implode("\n", $clean_paths), $values['weight'], $form_state['spaid']['#value']) */
  db_update('statspro_path_aggregator')
  ->fields(array(
    'name' => $name,
    'paths' => implode("\n", $clean_paths),
    'weight' => $values['weight'],
  ))
  ->condition('spaid', $form_state['spaid']['#value'])
  ->execute();
  drupal_set_message(t('Aggregator %path_aggregator_name updated.', array('%path_aggregator_name' => $name)));
  drupal_goto('admin/config/statspro/path_aggregator/list');
}

/**
 * Delete path aggregator.
 */
function statspro_path_aggregator_delete($spaid) {
  if (!statspro_path_aggregated_mandatory()) {
    return "\n<p>" . t('Dependencies not met.') . "</p>\n";
  }

  $result = db_query("SELECT
    n.spaid, n.name, n.paths, n.weight
    FROM {statspro_path_aggregator} n
    WHERE n.spaid = :n.spaid", array(':n.spaid' => $spaid));
  $spa = db_fetch_array($result);
  if (!is_array($spa)) {
    drupal_set_message(t('Aggregator spaid %path_aggregator_id not found.', array('%path_aggregator_id' => $spaid)));
    drupal_goto('admin/config/statspro/path_aggregator/list');
    return;
  }
  $output = drupal_get_form('statspro_path_aggregator_delete_form', $spa);
  return $output;
}

/**
 * The statspro_path_aggregator_delete_form form.
 */
function statspro_path_aggregator_delete_form($form, $spa) {
  $form = array();

  $form['spaid'] = array(
    '#type' => 'value',
    '#value' => (int) $spa['spaid'],
  );

  $form['name'] = array(
    '#type' => 'item',
    '#title' => t('Name'),
    '#size' => 54,
    '#value' => $spa['name'],
    '#disabled' => TRUE,
  );

  $form['paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => t('Include one path per line. Wildcards are accepted.'),
    '#size' => 54,
    '#value' => $spa['paths'],
    '#disabled' => TRUE,
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#description' => t('In the report, the items with greater weight will sink and the ones width smaller weight will be positioned nearer the top.'),
    '#value' => (int) $spa['weight'],
    '#disabled' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  $form['cancel'] = array(
    '#value' => sprintf("<a href='/admin/settings/statspro/path_aggregator/list'>%s</a>", t('Cancel')),
  );

  return $form;
}

/**
 * Implementation of submit for the statspro_path_aggregator_delete_form form.
 */
function statspro_path_aggregator_delete_form_submit($form, &$form_state) {
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query("DELETE FROM
   {statspro_path_aggregator}
   WHERE spaid = %d", $form_state['spaid']['#value']) */
  db_delete('statspro_path_aggregator')
  ->condition('spaid', $form_state['spaid']['#value'])
  ->execute();
  drupal_set_message(t('Aggregator spaid %path_aggregator_id deleted.', array('%path_aggregator_id' => $form_state['spaid']['#value'])));
  drupal_goto('admin/config/statspro/path_aggregator/list');
}

/**
 * Callback function for path aggregated page.
 */
function statspro_path_aggregated($period = '', $number_of_days = NULL) {
  $dependency_ok = statspro_path_aggregated_mandatory();

  if (!$dependency_ok) {
    return "\n<p>" . t('Dependencies not met.') . "</p>\n";
  }

  // add settings form
  $output = drupal_get_form(
    'statspro_path_aggregated_report_form',
    $period,
    $number_of_days
  );

  $period_info = statspro_get_period_info(
    $_SESSION['statspro_period_path_aggregated'],
    $_SESSION['statspro_custom_number_days_path_aggregated']
  );

  $output .= _statspro_path_aggregated_html_configurable_period_report(
    $period_info['period'],
    $period_info['period_name']
  );

  return $output;
}

/**
 * statspro_path_aggregated_report_form() definition.
 */
function statspro_path_aggregated_report_form(&$form_state, $period = '', $number_of_days = NULL) {
  set_default_session_values('_path_aggregated', $period, $number_of_days);

  return statspro_settings_form(
    $_SESSION['statspro_period_path_aggregated'],
    $_SESSION['statspro_custom_number_days_path_aggregated']
  );
}

/**
 * Implements hook_modulename_submit() for().
 * statspro_path_aggregated_report_form form.
 */
function statspro_path_aggregated_report_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Reset to defaults')) {
    unset($_SESSION['statspro_period_path_aggregated']);
    unset($_SESSION['statspro_custom_number_day_path_aggregated']);
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
  else {
    $_SESSION['statspro_period_path_aggregated'] = $form_state['values']['statspro_period'];
    if ($form_state['values']['statspro_period'] == 'custom_days') {
      $_SESSION['statspro_custom_number_days_path_aggregated'] = $form_state['values']['statspro_custom_number_days'];
    }
    drupal_set_message(t('The configuration options have been saved.'));
  }
}

/**
 * Returns the HTML version of the configurable period report.
 *
 * @param <string> $period
 * @return <string> HTML version of the configurable period report.
 */
function _statspro_path_aggregated_html_configurable_period_report($period, $period_name) {
  $table_data = statspro_path_aggregated_get_data_for_configurable_period_report($period);
  if (count($table_data) == 0) {
    return "\n<p class='statspro_warning'>" . t('No path aggregators defined.') . "</p>\n";
  }
  if (statspro_get_available_charts_graphs()) {
    $content = statspro_get_path_aggregated_graphs(
      $table_data['headers'],
      $table_data['rows']
    );
  }
  else {
    $content = '';
  }
  // TODO Please change this theme call to use an associative array for the $variables parameter.
  $content .= theme(
    'table',
    $table_data['headers'],
    $table_data['rows'],
    array(),
    $period_name
  );
  return $content;
}

/**
 * Returns the HTML for all path aggregated graphs.
 *
 * @param <array> $series_names
 * @param <array> $series_data
 * @return <string>
 */
function statspro_get_path_aggregated_graphs($series_names, $series_data) {
  $output = '';
  $x_labels = array();

  $series_name_index = 0;
  foreach ($series_names as $serie_name) {
    if ($series_name_index == 0) {
      // Getting the name of each aggregator.
      foreach ($series_data as $serie_data) {
        $x_labels[] = $serie_data[0];
      }
    }
    else {
      // Building data serie for each graph.
      $title = t('Path aggregated - @title_suffix', array('@title_suffix' => $serie_name));
      $serie = array();
      foreach ($series_data as $serie_data) {
        $serie[] = $serie_data[$series_name_index];
      }
      if (count($serie) != 0) {
        $output .= statspro_get_pie_chart($title, array($serie), $x_labels);
      }
    }
    $series_name_index++;
  }
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_path_aggregated_get_data_for_configurable_period_report($period) {
  $aggregated_data = _statspro_path_aggregated_get_aggregated_data($period);
  if (count($aggregated_data) == 0) {
    return array();
  }
  $table_data = _statspro_path_aggregated_get_table_data($aggregated_data);
  return $table_data;
}

/**
 * Returns the provided path aggregated data in the default table format.
 *
 * @param <array> $path_aggregated_data
 * @return <array>
 */
function _statspro_path_aggregated_get_table_data($path_aggregated_data) {
  $table = array();
  $table['headers'] = array(
    t('Path aggregator'),
    t('Access counter'),
    t('Session'),
    t('Machine'),
  );
  $rows = array();
  foreach ($path_aggregated_data as $path_aggregator) {
    $row = array(
      $path_aggregator['name'],
      (int) $path_aggregator['access_counter'],
      count($path_aggregator['sessions']),
      count($path_aggregator['hosts']),
    );
    $rows[] = $row;
  }
  $table['rows'] = $rows;
  return $table;
}

function _statspro_path_aggregated_get_aggregated_data($period) {
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/statspro.inc';

  $statspro = new StatsPro();
  $qt_days = $statspro->get_qt_days_per_period($period);
  $minimal_execution_time = STATSPRO_PATH_AGGREGATED_EXECUTION_SECONDS_PER_DAY * $qt_days;
  if ($minimal_execution_time > ini_get('max_execution_time')) {
    drupal_set_time_limit($minimal_execution_time);
  }
  $sql_where = $statspro->get_period('a.timestamp', TRUE, $period);
  // TODO Please convert this statement to the D7 database API syntax.
  $result = db_query("SELECT
    a.sid, a.path, a.hostname
    FROM {accesslog} a
    WHERE %s", $sql_where);
  $path_aggregated_data = _statspro_path_aggregated_get_aggregated_data_from_result($result);
  return $path_aggregated_data;
}

function _statspro_path_aggregated_get_aggregated_data_from_result($result) {
  $path_aggregators = _statspro_path_aggregated_get_aggregators();
  while ($access = db_fetch_array($result)) {
    $translated_path = drupal_get_path_alias($access['path']);
    foreach ($path_aggregators as $key => $path_aggregator) {
      if (drupal_match_path($translated_path, $path_aggregator['paths'])) {
        _statspro_path_aggregated_add_access(
          $path_aggregators,
          $key,
          $access['sid'],
          $access['hostname']
        );
        break;
      }
    }
  }
  return $path_aggregators;
}

/**
 * Returns a path aggregator array in priority order.
 *
 * @return array Array of path aggregators in priority order.
 */
function _statspro_path_aggregated_get_aggregators() {
  $result = db_query('SELECT
    n.spaid, n.name, n.paths
    FROM {statspro_path_aggregator} n
    ORDER BY n.weight, n.name, n.spaid'
  );
  $path_aggregators = array();
  while ($path_aggregator = db_fetch_array($result)) {
    $path_aggregators[] = array(
      'name' => $path_aggregator['name'],
      'paths' => $path_aggregator['paths'],
      'spaid' => (int) $path_aggregator['spaid'],
      'access_counter' => 0,
      'hosts' => array(),
      'sessions' => array(),
    );
  }
  return $path_aggregators;
}

/**
 * Increments the counters on the $key path aggregator.
 *
 * @param array $path_aggregators
 * @param int $key
 * @param string $sid
 * @param string $host
 */
function _statspro_path_aggregated_add_access(&$path_aggregators, $key, $sid, $host) {
  $path_aggregators[$key]['access_counter']++;
  if (isset($path_aggregators[$key]['sessions'][$sid])) {
    $path_aggregators[$key]['sessions'][$sid]++;
  }
  else {
    $path_aggregators[$key]['sessions'][$sid] = 1;
  }
  if (isset($path_aggregators[$key]['hosts'][$host])) {
    $path_aggregators[$key]['hosts'][$host]++;
  }
  else {
    $path_aggregators[$key]['hosts'][$host] = 1;
  }
}
