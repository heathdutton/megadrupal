<?php
/**
 * @file
 * implements NewsletterProvider using CleverReach API.
 *
 * See http://api.cleverreach.com/soap/doc/5.0/ for documentation.
 */

namespace Drupal\campaignion_newsletters_cleverreach;

use \Drupal\campaignion_newsletters\ApiError;
use \Drupal\campaignion_newsletters\ApiPersistentError;
use \Drupal\campaignion_newsletters\NewsletterList;
use \Drupal\campaignion_newsletters\ProviderBase;
use \Drupal\campaignion_newsletters\Subscription;

class CleverReach extends ProviderBase {
  protected $account;
  protected $key;
  protected $url;
  protected $api;
  /**
   * Constructor. Gets settings and fetches intial group list.
   */
  public function __construct(array $params) {
    $this->account = $params['name'];
    $this->key = $params['key'];

    $url = variable_get('cleverreach_wsdl_url');
    $this->api = new \SoapClient($url);
  }

  /**
   * Fetches current lists from the provider.
   *
   * @return array
   *   An array of associative array
   *   (properties: identifier, title, source, language).
   */
  public function getLists() {
    $lists = array();
    $groups = $this->listGroups();
    foreach ($groups as $group) {
      $details = $this->getGroupDetails($group);
      $id = $this->toIdentifier($details->name);
      $lists[] = NewsletterList::fromData(array(
        'identifier' => $id,
        'title'      => $details->name,
        'source'     => 'CleverReach-' . $this->account,
        'data'       => $details,
        // @TODO: find a way to get an actual list specific language.
      ));
    }
    return $lists;
  }

  /**
   * Fetches current lists of subscribers from the provider.
   *
   * @return array
   *   an array of subscribers.
   */
  public function getSubscribers($list) {
    $page = 0;
    $receivers = array();

    $group_id = $list->data->id;

    do {
      $result = $this->api->receiverGetPage($this->key, $group_id,
                array(
                  'page'   => $page++,
                  'filter' => 'active',
                ));
      if ($result->message == 'data not found') {
        return $receivers;
      }
      else {
        $new_receivers = $this->handleResult($result);
        foreach ($new_receivers as $new_receiver) {
          $receivers[] = $new_receiver->email;
        }
      }
    } while ($new_receivers);
    return $receivers;
  }

  protected function attributeData(Subscription $subscription) {
    $list = $subscription->newsletterList();
    $attributes = array();

    $listAttributes = array_merge($list->data->attributes, $list->data->globalAttributes);
    if ($source = $this->getSource($subscription, 'cleverreach')) {
      foreach ($listAttributes as $attribute) {
        if ($value = $source->value($attribute->key)) {
          $attributes[] = array(
            'key' => $attribute->key,
            'value' => $value,
          );
        }
      }
    }
    return $attributes;
  }

  public function data(Subscription $subscription) {
    $data = $this->attributeData($subscription);
    $attr = $data;
    unset($attr['updated']);
    $fingerprint = sha1(serialize($attr));
    return array($data, $fingerprint);
  }

  /**
   * Subscribe a user, given a newsletter identifier and email address.
   *
   * @return: True on success.
   */
  public function subscribe($list, $mail, $data, $opt_in = FALSE, $welcome = FALSE) {
    $user = array(
      'email'  => $mail,
      'active' => TRUE,
      'attributes' => $data,
    );
    $group_id = $list->data->id;
    $result = $this->api->receiverGetByEmail($this->key, $group_id, $mail, 0);
    if ($result->message === 'data not found') {
      $result = $this->api->receiverAdd($this->key, $group_id, $user);
    }
    else {
      $result = $this->api->receiverUpdate($this->key, $group_id, $user);
    }
    return (bool) $this->handleResult($result);
  }

  /**
   * Unsubscribe a user, given a newsletter identifier and email address.
   *
   * Should ignore the request if there is no such subscription.
   *
   * @return: True on success.
   */
  public function unsubscribe($list, $mail) {
    $result = $this->api->receiverDelete($this->key, $list->data->id, $mail);
    return (bool) $this->handleResult($result);
  }

  /**
   * Fetches a list of groups (without details). Called by the constructor.
   */
  protected function listGroups() {
    $data = $this->handleResult($this->api->groupGetList($this->key));
    $return = array();
    if ($data !== FALSE) {
      foreach ($data as $group) {
        $identifier = $this->toIdentifier($group->name);
        $return[$identifier] = $group;
      }
      return $return;
    }
    else {
      return array();
    }
  }

  /**
   * Fetches details for a single, given group.
   */
  protected function getGroupDetails($group) {
    $result = $this->api->groupGetDetails($this->key, $group->id);
    return $this->handleResult($result);
  }

  /**
   * Handles errors if any, extracts data if not.
   */
  protected function handleResult($result) {
    if ($result->status !== 'SUCCESS') {
      $b = 'CleverReach';
      $args = array(
        '@status' => $result->status,
        '@message' => $result->message,
      );
      switch ($result->statuscode) {
        case 20:
        case 40:
          throw new ApiPersistentError($b, '@status #@code @message - removing item from queue.', $args, $result->statuscode);
        default:
          throw new ApiError($b, '@status #@code: @message', $args, $result->statuscode);
      }
    }
    return $result->data;
  }

  /**
   * Helper to create unified identifiers for newsletters.
   */
  public function toIdentifier($string) {
    return strtolower(drupal_clean_css_identifier($string));
  }

  /**
   * Protected clone method to prevent cloning of the singleton instance.
   */
  protected function __clone() {}

  /**
   * Protected unserialize method to prevent unserializing of singleton.
   */
  protected function __wakeup() {}
}
