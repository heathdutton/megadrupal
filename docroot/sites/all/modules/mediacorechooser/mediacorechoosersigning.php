<?php
require_once('vendor/autoload.php');

/**
 * Return a URL safe base64 encoded string (RFC 3458 Section 4) with all
 * padding = characters stripped off to avoid potential problems with
 * non-standard URL parsers that will choke on the = characters.
 */
function mcore_urlsafe_b64encode($value) {
    $encoded_value = base64_encode($value);
	$encoded_value = strtr($encoded_value, '+/', '-_');
	$encoded_value = trim($encoded_value, '=');
	return $encoded_value;
}

/**
 * Generate a signature for the given key and value.
 *
 * Assumes the given secret key is a Unicode string containing a bse64 encoded
 * version of the key.
 */
function mcore_get_signature($secret_key, $value) {
	$secret_key = base64_decode($secret_key);

	if (strlen($secret_key) == 128) {
		$mac = hash_hmac("sha512", $value, $secret_key, true);
	} else {
		$mac = hash_hmac("sha256", $value, $secret_key, true);
	}

	return $mac;
}

/**
 * Create a signature for the given MediaCore URL.
 *
 * If both ``ttl`` and ``expiry_epoch`` are specified, ``expiry_epoch`` will
 * be used. If neither is specified, the generated URL will be valid forever.
 *
 * Example usage:
 *
 *     $secret_key = 'secret==';
 *     $key_id = 'keyid';
 *     $url = 'http://localhost:8080/media/pandamp4/embed_player';
 *     $qs = 'iframe=True';
 *     $one_hour = 60 * 60;
 *     $localhost_only = '127.0.0.1/32';
 *     $signed_qs = get_signed_qs($url, $qs, $key_id, $secret_key, $one_hour, null, $localhost_only);
 *     $signed_url = $url . '?' . $signed_qs;
 *     echo '<a href="' . $signed_url . '">This link contains the signed version of ' . $url . '</a>';
 *
 * @param string $url A fully qualified MediaCore URL. Must not include query
 *     parameters.
 *
 * @param string $key_id The AWS ID for the RSA key pair.
 *
 * @param string $secret_key The secret key string (a base64 encoded 1024-bit
 *     key).
 *
 * @param string $query_string '*' for wildcard, '' for empty, or 'a=b&c=d' to
 *     require a specific QS.
 *
 * @param int|null $ttl The number of seconds from now that the URL will be valid.
 *
 * @param int|null $expiry_epoch The exact UTC unix epoch that the URL will expire.
 *
 * @param string|null $ip_mask An IP address mask restrict this URL to. e.g.:
 *     '192.168.0.0/24' would limit the IP to any IPs beginning with
 *     '192.168.0.'.
 *
 * @return string A query string to be appended to the URL. Does not include a
 *     leading & or ? character.
 */
function mcore_get_signed_qs($url, $query_string, $key_id, $secret_key, $ttl=null, $expiry_epoch=null, $ip_mask=null) {
	$uri = new \MediaCore\Uri($url);

	if (!isset($expiry_epoch) && isset($ttl)) {
		$expiry_epoch = time() + $ttl;
	}

	$policy = array();
	$policy['resource'] = $uri->toString();
	if (!empty($query_string)) {
		$policy['query_string'] = $query_string;
	}
	if (isset($ip_mask)) {
		$policy['ip_range'] = $ip_mask;
	}
	if (isset($expiry_epoch)) {
		$policy['expiry_epoch'] = $expiry_epoch;
	}

	$policy_string = json_encode($policy);
	$encoded_policy_string = mcore_urlsafe_b64encode($policy_string);

	$signature = mcore_get_signature($secret_key, $encoded_policy_string);
	$encoded_signature = mcore_urlsafe_b64encode($signature);

	$policy_qs = '_Policy=%s&_Signature=%s&_KeyId=%s';
	$new_qs = sprintf($policy_qs, $encoded_policy_string, $encoded_signature, $key_id);

	if (!in_array($query_string, array('', '*'))) {
		$new_qs = $query_string . '&' . $new_qs;
	}
	return $new_qs;
}

?>
