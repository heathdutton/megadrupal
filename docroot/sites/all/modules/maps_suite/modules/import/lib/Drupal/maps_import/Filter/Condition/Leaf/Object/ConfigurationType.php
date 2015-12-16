<?php

/**
 * @file
 * This filter targets the configuration type. This may affect objects that
 * are fetched through the configuration stream instead of the objects steram.
 */

namespace Drupal\maps_import\Filter\Condition\Leaf\Object;

use Drupal\maps_import\Filter\Condition\Leaf\Leaf;
use Drupal\maps_import\Fetcher\Configuration;

/**
 * Condition on object configuration type.
 */
class ConfigurationType extends Leaf {

  public $multiple = TRUE;

  public $criteria = array('none');

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'config_type';
  }

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'Configuration type';
  }

  /**
   * @inheritdoc
   */
  public function isMultiple() {
    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function match(array $entity) {
    if (!$this->getCriteria()) {
      return !$this->negate;
    }

    if (empty($entity['config_type'])) {
      $entity['config_type'] = 'none';
    }

    $criteria = $this->getCriteria();
    return $this->checkNegate($entity['config_type'] == reset($criteria));
  }

  /**
   * Return the configuration type form.
   */
  public function form($form, &$form_state) {
    $form['criteria'] = array(
      '#type' => 'select',
      '#title' => $this->getTitle(),
      '#options' => $this->getOptions(),
      '#default_value' => $this->criteria,
    );

    $form['negate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Negate'),
      '#default_value' => $this->negate,
    );

    return $form;
  }

  /**
   * Get the available options for configuration type.
   */
  public function getOptions() {
    return array(
      'none' => t('Not a configuration object'),
      Configuration::LINKED_OBJECT => t('Linked object'),
      Configuration::ATTRIBUTE_OBJECT => t('Attribute-object'),
    );
  }

  /**
   * @inheritdoc
   */
  public function getLabel() {
    $count = count($this->getCriteria());
    $t_args = array(
      '%condition' => t($this->getTitle()),
      '%$criteria' => implode(', ', array_intersect_key($this->getOptions(), array_flip($this->getCriteria()))),
    );

    return $this->negate ?
      format_plural($count, '%condition is not %$criteria.', '%condition do not belong to %$criteria.', $t_args) :
      format_plural($count, '%condition is %$criteria.', '%condition belong to %$criteria.', $t_args);
  }

}
