<?php
/**
 * @file
 * JSONlog module helper functions.
 */


/**
 * Establish default site ID.
 *
 * Server's hostname + __ + (default) database name.
 * If default database also has a prefix: + __ + database prefix.
 *
 * @return string
 */
function jsonlog_default_site_id() {
  if (($site_id = getenv('drupal_jsonlog_siteid'))) {
    return $site_id;
  }

  // Server's hostname + database name + database prefix (if any).
  $db =& $GLOBALS['databases']['default']['default'];
  $site_id = drupal_strtolower(preg_replace('/[^\w\d\.\-_]/', '-', gethostname()))
    . '__' . $db['database']
    . (!$db['prefix'] ? '' : ('__' . $db['prefix']));
  unset($db); // Clear ref.

  return $site_id;
}

/**
 * Establish default logging dir.
 *
 * Directory default:
 * PHP:ini error_log - unless that is 'syslog' (then checks the usual suspects
 * /var/log/...) - plus /drupal-jsonlog.
 *
 * @return string
 *   Empty upon failure.
 */
function jsonlog_default_dir() {
  if (($dir = getenv('drupal_jsonlog_dir'))) {
    return $dir;
  }

  // Default web server log dir for common *nix distributions.
  $default_log_dirs = array(
    'debian' => '/var/log/apache2',
    'redhat' => '/var/log/httpd',
  );

  $dir = '';
  if (!($server_log = ini_get('error_log')) || $server_log === 'syslog') {
    // Try default web server log dirs for common *nix distributions.
    foreach ($default_log_dirs as $val) {
      if (file_exists($val)) {
        $dir = $val;
        break;
      }
    }
  }
  else {
    $dir = str_replace('\\', '/', dirname($server_log));
  }

  if ($dir) {
    return $dir . '/drupal-jsonlog';
  }
  return '';
}

/**
 * Checks if log dir+file exists and is writable, and logs to watchdog.
 *
 * Usable by drush script; prints messages via drush_log() instead
 * drupal_set_message() if in CLI mode.
 *
 * @see jsonlog_default_dir()
 *
 * @param string $file
 *   Default: empty (~ use default file location algo)
 *
 * @return boolean
 */
