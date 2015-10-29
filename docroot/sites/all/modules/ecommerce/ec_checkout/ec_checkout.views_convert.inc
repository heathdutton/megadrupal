<?php

/**
 * @file
 * Field conversion for fields handled by this module.
 */

/**
 * Implements hook_views_convert().
 */
function ec_checkout_views_convert($display, $type, &$view, $field, $id = NULL) {
  switch ($type) {
    case 'field':
      switch ($field['tablename']) {
        case 'ec_product':
          switch ($field['field']) {
            case 'addtocartlink':
              $view->set_item_option($display, 'field', $id, 'field', 'checkout_links');
              break;
          }
          break;
      }
      break;
  }
}
