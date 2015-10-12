<?php
/**
 * @file
 * Openlayers-geocoder-result-html.tpl.php
 * Template file theming geosearch's response results.
 */
$output = '';

$output .= '<table class="openlayers-geosearch-result-struct tabs">';

$output .= "<tr>";

isset($locations[0]->data['geocoder_address_components']) ? $output .=
"<th>" . t('Name') . "</th><th>" . t('Address') . "</th>
<th>" . t('Type') . "</th>" : $output .= "";

$output .= "</tr>";
// We are going to give every feature an id, this should
// never happen in a template file, so TODO fix this.
$fid = 1;
foreach ($locations as $location) {
  $firstcolum = TRUE;
  $output .= "<tr>";
  $output .= '<td><a class="openlayers-geosearch-result-link" href="?lat=' . $location->coords[0] . '&lon=' . $location->coords[1] . '&fid=' . $provider . '_' . $fid . '">' . $location->data['geocoder_formatted_address'] . '</a></td>';

  if (isset($locations[0]->data['geocoder_address_components'])) {
    $component = $location->data['geocoder_address_components'][0];
    $output .= '<td>' . $component->long_name . '</td>';
    $types = "";
    if (isset($component->types)) {
      foreach($component->types as $type) {
        $types .= $type . ", ";
      }
      $types = substr($types, 0, strlen($types) - 2);
    }
    $output .= "<td>" . $types . "</td>";
  }
  $output .= "</tr>";
  $firstcolum = TRUE;
  $fid++;
}
$output .= "</table>";

echo $output;
