<?php

/**
 * @file
 * Allow users to be redirected to a product display from a barcode scan.
 */

return '{ "rules_commerce_barcode_scanner_redirect_to_product_display" : {
    "LABEL" : "Redirect to Product Display",
    "PLUGIN" : "reaction rule",
    "WEIGHT" : "5",
    "REQUIRES" : [ "rules", "commerce_barcode_scanner" ],
    "ON" : [ "commerce_barcode_scanner_scanned" ],
    "DO" : [ { "redirect" : { "url" : "[commerce-product-display:url]" } } ]
  }
}';
