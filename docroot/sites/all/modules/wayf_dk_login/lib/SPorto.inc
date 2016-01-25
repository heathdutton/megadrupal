<?php
/**
 * @file
 * SPorto is a minimal SAML SP implementation for use
 * in a hub federation as wayf.dk.
 *
 * Core functionality is:
 * - Send a signed AuthnRequest to an IdP - Only one IdP supported
 * - Receive and verify a signed SAMLResponse
 * - Accept an optional list of IdP entityID's used for scoping
 *
 * It returns an array of the attributes in the AttributeStatement of the
 * response and the response it self.
 */

/**
 * @namespace
 */
namespace WAYF;
use WAYF\SPortoException;

/**
 * The main SPorto class
 */
class SPorto {
  protected $config = array();

  /**
   * Function __construct.
   *
   * @param [array] $config
   *   config : configuration
   */
  public function __construct($config) {
    $this->config = $config;
  }

  /**
   * Authentication with the Single SignOn Service.
   *
   * @param array $providerids
   *
   * @return array
   *
   * @throws \WAYF\SPortoException
   */
  public function authenticate($providerids = array()) {
    if (isset($_POST['SAMLResponse'])) {
      // Handle SAML response.
      $message = base64_decode(check_plain($_POST['SAMLResponse']));
      $document = new \DOMDocument();
      $document->loadXML($message);
      $xp = new \DomXPath($document);
      $xp->registerNamespace('ds', 'http://www.w3.org/2000/09/xmldsig#');
      $xp->registerNamespace('samlp', 'urn:oasis:names:tc:SAML:2.0:protocol');
      $xp->registerNamespace('saml', 'urn:oasis:names:tc:SAML:2.0:assertion');
      $this->verifySignature($xp, TRUE);
      $this->validateResponse($xp);
      $this->storeSessionInformation($xp);

      return array(
        'attributes' => $this->extractAttributes($xp),
        'response' => $message,
      );
    }
    else {
      // Handle SAML request.
      $id = '_' . sha1(uniqid(mt_rand(), TRUE));
      $issue_instant = gmdate('Y-m-d\TH:i:s\Z', time());
      $sp = $this->config['entityid'];
      $asc = $this->config['asc'];
      $sso = $this->config['sso'];
      // Add scoping.
      $scoping = '';
      foreach ($providerids as $provider) {
        $scoping .= "<samlp:IDPEntry ProviderID=\"$provider\"/>";
      }
      if ($scoping) {
        $scoping = '<samlp:Scoping><samlp:IDPList>' . $scoping . '</samlp:IDPList></samlp:Scoping>';
      }
      // Construct request.
      $request = <<<eof
<?xml version="1.0"?>
<samlp:AuthnRequest
    ID="$id"
    Version="2.0"
    IssueInstant="$issue_instant"
    Destination="$sso"
    AssertionConsumerServiceURL="$asc"
    ProtocolBinding="urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST"
    xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol">
    <saml:Issuer xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion">$sp</saml:Issuer>
    $scoping
</samlp:AuthnRequest>
eof;

      // Construct request.
      $querystring = "SAMLRequest=" . urlencode(base64_encode(gzdeflate($request)));;
      $querystring .= '&SigAlg=' . urlencode('http://www.w3.org/2000/09/xmldsig#rsa-sha1');

      // Get private key.
      $key = openssl_pkey_get_private($this->config['private_key']);
      if (!$key) {
        throw new SPortoException('Invalid private key used');
      }

      // Sign the request.
      $signature = "";
      openssl_sign($querystring, $signature, $key, OPENSSL_ALGO_SHA1);
      openssl_free_key($key);

      // Send request.
      header('Location: ' . $this->config['sso'] . "?" . $querystring . '&Signature=' . urlencode(base64_encode($signature)));
      exit;
    }
  }

