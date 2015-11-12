<?php
/**
 * @file
 * Administration of the limesurvey_sync module.
 */

/**
 * Form builder; administer limesurvey_sync module options.
 *
 * Set options for the LimeSurvey synchronization and display the testing
 * interface to check if parameters are valid, on the
 * 'admin/config/media/ls_sync' page.
 *
 * @see limesurvey_sync_survey_admin_form_validate()
 * @see limesurvey_sync_survey_admin_form_submit()
 * @ingroup forms
 */
function limesurvey_sync_survey_admin_form($form, $form_state) {
  // Disable the overlay
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
  limesurvey_sync_disable_overlay();

  // Load default or actual settings.
  $default_ls_settings = array(
    'ls_path' => "http://{$_SERVER['HTTP_HOST']}/limesurvey",
    'ls_delta_time' => 0,
    'ls_delta_time_was_checked' => FALSE,
  );
  $settings =  variable_get('ls_settings', $default_ls_settings);

  // Use the provided values for the test interface.
  if (!empty($form_state['values'])) {
    // I'm not sure values have been sanitized throught the form process.
    $settings = array_map('check_plain', array_intersect_key($form_state['values'], $settings));
  }

  $intro = '<p>' . t('The LimeSurvey Sync module allows to connect the <strong><a href="@url_ls">LimeSurvey software</a></strong> (a free and open source Survey application) and the Drupal software together.', array('@url_ls' => 'http://www.limesurvey.org'));
  $urls = array(
    '@url_download' => 'http://www.limesurvey.org/en/download',
    '@url_install' => 'http://manual.limesurvey.org/Installation',
  );
  $intro .= '<br /><strong>' . t('In order to use this module, you have to <a href="@url_download">download</a> and <a href="@url_install">install</a> the LimeSurvey software on a server (in a distinct folder than the Drupal one).', $urls) . '</strong>';
  $intro .= '<br />' . t('Once installed your LimeSurvey site, the table below helps to correctly configure the synchronization process.');
  $intro .= '</p>';
  $form['intro'] = array(
    '#type' => 'markup',
    '#markup' => $intro,
    '#weight' => -4,
  );

  // Load LimeSurvey synchronization input settings.
  $form['limesurvey_sync'] = array(
    '#type' => 'fieldset',
    '#title' => t('Testing interface'),
    '#description' => t('Show testing results for synchronization between your Drupal site and your LimeSurvey site.')
    . '<br /><strong>' . t('Any test should fail.') . '</strong>',
  );

  // Load display tabs.
  $test_result = limesurvey_sync_test_interface($settings);
  $url_ok = FALSE;
  // Create the $row variable to display test page as a html table.
  foreach ($test_result as $property => $datas) {
    $warning_message = ($datas['result'] === NULL) ? '<div class ="description">' . t('Simple warning message, your setting may be correct.') . '</div>' : '';
    $form['limesurvey_sync']['display'][$property]['title'] =  array(
      '#type' => 'markup',
      '#markup' => $datas['title'],
    );
    $form['limesurvey_sync']['display'][$property]['result'] = array(
      '#type' => 'markup',
      '#markup' => limesurvey_sync_display_boolean_img($datas['result']) . $warning_message,
    );
    $form['limesurvey_sync']['display'][$property]['message'] = array(
      '#type' => 'markup',
      '#markup' => $datas['message'],
    );
    $form['limesurvey_sync']['display'][$property]['help'] = array(
      '#type' => 'markup',
      '#markup' => $datas['help'],
    );
    if ($property == 'ls_path') {
      // Store this result for the ls_path edit form.
      $url_ok = $datas['result'];
    }
  }

  // Load LimeSurvey synchronization input settings.
  $form['limesurvey_sync']['limesurvey_sync_sync'] = array(
    '#type' => 'container',
    //'#title' => t('LimeSurvey synchronization settings'),
  );

  // Part for rewriting the settings.php file.
  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_server'] = limesurvey_sync_ls_server_form($form, $form_state);

  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_path'] = array(
    '#type' => 'textfield',
    '#title' => t('LimeSurvey site path'),
    '#default_value' => $settings['ls_path'],
    '#description' => t('It can be a relative path or an external website'),
    '#size' => (empty($settings['ls_path'])) ? 30 : (drupal_strlen($settings['ls_path']) + 10),
  );

  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $ls_gset_text = t('LimeSurvey global setting page');
  $link_ls_time = ($url_ok) ? limesurvey_sync_link_to_ls($ls_gset_text, NULL, 'globalsettings', 'general') : $ls_gset_text;
  $ls_gset_text_lsserver = t('LimeSurvey Server time');
  $link_ls_time_lsserver = ($url_ok) ? limesurvey_sync_link_to_ls($ls_gset_text_lsserver, NULL, 'globalsettings', 'general') : $ls_gset_text_lsserver;
  $windows_tag = array(
    'attributes' => array(
      'title' => t('Open the link into a new window'),
      'onclick' => 'window.open(this.href); return false;',
    ),
  );

  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_time'] =  array(
    '#type' => 'container',
  );

  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_time']['ls_delta_time'] = array(
    '#type' => 'value',
    '#value' => $settings['ls_delta_time'],
  );

  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_delta_time_was_checked'] = array(
    '#type' => 'value',
    '#value' => $settings['ls_delta_time_was_checked'],
  );

  $distinct_storage = limesurvey_sync_distinct_storage();
  $use_same_server = is_array($distinct_storage) && !in_array('host', $distinct_storage) && !in_array('port', $distinct_storage);

  // Retrieve the original date_default_timezone_get() value before Drupal
  // set it.
  $uncheckable_tz = FALSE;
  $prior_php54 = (version_compare(PHP_VERSION, '5.4.0', '<'));
  if ($prior_php54 && !empty($_SERVER['TZ'])) {
    $tz_dr_server = $_SERVER['TZ'];
  }
  elseif (ini_get('date.timezone')) {
    $tz_dr_server = ini_get('date.timezone');
  }
  elseif ($prior_php54) {
    // Querying the host operating system, if supported and allowed by the OS.
    // The value can not be retrieve. Set it arbitrarily to UTC.
    $tz_dr_server = 'UTC';
    $uncheckable_tz = TRUE;
  }
  else {
    $tz_dr_server = 'UTC';
  }
  // Add human check.
  $time_need_human_check = (!($use_same_server && !$uncheckable_tz));

  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_time_need_human_check'] = array(
    '#type' => 'value',
    '#value' => $time_need_human_check,
  );

  $drupal_timezone = variable_get('date_default_timezone', @date_default_timezone_get());
  // The date() php function uses the user timezone in order to determine the
  // timezone.
  $user_timezone = drupal_get_user_timezone();
  $delta_tz = ($drupal_timezone == $user_timezone) ? 0 : limesurvey_sync_get_timezone_offset($drupal_timezone, $user_timezone);
  $delta_drupal_server = ($drupal_timezone == $tz_dr_server) ? 0 : limesurvey_sync_get_timezone_offset($tz_dr_server, $drupal_timezone);
  $ls_server_time = array();
  for ($i = -24; $i <= 24; $i++) {
    $delta_time_seconds = $i * 3600;
    $ls_server_time[$delta_time_seconds] = format_date((REQUEST_TIME + $delta_tz + $delta_drupal_server + $delta_time_seconds));
  }
  $ls_server_time['custom'] = t('Custom');
  $display_ls_time_defaut = format_date((REQUEST_TIME + $delta_tz + $delta_drupal_server + $settings['ls_delta_time']));
  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_time']['ls_delta_time_check'] = array(
    '#type' => 'radios',
    '#title' => t('The !link_ls_time value is %value', array(
      '!link_ls_time' => $link_ls_time_lsserver,
      '%value' => $display_ls_time_defaut,
    )),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
    //'#required' => TRUE,
    '#default_value' => ($time_need_human_check)  ? '' : 'yes',
    '#description' => t('Visit the !link_ls_settings, looking for the LimeSurvey <em>Server time</em> value.', array(
      '!link_ls_settings' => $link_ls_time,
    )),
  );

  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_time']['ls_delta_time_list'] = array(
    '#type' => 'select',
    '#title' => t('Select the right LimeSurvey <em>Server time</em> value and submit the page'),
    '#options' => $ls_server_time,
    '#default_value' => $settings['ls_delta_time'],
    '#states' => array('visible' => array(':input[name="ls_delta_time_check"]' => array('value' => 'no'))),
  );

  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_time']['ls_delta_time_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Tip time between the Drupal server and the LimeSurvey server'),
    '#options' => $ls_server_time,
    '#default_value' => $settings['ls_delta_time'],
    '#field_suffix' => format_plural((($settings['ls_delta_time'] == '0') ? 1 : $settings['ls_delta_time']) , 'second', 'seconds'),
    '#size' => (drupal_strlen($settings['ls_delta_time']) + 2),
    '#states' => array('visible' => array(
        ':input[name="ls_delta_time_list"]' => array('value' => 'custom'),
        ':input[name="ls_delta_time_check"]' => array('value' => 'no'),
      )),
  );

  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_time']['ls_timezone'] =  array(
    '#type' => 'container',
    '#states' => array('visible' => array(':input[name="ls_delta_time_check"]' => array('value' => 'yes'))),
  );
  $form['limesurvey_sync']['limesurvey_sync_sync']['ls_time']['ls_timezone']['ls_timezone_text'] =  array(
    '#type' => 'markup',
    '#markup' => t('The LimeSurvey <em>Time difference</em> value can be edited on the !link_ls_tz', array(
      '!link_ls_tz' => $link_ls_time,
    )),
  );

  // Load buttons.
  $form['actions'] = array('#type' => 'actions');

  $form['actions']['test_button'] = array(
    '#type' => 'button',
    '#value' => t("Test settings (don't save)"),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['limesurvey_sync']['#theme'] = 'limesurvey_sync_sync_admin_testing';

  return $form;
}

