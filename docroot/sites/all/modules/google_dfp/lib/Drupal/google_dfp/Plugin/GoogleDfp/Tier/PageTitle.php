<?php

/**
 * @file
 * Defines a page title ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

use Drupal\google_dfp\TierBase;
use Drupal\google_dfp\TierInterface;

/**
 * A page title ad tier plugin.
 */
class PageTitle extends TierBase implements TierInterface {

  /**
   * {@inheritdoc}
   */
  protected $title = 'Page title tier';

  /**
   * {@inheritdoc}
   */
  protected $defaultConfiguration = array(
    'fallback' => '',
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
    if ($title = $this->getPageTitle()) {
      return $this::filter($title);
    }
    return $this::filter($this->getConfiguration('fallback'));
  }

  /**
   * Wraps drupal_get_title().
   */
  protected function getPageTitle() {
    return drupal_get_title();
  }

}
