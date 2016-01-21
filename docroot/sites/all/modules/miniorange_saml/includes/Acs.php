<?php
/**
 * @package    miniOrange
 * @author	   miniOrange Security Software Pvt. Ltd.
 * @license    GNU/GPLv3
 * @copyright  Copyright 2015 miniOrange. All Rights Reserved.
 *
 *
 * This file is part of miniOrange SAML plugin.
 */

/**
 * The MiniOrangeAcs class.
 */
class MiniOrangeAcs {

  /**
   * The function processSamlResponse.
   */
  public function processSamlResponse($post, $acs_url, $cert_fingerprint, $issuer, $base_url, $spEntityId) {
    if (array_key_exists('SAMLResponse', $post)) {
      $saml_response = $post['SAMLResponse'];
    }
    else {
      throw new Exception('Missing SAMLRequest or SAMLResponse parameter.');
    }
	
    $saml_response = base64_decode($saml_response);
    $document = new DOMDocument();
    $document->loadXML($saml_response);

    $saml_response_xml = $document->firstChild;

    $cert_fingerprint = XMLSecurityKey::getRawThumbprint($cert_fingerprint);

    $signature_data = Utilities::validateElement($saml_response_xml);
    $saml_response = new SAML2_Response($saml_response_xml);
    $cert_fingerprint = preg_replace('/\s+/', '', $cert_fingerprint);
    $cert_fingerprint = iconv("UTF-8", "CP1252//IGNORE", $cert_fingerprint);

    if ($signature_data !== FALSE) {
      $valid_signature = Utilities::processResponse($acs_url, $cert_fingerprint, $signature_data, $saml_response);
      if ($valid_signature === FALSE) {
        throw new Exception('Invalid signature.');
      }
    }
	  $acs_url = substr($acs_url, 0, strpos($acs_url, "?"));
    Utilities::validateIssuerAndAudience($saml_response, $spEntityId, $issuer, $base_url);

    $username = current(current($saml_response->getAssertions())->getNameId());
    $attrs = current($saml_response->getAssertions())->getAttributes();
    // Get RelayState if any.
    if(array_key_exists('RelayState', $post)) {
      if($post['RelayState'] == 'testValidate') {
        $this->showTestResults($username, $attrs);
      }
    }
    return $username;
  }

  public function showTestResults($username, $attrs) {
    $module_path = drupal_get_path('module', 'miniorange_saml');
	
    echo '<div style="font-family:Calibri;padding:0 3%;">';
    if(!empty($username)) {
      echo '<div style="color: #3c763d;
          background-color: #dff0d8; padding:2%;margin-bottom:20px;text-align:center; border:1px solid #AEDB9A; font-size:18pt;">TEST SUCCESSFUL</div>
          <div style="display:block;text-align:center;margin-bottom:4%;"><img style="width:15%;"src="'. $module_path . '/includes/images/green_check.png"></div>';
    }
    else {
      echo '<div style="color: #a94442;background-color: #f2dede;padding: 15px;margin-bottom: 20px;text-align:center;border:1px solid #E6B3B2;font-size:18pt;">TEST FAILED</div>
          <div style="color: #a94442;font-size:14pt; margin-bottom:20px;">WARNING: Some Attributes Did Not Match.</div>
          <div style="display:block;text-align:center;margin-bottom:4%;"><img style="width:15%;"src="'. $module_path . 'includes/images/wrong.png"></div>';
    }
    echo '<span style="font-size:14pt;"><b>Hello</b>, '.$username.'</span><br/><p style="font-weight:bold;font-size:14pt;margin-left:1%;">ATTRIBUTES RECEIVED:</p>
        <table style="border-collapse:collapse;border-spacing:0; display:table;width:100%; font-size:14pt;background-color:#EDEDED;">
        <tr style="text-align:center;"><td style="font-weight:bold;border:2px solid #949090;padding:2%;">ATTRIBUTE NAME</td><td style="font-weight:bold;padding:2%;border:2px solid #949090; word-wrap:break-word;">ATTRIBUTE VALUE</td></tr>';
    if(!empty($attrs)) {
      foreach ($attrs as $key => $value) {
        echo "<tr><td style='font-weight:bold;border:2px solid #949090;padding:2%;'>" .$key . "</td><td style='padding:2%;border:2px solid #949090; word-wrap:break-word;'>" . implode("<br/>",$value) . "</td></tr>";
      }
    }  
    else {
      echo "No Attributes Received.";
    }
    echo '</table></div>';
    echo '<div style="margin:3%;display:block;text-align:center;"><input style="padding:1%;width:100px;background: #0091CD none repeat scroll 0% 0%;cursor: pointer;font-size:15px;border-width: 1px;border-style: solid;border-radius: 3px;white-space: nowrap;box-sizing: border-box;border-color: #0073AA;box-shadow: 0px 1px 0px rgba(120, 200, 230, 0.6) inset;color: #FFF;"type="button" value="Done" onClick="self.close();"></div>';
    exit;
  }

}
