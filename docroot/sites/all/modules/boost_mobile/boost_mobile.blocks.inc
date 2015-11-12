<?php

/**
 * @file
 * Blocks output functions for the boost mobile module.
 */

/**
 * Display the links to switch site's version.
 *
 * See @boost_mobile_block_view().
 */
function boost_mobile_block_view_switcher() {
  $bm_dir = variable_get('boost_mobile_dir', BOOST_MOBILE_DIR);
  $boostpath = $_SERVER['boostpath'];

  // Renderable array for the link to switch to mobile display.
  // You must not use t() for the ID attributes and query options.
  $switch_to_mobile = array(
    '#theme' => 'link',
    '#text' => t('Mobile version'),
    '#path' => current_path(),
    '#options' => array(
      'attributes' => array(
        'class' => array('boost-mobile-switch'),
        'id' => 'bm-switch-to-mobile',
      ),
      'html' => FALSE,
      'query' => array('bm_version' => 'mobile'),
    ),
  );

  // Renderable array for the link to switch to desktop display.
  // You must not use t() for the ID attributes and query options.
  $switch_to_desktop = array(
    '#theme' => 'link',
    '#text' => t('Desktop version'),
    '#path' => current_path(),
    '#options' => array(
      'attributes' => array(
        'class' => array('boost-mobile-switch'),
        'id' => 'bm-switch-to-desktop',
      ),
      'html' => FALSE,
      'query' => array('bm_version' => 'desktop'),
    ),
  );

  // We propose the right switch link depending on the display we are on.
  if ($boostpath == $bm_dir) {
    $switch = $switch_to_desktop;
  }
  else {
    $switch = $switch_to_mobile;
  }

  $block['subject'] = t("Change site's display");
  if (!empty($switch)) {
    $block['content'] = $switch;
  }

  return $block;
}
