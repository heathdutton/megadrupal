<?php
/**
 * @file
 * Preproccess functions for Comment element.
 */

/**
 * Implements da_vinci_preprocess_comment().
 */
function da_vinci_preprocess_comment(&$vars) {
  // Overridden comment created data with time ago:
  $vars['created_ago'] = format_interval(time() - $vars['comment']->created, 1);
}
