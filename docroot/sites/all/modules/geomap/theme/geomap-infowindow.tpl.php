<?php

/**
 * Print the info window information.
 * 
 * Template variables:
 * $windowtext
 *   The rendered microformat windowtext
 * $icon
 *   The rendered microformat for the icon
 * $infowindow_classes
 *   Required. String of pre-defined CSS class tags
 * $infowindow_id
 *   String of the pre-defined id
 */
?>
<?php if($windowtext): ?>
  <div<?php if(strlen($infowindow_id)): ?> id="<?php print $infowindow_id; ?>"<?php endif; ?> class="<?php print $infowindow_classes; ?>">
    <?php print $windowtext; ?>
  </div>
<?php endif; ?>