<?php

/**
 * Entity class for MapBox.js map presets
 */
class MapboxjsPreset extends Entity {

  public
    $mapboxjs_preset_id,
    $name,
    $label,
    $height,
    $lat,
    $lon,
    $zoom,
    $zoomer,
    $fullscreen,
    $legend,
    $layer_toggle,
    $base_layer,
    $composite,
    $type = 'mapboxjs_preset', // only one bundle type for now
    $created,
    $updated;

  public function __construct(array $values = array()) {
    parent::__construct($values, 'mapboxjs_preset');
  }

  public function save() {
    $this->updated = REQUEST_TIME;
    if (isset($this->is_new) && $this->is_new) {
      $this->created = REQUEST_TIME;
    }
    return parent::save();
  }

  protected function defaultUri() {
    return array('path' => 'admin/structure/mapboxjs/presets/manage/' . $this->identifier());
  }

}
