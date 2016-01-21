<?php
/**
 * @file
 * Theme functions for Background Audio Block configuration pages.
 */

function theme_background_audio_block_files_table($vars) {
  $form = $vars['form'];

  $header = array(
    t('Name'),
    t('Path'),
    t('Weight'),
    t('Operations'),
  );
  $rows = array();

  foreach (element_children($form) as $fid) {
    if (isset($form[$fid][$fid . '_enabled'])) {
      $row = array(
        drupal_render($form[$fid][$fid . '_enabled']),
        drupal_render($form[$fid][$fid . '_uri']),
        drupal_render($form[$fid][$fid . '_weight']),
        drupal_render($form[$fid][$fid . '_delete']),
      );

      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
      );
    }
  }

  //Finally, output the sortable table. Make sure the id variable is the same as the table id in drupal_add_tabledrag
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'background-audio-files')));
  $output .= drupal_render_children($form);

  //This function is the instantiator of the sorter. Make sure the 0th paramater is the id of your table, and the 3rd paramater is the class of your weight variable
  drupal_add_tabledrag('background-audio-files', 'order', 'sibling', 'background-audio-file-weight');

  return $output;
}
