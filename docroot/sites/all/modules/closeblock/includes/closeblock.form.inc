<?php

/**
 * Add Closeblock settings to theme settings form
 * 
 * @param $form
 * @param $form_state 
 */
function closeblock_system_theme_settings(&$form, $form_state) {
  $settings = variable_get($form['var']['#value'], array());
  $settings = array_merge(closeblock_default_settings(), $settings, array());

  $form['closeblock'] = array(
    '#type' => 'fieldset',
    '#title' => t('Closeblock selectors'),
    '#weight' => 0,
    '#attributes' => array('id' => 'closeblock_form'),
  );
  
  $form['closeblock'] += closeblock_settings_elements($settings);
}

/**
 * Closeblock settings form elements
 * 
 * @param $settings
 * @return
 *  Array of form elements 
 */
function closeblock_settings_elements($settings) {
  $form['closeblock_type'] = array(
    '#type' => 'radios',
    '#title' => t('Block close behavior'),
    '#options' => array(1 => t('None.'), 2 => t('Slide Up.'), 3 => t('Fade Out')),
    '#default_value' => $settings['closeblock_type'],
  );
  
  $form['closeblock_speed'] = array(
    '#type' => 'select',
    '#title' => t('Animation speed'),
    '#options' => drupal_map_assoc(array('50', '100', '200', '300', '400', '500', '700', '1000', '1300')),
    '#description' => t('The animation speed in milliseconds.'),
    '#default_value' => $settings['closeblock_speed'],
  );
  
  $form['closeblock_save'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save state for user'),
    '#default_value' => $settings['closeblock_save'],
    '#description' => t('Save state of block to database. If checked, the block will be hidden automatically when page is loaded.'),
  );
  
  $form['closeblock_count'] = array(
    '#type' => 'textfield',
    '#title' => t('How many times show'),
    '#default_value' => $settings['closeblock_count'],
    '#description' => t('How many times block will be appear until the block will not be hidden permanently.'),
  );
  
  $form['closeblock_frequency'] = array(
    '#type' => 'textfield',
    '#title' => t('Frequency'),
    '#default_value' => $settings['closeblock_frequency'],
    '#description' => t('Eq: <em>+1 day</em>, <em>+3 weeks</em>.'),
  );
  
  $form['closeblock_reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#submit' => array('closeblock_reset_submit'),
    '#suffix' => t('Remove saved information from the database.'),
  );
  
  
  // Use ctools to hide elements
  if (module_exists('ctools')) {
    ctools_include('dependent');
    
    $form['closeblock_type']['#dependency'] =
    $form['closeblock_speed']['#dependency'] =
    $form['closeblock_save']['#dependency'] = array(
      'edit-closeblock-custom' => array(1),
    );
    
    $form['closeblock_count']['#dependency'] =
    $form['closeblock_frequency']['#dependency'] = array(
      'edit-closeblock-custom' => array(1),
      'edit-closeblock-save' => array(1),
    );
    
    if (empty($settings['theme'])) {
      $form['closeblock_count']['#dependency_count'] =
      $form['closeblock_frequency']['#dependency_count'] = 2;
    }
  }
  
  return $form;
}

/**
 * Add Closeblock settings to block settings form
 * 
 * @param $form
 * @param $form_state 
 */
function closeblock_block_admin_configure(&$form, $form_state) {
  $settings = variable_get('closeblock_settings', array());
  $form['#submit'][] = 'closeblock_submit';
  $form['visibility']['closeblock'] = array(
    '#type' => 'fieldset',
    '#title' => t('Closeblock'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 10,
    '#group' => 'visibility',
  );
  
  $params = array(
    'module' => $form['module']['#value'],
    'delta' => $form['delta']['#value'],
  );
  $block_id = theme('closeblock_block_id', $params);
  
  if (isset($settings[$block_id])) {
    $settings = $settings[$block_id];
  } else {
    $settings = array();
  }
  
  $settings = array_merge(closeblock_default_settings(), $settings, array());
  
  $form['visibility']['closeblock']['closeblock_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use close button'),
    '#default_value' => $settings['closeblock_active'],
    '#description' => t('Advanced settings <a href="@link">here.</a>', array(
      '@link' => url('admin/appearance/settings'),
    )),
  );
  
  $form['visibility']['closeblock']['closeblock_custom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use custom settings'),
    '#default_value' => $settings['closeblock_custom'],
  );
  
  $form['visibility']['closeblock'] = array_merge($form['visibility']['closeblock'], closeblock_settings_elements($settings));
}

/**
 * Form submission handler for block_admin_configure().
 *
 * @see block_admin_configure()
 * @see closeblock_form_alter()
 */
function closeblock_submit($form, &$form_state) {
  $settings = variable_get('closeblock_settings', array());
  $params = array(
    'module' => $form_state['values']['module'],
    'delta' => $form_state['values']['delta'],
  );
  $block_id = theme('closeblock_block_id', $params);
  
  $default_settings = closeblock_default_settings();
  
  foreach ($default_settings as $key=>&$value) {
    if (isset($form_state['values'][$key])) {
      $value = $form_state['values'][$key];
    }
  }
  
  $settings[$block_id] = $default_settings;
  $settings[$block_id]['module'] = $form_state['values']['module'];
  $settings[$block_id]['delta'] = $form_state['values']['delta'];
  variable_set('closeblock_settings', $settings);
}

/**
 * Form submission handler for Reset button.
 *
 * @see closeblock_settings_elements()
 */
function closeblock_reset_submit($form, &$form_state) {
  switch ($form['#id']) {
    case 'system-theme-settings':
      $params = array(
        'theme' => empty($form_state['values']['theme']) ? arg(3) : $form_state['values']['theme'],
      );
      break;
    case 'block-admin-configure':
      $params = array(
        'module' => $form_state['values']['module'],
        'delta' => $form_state['values']['delta'],
      );
      break;
  }
  
  if (!empty($params)) {
    closeblock_db_delete($params);
  }  
}
