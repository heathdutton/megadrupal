<?php
/**
 * @file
 * Settings page & associated functionality.
 */

/**
 * Main settings form.
 */
function advagg_replace_settings_form() {
  $form = array();
  $form['#attached']['css'] = array(drupal_get_path('module', 'advagg_replace') . '/advagg_replace.admin.css');
  $form['#attached']['js'] = array(drupal_get_path('module', 'advagg_replace') . '/advagg_replace.admin.js');

  foreach (array('css', 'js') as $type) {
    $settings = variable_get('advagg_replace_' . $type . '_search', array());
    if (!is_array($settings)) {
      $settings = array();
    }

    // Each section will have two blank rows at the bottom as a way of adding
    // new values.
    $settings_blank = array(
      array(
        'string' => '',
        'default' => '',
        'help' => '',
      ),
      array(
        'string' => '',
        'default' => '',
        'help' => '',
      ),
    );

    $form[$type] = array(
      '#type' => 'fieldset',
      '#title' => t(strtoupper($type)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => t('Leave a <em>search string</em> value blank to remove it.'),
    );

    // Display each item.
    $ctr = 0;
    foreach (array_merge($settings, $settings_blank) as $search => $data) {
      $form[$type][$type . ':' . $ctr] = array(
        '#tree' => TRUE,
      );
      $form[$type][$type . ':' . $ctr]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 50,
        '#default_value' => $ctr,
      );
      $form[$type][$type . ':' . $ctr]['string'] = array(
        '#type' => 'textfield',
        '#title' => t('String to search for'),
        '#default_value' => $data['string'],
        '#title_display' => 'invisible',
        '#attributes' => array(
          'title' => t('The string to search for.'),
          'class' => array('search-string'),
        ),
      );
      $form[$type][$type . ':' . $ctr]['default'] = array(
        '#type' => 'textfield',
        '#title' => t('Default replacement'),
        '#title_display' => 'invisible',
        '#default_value' => $data['default'],
        '#attributes' => array(
          'title' => t('If not provided, the string itself will be used.'),
          'class' => array('search-default'),
        ),
      );
      $form[$type][$type . ':' . $ctr]['help'] = array(
        '#type' => 'textarea',
        '#title' => t('Help message to show on the settings form (optional)'),
        '#title_display' => 'invisible',
        '#default_value' => $data['help'],
        '#wysiwyg' => FALSE,
        '#attributes' => array(
          'title' => t('Can be used to help the user understand where the string will be used.'),
          'class' => array('search-help'),
        ),
      );
      $ctr++;
    }

    $form[$type][$type . '_help'] = array(
      '#type' => 'textarea',
      '#title' => t('Help message to show on the replacement form'),
      '#wysiwyg' => TRUE,
      '#weight' => 10,
      '#default_value' => variable_get('advagg_replace_' . $type . '_help', ADVAGG_REPLACE_HELP_MESSAGE),
      '#description' => t("This will be passed through the site's translation system, so it can be further customized per language, if needed."),
    );
  }

  $form['actions'] = array();
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#validate'][] = 'advagg_replace_settings_form_validate';
  $form['#submit'][] = 'advagg_replace_settings_form_submit';

  return $form;
}

/**
 * Validation callback for advagg_replace_settings_form.
 */
function advagg_replace_settings_form_validate($form, &$form_state) {
}

/**
 * Submission callback for advagg_replace_settings_form.
 */
function advagg_replace_settings_form_submit($form, &$form_state) {
  foreach (array('css', 'js') as $type) {
    $settings = array();

    // Compile the records.
    foreach ($form_state['values'] as $key => $val) {
      if (strpos($key, $type . ':') === 0) {
        // Make sure the setting is not being removed.
        if (!empty($form_state['values'][$key]['string'])) {
          $weight = $form_state['values'][$key]['weight'];
          // Update the replacement string.
          $settings[$weight] = array(
            'string' => $form_state['values'][$key]['string'],
            'default' => $form_state['values'][$key]['default'],
            'help' => $form_state['values'][$key]['help'],
          );
        }
      }
    }

    // Save the settings.
    ksort($settings);
    variable_set('advagg_replace_' . $type . '_search', $settings);

    // Save the help message.
    if (isset($form_state['values'][$type . '_help'])) {
      variable_set('advagg_replace_' . $type . '_help', $form_state['values'][$type . '_help']);
    }
  }
}

