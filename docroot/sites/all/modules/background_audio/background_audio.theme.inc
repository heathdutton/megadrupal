<?php
/**
 * @file
 * Theme functions for Background Audio administrative pages.
 */

function theme_background_audio_controls_table($vars) {
  $form = $vars['form'];

  $header = array(
    t('Name'),
    t('Weight'),
  );
  $rows = array();

  foreach (element_children($form) as $control) {
    if (isset($form[$control][$control . '_enabled'])) {
      $row = array(
        drupal_render($form[$control][$control . '_enabled']),
        drupal_render($form[$control][$control . '_weight']),
      );

      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
      );
    }
  }

  //Finally, output the sortable table. Make sure the id variable is the same as the table id in drupal_add_tabledrag
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'background-audio-controls')));
  $output .= drupal_render_children($form);

  //This function is the instantiator of the sorter. Make sure the 0th paramater is the id of your table, and the 3rd paramater is the class of your weight variable
  drupal_add_tabledrag('background-audio-controls', 'order', 'sibling', 'background-audio-control-weight');

  return $output;
}
