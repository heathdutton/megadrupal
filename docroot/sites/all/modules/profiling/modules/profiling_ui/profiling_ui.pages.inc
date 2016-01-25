<?php

/**
 * @file
 * Profiling user interface pages.
 * 
 * Remember, for most complex operations, the DBMS is ALWAYS FASTER THAN US!
 * (except when you use MySQL).
 */

/**
 * Threshold form.
 */
function profiling_ui_threshold_form($form, &$form_state, $path = NULL, $fieldset = TRUE) {
  $form = array();

  // FIXME: Fetch saved defaults for page.
  if ($path) {
    $defaults = profiling_ui_threshold_get($path);
  }

  if ($fieldset) {
    $form['values'] = array(
      '#type' => 'fieldset',
      '#title' => t("Configure threshold"),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
  }
  if (!isset($defaults)) {
    // Normalize array for later form building.
    $defaults = array();
  }
  else {
    $form['values']['#title'] . ' <em>' . t("overriden") . '</em>';
  }

  foreach (profiling_ui_threshold_types() as $type => $label) {
    // Proceed to unit specific alteration (for human readability).
    $unit = NULL;

    switch ($type) {
      case PROFILING_TYPE_MEMORY:
        $unit = PROFILING_UNIT_MBYTES;
        break;
      default:
        $unit = profiling_ui_unit_default($type);
    }

    // Common code.
    $form['values'][$type] = array(
      '#type' => 'textfield',
      '#default_value' => isset($defaults[$type]) ? $defaults[$type] : profiling_ui_threshold_get($type, $unit),
      '#title' => $label,
    );
    if ($suffix = profiling_ui_unit_suffix($unit)) {
      $form['values'][$type]['#title'] .= " (" . $suffix . ")";
    }
  }

  $form['values']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save"),
    '#submit' => array('profiling_ui_threshold_form_submit_save'),
  );
  $form['values']['buttons']['revert'] = array(
    '#type' => 'submit',
    '#value' => t("Reset to defaults"),
    '#submit' => array('profiling_ui_threshold_form_submit_reset'),
  );

  return $form;
}

/**
 * Threshold form save handler.
 */
function profiling_ui_threshold_form_submit_save($form, &$form_state) {
  $settings = array();

  foreach (profiling_ui_threshold_types() as $type => $label) {
    // Proceed to unit specific alteration (for human readability).
    $from = NULL;
    $to = profiling_ui_unit_database($type);
  
    switch ($type) {
      case PROFILING_TYPE_MEMORY:
        $from = PROFILING_UNIT_MBYTES;
        break;
      default:
        $from = profiling_ui_unit_default($type);
    }

    $settings[$type] = profiling_ui_unit_convert($from, $to, $form_state['values'][$type]);
  }

  profiling_ui_threshold_set($settings);
}

/**
 * Threshold form reset handler.
 */
function profiling_ui_threshold_form_submit_reset($form, &$form_state) {
  // Delete variable for this page.
  profiling_ui_threshold_set(NULL);
}

/**
 * Render a chart using the given query.
 */
function _profiling_ui_render_chart($values, $title = NULL, $type = 'pie2D') {
  if (module_exists('charts') && !empty($values)) {
    $chart = array(
      '#width' => 800,
      '#height' => 250,
      '#type' => $type,
      '#label' => 0,
      '#legend' => 1,
    );
    if ($title) {
      $chart['#title'] = $title;
    }
    foreach ($values as $index => $values) {
      $chart[$index] = $values;
    }
    return charts_chart($chart);
  }
  else {
    return '';
  }
}

/**
 * Simple helper for build table rows.
 */
function _profiling_ui_build_table_row($row, $headers) {
  $cells = array();
  foreach ($headers as $header) {
    if (isset($header['field'])) {
      if (isset($header['formatter']) && function_exists($header['formatter'])) {
        $formatter = $header['formatter'];
      }
      else {
        $formatter = 'check_plain';
      }
      $cells[] = array(
        'data' => $formatter($row->{$header['field']}),
      );
    }
  }
  return $cells;
}

/**
 * Simple helper for build chart values.
 */
function _profiling_ui_build_chart_value($row, $headers, $label_field = NULL) {
  $ts = tablesort_init($headers);
  if ($ts['sql'] && isset($row->{$ts['sql']}) && is_numeric($row->{$ts['sql']})) {
    if ($label_field && isset($row->{$label_field})) {
      $label = check_plain($row->{$label_field});
    }
    else {
      $label = check_plain($row->{$headers[0]['field']});
    }
    return array(
      '#label' => $label,
      '#value' => $row->{$ts['sql']},
    );
  }
}

/**
 * Per page overview.
 */
