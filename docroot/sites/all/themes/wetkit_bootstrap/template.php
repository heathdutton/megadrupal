<?php
/**
 * @file
 * template.php
 */

/**
 * Allows sub-themes to alter the array used for colorizing text.
 *
 * @param array $texts
 *   An associative array containing the text and classes to be matched, passed
 *   by reference.
 *
 * @see _bootstrap_colorize_text()
 */
function wetkit_bootstrap_bootstrap_colorize_text_alter(&$texts) {

  // Replace default class for search.
  $wxt_active = variable_get('wetkit_wetboew_theme', 'wet-boew');
  $wxt_active = str_replace('-', '_', $wxt_active);
  $wxt_active = str_replace('theme_', '', $wxt_active);

  if ($wxt_active == 'gcweb') {
    $texts['contains'][t('Search')] = 'primary btn-small';
  }
  else {
    $texts['contains'][t('Search')] = 'default';
  }
}

/**
 * Allows sub-themes to alter the array used for associating an icon with text.
 *
 * @param array $texts
 *   An associative array containing the text and icons to be matched, passed
 *   by reference.
 *
 * @see _bootstrap_iconize_text()
 */
function wetkit_bootstrap_bootstrap_iconize_text_alter(&$texts) {
  // Change the icon that matches "Upload".
  $texts['contains'][t('Upload')] = 'upload';
}

/**
 * Implements hook_block_view_alter().
 *
 * See: https://gist.github.com/gagarine/3201854
 */
function wetkit_bootstrap_block_view_alter(&$data, $block) {
  // Check we get the right menu block (side bar)
  if ($block->delta == 'MENU-NAME') {
    // change the theme wrapper for the first level
    $data['content']['#theme_wrappers'][] = array('menu_tree__menu_block__1__level1');
  }
}

/**
 * Implements hook_css_alter().
 */
function wetkit_bootstrap_css_alter(&$css) {
  $theme_path = drupal_get_path('theme', 'bootstrap');
  $wxt_loaded = TRUE;
  if ($wxt_loaded) {

    // Add overrides.
    $override = $theme_path . '/css/overrides.css';
    $css[$override] = array(
      'data' => $override,
      'type' => 'file',
      'every_page' => TRUE,
      'media' => 'all',
      'preprocess' => TRUE,
      'group' => CSS_THEME,
      'browsers' => array('IE' => TRUE, '!IE' => TRUE),
      'weight' => -1,
    );
  }
}
