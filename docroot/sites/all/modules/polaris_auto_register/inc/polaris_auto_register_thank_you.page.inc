<?php

/**
 * @file
 * Provides page related functions for the Polaris Auto Register module Thank You page
 * (after a successful registration)
 */

$path_barcodegen = libraries_get_path('barcodegen');
$path_phpmailer = libraries_get_path('phpmailer');
$barcode_format = variable_get('polaris_auto_register_barcode_format', 'codabar');

// Include files from libraries
include_once($path_barcodegen . '/class/BCGFontFile.php');
include_once($path_barcodegen . '/class/BCGColor.php');
include_once($path_barcodegen . '/class/BCGDrawing.php');
include_once($path_barcodegen . '/class/BCG' . $barcode_format . '.barcode.php');
include_once($path_phpmailer . '/class.phpmailer.php');

/**
 * [polaris_auto_register_thank_you description]
 * @return [page] [thank you page with image of library card]
 */
function polaris_auto_register_thank_you() {
  $barcode = $_SESSION['barcode_values']['barcode'];
  $pin = $_SESSION['barcode_values']['pin'];
  $email = $_SESSION['barcode_values']['email'];
  $name = $_SESSION['barcode_values']['lname'] . ', ' . $_SESSION['barcode_values']['fname'];
  //$barcode = '99999901122999';

  $page = array();

  $page['polaris_auto_register_thank_you_subtitle'] = array(
    '#markup' => variable_get('polaris_auto_register_thank_you_subtitle')
  );

  $page['polaris_auto_register_thank_you_markup1'] = array(
    '#markup' => variable_get('polaris_auto_register_thank_you_markup1')
  );

  $page['polaris_auto_register_thank_you_markup2'] = array(
    '#markup' => variable_get('polaris_auto_register_thank_you_markup2')
  );

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

  $card_image_path = variable_get('polaris_auto_register_card_image_path');
  if (strstr($_SERVER['SERVER_NAME'], 'richlandlibrary.com')) {
    $page['polaris_auto_register_library_card'] = array(
      '#markup' => '<div class="par-card-container"><div class="par-card-imageone image"><img src="/' . $path2 .
        '/images/card4.png"></div><div class="par-card-imagetwo image"><img src="/' . $path2 . '/tmp/' . $barcode .
        'rs.png"></div><div class="par-card-label">' . $barcode . '</div></div>',
    );
  }
  elseif (!empty($card_image_path)) {
    $page['polaris_auto_register_library_card'] = array(
      '#markup' => '<div class="par-card-container"><div class="par-card-imageone image"><img src="/' . $path2 .
        '/images/card4-blank.png"></div><div class="par-card-logo"><img src="' . $card_image_path . '"></div><div
        class="par-card-imagetwo image"><img src="/' . $path2 . '/tmp/' . $barcode . 'rs.png"></div><div class="par-card-label">' . $barcode . '</div></div>',
    );
  }
  else {
    // No logo. Just use text.
    $page['polaris_auto_register_library_card'] = array(
      '#markup' => '<div class="par-card-container"><div class="par-card-imageone image"><img src="/' . $path2 .
        '/images/card4-blank.png"></div><div class="par-card-logo">' . variable_get('site_name') . ' Card</div><div
        class="par-card-imagetwo image"><img src="/' . $path2 . '/tmp/' . $barcode . 'rs.png"></div><div class="par-card-label">' . $barcode . '</div></div>',
    );
  }
  // DEBUGGING for email attachment image
  /*polaris_auto_register_create_card($barcode, $path2);
  $image_new = $path2 . '/tmp/lc_' . $barcode . '_n.png';
  $page['temporary'] = array(
    '#markup' => '<img src="/' . $image_new . '"><br>',
  );*/

  $pin_text = '';
  if (variable_get('polaris_auto_register_user_pin') == 1) {
    $pin_text = t('(the last four digits of your primary phone number)');
  }
  $page['polaris_auto_register_thank_you_markup_customer'] = array(
    '#markup' => 'Your new card number is <b>' . $barcode . '</b><br>Your PIN number is <b> ' . $_SESSION['barcode_values']['pin'] . '</b> ' . $pin_text,
  );

  $page['polaris_auto_register_thank_you_markup3'] = array(
    '#markup' => variable_get('polaris_auto_register_thank_you_markup3')
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

  // create the email
  polaris_auto_register_email($barcode, $pin, $email, $name, $path2);

  // unset any existing session variables
  if ($_SESSION['barcode_values']['barcode']) {
    unset($_SESSION['barcode_values']['barcode']);
  }

  if ($_SESSION['barcode_values']['fname']) {
    unset($_SESSION['barcode_values']['fname']);
  }

  if ($_SESSION['barcode_values']['lname']) {
    unset($_SESSION['barcode_values']['lname']);
  }

  if ($_SESSION['barcode_values']['pin']) {
    unset($_SESSION['barcode_values']['pin']);
  }

  if ($_SESSION['barcode_values']['email']) {
    unset($_SESSION['barcode_values']['email']);
  }

  if ($_SESSION['phone_voice1']) {
    unset($_SESSION['phone_voice1']);
  }


  if ($_SESSION['phone_voice2']) {
    unset($_SESSION['phone_voice2']);
  }

  if ($_SESSION['dob']) {
    unset($_SESSION['dob']);
  }

  return $page;

} // end of function polaris_auto_register_thank_you


/**
 * [polaris_auto_register_create_card description]
 * @param  $barcode the barcode just registered with Polaris
 * @param  $path2   the path to the blank card image
 * @return [type]          [description]
 */
function polaris_auto_register_create_card($barcode, $path2) {

  // create library card image with barcode
  if (strstr($_SERVER['SERVER_NAME'], 'richlandlibrary.com')) {
    $img1 = imagecreatefrompng($path2 . '/images/card5.png');  // destination image
  }
  else {
    $img1 = imagecreatefrompng($path2 . '/images/card5-blank.png');  // destination image
  }
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

  $font = 10;
  $width = imagefontwidth($font) * (2 * strlen($barcode)) ;
  $height = imagefontheight($font) + 10;
  $im = imagecreatefrompng($image_orig);
  $x = imagesx($im) - ($width - 40);
  $y = imagesy($im) - ($height + 10);
  $text_color = imagecolorallocate($im, 0, 0, 0);
  imagestring($im, $font, $x, $y, $barcode, $text_color);

  // write the library card image with barcode and now label
  imagepng($im, $image_new);
  imagedestroy($im);

  $card_image_path = variable_get('polaris_auto_register_card_image_path');
  if (strstr($_SERVER['SERVER_NAME'], 'richlandlibrary.com')) {
    // Do nothing further.
  }
  elseif (!empty($card_image_path)) {
    // Let's add the library logo onto the card.
    $im3 = imagecreatefrompng($image_new);
    $img3 = imagecreatefrompng($card_image_path);
    $x3 = imagesx($img3);
    $y3 = imagesy($img3);
    // Needs to be maximum of 265 x 70.
    // Set a maximum height and width
    $width = 265;
    $height = 70;
    $ratio_orig = $x3/$y3;
    if ($width/$height > $ratio_orig) {
      $width = intval($height*$ratio_orig);
    }
    else {
      $height = intval($width/$ratio_orig);
    }
    imagecopyresampled(
      $im3, // destination image (library card w/ background, barcode, & number)
      $img3, // logo
      25, // x-coordinate of destination point
      25, // y-coordinate of destination point
      0, // x - coordinate of source point
      0, // y - coordinate of source point
      $width, // destination width
      $height, // destination height
      $x3, // source width
      $y3 // source height
    );
    // write the library card image with barcode
    imagepng($im3, $image_new);
    imagedestroy($im);
  }
  else {
    // Layer the text onto it.
    $logo_text = variable_get('site_name') . ' Card';
    $im3 = imagecreatefrompng($image_new);
    $x = 25;
    $y = 25;
    imagestring($im3, $font, $x, $y, $logo_text, $text_color);

    // write the library card image with barcode and now label
    imagepng($im3, $image_new);
    imagedestroy($im3);
  }
}

/**
 * [polaris_auto_register_email - sends email confirmation with copy of card]
 * @param  $barcode the barcode just registered with Polaris
 * @param  $pin set to last four digits of primary phone number
 * @param  $email email addressed used for registration
 * @param  $name first and last name
 * @param  $path2 path to the library card
 * @return [type]          [description]
 */
function polaris_auto_register_email($barcode, $pin, $email, $name, $path2) {
  $mail = new PHPMailer(); // defaults to using php "mail()"

  $first_markup = variable_get('polaris_auto_register_email_markup1');
  $second_markup = variable_get('polaris_auto_register_email_markup2');

  $body = $first_markup;

  $body .= '<p>Card #' . check_plain($barcode) . '<br> Pin #' . check_plain($pin) . '</p>';

  $body .= $second_markup;

  $mail->SetFrom(variable_get('polaris_auto_register_email'), variable_get('site_name'));

  $address = $email;
  $mail->AddAddress($address, $name);

  $mail->Subject = 'Welcome to ' . variable_get('site_name');

  //$mail->AltBody = "To view the message, please use an HTML compatible email viewer!"; // optional

  $mail->MsgHTML($body);

  $mail->AddAttachment($path2 . '/tmp/lc_' . check_plain($barcode) . '_n.png'); // attachment


  if (!$mail->Send()) {
    drupal_set_message(t('Mailer Error: %string', array('%string' => $mail->ErrorInfo))) ;
  }
  else {
    drupal_set_message(t('A confirmation email was sent to your email address %string <br>Please check your email inbox. If you don\'t see an email from "' . variable_get('site_name') . '" try your SPAM folder.', array('%string' => $email)));
  }
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
