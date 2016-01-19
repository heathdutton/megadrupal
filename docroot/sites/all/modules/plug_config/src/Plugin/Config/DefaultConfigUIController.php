<?php

/**
 * @file
 * Contains class \Drupal\twisters_contract\Entity\Contract\ContractTypeUIController.
 */

namespace Drupal\plug_config\Plugin\Config;

use EntityDefaultUIController;

class DefaultConfigUIController extends \EntityDefaultUIController {

  /**
   * {@inheritdoc}
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $items[$this->path . '/add']['page callback'] = 'plug_config_get_form';
    $items[$this->path . '/manage/' . $wildcard . '/clone']['page callback'] = 'plug_config_get_form';
    $items[$this->path . '/manage/' . $wildcard]['page callback'] = 'plug_config_get_form';
    return $items;
  }

}
