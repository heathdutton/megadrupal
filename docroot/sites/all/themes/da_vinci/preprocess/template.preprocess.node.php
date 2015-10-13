<?php
/**
 * @file
 * Preproccess functions for Node element.
 */

/**
 * Implements da_vinci_preprocess_node().
 */
function da_vinci_preprocess_node(&$vars) {

  if ($vars['view_mode'] == 'full' && node_is_page($vars['node'])) {
    $vars['classes_array'][] = 'node-full';
  }

  $vars['date'] = t('!datetime', array('!datetime' => date('j F Y', $vars['created'])));

  // Providing templates suggestions for the nodes for view mode.
  $vars['theme_hook_suggestions'][] = 'node__' . $vars['view_mode'];
  $vars['theme_hook_suggestions'][] = 'node__' . $vars['node']->type . '__' . $vars['view_mode'];
  $vars['theme_hook_suggestions'][] = 'node__' . $vars['node']->nid . '__' . $vars['view_mode'];

}
