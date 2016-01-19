<?php

class saml_sp_Response extends OneLogin_Saml2_Response {
  /**
   * Verifies that the document has the expected signed nodes.
   *
   * @return bool
   */
  public function validateSignedElements($signedElements) {
     if (count($signedElements) > 2) {
         return false;
     }
     $ocurrence = array_count_values($signedElements);

     if (
          (in_array('samlp:Response', $signedElements) && $ocurrence['samlp:Response'] > 1) ||
          (in_array('saml:Assertion', $signedElements) && $ocurrence['saml:Assertion'] > 1) ||
          (in_array('Assertion', $signedElements) && $ocurrence['Assertion'] > 1) ||
          (
            !in_array('samlp:Response', $signedElements) &&
            !in_array('saml:Assertion', $signedElements) &&
            !in_array('Assertion', $signedElements)
          )
     ) {
         return false;
     }
     return true;
  }
}
