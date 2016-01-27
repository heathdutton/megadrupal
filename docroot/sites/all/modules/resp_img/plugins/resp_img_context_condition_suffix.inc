<?php
class resp_img_context_condition_suffix extends context_condition {
  public function  condition_values() {
    $values = array(
      variable_get('resp_img_default_suffix', '') => t('Default suffix')
    );
    $sfxs = resp_img_suffix_load_all();
    foreach ($sfxs as $sfx) {
      $values[$sfx->suffix] = $sfx->label;
    }
    return $values;
  }

  public function execute($value) {
    foreach ($this->get_contexts($value) as $context) {
      $this->condition_met($context, $value);
    }
  }
}