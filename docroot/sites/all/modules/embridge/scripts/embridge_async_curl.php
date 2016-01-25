<?php
/**
 * @file
 * EMBridge Aysnc Curl.
 */

// Default path to login to server.
define('ENTERMEDIA_LOGIN_PATH_DEFAULT', '/media/services/rest/login.xml');

// Default file name to save cookie after login.
define('ENTERMEDIA_COOKIE_FILE_DEFAULT', realpath('.') . '/files/cookie.txt');

$url = $argv[1];
$ref_url = $argv[2];
$login = $argv[3];

$username = 'admin';
$password = 'admin';
$server_url = 'http://hostname';
$server_port = '8080';
$login_url = $server_url . ':' . $server_port . ENTERMEDIA_LOGIN_PATH_DEFAULT . '?accountname=' . $username . '&password=' . $password;

$ch = curl_init();
curl_setopt($ch, CURLOPT_COOKIEJAR, ENTERMEDIA_COOKIE_FILE_DEFAULT);
curl_setopt($ch, CURLOPT_COOKIEFILE, ENTERMEDIA_COOKIE_FILE_DEFAULT);
curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)");
curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 60);
curl_setopt($ch, CURLOPT_TIMEOUT, 120);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($ch, CURLOPT_REFERER, $ref_url);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
curl_setopt($ch, CURLOPT_URL, $url);
$response = curl_exec($ch);

$response = str_replace('&', '&amp;', $response);
$xml_object = simplexml_load_string($response);
$xmlarr = (array) $xml_object;
// Need to relogin to entermedia if REST API fails.
if (empty($xmlarr['@attributes']['stat']) || $xmlarr['@attributes']['stat'] == 'fail') {
  curl_setopt($ch, CURLOPT_URL, $login_url);
  curl_exec($ch);
  curl_setopt($ch, CURLOPT_URL, $url);
  $response = curl_exec($ch);
  $response = str_replace('&', '&amp;', $response);
}

curl_close($ch);
unset($ch);
