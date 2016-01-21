<?php

/**
 * Configure entity visits settings.
 */
function redislog_admin_settings() {
  $form = array();

  $options = array(
    60 * 15 => t('15 minutes'),
    60 * 30 => t('30 minutes'),
    60 * 60 => t('1 hour'),
    60 * 60 * 6 => t('6 hours'),
    60 * 60 * 24 => t('1 day'),
    60 * 60 * 24 * 7  => t('7 days'),
    60 * 60 * 24 * 28 => t('28 days'),
  );

  $form['redislog_ttl'] = array(
    '#type' => 'select',
    '#title' => t('Default TTL'),
    '#options' => $options,
    '#default_value' => variable_get('redislog_ttl', 60 * 60 * 24),
    '#description' => t('Set a timeout on log message. After the timeout has expired, the message will automatically be deleted.'),
  );

  foreach (redislog_get_types() as $type) {
    $form['type']['redislog_ttl_' . $type] = array(
      '#type' => 'select',
      '#title' => $type,
      '#options' => $options,
      '#default_value' => variable_get('redislog_ttl_' . $type, 60 * 60 * 24),
    );
  }

  return system_settings_form($form);
}

/**
 * Menu callback; displays a listing of log messages.
 *
 * Messages are truncated at 56 chars. Full-length message could be viewed at
 * the message details page.
 */
function redislog_overview() {
  $form = array('#attributes' => array('ng-controller' => 'redisLogCtrl'));

  $form['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear log messages'),
  );

  $form['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#attributes' => array(
      'ng-model' => 'search',
    ),
  );

  $form['redislog_table'] = array(
    '#theme' => 'table',
    '#header' => array(t('Type'), t('Date'), t('Message'), t('User'), t('Operations')),
    '#rows' => array(
      array(
        'ng-repeat' => "log in logs | orderBy:'timestamp':true | filter: search",
        'data' => array(
          "{{log.type}}",
          "{{log.timestamp | date:'medium'}}",
          "{{log.message}}",
          "{{log.user}}",
          "<a href='{{log.url}}'>View</a>",
        ),
        'ng-class' => "{
          info: log.severity === 'info',
          ok: log.severity === 'notice',
          warning: ['debug', 'warning'].indexOf(log.severity) !== -1,
          error: ['error', 'critical', 'alert', 'emerg'].indexOf(log.severity) !== -1,
        }"
      )
    ),
  );

  $logs = array();
  foreach (redislog_get_multiple() as $id => $log) {
    $message = t($log['message'], $log['variables'] ?: array());

    $logs[] = array(
      'type' => t($log['type']),
      'severity' => _redislog_get_severity($log['severity']),
      'timestamp' => $log['timestamp'] . '000', // Add milliseconds.
      'message' => truncate_utf8(filter_xss($message, array()), 56, TRUE, TRUE),
      'user' => isset($log['user']->name) && $log['user']->name
        ? t('!name (!uid)', array('!name' => $log['user']->name, '!uid' => $log['user']->uid))
        : t('Anonymous (not verified)'),
      'url' => url('admin/reports/redislog/event/' . $log['key'] . '/' . $log['id'], array('absolute' => TRUE))
    );
  }

  angularjs_init_application('redisLog');
  drupal_add_js(array('redislog' => array('logs' => $logs)), 'setting');
  drupal_add_js(drupal_get_path('module', 'redislog') . '/js/redislog.js');

  return $form;
}

/**
 * Get severity of the message.
 *
 * @param int $severity
 *
 * @return string
 */
function _redislog_get_severity($severity) {
  $classes = array(
    WATCHDOG_DEBUG     => 'debug',
    WATCHDOG_INFO      => 'info',
    WATCHDOG_NOTICE    => 'notice',
    WATCHDOG_WARNING   => 'warning',
    WATCHDOG_ERROR     => 'error',
    WATCHDOG_CRITICAL  => 'critical',
    WATCHDOG_ALERT     => 'alert',
    WATCHDOG_EMERGENCY => 'emerg',
  );

  return $classes[$severity];
}

/**
 * Menu callback; displays details about a log message.
 */
function redislog_event($key, $id) {
  $redis = Redis_Client::getClient();

  if ($log = $redis->lGet($key, $id)) {
    $severity = watchdog_severity_levels();
    $log = unserialize($log);

    $rows = array(
      array(
        array('data' => t('Type'), 'header' => TRUE),
        t($log['type']),
      ),
      array(
        array('data' => t('Date'), 'header' => TRUE),
        format_date($log['timestamp'], 'long'),
      ),
      array(
        array('data' => t('User'), 'header' => TRUE),
        theme('username', array('account' => $log['user'])),
      ),
      array(
        array('data' => t('Location'), 'header' => TRUE),
        l($log['location'], $log['location']),
      ),
      array(
        array('data' => t('Referrer'), 'header' => TRUE),
        l($log['referer'], $log['referer']),
      ),
      array(
        array('data' => t('Message'), 'header' => TRUE),
        filter_xss(t($log['message'], $log['variables'] ?: array())),
      ),
      array(
        array('data' => t('Severity'), 'header' => TRUE),
        $severity[$log['severity']],
      ),
      array(
        array('data' => t('Hostname'), 'header' => TRUE),
        check_plain($log['hostname']),
      ),
      array(
        array('data' => t('Operations'), 'header' => TRUE),
        $log['link'],
      ),
    );

    $build['dblog_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#attributes' => array('class' => array('dblog-event')),
    );

    return $build;
  }
  else {
    return '';
  }
}

/**
 * Form submission handler for redislog_overview_form().
 */
function redislog_overview_submit() {
  $redis = Redis_Client::getClient();
  $redis->delete($redis->keys(REDISLOG_KEY . ':*'));

  drupal_set_message(t('Redis log cleared.'));
}