/**
 * Validate limesurvey_sync module options.
 */
function limesurvey_sync_survey_admin_form_validate($form, &$form_state) {

  // Retrieve the correct delta time value.
  if (isset($form_state['values']['ls_delta_time_check']) && $form_state['values']['ls_delta_time_check'] == 'no') {
    // Overwrite the ls_delta_time value.
    $form_state['values']['ls_delta_time'] = ($form_state['values']['ls_delta_time_list'] != 'custom') ? $form_state['values']['ls_delta_time_list'] : $form_state['values']['ls_delta_time_custom'];
  }

  // Fix the ls_deta_time_was_checked value.
  if (isset($form_state['values']['ls_time_need_human_check']) && $form_state['values']['ls_time_need_human_check']) {
    if (!empty($form_state['values']['ls_delta_time_check'])) {
      $form_state['values']['ls_delta_time_was_checked'] = REQUEST_TIME;
    }
  }

  // Validate the ls_path value.
  $error = FALSE;
  if (!isset($form_state['values']['ls_path']) || !is_string($form_state['values']['ls_path'])) {
    form_set_error('ls_path', t("The '%setting' setting is not valid.", array('%setting' => $form['ls_survey']['ls_path']['#title'])));
    $error = TRUE;
  }
  elseif (drupal_substr($form_state['values']['ls_path'], -1, 1) == '/') {
    form_set_error('ls_path', t('Delete trailing slash.'));
    $error = TRUE;
  }

  // Validate the ls_delta_time value.
  if (!isset($form_state['values']['ls_delta_time']) || (!empty($form_state['values']['ls_delta_time']) && !is_numeric($form_state['values']['ls_delta_time']))) {
    form_set_error('ls_delta_time', t("The '%setting' setting is not valid.", array('%setting' => $form['ls_survey']['ls_delta_time']['#title'])));
    $error = TRUE;
  }

  // Display specific error messages only if the 'save' button is clicked.
  if ($form_state['values']['op'] == t('Save')) {

    // Test if all tests have passed.
    // I'm not sure values have been sanitized through the form process.
    $settings = array_map('check_plain', array_intersect_key($form_state['values'], array_flip(array('ls_path', 'ls_delta_time', 'ls_delta_time_was_checked'))));
    module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
    $result = limesurvey_sync_test_interface($settings);
    foreach ($result as $property => $datas) {
      if ($datas['result'] === FALSE) {
        $target = ($property == 'time') ? 'ls_delta_time' : ($property == 'url') ? 'ls_path' : '';
        form_set_error($target, $datas['message']);
      }
      elseif ($datas['result'] === NULL) {
        // Allow to save form inputs.
        drupal_set_message($datas['message'], 'warning');
      }
    }
  }
  else {
    if ($error) {
      // Only update the test interface display.
      // After submitting, don't use cache and rebuild the form in order to update
      // the test interface results with the provided values.
      // Note : If the user click on the 'save' button and a error is generated
      // during validation, the form will not be rebuild (form API limitation).
      // Not so big deal, the error message will provide the error information.
      $form_state['rebuild'] = TRUE;
    }
    $settings =  variable_get('ls_settings', array());
    // I'm not sure values have been sanitized throught the form process.
    $modified_settings = array_map('check_plain', array_intersect_key($form_state['values'], array_flip(array('ls_path', 'ls_delta_time', 'ls_delta_time_was_checked'))));
    if ($modified_settings != $settings) {
      drupal_set_message(t("The new settings have not been saved. Click on the 'Save' button in order to save them."), 'warning');
    }
  }
}