function profiling_ui_page_path_overview() {
  $headers = array(
    array('data' => t("Path"), 'field' => 'path', 'order' => 'desc', 'formatter' => 'profiling_ui_format_path'), 
    array('data' => t("Hits"), 'field' => 'hits'),
    array('data' => t("Queries min"), 'field' => 'query_count_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Queries max"), 'field' => 'query_count_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Queries avg."), 'field' => 'query_count_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Time min"), 'field' => 'duration_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Time max"), 'field' => 'duration_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Time avg."), 'field' => 'duration_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Mem min"), 'field' => 'memory_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Mem max"), 'field' => 'memory_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Mem avg."), 'field' => 'memory_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
  );
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        r.path AS path,
  //        COUNT(r.path) AS hits,
  //        MIN(r.query_count) AS query_count_min,
  //        MAX(r.query_count) AS query_count_max,
  //        AVG(r.query_count) AS query_count_avg,
  //        MIN(r.duration) AS duration_min,
  //        MAX(r.duration) AS duration_max,
  //        AVG(r.duration) AS duration_avg,
  //        MIN(r.memory) AS memory_min,
  //        MAX(r.memory) AS memory_max,
  //        AVG(r.memory) AS memory_avg
  //      FROM {profiling_request} r
  //      GROUP BY r.path

  // Using the addExpression() method seems to be the only way to make the
  // pager and sort extenders working. I'd rather keep the full hand written
  // SQL query since it's full standard SQL and it remain more readable.
  $query = db_select('profiling_request', 'r');
  $query->addField('r', 'path', 'path');
  $query->addExpression('COUNT(r.path)', 'hits');
  $query->addExpression('MIN(r.query_count)', 'query_count_min');
  $query->addExpression('MAX(r.query_count)', 'query_count_max');
  $query->addExpression('AVG(r.query_count)', 'query_count_avg');
  $query->addExpression('MIN(r.duration)', 'duration_min');
  $query->addExpression('MAX(r.duration)', 'duration_max');
  $query->addExpression('AVG(r.duration)', 'duration_avg');
  $query->addExpression('MIN(r.memory)', 'memory_min');
  $query->addExpression('MAX(r.memory)', 'memory_max');
  $query->addExpression('AVG(r.memory)', 'memory_avg');

  $objects = $query
    ->extend('TableSort')
    ->groupBy('r.path')
    ->orderByHeader($headers)
    ->execute()
    ->fetchAll();

  $chart_values = array();
  $chart_limit = 7;

  foreach ($objects as $object) {
    $rows[] = _profiling_ui_build_table_row($object, $headers);
    if ($chart_limit-- > 0 && ($value = _profiling_ui_build_chart_value($object, $headers))) {
      $chart_values[0][] = $value;
    }
  }

  $build['#theme'] = 'profiling_report';
  $build['#items'][] = array(
    'title' => t("Timer table"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)),
  );
  if (module_exists('charts') && !empty($chart_values[0])) {
    $build['#items'][] = array(
      'title' => t("Charts"),
      'content' => _profiling_ui_render_chart($chart_values),
      'collapsed' => TRUE,
    );
  }
  $build['#items'][] = array(
    'title' => t("Threshold"),
    'content' => drupal_get_form('profiling_ui_threshold_form'),
    'collapsed' => TRUE,
  );
  return $build;
}

/**
 * Global overview page.
 */
function profiling_ui_page_timer_overview() {
  $headers = array(
    array('data' => t("Name"), 'field' => 'name', 'order' => 'desc', 'formatter' => 'profiling_ui_format_name'), 
    array('data' => t("Collection"), 'field' => 'collection'),
    array('data' => t("Hits"), 'field' => 'hits'),
    array('data' => t("Queries min"), 'field' => 'query_count_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Queries max"), 'field' => 'query_count_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Queries avg."), 'field' => 'query_count_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Time min"), 'field' => 'duration_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Time max"), 'field' => 'duration_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Time avg."), 'field' => 'duration_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Mem min"), 'field' => 'memory_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Mem max"), 'field' => 'memory_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Mem avg."), 'field' => 'memory_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Time /page"), 'field' => 'per_page_duration_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_percent'),
    array('data' => t("Mem /page"), 'field' => 'per_page_memory_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_percent'),
  );
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        t.name AS name,
  //        t.collection AS collection,
  //        COUNT(t.name) AS hits,
  //        MIN(t.query_count) AS query_count_min,
  //        MAX(t.query_count) AS query_count_max,
  //        AVG(t.query_count) AS query_count_avg,
  //        MIN(t.duration) AS duration_min,
  //        MAX(t.duration) AS duration_max,
  //        AVG(t.duration) AS duration_avg,
  //        MIN(t.memory) AS memory_min,
  //        MAX(t.memory) AS memory_max,
  //        AVG(t.memory) AS memory_avg,
  //        AVG(t.duration / r.duration) AS per_page_duration_avg,
  //        AVG(t.memory / r.memory) AS per_page_memory_avg
  //      FROM {profiling_timers} t
  //      JOIN {profiling_request} r ON r.identifier = t.identifier
  //      GROUP BY t.name

  // Using the addExpression() method seems to be the only way to make the
  // pager and sort extenders working. I'd rather keep the full hand written
  // SQL query since it's full standard SQL and it remain more readable.
  $query = db_select('profiling_timers', 't');
  $query->join('profiling_request', 'r', 'r.identifier = t.identifier');
  $query->addField('t', 'name', 'name');
  $query->addField('t', 'collection', 'collection');
  $query->addExpression('COUNT(t.name)', 'hits');
  $query->addExpression('MIN(t.query_count)', 'query_count_min');
  $query->addExpression('MAX(t.query_count)', 'query_count_max');
  $query->addExpression('AVG(t.query_count)', 'query_count_avg');
  $query->addExpression('MIN(t.duration)', 'duration_min');
  $query->addExpression('MAX(t.duration)', 'duration_max');
  $query->addExpression('AVG(t.duration)', 'duration_avg');
  $query->addExpression('MIN(t.memory)', 'memory_min');
  $query->addExpression('MAX(t.memory)', 'memory_max');
  $query->addExpression('AVG(t.memory)', 'memory_avg');
  $query->addExpression('AVG(t.duration / r.duration)', 'per_page_duration_avg');
  $query->addExpression('AVG(t.memory / r.memory)', 'per_page_memory_avg');

  $objects = $query
    ->extend('TableSort')
    ->groupBy('t.name')
    ->orderByHeader($headers)
    ->execute()
    ->fetchAll();

  $chart_values = array();
  $chart_limit = 7;

  foreach ($objects as $object) {
    $rows[] = _profiling_ui_build_table_row($object, $headers);
    if ($chart_limit-- > 0 && ($value = _profiling_ui_build_chart_value($object, $headers))) {
      $chart_values[0][] = $value;
    }
  }

  $build = array('#theme' => 'profiling_report');
  $build['#items'][] = array(
    'title' => t("Timer table"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)),
  );
  if (module_exists('charts') && !empty($chart_values[0])) {
    $build['#items'][] = array(
      'title' => t("Charts"),
      'content' => _profiling_ui_render_chart($chart_values),
      'collapsed' => TRUE,
    );
  }
  $build['#items'][] = array(
    'title' => t("Threshold"),
    'content' => drupal_get_form('profiling_ui_threshold_form'),
    'collapsed' => TRUE,
  );
  return $build;
}

