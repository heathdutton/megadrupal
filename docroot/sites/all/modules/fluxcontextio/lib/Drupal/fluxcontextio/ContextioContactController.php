<?php

/**
 * @file
 * Contains ContextioContactController.
 */

namespace Drupal\fluxcontextio;

use Drupal\fluxservice\Plugin\Entity\AccountInterface;
use Drupal\fluxservice\Plugin\Entity\ServiceInterface;
use Drupal\fluxservice\Entity\RemoteEntityInterface;
use Drupal\fluxservice\Entity\RemoteEntityControllerByAccount;


/**
 * Class 
 */
class ContextioContactController extends RemoteEntityControllerByAccount {

  /**
   * {@inheritdoc}
   */
  protected function loadFromService($ids, ServiceInterface $service, AccountInterface $account) {
    $output = array();
    $client = $account->client();
    foreach ($ids as $id) {
      // We need to cast to (int) because of the strict type validation
      // implemented by Guzzle.
      if ($response = $client->getContact(array('id' => (int) $id))) {
        $output[$id] = $response;
      }
    }
    return $output;
  }

  /**
   * {@inheritdoc}
   */
  protected function sendToService(RemoteEntityInterface $tweet) {
    return $tweet->getAccount()->client()->sendContact(array('status' => $tweet->text));
  }

  /**
   * {@inheritdoc}
   */
  protected function preEntify(array &$items, ServiceInterface $service, AccountInterface $account = NULL) {
    foreach ($items as &$values) {
      if (!empty($values['user'])) {
        // Process the attached Contextio user entity.
        $values['user'] = fluxservice_bycatch($values['user'], 'fluxcontextio_user', $account);
      }

      $values['name'] = trim($values['name'], "'`\"");

      $values['firstname'] = $values['name'];
      $values['lastname'] = '';
      // attempt to split the name into first/last name
      list($first, $last, $extra) = explode(' ', $values['name']);
      if (!empty($first) && !empty($last) && empty($extra)) {
        $values['firstname'] = $first;
        $values['lastname'] = $last;
      }
      
      list($name, $domain) = explode('@', $values['email']);
      $values['domain'] = $domain;
      $values['company_name'] = strtok($domain, '.');
      $values['domain'] = $domain;
      if (empty($values['firstname']) && empty($values['firstname'])) {
        $values['firstname'] = $name;
      }
    }
  }

}
