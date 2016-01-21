<?php

/**
 * Print all geolocations. The result must conform to the
 * example given in README.txt of this module, otherwise
 * the map may not render
 * 
 * Template variables:
 * //NODE SPECIFIC
 * $nid
 *   The referenced node id
 * $type
 *   The referenced node type
 * $title
 *   The referenced node title or simply a title
 * $path
 *   The nice Drupal path instead of defaulting to ./node/ABC
 *
 * //Marker Specific
 * $marker
 *  The rendered marker object
 *
 * //GEO SPECIFIC
 * $latitude
 *   Required. The latitude.
 * $longitude 
 *   Required. The longitude.
 * $latitude_display
 *   Required. The latitude.
 * $longitude_display 
 *   Required. The longitude.
 * $geolocation_classes
 *   Required. String of pre-defined CSS class tags. For the GEOMAP JS to work, it must have geo as one class
 * $geolocation_id
 *   String of the pre-defined id
 */
?>
<?php if(isset($latitude) || isset($longitude)): ?>
  <div<?php if(isset($geolocation_id)): ?>id="<?php print $geolocation_id; ?>" <?php endif; ?> class="<?php print $geolocation_classes; ?>"<?php if($title): ?> title="<?php print $title; ?>"<?php endif; ?>>
    <?php if(isset($title_display)): ?><?php print $title_display; ?><?php endif; ?>
    <span class="latitude" title="<?php print $latitude; ?>"><?php if($latitude_display): ?><?php print $latitude_display; ?><?php endif; ?></span>
    <span class="longitude" title="<?php print $longitude; ?>"><?php if($longitude_display): ?><?php print $longitude_display; ?><?php endif; ?></span>
    <?php if($marker): ?><?php print $marker; ?><?php endif; ?>
    <?php if($nid): ?>
      <div class="link" name="link"<?php if($nid): ?> nid="<?php print $nid; ?>"<?php endif; ?><?php if($path): ?> link="<?php print $path; ?>"<?php endif; ?>></div>
    <?php endif; ?>
  </div>
<?php endif; ?>