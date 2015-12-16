<?php
/**
 * @file
 * Generate render array to attach to a form, to configure default button.
 */

/**
 * Prepares JS setting for attaching to the render array.
 */
function _form_default_button_setting($form_html_id, $button_html_id, $form_settings) {
  $configuration = unserialize($form_settings->configuration);

  return array(
    "$form_html_id" => array(
      'action' => $form_settings->default_action,
      'target' => isset($configuration['click_selector']) ? $configuration['click_selector'] : '',
      'button_id' => $button_html_id,
    ),
  );
}

/**
 * Outputs the render array for a form.
 *
 * Using dedicated class "form-default-button-wrapper", rather than
 * "element-invisible" to prevent possibly setting "display: none" by
 * mistake, and preventing this functionality from working correctly on IE.
 */
function _form_default_button_element($form_html_id, $form_settings) {
  $build = array(
    '#type' => 'container',
    '#weight' => -99,
    '#attributes' => array(
      'class' => array('form-default-button-wrapper'),
    ),
    'button' => array(
      '#type' => 'button',
      '#value' => 'Form default button',
      '#id' => drupal_html_id('form-default-button'),
      // Disable the button initially.
      '#attributes' => array('disabled' => 'disabled'),
    ),
  );

  // Attachments.
  $path = drupal_get_path('module', 'form_default_button') . '/form_default_button';
  $build['#attached']['js'][] = "$path.js";
  $build['#attached']['css'][] = "$path.css";
  $setting = _form_default_button_setting($form_html_id, $build['button']['#id'], $form_settings);
  $build['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => array('form_default_button' => $setting),
  );

  return $build;
}
