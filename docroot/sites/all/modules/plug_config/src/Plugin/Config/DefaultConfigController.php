<?php

/**
 * @file
 * Contains class \Drupal\twisters_contract\Entity\Contract\ContractTypeController.
 */

namespace Drupal\plug_config\Plugin\Config;

use EntityAPIControllerExportable;

class DefaultConfigController extends \EntityAPIControllerExportable {

  /**
   * {@inheritdoc}
   */
  public function create(array $values = array()) {
    $values += array(
      'label' => '',
      'name' => '',
    );
    return parent::create($values);
  }

  /**
   * {@inheritdoc}
   */
  public function save($entity, \DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);
    // Rebuild menu registry. We do not call menu_rebuild directly, but set
    // variable that indicates rebuild in the end.
    // @see http://drupal.org/node/1399618
    variable_set('menu_rebuild_needed', TRUE);
  }

}
