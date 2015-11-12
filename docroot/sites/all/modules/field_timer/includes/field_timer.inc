<?php

/**
 * @file
 * Contains some help functions for formatters and settings forms.
 */

/**
 * Help function.
 *
 * Build renderable array for Text formatter.
 */
function _field_timer_text_formatter($items, $settings) {
  $elements = array();

  foreach ($items as $delta => $item) {
    // Check what we should display - timer or countdown.
    switch ($settings['type']) {
      case 'auto':
        $interval = REQUEST_TIME - $item['timestamp'];
        if ($interval > 0) {
          $sign = '+';
        }
        else {
          $sign = '-';
        }
        $time = $sign . format_interval(abs($interval), $settings['granularity']);
        break;

      case 'timer':
        $time = format_interval(REQUEST_TIME - $item['timestamp'], $settings['granularity']);
        break;

      case 'countdown':
        $time = format_interval($item['timestamp'] - REQUEST_TIME, $settings['granularity']);
        break;
    }
    $elements[$delta] = array(
      '#type' => 'markup',
      '#markup' => $time,
    );
  }

  return $elements;
}

/**
 * Help function.
 *
 * Build renderable array and load neccessary files for County formatter.
 */
function _field_timer_county_formatter($items, $settings, $entity_type, $entity) {
  $info = entity_get_info($entity_type);
  $id_key = $info['entity keys']['id'];
  $js_settings = array();
  $elements = array();
  $js_settings[$entity_type][$entity->{$id_key}]['nid'] = $entity->{$id_key};

  foreach ($items as $delta => $item) {
    $elements[$delta] = array(
      '#type' => 'markup',
      '#markup' => '<div id="county-' . $entity_type . '-' . $entity->{$id_key} . '-' . $delta . '" ' . 'style="background: ' . $settings['background'] . ';"></div>',
    );
    $js_settings[$entity_type][$entity->{$id_key}][$delta] = $item['timestamp'];
  }
  $js_settings[$entity_type][$entity->{$id_key}]['options'] = $settings;
  $js_settings[$entity_type][$entity->{$id_key}]['plugin'] = 'county';

  // Load library files.
  $library = libraries_load('county');
  $library_loaded = $library && !empty($library['loaded']);
  $js_name = 'county.js';
  $base_path = 'sites/all/libraries/county';
  if (!$library_loaded) {
    drupal_set_message(t("Can't load County library. Please download !url jQuery plugin and extract it to @path, so @js can be found at @full_path.", array(
        '!url' => l(t('County'), 'http://www.egrappler.com/free-jquery-count-down-plugin-county/', array('absolute' => TRUE)),
        '@path' => $base_path,
        '@js' => $js_name,
        '@full_path' => $base_path . '/js/' . $js_name,
    )), 'error');
    return array();
  }

  drupal_add_js(array('field_timer' => $js_settings), 'setting');
  drupal_add_js(drupal_get_path('module', 'field_timer') . '/js/field_timer.js', 'file');

  return $elements;
}

/**
 * Help function.
 *
 * Build renderable array and load neccessary files for jQuery Countdown
 * formatter.
 */
