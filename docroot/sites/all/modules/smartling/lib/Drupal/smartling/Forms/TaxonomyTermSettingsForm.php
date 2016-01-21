<?php

namespace Drupal\smartling\Forms;

class TaxonomyTermSettingsForm extends GenericEntitySettingsForm {
  public function __construct() {
    $this->entity_name_translated = t('Term');
    $this->entity_key = '#term';
  }

  protected function getOriginalEntity($entity) {
    return smartling_get_original_taxonomy_term($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'smartling_get_term_settings_form';
  }
}