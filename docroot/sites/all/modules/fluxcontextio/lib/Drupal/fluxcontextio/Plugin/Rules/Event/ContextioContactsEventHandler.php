<?php

/**
 * @file
 * Contains ContextioHomeTimelineEventHandler.
 */

namespace Drupal\fluxcontextio\Plugin\Rules\Event;

/**
 * Event handler for tweets on the personal timeline.
 */
class ContextioContactsEventHandler extends ContextioEventHandlerBase {

  /**
   * Defines the event.
   */
  public static function getInfo() {
    return static::getInfoDefaults() + array(
      'name' => 'fluxcontextio_contact',
      'label' => t('A new contact appears on your emails.'),
      'variables' => array(
        'account' => static::getServiceVariableInfo(),
        'contact' => static::getContactVariableInfo(),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getTaskHandler() {
    return 'Drupal\fluxcontextio\TaskHandler\ContextioContactsTaskHandler';
  }

  /**
   * {@inheritdoc}
   */
  public function summary() {
    $settings = $this->getSettings();
    if ($settings['account'] && $account = entity_load_single('fluxservice_account', $settings['account'])) {
      return t('A new Contact appears on the account for %account.', array('%account' => "{$account->label()}"));
    }
    return $this->eventInfo['label'];
  }

}