function _field_timer_jquery_countdown_formatter($items, $settings, $entity_type, $entity, $instance) {
  $info = entity_get_info($entity_type);
  $id_key = $info['entity keys']['id'];
  $js_settings = array();
  $elements = array();
  $js_settings[$entity_type][$entity->{$id_key}]['nid'] = $entity->{$id_key};

  foreach ($items as $delta => $item) {
    $elements[$delta] = array(
      '#type' => 'markup',
      '#markup' => '<span id="jquery-countdown-' . $entity_type . '-' . $entity->{$id_key} . '-' . $delta . '" class="jquery-countdown"></span>',
    );
    $js_settings[$entity_type][$entity->{$id_key}][$delta] = $item['timestamp'];
  }

  // Add description, expiry text and url.
  $instance_settings = $instance['settings']['jquery-countdown'];
  $data = array($entity_type => $entity);
  $instance_settings['expiryUrl'] = token_replace($instance_settings['expiryUrl'], $data);
  $instance_settings['expiryText'] = token_replace($instance_settings['expiryText'], $data);
  $instance_settings['description'] = token_replace($instance_settings['description'], $data);

  $js_settings[$entity_type][$entity->{$id_key}]['options'] = array_merge($settings, $instance_settings);
  if ($settings['type'] == 'timer' || ($settings['type'] == 'auto' && $item['timestamp'] <= REQUEST_TIME)) {
    $js_settings[$entity_type][$entity->{$id_key}]['options']['until'] = FALSE;
    $js_settings[$entity_type][$entity->{$id_key}]['options']['since'] = TRUE;
  }
  elseif ($settings['type'] == 'countdown' || ($settings['type'] == 'auto' && $item['timestamp'] > REQUEST_TIME)) {
    $js_settings[$entity_type][$entity->{$id_key}]['options']['until'] = TRUE;
    $js_settings[$entity_type][$entity->{$id_key}]['options']['since'] = FALSE;
  }
  $js_settings[$entity_type][$entity->{$id_key}]['plugin'] = 'jquery.countdown';

  // Load library files.
  $library = libraries_load('jquery.countdown');
  $library_loaded = $library && !empty($library['loaded']);
  $js_name = 'jquery.countdown.js';
  $js_name_min = 'jquery.countdown.min.js';
  $js_plugin_name = 'jquery.plugin.js';
  $js_plugin_name_min = 'jquery.plugin.min.js';
  $base_path = 'sites/all/libraries/jquery.countdown';
  if (!$library_loaded) {
    drupal_set_message(t("Can't load jQuery Countdown library. Please download !url plugin and extract it to @path, so @js, @js_min, @plugin and @plugin_min can be found at @full_path and @full_path_min.", array(
        '!url' => l(t('jQuery Countdown'), 'http://keith-wood.name/countdown.html', array('absolute' => TRUE)),
        '@path' => $base_path,
        '@js' => $js_name,
        '@js_min' => $js_name_min,
        '@plugin' => $js_plugin_name,
        '@plugin_min' => $js_plugin_name_min,
        '@full_path' => $base_path . '/' . $js_name,
        '@full_path_min' => $base_path . '/' . $js_name_min,
    )), 'error');
    return array();
  }

  // Add neccessary js and css files.
  drupal_add_js(array('field_timer' => $js_settings), 'setting');
  $path = libraries_get_path('jquery.countdown');
  if (file_exists($path . '/jquery.countdown-' . $settings['regional'] . '.js')) {
    drupal_add_js($path . '/jquery.countdown-' . $settings['regional'] . '.js');
  }
  drupal_add_js(drupal_get_path('module', 'field_timer') . '/js/field_timer.js', 'file');
  drupal_add_css(drupal_get_path('module', 'field_timer') . '/css/field_timer.css');

  return $elements;
}

/**
 * Help function.
 *
 * Build renderable array and load neccessary files for jQuery Countdown LED
 * formatter.
 */
