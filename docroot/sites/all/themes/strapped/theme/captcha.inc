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
      '#title' => $element['#title'],
      '#title_display' => $element['#title_display'],
      '#form_type' => $element['#form_type'],
      '#element_grid' => $element['#element_grid'],
      '#title_grid' => $element['#title_grid'],
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