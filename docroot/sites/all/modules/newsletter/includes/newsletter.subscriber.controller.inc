<?php

/**
 * @file
 * Controller class definition file for newsletter_subscriber entity.
 */

/**
 * Newsletter Subscriber Controller
 */
class NewsletterSubscriberController extends DrupalDefaultEntityController {
  public function __construct($entityType) {
    parent::__construct('newsletter_subscriber');
  }

  // The Drupal core default controller does not provide a create() method, so
  // we add one. This is a useful place to assign defaults to the properties.
  public function create() {
    $subscriber = new stdClass();

    $subscriber->email = '';
    $subscriber->uid = NULL;
    $subscriber->firstname = NULL;
    $subscriber->lastname = NULL;
    $subscriber->gender = NULL;
    $subscriber->receive_format = NULL;
    $subscriber->confirmed = 0;

    return $subscriber;
  }

  /*
   * Saves a new newsletter subscriber in database.
   *
   * @param $subscriber
   *   An object conatining the following:
   *    email - Required.
   *    uid - Optional.
   *    firstname - Optional.
   *    lastname - Optional.
   *    gender - Optional.
   *    ip - Optional.
   *    confirmed - Required.
   * @param $needs_confirm
   *   Whether a email asking for subscription confirmation should be sent.
   * @return
   *   The full, saved subscriber object.
   */
  public function save($subscriber, $needs_confirm = FALSE) {

    if (isset($subscriber->uid) && is_numeric($subscriber->uid)) {
      // Make sure that user's email is same with subscriber's,
      // so we can safely assume that this subscriber is same with user.
      $user = user_load($subscriber->uid);

      $subscriber->uid = (isset($user->mail) && $subscriber->email == $user->mail) ? $user->uid : NULL;
    }

    field_attach_presave('newsletter_subscriber', $subscriber);
    // Make sure to invoke the presave hook.
    module_invoke_all('entity_presave', $subscriber, 'newsletter_subscriber');

    if (!isset($subscriber->nsid)) {
      if (!valid_email_address($subscriber->email))  {
        return drupal_set_message(t('%mail is not a valid e-mail.Please provide a valid e-mail', array('%mail' => $subscriber->email)), 'warning');
      }
      elseif (newsletter_is_subscribed($subscriber->email)) {
        return drupal_set_message(t('%mail is already subscribed', array('%mail' => $subscriber->email)), 'warning');
      }

      $subscriber->created = REQUEST_TIME;
      $subscriber->hash = drupal_hmac_base64(REQUEST_TIME . $subscriber->email, drupal_get_hash_salt() . ip_address());
      $subscriber->confirmation_timestamp = $needs_confirm ? 0 : REQUEST_TIME;
      drupal_write_record('newsletter_subscriber', $subscriber);
      field_attach_insert('newsletter_subscriber', $subscriber);

      // Make sure to invoke the insert hook.
      module_invoke_all('entity_insert', $subscriber, 'newsletter_subscriber');
    }
    else {
      drupal_write_record('newsletter_subscriber', $subscriber, 'nsid');
      field_attach_update('newsletter_subscriber', $subscriber);

      // Make sure to invoke the update hook.
      module_invoke_all('entity_update', $subscriber, 'newsletter_subscriber');
    }
    if ($needs_confirm && arg(0) != 'admin') {
      $sent = newsletter_create()->sendBasic(1, $subscriber->email);
      $sent
        ? drupal_set_message(t("Thank you for your subscription. An e-mail has been sent to your address. Please click the provided link to prove you are the owner of this e-mail address and confirm subscription. If you can't find our e-mail please check your spam folder."), 'status')
        : drupal_set_message(t('Thank you for your subscription.Your subscription was recorded but the confirmation mail fail to sent probably due to a server failure.'), 'warning');
    }

    return $subscriber;
  }

  public function delete($nsid) {

    $subscriber = newsletter_subscriber_load($nsid);
    $deleted = db_delete('newsletter_subscriber')
      ->condition('nsid', $nsid)
      ->execute();

    module_invoke_all('entity_delete', $subscriber, 'newsletter_subscriber');
    field_attach_delete('newsletter_subscriber', $subscriber);

    cache_clear_all();
    $this->resetCache();
    return $deleted ? TRUE : FALSE;
  }
}
