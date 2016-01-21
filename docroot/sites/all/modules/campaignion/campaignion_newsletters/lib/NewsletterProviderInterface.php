<?php
/**
 * @file
 * Abstract base class for email marketing providers
 */

namespace Drupal\campaignion_newsletters;

interface NewsletterProviderInterface {
  public function __construct(array $params);
  /**
   * Fetches current lists from the provider.
   *
   * @return array
   *   An array of Drupal\campaignion_newsletters\NewsletterList objects
   */
  public function getLists();

  /**
   * Fetches current lists of subscribers from the provider.
   *
   * @return array
   *   an array of subscribers.
   */
  public function getSubscribers($list);

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   *
   * @return: True on success.
   */
  public function subscribe($newsletter, $mail, $data, $opt_in = FALSE, $welcome = FALSE);

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   *
   * Should ignore the request if there is no such subscription.
   *
   * @return: True on success.
   */
  public function unsubscribe($newsletter, $mail);

  /**
   * Get additional data for this subscription and a unique fingerprint.
   *
   * @param Subscription $subscription
   *   The subscription object.
   *
   * @return array
   *   An array containing some data object and a fingerprint:
   *   array($data, $fingerprint).
   *   - The $data is passed as $data parameter of subscribe() during
   *     cron runs.
   *   - The $fingerprint must be an sha1-hash. Usually it's a hash
   *     of some subset of $data.
   */
  public function data(Subscription $subscription);
}
