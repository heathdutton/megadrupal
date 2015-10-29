<?php

/**
 * @file
 * API Documentation of hooks related to product features
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provides information to feature system about features offered by this module
 *
 * @return
 * array of info about each feature that the module implements
 *
 */
function hook_feature_info() {
  return array(
    'featurename1' => array( // machine name of feature
      'name' => t('Feature 1 name'), // pretty name of feature
      'description' => t('Feature 1 description'), // pretty description
      'module' => 'hook_featurename1', // name of module providing the feature
      'allow_edit' => TRUE, // allow edits on product node
      'allow_disable' => TRUE, // allow deletion from the product type feature list once this is added
      'hidden' => TRUE, // if TRUE, feature is not available via UI
      'file' => 'filename.inc', // (optional) the filename containing the functions for this feature's implmentation
    ),
    'featurename2' => array( // (required) machine name of feature
      'name' => t('Feature 2 name'), // (required) pretty name of feature
      'description' => t('Feature 2 description'), // pretty description
      'module' => 'hook_featurename2', // (required) name of module providing the feature
      'allow_edit' => TRUE, // allow edits on product node
      'allow_disable' => TRUE, // allow deletion from the product type feature list once this is added
      'hidden' => TRUE, // if TRUE, feature is not available via UI
      'file' => 'filename.inc', // (optional) the filename containing the functions for this feature's implmentation
    ),
  );
}


/**
 * Provide added product attributes via a feature
 *
 * Return additional attributes for a product based upon features that have
 * been added to a product.
 *
 * For more information see hook_attributes().
 *
 * @param $node
 *   The node of the product that is to be checked.
 *
 * @return
 *   Return an array of attributes which are to be enabled or disabled.
 */
function hook_feature_featurename_attributes($node) {
  return array('no_discount' => TRUE);
}

/**
 * Check that a product can be purchased by the current user.
 *
 * For every product a test is done to make sure the current user or some
 * other setting such as product availability to able to purchase the product
 * in question.
 *
 * See hook_ec_checkout_validate_item() for more information.
 *
 * @param $node
 *   The product in question to check the if it can be purchased.
 * @param $type
 *   Specifies the context of how this check is to be preformed. The product
 *   may be able to be purchased by buy now, but not the cart.
 * @param $qty
 *   Specifies the quantity to be purchased.
 * @param $data
 *   Provides the additional data which is used by the system for the
 *   pruchase.
 * @param $return
 *   provides the result of the previous check if it is implemented by
 *   multiple features
 *
 * @return
 *   Return TRUE or FALSE depending on if your check can be reversed.
 */
function hook_feature_featurename_ec_checkout_validate_item(&$node, $type, &$qty, &$data, $severity, $return) {
  return user_access('purchase products');
}


/**
 * this is 
 *
 */
/**
 * called when all the update functions have run when checkout-update-order button is pressed
 *
 * @param <type> $node - the product node
 * @param <type> $cart_id - the cart id
 * @param <type> $data - data from the product node
 * @param <type> $a6
 * @param <type> $a7
 */
function hook_feature_featurename_ec_checkout_add_complete($node, $cart_id, $data, $a6=NULL, $a7=NULL) {
  
}

/**
 * Load additional information into the transaction item.
 *
 * When loading transaction this hook will allow the feature to load any
 * additional information that will be required.
 *
 * @param $item
 *  The product item that is attached to the transaction.
 *
 * @return
 *  An array of new fields that need to be added to the item.
 */
function hook_feature_featurename_transaction_load($item) {
  return array('qty_x_2' => $item->qty*2);
}

/**
 * Insert additional data into the system for later loading.
 */
function hook_feature_featurename_transaction_insert($item) {
  insert_item();
}

/**
 * Update additional data into the system for later loading.
 */
function hook_feature_featurename_transaction_update($item) {
  update_item();
}
/**
 * Delete additional data into the system for later loading.
 */
function hook_feature_featurename_transaction_delete($item) {
  delete_item();
}

/*
 * Provides the ec_feature API with form details to present to
 * the user to allow entry of settings this feature needs.
 * This is only needed if the feature has user-adjustable settings.
 */
function hook_feature_featurename_featurename_admin_form(&$form_state, $settings) {
  $form = array();

  $form['featurename_variablename1'] = array(
    '#type' => 'textfield',
    '#title' => t('Something Descriptive'),
    '#default_value' => $settings->featurename_variablename1,
  );

  $form['featurename_variablename2'] = array(
    '#type' => 'textfield',
    '#title' => t('Something Else Descriptive'),
    '#default_value' => $settings->featurename_variablename2,
  );

  return $form;
}

/*
 * Provides the ec_feature API with variable names used to
 * save the feature settings into ec_product_features table.
 * This is only needed if you create an hook_feature_featurename_admin_form
 * to administer those settings.
 *
 * @return
 *  An array of variable names
 *
 */
function hook_feature_featurename_settings() {
  return array(
    'featurename_variablename1',
    'featurename_variablename2',
  );
}

/*
 * called by ec_product_checkout_calculate
 * modifies the product price based on feature capability
 *
 */
function hook_feature_featurename_product_calculate(&$txn, $item) {

}


function hook_feature_featurename_cart_remove_item() {

}

function hook_feature_featurename_cart_add_item() {

}

function hook_feature_featurename_cart_update_item() {

}

/**
 * @} End of "addtogroup hooks".
 */
