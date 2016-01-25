<?php
/**
 * @file
 * Statistics functionality for Bazaarvoice Products.
 */

/**
 * Render the statistics for a product.
 *
 * @param string $type
 *   The entity type.
 * @param object $entity
 *   The entity.
 *
 * @return string
 *   Rendered Product statistics.
 */
function bazaarvoice_products_view_product_statistics($type, $entity) {
  $content = '';
  // Attempt to get product info.
  if ($product_info = bazaarvoice_products_get_product_info($type, $entity)) {
    // Attempt to retrieve product statistics.
    if ($statistics = bazaarvoice_products_get_product_statistics($product_info['product_id'])) {
      // Template the statistics.
      $content = array(
        '#theme' => 'bazaarvoice_product_statistics',
        '#review_count' => $statistics['TotalReviewCount'],
        '#rating' => t('@rating out of @range', array(
          '@rating' => round($statistics['AverageOverallRating'], 2),
          '@range' => $statistics['OverallRatingRange'],
        )),
        '#average_rating' => $statistics['AverageOverallRating'],
        '#rating_range' => $statistics['OverallRatingRange'],
        '#statistics' => $statistics,
        '#product_info' => $product_info,
      );
      $content = render($content);
    }
  }

  return $content;
}

/**
 * Retrieve the statistics for a product from Bazaarvoice.
 *
 * @param string $product_id
 *   The Bazaarvoice product id.
 *
 * @return bool|array
 *   Array of product statistics from Bazaarvoice or boolean FALSE.
 */
function bazaarvoice_products_get_product_statistics($product_id) {
  $product_statistics = drupal_static(__FUNCTION__);
  // Already retrieved statistics?
  if (!isset($product_statistics[$product_id])) {
    // Instantiate product object.
    $bazaarvoice_product = new BazaarvoiceProduct();
    // Attempt to fetch statistics.
    if ($statistics = $bazaarvoice_product->getProductStatistics($product_id)) {
      // Allow other modules to modify.
      drupal_alter('bazaarvoice_products_product_statistics', $statistics, $product_id);
      // Store stats.
      $product_statistics[$product_id] = $statistics;
    }
  }

  return isset($product_statistics[$product_id]) ? $product_statistics[$product_id] : FALSE;
}
