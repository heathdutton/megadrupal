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
  
  // UIOPTIONS
  $form['uioptions'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('uioptions_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['uioptions']['markup'] = array(
    '#markup' => _mms_help(NULL, 'advanced-entry-iu'),
  );
  $form['uioptions'][MMS_UIOPTIONS_FOLLOW] = array(
    '#type' => 'radios',
    '#title' => _mms_admin_data('uioptions_follow_title'),
    '#description' => _mms_admin_data('uioptions_follow_description'),
    '#options' => array(
      _mms_admin_data('follow_false'),
      _mms_admin_data('follow_true'),
    ),
    '#default_value' => _mms_conf(MMS_UIOPTIONS_FOLLOW),
  );
  $form['uioptions'][MMS_UIOPTIONS_RAW] = array(
    '#type' => 'radios',
    '#title' => _mms_admin_data('uioptions_raw_title'),
    '#description' => _mms_admin_data('uioptions_raw_description'),
    '#options' => array(
      _mms_admin_data('raw_false'),
      _mms_admin_data('raw_true'),
    ),
    '#default_value' => _mms_conf(MMS_UIOPTIONS_RAW),
  );

  // MULTIFY
  $form['multify'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('multify_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['multify']['markup'] = array(
    '#markup' => _mms_help('multify_markup', 'advanced-entry-multi'),
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
    '#markup' => _mms_help('widget_markup', 'advanced-entry-widget'),
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
    '#markup' => _mms_help('fallback_markup', 'advanced-rendering-fallback'),
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
    '#markup' => _mms_help('pages_markup', 'advanced-rendering-pages'),
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
    '#markup' => _mms_help('linebreaks_markup', 'advanced-rendering-linebreaks'),
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
    '#markup' => _mms_help('tokens_markup', 'advanced-tokens-raw'),
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
    '#markup' => _mms_help('field_types_markup', 'advanced-tokens-fields'),
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
 * mms_reset_form.
 *
 * This form allows to reset some or all of the configuration options back to
 * their default values.
 */
function mms_reset_form($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.conf');
  module_load_include('inc', 'mms', 'mms.data');
  
  $form['reset'] = array(
    '#type' => 'checkboxes',
    '#title' => _mms_admin_data('reset_title'),
    '#description' => _mms_admin_data('reset_description'),
    '#options' => array(
      MMS_UIOPTIONS_FOLLOW => _mms_reset_build_title(
        'uioptions_follow_title', 'menu_entry_title', 'uioptions_title'),
      MMS_UIOPTIONS_RAW => _mms_reset_build_title(
        'uioptions_raw_title', 'menu_entry_title', 'uioptions_title'),
      MMS_MULTIFY_FROMLANG => _mms_reset_build_title(
        'multify_fromlang_title', 'menu_entry_title', 'multify_title'),
      MMS_MULTIFY_BACKLANG => _mms_reset_build_title(
        'multify_backlang_title', 'menu_entry_title', 'multify_title'),
      MMS_NO_WIDGET => _mms_reset_build_title(
        'widget_no_widget_title', 'menu_entry_title', 'widget_title'),
      MMS_FALLBACK_EMPTY => _mms_reset_build_title(
        'fallback_empty_title', 'menu_rendering_title', 'fallback_title'),
      MMS_FALLBACK_OPTION => _mms_reset_build_title(
        'fallback_option_title', 'menu_rendering_title', 'fallback_title'),
      MMS_FALLBACK_MESSAGE => _mms_reset_build_title(
        'fallback_message_title', 'menu_rendering_title', 'fallback_title'),
      MMS_FALLBACK_TIP =>_mms_reset_build_title(
        'fallback_tip_title', 'menu_rendering_title', 'fallback_title'),
      MMS_FALLBACK_CSS => _mms_reset_build_title(
        'fallback_css_title', 'menu_rendering_title', 'fallback_title'),
      MMS_EXCLUDE_PATHS => _mms_reset_build_title(
        'pages_paths_title', 'menu_rendering_title', 'pages_title'),
      MMS_LINEBREAKS_USE => _mms_reset_build_title(
        'linebreaks_use_title', 'menu_rendering_title', 'linebreaks_title'),
      MMS_TOKENS => _mms_reset_build_title(
        'tokens_tokens_title', 'menu_tokens_title', 'tokens_title'),
      MMS_FIELD_TYPES => _mms_reset_build_title(
        'field_types_title', 'menu_tokens_title', 'field_types_title'),
    ),
    '#default_value' => array(),
  );
  
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => _mms_admin_data('reset_submit_button'),
  );

  return $form;
}
function mms_reset_form_submit($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.data');
  if ($categories = array_filter($form_state['values']['reset'])) {
    foreach ($categories as $category) {
      $operations[] = array('mms_reset_process', array($category));
    }
    batch_set(array(
      'title' => _mms_admin_data('reset_submit_title'),
      'init_message' => _mms_admin_data('reset_submit_init') .
        ' ' . implode(', ', $categories ),
      'operations' => $operations,
      'finished' => 'mms_reset_finished',
      'file' => drupal_get_path('module', 'mms') . '/mms.admin.inc',
    ));
  } else {
    drupal_set_message(_mms_admin_data('reset_submit_none'), 'warning');
  }
}
function mms_reset_process($category, &$context) {
  // Build the name of the function that gets default value for $category.
  $reset = '_' . $category . '_default';
  // Reset $category to its default values.
  module_load_include('inc', 'mms', 'mms.conf');
  variable_set($category, $reset());
  $context['results'][] = $category;
}
function mms_reset_finished($success, $results, $operations) {
  module_load_include('inc', 'mms', 'mms.data');
  if ($results) {
    // List categories which have been reset.
    drupal_set_message(
      _mms_admin_data('reset_finished_ok') .
      ' ' . implode(', ', $results) . '.'
    );
  }
  if ($operations) {
    // List categories remaining to process.
    foreach ($operations as $operation) {
      $remaining[] = end($operation);
    }
    drupal_set_message(
      _mms_admin_data('reset_finished_error') .
      ' ' . implode(', ', $remaining) . '.',
      'error'
    );
  }
}
function _mms_reset_build_title($option_name, $tab_name, $group_name) {
  return
    _mms_admin_data($option_name) .
    ' [ <em>' . _mms_menu_data($tab_name) .
    '</em> | <em>'. _mms_admin_data($group_name) . '</em> ]';
}

/**
 * mms_uitranslate_form.
 *
 * This form allows to import/export MMS UI translations to/from dedicated nodes.
 */
function mms_uitranslate_form($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.conf');
  module_load_include('inc', 'mms', 'mms.data');
  module_load_include('inc', 'mms', 'mms.core');
  module_load_include('inc', 'mms', 'mms.ui');
  
  $form['uitranslate'] = array(
    '#type' => 'fieldset',
    '#title' => _mms_admin_data('uitranslate_title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['uitranslate']['markup'] = array(
    '#markup' => format_string(
      _mms_help('uitranslate_markup', 'advanced-translate'),
      ['!languageAddLink' => _mms_url('admin/config/regional/language/add')]
    ),
  );
  $options['export'] = _mms_admin_data('uitranslate_export');
  if (variable_get(MMS_UI_NODES)) {
    $options['export'] .= _mms_admin_data('uitranslate_export_warning');
    $options['show'] = _mms_admin_data('uitranslate_show');
    $options['import'] = _mms_admin_data('uitranslate_import');
  } else {
    $options['export'] .= _mms_admin_data('uitranslate_export_advice');
  }
  $form['uitranslate'][MMS_UITRANSLATE_OPERATION] = array(
    '#type' => 'radios',
    '#title' => _mms_admin_data('uitranslate_operation_title'),
    //'#description' => _mms_admin_data('uitranslate_operation_description'),
    '#options' => $options,
    '#default_value' => array(),
  );
  
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => _mms_admin_data('uitranslate_submit_button'),
  );

  return $form;
}
function mms_uitranslate_form_submit($form, &$form_state) {
  module_load_include('inc', 'mms', 'mms.data');
  $operation = $form_state['values'][MMS_UITRANSLATE_OPERATION];
  if ($operation) {
    batch_set(array(
      'title' => _mms_admin_data('uitranslate_submit_title'),
      'init_message' => format_string(
        _mms_admin_data('uitranslate_submit_init'),
        ['@operation' => $operation]),
      'operations' => [array('mms_uitranslate_process',[$operation])],
      'finished' => 'mms_uitranslate_finished',
      'file' => drupal_get_path('module', 'mms') . '/mms.admin.inc',
    ));
  } else {
    drupal_set_message(_mms_admin_data('uitranslate_submit_none'), 'warning');
  }
}
function mms_uitranslate_process($operation, &$context) {
  module_load_include('inc', 'mms', 'mms.ui');
  // Execute the given $operation.
  $callback = '_mms_ui_' . $operation;
  $context['results']['result'] = $callback();
  $context['results']['operation'] = $operation;
}
function mms_uitranslate_finished($success, $results, $operations) {
  module_load_include('inc', 'mms', 'mms.data');
  $operation = $results['operation'];
  if (!$success OR !$results['result']) {
    // An error occurred.
    drupal_set_message(
      format_string(
        _mms_admin_data('uitranslate_finished_error'),
        ['@operation' => $operation]
      ), 'error'
    );
    return;
  }
  // Otherwise success, display standard or dedicated message.
  $message =
    $results['result'] === TRUE ?
      format_string(
        _mms_admin_data('uitranslate_finished_ok'),
        ['@operation' => $operation]
      )
    :
      $message = $results['result']
    ;
  drupal_set_message($message);
}
/*----------------------------------------------------------------------------*/
function _mms_help($group, $anchor) {
  module_load_include('inc', 'mms', 'mms.core');
  return
    _mms_url(
      '/admin/help/mms',
      '<img src="' . MMS_PATH . '/help.png" alt="Link to help" />',
      ['fragment' => $anchor]
      ) .
    ($group ? ('&nbsp;' . _mms_admin_data($group)) : NULL)
    ;
}
/*----------------------------------------------------------------------------*/