/**
 * Submit limesurvey_sync module options.
 */
function limesurvey_sync_survey_admin_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Save')) {
    $settings =  variable_get('ls_settings', array());
    // I'm not sure values have been sanitized throught the form process.
    $modified_settings = array_map('check_plain', array_intersect_key($form_state['values'], array_flip(array('ls_path', 'ls_delta_time', 'ls_delta_time_was_checked'))));

    if ($modified_settings != $settings) {
      variable_set('ls_settings', $modified_settings);
      drupal_set_message(t('New settings have been saved.'));
      // Watchdog the new settings :
      module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
      limesurvey_sync_custom_watchdog('ls_settings', $settings, $modified_settings);
    }
    else {
      drupal_set_message(t("Settings haven't been changed."));
    }
  }
}

/**
 * Displays results concerning tests on the LimeSurvey synchronization settings.
 *
 */
function theme_limesurvey_sync_sync_admin_testing($variables) {
  $form = $variables['form'];
  $rows = $row = $header = array();
  $i = 0;
  foreach (element_children($form['display'], TRUE) as $property) {

    $row = array();
    $i++;
    $row[] = array(
      'data' => $i . ') ' . drupal_render($form['display'][$property]['title']),
      'header' => TRUE,
    );
    $row[] = drupal_render($form['display'][$property]['result']);
    $row[] = drupal_render($form['display'][$property]['message']);
    $row[] = (isset($form['limesurvey_sync_sync'][$property])) ? drupal_render($form['limesurvey_sync_sync'][$property]) : '';
    $row[] = drupal_render($form['display'][$property]['help']);
    $rows[] = $row;
  }

  $header[] = array('data' => t('Tested property'));
  $header[] = array('data' => t('Test result'));
  $header[] = array('data' => t('Comment'));
  $header[] = array('data' => t('Edit'));
  $header[] = array('data' => t('Help'));

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output . drupal_render_children($form);
}

