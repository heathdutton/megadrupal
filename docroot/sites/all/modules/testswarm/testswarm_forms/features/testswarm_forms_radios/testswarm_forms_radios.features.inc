<?php
/**
 * @file
 * testswarm_forms_radios.features.inc
 */

/**
 * Implements hook_node_info().
 */
function testswarm_forms_radios_node_info() {
  $items = array(
    'radios_test' => array(
      'name' => t('Radios test'),
      'base' => 'node_content',
      'description' => t('see http://drupal.org/node/811542'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
