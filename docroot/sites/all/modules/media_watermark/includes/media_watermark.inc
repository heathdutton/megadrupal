<?php
/**
 * @file
 * Media watermark class.
 *
 * Class to process image watermarks.
 */

/**
 * Custom watermark class.
 */
class MediaWatermark {
  /**
   * Function to add watermark.
   *
   * @param object $main_img_obj
   *   image object to add watermark
   * @param object $watermark_img_obj
   *   watermark image object
   * @param object $watermark
   *   watermark position object
   *
   * @return mixed
   *   return resource of image with watermark already add
   */
  public function addWatermark($main_img_obj, $watermark_img_obj, $watermark, $ext) {
    $main_img_obj_w = imagesx($main_img_obj);
    $main_img_obj_h = imagesy($main_img_obj);
    $watermark_img_obj_w = imagesx($watermark_img_obj);
    $watermark_img_obj_h = imagesy($watermark_img_obj);

    $vm = $watermark->ver_margin;
    $hm = $watermark->hor_margin;
    switch ($watermark->hor_position) {
      case 'left':
        $margin_x = $hm;
        break;

      case 'middle':
        // Determine center position coordinates.
        $margin_x = floor(($main_img_obj_w / 2) - ($watermark_img_obj_w / 2)) + $hm;
        break;

      case 'right':
        $margin_x = $main_img_obj_w - $watermark_img_obj_w + $hm;
        break;

    }
    switch ($watermark->ver_position) {
      case 'top':
        $margin_y = $vm;
        break;

      case 'center':
        $margin_y = floor(($main_img_obj_h / 2) - ($watermark_img_obj_h / 2)) + $hm;
        break;

      case 'bottom':
        $margin_y = $main_img_obj_h - $watermark_img_obj_h + $vm;
        break;

    }

    // Set the margins for stamp and get the height/width of the stamp image.
    $sx = imagesx($watermark_img_obj);
    $sy = imagesy($watermark_img_obj);
    imagecopy($main_img_obj, $watermark_img_obj, $margin_x, $margin_y, 0, 0, $sx, $sy);
    imagejpeg($main_img_obj);
    return $main_img_obj;
  }
}
