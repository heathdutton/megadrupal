<?php

/**
 * Uses response object.
 */
// require_once 'Response.php';
// require_once 'Message.php';

/**
 * @file
 * 		W3CvalidatorAPI.php
 *
 * This class helps validating a page using W3Cvalidator regardless it is an
 * online or offline version.
 */
class W3CvalidatorAPI {

	// URI to the w3 validator.
	var $baseUrl = 'http://validator.w3.org/check';

	// Output format
 	// Triggers the various outputs formats of the validator.
 	// - If unset, the usual Web format will be sent.
 	// - If set to soap12, the SOAP1.2 interface will be triggered.
 	// - If set to json, the JSON output will be triggered.
	var $output	= 'soap12';

	/**
	 * W3CvalidatorAPIv3 constructor.
	 * @param string $baseUrl
	 * 						an overrided URL to a custom W3Cvalidator instance.
	 */
	function W3CvalidatorAPI($baseUrl) {
		if ($baseUrl == '') {
			$this->baseUrl = $baseUrl;
		}
	}

	/**
	 * Build the full validation URL to call W3C.
	 * @param string $uri
	 * 						the URL to validate.
	 * @return string
	 * 						the URL ready to call.
	 */
	private function makeCallUrl($uri) {
		$data = array(
				'output' 	=> $this->output,
				'uri' 		=> $uri,
		);
		return url($this->baseUrl, array('query' => $data));
	}

	/**
	 * Validate the input URI.
	 * @param string $uri
	 * 						the URL to validate.
	 */
	public function validate($uri) {

		// build the callUrl to call W3C validator
		$callUrl = $this->makeCallUrl($uri);

		// call the W3Cvalidator WS
	 	$result = drupal_http_request($callUrl);

	 	if (!isset($result->error)) {
	 		return $this->parseSOAP12Response($result->data);
	 	} else {
	 		return null;
	 	}
	}

	/**
	 * Parse an XML response from the validator
	 *
	 * This function parses a SOAP 1.2 response xml string from the validator.
	 *
	 * @param string $xml The raw soap12 XML response from the validator.
	 *
	 * @return mixed object W3Cvalidator_Response | bool false
	 */
	static function parseSOAP12Response($xml)
	{

	  // If the document the answer is correct : let's analyse it.
	  $doc = new DOMDocument();
	  if ($doc->loadXML($xml)) {

	    $response = new W3Cvalidator_Response();

	    // Get the standard CDATA elements.
	    foreach (array('uri','checkedby','doctype','charset') as $var) {
	      $element = $doc->getElementsByTagName($var);
	      if ($element->length) {
	        $response->$var = $element->item(0)->nodeValue;
	      }
	    }
	    // Handle the bool element validity.
	    $element = $doc->getElementsByTagName('validity');
	    if ($element->length &&  $element->item(0)->nodeValue == 'true') {
          $response->validity = true;
        } else {
          $response->validity = false;
        }
        // If response is unvalid : get the errors corresponding.
        if (!$response->validity) {
          $errors = $doc->getElementsByTagName('error');
          foreach ($errors as $error) {
            $response->errors[] =
            new W3Cvalidator_Message($error);
          }
        }
        $response->error_count = count($response->errors);
        // Get the eventual warnings.
        $warnings = $doc->getElementsByTagName('warning');
        foreach ($warnings as $warning) {
          $response->warnings[] =
          new W3Cvalidator_Message($warning);
        }
        $response->warning_count = count($response->warnings);
        return $response;
	  } else {
	    // Could not load the XML.
	    return false;
	  }
	}
}

?>