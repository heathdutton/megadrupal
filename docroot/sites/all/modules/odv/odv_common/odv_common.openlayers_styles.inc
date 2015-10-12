<?php
/**
 * @file
 * odv_common.openlayers_styles.inc
 */

/**
 * Implements hook_openlayers_styles().
 */
function odv_common_openlayers_styles() {
  $export = array();

  $openlayers_styles = new stdClass;
  $openlayers_styles->disabled = FALSE; /* Edit this to true to make a default openlayers_styles disabled initially */
  $openlayers_styles->api_version = 1;
  $openlayers_styles->name = 'odv_map_default_style';
  $openlayers_styles->title = 'ODV default style';
  $openlayers_styles->description = 'ODV default style';
  $openlayers_styles->data = array(
    'pointRadius' => 8,
    'fillColor' => '#333333',
    'strokeColor' => '#FFFFFF',
    'strokeWidth' => 2,
    'fillOpacity' => 0.9,
    'strokeOpacity' => 1,
    'strokeLinecap' => 'round',
    'strokeDashstyle' => 'solid',
    'graphicOpacity' => 1,
    'labelAlign' => 'cm',
  );
  $export['odv_map_default_style'] = $openlayers_styles;

  return $export;
}
