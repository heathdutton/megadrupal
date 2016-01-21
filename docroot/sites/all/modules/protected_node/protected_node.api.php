<?php

/**
 * @file
 * Hooks provided by the Protected node module.
 */

/**
 * Removes information from the node.
 *
 * Request that $node parameters that are expected to be protected to somehow
 * be removed from the $node.
 *
 * This function gives 3rd party modules adding content to a node a chance for
 * hiding that content before it gets displayed or indexed.
 *
 * The default implementation hides the fields. It either replace the title with
 * "Password protected page" or keeps it intact depending on the protected node
 * "Show Title" flag.
 *
 * The hook is only invoked when the node is protected and the user did not yet
 * provide the correct password.
 *
 * @see protected_node_protected_node_hide()
 */
function hook_protected_node_hide(&$node) {
  // Core module fields.
  if (!$node->protected_node_show_title) {
    $node->title = t('Password protected page');
  }
  $node->body = '';
  // Remove $node->content children to avoid the user see content he/she should
  // not see.
  $content_children = element_children($node->content);
  foreach ($content_children as $content_key) {
    unset($node->content[$content_key]);
  }
}
