<?php

/**
 * @file
 * Default theme implementation for google map fields.
 *
 * Available variables:
 * - $name: the display name of the map
 * - $lat: The latitude of the marker for the map being displayed
 * - $lng: The longitude of the marker for the map being displayed
 * - $zoom: The zoom factor of the map being displayed
 */

?>
<div class="google-map-field">
  <div class="google-map-field-label">
    <?php print $name; ?>
  </div>
  <div class="google_map_field_display" data-lat="<?php print $lat; ?>" data-lng="<?php print $lng; ?>" data-zoom="<?php print $zoom; ?>"></div>
</div>
