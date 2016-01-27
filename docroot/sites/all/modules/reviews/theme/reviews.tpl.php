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

global $user;

?>
<div class="reviews">

  <p>
    <?php print t('The following reviews have been left for this content:'); ?>
  </p>

  <?php

    foreach ($variables['reviews'] as $index => $review):
      print theme('review', array(
        'nid' => $variables['nid'],
        'index' => $index,
        'total_reviews' => $variables['review_count'],
        'review' => $review
      ));
    endforeach;

  ?>
  
  <?php print render($pager); ?>

  <?php if (count($reviews) != 0) { ?>

    <div class="reviews-count-info">
      <div class="reviews-published-count">
        <?php
          if ($variables['review_count'] == 1):
            print t('There is 1 published review for this content.');
          endif;

          if ($variables['review_count'] != 1):
            print t('There are !num published reviews for this content.', array('!num' => $variables['review_count']));
          endif;
        ?>
      </div>

      <div class="reviews-pending-count">
        <?php
          if ($variables['pending_count'] == 1):
            print t('There is 1 pending review for this content.');
          endif;

          if ($variables['pending_count'] != 1):
            print t('There are !num pending reviews for this content.', array('!num' => $variables['pending_count']));
          endif;
        ?>
      </div>

    </div>

  <?php } ?>

  <?php if (!reviews_check_user_review($nid, $user->uid)) { ?>

    <div class="reviews-add-review">
      <?php print l(t('Add review'), 'node/' . $nid . '/add-review'); ?>
    </div>

  <?php } ?>

  <div class="reviews-back-to-page">
    <?php print l(t('Back to page'), 'node/' . $nid); ?>
  </div>

</div>
