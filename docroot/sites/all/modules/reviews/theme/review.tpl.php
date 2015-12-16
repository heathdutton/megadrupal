<?php

/**
 * @file
 * Theme template file used to format the reviews page of enabled content
 * types when there are reviews.
 * $variables:
 *  $nid: the node ID of the main node being viewed.
 *  $reviews: array of reviews
 *    $rid: review ID.
 *    $uid: user ID of reviewer.
 *    $review: review text.
 *    $rating: rating value from 0 to 100
 *    $created: timestamp of when the review was created.
 */
  global $user;

  $use_rating = variable_get('reviews_use_rating') and module_exists('firestar');

  $review = $variables['review'];
  $review_content = unserialize($review->review);
  if ($use_rating) {
    $rating = $review->rating;
  }
  $review_nid = $review->nid;
  $reviewer_uid = $review->uid;
  $classes = '';

  $rating_widget = variable_get('reviews_rating_star_widget','default');
  $default_widget = $rating_widget == 'default';

  if ($variables['index'] == 0):
    $classes .= ' first';
  endif;

  if ($variables['index'] == (int) ($variables['total_reviews'] - 1)):
    $classes .= ' last';
  endif;

  if ($variables['index'] % 2 == 1):
    $classes .= ' even';
  else:
    $classes .= ' odd';
  endif;

  if ($review->status == 0):
    $classes .= ' unpublished';
  endif;

  if ($use_rating and $rating){
    //create Fivestar rating
    $path = drupal_get_path('module', 'fivestar');
    drupal_add_js($path . '/js/fivestar.js');
    drupal_add_css($path . '/css/fivestar.css');
    if (!$default_widget)
      drupal_add_css($rating_widget);
    $widgets = module_invoke_all('fivestar_widgets');

    $variables = array(
      'rating' => $rating,
      'stars' => variable_get('reviews_rating_star_count', 5),
      'widget' => array(
        'name' => $default_widget ? 'default' : strtolower($widgets[$rating_widget]),
        'css' => 'active',
      ),
    );
    $star_display = theme('fivestar_static', $variables);
    $fivestar = theme('fivestar_static_element', array('description' => '', 'star_display' => $star_display));
  }

?>
<div class="reviews-review <?php print $classes; ?>">
  <a name="review_<?php print $review->rid; ?>"></a>
  <div class="reviews-date-author">
    <span class="date"><?php print format_date($review->created, 'long'); ?></span> <?php print t('by'); ?> <span class="author"><?php print reviews_get_username($review->uid); ?></span>
  </div>
  <div class="reviews-content">
    <?php print check_markup($review_content['value'], $review_content['format']); ?>
  </div>
  <?php if ($use_rating and $rating) print $fivestar; ?>
  <?php if ($user->uid == $reviewer_uid && user_is_logged_in()) { ?>
    <div class="reviews-action-links">
      <?php print l(t('edit'), 'node/' . $review_nid . '/edit-review/' . $review->rid); ?>
    </div>
  <?php } ?>
</div>