/**
 * Form builder for the 'Rewrite settings.php' part.
 *
 * Display input data that will be used for setting the
 * $database['ls']['default'] variable, allowing to connect to the LimeSurvey
 * database. On submit, this variable will be saved into the settings.php file.
 *
 * @see limesurvey_sync_ls_server_validate()
 * @see limesurvey_sync_ls_server_submit()
 * @ingroup forms
 */
function limesurvey_sync_ls_server_form($form, $form_state) {
  $form = array();
  // Code from the install_settings_form() core function.
  global $databases;
  $database = isset($databases['default']['default']) ? $databases['default']['default'] : array();
  form_load_include($form_state, 'inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');

  include_once DRUPAL_ROOT . '/includes/install.core.inc';
  $drivers = drupal_get_database_types();
  $drivers_keys = array_keys($drivers);

  $form['ls_rewrite_settings_file'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set the LimeSurvey database informations'),
    '#description' => t('LimeSurvey database connexion parameters will be generated according to the datas below and will be saved into the settings.php Drupal file.') .
      ' ' . t('Datas should match the ones from the <em>application/config/config.php</em> LimeSurvey file'),
    '#collapsible' => TRUE,
    '#collapsed' => isset($databases['ls']),
  );
  $form['ls_rewrite_settings_file']['same_server'] = array(
    '#type' => 'checkbox',
    '#title' => t('The LimeSuvey site and this Drupal site <strong>are using the same hosting server, database type (@driver) and database</strong>', array('@driver' => $databases['default']['default']['driver'])),
    '#default_value' => 1,
  );
  $form['ls_rewrite_settings_file']['driver'] = array(
    '#type' => 'radios',
    '#title' => t('LimeSurvey site Database type'),
    '#required' => TRUE,
    '#default_value' => !empty($database['driver']) ? $database['driver'] : current($drivers_keys),
    '#description' => t('The type of database your @drupal data will be stored in.', array('@drupal' => t('LimeSurvey'))),
  );
  if (count($drivers) == 1) {
    $form['ls_rewrite_settings_file']['driver']['#disabled'] = TRUE;
    $form['ls_rewrite_settings_file']['driver']['#description'] .= ' ' . t('Your PHP configuration only supports a single database type, so it has been automatically selected.');
  }
  $form['ls_rewrite_settings_file']['driver']['#states'] = array(
    'invisible' => array(
      ':input[name=same_server]' => array('checked' => TRUE),
    ),
  );
  $form['ls_rewrite_settings_file']['driver']['#tree'] = TRUE;
  // Add driver specific configuration options.
  foreach ($drivers as $key => $driver) {
    $form['ls_rewrite_settings_file']['driver']['#options'][$key] = $driver->name();

    $form['ls_rewrite_settings_file']['settings'][$key] = $driver->getFormOptions($database);
    $form['ls_rewrite_settings_file']['settings'][$key]['#prefix'] = '<h2 class="js-hide">' . t('@driver_name settings', array('@driver_name' => $driver->name())) . '</h2>';
    $form['ls_rewrite_settings_file']['settings'][$key]['#type'] = 'container';
    $form['ls_rewrite_settings_file']['settings'][$key]['#tree'] = TRUE;
    $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options']['#parents'] = array($key);
    $form['ls_rewrite_settings_file']['settings'][$key]['#states'] = array(
      'visible' => array(
        ':input[name=driver]' => array('value' => $key),
      ),
    );

    // Move the db_prefix from the advanced option tab.
    if (isset($form['ls_rewrite_settings_file']['settings'][$key]['advanced_options']['db_prefix'])) {
      $form['ls_rewrite_settings_file']['settings'][$key]['db_prefix'] = $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options']['db_prefix'];
      $form['ls_rewrite_settings_file']['settings'][$key]['db_prefix']['#default_value'] = 'lime_';
      $form['ls_rewrite_settings_file']['settings'][$key]['db_prefix']['#description'] = t('a table prefix such as %prefix for your LimeSurvey site here.', array('%prefix' => 'lime_'));
      $form['ls_rewrite_settings_file']['settings'][$key]['db_prefix']['#title'] = t('Your LimeSurvey site table prefix');
      unset($form['ls_rewrite_settings_file']['settings'][$key]['advanced_options']['db_prefix']);
    }
    // for security reason : do not display datas from the settings.php file.
    $to_delete_value = array('database', 'username', 'password', 'host', 'port');
    $same_type = isset($databases['ls']) && $databases['ls']['default']['driver'] == $key;
    foreach ($to_delete_value as $key_to_delete_value) {

      if (array_key_exists($key_to_delete_value, $form['ls_rewrite_settings_file']['settings'][$key])) {
        $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#default_value'] = '';
        switch ($key_to_delete_value) {
          case 'database':
            $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#title'] = ($form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#title'] != t('Database file')) ? t('Your LimeSurvey site database name') : t('Your LimeSurvey site database file');
            $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#description'] = t('The name of the database your LimeSurvey data is stored in.');
            break;
          case 'username':
            $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#title'] = t('Your LimeSurvey site database username');
            break;
          case 'password':
            $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#title'] = t('Your LimeSurvey site database password');
            break;
        }
      }
      elseif (array_key_exists($key_to_delete_value, $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'])) {
        $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'][$key_to_delete_value]['#default_value'] = '';
        switch ($key_to_delete_value) {
          case 'host':
            $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'][$key_to_delete_value]['#default_value'] = 'localhost';
            $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'][$key_to_delete_value]['#title'] = t('Your LimeSurvey site database host');
            break;
          case 'port':
            $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'][$key_to_delete_value]['#title'] = t('Your LimeSurvey site database port');
            break;
        }
      }

      // Hide advanced fields and the all fields except 'prefix', if 'using the
      // same server' is true.
      if (TRUE) {
        if (in_array($key_to_delete_value, array('database', 'username', 'password'))) {
          $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#states']['invisible'] = array(
            ':input[name=same_server]' => array('checked' => TRUE),
          );
        }
        if (!array_key_exists('#states', $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'])) {
          $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options']['#states']['invisible'] = array(
            ':input[name=same_server]' => array('checked' => TRUE),
          );
        }

      }
      // If same server is checked : mark as 'not required' for fields :
      // database, username, and into the advanced tab : host.
      // This elements will be validate into the _validate function.
      if (array_key_exists($key_to_delete_value, $form['ls_rewrite_settings_file']['settings'][$key]) && array_key_exists('#required', $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]) && $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#required']) {
        $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#required'] = FALSE;
        $form['ls_rewrite_settings_file']['settings'][$key][$key_to_delete_value]['#states']['required'] = array(
          ':input[name=same_server]' => array('checked' => FALSE),
        );
      }
      if (array_key_exists($key_to_delete_value, $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options']) && array_key_exists('#required', $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'][$key_to_delete_value]) && $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'][$key_to_delete_value]['#required']) {
        $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'][$key_to_delete_value]['#required'] = FALSE;
        $form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'][$key_to_delete_value]['#states']['required'] = array(
          ':input[name=same_server]' => array('checked' => FALSE),
        );
      }
    }

    // Delete the advanced option if empty (for SQLite).
    if (count(element_children($form['ls_rewrite_settings_file']['settings'][$key]['advanced_options'])) == 0) {
      unset($form['ls_rewrite_settings_file']['settings'][$key]['advanced_options']);
    }

  }

  $form['ls_rewrite_settings_file']['actions'] = array('#type' => 'actions');
  $form['ls_rewrite_settings_file']['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Rewrite the settings.php file !'),
    '#validate' => array('limesurvey_sync_ls_server_form_validate'),
    '#submit' => array('limesurvey_sync_ls_server_form_submit'),
  );

  $settings_file = './' . conf_path() . '/settings.php';
  $form['ls_rewrite_settings_file']['settings_file'] = array(
    '#type' => 'value',
    '#value' => $settings_file,
  );

  return $form;

}

