<?php

/**
 * @file
 * Admin page callbacks for queryloader module
 */

/**
 * Administration settings
 */
function queryloader_admin($form, &$form_state) {

  // Get the current set variables.
  $paths = variable_get('queryloader_paths', '');
  $options = variable_get('queryloader_options');
  $visibility = variable_get('queryloader_visibility', QUERYLOADER_VISIBILITY_LISTED);
  $types = variable_get('queryloader_types');

  // Script options (labelled with their jQuery option names).
  $form['script_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('QueryLoader script settings'),
    '#description' => t('Configurable settings for QueryLoader 2'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['script_settings']['background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color of the loader'),
    '#size' => 7,
    '#maxlength' => 7,
    '#default_value' => $options['backgroundColor'],
    '#attributes' => array(
      'class' => array(
        'color-box',
      ),
    ),
  );
  $form['script_settings']['bar_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color of the bar'),
    '#size' => 7,
    '#maxlength' => 7,
    '#default_value' => $options['barColor'],
    '#attributes' => array(
      'class' => array(
        'color-box',
      ),
    ),
  );
  $form['script_settings']['bar_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height of the bar in pixels'),
    '#size' => 4,
    '#maxlength' => 255,
    '#default_value' => $options['barHeight'],
  );
  $form['script_settings']['complete_animation'] = array(
    '#type' => 'select',
    '#multiple' => FALSE,
    '#title' => t('End animation type'),
    '#options' => array(
      'grow' => t('Grow'),
      'fade' => t('Fade'),
    ),
    '#default_value' => $options['completeAnimation'],
  );
  $form['script_settings']['percentage'] = array(
    '#type' => 'radios',
    '#title' => t('Percentage visualizing'),
    '#description' => t('Enable to see percentage text on the loading screen.'),
    '#options' => array(
      1 => 'Enabled',
      0 => 'Disabled',
    ),
    '#default_value' => strval($options['percentage']),
  );
  $form['script_settings']['on_load_complete'] = array(
    '#type' => 'textfield',
    '#title' => t('onLoadComplete function'),
    '#description' => t('This function is called once the loading is complete. This is handy when changing the animation at the end.'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => isset($options['onLoadComplete']) ? $options['onLoadComplete'] : '',
  );
  $form['script_settings']['on_complete'] = array(
    '#type' => 'textfield',
    '#title' => t('onComplete function'),
    '#description' => t('This function is called once the loading and animation are completed.'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => isset($options['onComplete']) ? $options['onComplete'] : '',
  );
  $form['script_settings']['ajax_requests'] = array(
    '#type' => 'radios',
    '#title' => t('Apply to all AJAX requests'),
    '#description' => t('Show the loading screen for all Drupal AJAX requests (can be troublesome for simultaneous requests)'),
    '#options' => array(
      1 => 'Enabled',
      0 => 'Disabled',
    ),
    '#default_value' => strval($options['onAjax']),
  );

  // Visibility settings.
  $form['visibility_title'] = array(
    '#type' => 'item',
    '#title' => t('Visibility settings'),
  );
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'queryloader') . '/queryloader_admin.js'),
    ),
  );

  // Per-path visibility.
  $form['visibility']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 0,
  );


  $options = array(
    QUERYLOADER_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    QUERYLOADER_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
    '%blog' => 'blog',
    '%blog-wildcard' => 'blog/*',
    '%front' => '<front>',
  ));

  $title = t('Pages');

  $form['visibility']['path']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Enable QueryLoader on specific pages'),
    '#options' => $options,
    '#default_value' => $visibility,
  );
  $form['visibility']['path']['pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . $title . '</span>',
    '#default_value' => $paths,
    '#description' => $description,
  );
  $form['visibility']['node_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 5,
  );
  $form['visibility']['node_type']['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable QueryLoader for specific content types'),
    '#default_value' => isset($types) ? $types : array(),
    '#options' => node_type_get_names(),
    '#description' => t('Enable QueryLoader only on pages that display content of the given type(s). If you select no types, there will be no type-specific limitation.'),
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );


  return $form;
}

/**
 * Form validation handler for the QueryLoader configuration form.
 *
 * @see queryloader_admin()
 */
function queryloader_admin_validate($form, &$form_state) {
  // Test numbers.
  if ((int) $form['script_settings']['bar_height']['#value'] <= 0) {
    form_set_error('bar_height', t('Bar height must be set and must be a number'));
  };

  // Test colors.
  if (!preg_match('/^#([a-f0-9]{3}){1,2}$/iD', $form['script_settings']['background_color']['#value'])) {
    form_set_error('background_color', t('Background color must be a hex color value (eg #000000)'));
  };
  if (!preg_match('/^#([a-f0-9]{3}){1,2}$/iD', $form['script_settings']['bar_color']['#value'])) {
    form_set_error('bar_color', t('Bar color must be a hex color value (eg #000000)'));
  };
}

/**
 * Form submission handler for the QueryLoader configuration form.
 *
 * @see queryloader_admin()
 */
function queryloader_admin_submit($form, &$form_state) {

  // Script settings.
  $queryloader_settings = array(
    'backgroundColor' => $form['script_settings']['background_color']['#value'],
    'barColor' => $form['script_settings']['bar_color']['#value'],
    'barHeight' => (int) $form['script_settings']['bar_height']['#value'],
    'completeAnimation' => $form['script_settings']['complete_animation']['#value'],
    'percentage' => (bool) $form['script_settings']['percentage']['#value'],
    'onAjax' => (bool) $form['script_settings']['ajax_requests']['#value'],
  );

  // JavaScript Callback functions.
  $callbacks = array(
    'onLoadComplete' => $form['script_settings']['on_load_complete']['#value'],
    'onComplete' => $form['script_settings']['on_complete']['#value'],
  );

  // Only set callback functions if they properly exist (otherwise JS errors will occur).
  foreach ($callbacks as $key => $function) {
    (strlen($function) > 0) ? $queryloader_settings[$key] = $function : '';
  };

  // Save the options.
  variable_set('queryloader_options', $queryloader_settings);

  // Visibility settings.
  variable_set('queryloader_visibility', $form['visibility']['path']['visibility']['#value']);
  variable_set('queryloader_types', $form['visibility']['node_type']['types']['#value']);
  variable_set('queryloader_paths', $form['visibility']['path']['pages']['#value']);

  // Clear the JavaScript aggregation cache.
  drupal_clear_js_cache();

  // Watchdog message.
  watchdog('queryloader', 'Queryloader settings changed');

  // Form success message.
  drupal_set_message(t('Successfully saved QueryLoader2 settings'));
}

