<?php

/**
 * Returns HTML for disabled blocks, for use in dashboard customization mode.
 *
 * @param $variables
 *   An associative array containing:
 *   - block: A block object from _block_rehash().
 *
 * @ingroup themeable
 */
function twbs_dashboard_disabled_block($variables) {
  extract($variables);
  $output = "";
  if (isset($block)) {
    $output .= '<div id="block-' . $block['module'] . '-' . $block['delta']
    . '" class="disabled-block block block-' . $block['module'] . '-' . $block['delta']
    . ' module-' . $block['module'] . ' delta-' . $block['delta'] . '">'
    . '<h2>' . (!empty($block['title']) && $block['title'] != '<none>' ? check_plain($block['title']) : check_plain($block['info'])) . '</h2>'
    . '<div class="content"></div>'
    . '</div>';
  }
  return $output;
}
