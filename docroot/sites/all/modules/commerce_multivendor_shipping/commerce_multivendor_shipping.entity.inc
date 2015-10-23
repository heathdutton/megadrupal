<?php

/**
 * @file
 * Commerce multivendor shipping entity.
 */

/**
 * Implements hook_entity_info().
 */
function commerce_multivendor_shipping_entity_info() {

  $return = array();

  $return['commerce_multivendor_shipping_vendor'] = array(

    // Define basic information.
    'label' => t('Commerce multivendor shipping'),
    'plural label' => t('Commerce multivendor shipping entities'),
    'description' => t('An entity which stores multivendor shipping information.'),
    'fieldable' => FALSE,
    'controller class' => 'MultivendorShippingController',

    // Provide information about the database table.
    'base table' => 'commerce_multivendor_shipping_vendor',
    // These keys are identical to the the fields in the database schema.
    'entity keys' => array(
      'id' => 'id',
      'label' => 'label',
    ),

    // Access callback.
    'access callback' => 'commerce_multivendor_shipping_entity_access',

    'view modes' => array(
      'full' => array(
        'label' => t('Full'),
        'custom settings' => FALSE,
      ),
    ),
  );

  return $return;
}

/**
 * Access callback.
 */
function commerce_multivendor_shipping_entity_access() {
  return TRUE;
}

/**
 * Implements hook_theme().
 */
function commerce_multivendor_shipping_theme() {
  return array(
    'commerce_multivendor_shipping' => array(
      'variables' => array('vendor' => NULL),
      'template' => 'commerce-multivendor-shipping',
    ),
    'commerce_multivendor_shipping_vendor' => array(
      'template' => 'commerce-multivendor-shipping-vendor',
    ),
  );
}

class MultivendorShippingController extends EntityAPIController {

  /**
   * Overriding the buldContent function.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content['vendor'] = array(
      '#markup' => theme('commerce_multivendor_shipping', array('vendor' => $entity)),
    );
    return $content;
  }

}
