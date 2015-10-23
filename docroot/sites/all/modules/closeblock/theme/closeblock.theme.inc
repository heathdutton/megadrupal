<?php

/**
 * Get block id
 * 
 * return
 *  Block id string
 */
function theme_closeblock_block_id($vars) {
  return 
    'block-' . str_replace('_', '-', $vars['module']) . '-' . str_replace('_', '-', $vars['delta']);
}