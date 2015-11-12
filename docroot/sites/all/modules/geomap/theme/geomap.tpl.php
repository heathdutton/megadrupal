<?php

/**
 * Print all geolocations. The result must conform to the
 * example given in README.txt of this module, otherwise
 * the map will not render
 * 
 * Template variables:
 * //NODE SPECIFIC
 * $classes
 *   String of pre-generated classes
 * $mapid
 *   The generated map id (required)
 */
?>
<?php if(isset($mapid)): ?>
  <?php if(isset($loader_image)): ?><div id="map-loader-image" style="display: none;" ><?php print $loader_image; ?></div><?php endif; ?>
  <div id="<?php print $mapid; ?>"<?php if(strlen($classes)): ?> class="<?php print $classes; ?>"<?php endif; ?>></div>
<?php endif; ?>