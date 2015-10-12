<?php
/**
 * @file
 * Entity API module integration.
 */

/**
 * Implements hook_entity_property_info().
 */
function page_manager_search_entity_property_info() {
  $info = array();

  // Add meta-data about the basic page_manager_search properties.
  $properties = &$info['page_manager_search']['properties'];

  $properties['title'] = array(
   'label' => t('Title'),
    'description' => t('The title of the page.'),
    'type' => 'text',
    'required' => TRUE,
  );

  $properties['content'] = array(
   'label' => t('Content'),
    'description' => t('The content of the page.'),
    'type' => 'text',
    'required' => TRUE,
  );

  return $info;
}
