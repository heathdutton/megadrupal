<?php

/**
 * @file
 * The payment kit of the module.
 */

define("CMCIC_CTLHMAC", "V1.04.sha1.php--[CtlHmac%s%s]-%s");
define("CMCIC_CTLHMACSTR", "CtlHmac%s%s");
define("CMCIC_CGI2_RECEIPT", "version=2\ncdr=%s");
define("CMCIC_CGI2_MACOK", "0");
define("CMCIC_CGI2_MACNOTOK", "1\n");
define("CMCIC_CGI2_FIELDS",
       "%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*");
define("CMCIC_CGI1_FIELDS",
       "%s*%s*%s%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s");
define("CMCIC_URLPAIEMENT", "paiement.cgi");

/**
 * This class represents all information of the payment kit.
 */
class CmcicTpe {


  // TPE Version (Ex : 3.0).
  public $sVersion;

  // TPE Number (Ex : 1234567).
  public $sNumero;

  // Company code (Ex : companyname).
  public $sCodeSociete;

  // Language (Ex : FR, DE, EN, ..).
  public $sLangue;

  // Return URL OK.
  public $sUrlOK;

  // Return URL KO.
  public $sUrlKO;

  // Payment Server URL (Ex : https://paiement.creditmutuel.fr/paiement.cgi).
  public $sUrlPaiement;

  // The Key.
  protected $sCle;

  /**
   * The constructor of the class.
   *
   * @param string $language
   *   The code language.
   */
  public function __construct($settings, $language = "FR") {

    // Initialize fields.
    $this->sVersion = $settings['version'];
    $this->sCle = $settings['security_key'];
    $this->sNumero = $settings['tpe'];
    $this->sUrlPaiement = $settings['url_server'] . CMCIC_URLPAIEMENT;

    $this->sCodeSociete = $settings['company'];
    $this->sLangue = $language;

    $this->sUrlOK = $settings['return'];
    $this->sUrlKO = $settings['cancel_return'];

  }

  /**
   * Get the private key.
   *
   * @return string
   *   The private key.
   */
  public function getCle() {
    return $this->sCle;
  }

}

/**
 * This class allows to manipulate the MAC code.
 */
class CmcicHmac {

  // The usable TPE key.
  protected $sUsableKey;

  /**
   * Constructor class.
   *
   * @param object $tpe
   *   The TPE object.
   */
  public function __construct($tpe) {
    $this->sUsableKey = $this->getUsableKey($tpe);
  }

  /**
   * Get the key to be used for MAC generation.
   *
   * @param object $tpe
   *   The TPE object.
   *
   * @return string
   *   The key ready to be used.
   */
  protected function getUsableKey($tpe) {

    $hex_str_key  = drupal_substr($tpe->getCle(), 0, 38);
    $hex_final   = "" . drupal_substr($tpe->getCle(), 38, 2) . "00";

    $cca0 = ord($hex_final);

    if ($cca0 > 70 && $cca0 < 97) {
      $hex_str_key .= chr($cca0 - 23) . drupal_substr($hex_final, 1, 1);
    }
    else {
      if (drupal_substr($hex_final, 1, 1) == "M") {
        $hex_str_key .= drupal_substr($hex_final, 0, 1) . "0";
      }
      else {
        $hex_str_key .= drupal_substr($hex_final, 0, 2);
      }
    }

    return pack("H*", $hex_str_key);

  }

  /**
   * Allows to generate the MAC seal.
   *
   * @param string $data
   *   The data to compute.
   *
   * @return string
   *   The HMAC.
   */
  public function computeHmac($data) {
    return drupal_strtolower(hash_hmac("sha1", $data, $this->sUsableKey));
  }

}
