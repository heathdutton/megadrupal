<?php
/**
 * Set recipients of an invoice, and mail it.
 *
 * @ingroup forms
 * @see
 *   commerce_invoice_receipt_mail_form_validate()
 *   commerce_invoice_receipt_mail_form_submit()
 */
function commerce_invoice_receipt_mail_form($form_state, $order) {
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order['build_info']['args'][0]->order_id,
  );
  
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient e-mail address'),
    '#default_value' => $order['build_info']['args'][0]->mail,
  );
  
  $form['submit' ] = array(
    '#type' => 'submit',
    '#value' => t('Email invoice'),
  );

  return $form;
}

/**
 * Only mail invoices to valid email addresses.
 *
 * @see commerce_invoice_receipt_mail_form()
 */
function commerce_invoice_receipt_mail_form_validate($form, &$form_state) {
  $recipient = check_plain($form_state['values']['email']);
  if (empty($recipient) || !valid_email_address($recipient)) {
    form_set_error('email', t('Invalid e-mail address.'));
  }
}

/**
 * @see commerce_invoice_receipt_mail_form()
 */
function commerce_invoice_receipt_mail_form_submit($form, &$form_state) {
  $order = commerce_order_load($form_state['values']['order_id']);
  
  if ($order === FALSE) {
    drupal_set_message(t('Order @order_id does not exist.', array('@order_id' => $form_state['values']['order_id'])));
    drupal_goto('admin/commerce/orders');
  }
  
  $recipient = check_plain($form_state['values']['email']);
  
  $params = array('order' => $order);
  $from_email = variable_get('site_mail', '');
  
  $account = user_load($order->uid);
  $language = user_preferred_language($account);
  
  $sent = drupal_mail('commerce_invoice_receipt', 'invoice', $recipient, $language, $params, $from_email);
  
  if (!$sent) {
    drupal_set_message(t('E-mail failed.'));
  }
  else {
    $message = t('Invoice e-mailed to @email.', array('@email' => $recipient));
    drupal_set_message($message);
  }
}

/**
* Modify the drupal mail system to send HTML emails.
*/
class InvoiceMailSystem implements MailSystemInterface {
  /**
   * Concatenate and wrap the e-mail body for plain-text mails.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
   *   The formatted $message.
   */
  public function format(array $message) {
    $message['body'] = implode("\n\n", $message['body']);
    return $message;
  }
  
  /**
   * Send an e-mail message, using Drupal variables and default settings.
   *
   * @see <a href="http://php.net/manual/en/function.mail.php" title="http://php.net/manual/en/function.mail.php" rel="nofollow">http://php.net/manual/en/function.mail.php</a>
   * @see drupal_mail()
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   * @return
   *   TRUE if the mail was successfully accepted, otherwise FALSE.
   */
  public function mail(array $message) {
    $mimeheaders = array();
    foreach ($message['headers'] as $name => $value) {
      $mimeheaders[] = $name . ': ' . mime_header_encode($value);
    }
    $line_endings = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
    return mail(
      $message['to'],
      mime_header_encode($message['subject']),
      // Note: e-mail uses CRLF for line-endings. PHP's API requires LF
      // on Unix and CRLF on Windows. Drupal automatically guesses the
      // line-ending format appropriate for your system. If you need to
      // override this, adjust $conf['mail_line_endings'] in settings.php.
      preg_replace('@\r?\n@', $line_endings, $message['body']),
      // For headers, PHP's API suggests that we use CRLF normally,
      // but some MTAs incorrectly replace LF with CRLF. See #234403.
      join("\n", $mimeheaders)
    );
  }
}