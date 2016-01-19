<?php
/**
 * Created by PhpStorm.
 * User: thaohuynh
 * Date: 11/28/14
 * Time: 15:46
 */

$zones = zonepanels_default_panels_config();
$zones_regions = array();

for ($i = 1; $i <= $zones['rows']; $i++) {
  for ($j = 1; $j <= $zones['cols']; $j++) {
    $zones_regions["zone_{$i}_{$j}"] = t("Z-{$i} R-{$j}");
  }
}


$plugin = array(
  'title'     => t('ZONESTRAP'),
  'category'  => t('ZONES'),
  'icon'      => 'zonestrap.png',
  'theme'     => 'zonestrap',
  'admin css' => 'zones-layouts-admin.css',
  'regions'   => $zones_regions,
);

/**
 * Preprocess zone panels layout.
 *
 */
function template_preprocess_zonestrap(&$variables){
  if(!isset($variables['display']->layout_settings['zonepanels'])){
    $variables['display']->layout_settings['zonepanels'] = zonepanels_default_panels_config();
  }

  if ($variables['renderer']->admin) {
    $variables['admin_panels'] = TRUE;
  }
  else {
    $variables['admin_panels'] = FALSE;
    $variables['panel_prefix'] = $variables['display']->layout_settings['zonepanels']['wrapper']['prefix'];
    $variables['panel_suffix'] = $variables['display']->layout_settings['zonepanels']['wrapper']['suffix'];
  }
}