<?php

/**
 * @file
 * Enmask Captcha Class freely available.
 * @version 1.0
 * @copyright 2011
 * @Author : Enmask.com
 */

class EnmaskCaptcha {
  private $protocol = "http://";
  private $server = "enmask.com";
  private $port = "";

  /**
   * Gets the captcha js script from enmask server.
   */
  public function getHtml($name) {
    // Only valid for drupal.
    global $language;
    $lang_code = $language->language;
    // Ends drupal code.

    if ($lang_code != '') {
      $js_append = 'data-enmask-langcode="' . $lang_code . '"';
    }
    else {
      $js_append = '';
    }

    $url = $this->protocol . $this->server . $this->port . "/Scripts/Enmask.Captcha.js";
    return '<script type="text/javascript" ' . $js_append . ' src="' . $url . '" data-enmask="true" data-enmask-name="' . $name . '"></script>';
  }

  /**
   * Validating by posting data in enmask server using drupal_http_request.
   */
  public function validate($captchaResponse, $captchaChallenge) {
    $url = $this->protocol . $this->server . $this->port . "/CaptchaFont/ValidateCaptcha";
    $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
    $data = array(
      'response' => $captchaResponse,
      'challenge' => $captchaChallenge);
    $buffer = drupal_http_request($url, array('headers' => array('Content-Type' => 'application/x-www-form-urlencoded'), 'method' => 'POST', 'data' => http_build_query($data, '', '&')));

    if ($buffer->code == '200') {
      $body = json_decode($buffer->data);
      if ($body->isValid) {
        $isValid = TRUE;
        $message = t('Thank you, CAPTCHA validated');
      }
      else {
        $isValid = FALSE;
        $message = t('You have entered the wrong CAPTCHA phrase.');
      }
    }
    else {
      $isValid = FALSE;
      $message = t('Error while connecting to enmask server');
    }
    return array($isValid, $message);
  }
}
