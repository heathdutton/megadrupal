  ------------------------------------------------------------------------------------
                           ABOUT
  ------------------------------------------------------------------------------------

	Implements an object orientated cURL wrapper. 
	
	See http://semlabs.co.uk/journal/object-oriented-curl-class-with-multi-threading

  ------------------------------------------------------------------------------------
                           USAGE
  ------------------------------------------------------------------------------------

	Define an array of cURL sessions.
	
	$sessions = array(
	  array(
		'endpoint' => [url],
		'options' => [curl options]
	  ),
	  array(
	  	'endpoint' => [url]
	  )
	);
	
	
	$response = curl_multi($sessions);
	
	See http://php.net/manual/en/function.curl-setopt.php for cURL options.

  ------------------------------------------------------------------------------------
                         INSTALLATION
  ------------------------------------------------------------------------------------
  
  	No special installation requirements, just download it to your module directory and turn it on.
 