/**
 * Recent pages overview.
 */
function profiling_ui_page_request_overview() {
  $limit = 16;

  $headers = array(
    array('data' => t("Path"), 'field' => 'path', 'order' => 'desc', 'formatter' => 'profiling_ui_format_path'),
    array('data' => t("Request"), 'field' => 'identifier', 'order' => 'desc', 'formatter' => 'profiling_ui_format_identifier'), 
    array('data' => t("Date"), 'field' => 'created', 'order' => 'desc', 'formatter' => 'format_date'),
    array('data' => t("Timers"), 'field' => 'timer_count', 'order' => 'desc'),
    array('data' => t("Queries"), 'field' => 'query_count', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Time"), 'field' => 'duration', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Mem"), 'field' => 'memory', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
  );
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        r.path AS path,
  //        r.created AS created,
  //        r.identifier AS identifier,
  //        r.query_count AS query_count,
  //        r.duration AS duration,
  //        r.memory AS memory,
  //        COUNT(t.identifier) AS timer_count
  //      FROM {profiling_request} r
  //      JOIN {profiling_timers} t ON t.identifier = r.identifier
  //      GROUP BY r.identifier

  $query = db_select('profiling_request', 'r');
  $query->leftJoin('profiling_timers', 't', 'r.identifier = t.identifier');
  $query->addField('r', 'path', 'path');
  $query->addField('r', 'created', 'created');
  $query->addField('r', 'identifier', 'identifier');
  $query->addField('r', 'query_count', 'query_count');
  $query->addField('r', 'duration', 'duration');
  $query->addField('r', 'memory', 'memory');
  $query->addExpression('COUNT(t.identifier)', 'timer_count');

  $objects = $query
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->groupBy('r.identifier')
    ->orderByHeader($headers)
    ->execute()
    ->fetchAll();

  foreach ($objects as $object) {
    $rows[] = _profiling_ui_build_table_row($object, $headers);
  }

  $build = array('#theme' => 'profiling_report');
  $build['#items'][] = array(
    'title' => t("Latest hits"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)) . theme('pager'),
  );
  $build['#items'][] = array(
    'title' => t("Threshold"),
    'content' => drupal_get_form('profiling_ui_threshold_form'),
    'collapsed' => TRUE,
  );
  return $build;
}

/**
 * Single page overview.
 */
