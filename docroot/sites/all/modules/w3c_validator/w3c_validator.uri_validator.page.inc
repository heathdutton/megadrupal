<?php

/**
 * @file
 * Page description for single URI validation.
 */

/**
 * Page callback to validate an URI.
 *
 * The URL for this callback is <yoursite.com>/validator
 * It is possible to use <yoursite.com>/validator?output=...
 *
 * @return void
 */
function w3c_validator_uri_validator_page($form, $form_state) {

  // If a URI is passed in the URL, display the result for that URI...
  if (!empty($_GET['uri'])) {
  	
  	drupal_set_title('');																						// Remove page title if on result page (just for esthetic matters)
  	
  	$form['w3c_analysis-results'] = array(													// Prepare somewhere to display the result.
  			'#type' => 'markup',
  			'#prefix' => '<div id="w3c_analysis" class="w3c_analysis-results">',
  			'#suffix' => '</div>',
  	);
  	
  	$uri = urldecode($_GET['uri']);																	// Get the URI from the URL.

  	if ($system_uri = drupal_lookup_path('source', $uri)) { 				// Check if the URL belongs to this website :
  		$uri = $system_uri;																								// If yes, use it's Drupal System path.
  	}
  	
  	$result = _w3c_validator_retrieve_result($uri);									// Look if the URI has already been validated.
		
		if ($result) {																									// If the URI has already been validated :
			if ($result->need_validation) {																		// If the result is outdated : revalidate it.
				$form_state['values']['uri'] = $uri;														// Submit the URL.
				_w3c_validator_uri_validator_form_submit($form, $form_state);		// Call validation process.
			} else {																													// Otherwise, just :
				$form['w3c_analysis-results']['#markup'] =_w3c_validator_display_result($result);										// Display the result.
			}
		} else {																												// Otherwise, it means the URI has never been validated.
			$form['w3c_analysis-results']['#markup'] = t('No results found for the URI : @uri', array('@uri' => $uri));
		}
	} else {
  	
  	// If no URI in URL: build the validation form.
  	_w3c_validator_uri_validator_build_form($form, $form_state);
  }

  drupal_add_css(drupal_get_path('module', 'w3c_validator') . '/css/w3c_validator.css');
  
  return $form;
}

/**
 * Form where you enter a uri to be validated.
 *
 * @param string $form_state
 * @return void
 */
function _w3c_validator_uri_validator_build_form(&$form, &$form_state) {
	
  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('URI of the page to validate'),
    '#description' => t('Location (URL) of the webpage that you want to send to the W3C validator.'),
    '#required' => TRUE,
  );

  $form['validate'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#ajax' => array(
			'callback' => '_w3c_validator_uri_validator_form_submit',
			'wrapper' => 'w3c_analysis',
		),
  );
  
  $form['w3c_analysis-results'] = array(
  		'#type' => 'markup',
  		'#prefix' => '<div id="w3c_analysis" class="w3c_analysis-results">',
  		'#suffix' => '</div>',
  );

  return $form;
}

/**
 * Validate a uri and store the result.
 *
 * @param string $form
 * @param string $form_state
 * @return void
 */
function _w3c_validator_uri_validator_form_submit($form, $form_state) {
	static $result;
	
  // Retrieve the URI to validate
  $uri = $form_state['values']['uri'];

  // Call the validator processor (located in w3c_validator.module)
  $result = _w3c_validator_validate_uri($uri);
  
  // Save the result
  _w3c_validator_save_result($result);
  
  // Display the result
  $form['w3c_analysis-results']['#markup'] =_w3c_validator_display_result($result);
  
  return $form['w3c_analysis-results'];
}
