<?php

  $path_barcodegen = libraries_get_path('barcodegen');
  $barcode_format = variable_get('polaris_auto_register_barcode_format', 'codabar');

  // Include files from libraries
  include_once($path_barcodegen . '/class/BCGFontFile.php');
  include_once($path_barcodegen . '/class/BCGColor.php');
  include_once($path_barcodegen . '/class/BCGDrawing.php');
  include_once($path_barcodegen . '/class/BCG' . $barcode_format . '.barcode.php');

/**
 * @file polaris_auto_register_barcode.page.inc
 * [polaris_auto_register_reprint_barcode description]
 * @return [type] [description]
 */
function polaris_auto_register_reprint_barcode() {

  // the page
  $page = array();

  // markup before the form
  $subtitle = variable_get('polaris_auto_register_reprint_barcode_success_subtitle');

  $page['subtitle'] = array(
    '#markup' => $subtitle,
  );

  $markup1 = variable_get('polaris_auto_register_reprint_barcode_success_markup1');

  $page['markup1'] = array(
    '#markup' => $markup1,
  );

  $account = user_load($GLOBALS['user']->uid);
  $barcode = $account->field_patron_login_user_id['und'][0]['value'];

  if (empty($barcode)) {
    $barcode = $account->name;
  }

  // set color for barcode and background
  $color_black = new BCGColor(0, 0, 0);
  $color_white = new BCGColor(255, 255, 255);

  // add the font to display text under barcode
  $path2 = drupal_get_path('module', 'polaris_auto_register');
  $path_barcodegen = libraries_get_path('barcodegen');
  $font = new BCGFontFile($path_barcodegen . '/font/Arial.ttf', 12);

  // Barcode Part
  $code = new BCGcodabar();
  $code->setScale(2);
  $code->setThickness(30);
  $code->setForegroundColor($color_black);
  $code->setBackgroundColor($color_white);
  $code->setFont(0);
  $code->parse('A' . $barcode . 'B');

  // Drawing Part
  $drawing = new BCGDrawing($path2 . '/tmp/' . $barcode . '.png', $color_white);
  $drawing->setBarcode($code);
  $drawing->draw();

  $drawing->finish(BCGDrawing::IMG_FORMAT_PNG);

  // resize the barcode to 85% of it's original size
  polaris_auto_register_resize_barcode($barcode, $path2);

  drupal_add_css(drupal_get_path('module', 'polaris_auto_register') . '/css/polaris_auto_register.css');

  $page['info_container1'] = array(
    '#markup' => '<div class="reprint_barcode1">',
  );

  $page['info_container2'] = array(
    '#markup' => '<div class="reprint_barcode2">',
  );

  $card_image_path = variable_get('polaris_auto_register_card_image_path');
  if (strstr($_SERVER['SERVER_NAME'], 'richlandlibrary.com')) {
    $page['polaris_auto_register_reprint_library_card'] = array(
      '#markup' => '<div class="par-card-container"><div class="par-card-imageone image"><img src="/' . $path2 . '/images/card4.png"></div><div class="par-card-imagetwo image"><img src="/' . $path2 . '/tmp/' . $barcode . 'rs.png"></div><div class="par-card-label">' . $barcode . '</div></div>',
    );
  }
  elseif (!empty($card_image_path)) {
    $page['polaris_auto_register_reprint_library_card'] = array(
      '#markup' => '<div class="par-card-container"><div class="par-card-imageone image"><img src="/' . $path2 . '/images/card4-blank.png"></div><div class="par-card-logo"><img src="' . $card_image_path . '"></div><div class="par-card-imagetwo image"><img src="/' . $path2 . '/tmp/' . $barcode . 'rs.png"></div><div class="par-card-label">' . $barcode . '</div></div>',
    );
  }
  else {
    // No logo. Just use text.
    $page['polaris_auto_register_reprint_library_card'] = array(
      '#markup' => '<div class="par-card-container"><div class="par-card-imageone image"><img src="/' . $path2 . '/images/card4-blank.png"></div><div class="par-card-logo">' . variable_get('site_name') . ' Card</div><div class="par-card-imagetwo image"><img src="/' . $path2 . '/tmp/' . $barcode . 'rs.png"></div><div class="par-card-label">' . $barcode . '</div></div>',
    );
  }

  $page['info_container2a'] = array(
    '#markup' => '</div>',
  );

  $page['info_container3'] = array(
    '#markup' => '<div class="reprint_barcode3">',
  );

   $page['polaris_auto_register_reprinted_barcode'] = array(
     '#prefix' => '<div class="par-barcode-info">',
     '#suffix' => '</div>',
     '#markup' => 'Your card number is <b>' . $barcode . '</b>',
   );

  $page['info_container3a'] = array(
    '#markup' => '</div>',
  );

  $page['info_container4'] = array(
    '#markup' => '<div class="reprint_barcode4">',
  );

  $page['info_container4a'] = array(
    '#markup' => '</div>',
  );

  $page['info_container1a'] = array(
    '#markup' => '</div>',
  );

  $markup2 = variable_get('polaris_auto_register_reprint_barcode_success_markup2');

  $page['markup2'] = array(
    '#markup' => $markup2,
  );

  $jslink = "javascript:window.print()";
  $page['library_card_print'] = array(
    '#type' => 'submit',
    '#value' => t('Print Your Library Card'),
    '#attributes' => array('onclick' => $jslink),
    '#executes_submit_callback' => FALSE,
  );

  // create a library card image
  polaris_auto_register_create_card($barcode, $path2);


  return $page;

  // markup after the form

}

