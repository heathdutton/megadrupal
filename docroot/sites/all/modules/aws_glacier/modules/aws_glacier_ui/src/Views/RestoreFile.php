<?php

namespace Drupal\aws_glacier_ui\Views;

use Drupal\aws_glacier\Entity\Archive\Archive;
use Drupal\aws_glacier\Entity\Job\Job;
use Drupal\aws_glacier\Entity\Vault\Vault;

/**
 * Class RestoreFile
 * @package Drupal\aws_glacier_ui\Views
 */
class RestoreFile extends \views_handler_field_entity {

  /**
   * {@inheritDoc}
   */
  function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->render_link($entity, $values);
    }
  }

  /**
   * @param \Drupal\aws_glacier\Entity\Archive\Archive $archive
   *
   * @param $values
   *
   * @return string
   *   Link text
   */
  function render_link(Archive $archive, $values) {
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['query'] = drupal_get_destination();
    /** @var Vault $vault */
    $vault = entity_create('glacier_vault', array('VaultName' => $archive->vaultName));
    $vault = $vault->loadByUniqueProperty();
    $job = Job::loadForArchive($vault, $archive);
    if (($id = $job->identifier())) {
      $this->options['alter']['path'] = AWS_GLACIER_ADMIN_PATH . '/jobs/' . $id . '/status';
      return t('Restore status: @status', array('@status' => $job->getStatusCode()));
    }
    else {
      $this->options['alter']['path'] = AWS_GLACIER_ADMIN_PATH . '/jobs/archive/' . $this->get_value($values, 'id') . '/create';
      return t('Restore');
     }
  }
}
