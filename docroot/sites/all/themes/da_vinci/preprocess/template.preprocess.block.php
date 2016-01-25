<?php
/**
 * @file
 * Preproccess functions for the Block element.
 */

/**
 * Implements da_vinci_preprocess_block().
 */
function da_vinci_preprocess_block(&$vars) {
  $block = & $vars['block'];

  $vars['title_attributes_array']['class'][] = 'block__title';

  $title_attributes = $vars['title_attributes_array'] ? drupal_attributes($vars['title_attributes_array']) : '';

  if ($block->subject && (!$block->region == "sidebar_first" || $block->region == "sidebar_second" || $block->region == 'left')) {
    $block->subject = '<h2' . $title_attributes . '>' . $block->subject . '</h2>';
  }
  elseif ($block->subject != '') {
    $block->subject = '<h3' . $title_attributes . '>' . $block->subject . '</h3>';
  }

  if (drupal_is_front_page()) {
    $vars['theme_hook_suggestions'][] = 'block__home_content';

  }
}
