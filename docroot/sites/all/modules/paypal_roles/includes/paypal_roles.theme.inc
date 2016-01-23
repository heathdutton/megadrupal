<?php

/**
 * Theme function to render a form as a draggable table to show ordering items
 * for account selection.
 */
function theme_paypal_roles_account_selection_form($variables) {
  $form = $variables['form'];

  $header = array(
    t('Selection Method'),
    t('Weight'),
  );

  $rows = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $row = array();
      $row[] = drupal_render($form[$key]['name']) . drupal_render($form[$key]['item']);

      if (isset($form[$key]['weight'])) {
        $form[$key]['weight']['#attributes']['class'] = array('ordering-item-weight');
        // Add the weight field to the row. The Javascript to make our table drag
        // and drop will end up hiding this cell.
        $row[] = drupal_render($form[$key]['weight']);
      }

      // Add the row to the array of rows.
      $rows[] = array('data' => $row, 'class' => array('draggable'));
    }
  }

  $table_id = 'ordering-of-identifying-process';

  // We can render our tabledrag table for output.
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  $output .= drupal_render_children($form);

  // This function is what brings in the javascript to make our table as a
  // drag-and-drop table.
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'ordering-item-weight');

  return $output;
}
