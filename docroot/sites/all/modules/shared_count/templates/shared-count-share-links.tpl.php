
<?php
/**
 * @file
 * Share links template
 */
 ?>
<aside class="shared-count-block">
  <div class="total-shares">
    <em class="shared-count"></em>
      <div class="caption"><?php print $caption_text; ?></div>
  </div>
  <div  class="share-container">
    <div  class="shared-count-primary-shares">
      <?php print $primary_items ; ?>
      <div class="shared-count-more-social-networks"><?php print($more_text); ?></div>
    </div>
    <div class="shared-count-secondary-shares">
      <?php print $secondary_items; ?>
    </div>
  </div>
</aside>
