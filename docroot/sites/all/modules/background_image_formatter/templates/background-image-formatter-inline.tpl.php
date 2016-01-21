<?php
/**
 * @file
 * Default template implementation to display the background inline field.
 *
 * Available variables:
 * - $background_image_selector: (String) The css selector.
 * - $image_url: (String) The absolute url to the image.
 */
?>
<div class="<?php print $background_image_selector ?>" style="background-image: url('<?php print $image_url ?>')">
  &nbsp;
</div>
