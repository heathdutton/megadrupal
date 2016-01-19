<?php

/**
 * @file
 * Contains ContextioHomeTimelineEventHandler.
 */

namespace Drupal\fluxcontextio\Plugin\Rules\Event;

/**
 * Event handler for tweets on the personal timeline.
 */
class ContextioMessagesEventHandler extends ContextioEventHandlerBase {

  /**
   * Defines the event.
   */
  public static function getInfo() {
    return static::getInfoDefaults() + array(
      'name' => 'fluxcontextio_message',
      'label' => t('A new message appears on your emails.'),
      'variables' => array(
        'account' => static::getServiceVariableInfo(),
        'message' => static::getMessageVariableInfo(),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getTaskHandler() {
    return 'Drupal\fluxcontextio\TaskHandler\ContextioMessagesTaskHandler';
  }

  /**
   * {@inheritdoc}
   */
  public function summary() {
    $settings = $this->getSettings();
    if ($settings['account'] && $account = entity_load_single('fluxservice_account', $settings['account'])) {
      return t('A new Message appears on the account for %account.', array('%account' => "{$account->label()}"));
    }
    return $this->eventInfo['label'];
  }

}