function profiling_ui_page_path_details($path) {
  $build = array('#theme' => 'profiling_report');
  $limit = 16;

  /*
   * General information.
   */

  $headers = array(t("Information"), t("Value"));
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        AVG(r.query_count) AS query_count_avg,
  //        MIN(r.query_count) AS query_count_min,
  //        MAX(r.query_count) AS query_count_max,
  //        AVG(r.duration) AS duration_avg,
  //        MIN(r.duration) AS duration_min,
  //        MAX(r.duration) AS duration_max,
  //        AVG(r.memory) AS memory_avg,
  //        MIN(r.memory) AS memory_min,
  //        MAX(r.memory) AS memory_max
  //      FROM {profiling_request} r
  //      WHERE r.path = '%s'
  //      GROUP BY r.path

  $query = db_select('profiling_request', 'r');
  $query->addExpression('COUNT(r.path)', 'hits');
  $query->addExpression('AVG(r.query_count)', 'query_count_avg');
  $query->addExpression('MIN(r.query_count)', 'query_count_min');
  $query->addExpression('MAX(r.query_count)', 'query_count_max');
  $query->addExpression('AVG(r.duration)', 'duration_avg');
  $query->addExpression('MIN(r.duration)', 'duration_min');
  $query->addExpression('MAX(r.duration)', 'duration_max');
  $query->addExpression('AVG(r.memory)', 'memory_avg');
  $query->addExpression('MIN(r.memory)', 'memory_min');
  $query->addExpression('MAX(r.memory)', 'memory_max');
  $query->condition('r.path', $path);
  $query->groupBy('r.path');

  $global = $query
    ->extend('TableSort')
    ->orderByHeader($headers)
    ->execute()
    ->fetchObject();

  $rows[] = array(t("Total number of hits"), $global->hits);
  if ($global->hits > 1) {
    $rows[] = array(t("Min query count"), profiling_ui_format_query_count($global->query_count_min));
    $rows[] = array(t("Max query count"), profiling_ui_format_query_count($global->query_count_max));
    $rows[] = array(t("Average query count"), profiling_ui_format_query_count($global->query_count_avg));
    $rows[] = array(t("Min execution time"), profiling_ui_format_duration($global->duration_min));
    $rows[] = array(t("Max execution time"), profiling_ui_format_duration($global->duration_max));
    $rows[] = array(t("Average execution time"), profiling_ui_format_duration($global->duration_avg));
    $rows[] = array(t("Min memory consumption"), profiling_ui_format_memory($global->memory_min));
    $rows[] = array(t("Max memory consumption"), profiling_ui_format_memory($global->memory_max));
    $rows[] = array(t("Average memory consumption"), profiling_ui_format_memory($global->memory_avg));
  }
  else {
    $rows[] = array(t("Query count"), profiling_ui_format_duration($global->query_count_avg));
    $rows[] = array(t("Execution time"), profiling_ui_format_duration($global->duration_avg));
    $rows[] = array(t("Memory consumption"), profiling_ui_format_memory($global->memory_avg));
  }

  $build['#items'][] = array(
    'title' => t("General statistics"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)),
  );

  /*
   * Timers information.
   */

  $headers = array(
    array('data' => t("Name"), 'field' => 'name', 'order' => 'desc', 'formatter' => 'profiling_ui_format_name'), 
    array('data' => t("Collection"), 'field' => 'collection'),
    array('data' => t("Hits"), 'field' => 'hits'),
    array('data' => t("Queries min"), 'field' => 'query_count_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Queries max"), 'field' => 'query_count_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Queries avg."), 'field' => 'query_count_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Time min"), 'field' => 'duration_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Time max"), 'field' => 'duration_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Time avg."), 'field' => 'duration_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Mem min"), 'field' => 'memory_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Mem max"), 'field' => 'memory_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Mem avg."), 'field' => 'memory_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Time /page"), 'field' => 'per_page_duration_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_percent'),
    array('data' => t("Mem /page"), 'field' => 'per_page_memory_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_percent'),
  );
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        t.name AS name,
  //        t.collection AS collection,
  //        COUNT(t.name) AS hits,
  //        MIN(t.query_count) AS query_count_min,
  //        MAX(t.query_count) AS query_count_max,
  //        AVG(t.query_count) AS query_count_avg,
  //        MIN(t.duration) AS duration_min,
  //        MAX(t.duration) AS duration_max,
  //        AVG(t.duration) AS duration_avg,
  //        MIN(t.memory) AS memory_min,
  //        MAX(t.memory) AS memory_max,
  //        AVG(t.memory) AS memory_avg,
  //        AVG(t.duration / r.duration) AS per_page_duration_avg,
  //        AVG(t.memory / r.memory) AS per_page_memory_avg
  //      FROM {profiling_timers} t
  //      JOIN {profiling_request} r ON r.identifier = t.identifier
  //      WHERE r.path = '%s'
  //      GROUP BY t.name

  $query = db_select('profiling_timers', 't');
  $query->join('profiling_request', 'r', 'r.identifier = t.identifier');
  $query->addField('t', 'name', 'name');
  $query->addField('t', 'collection', 'collection');
  $query->addExpression('COUNT(t.name)', 'hits');
  $query->addExpression('AVG(r.query_count)', 'query_count_avg');
  $query->addExpression('MIN(r.query_count)', 'query_count_min');
  $query->addExpression('MAX(r.query_count)', 'query_count_max');
  $query->addExpression('AVG(r.duration)', 'duration_avg');
  $query->addExpression('MIN(r.duration)', 'duration_min');
  $query->addExpression('MAX(r.duration)', 'duration_max');
  $query->addExpression('AVG(r.memory)', 'memory_avg');
  $query->addExpression('MIN(r.memory)', 'memory_min');
  $query->addExpression('MAX(r.memory)', 'memory_max');
  $query->addExpression('AVG(t.duration / r.duration)', 'per_page_duration_avg');
  $query->addExpression('AVG(t.memory / r.memory)', 'per_page_memory_avg');
  $query->condition('r.path', $path);
  $query->groupBy('t.name');

  $objects = $query
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->orderByHeader($headers)
    ->execute()
    ->fetchAll();

  $i = 0;
  foreach ($objects as $object) {
    $rows[] = _profiling_ui_build_table_row($object, $headers);
  }

  $build['#items'][] = array(
    'title' => t("Per timer statistics"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)),
  );

  /*
   * Latest hits.
   */

  $headers = array(
    array('data' => t("Date"), 'field' => 'created', 'order' => 'desc', 'formatter' => 'format_date'),
    array('data' => t("Request"), 'field' => 'identifier', 'order' => 'desc', 'formatter' => 'profiling_ui_format_identifier'),
    array('data' => t("User"), 'field' => 'uid', 'order' => 'desc', 'formatter' => 'profiling_ui_format_user'),
    array('data' => t("Theme"), 'field' => 'theme', 'order' => 'desc'),
    array('data' => t("Queries"), 'field' => 'query_count', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Time"), 'field' => 'duration', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Mem"), 'field' => 'memory', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
  );
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        r.path AS path,
  //        r.identifier AS identifier
  //        r.created AS created,
  //        r.uid AS uid,
  //        r.theme AS theme,
  //        r.query_count AS query_count,
  //        r.duration AS duration,
  //        r.memory AS memory
  //      FROM {profiling_request} r
  //      WHERE r.path = '%s'

  $query = db_select('profiling_request', 'r');
  $query->addField('r', 'identifier', 'identifier');
  $query->addField('r', 'path', 'path');
  $query->addField('r', 'created', 'created');
  $query->addField('r', 'uid', 'uid');
  $query->addField('r', 'theme', 'theme');
  $query->addField('r', 'query_count', 'query_count');
  $query->addField('r', 'duration', 'duration');
  $query->addField('r', 'memory', 'memory');
  $query->condition('r.path', $path);

  $objects = $query
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->orderByHeader($headers)
    ->execute()
    ->fetchAll();

  foreach ($objects as $object) {
    $rows[] = _profiling_ui_build_table_row($object, $headers);
  }

  $build['#items'][] = array(
    'title' => t("Page hits statistics"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)) . theme('pager'),
  );

  /*
   * Timeline.
   */
