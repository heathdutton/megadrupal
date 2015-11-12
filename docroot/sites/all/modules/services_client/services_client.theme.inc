<?php

/**
 * Theme mapping widget. Mapping expects to have two columns with local and remote values.
 * Each row of form element should have 'local' and 'remote' key. Elements
 * will be formatted to table.
 *
 *  +--------------+--------------+
 *  | Local        | Remote       |
 *  +--------------+--------------+
 *  | selecbox     | selectbox    |
 *  +--------------+--------------+
 *  | ....         | ....         |
 *  +--------------+--------------+
 *
 * @param array $variables
 *   'render element' => 'form',
 *   Element that should be rendered.
 *
 * @return string
 *   Formatted output.
 */
function theme_services_client_mapping_rows($variables) {
  $element = $variables['form'];

  $rows = array();
  $header = array(t('Local'), t('Remote'));
  foreach (element_children($element) as $key) {
    if ($key !== 'add_row') {
      unset($element[$key]['local']['#title']);
      unset($element[$key]['remote']['#title']);
      $rows[] = array(
        drupal_render($element[$key]['local']),
        drupal_render($element[$key]['remote']),
      );
    }
  }

  $output = theme('table', array(
    'rows' => $rows,
    'header' => $header
  ));

  if (isset($element['add_row'])) {
    $output .= drupal_render($element['add_row']);
  }

  return $output;
}
