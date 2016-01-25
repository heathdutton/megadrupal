<?php

/**
 * @file
 * Allow users to edit products from a barcode scan.
 */

return '{ "rules_commerce_barcode_scanner_edit_product" : {
    "LABEL" : "Edit Product",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "REQUIRES" : [ "rules", "commerce_barcode_scanner" ],
    "ON" : [ "commerce_barcode_scanner_scanned" ],
    "DO" : [
      { "redirect" : {
          "url" : "admin\/commerce\/products\/[commerce-product:product-id]\/edit",
          "destination" : 1
        }
      }
    ]
  }
}';