function _field_timer_jquery_countdown_led_formatter($items, $settings, $entity_type, $entity, $instance) {

  $info = entity_get_info($entity_type);
  $id_key = $info['entity keys']['id'];
  $js_settings = array();
  $elements = array();
  $js_settings[$entity_type][$entity->{$id_key}]['nid'] = $entity->{$id_key};

  // Add description, expiry text and url.
  $instance_settings = $instance['settings']['jquery-countdown'];
  $data = array($entity_type => $entity);
  $instance_settings['expiryUrl'] = token_replace($instance_settings['expiryUrl'], $data);
  $instance_settings['expiryText'] = token_replace($instance_settings['expiryText'], $data);
  $instance_settings['description'] = token_replace($instance_settings['description'], $data);

  foreach ($items as $delta => $item) {
    $layout = '<div class="jquery-countdown-led-display-wrapper">';
    if ($settings['display_days']) {
      for ($i = $settings['max_count_of_days']; $i > 0; $i--) {
        $layout .= '<span class="%t% image{d1' . substr('000', 0, $i - 1) . '}"></span>';
      }
      $layout .= '<span class="%t% imageDay"></span><span class="%t% imageSpace"></span>';
    }
    if ($settings['display_hours']) {
      $layout .= '<span class="%t% image{h10}"></span><span class="%t% image{h1}"></span>';
      if ($settings['display_minutes'] || $settings['display_seconds']) {
        $layout .= '<span class="%t% imageSep"></span>';
      }
    }
    if ($settings['display_minutes']) {
      $layout .= '<span class="%t% image{m10}"></span><span class="%t% image{m1}"></span>';
      if ($settings['display_seconds']) {
        $layout .= '<span class="%t% imageSep"></span>';
      }
    }
    if ($settings['display_seconds']) {
      $layout .= '<span class="%t% image{s10}"></span><span class="%t% image{s1}"></span>';
    }
    $layout .= '</div>';
    if (!empty($instance_settings['description'])) {
      $layout .= '<div class="jquery-countdown-led-description">{desc}</div>';
    }

    $elements[$delta] = array(
      '#type' => 'markup',
      '#markup' => '<span id="jquery-countdown-led-' . $entity_type . '-' . $entity->{$id_key} . '-' . $delta . '" class="jquery-countdown-led ' . $settings['countdown_theme'] . '">' . str_replace('%t%', $settings['countdown_theme'], $layout) . '</span>',
    );
    $js_settings[$entity_type][$entity->{$id_key}][$delta] = $item['timestamp'];
  }

  $js_settings[$entity_type][$entity->{$id_key}]['options'] = array_merge($settings, $instance_settings);
  if ($settings['type'] == 'timer' || ($settings['type'] == 'auto' && $item['timestamp'] <= REQUEST_TIME)) {
    $js_settings[$entity_type][$entity->{$id_key}]['options']['until'] = FALSE;
    $js_settings[$entity_type][$entity->{$id_key}]['options']['since'] = TRUE;
  }
  elseif ($settings['type'] == 'countdown' || ($settings['type'] == 'auto' && $item['timestamp'] > REQUEST_TIME)) {
    $js_settings[$entity_type][$entity->{$id_key}]['options']['until'] = TRUE;
    $js_settings[$entity_type][$entity->{$id_key}]['options']['since'] = FALSE;
  }
  $js_settings[$entity_type][$entity->{$id_key}]['plugin'] = 'jquery.countdown.led';

  // Load library files.
  $library = libraries_load('jquery.countdown');
  $library_loaded = $library && !empty($library['loaded']);
  $js_name = 'jquery.countdown.js';
  $js_name_min = 'jquery.countdown.min.js';
  $base_path = 'sites/all/libraries/jquery.countdown';
  if (!$library_loaded) {
    drupal_set_message(t("Can't load jQuery Countdown library. Please download !url plugin and extract it to @path, so @js and @js_min can be found at @full_path and @full_path_min.", array(
        '!url' => l(t('jQuery Countdown'), 'http://keith-wood.name/countdown.html', array('absolute' => TRUE)),
        '@path' => $base_path,
        '@js' => $js_name,
        '@js_min' => $js_name_min,
        '@full_path' => $base_path . '/' . $js_name,
        '@full_path_min' => $base_path . '/' . $js_name_min,
    )), 'error');
    return array();
  }

  // Add neccessary js and css files.
  drupal_add_js(array('field_timer' => $js_settings), 'setting');
  drupal_add_js(drupal_get_path('module', 'field_timer') . '/js/field_timer.js', 'file');
  drupal_add_css(drupal_get_path('module', 'field_timer') . '/css/field_timer.css');
  // Since we can't get path to image from CSS file, we add this rule here.
  $path = libraries_get_path('jquery.countdown');
  drupal_add_css('.jquery-countdown-led.green span{background: url(/' . $path . '/countdownLED.png) no-repeat 0px 0px;} .jquery-countdown-led.blue span{background: url(/' . $path . '/countdownGlowing.gif) no-repeat 0px 0px;}', 'inline');

  return $elements;
}

/**
 * Help function.
 *
 * Build summary for field_timer_text formatter.
 */
function _field_timer_text_summary($settings) {
  $summary = array();
  $types = array(
    'auto' => t('Auto'),
    'timer' => t('Timer'),
    'countdown' => t('Countdown'),
  );
  $summary[] = t('Type: %type', array('%type' => $types[$settings['type']]));
  $summary[] = t('Granularity: %granularity', array('%granularity' => $settings['granularity']));

  return implode('<br/>', $summary);
}

/**
 * Help function.
 *
 * Build summary for field_timer_county formatter.
 */
