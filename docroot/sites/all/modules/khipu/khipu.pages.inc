<?php

/**
 * @file
 * Page callback for listen notification.
 */

/**
 * Listen Notification.
 *
 * @param string $instance_id
 *   The instance ID that uses the payment.
 * @param object $khipu_account
 *   The Khipu Account object.
 */
function khipu_listen_notification($instance_id, $khipu_account) {
  if (empty($_POST)) {
    // If not done the POST request, then page not found.
    drupal_not_found();
    drupal_exit();
  }

  if (!khipu_load_class()) {
    drupal_goto('<front>');
  }
  $khipu = new Khipu();
  $khipu_service = $khipu->loadService('VerifyPaymentNotification');
  // Attached POST values ​​in service.
  $khipu_service->setDataFromPost();
  // We make a request to khipu to verify.
  $response = $khipu_service->verify();

  if ($response['response'] == 'VERIFIED' && $khipu_account->receiver_id == $_POST['receiver_id']) {
    module_invoke_all('khipu_verified', $instance_id);
  }
}