  /**
   * Logout using the current session information.
   *
   * @throws \WAYF\SPortoException
   */
  public function logout() {
    $id = '_' . sha1(uniqid(mt_rand(), TRUE));
    $issue_instant = gmdate('Y-m-d\TH:i:s\Z', time());
    $sp = $this->config['entityid'];
    $slo = $this->config['slo'];

    $ids = $_SESSION['wayf_dk_login'];

    // Construct request.
    $request = <<<eof
<samlp:LogoutRequest
    xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol"
    xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion"
    ID="{$id}"
    Version="2.0"
    Destination="{$slo}"
    IssueInstant="{$issue_instant}">
    <saml:Issuer>{$sp}</saml:Issuer>
    <saml:NameID SPNameQualifier="urn:mace:feide.no:services:no.feide.foodle" Format="urn:oasis:names:tc:SAML:2.0:nameid-format:transient">{$ids['nameID']}</saml:NameID>
    <samlp:SessionIndex>{$ids['sessionIndex']}</samlp:SessionIndex>
</samlp:LogoutRequest>
eof;

    // Construct request.
    $query = "SAMLRequest=" . urlencode(base64_encode(gzdeflate($request)));;
    $query .= '&SigAlg=' . urlencode('http://www.w3.org/2000/09/xmldsig#rsa-sha1');

    // Get private key.
    $key = openssl_pkey_get_private($this->config['private_key']);
    if (!$key) {
      throw new SPortoException('Invalid private key used');
    }

    // Sign the request.
    $signature = "";
    openssl_sign($query, $signature, $key, OPENSSL_ALGO_SHA1);
    openssl_free_key($key);

    // Remove session information to end redirect loop in logout endpoint. This
    // assumes that we get logged out at WAYF. This is not optimal, but the best
    // we have.
    unset($_SESSION['wayf_dk_login']);

    // Send logout request.
    header('Location: ' . $slo . "?" . $query . '&Signature=' . urlencode(base64_encode($signature)));
    exit;
  }

  /**
   * Check if the user is logged in.
   *
   * As we don't know if the user is logged in we simply check if session WAYF
   * variable exists for the user. This don't grantee that the user is logged
   * into WAYF, but it's the best we have.
   */
  public function isLoggedIn() {
    $ids = $_SESSION['wayf_dk_login'];
    return (isset($ids['sessionIndex']) && isset($ids['nameID']));
  }

  /**
   * Function extractAttributes.
   *
   * @param [object] $xp
   *   xp : samlresponse
   *
   * @return array
   *   array with attributes
   */
  protected function extractAttributes($xp) {
    $res = array();
    // Grab attributes from AttributeSattement.
    $attributes  = $xp->query("/samlp:Response/saml:Assertion/saml:AttributeStatement/saml:Attribute");
    foreach ($attributes as $attribute) {
      $valuearray = array();
      $values = $xp->query('./saml:AttributeValue', $attribute);
      foreach ($values as $value) {
        $valuearray[] = $value->textContent;
      }
      $res[$attribute->getAttribute('Name')] = $valuearray;
    }
    return $res;
  }

  /**
   * Stores nameID and sessionID in drupal session.
   *
   * This information is needed to enabled logout from WAYF.dk.
   *
   * @param $xp
   *   xp : samlresponse
   */
  protected function storeSessionInformation($xp) {
    $assertion = $xp->query('/samlp:Response/saml:Assertion')->item(0);
    $_SESSION['wayf_dk_login'] = array(
      'nameID' => $xp->query('./saml:Subject/saml:NameID', $assertion)->item(0)->nodeValue,
      'sessionIndex' => $xp->query('./saml:AuthnStatement/@SessionIndex', $assertion)->item(0)->value,
    );
  }

