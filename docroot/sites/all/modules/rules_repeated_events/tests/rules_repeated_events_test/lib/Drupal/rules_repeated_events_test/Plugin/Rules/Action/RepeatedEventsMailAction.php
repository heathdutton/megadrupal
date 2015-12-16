<?php

/**
 * @file
 * Contains SendEmailDigest.
 */

namespace Drupal\rules_repeated_events_test\Plugin\Rules\Action;

use Drupal\rules_repeated_events\Rules\RulesPluginHandlerBase;

/**
 * "Add a user to list" action.
 */
class RepeatedEventsMailAction extends RulesPluginHandlerBase implements \RulesActionHandlerInterface {

  /**
   * Defines the action.
   */
  public static function getInfo() {
    return static::getInfoDefaults() + array(
      'name' => 'repeated_events_mail_action',
      'label' => t('Mail Action'),
      'parameter' => array(),
      'group' => t('Rules'),
    );
  }

  public function dependencies() {
    return array(
      'rules_repeated_events_test'
    );
  }

  /**
   * Executes the action.
   */
  public function execute() {
    drupal_mail('rules_repeated_events_test', 'test', 'foo@example.com', language_default(), array(), 'admin@example.com', TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public function form_alter(&$form, $form_state, $options) {
    // Use ajax and trigger as the reload button.
    $form['help'] = array(
      '#type' => 'item',
      '#markup' => t('This action will send out a test email.'),
    );
  }


}
