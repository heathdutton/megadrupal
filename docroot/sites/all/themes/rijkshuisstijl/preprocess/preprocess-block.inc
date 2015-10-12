<?php
/**
 * @file
 * Implements hook_preprocess_block().
 */

// Set a default heading level. Can be changed in a subtheme by using
// hook_preprocess_block() and change the variable depending on the
// block ID / module.
$vars['block_heading_level'] = 2;


// Set new Block variables.
// After this, don't use the old ones, they are not used anymore!

$title_classes = &$vars['title_attributes_array']['class'];
$classes = &$vars['classes_array'];

// Depending on which module added the block, we can change the wrapper element
// to either a nav, section, article or div. For instance, when a menu block is
// used, we add the <nav> element instead of a non-semantic <div>.
switch ($vars['elements']['#block']->module) {
  case 'menu':
  case 'menu_block':
    $vars['block_wrapper'] = 'nav';
    $classes[] = 'nav';
    break;

  case 'views':
    $vars['block_wrapper'] = 'section';
    break;

  default:
    $vars['block_wrapper'] = 'div';
}

// For system menus, we want to use the nav element too.
$system_menus = array_merge(menu_list_system_menus(), array(
  'main-menu' => 'Main menu',
  'secondary-menu' => 'Secondary menu',
));
if (in_array($vars['block']->delta, array_keys($system_menus))) {
  $vars['block_wrapper'] = 'nav';
  $classes[] = 'nav';
}

// Add first/last classes on blocks.
$region_count = count(block_list($vars['block']->region));
($vars['block_id'] == 1) ? ($classes[] = 'first') : '';
($vars['block_id'] == $region_count) ? ($classes[] = 'last') : '';

// Set shortcut variables. Hooray for less typing!
$block_id = $vars['block']->module . '-' . $vars['block']->delta;
foreach ($classes as $key => $class) {
  if (in_array($class, array(
    'block',
    'block-system',
    'block-menu',
    'block-views',
    'block-bean',
    ))) {
    unset($classes[$key]);
  }
}


switch ($block_id) {
  // Main-menu in the header.
  case 'system-main-menu':
    $vars['block_html_id'] = 'main-menu';
    break;

  // Search block.
  case 'search-form':
    $vars['block_html_id'] = 'search';
    $classes[] = 'search';
    break;

  // No action needed for nicely named blocks.
  case 'system-main':
    $vars['block_html_id'] = $block_id;
    break;

  default:
    // Set standard ID class for beans.
    if ($vars['block']->module == 'bean') {
      $vars['block_html_id'] = drupal_strtolower($vars['block']->delta);
      break;
    }
    // drupal_set_message(t('Please add %block_id to the template_preprocess_block function', array('%block_id' => $block_id)));
    break;
}

// Hide the blocks subject when the block is placed in the
// "header menu" or "main bar" region.
if (in_array($vars['block']->region, array('header_menu', 'main_bar'))) {
  $title_classes[] = 'element-invisible';
}
