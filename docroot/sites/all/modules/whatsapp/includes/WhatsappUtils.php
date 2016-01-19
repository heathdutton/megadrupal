<?php

/**
 * @file
 * Whatsapp utils class implementation.
 */

/**
 * Class implementation.
 */
class WhatsappUtils {
  /**
   * Dissect country code from phone number.
   *
   * @param integer $phone
   *   Integer phone number with country code.
   *
   * @return array
   *   An associative array with country code and phone number.
   *   - cc: The detected country code.
   *   - phone: The phone number.
   *   Return FALSE if country code is not found.
   */
  public function dissect($phone) {
    if (($handle = fopen(drupal_get_path('module', 'whatsapp') . '/includes/countries.csv', 'rb')) !== FALSE) {
      while (($data = fgetcsv($handle, 1000)) !== FALSE) {
        if (strpos($phone, $data[1]) === 0) {
          // Return the first appearance.
          fclose($handle);
          return array(
            'cc' => $data[1],
            'phone' => substr($phone, strlen($data[1]), strlen($phone)),
          );
        }
      }
      fclose($handle);
    }

    return FALSE;
  }
  
  /**
   * Creates an icon image based on the original image or URI.
   * 
   * @param mixed $source
   *   Input uri/image.
   * 
   * @return string
   *   URI containing the icon image.
   */
  public function createIcon($source) {
    if (is_string($source)) {
      $source = image_load($source);
    }
    module_load_include('inc', 'image', 'image.effects');
    $icon = $source;
    $icon->source = file_create_filename('icon.' . drupal_basename($source->source), drupal_dirname($source->source));
    image_scale_and_crop_effect($icon, array(
      'width' => 100,
      'height' => 100,
    ));
    image_save($icon);
    return $icon->source;
  }
  
  /**
   * Creates an icon encoded in b64.
   * 
   * @param mixed $source
   *   Input image uri/class.
   * 
   * @return string
   *   Base 64 encoded image.
   */
  public function createIconB64($source) {
    $icon_uri = $this->createIcon($source);
    return base64_encode(file_get_contents(drupal_realpath($icon_uri)));
  }
}
