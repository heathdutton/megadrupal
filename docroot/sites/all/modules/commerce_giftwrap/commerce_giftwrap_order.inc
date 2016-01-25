<?php
/**
 * @file
 * Contains all code for the gift wrap local tasks on the order page.
 * Called from commerce_payment_ui_menu().
 */

/**
 * Builds the content for the gift wrap local task on the order page.
 *
 * @param object $order
 *   The order to show gift wrapping information on.
 *
 * @return array
 *   An Drupal render array of content for the gift wrap local task.
 */
function commerce_giftwrap_order_tab($order) {
  $message = commerce_giftwrap_get_message($order);
  $output = array(
    'title' => array(
      '#type' => 'markup',
      '#markup' => '<h3>' . t('Giftwrap Message') . '</h3>',
    ),
    'giftwrap_message' => array(
      '#type' => 'markup',
      '#markup' => '<p>' . filter_xss($message) . '</p>',
    ),
  );
  return $output;
}
