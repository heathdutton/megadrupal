<?php

/**
 * @file
 * Allow users to add products to their cart from a barcode scan.
 */

return '{ "rules_commerce_barcode_scanner_add_product_to_cart" : {
    "LABEL" : "Add Product to Cart",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "REQUIRES" : [ "rules", "commerce_cart", "commerce_barcode_scanner" ],
    "ON" : [ "commerce_barcode_scanner_scanned" ],
    "DO" : [
      { "commerce_cart_product_add_by_sku" : {
          "USING" : {
            "user" : [ "site:current-user" ],
            "sku" : "[commerce-product:sku]",
            "quantity" : "1",
            "combine" : 1
          },
          "PROVIDE" : { "product_add_line_item" : { "product_add_line_item" : "Added product line item" } }
        }
      }
    ]
  }
}';