/**
 * Validate limesurvey_sync_ls_server_form.
 */
function limesurvey_sync_ls_server_form_validate($form, &$form_state) {

  // Adapted from the install_settings_form_validate() function.
  // Build the correct LimeSurvey Database array.
  global $databases;
  $same_server = $form_state['values']['same_server'];

  if ($same_server) {
    $driver = $databases['default']['default']['driver'];
    $ls_database = $databases['default']['default'];
  }
  else {
    $driver = $form_state['values']['driver'];
    $ls_database = array('driver' => $driver);
    foreach (array('database', 'username', 'password', 'host', 'port') as $key) {
      // For SQlite, 'username', 'password', 'host', 'port' does not exists.
      if (array_key_exists($key, $form_state['values'][$driver])) {
        $ls_database[$key] = $form_state['values'][$driver][$key];
      }
    }
  }
  $errors = array();
  // Required options has been disabled : check it here :
  $form_array = $form['limesurvey_sync']['limesurvey_sync_sync']['ls_server']['ls_rewrite_settings_file']['settings'][$driver];
  if (empty($ls_database['database'])) {
    $errors[$driver . '][database'] = t('!name field is required.', array('!name field' => $form_array['database']['#title']));
  }
  if (array_key_exists('username', $ls_database) && empty($ls_database['username'])) {
    $errors[$driver . '][username'] = t('!name field is required.', array('!name field' => $form_array['username']['#title']));
  }
  if (array_key_exists('host', $ls_database) && empty($ls_database['host'])) {
    $errors[$driver . '][host'] = t('!name field is required.', array('!name field' => $form_array['advanced_options']['host']['#title']));
  }
  // change from prefix to db_prefix.
  $ls_database['prefix'] = $form_state['values'][$driver]['db_prefix'];
  //$ls_databases['collation'] = 'utf8_general_ci';

  // Check if the connection succeed.
  if (empty($errors)) {
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
    $errors = limesurvey_sync_ls_server_install_database_errors($ls_database, $form_state['values']['settings_file']);
  }
  foreach ($errors as $name => $message) {
    form_set_error($name, $message);
  }

  $form_state['storage']['database'] = $ls_database;

  // Look if there are some errors :
  if (!form_get_errors()) {
    // No errors, the submit function rewritting the settings.php files is on
    // the verge to be fired.
    // Code from the update_prepare_d7_bootstrap() function.
    include_once DRUPAL_ROOT . '/includes/install.inc';
    $settings_file = conf_path() . '/settings.php';
    // When checking if the file is writeable, Drupal Make it writeable and
    // leave it in that state !
    $writable = drupal_verify_install_file($settings_file, FILE_EXIST | FILE_READABLE | FILE_WRITABLE);
    if (!$writable) {
      form_set_error('any', t('Drupal requires write permissions to %setting_file. If you are unsure how to grant file permissions, consult the <a href="@url">online handbook</a>.', array('%setting_file' => $settings_file, '@url' => 'http://drupal.org/server-permissions')));
    }
  }
}

