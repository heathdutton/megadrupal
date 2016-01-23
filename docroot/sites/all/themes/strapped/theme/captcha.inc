<?php
/**
 * @file
 * Captcha related theme functions.
 */

/**
 * Overrides theme_captcha().
 *
 * @param $variables
 * @return string
 */
function strapped_captcha($variables) {
  $element = $variables['element'];
  if (!empty($element['#description']) && isset($element['captcha_widgets'])) {
    $fieldset = array(
      '#type' => 'fieldset',
      '#title' => isset($element['#title']) ? $element['#title'] : '',
      '#title_display' => $element['#title_display'],
      '#form_type' => isset( $element['#form_type']) ?  $element['#form_type'] : array(),
      '#element_grid' => isset($element['#element_grid']) ? $element['#element_grid'] : array(),
      '#title_grid' => isset($element['#title_grid']) ? $element['#title_grid'] : array(),
      '#description' => $element['#description'],
      '#children' => drupal_render_children($element),
      '#attributes' => array('class' => array('captcha')),
    );
    return theme('fieldset', array('element' => $fieldset));
  }
  else {
    return '<div class="captcha">' . drupal_render_children($element) . '</div>';
  }
}