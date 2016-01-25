<?php
/**
 * @file
 * CTools integration for Broadstreet Ads.
 */

/**
 * Implementation of hook_ctools_content_types().
 */
function broadstreet_ads_ad_ctools_content_types() {
  return array(
    'title' => t('Broadstreet Ads'),
    'all contexts' => TRUE,
  );
}

/**
 * Return all content types available.
 */
function broadstreet_ads_ad_content_type_content_types($plugin) {
  $types = array();

  $zones = variable_get('broadstreet_ads_zones', array());

  foreach ($zones as $zone_id => $label) {
    $types[$zone_id] = array(
      'title' => broadstreet_ads_zone_label($zone_id),
      'category' => 'Ads',    
    );
  }

  return $types;
}

/**
 * Implementation of hook_ctools_content_type_render().
 */
function broadstreet_ads_ad_content_type_render($subtype, $conf, $panel_args, $contexts) {
  // Build the arguments to be passed to the template.
  $vars = array(
    'account_id' => variable_get('broadstreet_ads_account_id'),
    'zone' => $subtype,
    'pid' => 0,
  );

  // Build a block-like object to display this pane.
  $block = new StdClass;
  $block->content = theme('broadstreet_ad', $vars);
  $block->title = '';
  $block->delta = $subtype;
  $block->css_class = 'broadstreet-ad broadstreet-' . $subtype;
  return $block;
}

/**
 * Implementation of hook_ctools_content_type_admin_title().
 */
function broadstreet_ads_ad_content_type_admin_title($subtype, $conf, $contexts) {
  return t('Broadstreet Ad');
}

/**
 * Implementation of hook_ctools_content_type_admin_info().
 */
function broadstreet_ads_ad_content_type_admin_info($subtype, $conf, $contexts) {
  $zones = variable_get('broadstreet_ads_zones', array());

  $block = new StdClass;
  $block->title = t('Zone') . ': ' . $zones[$subtype];
  $block->content = t('Display ad zone #!zone_id.', array('!zone_id' => $subtype)) . '<br />'
    . l(t('Manage Broadstreet Ads settings'), 'admin/config/services/broadstreet-ads') . '.';
  return $block;
}
