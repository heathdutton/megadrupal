<?php

/**
 * Returns HTML for a node preview for display during node creation and editing.
 *
 * @param $variables
 *   An associative array containing:
 *   - node: The node object which is being previewed.
 *
 * @see node_preview()
 * @ingroup themeable
 */
function theme_no_trimmed_preview($variables) {

  $node = $variables['node'];
  $elements = node_view($node, 'full');

  $output = '<div class="preview">';
  $output .= '<h3 class="post-preview" >' . t('Preview') . '</h3>';
  $output .= drupal_render($elements);
  $output .= "</div>\n";
  return $output;
}
