<?php

/**
 * @file
 * Theme template file used to format the reviews page of enabled content
 * types when there are no reviews.
 * $variables:
 *  nid: the node ID of the main node being viewed.
 */

?>
<div class="reviews no-reviews">
  <div class="clearfix">
    <p>
      <?php print t('There are currently no reviews for this content.'); ?>
    </p>
    <p>
      <?php print t('Why not be the first to review it');?> - <?php print l(t('click here'), 'node/' . $variables['nid'] . '/add-review'); ?>
    </p>
  </div>
  <div class="reviews-back-to-page">
    <?php print l(t('Back to page'), 'node/' . $variables['nid']); ?>
  </div>
</div>
