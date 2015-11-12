<?php
/**
 * @file
 * Default theme implementation for a Bazaarvoice review.
 *
 * Available variables:
 * - $review: An array of review fields from Bazaarvoice. Will vary depending
 *   on Bazaarvoice account setup.  Will contain at a minimum the following
 *   items:
 *    - $review['Id']: The unique id for this review.
 *    - $review['Title']: The title of the review.
 *    - $review['UserNickname']: The name of the user who submitted the review.
 *    - $review['SubmissionTime']: The data the review was submitted.
 *    - $review['ReviewText']: The content of the review.
 *    - $review['Rating']: The rating value given for the review.
 *    - $review['RatingRange']: The max value that can be given for a review.
 */
?>
<div class="bazaarvoice-review" id="bazaarvoice-review-<?php print $review['Id']; ?>">
  <div class="bazaarvoice-head">
    <h2 class="bazaarvoice-review-title"><?php print $review['Title']; ?></h2>
    <?php print ' ' . t('By') . ' '; ?>
    <span class="bazaarvoice-review-author"><?php print $review['UserNickname']; ?></span>
    -
    <span class="bazaarvoice-review-date"><?php print date('m/d/Y', strtotime($review['SubmissionTime'])); ?></span>
    <br />
    <span class="bazaarvoice-review-rating">
      <?php print t('Rating: @numerator/@denominator', array('@numerator' => $review['Rating'], '@denominator' => $review['RatingRange'])); ?>
    </span>
  </div>
  <div class="bazaarvoice-review-body">
    <p class="bazaarvoice-review-text">
      <?php print $review['ReviewText']; ?>
    </p>
  </div>
</div>
