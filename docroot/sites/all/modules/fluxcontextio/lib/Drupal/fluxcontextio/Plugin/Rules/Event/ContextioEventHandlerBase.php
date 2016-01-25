<?php

/**
 * @file
 * Contains TwitterEventHandlerBase.
 */

namespace Drupal\fluxcontextio\Plugin\Rules\Event;

use Drupal\fluxservice\Rules\DataUI\AccountEntity;
use Drupal\fluxservice\Rules\DataUI\ServiceEntity;
use Drupal\fluxservice\Rules\EventHandler\CronEventHandlerBase;
use Drupal\fluxcontextio\Rules\RulesPluginHandlerBase;

/**
 * Cron-based base class for Contextio event handlers.
 */
abstract class ContextioEventHandlerBase extends CronEventHandlerBase {

  /**
   * Returns info-defaults for contextio plugin handlers.
   */
  public static function getInfoDefaults() {
    return RulesPluginHandlerBase::getInfoDefaults();
  }

  /**
   * Rules contextio integration access callback.
   */
  public static function integrationAccess($type, $name) {
    return fluxservice_access_by_plugin('fluxcontextio');
  }

  /**
   * Returns info for the provided contextio service account variable.
   */
  public static function getServiceVariableInfo() {
    return array(
      'type' => 'fluxservice_account',
      'bundle' => 'fluxcontextio',
      'label' => t('Contextio account'),
      'description' => t('The account used for authenticating with the Contextio API.'),
    );
  }

  /**
   * Returns info for the provided tweet variable.
   */
  public static function getContactVariableInfo() {
    return array(
      'type' => 'fluxcontextio_contact',
      'label' => t('Contact'),
      'description' => t('The contact that triggered the event.'),
    );
  }

  /**
   * Returns info for the provided tweet variable.
   */
  public static function getMessageVariableInfo() {
    return array(
      'type' => 'fluxcontextio_message',
      'label' => t('Message'),
      'description' => t('The message that triggered the event.'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getDefaults() {
    return array(
      'account' => '',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array &$form_state) {
    $settings = $this->getSettings();

    $form['account'] = array(
      '#type' => 'select',
      '#title' => t('Account'),
      '#description' => t('The service account used for authenticating with the Contextio API.'),
      '#options' => AccountEntity::getOptions('fluxcontextio', $form_state['rules_config']),
      '#default_value' => $settings['account'],
      '#required' => TRUE,
      '#empty_value' => '',
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function getEventNameSuffix() {
    return drupal_hash_base64(serialize($this->getSettings()));
  }

}
