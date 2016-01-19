<?php
/**
 * @file
 * The AuthnRequest.php file for the miniorange_samlauth module.
 *
 * @package miniOrange
 *
 * @license GNU/GPLv3
 *
 * @copyright Copyright 2015 miniOrange. All Rights Reserved.
 *
 * This file is part of miniOrange SAML plugin.
 */

/**
 * The MiniOrangeAuthnRequest class.
 */
class MiniOrangeAuthnRequest {

  /**
   * The function initiateLogin.
   */
  public function initiateLogin($acs_url, $sso_url, $issuer) {
	
    $saml_request = Utilities::createAuthnRequest($acs_url, $issuer);

    $redirect = $sso_url . '?SAMLRequest=' . $saml_request;
    header('Location: ' . $redirect);
  }

}