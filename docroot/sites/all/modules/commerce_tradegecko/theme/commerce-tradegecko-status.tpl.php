<h3>Product: <?php echo $product->product_name; ?> (<?php echo $product->sku; ?>)</h3>
<table>
  <tr>
    <th>Stock on hand</th>
    <td><?php echo $product->stock_on_hand; ?></td>
  </tr>
  <tr>
    <th>Last cost price</th>
    <td><?php echo $product->last_cost_price; ?></td>
  </tr>
  <tr>
    <th>Moving average cost</th>
    <td><?php echo $product->moving_average_cost; ?></td>
  </tr>
</table>
