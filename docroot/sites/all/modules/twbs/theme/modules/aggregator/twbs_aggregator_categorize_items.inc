<?php

/**
 * Returns HTML for the aggregator page list form for assigning categories.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function twbs_aggregator_categorize_items($variables) {
  $form = $variables['form'];

  $output = drupal_render($form['feed_source']);
  $rows = array();
  if (!empty($form['items'])) {
    foreach (element_children($form['items']) as $key) {
      $rows[] = array(
        drupal_render($form['items'][$key]),
        array('data' => drupal_render($form['categories'][$key]), 'class' => array('categorize-item')),
      );
    }
  }
  $output .= theme('table', array('header' => array('', t('Categorize')), 'rows' => $rows));
  $output .= drupal_render($form['submit']);
  $output .= drupal_render_children($form);

  return theme('aggregator_wrapper', array('content' => $output));
}
