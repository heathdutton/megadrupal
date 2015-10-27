<?php // $Id$ 
 // Conditionally load the Superfish JS

/**
 * Override or insert PHPTemplate variables into the page templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("page" in this case.)
 */

/**
 * Override or insert PHPTemplate variables into the block templates.
 *
 * @param $vars
 *   A sequential array of variables to pass to the theme template.
 * @param $hook
 *   The name of the theme function being called ("block" in this case.)
 */
function teleco_preprocess_block(&$vars, $hook) {
  $block = $vars['block'];

  // Special classes for blocks
  $block_classes = array();
  $block_classes[] = 'block-'. $block->module;
  $block_classes[] = 'region-'. $vars['block_zebra'];
  $block_classes[] = $vars['zebra'];
  $block_classes[] = 'region-count-'. $vars['block_id'];
  $block_classes[] = 'count-'. $vars['id'];
  $vars['block_classes'] = implode(' ', $block_classes);

}

/**
 * Converts a string to a suitable html ID attribute.
 *
 * - Preceeds initial numeric with 'n' character.
 * - Replaces space and underscore with dash.
 * - Converts entire string to lowercase.
 * - Works for classes too!
 *
 * @param string $string
 *   The string
 * @return
 *   The converted string
 */
function teleco_id_safe($string) {
  if (is_numeric($string{0})) {
    // If the first character is numeric, add 'n' in front
    $string = 'n'. $string;
  }
  return strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $string));
}







function teleco_preprocess_page(&$vars) {
      $vars['mainmenu'] = theme('links', array('links' => $vars['main_menu'], 'attributes' => array('id' => 'primary', 'class' => array('links', 'clearfix', 'main-menu'))));
      $vars['secondarylinks'] = theme('links', array('links' => $vars['secondary_menu'], 'attributes' => array('id' => 'secondary', 'class' => array('links', 'clearfix', 'secondary-menu'))));

}


