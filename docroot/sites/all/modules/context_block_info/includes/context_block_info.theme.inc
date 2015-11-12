<?php
/**
 * @file
 * Theme include file containing all theme funcitons and all theme helper
 * functions.
 */

/**
 * Theme context row.
 */
function theme_context_block_info_row($variables) {
  $context = $variables['context'];
  $context_title = l($context->name, 'admin/structure/context/list/' . $context->name . '/edit', array('query' => drupal_get_destination()));
  $context_description = '<div class="description">' . $context->description . '</div>';
  $blocks_output = $variables['blocks'];

  return array(
    ' ' . $context_title . ' ' . $context_description,
    $blocks_output,
  );
}

/**
 * Blocks info for each row.
 */
function theme_context_block_info_blocks($variables) {
  $regions = $variables['regions'];
  $context_list = $variables['context_list'];
  $context = $variables['context'];
  $out = '';

  foreach ($regions as $region_name => $region) {
    $out .= '<div class="region"><div class="label">' . $region_name . '</div><div class="region-inner">';

    foreach ($region as $block_name => $block) {
      $block_url = 'admin/structure/block/manage/' . $block['module'] . '/' . $block['delta'] . '/configure';
      $out .= l($block_name, $block_url, array('query' => drupal_get_destination()));

      $block = block_load($block['module'], $block['delta']);
      $out .= context_block_info_context_list($block, $context_list, $context);
      $out .= '<br />';
    }

    $out .= '<div class="clear-block"></div></div></div>';
  }
  return $out;

}

/**
 * Related blocks info.
 */
function theme_context_block_info_related_contexts($variables) {
  $related_contexts = $variables['related_contexts'];
  $out = '';

  $out .= '<div class="description">';
  $out .= t('Also used in') . ': ';
  $out .= implode(', ', $related_contexts);
  $out .= '</div>';

  return $out;
}

/**
 * Helper function, provides list of contexts where $block is used.
 */
function context_block_info_context_list($block, $context_list, $context_parent) {
  $out = '';
  $counter = 0;
  $related_contexts = array();

  foreach ($context_list as $context_list_tagged) {
    foreach ($context_list_tagged as $context) {
      if (isset($context->reactions['block'])) {
        foreach ($context->reactions['block']['blocks'] as $reaction_block) {
          if ($reaction_block['module'] == $block->module && $reaction_block['delta'] == $block->delta) {
            if ($context->name != $context_parent->name) {
              $related_contexts[] = $context->name . ' (' . $reaction_block['region'] . ')';
              $counter++;
            }
          }
        }
      }
    }
  }

  if ($counter > 0) {
    $out .= theme('context_block_info_related_contexts', array('related_contexts' => $related_contexts));
  }

  return $out;
}
