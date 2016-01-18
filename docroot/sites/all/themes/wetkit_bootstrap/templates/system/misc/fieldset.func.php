<?php
/**
 * @file
 * fieldset.func.php
 */

/**
 * Overrides theme_fieldset().
 */
function wetkit_bootstrap_fieldset($variables) {
  return theme('bootstrap_panel', $variables);
}
