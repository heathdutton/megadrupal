<?php
/**
 * @file
 * Default XML Schema template for Order FTP Export.
 * 
 * Available variables:
 * - order: the commerce order object.
 * - line_items: array of commerce line item objects indexed by line_item_id.
 * - products: array of commerce products objects indexed by product_id.
 * - profiles: array of commerce profile objects indexed by profile_id.
 * - user: user object (who placed the order).
 */
?>
<?php echo '<?xml version="1.0" encoding="utf-8" ?>' . PHP_EOL; ?>
<PurchaseOrder>
  <OrderID><?php print $order->order_id; ?></OrderID>
  <OrderNumber><?php print $order->order_number; ?></OrderNumber>
  <OrderDate><?php print date("m/d/Y", $order->revision_timestamp); ?></OrderDate>
  <OrderTime><?php print date("h:i:s", $order->revision_timestamp); ?></OrderTime>
  <?php $profile_id = $order->commerce_customer_billing['und'][0]['profile_id'];
        $profile_address = $profiles[$profile_id]->commerce_customer_address['und'][0]; ?>
  <BillToAddress>
    <AddressID><?php print $profile_id; ?></AddressID>
    <Name><?php print $profile_address['name_line']; ?></Name>
    <Line1><?php print $profile_address['thoroughfare']; ?></Line1>
    <Line2><?php print $profile_address['premise']; ?></Line2>
    <City><?php print $profile_address['locality']; ?></City>
    <StateProvinceCode><?php print $profile_address['administrative_area']; ?></StateProvinceCode>
    <PostalCode><?php print $profile_address['postal_code']; ?></PostalCode>
    <CountryCode><?php print $profile_address['country']; ?></CountryCode>
    <Email><?php print $user->mail; ?></Email>
  </BillToAddress>
  <?php foreach ($order->commerce_line_items['und'] as $key=>$line_item) { ?>
  <OrderItemDetail>
    <LineNumber><?php print $key+1; ?></LineNumber>
    <?php foreach ($line_items[$line_item['line_item_id']]->commerce_product['und'] as $product) { ?>
    <ProductSKU><?php print $products[$product['product_id']]->sku; ?></ProductSKU>
    <?php } ?>
    <Quantity><?php print $line_items[$line_item['line_item_id']]->quantity; ?></Quantity>
  </OrderItemDetail>
  <?php } ?>
</PurchaseOrder>