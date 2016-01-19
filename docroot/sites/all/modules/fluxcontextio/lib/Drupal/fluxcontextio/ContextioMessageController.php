<?php

/**
 * @file
 * Contains ContextioMessageController.
 */

namespace Drupal\fluxcontextio;

use Drupal\fluxservice\Plugin\Entity\AccountInterface;
use Drupal\fluxservice\Plugin\Entity\ServiceInterface;
use Drupal\fluxservice\Entity\RemoteEntityInterface;
use Drupal\fluxservice\Entity\RemoteEntityControllerByAccount;

/**
 * Class 
 */
class ContextioMessageController extends RemoteEntityControllerByAccount {

  /**
   * {@inheritdoc}
   */
  protected function loadFromService($ids, ServiceInterface $service, AccountInterface $account) {
    $output = array();
    $client = $account->client();
    foreach ($ids as $id) {
      // We need to cast to (int) because of the strict type validation
      // implemented by Guzzle.
      if ($response = $client->getMessage(array('id' => (int) $id))) {
        $output[$id] = $response;
      }
    }
    return $output;
  }

  /**
   * {@inheritdoc}
   */
  protected function sendToService(RemoteEntityInterface $tweet) {
    return $tweet->getAccount()->client()->sendMessage(array('status' => $tweet->text));
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
    }
  }

}