function _field_timer_county_summary($settings) {
  $summary = array();
  $animations = array(
    'fade' => t('Fade'),
    'scroll' => t('Scroll'),
  );
  $summary[] = t('Animation: %animation', array('%animation' => $animations[$settings['animation']]));
  $summary[] = t('Speed: %speed', array('%speed' => $settings['speed'] . 'ms'));
  $themes = array(
    'black' => t('Black'),
    'gray' => t('Gray'),
    'red' => t('Red'),
    'blue' => t('Blue'),
  );
  $summary[] = t('Theme: %theme', array('%theme' => $themes[$settings['county_theme']]));
  $summary[] = t('Background: %css', array('%css' => $settings['background']));
  $summary[] = t('Reflection: %state', array(
    '%state' => $settings['reflection'] ? 'Enabled' : 'Disabled',
  ));
  if ($settings['reflection']) {
    $summary[] = t('Reflection opacity: %opacity', array(
        '%opacity' => $settings['reflectionOpacity'],
    ));
  }

  return implode($summary, '<br />');
}

/**
 * Help function.
 *
 * Build summary for field_timer_jquery_countdown formatter.
 */
function _field_timer_jquery_countdown_summary($settings) {
  $summary = array();
  $types = array(
    'auto' => t('Auto'),
    'timer' => t('Timer'),
    'countdown' => t('Countdown'),
  );
  $summary[] = t('Type: %type', array('%type' => $types[$settings['type']]));

  $regions = _field_timer_jquery_countdown_regions();
  $summary[] = t('Region: %regional', array('%regional' => $regions[$settings['regional']]));

  $summary[] = t('Format: %format', array('%format' => $settings['format']));
  $summary[] = t('Layout: %layout', array('%layout' => $settings['layout']));
  $summary[] = t('Compact: %compact', array('%compact' => $settings['compact'] ? t('Yes') : t('No')));
  $summary[] = t('Significant: %significant', array('%significant' => $settings['significant']));
  $summary[] = t('Time separator: %timeSeparator', array('%timeSeparator' => $settings['timeSeparator']));
  $summary[] = t('Pad with zeroes: %padZeroes', array('%padZeroes' => $settings['padZeroes'] ? t('Yes') : t('No')));

  return implode($summary, '<br />');
}

/**
 * Help function.
 *
 * Build summary for field_timer_jquery_countdown_led formatter.
 */
function _field_timer_jquery_countdown_led_summary($settings) {
  $summary = array();
  $types = array(
    'auto' => t('Auto'),
    'timer' => t('Timer'),
    'countdown' => t('Countdown'),
  );
  $summary[] = t('Type: %type', array('%type' => $types[$settings['type']]));

  $themes = array(
    'green' => t('Green'),
    'blue' => t('Blue'),
  );
  $summary[] = t('Theme: %theme', array('%theme' => $themes[$settings['countdown_theme']]));

  $summary[] = t('Display days: %display_days', array('%display_days' => $settings['display_days'] ? t('Yes') : t('No')));
  if ($settings['display_days']) {
    $options = array(
      1 => 9,
      2 => 99,
      3 => 999,
      4 => 9999,
    );
    $summary[] = t('Maximum count of days: %max_count_of_days', array('%max_count_of_days' => $options[$settings['max_count_of_days']]));
  }

  $summary[] = t('Display hours: %display_hours', array('%display_hours' => $settings['display_hours'] ? t('Yes') : t('No')));
  $summary[] = t('Display minutes: %display_minutes', array('%display_minutes' => $settings['display_minutes'] ? t('Yes') : t('No')));
  $summary[] = t('Display seconds: %display_seconds', array('%display_seconds' => $settings['display_seconds'] ? t('Yes') : t('No')));

  return implode($summary, '<br />');
}

/**
 * Help function.
 *
 * Build settings form for field_timer_text formatter.
 */
function _field_timer_text_settings_form($settings) {
  $settings_form = array();
  $settings_form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      'auto' => t('Auto'),
      'timer' => t('Timer'),
      'countdown' => t('Countdown'),
    ),
    '#default_value' => $settings['type'],
  );

  $settings_form['granularity'] = array(
    '#type' => 'select',
    '#title' => t('Granularity'),
    '#options' => range(0, 7),
    '#default_value' => $settings['granularity'],
  );

  return $settings_form;
}

/**
 * Help function.
 *
 * Build settings form for field_timer_county formatter.
 */
