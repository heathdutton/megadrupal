<?php

/**
 * @file
 * Allow users to edit a product display from a barcode scan.
 */

return '{ "rules_commerce_barcode_scanner_edit_product_display" : {
    "LABEL" : "Edit Product Display",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "REQUIRES" : [ "rules", "commerce_barcode_scanner" ],
    "ON" : [ "commerce_barcode_scanner_scanned" ],
    "DO" : [
      { "redirect" : {
          "url" : "node\/[commerce-product-display:nid]\/edit",
          "destination" : 1
        }
      }
    ]
  }
}';
