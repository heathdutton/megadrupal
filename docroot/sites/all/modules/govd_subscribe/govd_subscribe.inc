<?php
/**
 * @file
 * Extension of Guzzle Client that speaks to GovDelivery's API.
 */

use Guzzle\Http\Client;

class GovDeliveryClient extends Client {

  protected $username;
  protected $password;
  protected $url;

  /**
   * Constructor.
   */
  public function __construct() {
    // @todo drush dl encrypt and enable it here because security
    $this->username = variable_get('govdelivery_username', NULL);
    $this->password = variable_get('govdelivery_password', NULL);

    /* @todo refactor to use govdelivery_account_key, when out of testing
     * $this->account_key = variable_get('govdelivery_account_key', NULL);
     * $this->url = 'https://api.govdelivery.com/api/account/'
     * . $this->account_key . '/';
     */
    $this->url      = variable_get('govdelivery_url', NULL);

    parent::__construct($this->url);
  }

  /**
   * GET to the topic endpoint.
   */
  public function getTopics() {
    return $this->get('topics')->setAuth($this->username, $this->password)->send()->getBody(TRUE);
  }

  /**
   * POST to the add_subscriptions endpoint.
   */
  public function setSubscription($data) {
    $xml = "<subscriber><email>" . $data['email'] . "</email><send-notifications type='boolean'>false</send-notifications><topics type='array'><topic><code>" . $data['topic'] . "</code></topic></topics></subscriber>";
    $request = $this->post('subscribers/add_subscriptions', array(
      'Content-Type' => 'text/xml; charset=UTF8',
    ), $xml)->setAuth($this->username, $this->password);
    $response = $request->send();
    return array('status' => $response->getStatusCode(), 'response' => $response->xml());
  }

}
