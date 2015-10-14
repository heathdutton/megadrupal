<div class="multivendor-shipping-vendor">

  <?php if ($vendor->base_amount > 0) : ?>
  <div class="multivendor-shipping-vendor-base">
    <?php echo t('Base shipping rate is @amount', array(
      '@amount' => commerce_currency_format($vendor->base_amount, commerce_default_currency())
      ), array('context' => 'Multivendor shipping')) ?>
  </div>
  <?php endif; ?>

  <?php if ($vendor->package_amount > 0) : ?>
  <div class="multivendor-shipping-vendor-package">
    <?php echo t('Each package with @size items costs @amount',
      array(
      	'@size' => $vendor->package_size,
        '@amount' => commerce_currency_format($vendor->package_amount, commerce_default_currency()),
      ), array('context' => 'Multivendor shipping')); ?>
  </div>
  <?php endif; ?>

  <?php if (!empty($vendor->description)) : ?>
  <div class="multivendor-shipping-vendor-description">
    <?php echo t($vendor->description, array(), array('context' => 'Multivendor shipping')); ?>
  </div>
  <?php endif; ?>

</div>