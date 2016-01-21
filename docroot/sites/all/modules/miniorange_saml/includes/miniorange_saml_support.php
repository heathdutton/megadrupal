<?php
/**
 * @file
 * Contains miniOrange Support class.
 */

/**
 * @file
 * This class represents support information for customer.
 */
class MiniorangeSamlSupport {

	public $email;

	public $phone;

	public $query;

	/**
     * Constructor.
     */
	public function __construct($email, $phone, $query) {
      $this->email = $email;
      $this->phone = $phone;
      $this->query = $query;
	}

	/**
	 * Send support query.
	 */
	public function sendSupportQuery() {
      $this->query = '[Drupal SAML Login] ' . $this->query;
      $fields = array (
        'company' => $_SERVER ['SERVER_NAME'],
        'email' => $this->email,
        'phone' => $this->phone,
        'query' => $this->query 
      );
      $field_string = json_encode ($fields);	
      $url = MiniorangeSAMLConstants::BASE_URL . '/moas/rest/customer/contact-us';	
      $ch = curl_init ( $url );
      curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
      curl_setopt($ch, CURLOPT_ENCODING, "");
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
      curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
      curl_setopt($ch, CURLOPT_MAXREDIRS, 10);
      curl_setopt($ch, CURLOPT_HTTPHEADER, array (
        'Content-Type: application/json',
        'charset: UTF-8',
        'Authorization: Basic' 
      ));
      curl_setopt($ch, CURLOPT_POST, TRUE);
      curl_setopt($ch, CURLOPT_POSTFIELDS, $field_string);
      $content = curl_exec($ch);	
	  if (curl_errno($ch)) {
        $error = array(
          '%method' => 'sendSupportQuery',
          '%file' => 'miniorange_saml_support.php',
          '%error' => curl_error($ch),
        );
        watchdog('miniorange_saml', 'cURL Error at %method of %file: %error', $error);
        return FALSE;
      }
      curl_close ($ch);
      return TRUE;
	}
}