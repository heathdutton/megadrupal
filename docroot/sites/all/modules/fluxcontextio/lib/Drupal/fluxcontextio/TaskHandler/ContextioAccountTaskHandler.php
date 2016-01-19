<?php

/**
 * @file
 * Contains ContextioHomeTimelineTaskHandler.
 */

namespace Drupal\fluxcontextio\TaskHandler;

/**
 * Event dispatcher for the Contextio home timeline of a given user.
 */
class ContextioAccountTaskHandler extends ContextioTaskHandlerBase {

  /**
   * {@inheritdoc}
   */
  protected function getAccount(array $arguments) {
    $account = $this->getAccount();
    $tweets = array();
    if ($response = $account->client()->getHomeTimeline($arguments)) {
      $tweets = fluxservice_entify_multiple($response, 'fluxcontextio_account', $account);
    };
    // Contextio sends the Tweets in the wrong order.
    return array_reverse($tweets);
  }

}
