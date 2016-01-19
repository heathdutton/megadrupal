<?php
/**
 * @file
 * Theming functions.
 */

/**
 * Default theme implementation for the advagg_replace_settings_form form.
 */
function theme_advagg_replace_settings_form($variables) {
  $form = $variables['form'];

  $header = array(
    t('Search string'),
    t('Default replacement'),
    t('Help / instructions (optional)'),
    t('Weight'),
  );

  foreach (array('css', 'js') as $type) {
    // Allow the items to be manually sorted.
    drupal_add_tabledrag('settings-' . $type, 'order', 'sibling', 'sort-order');

    $rows = array();
    foreach (element_children($form[$type]) as $element_id) {
      if (isset($form[$type][$element_id]['weight'])) {
        $element = &$form[$type][$element_id];

        // Add special classes to be used for tabledrag.js.
        $element['weight']['#attributes']['class'] = array('sort-order');

        $row = array();
        $row[] = drupal_render($element['string']);
        $row[] = drupal_render($element['default']);
        $row[] = drupal_render($element['help']);
        $row[] = drupal_render($element['weight']);

        $row = array_merge(array('data' => $row), $element['#attributes']);
        $row['class'][] = 'draggable';
        $rows[] = $row;
      }
    }
    if (empty($rows)) {
      $rows[] = array(array(
        'data' => t('Nothing to see here.'),
        'colspan' => '3',
      ));
    }
    $form[$type]['table'] = array(
      '#markup' => theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'settings-' . $type))),
      '#weight' => 5,
    );
  }

  $output = drupal_render_children($form);
  return $output;
}

/**
 * Default theme implementation for the advagg_replace_replacements_form form.
 */
function theme_advagg_replace_replacements_form($variables) {
  $form = $variables['form'];

  $header = array(
    // t('Field'),
    t('Original'),
    t('Replacement'),
    t('Help / instructions'),
  );

  foreach (array('css', 'js') as $type) {
    if (!empty($form[$type])) {
      $rows = array();
      foreach (element_children($form[$type]) as $element_id) {
        if (isset($form[$type][$element_id]['replace'])) {
          $element = &$form[$type][$element_id];

          $row = array();
          $row[] = drupal_render($element['original']);
          $row[] = drupal_render($element['replace']);
          $row[] = drupal_render($element['help']);

          $row = array_merge(array('data' => $row), $element['#attributes']);
          $rows[] = $row;
        }
      }
      if (!empty($rows)) {
        $form[$type]['table'] = array(
          '#markup' => theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'settings-' . $type))),
          '#weight' => 5,
        );
      }
    }
  }

  $output = drupal_render_children($form);
  return $output;
}
