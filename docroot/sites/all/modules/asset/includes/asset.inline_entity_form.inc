<?php

/**
 * @file
 * Defines the asset inline entity form controller.
 */

class AssetInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * {@inheritdoc}
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('asset'),
      'plural' => t('assets'),
    );

    return $labels;
  }

  /**
   * {@inheritdoc}
   */
  public function entityForm($entity_form, &$form_state) {
    $asset = $entity_form['#entity'];

    module_load_include('inc', 'asset', 'includes/asset.admin');
    return asset_base_form($entity_form, $form_state, $asset);
  }

  /**
   * {@inheritdoc}
   */
  public function tableFields($bundles) {
    $fields = parent::tableFields($bundles);

    $info = entity_get_info($this->entityType);
    $metadata = entity_get_property_info($this->entityType);

    $type_key = $info['entity keys']['bundle'];
    $fields[$type_key] = array(
      'type' => 'property',
      'label' => $metadata ? $metadata['properties'][$type_key]['label'] : t('Asset Type'),
      'weight' => 2,
    );

    return $fields;
  }

}
