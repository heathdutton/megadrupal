<?php
function light_get_predefined_param($param, $pre_array = array(), $suf_array = array()) {
  global $theme_key;
  $theme_data = list_themes();
  $result = isset($theme_data[$theme_key]->info[$param]) ? $theme_data[$theme_key]->info[$param] : array();
  return $pre_array + $result + $suf_array;
}

function light_special_regions() {
  $special_regions = array('sidebar' => array(), 'panel' => array());
  global $theme_key;
  $theme_data = list_themes();
  $regions = $theme_data[$theme_key]->info['regions'];
  foreach ($regions as $key => $value) {
  	foreach ($special_regions as $type => $info) {
      if (strpos($key, $type . "_") === 0) {
        $special_regions[$type][] = $key;
      }
  	}
  }
  return $special_regions;
}

function light_panel_regions() {
  $special_regions = light_special_regions();
  $panels = $special_regions['panel'];
  $panel_regions = array();
  foreach($panels as $region) {
  	$parts = explode("_", $region);
  	$panel = $parts[0] . "_" . $parts[1];
  	if(!isset($panel_regions[$panel])) {
  	  $panel_regions[$panel] = array();
  	}
  	$panel_regions[$panel][] = $region;
  }
  return $panel_regions;
}

function light_regions_width($page) {
  $special_regions = light_special_regions();
  $regions_width = array();
  $main_region_width = 12;
  foreach($special_regions['sidebar'] as $region) {
  	if ($markup = render($page[$region])) {
      $regions_width[$region] = theme_get_setting($region . "_width");
      $main_region_width -= $regions_width[$region];
  	}
  	else {
  	  $regions_width[$region] = 0;
  	}
  }
  $regions_width['content'] = $main_region_width;
  foreach($special_regions['panel'] as $region) {
  	$regions_width[$region] = theme_get_setting($region . "_width");
  }
  return $regions_width;
}

