<?php
/**
 * @file
 * MMS admin page callbacks.
 */

/**
 * mms_entry_form().
 *
 * This form allows to configure how MMS widget works.
 */
function mms_entry_form($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.conf');
  module_load_include('inc', 'mms', 'mms.data');

  // SELECTION
  $form['selection'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('selection_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['selection']['markup'] = array(
    '#markup' => _mms_admin_data('selection_markup'),
  );
  $form['selection'][MMS_SELECTION_FOLLOW] = array(
    '#type' => 'radios',
    '#title' => _mms_admin_data('selection_follow_title'),
    '#description' => _mms_admin_data('selection_follow_description'),
    '#options' => array(
      _mms_admin_data('follow_false'),
      _mms_admin_data('follow_true'),
    ),
    '#default_value' => _mms_conf(MMS_SELECTION_FOLLOW),
  );

  // MULTIFY
  $form['multify'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('multify_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['multify']['markup'] = array(
    '#markup' => _mms_admin_data('multify_markup'),
  );
  $form['multify'][MMS_MULTIFY_FROMLANG] = array(
    '#type' => 'radios',
    '#title' => _mms_admin_data('multify_fromlang_title'),
    '#description' => _mms_admin_data('multify_fromlang_description'),
    '#options' => array(
      'default' => _mms_admin_data('multify_default'),
      'current' => _mms_admin_data('multify_current'),
      'ask' => _mms_admin_data('multify_ask'),
    ),
    '#default_value' => _mms_conf(MMS_MULTIFY_FROMLANG),
  );
  $form['multify'][MMS_MULTIFY_BACKLANG] = array(
    '#type' => 'radios',
    '#title' => _mms_admin_data('multify_backlang_title'),
    '#description' => _mms_admin_data('multify_backlang_description'),
    '#options' => array(
      'default' => _mms_admin_data('multify_default'),
      'current' => _mms_admin_data('multify_current'),
      'ask' => _mms_admin_data('multify_ask'),
    ),
    '#default_value' => _mms_conf(MMS_MULTIFY_BACKLANG),
  );

  // WIDGET
  $form['widget'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('widget_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['widget']['markup'] = array(
    '#markup' => _mms_admin_data('widget_markup'),
  );
  $form['widget'][MMS_NO_WIDGET] = array(
    '#type' => 'textarea',
    '#title' => _mms_admin_data('widget_no_widget_title'),
    '#description' => _mms_admin_data('widget_no_widget_description'),
    '#default_value' => _mms_conf(MMS_NO_WIDGET),
  );

  return system_settings_form($form);
}
/**
 * mms_entry_form_validate().
 */
function mms_entry_form_validate($form, &$form_state) {
  // Actually format values as arrays.
  foreach ($form_state['values'] as $index => $value) {
    $form_state['values'][$index] =
      _mms_conf_set($form_state['values'][$index]);
  }
}

/**
 * mms_rendering_form().
 *
 * This form allows to configure how MMS renders the "multi" segments
 * found in pages.
 */
function mms_rendering_form($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.conf');
  module_load_include('inc', 'mms', 'mms.data');

  // FALLBACK
  $form['fallback'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('fallback_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['fallback']['markup'] = array(
    '#markup' => _mms_admin_data('fallback_markup'),
  );
  $form['fallback'][MMS_FALLBACK_EMPTY] = array(
    '#type' => 'radios',
    '#title' => _mms_admin_data('fallback_empty_title'),
    '#description' => _mms_admin_data('fallback_empty_description'),
    '#options' => array(
      'replace' => _mms_admin_data('fallback_empty_replace'),
      'empty' => _mms_admin_data('fallback_empty_empty'),
    ),
    '#default_value' => _mms_conf(MMS_FALLBACK_EMPTY),
  );
  $form['fallback'][MMS_FALLBACK_OPTION] = array(
    '#type' => 'radios',
    '#title' => _mms_admin_data('fallback_option_title'),
    '#description' => _mms_admin_data('fallback_option_description'),
    '#options' => array(
      'default' => _mms_admin_data('fallback_option_default'),
      'message' => _mms_admin_data('fallback_option_message'),
    ),
    '#default_value' => _mms_conf(MMS_FALLBACK_OPTION),
  );
  $form['fallback'][MMS_FALLBACK_MESSAGE] = array(
    '#type' => 'textfield',
    '#title' => _mms_admin_data('fallback_message_title'),
    '#states' => array(
      'visible' => array(
        ':input[name=mms_fallback_option]' => array('value' => 'message')
      ),
    ),
    '#description' => _mms_admin_data('fallback_message_description'),
    '#default_value' => _mms_conf(MMS_FALLBACK_MESSAGE),
  );
  $form['fallback'][MMS_FALLBACK_TIP] = array(
    '#type' => 'textfield',
    '#title' => _mms_admin_data('fallback_tip_title'),
    '#description' => _mms_admin_data('fallback_tip_description'),
    '#default_value' => _mms_conf(MMS_FALLBACK_TIP),
  );
  $form['fallback'][MMS_FALLBACK_CSS] = array(
    '#type' => 'textarea',
    '#title' => _mms_admin_data('fallback_css_title'),
    '#description' => _mms_admin_data('fallback_css_description'),
    '#default_value' => _mms_conf(MMS_FALLBACK_CSS),
  );

  // PAGES
  $form['pages'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('pages_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['pages']['markup'] = array(
    '#markup' => _mms_admin_data('pages_markup'),
  );
  $form['pages'][MMS_EXCLUDE_PATHS] = array(
    '#type' => 'textarea',
    '#title' => _mms_admin_data('pages_paths_title'),
    '#description' => _mms_admin_data('pages_paths_description'),
    '#default_value' => _mms_conf(MMS_EXCLUDE_PATHS),
  );

  // LINEBREAKS
  $form['linebreaks'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('linebreaks_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['linebreaks']['use'] = array(
    '#markup' => _mms_admin_data('linebreaks_markup'),
  );
  $form['linebreaks'][MMS_LINEBREAKS_USE] = array(
    '#type' => 'textarea',
    '#title' => _mms_admin_data('linebreaks_use_title'),
    '#description' => _mms_admin_data('linebreaks_use_description'),
    '#default_value' => _mms_conf(MMS_LINEBREAKS_USE),
  );

  return system_settings_form($form);
}
/**
 * mms_rendering_form_validate().
 */
function mms_rendering_form_validate($form, &$form_state) {
  // Actually format values as arrays.
  foreach ($form_state['values'] as $index => $value) {
    $form_state['values'][$index] =
      _mms_conf_set($form_state['values'][$index]);
  }
}

/**
 * mms_tokens_form().
 *
 * This form allows to point out the [type:token] tokens that MMS will
 * clone into [type:mms-token] and [type:mms-native-token].
 */
function mms_tokens_form($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.conf');
  module_load_include('inc', 'mms', 'mms.data');

  // Prepare tokens.
  $tokens = _mms_conf(MMS_TOKENS, FALSE);
  $raw_tokens = array(); // (force array rather than null if empty)
  foreach ($tokens as $type => $token_list) {
    foreach ($token_list as $token) {
      $raw_tokens[] = $type . ':' . $token;
    }
  }
  sort($raw_tokens);

  // Tokens.
  $form['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('tokens_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tokens']['markup'] = array(
    '#markup' => _mms_admin_data('tokens_markup'),
  );
  $form['tokens'][MMS_TOKENS] = array(
    '#type' => 'textarea',
    '#title' => _mms_admin_data('tokens_tokens_title'),
    '#description' => _mms_admin_data('tokens_tokens_description'),
    '#default_value' => implode("\n", $raw_tokens),
  );
  
  // Prepare field types.
  foreach (field_info_field_types() as $field_type => $field_info) {
    $options[$field_type] = array(
      'type' => $field_type,
      'label' => $field_info['label'],
      'description' => $field_info['description'],
      'module' => $field_info['module'],
    );
  }

  // Token field types.
  $form['field_types'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('field_types_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['field_types']['markup'] = array(
    '#markup' => _mms_admin_data('field_types_markup'),
  );
  $form['field_types'][MMS_FIELD_TYPES] = array(
    '#type' => 'tableselect',
    '#header' => array(
      'type' => 'Type',
      'label' => 'Label',
      'description' => 'Description',
      'module' => 'Module',
    ),
    '#options' => $options,
    '#default_value' => _mms_conf(MMS_FIELD_TYPES, FALSE),
    '#js_select' => FALSE,
  );

  return system_settings_form($form);
}

/**
 * mms_tokens_form_validate().
 */
function mms_tokens_form_validate($form, &$form_state){
  // Format actual tokens.
  $tokens = array(); // (force array rather than null if empty)
  if ($raw_tokens =
    _mms_conf_set($form_state['values'][MMS_TOKENS])
  ) {
    sort($raw_tokens);
    foreach ($raw_tokens as $token) {
      if(preg_match('`^\[?([-_a-z]+):([-_a-z]+)\]?$`', $token, $matches)) {
        // Valid token, register it.
        $tokens[$matches[1]][] = $matches[2];
      } else {
        if ($token) { // (or empty line, merely skip)
          // Invalid token, fire an error message and lock submission.
          $wrong_tokens[] = $token;
        }
      }
    }
  }
  if (@$wrong_tokens) {
    form_set_error(
      MMS_TOKENS, 'Malformed token(s): ' . implode(', ', $wrong_tokens));
  } else {
    $form_state['values'][MMS_TOKENS] = $tokens;
  }

  // Format actual field_types.
  $form_state['values'][MMS_FIELD_TYPES] =
    array_filter($form_state['values'][MMS_FIELD_TYPES]);
}

/**
 * Handling reset-to-default actions.
 *
 * This form allows to reset some or all of the configuration options back to
 * their default values.
 */
function mms_default_form($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.conf');
  module_load_include('inc', 'mms', 'mms.data');

  $form['default']['markup'] = array(
    '#markup' => _mms_admin_data('default_markup'),
  );
  
  $form['default'] = array(
    '#type' => 'checkboxes',
    '#title' => _mms_admin_data('default_title'),
    '#description' => _mms_admin_data('default_description'),
    '#options' => array(
      MMS_SELECTION_FOLLOW => _mms_default_set_title(
        'selection_follow_title', 'menu_entry_title', 'selection_title'),
      MMS_MULTIFY_FROMLANG => _mms_default_set_title(
        'multify_fromlang_title', 'menu_entry_title', 'multify_title'),
      MMS_MULTIFY_BACKLANG => _mms_default_set_title(
        'multify_backlang_title', 'menu_entry_title', 'multify_title'),
      MMS_NO_WIDGET => _mms_default_set_title(
        'widget_no_widget_title', 'menu_entry_title', 'widget_title'),
      MMS_FALLBACK_EMPTY => _mms_default_set_title(
        'fallback_empty_title', 'menu_rendering_title', 'fallback_title'),
      MMS_FALLBACK_OPTION => _mms_default_set_title(
        'fallback_option_title', 'menu_rendering_title', 'fallback_title'),
      MMS_FALLBACK_MESSAGE => _mms_default_set_title(
        'fallback_message_title', 'menu_rendering_title', 'fallback_title'),
      MMS_FALLBACK_TIP =>_mms_default_set_title(
        'fallback_tip_title', 'menu_rendering_title', 'fallback_title'),
      MMS_FALLBACK_CSS => _mms_default_set_title(
        'fallback_css_title', 'menu_rendering_title', 'fallback_title'),
      MMS_EXCLUDE_PATHS => _mms_default_set_title(
        'pages_paths_title', 'menu_rendering_title', 'pages_title'),
      MMS_LINEBREAKS_USE => _mms_default_set_title(
        'linebreaks_use_title', 'menu_rendering_title', 'linebreaks_title'),
      MMS_TOKENS => _mms_default_set_title(
        'tokens_tokens_title', 'menu_tokens_title', 'tokens_title'),
      MMS_FIELD_TYPES => _mms_default_set_title(
        'field_types_title', 'menu_tokens_title', 'field_types_title'),
    ),
    '#default_value' => array(),
  );
  
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => _mms_admin_data('default_submit_button'),
  );

  return $form;
}
function mms_default_form_submit($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.data');
  if ($categories = array_filter($form_state['values']['default'])) {
    foreach ($categories as $category) {
      $operations[] = array('mms_default_process', array($category));
    }
    batch_set(array(
      'title' => _mms_admin_data('default_submit_title'),
      'init_message' => _mms_admin_data('default_submit_init') .
        ' ' . implode(', ', $categories ),
      'operations' => $operations,
      'finished' => 'mms_default_finished',
      'file' => drupal_get_path('module', 'mms') . '/mms.admin.inc',
    ));
  } else {
    drupal_set_message(_mms_admin_data('default_submit_none'), 'warning');
  }
}
function mms_default_process($category, &$context) {
  // Build the name of the function that gets default value for $category.
  $default = '_' . $category . '_default';
  // Reset $category to its default values.
  module_load_include('inc', 'mms', 'mms.conf');
  variable_set($category, $default());
  $context['results'][] = $category;
}
function mms_default_finished($success, $results, $operations) {
  module_load_include('inc', 'mms', 'mms.data');
  if ($results) {
    // List categories which have been reset.
    drupal_set_message(
      _mms_admin_data('default_finished_ok') .
      ' ' . implode(', ', $results) . '.'
    );
  }
  if ($operations) {
    // List categories remaining to process.
    foreach ($operations as $operation) {
      $remaining[] = end($operation);
    }
    drupal_set_message(
      _mms_admin_data('default_finished_error') .
      ' ' . implode(', ', $remaining) . '.',
      'error'
    );
  }
}

function _mms_default_set_title($option_name, $tab_name, $group_name) {
  return
    _mms_admin_data($option_name) .
    ' [ <em>' . _mms_menu_data($tab_name) .
    '</em> | <em>'. _mms_admin_data($group_name) . '</em> ]';
}
