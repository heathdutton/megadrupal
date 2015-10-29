<?php
/**
 * @file
 * Source: XYZ.
 */

namespace Drupal\openlayers\Plugins\Source\XYZ;
use Drupal\Component\Annotation\Plugin;
use Drupal\openlayers\Types\Source;

/**
 * Class XYZ.
 *
 * @Plugin(
 *  id = "XYZ"
 * )
 *
 */
class XYZ extends Source {
  /**
   * {@inheritdoc}
   */
  public function optionsForm(&$form, &$form_state) {
    $form['options']['url'] = array(
      '#title' => t('URL(s)'),
      '#type' => 'textarea',
      '#default_value' => $this->getOption('url') ? $this->getOption('url') : '',
    );
  }
}
