<?php
/**
 * @file
 * Openlayers-geocoder-result-html.tpl.php
 * Template file theming geosearch's response results.
 */
$output = '';

$output .= '<ul class="openlayers-geosearch-result-struct panels">';

// We are going to give every feature an id, this should
// never happen in a template file, so TODO fix this.
$fid = 1;
foreach ($locations as $location) {
  $firstcolum = TRUE;
  $output .= '<li>';
  $output .= '<a class="openlayers-geosearch-result-link" href="?lat=' . $location->coords[0] . '&lon=' . $location->coords[1] . '&fid=' . $provider . '_' . $fid . '">' . $location->data['geocoder_formatted_address'] . '</a>';

  if (isset($locations[0]->data['geocoder_formatted_address'])) {
    $output .= '<div class="openlayers-geosearch-result-address">' . $locations[0]->data['geocoder_formatted_address'] . '</div>';
  }
  elseif (isset($locations[0]->data['geocoder_address_components'])) {
    $component = $location->data['geocoder_address_components'][0];
    $output .= '<div class="openlayers-geosearch-result-address">' . $locations[0]->data['long_name'] . '</div>';
    $types = "";
    if (isset($component->types)) {
      foreach($component->types as $type) {
        $types .= $type . ", ";
      }
      $types = substr($types, 0, strlen($types) - 2);
    }
    $output .= '<div class="openlayers-geosearch-result-types">' . $types . '</div>';
  }
  $output .= '</li>';
  $firstcolum = TRUE;
  $fid++;
}
$output .= "</ul>";

echo $output;
