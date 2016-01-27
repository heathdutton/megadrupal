<?php
// $Id$

/**
 * @file
 * Administration page callbacks for the slc module.
 */

/**
 * _slc_admin_settings function
 * returns form object for General Settings in slc module.
 * @return $form
 */
function _slc_admin_settings() {
  $form['api_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("List of API Settings"),
  );
  $form['api_settings']['slc_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('CLIENT_ID'),
    '#default_value' => variable_get('slc_client_id', ''),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE, 
    '#description' => t("the CLIENT_ID of your SLC App"),
  );
  $form['api_settings']['slc_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('CLIENT_SECRET'),
    '#default_value' => variable_get('slc_client_secret', ''),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE, 
    '#description' => t("the CLIENT_SECRET of your SLC App"),
  );
  $form['submit_authenticate'] = array(
    '#type' => 'submit',
    '#value' => t('Authenticate'),
    '#submit' => array('_slc_form_submit_authenticate'),
  );
  $form['submit_import'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#submit' => array('_slc_form_submit_import'),
  );
  return system_settings_form($form);
}

function slc_jsontoxml($args = null) {
$url = $_GET['url'];
	$json =  _slc_curl_connect($url);
	$xml = generate_valid_xml_from_array($json);
	print $xml;

	//print_r( $json);
	/*@header("Content-Type: text/xml");
print '< ?xml version="1.0" ?>' . "\n";
print ARRAYtoXML($json, 2);*/
}
function generate_xml_from_array($array, $node_name) {
	$xml = '';

	if (is_array($array) || is_object($array)) {
		foreach ($array as $key=>$value) {
			if (is_numeric($key)) {
				$key = $node_name;
			}

			$xml .= '<' . $key . '>' . "\n" . generate_xml_from_array($value, $node_name) . '</' . $key . '>' . "\n";
		}
	} else {
		$xml = htmlspecialchars($array, ENT_QUOTES) . "\n";
	}

	return $xml;
}

function generate_valid_xml_from_array($array, $node_block='nodes xmlns:dc="http://purl.org/dc/elements/1.1/"', $node_name='node') {
	$xml = '<?xml version="1.0" encoding="UTF-8" ?>' . "\n";

	$xml .= '<' . $node_block . '>' . "\n";
	$xml .= generate_xml_from_array($array, $node_name);
	$xml .= '</' . $node_block . '>' . "\n";

	return $xml;
}
function ARRAYtoXML($array, $depth = 0){
    $indent = '';
    $return = '';
    for($i = 0; $i < $depth; $i++)
        $indent .= "\t";
    foreach($array as $key => $item){
        $return .= "{$indent}< {$key}>\n";
        if(is_array($item))
            $return .= ARRAYtoXML($item, $depth + 1);
        else
            $return .= "{$indent}\t< ![CDATA[{$item}]]>\n";
            $return .= "{$indent}\n";
        }
    return $return;
}

function slc_endpoint($args = null) {
define( 'CLIENT_ID',      variable_get('slc_client_id'));
define( 'CLIENT_SECRET' , variable_get('slc_client_secret'));

define( 'REDIRECT_URI'       , url('slc/endpoint', array('absolute' => TRUE)));
define( 'AUTHORIZATION_ENDPOINT' , 'https://api.sandbox.slcedu.org/api/oauth/authorize');
define( 'TOKEN_ENDPOINT'       , 'https://api.sandbox.slcedu.org/api/oauth/token');


// If the session verification code is not set, redirect to the SLC Sandbox authorization endpoint
if (!isset($_GET['code'])) {
  $url = 'https://api.sandbox.slcedu.org/api/oauth/authorize?client_id=' . CLIENT_ID . '&redirect_uri=' . REDIRECT_URI;
header('Location: ' . $url);
    die('Redirect'); 
} else {
  
$url = 'https://api.sandbox.slcedu.org/api/oauth/token?client_id=' . CLIENT_ID . '&client_secret=' . CLIENT_SECRET . '&grant_type=authorization_code&redirect_uri='.REDIRECT_URI.'&code='. $_GET['code'];

//open connection
$ch = curl_init();

//set the url, number of POST vars, POST data
curl_setopt($ch,CURLOPT_URL,$url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($ch, CURLOPT_HEADER, 'Content-Type: application/vnd.slc+json');
curl_setopt($ch, CURLOPT_HEADER, 'Accept: application/vnd.slc+json');

//execute post
$result = curl_exec($ch);

//close connection
curl_close($ch);

// de-serialize the result into an object
$result = json_decode($result);

// set the session with the access_token and verification code
$_SESSION['access_token'] = $result->access_token;
$_SESSION['code'] = $_GET['code'];

// redirect to the start page of the application
header('Location: ' . '/admin/config/system/slc');
}
}
function _slc_curl_connect($url) {
//open connection
$ch = curl_init();

//$url = 'https://api.sandbox.slcedu.org/api/rest/v1/students';
//$url = 'https://api.sandbox.slcedu.org/api/rest/system/session/check';

$token = $_SESSION['access_token'];
$code = $_SESSION['code'];

$auth = sprintf('Authorization: bearer %s', $token);
//echo $auth;

$headers = array(
  'Content-Type: application/vnd.slc+json',
  'Accept: application/vnd.slc+json',
  $auth);

curl_setopt($ch, CURLOPT_URL, $url); 
curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

//execute post
$result = curl_exec($ch);
$json = json_decode($result);
//dsm( $json);
// If response is '401 Unauthorized', redirect back to home page for authentication
if (isset($json->code) && $json->code == '401') {
  header('Location: /slc/endpoint');
  die();
}
//close connection
curl_close($ch);
return $json;
}

function _slc_form_submit_authenticate($form, &$form_state) {
  variable_set('slc_client_id', $form_state['input']['slc_client_id']);
  variable_set('slc_client_secret', $form_state['input']['slc_client_secret']);
  drupal_goto('slc/endpoint');
}

function _slc_form_submit_import($form, &$form_state) {
  variable_set('slc_client_id', $form_state['input']['slc_client_id']);
  variable_set('slc_client_secret', $form_state['input']['slc_client_secret']);
 
$students_json =  _slc_curl_connect('https://api.sandbox.slcedu.org/api/rest/v1/students');

  $batch = array(
    'title' => t('Importing'),
    'operations' => array(
      array('_slc_import_users', array($students_json)),
    ),
	'finished' => '_slc_import_finished',
	'file' => drupal_get_path('module', 'slc') . '/slc.batch.inc',
  );
  batch_set($batch);
  
}

