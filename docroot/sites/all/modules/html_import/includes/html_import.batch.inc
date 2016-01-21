<?php

/**
 * @file
 * HTML Import batch function.
 */

function _html_import_import_batch_process_entity($book, $task, &$context) {
  if ($task == 'consolidate') {
    $nid = $book->bid;
    $node = node_load($nid);
    if (empty($node->field_full_text['und'][0]['value'])) {
      $sections = '';
      if (module_exists('book')) {
        // if book module is there, we can use its export function to consolidate all section
        module_load_include('inc', 'book', 'book.pages');
        $sections = book_export_html($book->bid);
      }
      else {
        // consolidate all sections using this query otherwise
        $book_menu_links = db_query('SELECT {book}.mlid AS book_mlid, {menu_links}.mlid AS menu_link_mlid, weight, link_path FROM {book} LEFT JOIN {menu_link}s ON {book}.mlid = {menu_links}.mlid WHERE bid = :bid ORDER BY weight', array(':bid' => $book->bid))->fetchAll();
        foreach ($book_menu_links as $book_menu_link) {
          $menu_object = menu_get_object('node', 1, $book_menu_link->link_path);
          $title = '<h1>' . $menu_object->title . '</h1>';
          $body = $menu_object->body['und'][0]['value'];
          $section = $title . $body;
          $sections .= $section;
        }
      }
      $node->field_full_text['und'][0]['value'] = $sections;
      try {
        node_save($node);
      } catch (Exception $e) {
        $message = t('An error occurred while processing node - %error_nid', array(
          '%error_nid' => $nid,
        ));
        $severity = WATCHDOG_ERROR;
        drupal_set_message($message);
      }
    }
  }
  return TRUE;
}

function _html_import_import_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = t('Import completed successfully');
  }
  else {
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE)
    ));
  }
  drupal_set_message($message);
}
