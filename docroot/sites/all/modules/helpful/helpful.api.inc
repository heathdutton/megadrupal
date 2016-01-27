<?php
/**
 * @file helpful.api.inc
 *
 * Hooks provided by helpful modue.
 *
 */

/**
 * Implements hook_helpful_help_access()
 *
 *  - To implement, add to *.help.ini:
 *  
 *    [node-pages]
 *    title = Page Nodes
 *    help path = node/%
 *    node type = page
 *    
 */
function hook_helpful_help_access($path, $args, $help_item){
  // Check path
  if ($path == 'node/%'){
    // Then check node type in INI
    $node = node_load($args[1]);
    return ($node->type == $help_item['ini']['node type']);
  }
}

/**
 * Preprocessor for theme_helpful_topic()
 * - Sends output through token_replace with the current node.
 */
function hook_preprocess_helpful_topic(&$vars){
  if ($vars['path'] == 'node/%'){
    $node = node_load($vars['args'][1]);
    $vars['contents'] = token_replace($vars['contents'], array('node' => $node));
  }
}