/*
  if (module_exists('charts')) {
    $sql = "
      SELECT
          r.query_count AS query_count,
          r.duration AS duration,
          r.memory AS memory
        FROM {profiling_request} r
        WHERE r.path = '%s'
        ORDER BY r.created DESC
      ";
    $args = array($path);
  
    $result = pager_query($sql, 200, 0, NULL, $args);
  
    $chart_count = 0;
    $chart_values = array(
      0 => array('#legend' => t("Query count")),
      1 => array('#legend' => t("Memory usage")),
      2 => array('#legend' => t("Time consumption")),
    );
  
    while ($object = db_fetch_object($result)) {
      $chart_values[0][] = array(
        '#label' => $chart_count++,
        '#value' => $object->query_count,
      );
      $chart_values[1][] = $object->memory;
      $chart_values[2][] = $object->duration;
    }
    // Reverse array to be in the correct order (reversed first by order desc).
    for ($i = 0; $i < 3; $i++) {
      $chart_values[$i] = array_reverse($chart_values[$i]);
    }
  
    if ($chart_count > 1) {
      $items[] = array(
        'title' => format_plural($chart_count, "Timelines from latest @count request", "Timelines from latest @count requests"),
        'content' => _profiling_ui_render_chart($chart_values, NULL, 'line2D')
      );
    }
  }
*/
  /*
   * Build report page.
   */

  $build['#items'][] = array(
    'title' => t("Threshold"),
    'content' => drupal_get_form('profiling_ui_threshold_form'),
    'collapsed' => TRUE,
  );
  return $build;
}

/**
 * Per timer overview page.
 */
