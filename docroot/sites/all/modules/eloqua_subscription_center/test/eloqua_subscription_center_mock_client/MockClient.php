<?php

/**
 * @file
 * Mock Elomentary client, used for testing the subscription center.
 */

class MockElomentaryClient {

  /**
   * Tracks the current active API.
   * @var string
   */
  protected $active_api = '';

  /**
   * Tracks the current contact ID when looking at subscriptions.
   * @var int
   */
  protected $subscription_cid = 0;

  /**
   * Stores contact data for behaviour tests.
   * @var array
   */
  protected $contacts = array(
    // Valid contact.
    '1' => array(
      'stored' => 'data',
      'emailAddress' => 'foobar@example.com',
    ),
  );

  /**
   * Mocks the Elomentary "api" facade.
   */
  public function api($type) {
    $this->active_api = $type;
    return $this;
  }

  /**
   * Mocks the Elomentary contact subscription client.
   */
  public function subscriptions($id) {
    $this->active_api = 'subscriptions';
    $this->subscription_cid = $id;
    return $this;
  }

  /**
   * Mocks the Elomentary e-mail group client.
   */
  public function groups() {
    $this->active_api = 'email_groups';
    return $this;
  }

  /**
   * Mocks all Elomentary "show" method calls.
   */
  public function show($id, $depth = null, $extensions = null) {
    switch ($this->active_api) {
      case 'contact':
      case 'contacts':
        return isset($this->contacts[$id]) ? $this->contacts[$id] : array();
        break;
    }
  }

  /**
   * Mocks all Elomentary "search" method calls.
   */
  public function search($search, $options = array()) {
    switch ($this->active_api) {
      case 'subscriptions':
        return array('elements' => array(
          1 => array(
            'type' => 'ContactEmailSubscription',
            'contactId' => $this->subscription_cid,
            'emailGroup' => array(
              'type' => 'EmailGroup',
              'id' => 1,
              'name' => 'This should be hidden',
              'description' => 'This test group should not be enabled.',
            ),
          ),
          2 => array(
            'type' => 'ContactEmailSubscription',
            'contactId' => $this->subscription_cid,
            'emailGroup' => array(
              'type' => 'EmailGroup',
              'id' => 2,
              'name' => 'Visible, default subscribed',
              'description' => 'This is the description from Eloqua.',
            ),
            'isSubscribed' => 'true',
          ),
          3 => array(
            'type' => 'ContactEmailSubscription',
            'contactId' => $this->subscription_cid,
            'emailGroup' => array(
              'type' => 'EmailGroup',
              'id' => 3,
              'name' => 'Visible and overridden',
              'description' => 'This description should be overridden.',
            ),
          ),
        ));
        break;

      case 'email_groups';
        return array('elements' => array(
          1 => array(
            'type' => 'EmailGroup',
            'id' => 1,
            'name' => 'This should be hidden',
            'description' => 'This test group should not be enabled.',
          ),
          2 => array(
            'type' => 'EmailGroup',
            'id' => 2,
            'name' => 'Visible, default subscribed',
            'description' => 'This is the description from Eloqua.',
          ),
          3 => array(
            'type' => 'EmailGroup',
            'id' => 3,
            'name' => 'Visible and overridden',
            'description' => 'This description should be overridden.',
          ),
          4 => array(
            'type' => 'EmailGroup',
            'id' => 4,
            'name' => 'New from e-mail group API',
            'description' => 'This group only shows up when searching the e-mail group API.',
          ),
        ));
        break;

      case 'contacts':
      case 'contact':
        return $search === $this->contacts['1']['emailAddress'] ? array('elements' => array(
          0 => array(
            'type' => 'Contact',
            'id' => 1,
            'emailAddress' => $this->contacts['1']['emailAddress'],
          ),
        )) : array();
        break;
    }
  }
}
