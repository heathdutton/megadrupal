<?php

/**
 * @file
 * Contains ContextioTaskHandlerBase.
 */

namespace Drupal\fluxcontextio\TaskHandler;

use Drupal\fluxservice\Rules\TaskHandler\RepetitiveTaskHandlerBase;

/**
 * Base class for Contextio task handlers that dispatch Rules events.
 */
class ContextioTaskHandlerBase extends RepetitiveTaskHandlerBase {

  /**
   * The account associated with this task.
   *
   * @var \Drupal\fluxcontextio\Plugin\Service\ContextioAccount
   */
  protected $account;


  protected $incomplete_task = FALSE;

  /**
   * Gets the configured event name to dispatch.
   */
  public function getEvent() {
    return $this->task['identifier'];
  }

  /**
   * Gets the configured Contextio account.
   *
   * @throws \RulesEvaluationException
   *   If the account cannot be loaded.
   *
   * @return \Drupal\fluxcontextio\Plugin\Service\ContextioAccount
   *   The account associated with this task.
   */
  public function getAccount() {
    if (isset($this->account)) {
      return $this->account;
    }
    if (!$account = entity_load_single('fluxservice_account', $this->task['data']['account'])) {
      throw new \RulesEvaluationException('The specified Contextio account cannot be loaded.', array(), NULL, \RulesLog::ERROR);
    }
    $this->account = $account;
    return $this->account;
  }

  /**
   * {@inheritdoc}
   */
  public function afterTaskQueued() {
    $increment = 0;
    if ($this->incomplete_task == FALSE) {
      try {
        $service = $this->getAccount()->getService();
        $increment = $service->getPollingInterval();
      }
      catch(\RulesEvaluationException $e) {
        rules_log($e->msg, $e->args, $e->severity);
      }
    }
    // Continuously reschedule the task.
    db_update('rules_scheduler')
      ->condition('tid', $this->task['tid'])
      ->fields(array('date' => $this->task['date'] + $increment))
      ->execute();
  }
}
