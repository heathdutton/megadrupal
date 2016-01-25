<?php

/**
 * @file
 *   Contains ContextioUserMessageQueryDriver.
 */

namespace Drupal\fluxcontextio;

use Drupal\fluxservice\Query\RangeRemoteEntityQueryDriverBase;

/**
 * Gets users via the authorised users messages.
 */
class ContextioMessageQueryDriver extends RangeRemoteEntityQueryDriverBase {

  /**
   * Prepare executing the query.
   *
   * This may be used to check dependencies and to prepare request parameters.
   */
  protected function prepareExecute(\EntityFieldQuery $query) {
    parent::prepareExecute($query);
    if (isset($query->range['length'])) {
      $this->requestParameter = array('count' => intval($query->range['length']));
    }
  }

  /**
   * Make a request.
   *
   * @return array
   */
  protected function makeRequest() {
    $response = $this->getAccount()->account()->listMessages($this->requestParameter);
    return $response;
  }

  /**
   * Runs the count query.
   */
  protected function makeCountRequest() {
    $response = $this->getAccount()->account()->listMessages(array('count' => 200));
    return count($response);
  }

  /**
   * {@inheritdoc}
   */
  public function getAccountPlugin() {
    return 'fluxcontextio';
  }
}
