<?php

$vars['title_attributes_array']['class'][] = 'block-title';


/**
 * @code We move grid- classes to the front so that we can use the CSS |= attr selector
 */
foreach ($vars['classes_array'] as $key => $class) {
  if (strpos($class, 'grid') !== FALSE) {
    unset($vars['classes_array'][$key]);
    $vars['classes_array'][0] = $class . ' ' . $vars['classes_array'][0];
  }
}

/**
 * @code for certain regions it makes more sense to stack blocks horizontally rather than vertically
 * @todo give configurator per-region per-@media control over this.
 */
$region_count = count(block_list($vars['block']->region));
$dynamic_regions = array('preblocks', 'postblocks', 'footer');
if (in_array($vars['block']->region, $dynamic_regions)) {
  $frac = '1-' . min('8', $region_count);
  if ($region_count > 1) {
    $vars['classes_array'][0] = 'grid-' . $frac . ' ' . $vars['classes_array'][0];
  }
}

// Special classes for blocks
$vars['classes_array'][] = ($vars['block_id'] == 1) ? 'first' : '';
$vars['classes_array'][] = ($vars['block_id'] == $region_count) ? 'last' : '';

switch ($vars['elements']['#block']->module) {
  case 'menu':
  case 'menu_block':
    $vars['block_el'] = 'nav';
    break;

  case 'search':
    $vars['block_el'] = 'div';
    break;

  case 'views':
    $vars['block_el'] = 'section';
    break;

  default:
    $vars['block_el'] = 'article';
}

$nav_blocks = array('navigation', 'main-menu', 'management', 'user-menu', 'superfish', 'nice_menus', 'superfish', 'nice_menus', 'popular-block');
if (in_array($vars['block']->delta, $nav_blocks)) {
  $vars['block_el'] = 'nav';
}
