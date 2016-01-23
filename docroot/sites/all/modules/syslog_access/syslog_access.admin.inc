<?php

/**
 * Admin hooks and functions for the Syslog Access module.
 */


/**
 * Adds Syslog Access settings to the Statistics setting form.
 *
 * @see syslog_access_form_statistics_settings_form_alter()
 */
function _syslog_access_form_statistics_settings_form_alter(&$form, &$form_state) {
  $help = module_exists('help') ? ' ' . l(t('More information'), 'admin/help/syslog_access') . '.' : NULL;

  // Give the option to disable database logging.
  $form['access']['statistics_log_to_db'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable database logging'),
    '#description' => t('Uncheck this to log access statistics only via syslog.'),
    '#default_value' => variable_get('statistics_log_to_db', TRUE),
  );

  $form['access_log_syslog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access log syslog'),
    '#description' => t('Configure how and where access statistics are logged with syslog.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -5,
  );

  $form['access_log_syslog']['syslog_access_identity'] = array(
    '#type' => 'textfield',
    '#title' => t('Syslog identity'),
    '#default_value' => variable_get('syslog_access_identity', 'drupal-access'),
    '#description' => t('A string that will be prepended to every message logged to Syslog. If you have multiple sites logging to the same Syslog log file, a unique identity per site makes it easy to tell the log entries apart.') . $help,
  );

  if (defined('LOG_LOCAL0')) {
    $form['access_log_syslog']['syslog_access_facility'] = array(
      '#type' => 'select',
      '#title' => t('Syslog facility'),
      '#default_value' => variable_get('syslog_access_facility', LOG_LOCAL0),
      '#options' => _syslog_access_facility_list(),
      '#description' => t('Depending on the system configuration, Syslog and other logging tools use this code to identify or filter messages from within the entire system log.') . $help,
     );
  }

  $form['access_log_syslog']['syslog_access_format'] = array(
    '#type' => 'textarea',
    '#title' => t('Syslog format'),
    '#default_value' => variable_get('syslog_access_format', '!timestamp|!hostname|!path|!title|!url|!uid|!sid|!timer'),
    '#description' => t('Specify the format of the syslog entry. Available variables are: !variables', array('!variables' => _syslog_access_available_variables())),
  );
}


/**
 * Help text for the Syslog Access module.
 *
 * @see syslog_access_help()
 *
 * @return
 *   Help markup.
 */
function _syslog_access_help() {
  $output = '';
  $output .= '<h3>' . t('About') . '</h3>';
  $output .= '<p>' . t("The Syslog Access module logs access statistics by sending messages to the logging facility of your web server's operating system. Syslog is an operating system administrative logging tool that provides valuable information for use in system management and security auditing. Most suited to medium and large sites, Syslog provides filtering tools that allow messages to be routed by type and severity. For more information, see PHP's <a href='@php_openlog'>openlog</a> and <a href='@php_syslog'>syslog</a> functions.", array('@php_openlog' => 'http://www.php.net/manual/function.openlog.php', '@php_syslog' => 'http://www.php.net/manual/function.syslog.php')) . '</p>';
  $output .= '<h3>' . t('Uses') . '</h3>';
  $output .= '<dl>';
  $output .= '<dt>' . t('Logging for UNIX, Linux, and Mac OS X') . '</dt>';
  $output .= '<dd>' . t('On UNIX, Linux, and Mac OS X, the file <em>/etc/syslog.conf</em> defines the routing configuration. Messages can be flagged with the codes <code>LOG_LOCAL0</code> through <code>LOG_LOCAL7</code>. For information on Syslog facilities, severity levels, and how to set up <em>syslog.conf</em>, see the <em>syslog.conf</em> manual page on your command line.') . '</dd>';
  $output .= '<dt>' . t('Logging for Microsoft Windows') . '</dt>';
  $output .= '<dd>' . t('On Microsoft Windows, messages are always sent to the Event Log using the code <code>LOG_USER</code>.') . '</dd>';
  $output .= '</dl>';

  return $output;
}


/**
 * Lists all possible syslog facilities for UNIX/Linux.
 *
 * @return array
 */
function _syslog_access_facility_list() {
  return array(
    LOG_LOCAL0 => 'LOG_LOCAL0',
    LOG_LOCAL1 => 'LOG_LOCAL1',
    LOG_LOCAL2 => 'LOG_LOCAL2',
    LOG_LOCAL3 => 'LOG_LOCAL3',
    LOG_LOCAL4 => 'LOG_LOCAL4',
    LOG_LOCAL5 => 'LOG_LOCAL5',
    LOG_LOCAL6 => 'LOG_LOCAL6',
    LOG_LOCAL7 => 'LOG_LOCAL7',
  );
}


/**
 * Builds a list of available variables for use in the syslog access format.
 *
 * @return
 *   An HTML string of definitions.
 */
function _syslog_access_available_variables() {
  $fields = variable_get('better_statistics_fields', better_statistics_get_default_fields());
  unset($fields['aid']);
  $output = '<dl>';

  foreach ($fields as $field => $data) {
    $output .= '<dt><code>!' . $field . '</code></dt>';
    $output .= '<dd>' . t((isset($data['views_field']['help']) ? $data['views_field']['help'] : $data['schema']['description'])) . '</dd>';
  }

  $output .= '</dl>';
  return $output;
}
