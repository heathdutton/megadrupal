<?php

/**
 * @file
 *   Exposed Hooks in 7.x:
 */

/**
 * Allow other modules to make changes to the product node right before saving.
 *
 * @param $node
 *   A Drupal node object of the imported product that is about to be saved.
 * @param $product
 *   The complete product payload response from Magento.
 */
function hook_magento_node_alter(&$node, $product) {

}
