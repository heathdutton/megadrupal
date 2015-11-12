<?php
/**
 * @file
 *   Template file for item details panel plugin
 */

// For each ItemAttribute to be reported, list the attribute
// and an array with:
// name, if different from the XML name
// outputElement - Child that can be output as is
// handler, if handling has to be done by a special function, which will be passed the name and object
// These will be done in the order listed here.
// If not found here, they will not be output
$handlers = array(
  'Author' => array('name' => 'Author', 'handler' => 'amazon_store_participant_format'),
  'Composer' => array('name' => 'Composer', 'handler' => 'amazon_store_participant_format'),
  'Artist' => array('name' => 'Artist', 'handler' => 'amazon_store_participant_format'),
  'PublicationDate' => array('name' => "Publication Date"),
  'Publisher' => array('name' => "Publisher"),
  'ProductGroup' => array('name' => "Product Group"),
  'Manufacturer' => array('name' => "Manufacturer", 'handler' => 'amazon_store_manufacturer_format'),
  'Binding' => array('name' => "Binding", 'handler' => 'amazon_store_binding_format'),
  'Brand' => array('name' => "Brand",),
  'Feature' => array('name' => "Features", 'handler' => 'amazon_store_feature_format'),
  'FormFactor' => array('name' => "Form Factor"),
  'HardwarePlatform'=>array('name' => "Hardware Platform"),
  'ItemDimensions' => array('name' => "Item Dimensions", 'handler' => 'amazon_store_dimensions_format'),
  'PackageDimensions' => array('name' => "Package Dimensions", 'handler' => 'amazon_store_dimensions_format'),
  'ListPrice' => array('name' => "List Price", 'outputElement' => 'FormattedPrice'),
  'Model' => array('name' => "Model Number"),
  'UPC' => array('name' => "UPC",),
  'ISBN' => array('name' => "ISBN"),
  'Warranty' => array('name' => "Warranty",),
);

?>
<div class="product_details">
<ul>
<?php
foreach ($handlers as $itemName=>$handler) {
  $output = "";
  if ($item->ItemAttributes->$itemName) {
    $output = "<li>";
    $output .= amazon_store_format_attribute($itemName, $item->ItemAttributes->{$itemName},
      $handler, $item->ItemAttributes);
    $output .= "</li>";
  }
  print $output;
}
print "<li>ASIN: $item->ASIN</li>";

?>
</ul>
</div>
