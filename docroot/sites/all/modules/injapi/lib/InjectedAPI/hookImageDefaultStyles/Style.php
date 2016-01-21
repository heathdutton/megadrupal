<?php


class injapi_InjectedAPI_hookImageDefaultStyles_Style extends injapi_InjectedAPI_Abstract_Item {

  protected $weight = 0;

  //                                                                        Meta
  // ---------------------------------------------------------------------------

  /**
   * Change a setting on the last added effect.
   * This method should be rarely used.
   */
  function effectSetting($key, $value) {
    $this->data['effects'][$this->weight - 1][$key] = $value;
    return $this;
  }

  /**
   * Change a data setting on the last added effect.
   * This method should be rarely used.
   */
  function effectDataSetting($key, $value) {
    $this->data['effects'][$this->weight - 1]['data'][$key] = $value;
    return $this;
  }

  //                                                                     Effects
  // ---------------------------------------------------------------------------

  function effect($name, $data = array(), $settings = array()) {
    $this->data['effects'][] = array(
      'name' => $name,
      'data' => $data,
      'weight' => $this->weight++,
    ) + $settings;
    return $this;
  }

  function resize($w, $h) {
    return $this->effect('image_resize', array(
      'width' => $w,
      'height' => $h,
    ));
  }

  function scale($w, $h, $upscale = TRUE) {
    return $this->effect('image_scale', array(
      'width' => $w,
      'height' => $h,
      'upscale' => $upscale ? 1 : 0,
    ));
  }

  function scaleAndCrop($w, $h) {
    return $this->effect('image_scale_and_crop', array(
      'width' => $w,
      'height' => $h,
    ));
  }

  function crop($w, $h, $anchor = 'center-center') {
    return $this->effect('image_crop', array(
      'width' => $w,
      'height' => $h,
      'anchor' => $anchor,
    ));
  }

  function grayscale() {
    return $this->effect('image_desaturate');
  }

  function rotate($degrees, $bgcolor = '#FFFFFF', $random = FALSE) {
    return $this->effect('image_rotate', array(
      'degrees' => $degrees,
      'bgcolor' => $bgcolor,
      // Randomize the rotation angle for each image.
      // The angle specified as 'degrees' is used as a maximum.
      'random' => $random ? 1 : 0,
    ));
  }
}