function profiling_ui_page_timer_details($name) {
  $build = array('#theme' => 'profiling_report');
  drupal_set_title(t("Statistics for <em>@item</em> timer", array('@item' => profiling_timer_name($name))));

  /*
   * General statistics.
   */

  $headers = array(t("Information"), t("Value"));
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        AVG(t.query_count) AS query_count_avg,
  //        MIN(t.query_count) AS query_count_min,
  //        MAX(t.query_count) AS query_count_max,
  //        AVG(t.duration) AS duration_avg,
  //        MIN(t.duration) AS duration_min,
  //        MAX(t.duration) AS duration_max,
  //        AVG(t.memory) AS memory_avg,
  //        MIN(t.memory) AS memory_min,
  //        MAX(t.memory) AS memory_max,
  //        t.collection AS collection
  //      FROM {profiling_timers} t
  //      WHERE t.name = '%s'
  //      GROUP BY t.name

  $query = db_select('profiling_timers', 't');
  $query->addField('t', 'collection', 'collection');
  $query->addExpression('COUNT(t.name)', 'hits');
  $query->addExpression('AVG(t.query_count)', 'query_count_avg');
  $query->addExpression('MIN(t.query_count)', 'query_count_min');
  $query->addExpression('MAX(t.query_count)', 'query_count_max');
  $query->addExpression('AVG(t.duration)', 'duration_avg');
  $query->addExpression('MIN(t.duration)', 'duration_min');
  $query->addExpression('MAX(t.duration)', 'duration_max');
  $query->addExpression('AVG(t.memory)', 'memory_avg');
  $query->addExpression('MIN(t.memory)', 'memory_min');
  $query->addExpression('MAX(t.memory)', 'memory_max');
  $query->condition('t.name', $name);
  $query->groupBy('t.name');

  $global = $query
    ->extend('TableSort')
    ->orderByHeader($headers)
    ->execute()
    ->fetchObject();

  $item = array();
  $item[] = array(
    '#prefix' => '<p>',
    '#suffix' => '</p>',
    '#markup' => '<strong>' . profiling_timer_help($name) . '</strong><br/>' . t("This timer belongs to the <strong title=\"@description\">@title</strong> timer collection.", array(
      '@description' => profiling_collection_help($global->collection),
      '@title' => profiling_collection_name($global->collection),
    )),
  );

  $rows[] = array(t("Total number of hits"), $global->hits);
  if ($global->hits > 1) {
    $rows[] = array(t("Min query count"), profiling_ui_format_query_count($global->query_count_min));
    $rows[] = array(t("Max query count"), profiling_ui_format_query_count($global->query_count_max));
    $rows[] = array(t("Average query count"), profiling_ui_format_query_count($global->query_count_avg));
    $rows[] = array(t("Min execution time"), profiling_ui_format_duration($global->duration_min));
    $rows[] = array(t("Max execution time"), profiling_ui_format_duration($global->duration_max));
    $rows[] = array(t("Average execution time"), profiling_ui_format_duration($global->duration_avg));
    $rows[] = array(t("Min memory consumption"), profiling_ui_format_memory($global->memory_min));
    $rows[] = array(t("Max memory consumption"), profiling_ui_format_memory($global->memory_max));
    $rows[] = array(t("Average memory consumption"), profiling_ui_format_memory($global->memory_avg));
  }
  else {
    $rows[] = array(t("Query count"), profiling_ui_format_query_count($global->query_count_avg));
    $rows[] = array(t("Execution time"), profiling_ui_format_duration($global->duration_avg));
    $rows[] = array(t("Memory consumption"), profiling_ui_format_memory($global->memory_min));
  }
  $item[]['#markup'] = theme('table', array('header' => $headers, 'rows' => $rows));

  $build['#items'][] = array(
    'title' => t("General statistics"),
    'content' => $item,
  );

  /*
   * Per page statistics.
   */

  $headers = array(
    array('data' => t("Path"), 'field' => 'path', 'order' => 'desc', 'formatter' => 'profiling_ui_format_path'), 
    array('data' => t("Hits"), 'field' => 'hits'),
    array('data' => t("Queries min"), 'field' => 'query_count_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Queries max"), 'field' => 'query_count_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Queries avg."), 'field' => 'query_count_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Time min"), 'field' => 'duration_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Time max"), 'field' => 'duration_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Time avg."), 'field' => 'duration_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Mem min"), 'field' => 'memory_min', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Mem max"), 'field' => 'memory_max', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
    array('data' => t("Mem avg."), 'field' => 'memory_avg', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
  );
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        r.path AS path,
  //        COUNT(r.path) AS hits,
  //        MIN(t.query_count) AS query_count_min,
  //        MAX(t.query_count) AS query_count_max,
  //        AVG(t.query_count) AS query_count_avg,
  //        MIN(t.duration) AS duration_min,
  //        MAX(t.duration) AS duration_max,
  //        AVG(t.duration) AS duration_avg,
  //        MIN(t.memory) AS memory_min,
  //        MAX(t.memory) AS memory_max,
  //        AVG(t.memory) AS memory_avg
  //      FROM {profiling_timers} t
  //      JOIN {profiling_request} r ON r.identifier = t.identifier
  //      WHERE t.name = '%s'
  //      GROUP BY r.path

  $query = db_select('profiling_timers', 't');
  $query->join('profiling_request', 'r', 'r.identifier = t.identifier');
  $query->addField('r', 'path', 'path');
  $query->addExpression('COUNT(t.name)', 'hits');
  $query->addExpression('AVG(r.query_count)', 'query_count_avg');
  $query->addExpression('MIN(r.query_count)', 'query_count_min');
  $query->addExpression('MAX(r.query_count)', 'query_count_max');
  $query->addExpression('AVG(r.duration)', 'duration_avg');
  $query->addExpression('MIN(r.duration)', 'duration_min');
  $query->addExpression('MAX(r.duration)', 'duration_max');
  $query->addExpression('AVG(r.memory)', 'memory_avg');
  $query->addExpression('MIN(r.memory)', 'memory_min');
  $query->addExpression('MAX(r.memory)', 'memory_max');
  $query->condition('t.name', $name);
  $query->groupBy('r.path');

  $objects = $query
    ->extend('TableSort')
    ->orderByHeader($headers)
    ->execute()
    ->fetchAll();

  foreach ($objects as $object) {
    $rows[] = _profiling_ui_build_table_row($object, $headers);
  }

  $build['#items'][] = array(
    'title' => t("Per page statistics"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)),
  );

  /*
   * Timeline.
   */

  /*
  if (module_exists('charts')) {
    $sql = "
      SELECT
          t.query_count AS query_count,
          t.duration AS duration,
          t.memory AS memory
        FROM {profiling_timers} t
        JOIN {profiling_request} r ON r.identifier = t.identifier
        WHERE t.name = '%s'
        ORDER BY r.created DESC
      ";
    $args = array($name);
  
    $result = pager_query($sql, 200, 0, NULL, $args);
  
    $chart_count = 0;
    $chart_values = array(
      0 => array('#legend' => t("Query count")),
      1 => array('#legend' => t("Memory usage")),
      2 => array('#legend' => t("Time consumption")),
    );
  
    while ($object = db_fetch_object($result)) {
      $chart_values[0][] = array(
        '#label' => $chart_count++,
        '#value' => $object->query_count,
      );
      $chart_values[1][] = $object->memory;
      $chart_values[2][] = $object->duration;
    }
    // Reverse array to be in the correct order (reversed first by order desc).
    for ($i = 0; $i < 3; $i++) {
      $chart_values[$i] = array_reverse($chart_values[$i]);
    }
  
    if ($chart_count > 1) {
      $items[] = array(
        'title' => format_plural($chart_count, "Timelines from latest @count request", "Timelines from latest @count requests"),
        'content' => _profiling_ui_render_chart($chart_values, NULL, 'line2D')
      );
    }
  }
   */

  /*
   * Build report page.
   */

  $build['#items'][] = array(
    'title' => t("Threshold"),
    'content' => drupal_get_form('profiling_ui_threshold_form'),
    'collapsed' => TRUE,
  );
  return $build;
}

