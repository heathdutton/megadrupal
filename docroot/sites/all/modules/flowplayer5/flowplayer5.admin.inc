<?php

/**
 * @file
 * Provides the administration settings for the Flowplayer Drupal module.
 */

/**
 * Administration settings for the Flowplayer Drupal module.
 */
function flowplayer5_admin_settings_page() {
  // Get settings for different sample player variations.
  $settings = flowplayer5_get_settings();
  $form = array();
  $form['flowplayer5_branding'] = array(
    '#prefix' => '<div class="flowplayer5-branding"><legend>' . t('Branding'),
    '#suffix' => '</legend></div>',
    '#type' => 'checkbox',
    '#title' => t('Remove Flowplayer logo or use your own (does not apply to watermarks on videos deployed via our free encoding service)'),
    '#default_value' => variable_get('flowplayer5_branding', FALSE),
  );

  $form['flowplayer5_key'] = array(
    '#type' => 'textfield',
    '#title' => t('License key (removes Flowplayer logo)'),
    '#default_value' => variable_get('flowplayer5_key', ''),
    '#states' => array(
      'invisible' => array(
        ':input[name="flowplayer5_branding"]' => array('checked' => FALSE)
      )
    ),
    '#attributes' => array('placeholder' => '$000000000000000')
  );
  $form['flowplayer5_logo'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom logo (optional)'),
    '#description' => t('!link to get the license key', array('!link' => l(t('Buy a Commercial license'), 'https://flowplayer.org/pricing/', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('flowplayer5_logo', ''),
    '#states' => array(
      'invisible' => array(
        ':input[name="flowplayer5_branding"]' => array('checked' => FALSE)
      )
    ),
    '#attributes' => array('placeholder' => 'http://mydomain.com/logo/path')
  );



  // Construct the color picker
  $form['color'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player colors'),
    '#attributes' => array('id' => 'flowplayer5-color'),
  );
  $form['color']['picker'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="flowplayer5-color-picker"></div>',
  );

  // Add Farbtastic color picker and the Flowplayer administration JS/CSS
  drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(array('flowplayer5' => array('settings' => $settings)), 'setting');

  drupal_add_js(drupal_get_path('module', 'flowplayer5') . '/js/flowplayer5.admin.js', array('weight' => 1));
  drupal_add_css(drupal_get_path('module', 'flowplayer5') . '/css/flowplayer5.admin.css');

  // Get variation
  $vari = variable_get('flowplayer5_variation', 'minimalist');

  $form['color']['flowplayer5_variation'] = array(
    '#type' => 'select',
    '#title' => t('Sample Variation'),
    '#options' => array(
      'minimalist' => t('Minimalist'),
      'functional' => t('Functional'),
      'playful' => t('Playful'),
    ),
    '#default_value' => variable_get('flowplayer5_variation', 'minimalist'),
  );



  // The selector controls
  $names = array(
    'backgroundColor' => t('Background'),
    'timelineColor' => t('Timeline'),
    'progressColor' => t('Progress'),
    'bufferColor' => t('Buffer'),
    'playerBgColor' => t('Player Background'),
  );
  // Default values
  $default = array(
    'backgroundColor' => '#000000',
    'timelineColor' => '#000000',
    'progressColor' => '#db0000',
    'bufferColor' => '#f9f9f9',
    'playerBgColor' => '#000000',
  );

  foreach ($names as $name => $clean_name) {
    $form['color']['flowplayer5_color_' . $name] = array(
      '#type' => 'textfield',
      '#title' => $clean_name,
      '#default_value' => variable_get('flowplayer5_color_' . $name . '_' . $vari, $default[$name]),
      '#size' => 7,
      '#maxlength' => 7,
      '#attributes' => array('rel' => $name),
    );
  }

  // Construct the styling
  $form['styling'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controlbar styling'),
    '#attributes' => array('id' => 'flowplayer5-styling'),
  );
  $form['styling']['flowplayer5_conts'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Buttons'),
    '#options' => array(
      'time' => t('Time'),
      'top_left' => t('Top/Left'),
      'volume' => t('Volume'),
      'slider' => t('Slider'),
      'play' => t('Play'),
      'embed' => t('Embed'),
      'ful_screen' => t('Fullscreen'),
      'darkicons' => t('Drak Icons'),
      'overlayed' => t('Overlayed'),
      'on_mouseover' => t('On mouseover'),
    ),
    // Dependent check box.
    'time' => array(
      '#prefix' => '<div class="flowplayer5 pair-checks">'
    ),
    'top_left' => array(
      '#suffix' => '</div>',
      '#states' => array(
        'disabled' => array(
          ':input[name="flowplayer5_conts[time]"]' => array('checked' => FALSE)
        )
      ),
      '#attributes' => array('disabled' => 'disabled'),
    ),
    // Dependent check box.
    'volume' => array(
      '#prefix' => '<div class="flowplayer5 pair-checks">'
    ),
    'slider' => array(
      '#suffix' => '</div>',
      '#states' => array(
        'disabled' => array(
          ':input[name="flowplayer5_conts[volume]"]' => array('checked' => FALSE)
        )
      ),
      '#attributes' => array('disabled' => 'disabled'),
    ),
    '#options_warpper' => array(array(array('time', 'volume'), array('div'))),
    '#default_value' => variable_get('flowplayer5_conts_' . $vari, array('time', 'volume', 'slider', 'embed', 'fullscreen')),
  );

  /**  $form['styling']['flowplayer5_border_radius'] = array(
    '#type' => 'select',
    '#title' => t('Border Radius'),
    '#options' => drupal_map_assoc(array(0, 10, 15, 23)),
    '#default_value' => variable_get('flowplayer5_border_radius', 0),
    );
   * */
  // Create the preview
  $config = array(
    'playlist' => array(
      'webm' => 'http://stream.flowplayer.org/bauhaus.webm',
      'mp4' => 'http://stream.flowplayer.org/bauhaus.mp4',
      'ogg' => 'http://stream.flowplayer.org/bauhaus.ogv'
    ),
    'ratio' => '3/4',
  );
  $form['preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview'),
    '#description' => theme('flowplayer5', array('config' => $config, 'id' => 'flowplayer5-preview', 'attributes' => array('style' => 'width:700px;height:350px;', 'class' => array($vari, 'no-toggle')))),
    '#attributes' => array('id' => 'flowplayer5-preview-wrapper'),
  );
  // Trigger save
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

function flowplayer5_admin_settings_page_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  // Get variation key to store values in respective variation.
  $vari = $form_state['values']['flowplayer5_variation'];
  // excluded vari fields.
  $ex = array('flowplayer5_branding', 'flowplayer5_logo', 'flowplayer5_key', 'flowplayer5_scaling', 'flowplayer5_variation');

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    if (!in_array($key, $ex)) {
      variable_set($key . '_' . $vari, $value);
    }
    else {
      variable_set($key, $value);
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
}
