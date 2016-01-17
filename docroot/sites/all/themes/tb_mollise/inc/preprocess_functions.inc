<?php
/**
 * @file
 * Override of preprocess functions.
 */

/**
 * Override or insert variables into the node template.
 */
function tb_mollise_preprocess_node(&$vars) {
  $vars['date'] = format_date($vars['created'], 'custom', 'm.d.Y');
}