  /**
   * Function verifySignature.
   *
   * @param object $xp
   *   xp: samlresponse
   * @param bool $assertion
   *   assertion : should assertions be checked.
   *
   * @throws \WAYF\SPortoException
   */
  protected function verifySignature($xp, $assertion = TRUE) {
    $status = $xp->query('/samlp:Response/samlp:Status/samlp:StatusCode/@Value')->item(0)->value;
    if ($status != 'urn:oasis:names:tc:SAML:2.0:status:Success') {
      $statusmessage = $xp->query('/samlp:Response/samlp:Status/samlp:StatusMessage')->item(0);
      throw new SPortoException('Invalid samlp response<br/>' . $statusmessage->C14N(TRUE, FALSE));
    }

    if ($assertion) {
      $context = $xp->query('/samlp:Response/saml:Assertion')->item(0);
    }
    else {
      $context = $xp->query('/samlp:Response')->item(0);
    }

    // Get signature and digest value.
    $signaturevalue = base64_decode($xp->query('ds:Signature/ds:SignatureValue', $context)->item(0)->textContent);
    $digestvalue = base64_decode($xp->query('ds:Signature/ds:SignedInfo/ds:Reference/ds:DigestValue', $context)->item(0)->textContent);
    $signedelement = $context;
    $signature = $xp->query("ds:Signature", $signedelement)->item(0);
    $signedinfo = $xp->query("ds:SignedInfo", $signature)->item(0)->C14N(TRUE, FALSE);
    $signature->parentNode->removeChild($signature);
    $canonicalxml = $signedelement->C14N(TRUE, FALSE);

    // Get IdP certificate.
    $publickey = openssl_get_publickey("-----BEGIN CERTIFICATE-----\n" . chunk_split($this->config['idp_certificate'], 64) . "-----END CERTIFICATE-----");
    if (!$publickey) {
      throw new SPortoException('Invalid public key used');
    }

    // Verify signature.
    if (!((sha1($canonicalxml, TRUE) == $digestvalue) && @openssl_verify($signedinfo, $signaturevalue, $publickey) == 1)) {
      throw new SPortoException('Error verifying incoming SAMLResponse');
    }
  }

  /**
   * Function validateResponse.
   *
   * @param object $xp
   *   xp : samlresponse
   *
   * @throws \WAYF\SPortoException
   */
  protected function validateResponse($xp) {
    $issues = array();

    // Verify destination.
    $destination = $xp->query('/samlp:Response/@Destination')->item(0)->value;
    if ($destination != NULL && $destination != $this->config['asc']) {
      // Destination is optional.
      $issues[] = "Destination: $destination is not here; message not destined for us";
    }

    // Verify timestamps.
    $skew = 60;
    $ashortwhileago = gmdate('Y-m-d\TH:i:s\Z', time() - $skew);
    $inashortwhile = gmdate('Y-m-d\TH:i:s\Z', time() + $skew);
    $assertion = $xp->query('/samlp:Response/saml:Assertion')->item(0);
    $subjectconfirmationdata_notbefore = $xp->query('./saml:Subject/saml:SubjectConfirmation/saml:SubjectConfirmationData/@NotBefore', $assertion);

    if ($subjectconfirmationdata_notbefore->length  && $ashortwhileago < $subjectconfirmationdata_notbefore->item(0)->value) {
      $issues[] = 'SubjectConfirmation not valid yet';
    }

    $subjectconfirmationdata_notonorafter = $xp->query('./saml:Subject/saml:SubjectConfirmation/saml:SubjectConfirmationData/@NotOnOrAfter', $assertion);
    if ($subjectconfirmationdata_notonorafter->length && $inashortwhile >= $subjectconfirmationdata_notonorafter->item(0)->value) {
      $issues[] = 'SubjectConfirmation too old';
    }

    $conditions_notbefore = $xp->query('./saml:Conditions/@NotBefore', $assertion);
    if ($conditions_notbefore->length && $ashortwhileago > $conditions_notbefore->item(0)->value) {
      $issues[] = 'Assertion Conditions not yet valid';
    }

    $conditions_notonorafter = $xp->query('./saml:Conditions/@NotOnOrAfter', $assertion);
    if ($conditions_notonorafter->length && $ashortwhileago >= $conditions_notonorafter->item(0)->value) {
      $issues[] = 'Assertions Condition too old';
    }

    $authstatement_sessionnotonorafter = $xp->query('./saml:AuthStatement/@SessionNotOnOrAfter', $assertion);
    if ($authstatement_sessionnotonorafter->length && $ashortwhileago >= $authstatement_sessionnotonorafter->item(0)->value) {
      $issues[] = 'AuthnStatement Session too old';
    }

    if (!empty($issues)) {
      throw new SPortoException('Problems detected with response. ' . PHP_EOL . 'Issues: ' . PHP_EOL . implode(PHP_EOL, $issues));
    }
  }
}
