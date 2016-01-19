<?php

/**
 * Returns HTML for the filter order form.
 *
 * @see assetic_admin_form()
 * @see assetic_theme()
 */
function theme_assetic_admin_form_filter($variables) {
  $element = $variables['element'];

  $rows = array();
  foreach (element_children($element, TRUE) as $alias) {
    $element[$alias]['weight']['#attributes']['class'][] = 'assetic-filter-order-weight';
    $rows[] = array(
      'data' => array(
        drupal_render($element[$alias]['filter']),
        drupal_render($element[$alias]['config']),
        drupal_render($element[$alias]['status']),
        drupal_render($element[$alias]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(t('Name'), t('Operations'), t('Enabled'), t('Weight'));
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'assetic-filter-order',
    ),
  ));

  $output .= drupal_render_children($element);
  drupal_add_tabledrag('assetic-filter-order', 'order', 'sibling', 'assetic-filter-order-weight');

  return $output;
}
