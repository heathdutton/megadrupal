<?php
/**
 * @file
 *
 * Contains the controller for uc_cart_item entities.
 */
//class AgresBookingsCartItemController extends UcOrderProductController {   
//  public function attachLoad(&$items, $revision_id = FALSE) {
//     
//    foreach ($items as &$item) {
//      $product = uc_product_load_variant($item->nid, $item->data);
//      // Merge in fields from the product.
//      foreach ($product as $key => $value) {
//        $item->$key = $value;
//      }
//      $item->module = $item->data['module'];
//      
//    }
////    drupal_set_message('<pre>'.print_r($items,true).'</pre>');
//    parent::attachLoad($items, $revision_id);
//  }
//
//}
//class UcCartItemController extends UcOrderProductController {
////
////  public function attachLoad(&$items, $revision_id = FALSE) {
////    foreach ($items as &$item) {
////      $product = uc_product_load_variant($item->nid, $item->data);
////      // Merge in fields from the product.
////      foreach ($product as $key => $value) {
////        $item->$key = $value;
////      }
////      $item->module = $item->data['module'];
////    }
////    parent::attachLoad($items, $revision_id);
////  }
//}