/**
 * Submit limesurvey_sync_ls_server_form.
 */
function limesurvey_sync_ls_server_form_submit($form, &$form_state) {
  global $databases;
  $ls_database = $form_state['storage']['database'];
  // Create/rewrite the LimeSurvey db connexion.
  limesurvey_sync_drupal_rewrite_settings($ls_database);
}

/**
 * Rewrite the settings.php file, adding the LimeSurvey database connection informations.
 *
 * Rewrite at the end of the file (append) the $databases['ls']['default']
 * variable. If it is already defined, it will be over driven by the new value.
 *
 * @param $ls_database
 *   An array. The Limesurvey database informations.
 */
function limesurvey_sync_drupal_rewrite_settings($ls_database) {
  global $user;
  $settings_file = conf_path() . '/settings.php';
  $full_path = DRUPAL_ROOT . '/' . $settings_file;
  $ls_dadabase_string = "\n" . '$databases[\'ls\'][\'default\'] = ' . var_export($ls_database, TRUE) . ";\n";
  // Set the text in english.
  $text_to_add = "\n\n" . str_repeat('/', 72);
  $text_to_add .=  "\n//<LIMESURVEY_SYNC-START>";
  $text_to_add .=  "\n// The lines below have been generated by the LimeSurvey_sync module,";
  $text_to_add .=  "\n// " . format_string('On the @date, by @user', array('@date' => format_date(REQUEST_TIME, 'short', '', NULL, 'en'), '@user' => $user->name));
  $text_to_add .=  "\n// " . format_string('From the @url page.', array('@url' => url('admin/config/media/ls_sync', array('absolute' => TRUE))));
  $text_to_add .=  $ls_dadabase_string;
  $text_to_add .=  "//<LIMESURVEY_SYNC-END>";
  $text_to_add .= "\n" . str_repeat('/', 72) . "\n\n";
  $fp = fopen($full_path, 'a');
  if ($fp) {
    if (fwrite($fp, $text_to_add) !== FALSE) {
      drupal_set_message(t('LimeSurvey database informations have been successfully appended to the %settings file', array('%settings' => $settings_file)));
      // The settings file has been edited, protect it again.
      if (!drupal_verify_install_file($full_path, FILE_EXIST | FILE_READABLE | FILE_NOT_WRITABLE)) {
        drupal_set_message(t("The file %file is not protected from modifications and poses a security risk. You must change the file's permissions to be non-writable.", array('%file' => $settings_file)), 'error');
      }
    }
    else {
      drupal_set_message(t('Failed to modify %settings. Verify the file permissions. If you are unsure how to grant file permissions, consult the <a href="@url">online handbook</a>.', array('%settings' => $settings_file, '@url' => 'http://drupal.org/server-permissions')), 'error');
    }
  }
  else {
    drupal_set_message(t('Failed to open %settings. Verify the file permissions. If you are unsure how to grant file permissions, consult the <a href="@url">online handbook</a>.', array('%settings' => $settings_file, '@url' => 'http://drupal.org/server-permissions')), 'error');
  }
}
