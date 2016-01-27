<?php

/**
 * @file
 * Theme template file used to display a block containing all reviews
 * submitted by the current user.
 * available variables:
 *  - $reviews: array of reviews.
 */

  foreach ($variables['reviews'] as $index => $review):
    print theme('block_review', array(
      'index' => $index,
      'total_reviews' => count($variables['reviews']),
      'review' => $review
    ));
  endforeach;

?>
