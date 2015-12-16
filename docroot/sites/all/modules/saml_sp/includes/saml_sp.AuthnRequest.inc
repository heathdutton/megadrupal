<?php

class saml_sp_AuthnRequest extends OneLogin_Saml2_AuthnRequest {

  /**
   * Constructs the AuthnRequest object.
   *
   * @param OneLogin_Saml2_Settings $settings Settings
   */
  public function __construct(OneLogin_Saml2_Settings $settings) {
    $this->_settings = $settings;

    $spData = $this->_settings->getSPData();
    $idpData = $this->_settings->getIdPData();
    $security = $this->_settings->getSecurityData();

    $id = OneLogin_Saml2_Utils::generateUniqueID();
    $issueInstant = OneLogin_Saml2_Utils::parseTime2SAML(time());

    $nameIDPolicyFormat = $spData['NameIDFormat'];
    if (isset($security['wantNameIdEncrypted']) && $security['wantNameIdEncrypted']) {
      $nameIDPolicyFormat = OneLogin_Saml2_Constants::NAMEID_ENCRYPTED;
    }

    $providerNameStr = '';
    $organizationData = $settings->getOrganization();
    if (!empty($organizationData)) {
      $langs = array_keys($organizationData);
      if (in_array('en-US', $langs)) {
        $lang = 'en-US';
      }
      else {
        $lang = $langs[0];
      }
      if (isset($organizationData[$lang]['displayname']) && !empty($organizationData[$lang]['displayname'])) {
        $providerNameStr = <<<PROVIDERNAME
ProviderName="{$organizationData[$lang]['displayname']}"
PROVIDERNAME;
      }
    }

    $request = <<<AUTHNREQUEST
<samlp:AuthnRequest
  xmlns:samlp="urn:oasis:names:tc:SAML:2.0:protocol"
  xmlns:saml="urn:oasis:names:tc:SAML:2.0:assertion"
  ID="$id"
  Version="2.0"
  {$providerNameStr}
  IssueInstant="$issueInstant"
  Destination="{$idpData['singleSignOnService']['url']}"
  ProtocolBinding="urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST"
  AssertionConsumerServiceURL="{$spData['assertionConsumerService']['url']}">
  <saml:Issuer>{$spData['entityId']}</saml:Issuer>
    <samlp:NameIDPolicy
        Format="{$nameIDPolicyFormat}"
        AllowCreate="true" />
    <samlp:RequestedAuthnContext Comparison="exact">
        <saml:AuthnContextClassRef>{$idpData['AuthnContextClassRef']}</saml:AuthnContextClassRef>
    </samlp:RequestedAuthnContext>
</samlp:AuthnRequest>
AUTHNREQUEST;

    $this->_id = $id;
    $this->_authnRequest = $request;
  }


  /**
   * Returns deflated, base64 encoded, unsigned AuthnRequest.
   *
   */
  public function getRequest() {
    $deflatedRequest = gzdeflate($this->_authnRequest);
    $base64Request = base64_encode($deflatedRequest);
    return $base64Request;
  }

    /**
   * Returns the AuthNRequest ID.
   *
   * @return string
   */
  public function getId() {
      return $this->_id;
  }
}
