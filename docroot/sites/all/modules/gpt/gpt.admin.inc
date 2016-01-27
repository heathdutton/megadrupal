<?php

/**
 * Form function; builds GPT ad slot form.
 *
 * @param $form
 * @param $form_state
 * @param $values
 *   Previously set settings for the form.
 */
function gpt_ad_slot_form($form, &$form_state, $settings) {
  $defaults = _gpt_ad_slot_form_defaults($settings, $form_state);
  _gpt_admin_attach_css();
  $form['#tree'] = TRUE;
  // Setting #submit seems to do nothing, so we have to force pass through our
  // subform submit handler in settings form.
  // $form['#submit'] = array('gpt_ad_slot_form_submit');

  $form['size_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ad slot creative size'),
    '#description' => $form_state['num_breakpoints'] === 1
        ? t('Enter <em>width</em> x <em>height</em>, separate multiple sizes with a comma.<br />Example: 300x600,300x250.</p>')
        : t('<p>In the <em>breakpoint</em> field, supply the browser width that the ad slot should use the corresponding size for, starting at 0.<br />Example: Breakpoint of 0 and 728 would display one size from 0-727 and another size for 728 and beyond.</p>' . '<p>In the <em>size</em> field, enter <em>width</em> x <em>height</em>, separate multiple sizes with a comma. Leave blank to not supply an ad at that breakpoint.<br />Example: 300x600,300x250.</p>'),
    '#prefix' => '<div id="gpt-size">',
    '#suffix' => '</div>',
    '#states' => array(
      'visible' => array(
        'input[id="edit-outofpage"]' => array('checked' => FALSE),
      ),
    ),
  );
  for ($i = 0; $i < $form_state['num_breakpoints']; $i++) {
    $form['size_wrapper'][] = array(
      '#prefix' => '<div class="inline clearfix">',
      '#suffix' => '</div>',
      '#element_validate' => array('gpt_element_validate_handler'),
      '#gpt_validate_handler' => 'gpt_validate_size',
    );
    $form['size_wrapper'][$i]['breakpoint'] = array(
      '#type' => $form_state['num_breakpoints'] === 1 ? 'hidden' : 'textfield',
      '#title' => t('Breakpoint'),
      '#size' => 10,
      '#default_value' => $defaults['size'][$i]['breakpoint'],
      '#disabled' => $i === 0,
    );
    $form['size_wrapper'][$i]['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Size'),
      '#size' => 25,
      '#default_value' => $defaults['size'][$i]['size'],
    );
  }
  // Build add button.
  $form['size_wrapper']['breakpoint_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add breakpoint'),
    '#submit' => array('gpt_submit_handler'),
    '#gpt_submit_handler' => 'gpt_ad_slot_form_breakpoint_add',
    '#attributes' => array('class' => array('gpt-breakpoint-add')),
    '#ajax' => array(
      'callback' => 'gpt_ad_slot_form_size_ajax_callback',
      'wrapper' => 'gpt-size',
    ),
  );

  $form['outofpage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Out of page slot'),
    '#description' => t('<p>Only available to DFP Premium accounts, enable on non-premium accounts at your own risk.</p><p>If checked the creative is loaded as an interstitial and ad slot size settings are ignored.</p>'),
    '#default_value' => $defaults['outofpage'],
  );

  $form['refresh'] = array(
    '#type' => 'checkbox',
    '#title' => t('Refreshable'),
    '#description' => t('<p>Allow the ad to be refreshed; requires aynchronous mode be enabled.</p>'),
    '#default_value' => $defaults['refresh'],
  );

  $form['targeting_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slot-level customized targeting'),
    '#description' => t('Provide key-value pairs for slot-level customized targeting. Multiple values may be associated with one key.'),
    '#prefix' => '<div id="gpt-targeting">',
    '#suffix' => '</div>',
  );
  // Determine if the user is allowed to alter/create Javascript targeting.
  $js_perm = user_access('gpt set javascript');
  for ($i = 0; $i < $form_state['num_targeting_keys']; $i++) {
    $form['targeting_wrapper'][] = array(
      '#prefix' => '<div class="key clearfix">',
      '#suffix' => '</div>',
      '#element_validate' => array('gpt_element_validate_handler'),
      '#gpt_validate_handler' => 'gpt_validate_targeting',
    );
    $form['targeting_wrapper'][$i]['key'] = array(
      '#type' => 'textfield',
      '#title' => t('Key'),
      '#size' => 15,
      '#default_value' => $defaults['targeting'][$i]['key'],
    );
    for ($j = 0; $j < $form_state['num_targeting_values_' . $i]; $j++) {
      $value_wrapper =& $form['targeting_wrapper'][$i]['values'][$j];
      $value_wrapper = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>',
      );
      $value_wrapper['value'] = array(
        '#type' => 'textfield',
        '#title' => t('Value'),
        '#size' => 25,
        '#default_value' => $defaults['targeting'][$i]['values'][$j]['value'],
        '#disabled' => !$js_perm && $defaults['targeting'][$i]['values'][$j]['eval'] ? TRUE : FALSE,
      );
      $value_wrapper['eval'] = array(
        '#type' => 'checkbox',
        '#title' => t('Value is Javascript'),
        '#disabled' => !$js_perm,
        '#default_value' => $defaults['targeting'][$i]['values'][$j]['eval'],
      );
    }
    // Build add button.
    $form['targeting_wrapper'][$i]['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add value'),
      // Without a unique #name value our form will not be able to distinguish
      // which add value button was clicked.
      '#name' => 'add-value-' . $i,
      '#submit' => array('gpt_submit_handler'),
      '#gpt_submit_handler' => 'gpt_ad_slot_form_targeting_value_add',
      '#prefix' => '<div class="row"><div>',
      '#suffix' => '</div></div>',
      '#ajax' => array(
        'callback' => 'gpt_ad_slot_form_targeting_ajax_callback',
        'wrapper' => 'gpt-targeting',
      ),
    );
  }
  // Build add button.
  $form['targeting_wrapper']['targeting_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add key'),
    '#submit' => array('gpt_submit_handler'),
    '#gpt_submit_handler' => 'gpt_ad_slot_form_targeting_key_add',
    '#attributes' => array('class' => array('gpt-targeting-add')),
    '#ajax' => array(
      'callback' => 'gpt_ad_slot_form_targeting_ajax_callback',
      'wrapper' => 'gpt-targeting',
    ),
  );

  return $form;
}

