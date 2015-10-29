<?php

/**
 * @file
 */

$flip = array(
  'odd' => 'even',
  'even' => 'odd',
);
$class = 'even';
?>


<div id="ec-cart-view-page">
  <table class="sticky-enabled sticky-table">
    <thead>
      <th class="ec-cart-title">Items</th>
      <th class="ec-cart-extra"><!-- extra --></th>
      <th class="ec-cart-price">Price</th>
      <th class="ec-cart-quantity">Qty</th>
      <th class="ec-cart-total">Total</th>
      <th class="ec-cart-ops"><!-- Operations header --></th>
    </thead>
    <tbody>
      <?php foreach ($items as $nid => $item) { ?>
        <tr id="item-<?php echo $nid ?>" class="cart-item <?php $class = $flip[$class]; echo $class; ?>">
          <td class="ec-cart-title"><?php echo $item['title']; ?></td>
          <td class="ec-cart-extra"><?php echo $item['extra']; ?></td>
          <td class="ec-cart-price"><?php echo $item['price']; ?></td>
          <td class="ec-cart-quantity"><?php echo $item['qty']; ?></td>
          <td class="ec-cart-total"><?php echo $item['total']; ?></td>
          <td class="ec-cart-ops"><?php echo $item['ops']; ?></td>
        </tr>
      <?php } ?>
      <tr id="ec-cart-final" class="<?php echo $class = $flip[$class]; echo $class; ?>">
        <td colspan="3"></td>
        <td class="ec-cart-total"><?php echo $total; ?></td>
        <td/>
      </tr>
    </tbody>
  </table>
  <?php echo $output; ?>
</div>
