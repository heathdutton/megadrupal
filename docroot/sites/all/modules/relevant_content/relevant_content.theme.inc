<?php

/**
 * @file relevant_content.theme.inc
 * This file contains all the theme-related functionality for the module
 */


/**
 * Theme preprocess function for rendering the relevant nodes into a block.
 *
 * This is provided so that an item list is the default, however a themer can
 * easily override this to make a teaser list or table.
 *
 * @param $vars
 *   Contains the variables for the theme function, including:
 *   $vars['nodes'] - An associative array of node information
 *   $vars['block'] - the block being rendered
 */
function template_preprocess_relevant_content_block(&$vars) {
  // Get the definition of the block we are rendering
  $block = &$vars['block'];

  // Set an "is_empty" flag
  $vars['is_empty'] = empty($vars['nodes']);

  // Default to "link" type
  $type = 'link';

  // Check tokens is enabled; it's optional. Also check we have token settings
  if (module_exists('token') && !empty($block['token_settings'])) {
    // Cleanup the token pattern. See @relevant_content_filter_xss($string)
    // Unfortunately, we cannot 'clean up' the token pattern once due to the
    // bug with filter_xss on '[node:url]' which breaks the token.
    $token_pattern = trim($block['token_settings']);

    // If the token pattern is not empty, switch to tokens mode
    if (!empty($token_pattern)) {
      $type = 'tokens';
    }
  }

  foreach ($vars['nodes'] as $nid => $node) {
    // If we're a link, default to a hyperlink - otherwise we should use tokens.
    switch ($type) {
      default :
      case 'link' :
        $node['output'] = l($node['title'], 'node/' . $node['nid']);
        break;

      case 'tokens' :
        $objects = array('global' => NULL, 'node' => node_load($node['nid']));
        $node['output'] = token_replace($token_pattern, $objects);
        $node['output'] = relevant_content_filter_xss($node['output']);
        break;
    }

    // Build some classes for the node link row
    $node['classes'] = array(
      'relevant-content-item',
      'relevant-node-' . (int)$node['nid'],
      'relevant-node-type-' . check_plain($node['type']),
    );

    // Store the updated node over the old one
    $vars['nodes'][$nid] = $node;
  }

  // Build the header text - defaults to empty
  $vars['header'] = '';
  if (!empty($block['header_text'])) {
    $vars['header'] = check_markup($block['header_text'], $block['header_format'], FALSE);
  }

  // Add a template suggestion; relevant_content_block-{block-id}.tpl.php
  $vars['template_files'][] = 'relevant_content_block-' . $block['id'];
}


/**
 * Preprocess handler for the overview rows
 */
function template_preprocess_relevant_content_admin_overview(&$vars) {
  drupal_add_css(drupal_get_path('module', 'relevant_content') . '/relevant_content.admin.css');

  foreach ($vars['rows'] as $block_id => $row) {
    // Render the Types and Vocabs
    $row['types_rendered']  = implode(', ', $row['block']['types']);
    $row['vocabs_rendered'] = implode(', ', $row['block']['vocabs']);

    // Render the "ops"
    $row['ops_rendered'] = implode(' | ', $row['ops']);

    // Render the "absolute links" boolean
    $row['absolute_links_rendered'] = $row['block']['absolute_links'] ? t('Absolute') : t('Relative');

    // Render the header
    $row['header_rendered'] = check_markup($row['block']['header_text'], $row['block']['header_format'], FALSE);

    // Define the classes
    $row['row_classes'] = array();
    $row['row_classes'][] = 'relevant-content-block';

    switch ($row['block']['storage']) {
      case RELEVANT_CONTENT_STORAGE_NORMAL :
        $row['row_classes'][] = 'relevant-content-block-normal';
        $row['status'] = t('Normal');
        break;

      case RELEVANT_CONTENT_STORAGE_DEFAULT :
        $row['row_classes'][] = 'relevant-content-block-default';
        $row['status'] = t('Default');
        break;

      case RELEVANT_CONTENT_STORAGE_OVERRIDDEN :
        $row['row_classes'][] = 'relevant-content-block-overriden';
        $row['status'] = t('Overriden');
        break;
    }

    $row['row_classes'][] = 'relevant-content-block-' . ($row['block']['status'] ? 'enabled' : 'disabled');

    // Output a 'rendered' set of classes
    $row['row_classes_rendered'] = implode(' ', $row['row_classes']);

    $vars['rows'][$block_id] = $row;
  }
}
