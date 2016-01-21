<?php

function watchdog_debug_admin_settings() {
  $severity_levels = watchdog_severity_levels();
  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General configuration'),
    '#collapsible' => TRUE,
  );
  $form['general']['watchdog_debug_row_limit'] = array(
    '#type' => 'select',
    '#title' => t('Database log messages to keep debug information for'),
    '#description' => t('The maximum number of stack traces to keep in the database log. Requires a <a href="@cron">cron maintenance task</a>.', array('@cron' => url('admin/reports/status'))),
    '#options' => drupal_map_assoc(array(100, 1000, 10000)),
    '#default_value' => variable_get('watchdog_debug_row_limit', WATCHDOG_DEBUG_DEFAULT_ROW_LIMIT),
    '#weight' => 5,
  );

  $collect_settings = variable_get('watchdog_debug_collect');
  foreach (_watchdog_debug_callbacks() as $collect_info_key => $collect_info_callback) {
    $options[$collect_info_key] = '';
    $form['general']['variable'][$collect_info_key] = array(
      '#type' => 'item',
      '#markup' => $collect_info_callback,
    );
    foreach ($severity_levels as $level_id => $level) {
      // Builds arrays for checked boxes for each role
      if (isset($collect_settings[$level_id][$collect_info_key]) && $collect_settings[$level_id][$collect_info_key]) {
        $status[$level][] = $collect_info_key;
      }
    }
  }
  foreach ($severity_levels as $level_id => $description) {
    $level = drupal_strtolower($description);
    $form['general']['checkboxes'][$level] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => isset($status[$level]) ? $status[$level] : array(),
    );
    $form['general']['severity_levels'][$level] = array('#markup' => check_plain($description), '#tree' => TRUE);
  }



  $form['backtrace'] = array(
    '#type' => 'fieldset',
    '#title' => t('debug_backtrace()'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['backtrace']['watchdog_debug_backtrace_hide_steps'] = array(
    '#type' => 'textfield',
    '#title' => t('Hide last <em>n</em> steps'),
    '#description' => t('Each backtrace includes steps leading to Watchdog entry save, which might be cut off the end of the backtrace array before saving it in the database.'),
    '#default_value' => variable_get('watchdog_debug_backtrace_hide_steps', NULL),
    '#size' => 5,
  );

  // We need our own theme function to put all checkboxes in the table.
  $form['#theme'] = 'watchdog_debug_admin_settings';
  $form['#submit'][] = 'watchdog_debug_admin_settings_submit';
  return system_settings_form($form);
}

/**
 * Themes admin settings form.
 */
function theme_watchdog_debug_admin_settings($variables) {
  $form = $variables['form'];

  $severity_levels = watchdog_severity_levels();
  foreach (element_children($form['general']['variable']) as $key) {
    $row = array();
    // Permission row.
    $row[] = array(
      'data' => drupal_render($form['general']['variable'][$key]),
      'class' => array('variable'),
    );
    foreach (element_children($form['general']['checkboxes']) as $level) {
      $form['general']['checkboxes'][$level][$key]['#title'] = $level . ' : ' . $form['general']['variable'][$key]['#markup'];
      $form['general']['checkboxes'][$level][$key]['#title_display'] = 'invisible';
      $row[] = array('data' => drupal_render($form['general']['checkboxes'][$level][$key]), 'class' => array('checkbox'));
    }
    $rows[] = $row;
  }
  $header = array('');
  foreach (element_children($form['general']['severity_levels']) as $level) {
    if (is_array($form['general']['severity_levels'][$level])) {
      $header[] = array('data' => drupal_render($form['general']['severity_levels'][$level]), 'class' => array('checkbox'));
    }
  }

  // Put the table with all checkboxes inside a fieldset.
  $markup = '<label>' . t('Severity levels to collect specific debug information for') . '</label>';
  $markup .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'permissions')));
  $form['general']['variables'] = array(
    '#type'   => 'markup',
    '#title'  => t('Severity levels'),
    '#markup' => $markup,
  );

  // We need our own theme function to put all checkboxes in the table.
  $form['#theme'] = 'watchdog_debug_admin_settings';
  // Render and return all remaining elements.
  return drupal_render_children($form);
}

/**
 * Saves additional debug info from submitted admin settings form.
 */
