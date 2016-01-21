<?php
/**
 * @file
 * Code for admin page for LiveInternet module.
 */

/**
 * General configuration page for LiveInternet module.
 */
function liveinternet_admin_settings_form($form, &$form_state) {

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'liveinternet') . '/liveinternet.admin.css',
  );

  $form['liveinternet_type'] = array(
    '#type' => 'radios',
    '#title' => t('Counter types'),
    '#options' => array(
      t('Type 1 (size: 88x31)'),
      t('Type 2 (size: 88x31)'),
      t('Type 3 (size: 88x31)'),
      t('Type 4 (size: 88x15)'),
      t('Type 5 (size: 31x31)'),
      t('Type 6 (size: 88x120)'),
    ),
    '#default_value' => variable_get('liveinternet_type', 0),
    // Change color scheme after selecting type.
    '#ajax' => array(
      'callback' => 'liveinternet_ajax_callback',
      'wrapper' => 'liveinternet-color-schemes',
      'method' => 'replace',
      'progress' => array('type' => 'none'),
    ),
    // Add addition description.
    0 => array(
      '#description' => t('Counters show number of pageviews and visitors for the last 24 hours.'),
    ),
    1 => array(
      '#description' => t('Counters show number of pageviews for 24 hours, number of visitors for 24 hours and number of visitors for today (from midnight according to Moscow time).'),
    ),
    3 => array(
      '#description' => t('Counters show number of visitors for today.'),
    ),
    5 => array(
      '#description' => t('Counters show number of pageviews and visitors.'),
    ),
  );

  $form['liveinternet-displays'] = array(
    '#type' => 'fieldset',
    '#title' => t('Displays'),
  );

  // Generate form sections for each type of counter.
  for ($i = 0; $i <= 5; $i++) {

    $formid = 'liveinternet_id' . $i;
    $options = unserialize(LIVEINTERNET_IDENTIFIERS);

    foreach ($options as &$value) {

      // Make keys are values.
      $value = array_combine($value, $value);

      foreach ($value as &$value2) {
        $attributes = _liveinternet_attributes($value2);
        $value2 = theme('liveinternet_image', array(
          // Do images without links.
          'link' => FALSE,
          'id' => $value2,
          'color' => 6,
          'height' => $attributes['height'],
          'width' => $attributes['width'],
          'alt' => t('Counter display number @display.', array('@display' => $value2)),
          'group' => NULL,
        ));
      }
    }

    // Get the first key of array.
    reset($options[$i]);
    $default_value = key($options[$i]);

    $form['liveinternet-displays'][$formid] = array(
      '#type' => 'radios',
      '#title' => t('Counter type @i displays', array('@i' => $i + 1)),
      '#options' => $options[$i],
      '#description' => t('Select a LiveInternet counter type and size.'),
      '#default_value' => variable_get($formid, $default_value),
      '#states' => array(
        'visible' => array(
          ':input[name="liveinternet_type"]' => array('value' => $i),
        ),
      ),
      // Change color scheme after selecting display.
      '#ajax' => array(
        'callback' => 'liveinternet_ajax_callback',
        'wrapper' => 'liveinternet-color-schemes',
        'method' => 'replace',
        'progress' => array('type' => 'none'),
      ),
    );
  }

  // Generate array with color schemes.
  $variables = array();
  // Do images without links.
  $variables['link'] = FALSE;
  $variables['id'] = variable_get('liveinternet_id', 52);

  // Get width and height of counter image.
  $attributes = _liveinternet_attributes($variables['id']);
  $variables['width'] = $attributes['width'];
  $variables['height'] = $attributes['height'];

  $color_array = array();

  // Default numbers of color schemes.
  $color_schemes = 18;

  // Counter type 6 has two addition color schemes.
  if (($variables['id'] == 27) || ($variables['id'] == 28) || ($variables['id'] == 29)) {
    $color_schemes = 20;
  }

  // If Ajax is done.
  // Make sure all form state values are available.
  if ((isset($form_state['values']['liveinternet_type']) &&
      (isset($form_state['values']['liveinternet_id0'])) &&
      (isset($form_state['values']['liveinternet_id1'])) &&
      (isset($form_state['values']['liveinternet_id2'])) &&
      (isset($form_state['values']['liveinternet_id3'])) &&
      (isset($form_state['values']['liveinternet_id4'])) &&
      (isset($form_state['values']['liveinternet_id5'])) &&
      (isset($form_state['values']['liveinternet_color'])))) {

    // Find the counter ID for selected type.
    $type = $form_state['values']['liveinternet_type'];
    $currentid = 'liveinternet_id' . $type;
    $selected_display = $form_state['values'][$currentid];

    // Counter type 6 has two addition color schemes.
    if (($selected_display == 27) || ($selected_display == 28) || ($selected_display == 29)) {
      $color_schemes = 20;
    }
    // Back to default settings.
    else {
      $color_schemes = 18;
    }

    // Color schemes 19 anв 20 is available only for counter type number 6.
    // If these schemes have been selected and then selected type less than 6
    // reset user's choose to color scheme number 1.
    if ((($form_state['values']['liveinternet_color']) >= 19) && ($type != 5)) {
      $form_state['input']['liveinternet_color'] = 1;
    }
  }

  // Generate color scheme array.
  // There are 18 different color schemes for each counter type.
  for ($i = 1; $i <= $color_schemes; $i++) {

    $variables['color'] = $i;

    // If Ajax is done.
    if (isset($selected_display)) {
      $variables['id'] = $selected_display;
      $attributes = _liveinternet_attributes($selected_display);
      $variables['width'] = $attributes['width'];
      $variables['height'] = $attributes['height'];
    }
    $variables['alt'] = t('Counter color scheme number @color for display number @display.',
      array('@color' => $i, '@display' => $variables['id']));

    $color_array[$i] = theme('liveinternet_image', $variables);

  }

  $form['liveinternet-displays']['liveinternet_color'] = array(
    '#type' => 'radios',
    '#title' => t('Counter color schemes'),
    '#options' => $color_array,
    '#description' => t('Select a LiveInternet color scheme.'),
    '#default_value' => variable_get('liveinternet_color', 1),
    '#prefix' => '<div id="liveinternet-color-schemes">',
    '#suffix' => '</div>',
  );

  $form['liveinternet-advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['liveinternet-advanced']['liveinternet_style'] = array(
    '#type' => 'radios',
    '#title' => t('Image code style'),
    '#options' => array(
      t('In the form of single image.'),
      t('In the form of two images.'),
    ),
    '#description' => t('Select a LiveInternet counter code style.'),
    '#default_value' => variable_get('liveinternet_style', 0),
    // Add addition description.
    0 => array(
      '#description' => t('Combining in itself counter and LiveInternet logotype.'),
    ),
    1 => array(
      '#description' => t('One of which is a counter (transparent GIF in size of 1x1), and the other is LiveInternet logotype. Such location mode allows you to insert code of invisible counter in the beginning of the page, and logotype – where allow design and content of the page.'),
    ),
  );

  $form['liveinternet-advanced']['liveinternet_js_scope'] = array(
    '#type' => 'select',
    '#title' => t('JavaScript scope'),
    '#options' => array(
      t('After <body> tag'),
      t('Before </body> tag'),
    ),
    '#description' => t('Select the scope where JavaScript code will be added.'),
    '#default_value' => variable_get('liveinternet_js_scope', 0),
    '#states' => array(
      'visible' => array(
        ':input[name="liveinternet_style"]' => array('value' => 1),
      ),
    ),
  );

  $form['liveinternet-advanced']['liveinternet_html_line'] = array(
    '#type' => 'checkbox',
    '#title' => t('"Stretch" html-code in one line.'),
    '#description' => t('If it is more convenient for you to use oneline html-code, turn on this checkmark; but remember that in this case JavaScript-code of the counter will be designed out of standards: in browsers, which do not JavaScript, it can be a cause of showing JavaScript-code as a straight text.'),
    '#default_value' => variable_get('liveinternet_html_line', 0),
  );

  $form['liveinternet-advanced']['liveinternet_page_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep record of page titles.'),
    '#description' => t('If you turn on this option, then statistics will be gathered by titles of your pages (what is written in tag !title in html-code of your pages, or you can substitute yourself in counter code !substring for convenient for you title). If your pages have sensible titles, then, perhaps, such statistics will be convenient for you. And if titles of your pages contain not short and brief description of page content (what as a matter of fact they must contain), but long nonsense of keywords for search engines, then, please, do not turn on this option.', array('!title' => '<code>&lt;title&gt;</code>', '!substring' => '<code>document.title.substring(0,80)</code>')),
    '#default_value' => variable_get('liveinternet_page_titles', 0),
  );

  $form['liveinternet-advanced']['liveinternet_group'] = array(
    '#type' => 'textfield',
    '#title' => t('Group name'),
    '#description' => t('Enter the group name for tracking a group of sites. See !documentation for more information. Leave blank if not using.', array('!documentation' => l(t('documentation'), 'http://www.liveinternet.ru/help/group.html'))),
    '#default_value' => variable_get('liveinternet_group', NULL),
  );

  $form['#submit'][] = 'liveinternet_admin_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Validate settings in LiveInternet admin interface form.
 */
