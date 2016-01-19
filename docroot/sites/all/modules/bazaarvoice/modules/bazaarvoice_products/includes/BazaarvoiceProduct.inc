<?php
/**
 * @file
 * Contains wrapper code for a BazaarvoiceProduct object.
 */

/**
 * Class BazaarvoiceProduct.
 */
class BazaarvoiceProduct extends Bazaarvoice {
  /**
   * Retrieve products from Bazaarvoice.
   *
   * @param array $parameters
   *   Array of parameters to send to request.
   *
   * @return mixed
   *   Data returned from API Call.
   */
  public function getProducts($parameters = array()) {
    return $this->apiRequest('products', $parameters);
  }

  /**
   * Retrieve product statistics from Bazaarvoice.
   *
   * @param string $product_id
   *   The product id.
   *
   * @return bool|array
   *   Boolean FALSE or array of statistics data.
   */
  public function getProductStatistics($product_id) {
    $stats = FALSE;

    $parameters = array(
      'filter' => array(
        'productid' => $product_id,
      ),
      'stats' => 'Reviews',
    );

    if ($data = $this->apiRequest('statistics', $parameters)) {
      $stats = $data['Results'][0]['ProductStatistics']['ReviewStatistics'];
    }

    return $stats;
  }

}
