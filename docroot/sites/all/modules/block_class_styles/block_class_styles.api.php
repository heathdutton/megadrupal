<?php
/**
 * @file
 * Defines the api functions of the module
 *
 * @ingroup block_class_styles Block Class Presets
 * @{
 */

/**
 * Implements hook_block_class_styles_info_alter().
 */
function hook_block_class_styles_info_alter(&$presets) {

}

/**
 * Implements hook_preprocess_block().
 *
 * An example to show how you can add a style group suggestion tpl based
 * on some logic, in this case the machine name of the style.
 *
 * The end result is that all styles that begin with panel_ will be given one
 * additional tpl suggestion
 *
 *   block--panel.tpl.php
 */
function hook_preprocess_block(&$vars) {
  $style = isset($vars['block']->block_class_styles) ? $vars['block']->block_class_styles : NULL;

  // Detects if our style suggests this is a panel block.
  if ($style && preg_match('/^panel_/', $style->name)) {
    $vars['theme_hook_suggestions'][] = 'block__panel';
  }
}