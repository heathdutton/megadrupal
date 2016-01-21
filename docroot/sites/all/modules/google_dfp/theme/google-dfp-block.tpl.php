<?php

/**
 * @file
 * Outputs an ad block.
 *
 * Available variables
 * -------------------
 * $id The ad id
 * $width The ad width in pixels
 * $height The ad height in pixels
 */
?>
<div id="<?php print $id; ?>" class="ad-placement" style="width:<?php print $width ?>;height:<?php print $height ?>;display:none;">
  <div class="ad-label">Advertisement</div>
</div>