/**
 * Submit handler for the add breakpoint button.
 */
function gpt_ad_slot_form_breakpoint_add($form, &$form_state) {
  $form_state['num_breakpoints']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback to return the size wrapper.
 */
function gpt_ad_slot_form_size_ajax_callback($form, $form_state) {
  return $form['size_wrapper'];
}

/**
 * Slot form submit handler, prepare values for use in parent form.
 */
function gpt_ad_slot_form_submit(&$form, &$form_state) {
  $values = array();
  $values['outofpage'] = $form_state['values']['outofpage'];
  $values['refresh'] = $form_state['values']['refresh'];

  $values['size'] = NULL;
  if (!$form_state['values']['outofpage']) {
    $values['size'] = new stdClass();
    // Look for breakpoints with data.
    $breakpoints = array();
    for ($i = 0; $i < $form_state['num_breakpoints']; $i++) {
      if (strlen($form_state['values']['size_wrapper'][$i]['breakpoint'])) {
        $breakpoints[(string) $form_state['values']['size_wrapper'][$i]['breakpoint']] = $form_state['values']['size_wrapper'][$i]['size'];
      }
    }
    // Sort breakpoints ascending.
    ksort($breakpoints);
    // Assign size settings per breakpoint, rather than auto-increment key.
    foreach ($breakpoints as $breakpoint => $size) {
      $values['size']->$breakpoint = _gpt_ads_size_to_array($size);
    }
  }

  $values['targeting'] = new stdClass();
  for ($i = 0; $i < $form_state['num_targeting_keys']; $i++) {
    if (strlen($form_state['values']['targeting_wrapper'][$i]['key'])) {
      $targeting = array();
      for ($j = 0; $j < $form_state['num_targeting_values_' . $i]; $j++) {
        if (strlen($form_state['values']['targeting_wrapper'][$i]['values'][$j]['value'])) {
          $targeting[] = array(
            'value' => $form_state['values']['targeting_wrapper'][$i]['values'][$j]['value'],
            'eval' => $form_state['values']['targeting_wrapper'][$i]['values'][$j]['eval'],
          );
        }
      }
      $values['targeting']->$form_state['values']['targeting_wrapper'][$i]['key'] = $targeting;
    }
  }

  $form_state['prepared_values'] = $values;
}

/**
 * Ajax callback to return the targeting wrapper.
 */
function gpt_ad_slot_form_targeting_ajax_callback($form, $form_state) {
  return $form['targeting_wrapper'];
}

/**
 * Submit handler for the targeting add key button.
 */
function gpt_ad_slot_form_targeting_key_add($form, &$form_state) {
  // Add a value for the new key.
  $form_state['num_targeting_values_' . $form_state['num_targeting_keys']] = 1;
  // Add the new key.
  $form_state['num_targeting_keys']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the targeting add value button.
 */
function gpt_ad_slot_form_targeting_value_add($form, &$form_state) {
  $form_state['num_targeting_values_' . $form_state['triggering_element']['#parents'][1]]++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Ad manager provider form.
 * 
 * Handles provider form through a subform.
 */
function gpt_ads_form(&$form, &$form_state) {
  $slot_form_values = isset($form_state['item']->settings['gpt_ads']) ?
      $form_state['item']->settings['gpt_ads'] : array();
  $form['slot_form'] = array(
    '#type' => 'subform',
    '#subform_id' => 'gpt_ad_slot_form',
    '#subform_file' => array('module' => 'gpt', 'name' => 'gpt.admin.inc'),
    '#subform_arguments' => array($slot_form_values),
    '#required' => TRUE,
  );
  $form['#buttons']['return']['#submit'][] = 'subform_submit_all';
  // No matter what I do with the subform I can't seem to get it to hit the
  // gpt_ad_slot_form_submit() function through subform automatically.
}

/**
 * Ad Manager ad provider form submit for AdSense Custom Search Ads.
 */
function gpt_ads_submit(&$form, &$form_state) {
  // A key of the provider name is used to prevent conflicting data if users
  // change providers for a specific ad.
  $settings =& $form_state['item']->settings['gpt_ads'];

  // Process subform submit handler.
  // We have to hack in our subform submit handler here, see note in
  // gpt_ads_form().
  $form_state['temporary']['subform']['subform-slot_form']['submit_handlers'] = array(
    'gpt_ad_slot_form_submit',
  );
  subform_submit_all($form, $form_state);

  // Now we use our values to do what we need:
  $values = $form_state['temporary']['subform']['subform-slot_form']['prepared_values'];
  foreach ($values as $key => $value) {
    $settings[$key] = $value;
  }
}

/**
 * Attach our settings form in global mode to the ad manager settings form.
 */
function gpt_global_settings_form(&$form, &$form_state) {
  $form['gpt'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Google Publisher Tags'),
  );
  $form['gpt']['gpt_settings'] = array(
    '#type' => 'subform',
    '#subform_id' => 'gpt_settings_form',
    '#subform_file' => array('module' => 'gpt', 'name' => 'gpt.admin.inc'),
    // We don't need to pass arguments as the subform will populate from global.
    '#subform_arguments' => array(array(), TRUE),
    '#subform_default_triggering_element' => array('submit'),
    '#required' => TRUE,
  );
  // Enable error validation.
  $form['submit']['#submit'][] = 'subform_submit_all';
  $form['submit']['#submit'][] = 'gpt_global_settings_form_submit';
}

/**
 * Global settings for submit handler.
 */
function gpt_global_settings_form_submit(&$form, &$form_state) {
  $values = $form_state['temporary']['subform']['subform-gpt-gpt_settings']['prepared_values'];
  $keys = array('network_code', 'targeted_ad_unit', 'async', 'sra', 'collapse', 'targeting');

  foreach ($keys as $key) {
    variable_set('gpt__' . $key, $values[$key]);
  }
}

/**
 * Set up the global config form.
 */
function gpt_settings_form($form, &$form_state, $settings, $global = FALSE) {
  $defaults = _gpt_settings_form_defaults($settings, $form_state, $global);
  $form_state['global_settings'] = $global;
  _gpt_admin_attach_css();
  $form['#tree'] = TRUE;
  // $form['#submit'][] = 'gpt_settings_form_submit';

  if (!$global) {
    $form['network_code_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override network code'),
      '#default_value' => $defaults['network_code_override'],
    );
  }
  $form['network_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Network code'),
    '#description' => t('<p>The network to use for inventory, can be found in the Admin tab of DFP. Likely a 4 digit number for Premium publishers and a longer number for SB publishers.</p><p>e.g. 1234</p>'),
    '#default_value' => $defaults['network_code'],
    '#required' => $global,
    '#states' => $global ? array() : array(
      'visible' => array(
        'input[id="edit-network-code-override"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (!$global) {
    $form['targeted_ad_unit_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override targeted ad unit'),
      '#default_value' => $defaults['targeted_ad_unit_override'],
    );
  }
  $form['targeted_ad_unit'] = array(
    '#type' => 'textfield',
    '#title' => t('Default targeted ad unit'),
    '#description' => t('<p>The targeted ad unit and, for Premium publishers, the optional hierarchy behind it.</p><p>SB publishers can utilize one (1) level of hierarchy, while Premium publishers may utilize up to five (5) levels of hierarchy. Do not begin or end with a slash.</p><p>e.g. travel or travel/asia/food</p>'),
    '#default_value' => $defaults['targeted_ad_unit'],
    '#states' => $global ? array() : array(
      'visible' => array(
        'input[id="edit-targeted-ad-unit-override"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (!$global) {
    $form['async_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override asynchronous'),
      '#default_value' => $defaults['async_override'],
    );
  }
  $form['async'] = array(
    '#type' => 'checkbox',
    '#title' => t('Asynchronous'),
    '#description' => t('Asynchronous causes non-blocking loading of creatives, option to use Single Request Architecture, and the ability to refresh ads. Some expanding creatives may not expand, if they were not developed as Friendly iFrames.'),
    '#default_value' => $defaults['async'],
    '#states' => $global ? array() : array(
      'visible' => array(
        'input[id="edit-async-override"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (!$global) {
    $form['sra_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override Single Request Architecture (SRA)'),
      '#default_value' => $defaults['sra_override'],
    );
  }
  $form['sra'] = array(
    '#type' => 'checkbox',
    '#title' => t('Single Request Architecture (SRA)'),
    '#description' => t('SRA reduces ad calls to one single request, improving performance of page loads, and also allows guarantee of roadblocks. However ads will not begin loading until all other assets on the page have loaded, and it also prevents new ads being loaded on the page via Javascript or Ajax.'),
    '#default_value' => $defaults['sra'],
    '#states' => $global ? array() : array(
      'visible' => array(
        'input[id="edit-sra-override"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (!$global) {
    $form['collapse_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override collapse empty divs'),
      '#default_value' => $defaults['collapse_override'],
    );
  }
  $form['collapse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapse empty divs'),
    '#description' => t('Collapse empty div elements when there is no creative to display.'),
    '#default_value' => $defaults['collapse'],
    '#states' => $global ? array() : array(
      'visible' => array(
        'input[id="edit-collapse-override"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['targeting_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page-level customized targeting'),
    '#description' => t('<p>Provide key-value pairs for page-level customized targeting. Multiple values may be associated with one key.</p>'),
    '#prefix' => '<div id="gpt-targeting">',
    '#suffix' => '</div>',
  );
  if (!$global) {
    $form['targeting_wrapper']['#description'] .= t('<p>Choosing override will override any page-level customized targeting set elsewhere for the overridden key.</p>');
  }
  // Determine if the user is allowed to alter/create Javascript targeting.
  $js_perm = user_access('gpt set javascript');
  for ($i = 0; $i < $form_state['num_targeting_keys']; $i++) {
    $form['targeting_wrapper'][] = array(
      '#prefix' => '<div class="key clearfix">',
      '#suffix' => '</div>',
      '#element_validate' => array('gpt_element_validate_handler'),
      '#gpt_validate_handler' => 'gpt_validate_targeting',
    );
    $form['targeting_wrapper'][$i]['key'] = array(
      '#type' => 'textfield',
      '#title' => t('Key'),
      '#size' => 15,
      '#default_value' => $defaults['targeting'][$i]['key'],
    );
    if (!$global) {
      $form['targeting_wrapper'][$i]['method'] = array(
        '#type' => 'radios',
        '#title' => t('Method'),
        '#prefix' => '<div class="row inline clearfix">',
        '#suffix' => '</div>',
        '#options' => array(
          0 => t('Append'),
          1 => t('Override'),
        ),
        '#default_value' => $defaults['targeting'][$i]['method'],
      );
    }
    for ($j = 0; $j < $form_state['num_targeting_values_' . $i]; $j++) {
      $value_wrapper =& $form['targeting_wrapper'][$i]['values'][$j];
      $value_wrapper = array(
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>',
      );
      $value_wrapper['value'] = array(
        '#type' => 'textfield',
        '#title' => t('Value'),
        '#size' => 25,
        '#default_value' => $defaults['targeting'][$i]['values'][$j]['value'],
        '#disabled' => !$js_perm && $defaults['targeting'][$i]['values'][$j]['eval'] ? TRUE : FALSE,
      );
      $value_wrapper['eval'] = array(
        '#type' => 'checkbox',
        '#title' => t('Value is Javascript'),
        '#disabled' => !$js_perm,
        '#default_value' => $defaults['targeting'][$i]['values'][$j]['eval'],
      );
    }
    // Build add button.
    $form['targeting_wrapper'][$i]['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add value'),
      // Without a unique #name value our form will not be able to distinguish
      // which add value button was clicked.
      '#name' => 'add-value-' . $i,
      '#submit' => array('gpt_submit_handler'),
      '#gpt_submit_handler' => 'gpt_settings_form_targeting_value_add',
      '#prefix' => '<div class="row"><div>',
      '#suffix' => '</div></div>',
      '#ajax' => array(
        'callback' => 'gpt_settings_form_targeting_ajax_callback',
        'wrapper' => 'gpt-targeting',
      ),
    );
  }
  // Build add button.
  $form['targeting_wrapper']['targeting_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add key'),
    '#submit' => array('gpt_submit_handler'),
    '#gpt_submit_handler' => 'gpt_settings_form_targeting_key_add',
    '#attributes' => array('class' => array('gpt-targeting-add')),
    '#ajax' => array(
      'callback' => 'gpt_settings_form_targeting_ajax_callback',
      'wrapper' => 'gpt-targeting',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#submit' => array('gpt_settings_form_submit'),
    '#access' => FALSE,
  );

  return $form;
}

/**
 * Global config form submission handler.
 */
function gpt_settings_form_submit(&$form, &$form_state) {
  $values = array();
  $keys = array('network_code', 'targeted_ad_unit', 'async', 'sra', 'collapse');

  foreach ($keys as $key) {
    if (!$form_state['global_settings']) {
      // Pass back null if not overridden.
      if ($form_state['values'][$key . '_override']) {
        $values[$key] = $form_state['values'][$key];
      }
      else {
        $values[$key] = NULL;
      }
    }
    else {
      $values[$key] = $form_state['values'][$key];
    }
  }

  // Targeting
  $values['targeting'] = new stdClass();
  for ($i = 0; $i < $form_state['num_targeting_keys']; $i++) {
    if (strlen($form_state['values']['targeting_wrapper'][$i]['key'])) {
      $targeting = array();
      if (!$form_state['global_settings']) {
        $targeting['method'] = $form_state['values']['targeting_wrapper'][$i]['method'];
      }
      for ($j = 0; $j < $form_state['num_targeting_values_' . $i]; $j++) {
        if (strlen($form_state['values']['targeting_wrapper'][$i]['values'][$j]['value'])) {
          $targeting[] = array(
            'value' => $form_state['values']['targeting_wrapper'][$i]['values'][$j]['value'],
            'eval' => $form_state['values']['targeting_wrapper'][$i]['values'][$j]['eval'],
          );
        }
      }
      $values['targeting']->$form_state['values']['targeting_wrapper'][$i]['key'] = $targeting;
    }
  }

  $form_state['prepared_values'] = $values;
}

/**
 * Ajax callback to return the targeting wrapper.
 */
function gpt_settings_form_targeting_ajax_callback($form, $form_state) {
  return $form['targeting_wrapper'];
}

/**
 * Submit handler for the targeting add key button.
 */
function gpt_settings_form_targeting_key_add($form, &$form_state) {
  // Add a value for the new key.
  $form_state['num_targeting_values_' . $form_state['num_targeting_keys']] = 1;
  // Add the new key.
  $form_state['num_targeting_keys']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the targeting add value button.
 */
function gpt_settings_form_targeting_value_add($form, &$form_state) {
  $form_state['num_targeting_values_' . $form_state['triggering_element']['#parents'][1]]++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Element validate size and breakpoint fields.
 *
 * @param array $element
 * @param array $form_state
 * @param array $form
 *
 * @return void
 */
function gpt_validate_size($element, &$form_state, $form) {
  _gpt_admin_attach_css();

  // If out of page is set, don't bother validating.
  if ($form_state['values']['outofpage']) {
    return;
  }

  // $breakpoints stores the breakpoints submitted. Used to flag duplicate
  // breakpoints.
  $breakpoints = &drupal_static(__FUNCTION__);
  if (!isset($breakpoints)) {
    foreach ($form_state['values']['size_wrapper'] as $sizes) {
      $breakpoints[] = $sizes['breakpoint'];
    }
  }

  // Retrieve breakpoint and size, strip spaces from both.
  $breakpoint =& $form_state['values']['size_wrapper'][$element['#parents'][1]]['breakpoint'];
  $breakpoint = str_replace(' ', '', $breakpoint);
  $size =& $form_state['values']['size_wrapper'][$element['#parents'][1]]['size'];
  $size = str_replace(' ', '', $size);

  // Validate breakpoint.
  if (strlen($breakpoint)) {
    // Invalid value supplied.
    if ($breakpoint !== '0' && !preg_match('#^[1-9][0-9]*$#', $breakpoint)) {
      form_error($element['breakpoint'], t('Invalid value supplied for breakpoint, must be a positive integer.'));
    }
    // Breakpoint supplied more than one time.
    elseif (count(array_keys($breakpoints, $breakpoint)) > 1) {
      form_error($element['breakpoint'], t('Duplicate assignment of breakpoint %breakpoint.', array('%breakpoint' => $breakpoint)));
    }
  }

  // Validate size.
  if (strlen($size)) {
    // If breakpoint is not set for the size.
    if (!strlen($breakpoint)) {
      form_error($element, t('A breakpoint must be defined for size %size.', array('%size' => $size)));
    }
    elseif (!preg_match('#^[1-9][0-9]*x[1-9][0-9]*(?:,[1-9][0-9]*x[1-9][0-9]*)*$#', $size)) {
      form_error($element['size'], t('Size %size does not match the expected format.', array('%size' => $size)));
    }
  }
}

/**
 * Element validate callback for targeting fields.
 *
 * @param array $element
 * @param array $form_state
 * @param array $form
 *
 * @return void
 */
function gpt_validate_targeting($element, &$form_state, $form) {
  _gpt_admin_attach_css();

  // $keys stores the keys submitted. Used to flag duplicate keys.
  $keys = &drupal_static(__FUNCTION__);
  if (!isset($keys)) {
    foreach ($form_state['values']['targeting_wrapper'] as $k => $targeting) {
      if ($k !== 'targeting_add') {
        $keys[] = trim($targeting['key']);
      }
    }
  }

  $key = trim($element['key']['#value']);

  if (strlen($key) && count(array_keys($keys, $key)) > 1) {
    form_error($element['key'], t('Duplicate assignment of key %key.', array('%key' => $key)));
  }
  else {
    $value_set = FALSE;
    foreach ($form_state['values']['targeting_wrapper'][$element['#parents'][1]]['values'] as $values) {
      $value =& $values['value'];
      $value = trim($value);

      if (strlen($value)) {
        $value_set = TRUE;

        if (!strlen($key)) {
          form_error($element['key'], t('Targeting key for value %value has not been set.', array('%value' => $value)));
        }
      }
    }
    if (!$value_set && strlen($key)) {
      form_error($element['values'][0]['value'], t('A targeting value for key %key has not been set.', array('%key' => $key)));
    }
  }
}

/**
 * Prepare default values for ad slot form.
 */
function _gpt_ad_slot_form_defaults(&$s, &$form_state) {
  $d = array();

  // Build defaults from inserted values.
  if (isset($form_state['values'])) {
    $v = $form_state['values'];

    // Size values
    $d['size'] = array();
    for ($i = 0; $i < $form_state['num_breakpoints']; $i++) {
      $value = isset($v['size_wrapper'][$i]) ? $v['size_wrapper'][$i] : FALSE;
      $d['size'][] = array(
        'breakpoint' => $value !== FALSE ? $value['breakpoint'] : '',
        'size' => $value !== FALSE ? $value['size'] : '',
      );
    }

    // Targeting values
    $d['targeting'] = array();
    for ($i = 0; $i < $form_state['num_targeting_keys']; $i++) {
      $value = isset($v['targeting_wrapper'][$i]) ? $v['targeting_wrapper'][$i] : FALSE;
      $d['targeting'][$i] = array(
        'key' => $value !== FALSE ? $value['key'] : '',
        'values' => array(),
      );
      for ($j = 0; $j < $form_state['num_targeting_values_' . $i]; $j++) {
        $value = isset($v['targeting_wrapper'][$i]['values'][$j]) ? $v['targeting_wrapper'][$i]['values'][$j] : FALSE;
        $d['targeting'][$i]['values'][$j] = array(
          'value' => $value !== FALSE ? $value['value'] : '',
          'eval' => $value !== FALSE ? $value['eval'] : 0,
        );
      }
    }

    // Out of page
    $d['outofpage'] = $v['outofpage'];

    // Refresh
    $d['refresh'] = $v['refresh'];
  }
  // Else, if available, build defaults from stored settings.
  elseif (!empty($s)) {
    $d = $s;

    // Convert sizes from object to indexed array.
    $sizes = array();
    foreach (get_object_vars($d['size']) as $breakpoint => $size) {
      $sizes[] = array(
        'breakpoint' => $breakpoint,
        'size' => _gpt_ads_size_to_friendly($size),
      );
    }
    $d['size'] = $sizes;
    // Update sizes to friendly from array.
    $form_state['num_breakpoints'] = count($sizes) ? count($sizes) : 1;

    // Convert targeting from object to indexed array.
    $targeting = array();
    $count = 0;
    $form_state['num_targeting_values_0'] = 1;
    foreach (get_object_vars($d['targeting']) as $key => $values) {
      $targeting[] = array(
        'key' => $key,
        'values' => $values,
      );
      $form_state['num_targeting_values_' . $count] = count($values) ? count($values) : 1;
      $count++;
    }
    $d['targeting'] = $targeting;
    // Initialize from scratch
    if (!$count) {
      $form_state['num_targeting_keys'] = 1;
      $form_state['num_targeting_values_0'] = 1;
    }
    else {
      $form_state['num_targeting_keys'] = $count;
    }
  }
  // Else set defaults from scratch.
  else {
    $d['size'] = array(
      array(
        'breakpoint' => '0',
        'size' => '',
      ),
    );
    $d['targeting'] = array(
      array(
        'key' => '',
        'values' => array(
          array(
            'value' => '',
            'eval' => 0,
          ),
        ),
      ),
    );
    $d['outofpage'] = 0;
    $d['refresh'] = 1;

    // Set our index trackers for the number of units.
    $form_state['num_breakpoints'] = 1;
    $form_state['num_targeting_keys'] = 1;
    $form_state['num_targeting_values_0'] = 1;
  }

  return $d;
}

/**
 * Helper to attach admin CSS once per page load.
 */
function _gpt_admin_attach_css() {
  static $loaded = FALSE;
  if (!$loaded) {
    drupal_add_css(drupal_get_path('module', 'gpt') . '/css/admin.css', array('preprocess' => FALSE));
    $loaded = TRUE;
  }
}

/**
 * Convert human friendly width by height string to size array for GPT.
 *
 * @param string|NULL $string
 *
 * @return array|void
 */
function _gpt_ads_size_to_array($string = NULL) {
  $string = str_replace(' ', '', $string);
  if (empty($string)) {
    return;
  }

  $sizes = explode(',', $string);
  foreach ($sizes as &$size) {
    $size = explode('x', $size);
    foreach ($size as &$s) {
      $s = (int) $s;
    }
  }

  return $sizes;
}

/**
 * Convert size array to more human friendly width by height format.
 *
 * @param array|NULL $sizes
 *
 * @return string|void
 */
function _gpt_ads_size_to_friendly($sizes = NULL) {
  if (empty($sizes)) {
    return;
  }

  // If size values are set.
  if (isset($sizes[0])) {
    if (is_array($sizes[0])) {
      $return = array();
      foreach ($sizes as $size) {
        $return[] = $size[0] . 'x' . $size[1];
      }
      return implode(',', $return);
    }

    return $sizes[0] . 'x' . $sizes[1];
  }
}

/**
 * Prepare default values for settings form.
 */
function _gpt_settings_form_defaults(&$s, &$form_state, $global) {
  // Initialize data structure.
  $d = array(
    'network_code' => variable_get('gpt__network_code', ''),
    'targeted_ad_unit' => variable_get('gpt__targeted_ad_unit', ''),
    'async' => variable_get('gpt__async', 1),
    'sra' => variable_get('gpt__sra', 0),
    'collapse' => variable_get('gpt__collapse', 0),
    // We don't variable_get targeting as it is not displayed on non-global
    // forms like the other data is.
    'targeting' => array(),
  );

  // Get keys array for use assigning basic variables.
  $keys = array_keys($d);
  // We don't want to include targeting in the basic assignments.
  unset($keys[5]);

  if (!$global) {
    foreach ($keys as $key) {
      $d[$key . '_override'] = 0;
    }
  }

  // Build defaults from inserted values.
  if (isset($form_state['values'])) {
    foreach ($keys as $key) {
      $d[$key] = $form_state['values'][$key];
    }
    // Targeting values
    $d['targeting'] = array();
    for ($i = 0; $i < $form_state['num_targeting_keys']; $i++) {
      $value = isset($v['targeting_wrapper'][$i]) ? $v['targeting_wrapper'][$i] : FALSE;
      $d['targeting'][$i] = array(
        'key' => $value !== FALSE ? $value['key'] : '',
        'values' => array(),
      );
      if (!$global) {
        $d['targeting'][$i]['method'] = $value !== FALSE ? $value['method'] : 0;
      }
      for ($j = 0; $j < $form_state['num_targeting_values_' . $i]; $j++) {
        $value = isset($v['targeting_wrapper'][$i]['values'][$j]) ? $v['targeting_wrapper'][$i]['values'][$j] : FALSE;
        $d['targeting'][$i]['values'][$j] = array(
          'value' => $value !== FALSE ? $value['value'] : '',
          'eval' => $value !== FALSE ? $value['eval'] : 0,
        );
      }
    }
  }
  // Else, if available, build defaults from stored settings.
  elseif (!empty($s)) {
    foreach ($keys as $key) {
      if (isset($s[$key])) {
        $d[$key] = $s[$key];
        $d[$key . '_override'] = 1;
      }
    }

    if (isset($s['targeting'])) {
      $d['targeting'] = array();
      // Convert targeting from object to indexed array.
      $targeting = array();
      foreach (get_object_vars($s['targeting']) as $key => $values) {
        $target = array();
        if (isset($values['method'])) {
          $target['method'] = $values['method'];
          unset($values['method']);
        }
        $target['key'] = $key;
        $target['values'] = $values;
        $targeting[] = $target;
      }
      $d['targeting'] = $targeting;
    }
  }
  // Elseif this is for a global config, load targeting from variable_get().
  elseif ($global) {
    $targeting = array();
    foreach (get_object_vars(variable_get('gpt__targeting', array())) as $key => $values) {
      $targeting[] = array(
        'key' => $key,
        'values' => $values,
      );
    }
    $d['targeting'] = $targeting;
  }

  // Provide default targeting.
  if (empty($d['targeting'])) {
    $d['targeting'] = array(
      array(
        'key' => '',
        'method' => 0,
        'values' => array(
          array(
            'value' => '',
            'eval' => 0,
          ),
        ),
      ),
    );
    if (!$global) {
      $d['targeting'][0]['method'] = 0;
    }
  }

  // Determine targeting key and value indexes if not yet set.
  if (!isset($form_state['num_targeting_keys'])) {
    for ($i = 0; $i < count($d['targeting']); $i++) {
      $form_state['num_targeting_values_' . $i] = count($d['targeting'][$i]['values']) ? count($d['targeting'][$i]['values']) : 1;
    }
    $form_state['num_targeting_keys'] = $i;
  }

  return $d;
}
