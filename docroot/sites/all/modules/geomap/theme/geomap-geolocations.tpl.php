<?php

/**
 * The wrapper for the geolocations.
 * 
 * Template variables:
 * $geolocations
 *   Array of all rendered geolocations
 * $geolocations_classes
 *   String of pre-defined CSS class tags
 * $geolocations_id
 *   String of the pre-defined id
 */
?>
<?php if(!empty($geolocations)): ?>
  <div<?php if(strlen($geolocations_id)): ?> id="<?php print $geolocations_id; ?>" <?php endif; ?><?php if(strlen($geolocations_classes)): ?> class="<?php print $geolocations_classes; ?>" <?php endif; ?>>
    <?php foreach($geolocations as $geolocation){ ?>
      <?php print $geolocation; ?>
    <?php } ?>
  </div>
<?php endif; ?>