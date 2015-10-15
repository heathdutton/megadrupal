<?php

namespace Drupal\aws_glacier_ui\Views;

use Drupal\aws_glacier\Entity\Job\Job;
use Drupal\aws_glacier\Entity\Vault\Vault;

/**
 * Class Inventory
 * @package Drupal\aws_glacier_ui\Views
 */
class Inventory extends \views_handler_field_entity {

  /**
   * {@inheritDoc}
   */
  function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->render_link($entity, $values);
    }
  }

  /**
   * @param \Drupal\aws_glacier\Entity\Vault\Vault $entity
   *
   * @param $values
   *
   * @return string
   *   Linktext
   */
  function render_link(Vault $entity, $values) {
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['query'] = drupal_get_destination();
    $job = Job::loadForVault($entity);
    if (($id = $job->identifier())) {
      $this->options['alter']['path'] = AWS_GLACIER_ADMIN_PATH . '/jobs/' . $id . '/status';
      return t('Inventory status: @status', array('@status' => $job->getStatusCode()));
    }
    else {
      $this->options['alter']['path'] = AWS_GLACIER_ADMIN_PATH . '/jobs/inventory/' . $entity->identifier() . '/create';
      return t('Initiate inventory');
    }
  }
}
