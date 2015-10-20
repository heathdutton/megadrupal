<?php

/**
 * @file
 * Add, Update and Delete block functions for the slidedeck module.
 */

/**
 * Menu callback for displaying the list of
 * slidedeck blocks created.
 */
function slidedeck_list() {
  $library_path = libraries_get_path('slidedeck.jquery.lite.js');
  if (!file_exists($library_path)) {
    drupal_set_message(t('Slidedeck module needs slidedeck.jquery.lite.js to work.Download slidedeck js from  <a href="@url">Slidedeck Jquery Plugin</a>  and place in the sites/all/libraries folder as slidedeck.jquery.lite.js'));
  }

  $result = db_select('slidedeck', 'sd')
    ->fields('sd', array('title', 'id'))
    ->orderBy('title', 'ASC')
    ->execute();

  $header = array(
    array('data' => t('Slidedeck')),
    array('data' => t('Operations'), 'colspan' => 3),
  );

  $rows = array();
  foreach ($result as $row) {
    $tablerow = array(
      array('data' => $row->title),
      array('data' => l(t('Edit'), 'admin/structure/slidedeck/manage/' . $row->id . '/edit')),
      array('data' => l(t('Delete'), 'admin/structure/slidedeck/manage/' . $row->id . '/delete')),
    );
    $rows[] = $tablerow;
  }
  if (!$rows) {
    $rows[] = array(array('data' => t('No Slidedeck blocks available.'), 'colspan' => 4));
  }

  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'slidedeck'),
  );

  return $build;
}

/**
 * Deletion of slidedeck block.
 */
function slidedeck_block_delete($form, $form_state, $slidedeck) {
  $form['id'] = array('#type' => 'hidden', '#value' => $slidedeck['id']);
  $form['sd_name'] = array('#type' => 'hidden', '#value' => $slidedeck['title']);

  return confirm_form($form, t('Are you sure you want to delete the slidedeck block %name?', array('%name' => $slidedeck['title'])), 'admin/structure/slidedeck', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for slidedeck block deletion.
 */
function slidedeck_block_delete_submit($form, &$form_state) {
  $delta = $form_state['values']['sd_name'] . "_" . $form_state['values']['id'];
  db_delete('slidedeck')
    ->condition('id', $form_state['values']['id'])
    ->execute();

  db_delete('block')
    ->condition('module', 'slidedeck')
    ->condition('delta', $delta)
    ->execute();

  drupal_set_message(t('The Slidedeck block %name has been removed.', array('%name' => $form_state['values']['sd_name'])));

  cache_clear_all();

  $form_state['redirect'] = 'admin/structure/slidedeck';
}
