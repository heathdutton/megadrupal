<?php

/**
 * @file
 * Defines a placement name ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

use Drupal\google_dfp\TierBase;
use Drupal\google_dfp\TierInterface;

/**
 * A placement tier plugin.
 */
class Placement extends TierBase implements TierInterface {

  /**
   * {@inheritdoc}
   */
  protected $title = 'Placement id tier';

  /**
   * {@inheritdoc}
   */
  protected $defaultConfiguration = array(
    'value' => '',
    'weight' => 0,
  );

  /**
   * {@inheritdoc}
   */
  public function settingsForm(&$form, &$form_state) {
    $element = array();
    $element['value'] = array(
      '#type' => 'markup',
      '#markup' => t('There is no configuration for this tier.'),
    );
    if ($value = $this->getConfiguration('value')) {
      $element['value']['#markup'] = t('Using %placement', array(
        '%placement' => $value,
      ));
    }
    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function getTier() {
    return $this::filter($this->getConfiguration('value'));
  }

  /**
   * {@inheritdoc}
   */
  public function settingsFormSubmit(&$form, &$form_state) {
    $item = $form_state['item'];
    $values = $form_state['values']['tiers'][$this->getId()];
    $values['value'] = $item->placement;
    $this->setConfiguration($values);
  }

}
