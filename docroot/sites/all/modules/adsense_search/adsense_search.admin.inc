<?php

/**
 * Helper function providing the ad provider form.
 */
function adsense_search_ads_form(&$form, &$form_state) {
  $settings =& $form_state['item']->settings['adsense_search_ads'];

  // Create form here, for values use $settings[$key]
  $adsense_settings = _adsense_search_ad_settings();
  foreach ($adsense_settings as $k => $v) {
    // Attach the element to the form.
    $form[$k] = $v;
    // Override the default value to the stored value, if it exists.
    if (isset($settings[$k])) {
      $form[$k]['#default_value'] = $settings[$k];
    }
  }
}


/**
 * Ad Manager ad provider form submit for AdSense Custom Search Ads.
 */
function adsense_search_ads_submit(&$form, &$form_state) {
  // A key of the provider name is used to prevent conflicting data if users
  // change providers for a specific ad.
  $settings =& $form_state['item']->settings['adsense_search_ads'];
  $values =& $form_state['values'];

  // Set settings for storage.
  $adsense_settings = _adsense_search_ad_settings();
  foreach ($adsense_settings as $k => $v) {
    // Save the setting if it is not the default for the field.
    if ($v['#default_value'] != $values[$k]) {
      $settings[$k] = $values[$k];
    }
    else {
      unset($settings[$k]);
    }
  }
}

/**
 * Set up the global config form.
 */
function adsense_search_ads_settings_form(&$form, &$form_state) {
  $form['adsense_search'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('AdSense Custom Search Ads'),
    'pub_id' => array(
      '#type' => 'textfield',
      '#title' => t('Publisher ID'),
      '#description' => t('e.g. pub-xxxxxxxxxxxxxxxx'),
      '#default_value' => variable_get('adsense_search__pub_id', ''),
      '#required' => TRUE,
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^pub-[A-Za-z0-9]*$#',
        'error' => t('Provide your publisher ID in the format <em>pub-xxxxxxxxxxxxxxxx</em>.'),
      ),
    ),
    'channel' => array(
      '#type' => 'textfield',
      '#title' => t('Channel'),
      '#description' => t('An AdSense for Search channel may be supplied for tracking the performance of the search. Use the unique channel ID generated from your AdSense account.'),
      '#default_value' => variable_get('adsense_search__channel', ''),
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^[1-9\+]*$#',
        'error' => t('Channel should consist of numbers only. Multiple channels may be used together, separated by <em>+</em>'),
      ),
    ),
  );

  $form['#submit'][] = 'adsense_search_ads_settings_submit';
}

/**
 * Global config form submission handler.
 */
function adsense_search_ads_settings_submit(&$form, &$form_state) {
  foreach ($form_state['values']['adsense_search'] as $key => $value) {
    variable_set('adsense_search__' . $key, $value);
  }
}

/**
 * Attach a default button to a form element to reset to default.
 */
function adsense_search_form_element_default_button($element) {
  // Attach Javascript to make and reset buttons, once per page load.
  static $loaded = FALSE;
  if (!$loaded) {
    drupal_add_js(drupal_get_path('module', 'adsense_search') . '/js/button-default-reset.js');
    $loaded = TRUE;
  }

  // Attach default-reset data.
  $element['#attributes']['data-default-reset'] = $element['#default_reset'];
  return $element;
}

/**
 * Element validate callback for AdSense provider form, apply regex validation.
 */
function _adsense_search_ad_regex_validate($element, &$form_state, $form) {
  $regex =& $element['#regex_validate'];
  if (!preg_match($regex['regex'], $element['#value'])) {
    if (isset($regex['error'])) {
      form_error($element, $regex['error']);
    }
    else {
      form_error($element, t('@title contains invalid input.', array('@title' => $element['#title'])));
    }
  }
}

/**
 * Settings form array for AdSense Custom Search Ads.
 *
 * Helper function to define all of the form elements needed for ad
 * customization.
 *
 * @return array
 */
function _adsense_search_ad_settings() {
  $font_size_options = drupal_map_assoc(array(t('8px'), t('9px'), t('10px'),
      t('11px'), t('12px'), t('13px'), t('14px'), t('15px'), t('16px')));
  $colorpicker = module_exists('jquery_colorpicker');

  $form = array(
    'number' => array(
      '#type' => 'textfield',
      '#title' => t('Number of ads'),
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^[1-9][0-9]*$#',
        'error' => t('Provide a positive integer for the number of ads to display.'),
      ),
      '#default_value' => 2,
    ),
    'width' => array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#description' => t('e.g. 500px'),
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^(?:[1-9][0-9]*px|auto)$#',
        'error' => t('Width must be in the format of a positive integer followed by <em>px</em> or be set to <em>auto</em>.'),
      ),
      '#default_value' => 'auto',
    ),
    'lines' => array(
      '#type' => 'select',
      '#title' => t('Lines'),
      '#options' => drupal_map_assoc(array(t('1'), t('2'), t('3'))),
      '#default_value' => '3',
    ),
    'fontFamily' => array(
      '#type' => 'select',
      '#title' => t('Font family'),
      '#options' => array(
        'arial' => t('Arial'),
        'times' => t('Times New Roman'),
        'verdana' => t('Verdana'),
        'tahoma' => t('Tahoma'),
        'georgia' => t('Georgia'),
        'trebuchet' => t('Trebuchet'),
      ),
      '#default_value' => 'arial',
    ),
    'fontSizeTitle' => array(
      '#type' => 'select',
      '#title' => t('Title font size'),
      '#options' => $font_size_options,
      '#default_value' => '12px',
    ),
    'fontSizeDescription' => array(
      '#type' => 'select',
      '#title' => t('Description font size'),
      '#options' => $font_size_options,
      '#default_value' => '12px',
    ),
    'fontSizeDomainLink' => array(
      '#type' => 'select',
      '#title' => t('Link font size'),
      '#options' => $font_size_options,
      '#default_value' => '12px',
    ),
    'colorTitleLink' => array(
      '#type' => $colorpicker ? 'jquery_colorpicker' : 'textfield',
      '#title' => t('Title color'),
      '#default_value' => '0000FF',
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$#',
        'error' => t('Title color must be a hex color code.'),
      ),
    ),
    'colorText' => array(
      '#type' => $colorpicker ? 'jquery_colorpicker' : 'textfield',
      '#title' => t('Description color'),
      '#default_value' => '000000',
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$#',
        'error' => t('Description color must be a hex color code.'),
      ),
    ),
    'colorDomainLink' => array(
      '#type' => $colorpicker ? 'jquery_colorpicker' : 'textfield',
      '#title' => t('Link color'),
      '#default_value' => '008000',
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$#',
        'error' => t('Link color must be a hex color code.'),
      ),
    ),
    'colorBackground' => array(
      '#type' => $colorpicker ? 'jquery_colorpicker' : 'textfield',
      '#title' => t('Background color'),
      '#default_value' => 'FFFFFF',
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$#',
        'error' => t('Background color must be a hex color code.'),
      ),
    ),
    'colorBorder' => array(
      '#type' => $colorpicker ? 'jquery_colorpicker' : 'textfield',
      '#title' => t('Border color'),
      '#default_value' => 'FFFFFF',
      '#element_validate' => array('adsense_search_ad_regex_validate'),
      '#regex_validate' => array(
        'regex' => '#^([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$#',
        'error' => t('Border color must be a hex color code.'),
      ),
    ),
  );

  foreach ($form as $k => $v) {
    $form[$k]['#default_reset'] = $v['#default_value'];
    $form[$k]['#process'] = array('adsense_search_form_element_default_button');
  }

  return $form;
}
