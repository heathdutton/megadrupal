<?php
/**
 * @file
 * pdf_archive_example.features.inc
 */

/**
 * Implements hook_node_info().
 */
function pdf_archive_example_node_info() {
  $items = array(
    'pdf_example' => array(
      'name' => t('PDF example'),
      'base' => 'node_content',
      'description' => t('An example content type to showcase the PDF Archive modules functionality.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
