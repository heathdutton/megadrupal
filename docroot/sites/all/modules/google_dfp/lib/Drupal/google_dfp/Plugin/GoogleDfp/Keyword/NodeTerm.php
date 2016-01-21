<?php

/**
 * @file
 * Defines a node term ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Keyword;

use Drupal\google_dfp\PluginBase;
use Drupal\google_dfp\KeywordInterface;

/**
 * A node term ad tier plugin.
 */
class NodeTerm extends PluginBase implements KeywordInterface {

  /**
   * {@inheritdoc}
   */
  protected $title = 'Node term keyword';

  /**
   * {@inheritdoc}
   */
  protected $defaultConfiguration = array(
    'fields' => array(),
  );

  /**
   * {@inheritdoc}
   */
  public function settingsForm(&$form, &$form_state) {
    $element = array();
    $options = array();
    foreach (array_keys(field_read_fields(array('type' => 'taxonomy_term_reference'))) as $field_name) {
      $field = field_info_field($field_name);
      if (!empty($field['bundles']['node'])) {
        $sample_instance = field_info_instance('node', $field_name, reset($field['bundles']['node']));
        $options[$field_name] = t('@field_label (@field_name)', array(
          '@field_label' => $sample_instance['label'],
          '@field_name' => $field_name,
        ));
      }
    }
    $element['fields'] = array(
      '#type' => 'checkboxes',
      '#description' => t('Select the fields to consider for taxonomy'),
      '#default_value' => $this->getConfiguration('fields'),
      '#title' => t('Node fields'),
      '#options' => $options,
    );
    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function getKeywords() {
    if ($node = $this->getActiveNode()) {
      $items = $values = array();
      foreach (array_keys(array_filter($this->getConfiguration('fields'))) as $field_name) {
        if (!empty($node->{$field_name})) {
          $field_values = $this->getFieldItems('node', $node, $field_name);
          if (!empty($field_values)) {
            foreach ($field_values as $field_value) {
              $items[] = $field_value['tid'];
            }
          }
        }
      }
      if (!empty($items)) {
        if ($terms = $this->loadTerms($items)) {
          foreach ($terms as $term) {
            $values[] = $this::filter($term->name);
          }
          return $values;
        }
      }
    }
    return array();
  }

}
