<?php
/**
 * @file
 * fancy_i18n.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_i18n_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_eck_bundle_info().
 */
function fancy_i18n_eck_bundle_info() {
  $items = array(
    'content_i18n_text_i18n' => array(
      'machine_name' => 'content_i18n_text_i18n',
      'entity_type' => 'content_i18n',
      'name' => 'text_i18n',
      'label' => 'Text i18n',
    ),
    'fancy_section_responsive_i18n' => array(
      'machine_name' => 'fancy_section_responsive_i18n',
      'entity_type' => 'fancy_section',
      'name' => 'responsive_i18n',
      'label' => 'Responsive i18n',
    ),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function fancy_i18n_eck_entity_type_info() {
  $items = array(
    'content_i18n' => array(
      'name' => 'content_i18n',
      'label' => 'Content i18n',
      'properties' => array(
        'language' => array(
          'label' => 'Entity language',
          'type' => 'language',
          'behavior' => 'language',
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implements hook_node_info().
 */
function fancy_i18n_node_info() {
  $items = array(
    'fancy_page_i18n' => array(
      'name' => t('Fancy Page i18n'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Working title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
