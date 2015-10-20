<?php

/**
 * Implements hook_theme().
 *
 * Purpose:
 *   - Defines theme callback for 'add entity ...' page.
 *   - Defines theme callback for property field of entity (pseudo fields).
 *   - Defines theme callback for default ES page '/es'.
 */
function entityspice_theme() {
  $info = array(
    'entityspice_entity_add_list' => array('content', 'entity_type'),
    'entityspice_entity_list' => array('header', 'rows', 'entity_type', 'entities'),
    'entityspice_default_page' => array('entity_types'),
  );

  $theme = array();
  foreach ($info as $t_name => $t_vars) {
    foreach ($t_vars as $var) {
      $theme[$t_name]['variables'][$var] = NULL;
    }
  }

  $theme['entityspice_entity_property_field'] = array(
    'variables' => array(
      'label_hidden' => FALSE,
      'title_attributes' => NULL,
      'label' => '',
      'content_attributes' => NULL,
      'items' => array(),
      'item_attributes' => array(0 => ''),
      'classes' => '',
      'attributes' => '',
    ),
  );


  return $theme;
}

/**
 * Returns HTML for a list of available entity types for entity creation.
 *
 * @param array $variables
 *   An associative array containing:
 *   - entity_types: An array of entity types.
 *
 * @ingroup themeable
 */
function theme_entityspice_default_page($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="node-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    $output  = '<p>';
    $output .= t('You have not created any entity types yet. Use drush to generate one.');
    $output .= '</p>';
  }

  return $output;
}
