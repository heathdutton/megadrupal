<?php

/**
 * Print the info window information.
 * 
 * Template variables:
 * $src
 *   The address (relative or full) to the (png) icon image source
 * $options
 *   Array of options. Each option REQUIRES $option_classes, $option_name, $option_value
 * $icon_classes
 *   Required. String of pre-defined CSS class tags
 * $icon_id
 *   String of the pre-defined id
 */
?>
<?php if(isset($src)): ?>
  <div<?php if(isset($icon_id)): ?> id="<?php print $icon_id; ?>"<?php endif; ?> class="<?php print $icon_classes; ?>" src="<?php print $src; ?>">
    <?php if(isset($icon_options)): ?>
      <?php foreach($icon_options as $option): ?>
        <div class="<?php print $option['option_classes']; ?>" name="<?php print $option['option_name']; ?>" value="<?php print $option['option_value']; ?>" ></div>
      <?php endforeach; ?>
    <?php endif; ?>
  </div>
<?php endif; ?>