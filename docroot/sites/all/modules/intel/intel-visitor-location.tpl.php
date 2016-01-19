<?php

/**
 * @file
 * Default theme implementation to present a picture configured for the
 * user's account.
 *
 * Available variables:
 * - $user_picture: Image set by the user or the site's default. Will be linked
 *   depending on the viewer's permission to view the user's profile page.
 * - $account: Array of account information. Potentially unsafe. Be sure to
 *   check_plain() before use.
 *
 * @see template_preprocess_user_picture()
 *
 * @ingroup themeable
 */
?>
<script>
function initialize_map_$map_index() {
	  var mapOptions = {
	    zoom: 4,
	    center: new google.maps.LatLng(<?php print $center['lat']; ?>, <?php print $center['lon']; ?>),
	    disableDefaultUI: true,
	    mapTypeId: google.maps.MapTypeId.ROADMAP
	  };

	  var map = new google.maps.Map(document.getElementById('map-canvas-$map_index'),
	      mapOptions);
	      
	  var circleOptions = {
	    strokeColor: '#FF0000',
	    strokeOpacity: 0.8,
	    strokeWeight: 1,
	    fillColor: '#FF0000',
	    fillOpacity: 0.35,
	    map: map,
	    center: new google.maps.LatLng(<?php print $center['lat']; ?>, <?php print $center['lon']; ?>),
	    radius: 50000
	  };
	  locCircle = new google.maps.Circle(circleOptions);
	}
	google.maps.event.addDomListener(window, 'load', initialize_map_<?php print $map_index ; ?>);
</script>
<div class="google-map"><div id="map-canvas-<?php print $map_index ; ?>" class="map-canvas"></div></div>

<div class="summary-item <?php if (isset($class)) { print $class; } ?>">
  <div class="summary-item-box">
    <div class="value"><?php print $value; ?></div>
    <div class="title"><?php print $title; ?></div>
  </div>
</div>

  static $map_index;
  if (!isset($map_index)) {
    $map_index = 0;
  }
  $locations = $vars['locations'];

  $div_id = 'map_div_' . $map_index;
  // check if single element was passed
  if (isset($locations['latitude'])) {
    $locations = array(
      $locations,
    );
  }
  $mode = 1;
  $output = '';
  if ($mode == 1) {
    $options = array('type' => 'external', 'weight' => -1);
    drupal_add_js('https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false', $options);
    $locstr = '';
    $center = array('lat' => 0, 'lon' => 0, );
    foreach ($locations AS $loc) {
      if (!isset($loc['lat']) && isset($loc['latitude'])) {
        $loc['lat'] = $loc['latitude'];
      }
      if (!isset($loc['lon']) && isset($loc['longitude'])) {
        $loc['lon'] = $loc['longitude'];
      }
      $locstr .= "[" . $loc['lat'] . ", " . $loc['lon'] . ", " . "'" . (isset($loc['name']) ? $loc['name'] : '') . "'],\n";
      $center['lat'] = $loc['lat']; 
      $center['lon'] = $loc['lon']; 
    }
    $locstr = substr($locstr, 0, -1); // chop last comma
    //$output .= <<<EOT
    $script = <<<EOT
function initialize_map_$map_index() {
  var mapOptions = {
    zoom: 4,
    center: new google.maps.LatLng({$center['lat']}, {$center['lon']}),
    disableDefaultUI: true,
    mapTypeId: google.maps.MapTypeId.ROADMAP
  };

  var map = new google.maps.Map(document.getElementById('map-canvas-$map_index'),
      mapOptions);
      
  var circleOptions = {
    strokeColor: '#FF0000',
    strokeOpacity: 0.8,
    strokeWeight: 1,
    fillColor: '#FF0000',
    fillOpacity: 0.35,
    map: map,
    center: new google.maps.LatLng({$center['lat']}, {$center['lon']}),
    radius: 50000
  };
  locCircle = new google.maps.Circle(circleOptions);
}
google.maps.event.addDomListener(window, 'load', initialize_map_$map_index);
EOT;
    drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
    $output = '<div class="google-map"><div id="map-canvas-' . $map_index . '" class="map-canvas"></div></div>' . "\n";
    $map_index++;
  } 
  return $output; 