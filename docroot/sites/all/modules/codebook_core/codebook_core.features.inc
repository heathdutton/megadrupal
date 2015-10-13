<?php
/**
 * @file
 * codebook_core.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function codebook_core_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function codebook_core_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function codebook_core_node_info() {
  $items = array(
    'codebook_chapter' => array(
      'name' => t('Chapter'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Chapter Heading'),
      'help' => '',
    ),
    'codebook_ordinance' => array(
      'name' => t('Ordinance'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'codebook_resolution' => array(
      'name' => t('Resolution'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'codebook_section' => array(
      'name' => t('Section'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Section Heading'),
      'help' => '',
    ),
    'codebook_title' => array(
      'name' => t('Title'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title Heading'),
      'help' => '',
    ),
  );
  return $items;
}