function jsonlog_test_filing($file = '') {
  $is_drush = drupal_is_cli();

  if ($file) {
    $restore_dir_setting = TRUE;

    $dir = dirname($file);
    if (!$dir) {
      if (!$is_drush) {
        drupal_set_message(t('jsonlog: Log directory name is empty.'), 'error');
      }
      else {
        drush_log(dt('jsonlog: Log directory name is empty.'), 'error');
      }
      return FALSE;
    }

    $file_time_at_all = preg_match('/.+\.\d{6,8}\.json\.log$/', $file);
  }
  else {
    $restore_dir_setting = FALSE;

    if (!($dir = getenv('drupal_jsonlog_dir'))) {
      if (!($dir = variable_get('jsonlog_dir'))) {
        if (!($dir = jsonlog_default_dir())) {
          if (!$is_drush) {
            drupal_set_message(t('jsonlog: Failed to establish the server\'s default logging directory.'), 'error');
          }
          else {
            drush_log(dt('jsonlog: Failed to establish the server\'s default logging directory.'), 'error');
          }
          return FALSE;
        }
      }
    }

    // Make file name.
    if (!($site_id = getenv('drupal_jsonlog_siteid'))) {
      if (!($site_id = variable_get('jsonlog_siteid'))) {
        $site_id = jsonlog_default_site_id();
      }
    }
    if (!($file_time = getenv('drupal_jsonlog_file_time'))) {
      $file_time = variable_get('jsonlog_file_time', 'Ymd');
    }
    $file = $dir . '/' . $site_id . ($file_time == 'none' ? '' : ('.' . date($file_time))) . '.json.log';

    $file_time_at_all = $file_time != 'none';
  }

  if (!file_exists($dir)) {
    if (!$is_drush) {
      drupal_set_message(
        t(
          'jsonlog: The log directory doesn\'t exist.!breakPlease create the directory \'!dir\'!breakor change the \'Log file\' setting.!breakAnd if using \'Timestamped log file name\' then the directory must be writable for the web server user.',
          array('!dir' => $dir, '!break' => '<br/>')
        ),
        'error'
      );
    }
    else {
      drush_log(
        dt(
          'jsonlog: The log directory doesn\'t exist.!breakPlease create the directory \'!dir\'!breakor change the \'Log file\' setting.!breakAnd if using \'Timestamped log file name\' then the directory must be writable for the web server user.',
          array('!dir' => $dir, '!break' => ', ')
        ),
        'error'
      );
    }
    return FALSE;
  }
  if (!file_exists($file)) {
    if (!touch($file)) {
      if (!$file_time_at_all) {
        if (!$is_drush) {
          drupal_set_message(
            t(
              'jsonlog: The log file doesn\'t exist.!breakPlease create the file \'!file\'!breakor change the \'Log directory\' setting.',
              array('!file' => $file, '!break' => '<br/>')
            ),
            'error'
          );
        }
        else {
          drush_log(
            dt(
              'jsonlog: The log file doesn\'t exist.!breakPlease create the file \'!file\'!breakor change the \'Log directory\' setting.',
              array('!file' => $file, '!break' => ', ')
            ),
            'error'
          );
        }
      }
      else {
        if (!$is_drush) {
          drupal_set_message(
            t(
              'jsonlog: When using \'Timestamped log file name\' then the log directory !dir must be writable (and executable) for the web server user.',
              array('!dir' => $dir, '!break' => '<br/>')
            ),
            'error'
          );
        }
        else {
          drush_log(
            dt(
              'jsonlog: When using \'Timestamped log file name\' then the log directory !dir must be writable (and executable) for the web server user.',
              array('!dir' => $dir, '!break' => ', ')
            ),
            'error'
          );
        }
      }
      return FALSE;
    }
  }
  elseif (!is_writable($file)) {
    if (!$file_time_at_all) {
      if (!$is_drush) {
        drupal_set_message(
          t(
            'jsonlog: The log file is not writable for the webserver user.!breakPlease check permissions of the file \'!file\'!breakand make sure that it\'s parent directory is executable for the webserver user.',
            array('!file' => $file, '!break' => '<br/>')
          ),
          'error'
        );
      }
      else {
        drush_log(
          dt(
            'jsonlog: The log file is not writable for the webserver user.!breakPlease check permissions of the file \'!file\'!breakand make sure that it\'s parent directory is executable for the webserver user.',
            array('!file' => $file, '!break' => ', ')
          ),
          'error'
        );
      }
    }
    else {
      if (!$is_drush) {
        drupal_set_message(
          t(
            'jsonlog: When using \'Timestamped log file name\' then the log directory !dir must be writable (and executable) for the web server user.',
            array('!dir' => $dir, '!break' => '<br/>')
          ),
          'error'
        );
      }
      else {
        drush_log(
          dt(
            'jsonlog: When using \'Timestamped log file name\' then the log directory !dir must be writable (and executable) for the web server user.',
            array('!dir' => $dir, '!break' => ', ')
          ),
          'error'
        );
      }
    }

    return FALSE;
  }

  // Temporarily set the conf var to test value
  // - otherwise jsonlog_watchdog() would still use the old value for this write
  $original_dir = NULL;
  if ($restore_dir_setting) {
    $original_dir = variable_get('jsonlog_dir', NULL);
    variable_set('jsonlog_dir', $dir);
  }

  try {
    watchdog(
      'jsonlog',
      'Testing watchdog logging - please check if this entry was written to file \'!file\'.',
      array('!file' => $file),
      ($threshold = getenv('drupal_jsonlog_severity_threshold')) ? $threshold : variable_get('jsonlog_severity_threshold', WATCHDOG_WARNING)
    );
  }
  catch (Exception $xc) {
    // Ignore; some watchdog implementation failed, and (probably) threw a
    // (database) PDOException.
  }

  // Restore original value.
  if ($restore_dir_setting) {
    if ($original_dir === NULL) {
      variable_del('jsonlog_dir');
    }
    else {
      variable_set('jsonlog_dir', $original_dir);
    }
  }

  if (!$is_drush) {
    drupal_set_message(
      t(
        'jsonlog: Logging to \'!file\' seems to work,!breakbut please check if that file now contains an entry whose message starts with \'Testing watchdog logging\'.',
        array('!file' => $file, '!break' => '<br/>')
      ),
      'warning'
    );
  }
  else {
    drush_log(
      dt(
        'jsonlog: Logging to \'!file\' seems to work,!breakbut please check if that file now contains an entry whose message starts with \'Testing watchdog logging\'.',
        array('!file' => $file, '!break' => ' ')
      ),
      'warning'
    );
  }

  return TRUE;
}