function liveinternet_admin_settings_form_validate($form, &$form_state) {

  // These is common used then JavaScript is set off and Ajax does not work.
  if (isset($form_state['values']['liveinternet_type']) &&
      isset($form_state['values']['liveinternet_color'])) {

    if ($form_state['values']['liveinternet_type'] <= 4 && $form_state['values']['liveinternet_color'] >= 19) {
      form_set_error('liveinternet_color', t('Last two color schemes are available only for counter type 6. You have chosen counter type @type.', array('@type' => $form_state['values']['liveinternet_type'] + 1)));

    }
  }

  if (!empty($form_state['values']['liveinternet_group'])) {

    if (drupal_strlen($form_state['values']['liveinternet_group']) < 4) {
      form_set_error('liveinternet_group', t('Group name must contain at least four characters.'));
    }

    if (preg_match('/[^A-Z0-9-_]/i', $form_state['values']['liveinternet_group'])) {
      form_set_error('liveinternet_group', t('Illegal symbols in the group name: only figures, Roman letters, underlining, hyphen and slash can be used.'));
    }
  }

}

/**
 * Save current selected counter ID in addition variable.
 */
function liveinternet_admin_settings_form_submit($form, &$form_state) {

  $type = $form['liveinternet_type']['#value'];
  $current_element = 'liveinternet_id' . $type;
  variable_set('liveinternet_id', $form['liveinternet-displays'][$current_element]['#value']);

}

/**
 * Return color schemes for current selected counter ID.
 *
 * @return array
 *   Renderable array (the textfields element)
 */
function liveinternet_ajax_callback($form, $form_state) {
  return $form['liveinternet-displays']['liveinternet_color'];
}
