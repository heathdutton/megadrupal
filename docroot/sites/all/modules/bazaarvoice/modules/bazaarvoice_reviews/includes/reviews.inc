<?php
/**
 * @file
 * Functionality for displaying Bazaarvoice reviews.
 */

/**
 * Display Reviews for a product.
 */
function bazaarvoice_reviews_view_reviews($type, $entity) {
  module_load_include('module', 'bazaarvoice_products');
  $content = '';
  // Attempt to get product info.
  if ($product_info = bazaarvoice_products_get_product_info($type, $entity)) {
    $review_link = '';
    // User have access to provide reviews?
    if (user_access('submit bazaarvoice reviews')) {
      $review_link = bazaarvoice_reviews_product_write_a_review_link($product_info['type'], $product_info['eid']);
    }

    // Attempt to retrieve arrays for this product/locale.
    if ($reviews = bazaarvoice_reviews_get_reviews($product_info['product_id'])) {
      $content = array(
        '#theme' => 'bazaarvoice_reviews_reviews',
        '#reviews' => array(),
        '#product_info' => $product_info,
        '#review_link' => $review_link,
        '#pager' => array(
          '#theme' => 'pager',
          '#quantity' => count($reviews),
        ),
      );
      // Get how many reviews to display per page.
      $per_page = variable_get('bazaarvoice_reviews_per_page', 10);
      // Initialize a pages.
      $current_page = pager_default_initialize(count($reviews), $per_page);
      // Split the current reviews into chunks.
      $chunks = array_chunk($reviews, $per_page, TRUE);
      foreach ($chunks[$current_page] as $review) {
        $content['#reviews'][] = array(
          '#theme' => 'bazaarvoice_reviews_review',
          '#review' => $review,
        );
      }

      // Template the reviews.
      $content = render($content);
    }
    // No reviews, display a message.
    else {
      $content = t('There are no reviews. !review_link', array(
        '!review_link' => $review_link,
      ));
    }
  }

  return $content;
}

/**
 * Retrieve all reviews for a product.
 *
 * @param string $product_id
 *   The product Id.
 * @param bool $locale
 *   The locale to retrieve reviews for.
 *
 * @return array|bool
 *   Array of reviews or boolean FALSE
 */
function bazaarvoice_reviews_get_reviews($product_id, $locale = FALSE) {
  $product_reviews = drupal_static(__FUNCTION__);
  // Locale not provided? Get default.
  if (!$locale) {
    $locale = bazaarvoice_get_locale(FALSE, TRUE);
  }
  // Reviews have not already been retrieved?
  if (!isset($product_reviews[$product_id][$locale])) {
    // Get a BazaarvoiceReviews object.
    $bazaarvoice_review = new BazaarvoiceReview();
    // Attempt to get reviews.
    if ($reviews = $bazaarvoice_review->getAllReviews($product_id, $locale)) {
      // Allow other modules to alter reviews.
      drupal_alter('bazaarvoice_reviews_reviews', $reviews, $product_id, $locale);
      // Add to static.
      $product_reviews[$product_id][$locale] = $reviews;
    }
  }
  // Allow other modules to perform actions.
  module_invoke_all('bazaarvoice_reviews_get_reviews', $product_id, $locale);
  // Return fetched reviews or boolean FALSE.
  return isset($product_reviews[$product_id][$locale]) ? $product_reviews[$product_id][$locale] : FALSE;
}
