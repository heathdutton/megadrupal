<?php
/*
 * @file commerce_post_affiliate_pro.rules_default.inc
 * Default rules
 * @copyright Copyright(c) 2011 Rowlands Group
 * @license GPL v3 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlandsgroup dot com
 * 
 */

/**
 * Implements hook_default_rules_configuration().
*/
function commerce_post_affiliate_pro_default_rules_configuration() {
  $items = array();
  $items['commerce_post_affiliate_pro_checkout'] = entity_import('rules_config', '{ "commerce_post_affiliate_pro_checkout" : {
      "LABEL" : "Register Completed Cart Sale With Post Affiliate Pro",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "commerce_post_affiliate_pro", "commerce_checkout" ],
      "ON" : [ "commerce_checkout_complete" ],
      "DO" : [
        { "commerce_post_affiliate_pro_sales_notify" : { "commerce_order" : [ "commerce_order" ] } }
      ]
    }
  }');
  $items['commerce_post_affiliate_pro_manual'] = entity_import('rules_config', '{ "commerce_post_affiliate_pro_manual" : {
      "LABEL" : "Register Non-Cart Sale With Post Affiliate Pro",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "commerce_cart", "commerce_post_affiliate_pro", "entity" ],
      "ON" : [ "commerce_order_insert" ],
      "IF" : [
        { "NOT commerce_order_is_cart" : { "commerce_order" : [ "commerce_order" ] } }
      ],
      "DO" : [
        { "commerce_post_affiliate_pro_sales_notify" : { "commerce_order" : [ "commerce_order" ] } }
      ]
    }
  }');
  return $items;
}
