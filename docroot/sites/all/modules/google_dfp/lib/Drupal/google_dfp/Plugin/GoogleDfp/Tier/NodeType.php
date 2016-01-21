<?php

/**
 * @file
 * Defines a node type ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

use Drupal\google_dfp\TierBase;
use Drupal\google_dfp\TierInterface;

/**
 * A node type ad tier plugin.
 */
class NodeType extends TierBase implements TierInterface {

  /**
   * {@inheritdoc}
   */
  protected $title = 'Node type tier';

  /**
   * {@inheritdoc}
   */
  protected $defaultConfiguration = array(
    'fallback' => FALSE,
    'weight' => 0,
  );

  /**
   * {@inheritdoc}
   */
  public function settingsForm(&$form, &$form_state) {
    $element = array();
    $element['fallback'] = array(
      '#type' => 'textfield',
      '#description' => t('Enter the fallback value if there is no active node context'),
      '#default_value' => $this->getConfiguration('fallback'),
      '#title' => t('Fallback value'),
    );
    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function getTier() {
    if ($node = $this->getActiveNode()) {
      return $this::filter($node->type);
    }
    return $this::filter($this->configuration['fallback']);
  }

}
