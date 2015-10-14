<?php

/**
 * @file
 * Contains form alter callback bodies.
 */

/**
 * Menu callback to configure the default behaviour of the module.
 */
function readmorecontrol_admin_settings_form($form, &$form_state, $entity_type = 'node') {
  $entity_info = entity_get_info($entity_type);
  if (!$entity_info || empty($entity_info['uri callback'])) {
    drupal_not_found();
    return;
  }

  // Configures the default behaviour of the module.
  $key = 'readmorecontrol_behaviour__' . $entity_type;
  $form[$key] = array(
    '#type' => 'radios',
    '#title' => t('Default "Read more" link behaviour'),
    '#default_value' => variable_get($key, $entity_type == 'node' ? 'always' : 'never'),
    '#required' => TRUE,
    '#options' => readmorecontrol_display_options($entity_type),
    '#description' => t('This setting can be overridden by individual content types and fields. Note that most themes will link teaser titles to the node irrespective of these settings.'),
  );

  // Global and bundle based processing.
  $key = 'readmorecontrol_format__' . $entity_type;
  $form[$key] = array(
    '#type' => 'fieldset',
    '#title' => t('Global formatting settings'),
    '#description' => t('Here you can specify the wording, appearance and position of the Read More Link globally.'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $settings = readmorecontrol_format_settings($entity_type, NULL, NULL, TRUE);
  $form[$key] += readmorecontrol_formatting_settings_form($entity_type, $entity_info, $settings, $key);
  unset($form[$key]['enabled']['#options']['global']);
  return system_settings_form($form);
}

function readmorecontrol_formatting_settings_form($entity_type, $entity_info, $settings, $key) {
  $subform['#attached']['css'] = array(
    drupal_get_path('module', 'readmorecontrol') . '/readmorecontrol.css',
  );
  $subform['enabled'] = array(
    '#type' => 'radios',
    '#title' => t('Enable custom link processing'),
    '#default_value' => $settings['enabled'],
    '#options' => array(
      'global' => t('Inherit default format settings'),
      'none' => t('Do not modify'),
      'custom' => t('Custom settings'),
    ),
  );
  $states = array(
    'visible' => array(
      ':input[name$="' . $key . '[enabled]"]' => array('value' => 'custom'),
    ),
  );

  $subform['text'] = array(
    '#type' => 'textfield',
    '#title' =>  t('Link text'),
    '#default_value' => $settings['text'],
    '#states' => $states,
    '#description' => t('Defaults to: "<em>Read more&lt;span class="element-invisible"&gt; about @title&lt;/span&gt;</em>".<br/>Replace "@title with" corresponding token, eg: "[node:title]"'),
    '#attributes' => array('class' => array('readmorecontrol')),
  );
  $subform['href'] = array(
    '#type' => 'textfield',
    '#title' =>  t('URL'),
    '#default_value' => $settings['href'],
    '#states' => $states,
    '#prefix' => '<div class="clearfix"><div class="readmorecontrol-inline">',
    '#suffix' => '</div>',
    '#size' => 50,
    '#description' => t('Default links to content. e.g. "<em>node/1</em>".<br/>Use "<em>&lt;none&gt;</em>" to remove link and just show the text.'),
    '#attributes' => array('class' => array('readmorecontrol')),
  );
  $subform['query'] = array(
    '#type' => 'textfield',
    '#title' =>  t('Query'),
    '#default_value' => $settings['query'],
    '#states' => $states,
    '#field_prefix' => '&nbsp;?&nbsp;',
    '#prefix' => '<div class="readmorecontrol-inline">',
    '#suffix' => '</div>',
    '#size' => 20,
    '#attributes' => array('class' => array('readmorecontrol')),
  );
  $subform['fragment'] = array(
    '#type' => 'textfield',
    '#title' =>  t('Fragment'),
    '#default_value' => $settings['fragment'],
    '#states' => $states,
    '#field_prefix' => '&nbsp;#&nbsp;',
    '#prefix' => '<div class="readmorecontrol-inline">',
    '#suffix' => '</div></div>',
    '#size' => 20,
    '#attributes' => array('class' => array('readmorecontrol')),
  );
  $subform['title'] = array(
    '#type' => 'textfield',
    '#title' =>  t('Link title (hover) text'),
    '#default_value' => $settings['title'],
    '#states' => $states,
    '#field_prefix' => 'title = "',
    '#field_suffix' => '"',
    '#attributes' => array('class' => array('readmorecontrol')),
  );
  $subform['class'] = array(
    '#type' => 'textfield',
    '#title' =>  t('Additional classes'),
    '#default_value' => $settings['class'],
    '#field_prefix' => 'class = "',
    '#field_suffix' => '"',
    '#states' => $states,
    '#attributes' => array('class' => array('readmorecontrol')),
  );
  $subform['rel'] = array(
    '#type' => 'textfield',
    '#title' => t('Rel attribute'),
    '#description' => t('When output, this link will have this rel attribute. The most common usage is <a href="http://en.wikipedia.org/wiki/Nofollow">rel=&quot;nofollow&quot;</a> which prevents some search engines from spidering links.'),
    '#default_value' => $settings['rel'],
    '#field_prefix' => 'rel = "',
    '#field_suffix' => '"',
    '#states' => $states,
    '#attributes' => array('class' => array('readmorecontrol')),
  );
  $subform['target'] = array(
    '#type' => 'radios',
    '#title' => t('Link target'),
    '#default_value' => $settings['target'],
    '#options' => array(
      '' =>  t('Default (no target attribute)'),
      '_blank' => t('Opens the linked document in a new window or tab'),
      '_top' => t('Opens the linked document in the top window frame'),
    ),
    '#states' => $states,
  );

  $subform['placement'] = array(
    '#type' => 'radios',
    '#title' => t('Link placement'),
    '#options' => array(
      'none'   => t("Don't move (rendered in links element)"),
      'body_inline'  => t('Inline on body field'),
      'body_append'  => t('Append on new line after body field'),
      'text_inline'  => t('Inline on final long text field'),
      'text_append'  => t('Append on new line after final long text field'),
      'field_inline'  => t('Inline on final field'),
      'field_append'  => t('Append on new line after final field'),
      'append'  => t('Append on new line after final field or addition field'),
    ),
    '#default_value' => $settings['placement'],
    '#states' => $states,
  );

  $subform['help'] = array(
    '#type' => 'fieldset',
    '#title' =>  t('Help'),
    '#states' => $states,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $subform['help']['general'] = array(
    '#markup' => t('<p>Leave fields empty to use the system defaults. Tokens can be used in all text fields for the current %entity.</p>',
      array('%entity' => $entity_info['label'])),
  );
  if (module_exists('token')) {
    $subform['help']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array($entity_type),
      '#recursion_limit' => 2,
    );
  }
  return $subform;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function _readmorecontrol_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id) {
  // Add readmore_behaviour flag to field_ui_field_edit_form to set fields
  // specific behaviour on the read more link (if applicable).
  $instance = $form['#instance'];
  $field = $form['#field'];
  $entity_type = $instance['entity_type'];
  if (readmorecontrol_supported_entity($entity_type)) {
    $form['instance']['readmore_behaviour'] = array(
      '#type' => 'radios',
      '#title' => t('Read More Behaviour'),
      '#default_value' => empty($instance['readmore_behaviour']) ? 'default' : $instance['readmore_behaviour'],
      '#options' => array(
        'default' => t('Use the default behaviour'),
        'process' => t('Check this field when determining the Read more link'),
        'ignore' => t('Ignore this field when determining the Read more link'),
      ),
      '#weight' => -9,
    );

    $usage_desc = array();
    $info = entity_get_info($entity_type);
    $usage_desc[] = t('Display @view_mode: !usage', array(
      '@view_mode' => t('Default'),
      '!usage' => readmorecontrol_field_readmore_settings($field, $instance),
    ));
    $excluded = readmorecontrol_excluded_view_modes();
    $excluded[] = 'full';
    foreach($info['view modes'] as $view_mode => $view_mode_info) {
      if (!in_array($view_mode, $excluded)) {
        $usage_desc[] = t('Display @view_mode: !usage', array(
          '@view_mode' => $view_mode_info['label'],
          '!usage' => readmorecontrol_field_readmore_settings($field, $instance, $view_mode),
        ));
      }

    }
    $form['instance']['readmore_behaviour']['#description']= implode('<br/>', $usage_desc);
  }
}

/**
 * Helper function to explain how the current instance setting would be used.
 */
function readmorecontrol_field_readmore_settings($field, $instance, $view_mode = 0) {
  $bundle = $instance['bundle'];
  $entity_type = $instance['entity_type'];

  $system_default = $entity_type == 'node' ? 'always' : 'never';
  $entity_default = variable_get('readmorecontrol_behaviour__' . $entity_type, $system_default);
  $bundle_default = variable_get("readmorecontrol_behaviour__{$entity_type}__{$bundle}", $entity_default);

  $t_args = array();
  if ($view_mode) {
    $view_default = variable_get("readmorecontrol_behaviour__{$entity_type}__{$bundle}__{$view_mode}", $bundle_default);
    if ($view_default != 'default') {
      $actual_behaviour = $view_default;
      $t_args['!settings_source'] = t('display');
    }
    elseif ($bundle_default != 'default') {
      $actual_behaviour = $bundle_default;
      $t_args['!settings_source'] = t('default display');
    }
    else {
      $actual_behaviour = $entity_default;
      $t_args['!settings_source'] = t('entity');
    }
  }
  else {
    if ($bundle_default != 'default') {
      $actual_behaviour = $bundle_default;
      $t_args['!settings_source'] = t('default display');
    }
    else {
      $actual_behaviour = $entity_default;
      $t_args['!settings_source'] = t('entity');
    }
  }

  $output = '';
  switch ($actual_behaviour) {
    case 'always':
      $t_args['%behaviour'] = t('always show');
      break;

    case 'when_required':
      $t_args['%behaviour'] = t('conditionally show based on any field');
      break;

    case 'when_required_text':
      $t_args['%behaviour'] = t('conditionally show based on any text fields');
      break;

    case 'when_required_body':
      $t_args['%behaviour'] = t('conditionally show based on the body field');
      break;

    case 'never':
      $t_args['%behaviour'] = t('never show link');
      break;

    case 'none':
      $t_args['%behaviour'] = t('disabled');
      break;

  }

  $output .= ' ' . t('set to "%behaviour" using <strong>!settings_source</strong> settings.', $t_args);
  return $output;
}



/**
 * Internal implementation of hook_form_FORM_ID_alter().
 *
 * Alters the fields overview and display overview screen.
 */
function _readmorecontrol_form_field_ui_display_overview_form_alter(&$form, &$form_state, $display_overview = FALSE) {
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];
  $default = $view_mode == 'default';

  $entity_info = entity_get_info($entity_type);
  if (!readmorecontrol_supported_entity($entity_type, $entity_info)) {
    return;
  }

  // Add additional settings vertical tab.
  if (!isset($form['additional_settings'])) {
    $form['additional_settings'] = array(
      '#type' => 'vertical_tabs',
      '#theme_wrappers' => array('vertical_tabs'),
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );
    $form['#attached']['js'][] = 'misc/form.js';
    $form['#attached']['js'][] = 'misc/collapse.js';
  }

  // Add extra guidelines for webmaster.
  $form['additional_settings']['readmorecontrol'] = array(
    '#type' => 'fieldset',
    '#title' => t('Read More Control'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#parents' => array('additional_settings'),
    '#tree' => FALSE,
  );
  $fieldset = &$form['additional_settings']['readmorecontrol'];

  $t_args = array(
    '%label' => $entity_info['bundles'][$bundle]['label'],
    '@content' => $entity_info['label'],
  );
  if ($default) {
    $fieldset['#description'] = t('<p class="readmorecontrol-help">These settings define the behaviours for all %label @content items. These can be overriden if you use custom display settings.</p>', $t_args);
  }
  else {
    $t_args['%display'] = $entity_info['view modes'][$view_mode]['label'];
    $disabled_modes = variable_get('readmorecontrol_disabled_view_modes', readmorecontrol_excluded_view_modes());
    if ($view_mode == 'full' || in_array($view_mode, $disabled_modes)) {
      $fieldset['#description'] = t('<p class="readmorecontrol-help">Read more settings are not available on the %display display mode.</p>', $t_args);
      return;
    }
    else {
      $fieldset['#description'] = t('<p class="readmorecontrol-help">These settings define the behaviours for all %label @content items when viewed using the %display display settings.</p>', $t_args);
    }
  }

  $key = 'readmorecontrol_behaviour__' . $entity_type . '__' . $bundle . ($default ? '' : '__' . $view_mode);
  $fieldset[$key] = array(
    '#type' => 'radios',
    '#title' => t('Display Read More'),
    '#options' => readmorecontrol_display_options($entity_type, $bundle, $view_mode),
    '#default_value' => variable_get($key, 'default'),
    '#required' => TRUE,
    '#description' => t('Note that most themes will link to the main view irrespective of these settings.'),
  );

  $key = 'readmorecontrol_format__' . $entity_type . '__' . $bundle . ($default ? '' : '__' . $view_mode);
  $fieldset[$key] = array(
    '#type' => 'fieldset',
    '#title' => t('Formatting settings'),
    '#description' => t('Here you can specify the wording, appearance and position of the Read More Link.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $settings = readmorecontrol_format_settings($entity_type, $bundle, $view_mode, TRUE);
  $fieldset[$key] += readmorecontrol_formatting_settings_form($entity_type, $entity_info, $settings, $key);

  if ($view_mode == 'search_result') {
    $fieldset[$key]['placement']['#options'] = array(
      'search_snippet_inline' => t('Inline in the search results snippet'),
      'search_snippet' => t('New line after the search results snippet'),
      'search_info' => t('In the results info'),
    );
  }
  $form['#submit'][] = 'readmorecontrol_form_field_ui_display_overview_form_alter_submit';
}

function _readmorecontrol_form_field_ui_display_overview_form_alter_submit(&$form, &$form_state) {
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];
  $default = $view_mode == 'default';

  $key = 'readmorecontrol_behaviour__' . $entity_type . '__' . $bundle . ($default ? '' : '__' . $view_mode);
  if (isset($form_state['values'][$key])) {
    variable_set($key, $form_state['values'][$key]);
  }
  $key = 'readmorecontrol_format__' . $entity_type . '__' . $bundle . ($default ? '' : '__' . $view_mode);
  if (isset($form_state['values'][$key])) {
    variable_set($key, $form_state['values'][$key]);
  }
}

/**
 * Helper function to get the default display options.
 */
function readmorecontrol_display_options($entity_type, $bundle = NULL, $view_mode = NULL) {
  $entity_info = entity_get_info($entity_type);
  $options = array();
  if (isset($bundle)) {
    $options['default'] = t('Inherit default behaviour');
  }
  if ($entity_type == 'node') {
    $options += array(
      'always' => t('Always show link<br/><small>Standard Drupal 7 behaviour.</small>'),
      'when_required' => t('Show link when required by any supported fields'),
      'when_required_text' => t('Show link when required by any supported text based fields'),
      'when_required_body' => t('Show link when required by the Body (body) field<br/><small>Standard Drupal 6 behaviour.</small>'),
      'never' => t('Never show link'),
    );
  }
  else {
    $options += array(
      'always' => t('Add a link'),
    );
    if ($entity_info['fieldable']) {
      $options['when_required'] = t('Add link when required by any supported fields');
      $options['when_required_text'] = t('Add link when required by any supported text based fields');
    }
    $options['never'] = t('Never add link');
  }
  $options['none'] = t('Do not process');
  return $options;
}
