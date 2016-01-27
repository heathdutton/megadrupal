<?php

/**
 * @file
 * Defines a site-wide ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

use Drupal\google_dfp\TierBase;
use Drupal\google_dfp\TierInterface;

/**
 * A site-wide ad tier plugin.
 */
class SiteWide extends TierBase implements TierInterface {

  /**
   * {@inheritdoc}
   */
  protected $title = 'Site wide tier';

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
      '#type' => 'textfield',
      '#description' => t('Enter the default site-wide ad-tier'),
      '#default_value' => $this->getConfiguration('value'),
      '#title' => t('Tier value'),
    );
    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function getTier() {
    return $this::filter($this->getConfiguration('value'));
  }

}
