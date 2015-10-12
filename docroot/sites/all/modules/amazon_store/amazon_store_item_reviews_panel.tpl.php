<?php
/**
 * @file
 *   template for item_reviews panel
 */
?>

<?php if (!empty($item->CustomerReviews->IFrameURL)): ?>
  <div class="customer_review">
    <iframe class="customer_review_iframe" src="<?php print check_url($item->CustomerReviews->IFrameURL);?>"></iframe>
  </div>
<?php endif; ?>
