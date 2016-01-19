<?php
/**
 * @file
 * The admin settings for the UpTime module.
 */

/**
 * Implements hook_settings().
 */
function uptime_admin_settings() {
  $last_refresh = variable_get('uptime_next_execution', 0) - variable_get('uptime_interval', 24 * 60 * 60);
  // A preview area.
  $form['uptime_preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview'),
    '#collapsible' => TRUE,
    '#collapsed' => variable_get('uptime_preview_collapsed', TRUE),
    '#description' => t("The actual font color of the caption and optional copyright notice can differ depending on the background color of the region the widget is in.") . '</br></br>',
  );
  // Form submit resulted in an uncollapsed preview. Set it back.
  variable_set('uptime_preview_collapsed', TRUE);
  drupal_add_css(drupal_get_path('module', 'uptime') . '/uptime.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  $results = array(
    'module_path' => drupal_get_path('module', 'uptime'),
  );
  $form['uptime_preview']['uptime_preview_markup'] = array(
    '#markup' => theme('uptime', array('results' => $results)),
  );

  // Essential to have some credentials.
  $api_key = trim(variable_get('uptime_api_key', ''));
  $monitor_id = trim(variable_get('uptime_monitor_id', ''));
  // Where to find the all-time uptime ratio.
  $url = "http://api.uptimerobot.com/getMonitors?apiKey=" . $api_key . "&monitors=" . $monitor_id . "&format=xml";

  $form['api_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Uptime'),
  );

  $form['api_settings']['uptime_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('uptime_enabled', 1),
    '#description' => t('Disabling pauses the monitor until re-enabling and removes the ratio display. Disable uptime when your site might go down temporarily,for example during development, or if you want to use only the copyright notice.'),
  );

  // CSS variables.
  $style_settings_module = l(t('Style (CSS) Settings module'), 'https://drupal.org/project/style_settings', array(
      'attributes' => array(
        'title' => t('Style (CSS) Settings | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  // Put CSS variables together in a fieldset. Remove if only one is given.
  $form['api_settings']['css_variables'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS variables'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (module_exists('style_settings')) {
    $form['api_settings']['css_variables']['#collapsed'] = TRUE;
    $form['api_settings']['css_variables']['uptime_reset'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reset all values back to default'),
      '#default_value' => 0,
    );
    // A SELECTABLE MEASUREMENT UNIT (px, em). It goes together with a submit
    // handler inside the 'function uptime_admin_settings_submit()'.
    $form['api_settings']['css_variables']['uptime_fontsize'] = array(
      '#type' => 'fieldset',
      '#title' => t('Font size'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Number field input help (min, max, step) will be appended.
    );
    // Number field without a '#field_suffix'.
    $form['api_settings']['css_variables']['uptime_fontsize']['uptime_fontsize_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('uptime_fontsize_value', '1'),
    );
    // A measurement unit select field.
    $form['api_settings']['css_variables']['uptime_fontsize']['uptime_fontsize_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
        '%' => t('%'),
      ),
      '#default_value' => variable_get('uptime_fontsize_unit', 'em'),
      '#required' => TRUE,
    );
    // A SELECTABLE MEASUREMENT UNIT (px, em). It goes together with a submit
    // handler inside the 'function uptime_admin_settings_submit()'.
    $form['api_settings']['css_variables']['uptime_radius'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rounded corner radius'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Number field input help (min, max, step) will be appended.
    );
    // Number field without a '#field_suffix'.
    $form['api_settings']['css_variables']['uptime_radius']['uptime_radius_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('uptime_radius_value', '5'),
    );
    // A measurement unit select field.
    $form['api_settings']['css_variables']['uptime_radius']['uptime_radius_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
      ),
      '#default_value' => variable_get('uptime_radius_unit', 'px'),
      '#required' => TRUE,
    );
    // NUMBER in this case with an appended measurement unit (px).
    // E.g. user input: '2', field_suffix: 'px' => stored variable: '2px'.
    $form['api_settings']['css_variables']['uptime_borderwidth'] = array(
      '#type' => 'style_settings_number',
      '#title' => t('Border width'),
      '#default_value' => floatval(variable_get('uptime_borderwidth', '3px')),
      // The suffix gets added to the input on submit if valid measurement unit.
      '#field_suffix' => 'px',
    );

    // COLOR PICKERS.
    $form['api_settings']['css_variables']['uptime_color'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Widget color'),
      // Besides hex color value also color names are accepted.
      '#default_value' => variable_get('uptime_color', 'DarkOrange'),
    );
    $form['api_settings']['css_variables']['uptime_bgcolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Background color'),
      '#default_value' => variable_get('uptime_bgcolor', 'WhiteSmoke'),
    );
    $form['api_settings']['css_variables']['uptime_shadowcolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Shadow color'),
      '#default_value' => variable_get('uptime_shadowcolor', 'Black'),
    );
    $form['api_settings']['css_variables']['uptime_fontcolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Font color'),
      '#default_value' => variable_get('uptime_fontcolor', 'White'),
    );
    $form['api_settings']['css_variables']['uptime_ratiocolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Ratio font color'),
      '#default_value' => variable_get('uptime_ratiocolor', 'DimGray'),
    );
  }

  // If the Style Settings module is not enabled, provide some instructions.
  else {
    $form['api_settings']['css_variables']['uptime_note'] = array(
      '#markup' => t("Enable the !style_settings_module to get style options exposed here. They consist of:<ul>
          <li>Font size.</li>
          <li>Rounded corner radius.</li>
          <li>Border width.</li>
          <li>All of the widget's colors (with color picker).</li>
        </ul>", array('!style_settings_module' => $style_settings_module)),
    );
  }

  $form['api_settings']['uptime_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => $api_key,
    '#description' => t('To get an API key go to') . ' ' . l(t('www.uptimerobot.com'), 'http://www.uptimerobot.com', array(
        'attributes' => array('target' => '_blank'),
        )) . ' ' . t('▶ Start Now ▶ Register ▶ Sign-in ▶ Add New ▶ Monitor Type: HTTP(s) ▶ My Settings ▶ API Information (show details)'),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => TRUE,
  );

  $form['api_settings']['uptime_monitor_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Monitor ID'),
    '#default_value' => $monitor_id,
    '#description' => t('To find your Monitor ID go to') . ' ' . l(t('api.uptimerobot.com/getMonitors?apiKey=FILL-IN-YOUR-API-KEY-HERE'), 'http://api.uptimerobot.com/getMonitors?apiKey=FILL-IN-YOUR-API-KEY-HERE', array(
        'attributes' => array('target' => '_blank'),
        )) . t(', change the last part of the url, reload and copy your monitor ID to here.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );

  // Grabbing the uptime ratio once a day is good enough, but leave it up to
  // the site owner to decide. Second option is the actual set cron interval.
  $form['api_settings']['uptime_interval'] = array(
    '#type' => 'radios',
    '#title' => t('Refresh interval'),
    '#options' => array(
      24 * 60 * 60 => t('24 hours (recommended)'),
      0 => format_interval(DRUPAL_CRON_DEFAULT_THRESHOLD, 2) . ' ' . t('(every cron run)'),
    ),
    '#default_value' => variable_get('uptime_interval', 24 * 60 * 60),
    '#description' => t('Saving this form refreshes the uptime ratio instantly, independent from this setting. Last refresh was @interval ago.', array('@interval' => format_interval(REQUEST_TIME - $last_refresh))),
  );

  // Offering the possibility to check the source of the data.
  $form['api_settings']['raw check'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data check'),
    '#description' => t('Once you saved your credentials, you can check the raw data at') . ':<br />' . l($url, $url, array(
        'attributes' => array('target' => '_blank'),
        )),
  );

  $form['uptime_notice'] = array(
    '#type' => 'fieldset',
    '#title' => t('Copyright notice'),
  );

  $form['uptime_notice']['uptime_notice_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('uptime_notice_enabled', 0),
  );

  // For the examples we use real data.
  // Current domain name without the leading protocol.
  $host = parse_url($GLOBALS['base_url'], PHP_URL_HOST);
  $year = variable_get('uptime_year', '');
  $notice = variable_get('uptime_prepend', 'All rights reserved') . ' © ' . (($year != date('Y') && !empty($year)) ? $year . '-' . date('Y') : date('Y'));
  $form['uptime_notice']['uptime_url_name'] = array(
    // Create different types of notices to choose from.
    '#type' => 'radios',
    '#title' => t('Choose a notice'),
    '#options' => array(
      $host => '<strong>' . $notice . ' ' . $host . '</strong> ' . t('(default)'),
      variable_get('site_name', '') => '<strong>' . $notice . ' ' . variable_get('site_name', '') . '</strong> ' . t("(preferable if the site name is a person's full name or a company name)"),
      ' ' => '<strong>' . $notice . '</strong> ' . t('(leaving out the designation of owner is not recommended)'),
    ),
    '#default_value' => variable_get('uptime_url_name', ' '),
    '#description' => t("'Year of first publication' is not used until entered below, for example © 2009-") . date('Y') . '. ' . t('Save this form to refresh above examples.'),
  );

  $form['uptime_notice']['uptime_year'] = array(
    '#type' => 'textfield',
    '#title' => t('What year was the domain first online?'),
    '#default_value' => variable_get('uptime_year', ''),
    '#description' => t("Leave empty to display only the current year (default). Also if the 'starting year' equals the 'current year' only one will be displayed until next year.<br />To play safe legally, it's best to enter a 'Year of first publication', although copyright is in force even without any notice."),
    '#size' => 4,
    '#maxlength' => 4,
  );

  $form['uptime_notice']['uptime_prepend'] = array(
    '#type' => 'textfield',
    '#title' => t('Prepend text'),
    '#default_value' => trim(variable_get('uptime_prepend', 'All rights reserved')),
    '#description' => t("For example 'All images' on a photographer's website."),
  );

  // Call submit_function() on form submission.
  $form['#submit'][] = 'uptime_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate uptime settings submission.
 */
function uptime_admin_settings_validate($form, &$form_state) {
  $limit = $form_state['values']['uptime_monitor_id'];
  if (!is_numeric($limit)) {
    form_set_error('uptime_monitor_id', '<strong>' . t('MONITOR ID SHOULD BE A NUMBER.') . '</strong>');
  }
  // Before 1991 there was no world wide web and the future can't be a
  // 'year of first publication' but it can be left empty.
  $limit = $form_state['values']['uptime_year'];
  if ((!is_numeric($limit) || $limit < 1991 || $limit > date('Y')) && !empty($limit)) {
    form_set_error('uptime_year', '<strong>' . t('INVALID YEAR.') . '</strong>');
  }
}

/**
 * Submit form data.
 */
function uptime_admin_settings_submit($form, &$form_state) {
  if (module_exists('style_settings')) {
    // Uncollapse the preview. Likely we want to see the changes we just made.
    variable_set('uptime_preview_collapsed', FALSE);
    // SELECTABLE MEASUREMENT UNIT: concatenate the value and unit in a new
    // variable (the one that will be used in the CSS).
    variable_set('uptime_fontsize', $form_state['values']['uptime_fontsize_value'] . $form_state['values']['uptime_fontsize_unit']);
    variable_set('uptime_radius', $form_state['values']['uptime_radius_value'] . $form_state['values']['uptime_radius_unit']);
    if ($form_state['values']['uptime_reset']) {
      $form_state['values']['uptime_fontsize_value'] = '1';
      $form_state['values']['uptime_fontsize_unit'] = 'em';
      variable_set('uptime_fontsize', '1em');
      $form_state['values']['uptime_radius_value'] = '5';
      $form_state['values']['uptime_radius_unit'] = 'px';
      variable_set('uptime_radius', '5px');
      $form_state['values']['uptime_borderwidth'] = '3px';
      $form_state['values']['uptime_color'] = 'YellowGreen';
      $form_state['values']['uptime_bgcolor'] = 'WhiteSmoke';
      $form_state['values']['uptime_shadowcolor'] = 'Black';
      $form_state['values']['uptime_fontcolor'] = 'White';
      $form_state['values']['uptime_ratiocolor'] = 'DimGray';
      drupal_set_message(t("All PageRank widget's CSS values have been set to default."), 'status', FALSE);
    }
    // Make sure changes are visible right after saving the settings.
    _drupal_flush_css_js();
  }
  $api_key = variable_get('uptime_api_key', '');
  $monitor_id  = variable_get('uptime_monitor_id', '');
  // Pause or activate update monitoring depending on the 'enabled' checkbox.
  $status = variable_get('uptime_enabled', 1);
  $url_status = "http://api.uptimerobot.com/editMonitor?apiKey=" . $api_key . "&monitorID=" . $monitor_id . "&monitorStatus=" . $status . "&format=xml";
  // Create a new cURL resource.
  $ch = curl_init();
  // Set URL and other appropriate options.
  curl_setopt($ch, CURLOPT_URL, $url_status);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  // Grab URL and pass it to the browser.
  curl_exec($ch);
  // Close cURL resource, and free up system resources.
  curl_close($ch);

  // Refresh the uptime ratio.
  $api_key = variable_get('uptime_api_key', '');
  if (empty($api_key)) {
    variable_set('uptime_ratio', '[Missing API key] ');
    // Leave a message.
    $message = t('UpTime: Missing API key');
    drupal_set_message($message, 'warning', FALSE);
    watchdog('uptime', $message, NULL, WATCHDOG_WARNING);
  }
  else {
    $monitor_id  = variable_get('uptime_monitor_id', '');
    $url = "http://api.uptimerobot.com/getMonitors?apiKey=" . $api_key . "&monitors=" . $monitor_id . "&format=xml";
    // Curl request.
    $c = curl_init($url);
    curl_setopt($c, CURLOPT_RETURNTRANSFER, TRUE);
    $response_xml = curl_exec($c);
    curl_close($c);
    // XML parsing.
    $xml = simplexml_load_string($response_xml);
    foreach ($xml->monitor as $monitor) {
      variable_set('uptime_ratio', (string) $monitor['alltimeuptimeratio']);
    }
    // Set the next time the hook_cron should be invoked.
    $interval = variable_get('uptime_interval', 24 * 60 * 60);
    variable_set('uptime_next_execution', time() + $interval);
    // Leave a message.
    $message = t('UpTime updated successfully = %uptime %', array('%uptime' => variable_get('uptime_ratio', (string) $monitor['alltimeuptimeratio'])));
    drupal_set_message($message, 'status', FALSE);
    watchdog('uptime', $message);
  }
}
