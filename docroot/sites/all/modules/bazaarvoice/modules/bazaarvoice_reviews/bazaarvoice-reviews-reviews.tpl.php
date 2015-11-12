<?php
/**
 * @file
 * Default theme implementation for a Bazaarvoice reviews.
 *
 * Available variables:
 * - $review_link: Link to the write a review page.
 * - $reviews: An array of rendered Bazaarvoice reviews.
 * - $pager: A pager object for paging reviews.
 */
?>
<div id="bazaarvoice-reviews">
  <div id="bazaarvoice-reviews-link">
    <?php print $review_link; ?>
  </div>
  <div id="bazaarvoice-reviews-list">
    <?php if($reviews): ?>
      <?php foreach($reviews as $review): ?>
        <?php print render($review); ?>
      <?php endforeach; ?>
      <?php if($pager): ?>
        <?php print render($pager); ?>
      <?php endif; ?>
    <?php else: ?>
      <?php print t('There are no reviews for this product.'); ?>
    <?php endif; ?>
  </div>
</div>
