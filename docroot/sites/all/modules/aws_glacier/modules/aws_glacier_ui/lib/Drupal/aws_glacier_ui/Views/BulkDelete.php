<?php
/**
 * Created by PhpStorm.
 * User: tba
 * Date: 12.09.14
 * Time: 12:45
 */

namespace Drupal\aws_glacier_ui\Views;

use Drupal\aws_glacier\Entity\Vault\Vault;
use Drupal\aws_glacier\Entity\Job\Job;

/**
 * Class BulkDelete
 * @package Drupal\aws_glacier_ui\Views
 */
class BulkDelete  extends \views_handler_field_entity {
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
    if (($id = $job->identifier()) && ($completed = $job->getCompletionDate()) && !empty($completed) && $entity->NumberOfArchives > 0) {
      $this->options['alter']['path'] = AWS_GLACIER_ADMIN_PATH . '/jobs/' . $id . '/bulkdelete';
    }
    return t('Delete all archives');
  }
}
