<?php
/**
 * @file
 * Template for the ecard image formatter.
 * 
 * $class
 *   The CSS class of the main div. Default class "ecard-if" is always added.
 * 
 * $ecard_css
 *   Inline CSS for the eard text from the settings for this field instance.
 */
?>
<div<?php print $class; ?><?php print $id; ?>>
  <?php print $image ?>
  <div class="ecard-if-text" <?php print $ecard_css; ?>>
    <?php print $text ?>
  </div>
</div>
