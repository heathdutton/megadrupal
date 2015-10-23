<?php

/**
 * @file
 * Template for the packing slip.
 */
$total = 0;
?>
<html>
  <head>
    <?php print $page['css']?>
  </head>
  <body>
    <div id="packing-header">
      <div class="header-company">
        <img src="<?php print $image; ?>" />
        <p class="company-info">
        <?php
        print '<strong>' . t('@company', array('@company' => $company)) . '</strong><br>'
          . t('@address', array('@address' => $address)) . '<br>' . t('Phone: @phone', array('@phone' => $phone));
        ?>
        </p>
      </div>
      <div class="slip-head">
        <h1>Packing Slip</h1>
        <p>
          Date: <?php print date('y/m/d');?><br>
          Package Type: <?php print $package_type ?>
        </p>
      </div>
    </div>
  <div class="customer-info">
    <p>
      <span class="customer-label">
          <Strong>Ship To:</strong>
      </span>
      <br>
      <?php
        if($shipping !== NULL) :
          print t('@shipping1 @shipping2', array('@shipping1' => $shipping[0], '@shipping2' => $shipping[1])) . '<br>';
          print t('@ship_add, @ship_city @ship_state', array(
              '@ship_add' => $shipping[2],
              '@ship_city' => $shipping[3],
              '@ship_state' => $shipping[4],
              )) . '<br>';
          print t('@ship_zip, @ship_country', array('@ship_zip' => $shipping[5], '@ship_country' => $shipping[6]));
        endif;
      ?>
    </p>
  </div>
  <div id="product-table">
    <table class="main-table">
      <tr class="table-header">
        <th>Item #:</th>
        <th>Item Name:</th>
        <th>Quantity:</th>
        <th>Price:</th>
        <th>Item Total:</th>
      </tr>
      <?php
      $row_count = 0;
      if($products !== NULL) :
        foreach($products as $line_item) :?>
          <tr class="populated-row">
            <?php
            $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
              ?>
              <td class="product-name-cell">
                <?php print t('@label', array('@label' => $line_item_wrapper->line_item_label->value())); ?>
              </td>
              <td class="product-id-cell">
                <?php
                  $com_product = commerce_product_load($line_item_wrapper->commerce_product->product_id->value());
                  $com_product_wrapper = entity_metadata_wrapper('commerce_product', $com_product);
                  $title = $com_product_wrapper->title->value();
                  print t('@title', array('@title' => $title));
                ?>
              </td>
              <td class="quantity-cell">
                <?php print t('@quantity', array('@quantity' => $line_item_wrapper->quantity->value())); ?>
              </td>
              <td class="unit-price-cell">
                <?php
                $price = $line_item_wrapper->commerce_unit_price->amount->value();
                $length = strlen($price);
                $price = substr_replace($price, '.', $length - 2, 0);
                print t('$@price', array('@price' => $price));
                ?>
              </td>
              <td class="total-price-cell">
                <?php
                $price = $line_item_wrapper->commerce_total->amount->value();
                $length = strlen($price);
                $price = substr_replace($price, '.', $length - 2, 0);
                print t('$@price', array('@price' => $price));
                $total += $line_item_wrapper->commerce_total->amount->value();
                $row_count++;
                ?>
              </td>
            </tr>
          <?php
          endforeach;
      endif;
      $row_count = 20 - $row_count;
      for($i = $row_count; $i > 0; $i--):
        ?>
        <tr class="empty-row">
          <td></td>
          <td></td>
          <td></td>
          <td></td>
          <td></td>
        </tr>
      <?php endfor; ?>
    </table>
    <div class="package-info">
        <span class="package-count">
          <?php print t('Package: 1 of @count', array('@count' => $count)); ?>
        </span>
        <span class="package-total">
          <?php
          $length = strlen($total);
          $total = substr_replace($total, '.', $length - 2, 0);
          print t('Total: $@total', array('@total' => $total));
          ?>
        </span>
      </div>
    </div>
  </body>
</html>
