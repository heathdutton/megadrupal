<?php
abstract class configurable_box extends boxes_box {

  public function override() {
    $existing = boxes_box_load($this->delta);
    $override = configurable_box_load_override($this->delta);

    if ($existing && ($existing->export_type & EXPORT_IN_CODE)) {
      if ($override) {
        drupal_write_record('box_overrides', $this, array('delta'));
      }
      else {
        drupal_write_record('box_overrides', $this);
      }
    }
  }

  public function reset_default() {
    self::reset();

    db_delete('box_overrides')
    ->condition('delta', $this->delta)
    ->execute();

    module_exists('context') ? context_invalidate_cache() : NULL;
  }

  public function save() {
    $existing = boxes_box_load($this->delta);
    $override = configurable_box_load_override($this->delta);

    if ($existing && $override && ($existing->export_type & EXPORT_IN_CODE)) {
      db_delete('box_overrides')
      ->condition('delta', $this->delta)
      ->execute();
    }

    parent::save();
  }
}