<?php

/**
 * @file
 */

/*
 * Called at the start of the checkout process.
 *
 * This allows modules hooking into the checkout process to set of their part of the
 * transaction.
 *
 * @param $txn
 */
function hook_checkout_init(&$txn) {

}

/*
 * adds form elements to the checkout form
 *
 *
 */
function hook_checkout_form(&$form, &$form_state) {

}

/*
 * runs validation
 *
 *
 */
function hook_checkout_validate(&$form, &$form_state) {

}


/*
 *
 *
 *
 */
function hook_checkout_calculate(&$form_state) {

}

/*
 *
 *
 *
 */
function hook_checkout_update(&$form, &$form_state) {

}

/*
 *
 *
 *
 */
function hook_checkout_post_update(&$form, &$form_state) {

}


/*
 * this is the first set of calls made from ec_checkout_form_process_order
 *
 *
 */
function hook_checkout_submit($form, $form_state) {

}

/*
 *
 *
 *
 */
function hook_checkout_post_submit($txn, &$form_state) {

}

