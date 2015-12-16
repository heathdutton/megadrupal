<?php

/**
 * @file
 * Class that defines operation on MaPS Object's library attribute.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Attribute;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;

class Library extends Attribute {

  /**
   * @inheritdoc
   */
  public function processValues($values, EntityInterface $entity, ConverterInterface $currentConverter) {
    $options = $this->getOptions();
    if (empty($options)) {
      $options = $this->getOptionsDefault();
    }

    if ($options['enabled']) {
      $libraryIndex = db_select('maps_import_library_index', 'li')
        ->fields('li')
        ->condition('pid', $entity->getProfile()->getPid())
        ->execute()
        ->fetchAllAssoc('id', \PDO::FETCH_ASSOC);

      foreach ($values as $key => $value) {
        foreach ($value as $multiple => $multiple_value) {
          $values[$key][$multiple] = $libraryIndex[$multiple]['tid'];
        }
      }
    }

    return $values;
  }

  /**
   * @inheritdoc
   */
  public function hasOptions() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function optionsForm($form, &$form_state) {
    $options = $form_state['item']->getOptions();

    return array(
      'enabled' => array(
        '#type' => 'checkbox',
        '#title' => t('Enable library management'),
        '#default_value' => isset($options['source']['enabled']) ? $options['source']['enabled'] : TRUE,
        '#description' => t('Enable the library management or only map the library value.'),
      )
    );
  }

  /**
   * @inheritdoc
   */
  public function getOptionsDefault() {
    return array('enabled' => TRUE);
  }
}
