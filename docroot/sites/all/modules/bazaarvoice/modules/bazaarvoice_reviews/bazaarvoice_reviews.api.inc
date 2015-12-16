<?php
/**
 * @file
 * bazaarvoice_reviews.api.inc
 *
 * Defines bazaarvoice_reviews hooks that can be used by other modules.
 */

/**
 * Alter the reviews retrieved for a particular product and locale.
 *
 * Invoked from bazaarvoice_reviews_get_reviews() after reviews have been
 * retrieved from Bazaarvoice.
 *
 * @param array &$reviews
 *   Array of reviews. Each review is an associative array of review fields from
 *   Bazaarvoice.  Structure of reviews varies depending on Bazaarvoice account
 *   But will contain at least the following attributes.
 *    - Id : The unique id from Bazaarvoice
 *    - Title: The title (subject) of the review.
 *    - UserNickname: The name of the user who submitted the review.
 *    - SubmissionTime: Date of the review submission
 *    - ReviewText: The body of the review.
 * @param string $product_id
 *   The Bazaarvoice product id.
 * @param string $locale
 *   The Bazaarvoice locale code.
 *
 * @ingroup bazaarvoice_reviews_alter_hooks
 */
function hook_bazaarvoice_reviews_reviews_alter(array &$reviews, $product_id, $locale) {
  // Force all review titles to be all caps.
  foreach ($reviews as &$review) {
    if (isset($review['Title'])) {
      $review['Title'] = strtoupper($review['Title']);
    }
  }
}

/**
 * Allows other modules to perform actions when reviews are retrieved for a
 * product and locale.
 *
 * Invoked from bazaarvoice_reviews_get_reviews() after reviews have been
 * retrieved from Bazaarvoice.
 *
 * @param string $product_id
 *   The Bazaarvoice product id.
 * @param string $locale
 *   The Bazaarvoice locale code.
 *
 * @ingroup bazaarvoice_reviews_hooks
 * @ingroup hooks
 */
function hook_bazaarvoice_reviews_get_reviews($product_id, $locale) {
  // Log that reviews were retrieved.
  watchdog('bazaarvoice_reviews', 'Reviews retrieved for Product !product_id in locale !locale', array(
    '!product_id' => $product_id,
    '!locale' => $locale,
  ));
}
