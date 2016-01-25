<?php
  /**
   * Theme the Google Map InfoWindow text
   *
   * Template variables:
   * $text
   *   Filtered output text
   * $text_classes
   *   String of pre-defined CSS class tags
   * $text_id
   *   String of the pre-defined id
   */
  
?>
<?php if(strlen($text)): ?>
  <h4<?php if(strlen($text_id)): ?> id="<?php print $text_id; ?>"<?php endif; ?> class="<?php print $text_classes; ?>"><?php print $text; ?></h4>
<?php endif; ?>