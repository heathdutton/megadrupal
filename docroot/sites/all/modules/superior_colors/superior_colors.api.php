<?php
/**
 * @file
 * Hooks provided by the Superior Colors module.
 */

/**
 * Alter color data before generating CSS files.
 *
 * @param Array $colors
 *   The color data structure.
 */
function hook_superior_colors_generate_alter(&$colors) {
  $colors['content']['body_text']['value'] = '7711DD';
  unset($colors['page']['footer']);
}