function _field_timer_county_settings_form($settings) {
  $settings_form = array();
  $settings_form['animation'] = array(
    '#type' => 'select',
    '#title' => t('Animation'),
    '#options' => array(
      'fade' => t('Fade'),
      'scroll' => t('Scroll'),
    ),
    '#default_value' => $settings['animation'],
  );

  $settings_form['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#default_value' => $settings['speed'],
  );

  $settings_form['county_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => array(
      'black' => t('Black'),
      'gray' => t('Gray'),
      'red' => t('Red'),
      'blue' => t('Blue'),
    ),
    '#default_value' => $settings['county_theme'],
  );

  $settings_form['background'] = array(
    '#type' => 'textfield',
    '#title' => t('Background'),
    '#default_value' => $settings['background'],
    '#description' => t("Data from this field will be added to css property 'background'."),
  );

  $settings_form['reflection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add reflection'),
    '#default_value' => $settings['reflection'],
  );

  $settings_form['reflectionOpacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Reflection opacity'),
    '#default_value' => $settings['reflectionOpacity'],
    '#description' => t('Float value between 0 and 1.'),
  );

  return $settings_form;
}

/**
 * Help function.
 *
 * Build settings form for field_timer_jquery_countdown formatter.
 */
function _field_timer_jquery_countdown_settings_form($settings) {
  $settings_form = array();
  $settings_form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      'auto' => t('Auto'),
      'timer' => t('Timer'),
      'countdown' => t('Countdown'),
    ),
    '#default_value' => $settings['type'],
  );

  $settings_form['regional'] = array(
    '#type' => 'select',
    '#title' => t('Region'),
    '#default_value' => $settings['regional'],
    '#options' => _field_timer_jquery_countdown_regions(),
  );

  $settings_form['format'] = array(
    '#type' => 'textfield',
    '#title' => t('Format'),
    '#default_value' => $settings['format'],
    '#description' => t("See !documentation for this parameter.", array(
      '!documentation' => l(t('documentation'),'http://keith-wood.name/countdownRef.html', array('fragment' => 'format'))
    )),
  );

  $settings_form['layout'] = array(
    '#type' => 'textarea',
    '#rows' => 3,
    '#cols' => 60,
    '#title' => t('Layout'),
    '#defualt_value' => $settings['layout'],
    '#description' => t("See !documentation for this parameter.", array(
      '!documentation' => l(t('documentation'), 'http://keith-wood.name/countdownRef.html', array('fragment' => 'layout'))
    )),
  );

  $settings_form['compact'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display in compact format'),
    '#default_value' => $settings['compact'],
  );

  $settings_form['significant'] = array(
    '#type' => 'select',
    '#title' => t('Significant'),
    '#options' => range(0, 7),
    '#default_value' => $settings['significant'],
  );

  $settings_form['timeSeparator'] = array(
    '#type' => 'textfield',
    '#title' => t('Time separator'),
    '#default_value' => $settings['timeSeparator'],
  );
  
  $settings_form['padZeroes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pad with zeroes'),
    '#default_value' => $settings['padZeroes'],
  );

  return $settings_form;
}

/**
 * Help function.
 *
 * Build settings form for field_timer_jquery_countdown_led formatter.
 */
function _field_timer_jquery_countdown_led_settings_form($settings) {
  $settings_form = array();
  $settings_form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      'auto' => t('Auto'),
      'timer' => t('Timer'),
      'countdown' => t('Countdown'),
    ),
    '#default_value' => $settings['type'],
  );

  $settings_form['countdown_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => array(
      'green' => t('Green'),
      'blue' => t('Blue'),
    ),
    '#default_value' => $settings['countdown_theme'],
  );

  $settings_form['display_days'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display days'),
    '#default_value' => $settings['display_days'],
    '#attributes' => array('class' => array('field-timer-dislay-days')),
  );

  $settings_form['max_count_of_days'] = array(
    '#type' => 'select',
    '#title' => t('Max count of days'),
    '#options' => array(
      1 => 9,
      2 => 99,
      3 => 999,
      4 => 9999,
    ),
    '#default_value' => $settings['max_count_of_days'],
    '#states' => array(
      'invisible' => array(
        'input.field-timer-dislay-days' => array('checked' => FALSE),
      ),
    ),
  );

  $settings_form['display_hours'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display hours'),
    '#default_value' => $settings['display_hours'],
  );

  $settings_form['display_minutes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display minutes'),
    '#default_value' => $settings['display_minutes'],
  );

  $settings_form['display_seconds'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display seconds'),
    '#default_value' => $settings['display_seconds'],
  );

  return $settings_form;
}
