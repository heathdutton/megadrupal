<?php

/**
 * Returns HTML for an administrative page.
 *
 * @param $variables
 *   An associative array containing:
 *   - blocks: An array of blocks to display. Each array should include a
 *     'title', a 'description', a formatted 'content' and a 'position' which
 *     will control which container it will be in. This is usually 'left' or
 *     'right'.
 *
 * @ingroup themeable
 */
function twbs_admin_page($variables) {
  $blocks = $variables['blocks'];

  $output = theme('system_compact_link');
  foreach ($blocks as $block) {
    $output .= theme('admin_block', array('block' => $block));
  }

  return $output;
}
