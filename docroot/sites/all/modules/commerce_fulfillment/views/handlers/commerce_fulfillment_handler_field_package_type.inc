<?php

/**
 * Field handler to display package type term label instead of id.
 */
class commerce_fulfillment_handler_field_package_type extends views_handler_field {

  function render($values) {

    if (!empty($values->commerce_fulfillment_packages_box_type)) {
      $term = taxonomy_term_load($values->commerce_fulfillment_packages_box_type);
      if ($term) {
        return t('@box_type', array('@box_type' => $term->name));
      }
    }

    return "";
  }

}
