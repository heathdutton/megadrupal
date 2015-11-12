<?php
/**
 * @file
 * Default theme implementation for a Bazaarvoice product statistics.
 */
?>
<div class="bazaarvoice-statistics" >
  <span class="bazaarvoice-statistics review-statistics">
    <strong><?php print t('Reviews'); ?>:</strong> <?php print $review_count; ?>
  </span>
  <span class="bazaarvoice-statistics rating-statistics">
    <strong><?php print t('Rating'); ?>:</strong> <?php print $rating; ?>
  </span>
</div>
