<?php

/**
 * Returns HTML for a breadcrumb trail.
 *
 * @param $variables
 *   An associative array containing:
 *   - breadcrumb: An array containing the breadcrumb links.
 */
function twbs_breadcrumb($variables) {
  $output = '';

  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    $output .= '<h2 class="sr-only">' . t('You are here') . '</h2>';
    $output .= theme('item_list', array(
      'items' => $breadcrumb,
      'type' => 'ol',
      'attributes' => array('class' => array('breadcrumb')),
    ));
  }

  return $output;
}