/**
 * Request details page.
 */
function profiling_ui_page_request_details($identifier) {
  $build = array('#theme' => 'profiling_report');

  /*
   * General statistics.
   */

  $headers = array(t("Information"), t("Value"));
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        r.uid AS uid,
  //        r.path AS path,
  //        r.time_start AS time_start,
  //        r.created AS created,
  //        r.theme AS theme,
  //        r.query_count AS query_count,
  //        r.duration AS duration,
  //        r.memory AS memory
  //      FROM {profiling_request} r
  //      WHERE r.identifier = '%s'

  $query = db_select('profiling_request', 'r');
  $query->addField('r', 'uid', 'uid');
  $query->addField('r', 'path', 'path');
  $query->addField('r', 'time_start', 'time_start');
  $query->addField('r', 'created', 'created');
  $query->addField('r', 'theme', 'theme');
  $query->addField('r', 'query_count', 'query_count');
  $query->addField('r', 'duration', 'duration');
  $query->addField('r', 'memory', 'memory');
  $query->condition('r.identifier', $identifier);

  $global = $query
    ->extend('TableSort')
    ->orderByHeader($headers)
    ->execute()
    ->fetchObject();

  $rows[] = array(t("Path"), profiling_ui_format_path($global->path));
  $rows[] = array(t("Server time"), format_date($global->created));
  $rows[] = array(t("Logged-in user"), profiling_ui_format_user($global->uid));
  $rows[] = array(t("Active theme"), check_plain($global->theme));
  $rows[] = array(t("Query count"), profiling_ui_format_query_count($global->query_count));
  $rows[] = array(t("Execution time"), profiling_ui_format_duration($global->duration));
  $rows[] = array(t("Memory usage"), profiling_ui_format_memory($global->memory));

  $build['#items'][] = array(
    'title' => t("General statistics"),
    'content' => theme('table', array('headers' => $headers, 'rows' => $rows)),
  );

  /*
   * Per timer statistics.
   */

  $headers = array(
    array('data' => t("Name"), 'field' => 'name', 'order' => 'desc', 'formatter' => 'profiling_ui_format_name'), 
    array('data' => t("Collection"), 'field' => 'collection'),
    array('data' => t("Queries"), 'field' => 'query_count', 'order' => 'desc', 'formatter' => 'profiling_ui_format_query_count'),
    array('data' => t("Time"), 'field' => 'duration', 'order' => 'desc', 'formatter' => 'profiling_ui_format_duration'),
    array('data' => t("Mem"), 'field' => 'memory', 'order' => 'desc', 'formatter' => 'profiling_ui_format_memory'),
  );
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        t.name AS name,
  //        t.collection AS collection,
  //        t.time_start AS time_start,
  //        t.time_stop AS time_stop,
  //        t.query_count AS query_count,
  //        t.duration AS duration,
  //        t.memory AS memory
  //      FROM {profiling_timers} t
  //      WHERE t.identifier = '%s'

  $query = db_select('profiling_timers', 't');
  $query->addField('t', 'name', 'name');
  $query->addField('t', 'delta', 'delta');
  $query->addField('t', 'collection', 'collection');
  $query->addField('t', 'time_start', 'time_start');
  $query->addField('t', 'time_stop', 'time_stop');
  $query->addField('t', 'query_count', 'query_count');
  $query->addField('t', 'duration', 'duration');
  $query->addField('t', 'memory', 'memory');
  $query->condition('t.identifier', $identifier);

  $timers = $query
    ->extend('TableSort')
    ->orderByHeader($headers)
    ->execute()
    ->fetchAll();

  foreach ($timers as $timer) {
    $rows[] = _profiling_ui_build_table_row($timer, $headers);
  }

  $build['#items'][] = array(
    'title' => t("Per timer statistics"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)),
  );

  /*
   * Timeline.
   */

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        t.name AS name,
  //        t.collection AS collection,
  //        t.time_start AS time_start,
  //        t.time_stop AS time_stop,
  //        t.query_count AS query_count,
  //        t.duration AS duration,
  //        t.memory AS memory,
  //        t.delta AS delta,
  //        t.identifier AS identifier
  //      FROM {profiling_timers} t
  //      WHERE t.identifier = '%s'
  //      ORDER BY t.time_start ASC

  $query = db_select('profiling_timers', 't');
  $query->addField('t', 'name', 'name');
  $query->addField('t', 'delta', 'delta');
  $query->addField('t', 'collection', 'collection');
  $query->addField('t', 'time_start', 'time_start');
  $query->addField('t', 'time_stop', 'time_stop');
  $query->addField('t', 'query_count', 'query_count');
  $query->addField('t', 'duration', 'duration');
  $query->addField('t', 'identifier', 'identifier');
  $query->condition('t.identifier', $identifier);

  $timers = $query
    ->orderBy('t.time_start', 'asc')
    ->execute()
    ->fetchAll();

  $build['#items'][] = array(
    'title' => t("Timeline"),
    'content' => array(
      '#theme' => 'profiling_timeline',
      '#timers' => $timers,
      '#time_start' => $global->time_start,
      '#duration' => $global->duration,
    ),
  );

  /*
   * Build report page.
   */

  $build['#items'][] = array(
    'title' => t("Threshold"),
    'content' => drupal_get_form('profiling_ui_threshold_form'),
    'collapsed' => TRUE,
  );
  return $build;
}

