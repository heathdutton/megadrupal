<?php
/**
 * @file
 * Basic cart shopping cart html template
 */
?>

<?php if (empty($cart)): ?>
  <p><?php print t('Your shopping cart is empty.'); ?></p>
<?php else: ?>
  <div class="basic-cart-cart basic-cart-grid">
    <?php if(is_array($cart) && count($cart) >= 1): ?>
      <?php foreach($cart as $nid => $node): ?>
        <div class="basic-cart-cart-contents row">
            
          <div class="cell basic-cart-cart-unit-price">
            <?php if (!isset($node->basic_cart_unit_price)) $node->basic_cart_unit_price = 0; ?>
            <strong><?php print basic_cart_price_format($node->basic_cart_unit_price); ?></strong>
          </div>
          <div class="cell basic-cart-cart-x">x</div>
          <div class="basic-cart-cart-quantity cell"><?php print $node->basic_cart_quantity; ?></div>
          
          <div class="basic-cart-cart-node-title cell">
            <?php print l($node->title, 'node/' . $node->nid); ?>
          </div>
        </div>
      <?php endforeach; ?>
   </div> 
   <div class="basic-cart-cart basic-cart-grid">
      <div class="basic-cart-cart-total-price-contents row">
        <div class="basic-cart-total-price cell">
          <?php print t('Total price'); ?>: <strong> <?php print $price; ?></strong>
        </div>
      </div>
    
      <?php if (!empty ($vat)): ?>
        <div class="basic-cart-cart-total-vat-contents row">
          <div class="basic-cart-total-vat cell"><?php print t('Total VAT'); ?>: <strong><?php print $vat; ?></strong></div>
        </div>
      <?php endif; ?>
    <?php endif; ?>
  </div>
<?php endif; ?>
