<?php
/**
 * Gets the text that should appear at the beginning of messages.
 *
 * @param stdClass $recipient
 *   The user to whom the message is addressed.
 *
 * @return array
 *   The lines of the salutation.
 */
function commerce_customer_contact_get_salutation(stdClass $recipient) {
  $header_lines = array(
    t("Hello @user_name,", array('@user_name' => $recipient->name)),
  );

  return $header_lines;
}

/**
 * Gets the text that should appear at the end of messages.
 *
 * @return array
 *   The lines of the message closing.
 */
function commerce_customer_contact_get_closing() {
  $site_name = variable_get('site_name', 'Drupal');

  $footer_lines = array(
    t("Sincerely,"),
    t("The @site_name team", array('@site_name' => $site_name)),
  );

  return $footer_lines;
}

/**
 * Form submit callback for the personal contact form, when embedded in orders.
 *
 * @param array $form
 *   The contact form array.
 * @param array $form_state
 *   A reference to the contact form state array.
 */
function commerce_customer_contact_form_submit_redirect(array $form, array &$form_state) {
  $form_state['redirect'] = entity_uri('commerce_order', $form_state['order']);
}

/**
 * Implements <code>hook_mail_alter()</code>.
 *
 * Customizes personal contact form messages sent to customers through the
 * customer contact form, eliminating several stock elements Drupal normally
 * includes in such messages.
 */
function commerce_customer_contact_mail_alter(&$message) {
  if (in_array($message['id'], array('contact_user_mail', 'contact_user_copy'))) {
    $order = menu_get_object('commerce_order', 3);

    if (!empty($order)) {
      $recipient  = $message['params']['recipient'];
      $salutation = commerce_customer_contact_get_salutation($recipient);
      $closing    = commerce_customer_contact_get_closing();

      $message['subject']
        = t('[@site_name] Message about your order (#@order_number) - @subject',
        array(
          '@site_name'    => variable_get('site_name', 'Drupal'),
          '@order_number' => $order->order_number,
          '@subject'      => $message['params']['subject'],
        ));

      // Removes:
      //   - "Hello X,"
      //   - "X has sent you a message via your contact form..."
      //   - "If you don't want to receive such..."
      //   - "Message:"
      array_splice($message['body'], 0, 4, $salutation);

      $message['body'] = array_merge($message['body'], $closing);
    }
  }
}