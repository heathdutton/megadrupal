<?php
/**
 * @file
 *
 */
?>

<div id="invoice-<?php print $txnid; ?>" class="invoice<?php echo $shippable ? ' shippable' : ' not-shippable'; ?>">
  <div class="header">
    <h1>Invoice from <?php print variable_get('site_name', 'Drupal'); ?></h1>
  </div>
  <div class="invoice-addresses clearfix">
  <div>
    <strong>Mail:</strong> <?php print $mail; ?>
  </div>

  <div class="invoice-details">
    <table>
      <tr>
        <th>Description</th>
        <th>Quantity</th>
        <th>Total</th>
      </tr>
<?php foreach ($items as $item) { ?>
      <tr>
        <td><?php print $item->title; ?></td>
        <td class="item-quantity"><?php print $item->quantity_visible ? $item->qty : ''; ?></td>
        <td class="item-price"><?php print $item->formatted_price; ?></td>
      </tr>
<?php } ?>
<?php if (!empty($misc)) { ?>
      <tr>
        <td colspan="2" class="subtotal-title">Subtotal</td>
        <td class="subtotal-amount"><?php print $subtotal; ?></td>
      </tr>
<?php foreach ($misc as $item) { ?>
      <tr>
        <td colspan="2" class="subtotal-title"><?php echo $item->description; ?></td>
        <td class="subtotal-amount"><?php print $item->price; ?></td>
      </tr>
<?php } ?>
<?php } ?>
      <tr>
        <td colspan="2" class="total-title">Total</td>
        <td class="total-amount"><?php print $gross; ?></td>
      </tr>
    </table>
  </div>
  <?php print $additional ?>
</div>
