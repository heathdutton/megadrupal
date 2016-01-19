<?php
// Copyright (C) 2012 Apptegic, Inc. All rights reserved.

class EncryptedUserIdUtility {
  protected $account_key;
  protected $api_token;

  public function __construct($account_key, $api_token) {
    $this->account_key = $account_key;
    $this->api_token = $api_token;
  }

  public function encrypt($userId) {
    if (!isset($userId) || trim($userId)==='') {
      throw new InvalidArgumentException("User id is null or empty.");
    }

    $hexApiToken = str_replace("-", "", $this->api_token);
    if (strlen($hexApiToken) != 32) {
      throw new InvalidArgumentException("API token was not 32 hex characters - length was " + strlen($hexApiToken) + " characters.");
    }
    $apiToken = pack("H*", $hexApiToken);

    $ivSize = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CBC);
    $iv = str_pad(substr($this->account_key, 0, $ivSize),
    $ivSize, "\0");

    $cipher = mcrypt_module_open(MCRYPT_RIJNDAEL_128, "", MCRYPT_MODE_CBC, "");
    mcrypt_generic_init($cipher, $apiToken, $iv);

    $plainText = round(microtime(true) * 1000) . ',' . trim($userId);

    $blockSize = mcrypt_get_block_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CBC);
    $pad = $blockSize - (strlen($plainText) % $blockSize);
    $pkcs5PaddedPlainText = $plainText . str_repeat(chr($pad), $pad);

    $encryptedText = mcrypt_generic($cipher, $pkcs5PaddedPlainText);

    mcrypt_generic_deinit($cipher);
    mcrypt_module_close($cipher);

    $encodedText = str_replace(array('+','/','=',"\n"), array('-','_','',''), base64_encode($encryptedText));
    return $encodedText;
  }
}