function watchdog_debug_admin_settings_submit($form, &$form_state) {
  $severity_levels = watchdog_severity_levels();
  $debug_info = array();
  foreach ($severity_levels as $level_id => $level) {
    foreach (array_keys(_watchdog_debug_callbacks()) as $variable_id) {
      $debug_info[$level_id][$variable_id] = ($form_state['values'][$level][$variable_id]) ? 1 : 0;
    }
    unset($form_state['values'][$level]);
  }
  variable_set('watchdog_debug_collect', $debug_info);
}

/**
 * Menu callback; displays details about a log message.
 */
function watchdog_debug_event($id) {
  module_load_include('inc', 'dblog', 'dblog.admin');

  $severity = watchdog_severity_levels();
  $result = db_query('SELECT w.*, wb.*, u.name, u.uid FROM {watchdog} w LEFT JOIN {watchdog_debug} wb USING(wid) INNER JOIN {users} u ON w.uid = u.uid WHERE w.wid = :id', array(':id' => $id))->fetchObject();

  if ($dblog = $result) {
    $rows = array(
      array(
        array('data' => t('Type'), 'header' => TRUE),
        t($dblog->type),
      ),
      array(
        array('data' => t('Date'), 'header' => TRUE),
        format_date($dblog->timestamp, 'long'),
      ),
      array(
        array('data' => t('User'), 'header' => TRUE),
        theme('username', array('account' => $dblog)),
      ),
      array(
        array('data' => t('Location'), 'header' => TRUE),
        l($dblog->location, $dblog->location),
      ),
      array(
        array('data' => t('Referrer'), 'header' => TRUE),
        l($dblog->referer, $dblog->referer),
      ),
      array(
        array('data' => t('Message'), 'header' => TRUE),
        theme('dblog_message', array('event' => $dblog)),
      ),
      array(
        array('data' => t('Severity'), 'header' => TRUE),
        $severity[$dblog->severity],
      ),
      array(
        array('data' => t('Hostname'), 'header' => TRUE),
        check_plain($dblog->hostname),
      ),
      array(
        array('data' => t('Operations'), 'header' => TRUE),
        $dblog->link,
      ),
    );
    $build['dblog_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#attributes' => array('class' => array('dblog-event watchdog-debug')),
    );


    if (!empty($dblog->backtrace)) {
      $backtrace = unserialize($dblog->backtrace);
      if (!empty($backtrace)) {

        // Remove top entries from each stack trace if required.
        $steps_to_hide = variable_get('watchdog_debug_hide_last_steps', NULL);
        if (!empty($steps_to_hide)) {
          $backtrace = array_slice($backtrace, $steps_to_hide);
        }

        $header = array(
          'Id',
          t('Function'),
          t('Location'),
        );
        $rows = array();
        $max = max(array_keys($backtrace));
        foreach ($backtrace as $id => $trace) {
          $rows[] = array(
            array('data' => '#' . ($max - $id), 'header' => TRUE),
            theme('watchdog_debug_function', array('trace' => $trace)),
            theme('watchdog_debug_location', array('trace' => $trace)),
          );
        }
        $build['backtrace_header'] = array(
          '#markup' => '<br /><h2>debug_backtrace()</h2>',
        );
        $build['backtrace_table'] = array(
          '#theme' => 'table',
          '#header' => $header,
          '#rows' => $rows,
          '#attributes' => array('class' => array('watchdog-debug watchdog-debug-backtrace')),
        );
      }
    }

    $header = array(t('Key'), t('Value'));
    foreach (_watchdog_debug_callbacks() as $callback_key => $callback) {
      if (!empty($dblog->{$callback_key}) && $callback_key != 'backtrace') {
        $rows = array();
        $callback_value = unserialize($dblog->{$callback_key});
        foreach ($callback_value as $key => $value) {
          $rows[] = array(
            array('data' => $key, 'header' => TRUE),
            $value,
          );
        }
        $build[$callback_key . '_header'] = array(
          '#markup' => '<br /><h2>' . $callback . '</h2>',
        );
        $build[$callback_key] = array(
          '#theme' => 'table',
          '#header' => $header,
          '#rows' => $rows,
          '#attributes' => array('class' => array('watchdog-debug watchdog-debug-' . $callback_key)),
        );
      }
    }

    drupal_add_css(drupal_get_path('module', 'watchdog_debug') . '/watchdog_debug.css');

    return $build;
  }
  else {
    return '';
  }
}
