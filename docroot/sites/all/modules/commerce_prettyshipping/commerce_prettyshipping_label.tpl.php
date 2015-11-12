<?php
$service = $shipping_service->data['shipping_service'];
$class = drupal_html_class($service['base']);
$cost = $shipping_service->commerce_unit_price[LANGUAGE_NONE][0];
?>
<div class="shipping-service-<?php echo $class; ?>">
  <h3><?php echo $service['display_title']; ?></h3>
  <?php
  $price = commerce_currency_format($cost['amount'], $cost['currency_code']);
  if ($cost['amount'] == 0) {
    $price = t('FREE');
  }
  ?>
  <p class="price"><?php echo $price; ?></p>

  <p class="description"><?php echo $service['description']; ?></p>
</div>
