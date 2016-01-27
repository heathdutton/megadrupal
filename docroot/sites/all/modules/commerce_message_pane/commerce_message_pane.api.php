<?php

/**
 * @file
 * API provided by the Message Pane module.
 */

/**
 * Implements hook_commerce_checkout_pane_info().
 *
 * Any number of message panes may be defined by implementing
 * hook_commerce_checkout_pane_info().  This hook is defined by Commerce
 * Checkout and provides the API to add more panes.
 *
 * Specify the following to create a Message Pane:
 * - 'base' => 'commerce_message_pane'
 *
 * The Message checkout pane callback functions are defined in
 * commerce_message_pane.module so that they are always available to other
 * modules. This is to overcome the hook limitation of 'file' which must be
 * relative to the module defining the pane.
 */
function hook_commerce_checkout_pane_info() {
  $checkout_panes = array();

  $checkout_panes['commerce_message_pane_example'] = array(
    'title' => t('Message Example'),
    'base' => 'commerce_message_pane',
    'enabled' => FALSE,
  );

  return $checkout_panes;
}
