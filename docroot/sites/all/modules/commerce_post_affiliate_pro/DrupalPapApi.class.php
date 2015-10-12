<?php
/*
 * @file DrupalPapApi.class.php
 * Provides wrapper/helper classes for PapApi classes
 * @author Peter Pokrivcak
 * @author Lee Rowlands leerowlands at rowlandsgroup dot com
 * @license GPL v3 http://www.fsf.org/licensing/licenses/gpl.html
 * 
 */

module_load_include('php', 'commerce_post_affiliate_pro', 'PapApi.class');
class Drupal_Gpf_Api_IncompatibleVersionException extends Exception {
  
	protected $api_ink;
	public $message;	  
	public $code;

	public function __construct($url) {
		$this->api_link = $url. '?C=Gpf_Api_DownloadAPI&M=download&FormRequest=Y&FormResponse=Y';
		parent::__construct('Version of API not corresponds to the Application version. Please <a href="' . $this->api_link . '">download latest version of API</a>.', 0);
	}
	
	public function getApiDownloadLink() {
		return $this->apiLink;
	}

}

if (!class_exists('Drupal_Gpf_Api_Session', false)) { 

 class Drupal_Gpf_Api_Session extends Gpf_Api_Session { 
	/**
	* @param $latestVersion
	* @throws Drupal_Gpf_Api_IncompatibleVersionException
	*/
	public function checkApiVersion(Gpf_Rpc_Form $form) {
		if ($form->getFieldValue('correspondsApi') === Gpf::NO) {
		
			$this->url = variable_get('commerce_post_affiliate_pro_merchant_url_prefix','http://demo.qualityunit.com/pax4/') . 'scripts/server.php';
			//throw new Drupal_Gpf_Api_IncompatibleVersionException($this->url);
			$errors = new Drupal_Gpf_Api_IncompatibleVersionException($this->url);
			if($errors->code > 0 ) {
				drupal_set_message(t("API call error: !error", array('!error' => $errors->message)), 'error');
			}
		}
	}
 } 
}