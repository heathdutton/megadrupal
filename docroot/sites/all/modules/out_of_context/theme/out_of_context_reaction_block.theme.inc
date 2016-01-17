<?php
/**
 * @file
 * Theming and Preprocess functions for context blocks.
 */

/**
 * Preprocessor for theme('out_of_context_block_browser').
 */
function template_preprocess_out_of_context_block_browser(&$vars) {
  $categories = array(
    '#type' => 'select',
    '#options' => array(0 => '<' . t('Choose a category') . '>'),
    '#attributes' => array('class' => array('context-block-browser-categories')),
    '#value' => 0,
    '#size' => 1,
    '#id' => '',
    '#name' => '',
    '#parents' => array(''),
    '#multiple' => FALSE,
    '#required' => FALSE,
  );
  $blocks = array();
  // Group blocks by module.
  foreach ($vars['blocks'] as $block) {
    $group = isset($block->context_group) ? $block->context_group : $block->module;
    // Normalize the $group, borrowed from drupal_html_id
    $group = strtr(drupal_strtolower($group), array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
    if (!isset($categories[$group])) {
      $info = system_get_info('module', $block->module);
      $title = isset($block->context_group) ? $block->context_group : (!empty($info['name']) ? $info['name'] : $block->module);
      $categories['#options'][$group] = $title;
    }
    $blocks[$group][$block->bid] = $block; // Don't call theme('out_of_context_block_browser_item') to allow others to alter.
  } 
  
  foreach ($blocks as $k => $module) {
    foreach ($module as $l => $block) {
      $default = variable_get("out_of_context_block_{$l}", 0);
      // Remove unchecked blocks.
      if (empty($default)) {
        unset($blocks[$k][$l]);
      }
    }
    // Remove empty blocks modules.
    if (empty($blocks[$k])) {
       unset($categories['#options'][$k]);
    }
  }
  $vars['categories'] = $categories; // Don't call theme('select') here to allow further preprocesses to alter the element.
  $vars['blocks'] = $blocks;
}

/**
 * Preprocessor for theme('out_of_context_block_browser_item').
 */
function template_preprocess_out_of_context_block_browser_item(&$vars) {
  $vars['bid'] = $vars['block']->bid;
  $vars['info'] = check_plain($vars['block']->info);
}