/**
 * Timer details inside one request.
 */
function profiling_ui_page_timer_details_single($timer) {
  $build = array('#theme' => 'profiling_report');

  /*
   * Request reminder.
   */

  $headers = array(t("Information"), t("Value"));
  $rows = array();

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        r.uid AS uid,
  //        r.path AS path,
  //        r.time_start AS time_start,
  //        r.created AS created,
  //        r.theme AS theme,
  //        r.query_count AS query_count,
  //        r.duration AS duration,
  //        r.memory AS memory
  //      FROM {profiling_request} r
  //      WHERE r.identifier = '%s'

  $query = db_select('profiling_request', 'r');
  $query->addField('r', 'uid', 'uid');
  $query->addField('r', 'path', 'path');
  $query->addField('r', 'time_start', 'time_start');
  $query->addField('r', 'created', 'created');
  $query->addField('r', 'theme', 'theme');
  $query->addField('r', 'query_count', 'query_count');
  $query->addField('r', 'duration', 'duration');
  $query->addField('r', 'memory', 'memory');
  $query->condition('r.identifier', $timer->identifier);

  $request = $query
    ->extend('TableSort')
    ->orderByHeader($headers)
    ->execute()
    ->fetchObject();

  $rows[] = array(t("Path"), profiling_ui_format_path($request->path));
  $rows[] = array(t("Server time"), format_date($request->created));
  $content = theme('table', array('header' => $headers, 'rows' => $rows));

  $build['#items'][] = array(
    'title' => t("Request reminder"),
    'content' => $content . '<p>' . profiling_ui_format_identifier($timer->identifier) . '</p>',
  );

  /*
   * Single timer entry details.
   */

  $headers = array(t("Information"), t("Value"));
  $rows = array();

  $rows[] = array(t("Name"), profiling_ui_format_name($timer->name));
  $rows[] = array(t("Collection"), profiling_ui_format_collection($timer->collection));
  $rows[] = array(t("Query count"), profiling_ui_format_query_count($timer->query_count));
  $rows[] = array(t("Execution time"), profiling_ui_format_duration($timer->duration));
  $rows[] = array(t("Memory consumption"), profiling_ui_format_memory($timer->memory));

  $build['#items'][] = array(
    'title' => t("Single timer execution details"),
    'content' => theme('table', array('header' => $headers, 'rows' => $rows)),
  );

  /*
   * Single timer inside timeline.
   */

  // Keeping the good old SQL query, since it's working standard SQL.
  // --
  //    SELECT
  //        t.name AS name,
  //        t.collection AS collection,
  //        t.time_start AS time_start,
  //        t.time_stop AS time_stop,
  //        t.query_count AS query_count,
  //        t.duration AS duration,
  //        t.memory AS memory,
  //        t.delta AS delta,
  //        t.identifier AS identifier
  //      FROM {profiling_timers} t
  //      WHERE t.identifier = '%s'
  //        AND t.time_start >= %f
  //        AND t.time_stop <= %f
  //      ORDER BY t.time_start ASC

  $query = db_select('profiling_timers', 't');
  $query->addField('t', 'name', 'name');
  $query->addField('t', 'delta', 'delta');
  $query->addField('t', 'collection', 'collection');
  $query->addField('t', 'time_start', 'time_start');
  $query->addField('t', 'time_stop', 'time_stop');
  $query->addField('t', 'query_count', 'query_count');
  $query->addField('t', 'duration', 'duration');
  $query->addField('t', 'identifier', 'identifier');
  $query->condition('t.identifier', $timer->identifier);
  $query->condition('t.time_start', $timer->time_start, '>=');
  $query->condition('t.time_stop', $timer->time_stop, '<=');

  $timers = $query
    ->orderBy('t.time_start', 'asc')
    ->execute()
    ->fetchAll();

  $build['#items'][] = array(
    'title' => t("Timeline"),
    'content' => array(
      '#theme' => 'profiling_timeline',
      '#timers' => $timers,
      '#time_start' => $timer->time_start,
      '#duration' => $timer->duration,
    ),
  );
  return $build;
}
