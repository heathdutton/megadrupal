<?php

/**
 * @file
 * Contains ContextioMessage.
 */

namespace Drupal\fluxcontextio\Plugin\Entity;

use Drupal\fluxservice\Entity\FluxEntityInterface;
use Drupal\fluxservice\Entity\RemoteEntity;

/**
 * Entity class for Contextio Messages.
 */
class ContextioMessage extends RemoteEntity implements ContextioMessageInterface {

  public function __construct(array $values = array(), $entity_type = NULL) {
    $values['addresses_from'] = $values['addresses']['from']['email'];
    foreach ($values['addresses']['to'] as $to) {
      $values['addresses_to'][] = $to['email'];
    }
    $values['addresses_to'] = implode(',', $values['addresses_to']);
    parent::__construct($values, $entity_type);
  }

  /**
   * Defines the entity type.
   *
   * This gets exposed to hook_entity_info() via fluxservice_entity_info().
   */
  public static function getInfo() {
    return array(
      'name' => 'fluxcontextio_message',
      'label' => t('Contextio: Message'),
      'module' => 'fluxcontextio',
      'service' => 'fluxcontextio',
      'controller class' => '\Drupal\fluxcontextio\ContextioMessageController',
      'entity keys' => array(
        'id' => 'drupal_entity_id',
        'remote id' => 'message_id',
      ),
      'fluxservice_efq_driver' => array(
        'default' => '\Drupal\fluxcontextio\ContextioMessageQueryDriver',
      ),
    );
  }

  /**
   * Gets the entity property definitions.
   */
  public static function getEntityPropertyInfo($entity_type, $entity_info) {
    $info['addresses'] = array(
      'label' => t('Addresses'),
      'description' => t('The unique remote identifier of the Message.'),
      'type' => 'struct',
    );

    $info['addresses_from'] = array(
      'label' => t('Addresses'),
      'description' => t('The unique remote identifier of the Message.'),
      'type' => 'text',
    );

    $info['addresses_to'] = array(
      'label' => t('Addresses'),
      'description' => t('The unique remote identifier of the Message.'),
      'type' => 'text',
    );

    $info['date'] = array(
      'label' => t('Date'),
      'description' => t('The number of emails sent and received.'),
      'type' => 'date',
    );

    $info['folders'] = array(
      'label' => t(''),
      'description' => t('The name of the message.'),
      'type' => 'text',
    );

    $info['sources'] = array(
      'label' => t(''),
      'description' => t('Thumbnail of the message.'),
      'type' => 'text',
    );

    $info['subject'] = array(
      'label' => t('Subject'),
      'description' => t('.'),
      'type' => 'text',
    );

    $info['message_id'] = array(
      'label' => t('Message Id'),
      'description' => t('.'),
      'type' => 'text',
    );

    $info['email_message_id'] = array(
      'label' => t('Email message Id'),
      'description' => t('.'),
      'type' => 'text',
    );

    $info['person_info'] = array(
      'label' => t('Person Info'),
      'description' => t('.'),
      'type' => 'text',
    );

    $info['date_received'] = array(
      'label' => t(''),
      'description' => t('.'),
      'type' => 'integer',
    );

    $info['date_indexed'] = array(
      'label' => t('Date Indexed'),
      'description' => t('.'),
      'type' => 'integer',
    );

    $info['resource_url'] = array(
      'label' => t('Resource Url'),
      'description' => t('.'),
      'type' => 'text',
    );

    return $info;
  }

}

