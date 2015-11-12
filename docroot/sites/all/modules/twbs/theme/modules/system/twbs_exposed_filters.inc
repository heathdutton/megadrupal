<?php

/**
 * Returns HTML for an exposed filter form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: An associative array containing the structure of the form.
 *
 * @return
 *   A string containing an HTML-formatted form.
 *
 * @ingroup themeable
 */
function twbs_exposed_filters($variables) {
  $form = $variables['form'];
  $output = '';

  if (isset($form['current'])) {
    $items = array();
    foreach (element_children($form['current']) as $key) {
      $items[] = drupal_render($form['current'][$key]);
    }
    $output .= theme('item_list', array('items' => $items, 'attributes' => array('class' => array('clearfix', 'current-filters'))));
  }

  $output .= drupal_render_children($form);

  return '<div class="exposed-filters">' . $output . '</div>';
}
