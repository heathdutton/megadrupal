<?php

/**
 * @file
 *   Provides the administrative UI.
 */

/**
 * Form builder: Masked Input settings
 */
function maskedinput_settings($form, &$form_state) {
  $form_state['values']['definitions'] = isset($form_state['values']['definitions']) ? $form_state['values']['definitions'] : variable_get('maskedinput_definitions', array());
  $form_state['maskedinput']['count'] = isset($form_state['maskedinput']['count']) ? $form_state['maskedinput']['count'] : count($form_state['values']['definitions']);
  
  $form['definition'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mask Definitions'),
  );
  
  $form['definition']['definitions'] = array(
    '#theme' => 'maskedinput_settings_definitions',
    '#prefix' => '<div id="maskedinput-definitions">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  
  for ($i = 0; $i < $form_state['maskedinput']['count']; $i++) {
    $form['definition']['definitions'][$i] = _maskinput_definitions_element($i, $form_state['values']['definitions']);
  }
  
  $form['definition']['add_another_definition'] = array(
    '#type' => 'submit',
    '#value' => t('Add another'),
    '#submit' => array('_maskedinput_definitions_add_another_submit'),
    '#ajax' => array(
      'callback' => '_maskedinput_definitions_add_another_callback',
      'wrapper' => 'maskedinput-definitions',
    ),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  
  return $form;
}


/**
 * Validation callback: remove invalid definitions.
 * @see maskedinput_settings()
 */
function maskedinput_settings_validate($form, &$form_state) {
  foreach ($form_state['values']['definitions'] as $key => $values) {
    if (empty($values['character']) || empty($values['regex'])) {
      unset($form_state['values']['definitions'][$key]);
    }
  }
}


/**
 * Submission callback: save configurations to variables table.
 * @see maskedinput_settings()
 */
function maskedinput_settings_submit($form, &$form_state) {
  variable_set('maskedinput_definitions', $form_state['values']['definitions']);
}


/**
 * Ajax callback: return definitions element.
 * @see maskedinput_settings()
 */
function _maskedinput_definitions_add_another_callback($form, $form_state) {
  return $form['definition']['definitions'];
}


/**
 * Ajax submission callback: adds another definition form element.
 * @see maskedinput_settings()
 */
function _maskedinput_definitions_add_another_submit(&$form, &$form_state) {
  $form_state['maskedinput']['count']++;
  $form_state['rebuild'] = TRUE;
  $form['definition']['definitions'][] = _maskinput_definitions_element();
}


/**
 * Helper function: builds masked input definition form element.
 */
function _maskinput_definitions_element($delta = 'new', $definitions = array()) {
  return array(
    '#type' => 'container',
    '#attributes' => array(),
    'character' => array(
      '#type' => 'textfield',
      '#size' => 1,
      '#maxlength' => 1,
      '#default_value' => isset($definitions[$delta]['character']) ? $definitions[$delta]['character'] : '',
    ),
    'regex' => array(
      '#type' => 'textfield',
      '#default_value' => isset($definitions[$delta]['regex']) ? $definitions[$delta]['regex'] : '',
    ),
    'description' => array(
      '#type' => 'textfield',
      '#default_value' => isset($definitions[$delta]['description']) ? $definitions[$delta]['description'] : '',
    ),
  );
}



/**
 * Theme callback: Masked Input Definitions settings
 * @see maskedinput_settings()
 */
function theme_maskedinput_settings_definitions($variables) {
  $form = $variables['form'];
  
  // Hard-code default mask definitions because they come with the plugin.
  $rows = _maskedinput_default_definitions();
  
  $header = array(
    t('Character'),
    t('Regular expression'),
    t('Description'),
  );

  foreach (element_children($form) as $key) {
    $delete_key = $key . '_delete';
    $column = array();
    $column[] = array('data' => drupal_render($form[$key]['character']));
    $column[] = array('data' => drupal_render($form[$key]['regex']));
    $column[] = array('data' => drupal_render($form[$key]['description']));
    $rows[] = $column;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}