/**
 * Control the values that are replaced in the output.
 */
function advagg_replace_replacements_form() {
  $form = array();
  $form['#attached']['css'] = array(drupal_get_path('module', 'advagg_replace') . '/advagg_replace.admin.css');
  $form['#attached']['js'] = array(drupal_get_path('module', 'advagg_replace') . '/advagg_replace.admin.js');

  foreach (array('css', 'js') as $type) {
    $settings = variable_get('advagg_replace_' . $type . '_search', array());
    $replacements = variable_get('advagg_replace_' . $type . '_replace', array());

    if (!empty($settings)) {
      $form[$type] = array(
        '#type' => 'fieldset',
        '#title' => t(strtoupper($type)),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#description' => t(variable_get('advagg_replace_' . $type . '_help', ADVAGG_REPLACE_HELP_MESSAGE)),
      );

      // It's relatively safe to use the 'weight' value as an array key in the
      // form, going on the theory that both the main settings forms and the
      // replacement forms won't be edited simultaneously.
      $ctr = 0;
      foreach ($settings as $weight => $data) {
        $form[$type][$type . ':' . $weight] = array(
          '#tree' => TRUE,
        );
        $form[$type][$type . ':' . $weight]['original'] = array(
          '#markup' => filter_xss($data['string']),
        );
        $form[$type][$type . ':' . $weight]['replace'] = array(
          '#type' => 'textfield',
          '#title' => t('The replacement value for: @string', array('@string' => $data['string'])),
          '#title_display' => 'invisible',
          '#default_value' => isset($replacements[$data['string']]) ? $replacements[$data['string']] : $data['default'],
          '#attributes' => array(
            'title' => t('Default: @default', array('@default' => $data['default'])),
            'class' => array('search-replace'),
          ),
          '#field_suffix' => theme('image', array(
            'path' => 'misc/message-16-error.png',
            'width' => 16,
            'height' => 16,
            'alt' => $data['default'],
            'title' => t('Revert this item to the default: @default', array('@default' => $data['default'])),
            'attributes' => array(
              'class' => array('replace-revert'),
              'id' => 'replace-revert:' . $ctr,
            ),
          )),
        );
        $form[$type][$type . ':' . $weight]['help'] = array(
          '#markup' => !empty($data['help']) ? filter_xss($data['help']) : '',
        );
        $form[$type][$type . ':' . $weight]['string'] = array(
          '#type' => 'hidden',
          '#default_value' => $data['string'],
        );
        $ctr++;
      }
    }
  }

  // Only add the actions & save logic if something's available.
  if (!empty($form)) {
    $form['actions'] = array();
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['#validate'][] = 'advagg_replace_replacements_form_validate';
    $form['#submit'][] = 'advagg_replace_replacements_form_submit';
  }

  // No settings were provided.
  else {
    if (user_access('administer site configuration')) {
      drupal_set_message(t('No configurable CSS or JS strings were configured, proceed to <a href="@url">the main settings page</a> to add some.', array('@url' => url(advagg_admin_config_root_path() . '/advagg/replace'))));
    }
    else {
      drupal_set_message(t('No configurable CSS or JS strings were configured.'));
    }
  }

  return $form;
}

/**
 * Validation callback for advagg_replace_replacements_form.
 */
function advagg_replace_replacements_form_validate($form, &$form_state) {
}

/**
 * Submission callback for advagg_replace_replacements_form.
 */
function advagg_replace_replacements_form_submit($form, &$form_state) {
  foreach (array('css', 'js') as $type) {
    $replacements = array();

    // Load the existing settings.
    $settings = variable_get('advagg_replace_' . $type . '_search', array());

    // Update the existing records.
    foreach ($form_state['values'] as $key => $val) {
      if (strpos($key, $type . ':') === 0) {
        $data = $settings[str_replace($type . ':', '', $key)];
        $replacements[$data['string']] = filter_xss($val['replace']);
      }
    }

    // Save the settings.
    variable_set('advagg_replace_' . $type . '_replace', $replacements);
  }
}
