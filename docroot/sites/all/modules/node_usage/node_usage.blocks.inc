<?php
/***
 * @file
 * site blocks
 */

/***
 * implementats hook_block_info()
 *
 */
function node_usage_block_info() {
  $blocks['node_usage_view'] = array(
    'info'        => t('node usage: Distribution of nodes'),
  );

  return $blocks;
}


/***
 * implementats hook_block_view()
 *
 * @param mixed $delta
 */
function node_usage_block_view($delta = '') {
  $block = NULL;

  switch ($delta) {
    case 'node_usage_view':
      $block['Subject'] = 'Distribution of nodes';

      $block['content'] = _node_usage_node_get_usage();
      break;

    default:
      $block['Subject'] = NULL;
      $block['content'] = NULL;
      break;
  }

  return $block;
}