/**
 * [polaris_auto_register_create_card description]
 * @param  $barcode the barcode just registered with Polaris
 * @param  $path2   the path to the blank card image
 * @return [type]          [description]
 */
function polaris_auto_register_create_card($barcode, $path2) {
  // create library card image with barcode
  $img1 = imagecreatefrompng($path2 . '/images/card5.png');  // destination image
  $img2 = imagecreatefrompng($path2 . '/tmp/' . $barcode . '.png'); // source image

  $x1 = imagesx($img1);
  $y1 = imagesy($img1);
  $x2 = imagesx($img2);
  $y2 = imagesy($img2);

  imagecopyresampled(
  $img1, // destination image (library card image)
  $img2, // source image (our barcode)
  10, // x-coordinate of destination point
  100, // y-coordinate of destination point
  0, // x - coordinate of source point
  0, // y - coordinate of source point
  (intval($x1 * .95)), // destination width
  (intval($y1/4)), // destination height
  $x2, // source width
  $y2 // source height
  );

  // write the library card image with barcode
  imagepng($img1, $path2 . '/tmp/lc_' . $barcode . '.png', 0);
  imagedestroy($img1);

  header("Content-type: image/png");

  $image_orig = $path2 . '/tmp/lc_' . $barcode . '.png';
  $image_new = $path2 . '/tmp/lc_' . $barcode . '_n.png';

  $font = 8;
  $width = imagefontwidth($font) * (2 * strlen($barcode)) ;
  $height = imagefontheight($font) + 10;
  $im = imagecreatefrompng($image_orig);
  $x = imagesx($im) - ($width - 25);
  $y = imagesy($im) - ($height + 10);
  $background_color = imagecolorallocate($im, 255, 255, 255);
  $text_color = imagecolorallocate($im, 0, 0, 0);
  imagestring($im, $font, $x, $y, $barcode, $text_color);

  // write the library card image with barcode and now label
  imagepng($im, $image_new);
  imagedestroy($im);
}

function polaris_auto_register_resize_barcode($barcode, $path2) {
  $filename = $path2 . '/tmp/' . $barcode . '.png';
  $filename2 = $path2 . '/tmp/' . $barcode . 'rs.png';

  $percent = .85;

  // Content type
  header("Content-type: image/png");

  // Get new dimensions
  list($width, $height) = getimagesize($filename);
  $new_width = intval($width * $percent);
  $new_height = intval($height * $percent);

  // Resample
  $image = imagecreatefrompng($filename);
  $new_image = imagecreatetruecolor($new_width, $new_height);
  imagecopyresampled($new_image, $image, 0, 0, 0, 0, $new_width, $new_height, $width, $height);

  // Output
  imagepng($new_image, $filename2);


}


