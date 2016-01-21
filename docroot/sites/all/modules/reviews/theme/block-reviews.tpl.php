<?php

/**
 * @file
 * Theme template file used to format the reviews page of enabled content
 * types when there are reviews.
 * $variables:
 *  $nid: the node ID of the main node being viewed.
 *  $review_count: the number of published reviews.
 *  $pending_count: the number of reviews awaiting moderation.
 *  $reviews: array of reviews.
 */

  foreach ($variables['reviews'] as $index => $review):
    print theme('block_review', array(
      'nid' => $variables['nid'],
      'index' => $index,
      'total_reviews' => count($variables['reviews']),
      'review' => $review
    ));
  endforeach;

?>
