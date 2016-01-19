<?php

/**
 * The wrapper for the geolocations.
 * 
 * Template variables:
 * $infowindow
 *   The rendered microformat infowindow
 * $icon
 *   The rendered microformat for the icon
 * $marker_classes
 *   Required. String of pre-defined CSS class tags
 * $marker_id
 *   String of the pre-defined id
 */
?>
<div<?php if(strlen($marker_id)): ?> id="<?php print $marker_id; ?>" <?php endif; ?> class="<?php print $marker_classes; ?>">
  <?php if($infowindow): ?>
    <?php print $infowindow; ?>
  <?php endif; ?>
  <?php if($icon): ?>
    <?php print $icon; ?>
  <?php endif; ?>
</div>