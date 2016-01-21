<?php

/**
 * @file
 * Contains ContextioContact.
 */

namespace Drupal\fluxcontextio\Plugin\Entity;

use Drupal\fluxservice\Entity\FluxEntityInterface;
use Drupal\fluxservice\Entity\RemoteEntity;

/**
 * Entity class for Contextio Contacts.
 */
class ContextioContact extends RemoteEntity implements ContextioContactInterface {

  /**
   * Defines the entity type.
   *
   * This gets exposed to hook_entity_info() via fluxservice_entity_info().
   */
  public static function getInfo() {
    return array(
      'name' => 'fluxcontextio_contact',
      'label' => t('Contextio: Contact'),
      'module' => 'fluxcontextio',
      'service' => 'fluxcontextio',
      'controller class' => '\Drupal\fluxcontextio\ContextioContactController',
      'entity keys' => array(
        'id' => 'drupal_entity_id',
        'remote id' => 'email',
      ),
      'fluxservice_efq_driver' => array(
        'default' => '\Drupal\fluxcontextio\ContextioContactQueryDriver',
      ),
    );
  }

  /**
   * Gets the entity property definitions.
   */
  public static function getEntityPropertyInfo($entity_type, $entity_info) {
    $info['email'] = array(
      'label' => t('Remote identifier'),
      'description' => t('The unique remote identifier of the Contact.'),
      'type' => 'text',
    );

    $info['count'] = array(
      'label' => t('Count'),
      'description' => t('The number of emails sent and received.'),
      'type' => 'integer',
    );

    $info['name'] = array(
      'label' => t('Name'),
      'description' => t('The name of the contact.'),
      'type' => 'text',
    );

    $info['firstname'] = array(
      'label' => t('First Name'),
      'description' => t('The name of the contact.'),
      'type' => 'text',
    );

    $info['lastname'] = array(
      'label' => t('Last Name'),
      'description' => t('The name of the contact.'),
      'type' => 'text',
    );

    $info['domain'] = array(
      'label' => t('Domain'),
      'description' => t('Domain'),
      'type' => 'text',
    );

    $info['company_name'] = array(
      'label' => t('Company Name'),
      'description' => t('Name of the company.'),
      'type' => 'text',
    );

    $info['thumbnail'] = array(
      'label' => t('Thumnail'),
      'description' => t('Thumbnail of the contact.'),
      'type' => 'text',
    );

    $info['resource'] = array(
      'label' => t('Resource'),
      'description' => t('.'),
      'type' => 'text',
    );

    return $info;
  }

}

