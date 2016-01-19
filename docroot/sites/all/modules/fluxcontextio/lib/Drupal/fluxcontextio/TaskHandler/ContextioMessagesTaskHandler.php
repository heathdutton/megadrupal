<?php

/**
 * @file
 * Contains ContextioMentionsTimelineTaskHandler.
 */

namespace Drupal\fluxcontextio\TaskHandler;

use Drupal\fluxcontextio\Plugin\Entity\ContextioContact;

/**
 * Event dispatcher for the Contextio contacts timeline of a given account.
 */
class ContextioMessagesTaskHandler extends ContextioTaskHandlerBase {

  /**
   * Retrieves an array.
   *
   * @param array $arguments
   *   The request arguments based on the event configuration.
   *
   * @return \Drupal\fluxcontextio\Plugin\Entity\ContextioContact[]
   *   An array of Contact entities.
   */
  protected function getMessages(array $arguments) {
    $account = $this->getAccount();
    $contacts = array();
    if ($response = $account->client()->listMessages($account->remote_id, $arguments)) {
      $data = $response->getData();
      $contacts = fluxservice_entify_multiple(array_values($data), 'fluxcontextio_message', $account);
    };
    return $contacts;
  }

  /**
   * {@inheritdoc}
   */
  public function runTask() {
    $identifier = $this->task['identifier'];
    $active_after = fluxservice_key_value('fluxcontextio.messages.date_after');
    $offset = fluxservice_key_value('fluxcontextio.messages.offset');
    $arguments = $this->getRequestArguments();
    $time = time();
    if ($contacts = $this->getMessages($arguments)) {
      foreach ($contacts as $contact) {
        $this->invokeEvent($contact);
      }

      if (count($contacts) >= $arguments['limit']) {
        $offset->set($identifier, ((int)$arguments['offset'] + (int)$arguments['limit']));
      }
      else {
        $offset->set($identifier, 0);
        $active_after->set($identifier, $time);
      }
    }
    elseif (empty($arguments['date_after'])) {
      $active_after->set($identifier, $time);
      $offset->set($identifier, 0);
    }
  }

  /**
   * Retrieves the request arguments based on the event configuration.
   *
   * @return array
   *   The request arguments.
   */
  protected function getRequestArguments() {
    $arguments = array('limit' => 100);
    // We store the remote identifier of the last Contact that was processed so
    // that we can benefit from the 'active_after' query argument.
    $active_after = fluxservice_key_value('fluxcontextio.messages.date_after');
    if ($active_val = $active_after->get($this->task['identifier'])) {
      $arguments['date_after'] = $active_val;
    }
    elseif ($active_val === NULL) {
      $arguments['date_after'] = 1;
    }

    $offset = fluxservice_key_value('fluxcontextio.messages.offset');
    if ($offset_val = $offset->get($this->task['identifier'])) {
      $arguments['offset'] = $offset_val;
    }
    elseif ($offset_val === NULL) {
      $arguments['offset'] = 0;
    }
    return $arguments;
  }

  /**
   * Invokes a rules event after a new Contact was received.
   *
   * @param ContextioContact $contact
   *   The Contact for which to invoke the event.
   */
  protected function invokeEvent(ContextioMessage $message) {
    rules_invoke_event($this->getEvent(), $this->getAccount(), $message);
  }

}