/**
 * Adds this module's setting fields to the system logging settings form.
 *
 * @see jsonlog_form_system_logging_settings_alter()
 *
 * @param array &$form
 * @param array &$form_state
 */
function _jsonlog_form_system_logging_settings_alter(&$form, &$form_state) {

  $form['#attributes']['autocomplete'] = 'off';

  drupal_add_css(
    drupal_get_path('module', 'jsonlog') . '/jsonlog.admin.css',
    array('type' => 'file', 'group' => CSS_DEFAULT, 'preprocess' => FALSE, 'every_page' => FALSE)
  );

  // Defaults.
  $siteid_default = jsonlog_default_site_id();
  if (!($dir_default = jsonlog_default_dir())) {
    drupal_set_message(t('Failed to establish the server\'s default logging directory.', array(), array('context' => 'module:jsonlog')), 'error');
  }

  // These translations are used a lot.
  $t_siteId = t('Site ID', array(), array('context' => 'module:jsonlog'));
  $t_overridden = t(' !sml¡overridden!!_sml', array('!sml' => '<small>', '!_sml' => '</small>'), array('context' => 'module:jsonlog'));

  // Using 'emergency' as threshold is simply not an option; because it's falsy
  // (expensive type checks, FALSE), and wouldn't make sense anyway.
  $severity_levels = watchdog_severity_levels();
  unset($severity_levels[0]);

  $form['jsonlog'] = array(
    '#type' => 'fieldset',
    '#title' => 'JSON Log',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['jsonlog']['description'] = array(
    '#type' => 'markup',
    '#markup' => '<p>'
      . t('Any server variable \'drupal_[jsonlog_varable_name]\' will override \'[jsonlog_varable_name]\' drupal conf. variable,!breakexcept for !emphtags!_emph (will be combined).', array('!break' => '<br/>', '!emph' => '<em>', '!_emph' => '</em>'), array('context' => 'module:jsonlog'))
      . '<br/>' . t('Tip: Server environment variables set in virtual host or .htaccess won\'t be visible by drush/CLI; /etc/environment might be your friend instead.', array('!break' => '<br/>'), array('context' => 'module:jsonlog'))
      . '</p>',
  );

  // Severity threshold.
  if (($severity_threshold = getenv('drupal_jsonlog_severity_threshold'))) {
    $overridden = TRUE;
  }
  else {
    $severity_threshold = variable_get('jsonlog_severity_threshold', WATCHDOG_WARNING);
    $overridden = FALSE;
  }
  $form['jsonlog']['jsonlog_severity_threshold'] = array(
    '#type' => 'select',
    '#title' => t(
      'Don\'t log events that are less severe than !sml[jsonlog_severity_threshold]!_sml!overridden',
      array('!overridden' => $overridden ? $t_overridden : '', '!sml' => '<small>', '!_sml' => '</small>'),
      array('context' => 'module:jsonlog')
    ),
    '#description' => t('Emergency is not an option.', array(), array('context' => 'module:jsonlog')),
    '#options' => $severity_levels,
    '#default_value' => $severity_threshold,
  );
  if ($overridden) {
    $form['jsonlog']['jsonlog_severity_threshold']['#attributes'] = array(
      'disabled' => 'disabled',
    );
  }

  // Truncation.
  if (($truncate = getenv('drupal_jsonlog_truncate')) !== FALSE) {
    $overridden = TRUE;
  }
  else {
    $truncate = variable_get('jsonlog_truncate', JSONLOG_TRUNCATE_DEFAULT);
    $overridden = FALSE;
  }
  $form['jsonlog']['jsonlog_truncate'] = array(
    '#type' => 'textfield',
    '#title' => t(
      'Truncate events to !sml[jsonlog_truncate]!_sml!overridden',
      array('!overridden' => $overridden ? $t_overridden : '', '!sml' => '<small>', '!_sml' => '</small>'),
      array('context' => 'module:jsonlog')
    ),
    '#description' => t(
      'Zero means no truncation.!breakLog entries longer than the file system\'s block size (typically 4 Kb) should not result in garbled logs (due to concurrent file writes), because the logger uses write locking.!breakDefaults to !default (Kb).',
      array(
        '!default' => JSONLOG_TRUNCATE_DEFAULT,
        '!break' => '<br/>',
      ),
      array('context' => 'module:jsonlog')
    ),
    '#default_value' => $truncate,
    '#size' => 5,
    '#field_suffix' => t('Kb', array(), array('context' => 'module:jsonlog')),
  );
  if ($overridden) {
    $form['jsonlog']['jsonlog_truncate']['#attributes'] = array(
      'disabled' => 'disabled',
    );
  }

  // Site ID.
  if (($site_id = getenv('drupal_jsonlog_siteid'))) {
    $overridden = TRUE;
  }
  else {
    if (!($site_id = variable_get('jsonlog_siteid'))) {
      $site_id = $siteid_default;
    }
    $overridden = FALSE;
  }
  $form['jsonlog']['jsonlog_siteid'] = array(
    '#type' => 'textfield',
    '#title' => t(
      'Site ID !sml[jsonlog_siteid]!_sml!overridden',
      array('!overridden' => $overridden ? $t_overridden : '', '!sml' => '<small>', '!_sml' => '</small>'),
      array('context' => 'module:jsonlog')
    ),
    '#description' => t(
      'Spaces and quotes get replaced by hyphens.!breakDefaults to the server\'s hostname and the site\'s database name and prefix (if any):!break!default',
      array(
        '!default' => '&nbsp; ' . $siteid_default,
        '!break' => '<br/>',
      ),
      array('context' => 'module:jsonlog')
    ),
    '#required' => TRUE,
    '#default_value' => $site_id,
  );
  if ($overridden) {
    $form['jsonlog']['jsonlog_siteid']['#attributes'] = array(
      'disabled' => 'disabled',
    );
  }

  // Canonical site identifier across site instances.
  if (($canonical = getenv('drupal_jsonlog_canonical'))) {
    $overridden = TRUE;
  }
  else {
    $canonical = variable_get('jsonlog_canonical', '');
    $overridden = FALSE;
  }
  $form['jsonlog']['jsonlog_canonical'] = array(
    '#type' => 'textfield',
    '#title' => t(
      'Canonical site name !sml[jsonlog_canonical]!_sml!overridden',
      array('!overridden' => $overridden ? $t_overridden : '', '!sml' => '<small>', '!_sml' => '</small>'),
      array('context' => 'module:jsonlog')
    ),
    '#description' => t(
      'For simpler identification across load balanced site instances.',
      array(),
      array('context' => 'module:jsonlog')
    ),
    '#required' => FALSE,
    '#default_value' => $canonical,
  );
  if ($overridden) {
    $form['jsonlog']['jsonlog_canonical']['#attributes'] = array(
      'disabled' => 'disabled',
    );
  }

  // Timestamped file; default 'Ymd',
  // values: 'Ymd' (~ YYYYMMDD)|'YW' (~ YYYYWW)|'Ym' (~ YYYYMM)|'none'.
  if (($file_time = getenv('drupal_jsonlog_file_time'))) {
    $overridden = TRUE;
  }
  else {
    if (!($file_time = variable_get('jsonlog_file_time'))) {
      $file_time = 'Ymd';
    }
    $overridden = FALSE;
  }
  $form['jsonlog']['jsonlog_file_time'] = array(
    '#type' => 'select',
    '#title' => t(
      'Timestamped log file name !sml[jsonlog_file_time]!_sml!overridden',
      array('!overridden' => $overridden ? $t_overridden : '', '!sml' => '<small>', '!_sml' => '</small>'),
      array('context' => 'module:jsonlog')
    ),
    '#description' => t(
      'Default: Day (\'Ymd\' ~ YYYYMMDD)',
      array(),
      array('context' => 'module:jsonlog')
    ),
    '#options' => array(
      // Deliberaterately not '_none'.
      'none' => t('None - use the same file forever', array(), array('context' => 'module:jsonlog')),
      'Ymd' => t('Day (\'Ymd\' ~ YYYYMMDD)', array(), array('context' => 'module:jsonlog')),
      'YW' => t('Week (\'YW\' ~ YYYYWW)', array(), array('context' => 'module:jsonlog')),
      'Ym' => t('Month (\'Ym\' ~ YYYYMM)', array(), array('context' => 'module:jsonlog')),
    ),
    '#default_value' => $file_time,
  );
  if ($overridden) {
    $form['jsonlog']['jsonlog_file_time']['#attributes'] = array(
      'disabled' => 'disabled',
    );
  }

  // Dir.
  if (($dir = getenv('drupal_jsonlog_dir'))) {
    $overridden = TRUE;
  }
  else {
    if (!($dir = variable_get('jsonlog_dir'))) {
      $dir = $dir_default;
    }
    $overridden = FALSE;
  }
  $form['jsonlog']['jsonlog_dir'] = array(
    '#type' => 'textfield',
    '#title' => t(
      'Log directory !sml[jsonlog_dir]!_sml!overridden',
      array('!overridden' => $overridden ? $t_overridden : '', '!sml' => '<small>', '!_sml' => '</small>'),
      array('context' => 'module:jsonlog')
    ),
    '#description' => t(
      'Defaults to PHP ini \'error_log\' path + /drupal-jsonlog:!break!default!breakNB: The web server user (www-data|apache) probably isn\'t allowed to write to a file in the \'error_log\' path!break- create a sub dir (like \'drupal-jsonlog\') as root user, and do a chown or chmod on that sub dir to make it writable (and executable) by the web server user.',
      array(
        '!default' => '&nbsp; ' . $dir_default,
        '!break' => '<br/>',
      ),
      array('context' => 'module:jsonlog')
    ),
    '#required' => FALSE,
    '#default_value' => $dir,
    '#size' => 80,
    '#field_suffix' => '/' . $site_id . ($file_time == 'none' ? '' : ('.' . date($file_time))) . '.json.log',
  );
  if ($overridden) {
    $form['jsonlog']['jsonlog_dir']['#attributes'] = array(
      'disabled' => 'disabled',
    );
  }

  // Test logging.
  $form['jsonlog']['jsonlog_test_filing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test filing a JSONlog entry', array(), array('context' => 'module:jsonlog')),
    '#description' => t('You\'ll be warned and guided along if it doesn\'t work correctly.', array(), array('context' => 'module:jsonlog')),
    '#default_value' => 0,
    '#title_display' => 'before',
  );

  // Tags.
  $tags_server = ($tags = getenv('drupal_jsonlog_tags')) !== FALSE ? $tags : '';
  $form['jsonlog']['jsonlog_tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags !sml[jsonlog_tags]!_sml', array('!sml' => '<small>', '!_sml' => '</small>'), array('context' => 'module:jsonlog')),
    '#description' => t(
      'Comma-separated list of tags.!breakTags set by server environment variable will be combined with tags set here.',
      array('!break' => '<br/>',),
      array('context' => 'module:jsonlog')
    ),
    '#default_value' => $tags_site = variable_get('jsonlog_tags', ''),
    '#size' => 100,
    '#field_prefix' => $tags_server !== '' ? ($tags_server . ', ') : '',
  );


  // Make table view of a log entry.

  // ISO 8601 timestamp.
  $millis = round(microtime(TRUE) * 1000);
  $seconds = (int) floor($millis / 1000);
  $millis -= $seconds * 1000;
  $millis = str_pad($millis, 3, '0', STR_PAD_LEFT);
  $timestamp = substr(gmdate('c', $seconds), 0, 19) . '.' . $millis . 'Z';

  if ($tags_server) {
    $tags = $tags_server;
    if ($tags_site) {
      $tags .= ',' . $tags_site;
    }
  }
  else {
    $tags = $tags_site;
  }
  if ($tags) {
    $tags = '[\'' . join('\',\'', explode(',', $tags)) . '\']';
  }
  else {
    $tags = 'null';
  }

  // JSON fields.
  $json_fields = array(
    'message' => array(
      'label' => t('Message', array(), array('context' => 'module:jsonlog')),
      'value' => t('Some message', array(), array('context' => 'module:jsonlog')),
    ),
    'timestamp' => array(
      'label' => t('Timestamp', array(), array('context' => 'module:jsonlog')),
      'name' => '@timestamp',
      'value' => $timestamp,
    ),
    'version' => array(
      'custom' => TRUE,
      'name' => '@version',
      'label' => t('Version', array(), array('context' => 'module:jsonlog')),
      'value' => 1,
    ),
    'message_id' => array(
      'custom' => TRUE,
      'name' => 'message_id',
      'label' => t(
        'Message ID (!site_id + unique ID)',
        array('!site_id' => $t_siteId),
        array('context' => 'module:jsonlog')
      ),
      'value' => uniqid($site_id, TRUE),
    ),
    'site_id' => array(
      'custom' => TRUE,
      'label' => $t_siteId,
      'value' => $site_id,
    ),
    'canonical' => array(
      'custom' => TRUE,
      'label' => t('Canonical', array(), array('context' => 'module:jsonlog')),
      'value' => $canonical,
    ),
    'tags' => array(
      'custom' => TRUE,
      'name' => 'tags',
      'label' => t('Tags (null or array of strings)', array(), array('context' => 'module:jsonlog')),
      'value' => $tags,
    ),
    'type' => array(
      'label' => t('Type', array(), array('context' => 'module:jsonlog')),
      'value' => 'drupal',
    ),
    'subtype' => array(
      'label' => t('Subtype (watchdog \'type\')', array(), array('context' => 'module:jsonlog')),
      'value' => 'some_module_name',
    ),
    'severity' => array(
      'label' => t('Severity', array(), array('context' => 'module:jsonlog')),
      'value' => $severity_threshold,
    ),
    'method' => array(
      'custom' => TRUE,
      'name' => 'method',
      'label' => t('Request method (GET, POST, cli)', array(), array('context' => 'module:jsonlog')),
      'value' => $_SERVER['REQUEST_METHOD'],
    ),
    'request_uri' => array(
      'label' => t('Request URI', array(), array('context' => 'module:jsonlog')),
      'value' => $GLOBALS['base_root'] . request_uri(),
    ),
    'referer' => array(
      'label' => t('Referer', array(), array('context' => 'module:jsonlog')),
      'value' => isset($_SERVER['HTTP_REFERER']) ? check_plain($_SERVER['HTTP_REFERER']) : '',
    ),
    'uid' => array(
      'label' => t('User ID', array(), array('context' => 'module:jsonlog')),
      'value' => $uid = $GLOBALS['user']->uid,
    ),
    'username' => array(
      'custom' => TRUE,
      'name' => 'username',
      'label' => t('Username (when not anonymous user)', array(), array('context' => 'module:jsonlog')),
      'value' => $uid && !empty($GLOBALS['user']->name) ? $GLOBALS['user']->name : '',
    ),
    'ip' => array(
      'name' => 'client_ip',
      'label' => t('User\'s I.P. address', array(), array('context' => 'module:jsonlog')),
      'value' => ip_address(),
    ),
    'link' => array(
      'label' => t('Link (URL path or NULL)', array(), array('context' => 'module:jsonlog')),
      'value' => NULL,
    ),
    'code' => array(
      'custom' => TRUE,
      'label' => t('Code (positive integer if watchdog \'link\' is N or \'N\')', array(), array('context' => 'module:jsonlog')),
      'value' => 0,
    ),
    'variables' => array(
      'label' => t('Variables (always null, variables get replaced into message)', array(), array('context' => 'module:jsonlog')),
      'value' => NULL,
    ),
    'truncation' => array(
      'custom' => TRUE,
      'name' => 'trunc',
      'label' => t('Truncation (null, or array [original message length, truncated message length])', array(), array('context' => 'module:jsonlog')),
      'value' => NULL,
    ),
  );

  $table_header = array(
    t('Property', array(), array('context' => 'module:jsonlog')),
    t('JSON field name', array(), array('context' => 'module:jsonlog')),
    t('Example', array(), array('context' => 'module:jsonlog')),
  );
  $table_rows = array();

  foreach ($json_fields as $name => $props) {
    $table_rows[] = array(
      'data' => array(
        $props['label'],
        !empty($props['name']) ? $props['name'] : $name,
        $props['value'] === NULL ? 'null' : $props['value'],
      ),
    );
  }

  $form['jsonlog']['example'] = array(
    '#type' => 'markup',
    '#markup' => '<label>'
      . t('JSONlog entry example', array(), array('context' => 'module:jsonlog')) . '</label>'
      . theme_table(array(
          'header' => $table_header,
          'rows' => $table_rows,
          'attributes' => array(
            'class' => array(
              'jsonlog-entry-example'
            ),
          ),
          'caption' => '',
          'colgroups' => array(),
          'sticky' => TRUE,
          'empty' => '',
        )
      ),
  );

  $form['#validate'][] = 'jsonlog_form_system_logging_settings_validate';

  // Prepend our submit handler; we need to get in first, otherwise our changes
  // to form values amounts to nothing.
  array_unshift($form['#submit'], 'jsonlog_form_system_logging_settings_submit');

  return system_settings_form($form);
}
