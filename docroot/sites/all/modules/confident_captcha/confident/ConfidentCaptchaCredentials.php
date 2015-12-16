<?php
/**
 * Created by ConfidentTechnologies.
 * User: byao@confidenttech.com
 * Date: 8/26/12
 * Time: 3:59 PM
 * PHP ConfidentCaptcha Implementation
 */
/**
 * Confident CAPTCHA Credential Class
 * @package confidentcaptcha-php
 */
class ConfidentCaptchaCredentials
{
    private $customer_id;
    private $site_id;
    private $api_username;
    private $api_password;

    /**
     * Public Constructor
     *
     * @param $customer_id                  Customer ID
     * @param $site_id                      Site ID
     * @param $api_username                 API User Name
     * @param $api_password                 API Password
     * @return ConfidentCaptchaCredentials
     */
    public function __construct($api_username = null, $api_password = null, $customer_id = null, $site_id = null ) {
        $this->api_username = $api_username;
        $this->api_password = $api_password;
        $this->customer_id = $customer_id;
        $this->site_id = $site_id;
    }


    /**
     * This is a typical factory pattern, creating an instance from the <credentials/> tag
     * @static                              Static function to construct the class object
     * @param $settingsXml                  Configuration XML file containing credentials and properties
     * @return ConfidentCaptchaCredentials
     */
    public static function confidentCaptchaCredentialsFromXml($settingsXml)
    {
        $xmlDom = simplexml_load_file($settingsXml);

        // Just parse out credential part
        $credentialsNode = $xmlDom->{'credentials'};

        if (isset($credentialsNode))
        {
            $confidentCaptchaCredentials = new ConfidentCaptchaCredentials();

            $confidentCaptchaCredentials->customer_id = (string)$credentialsNode->customer_id;
            $confidentCaptchaCredentials->site_id = (string)$credentialsNode->site_id;
            $confidentCaptchaCredentials->api_username = (string)$credentialsNode->api_username;
            $confidentCaptchaCredentials->api_password = (string)$credentialsNode->api_password;

            return $confidentCaptchaCredentials;
        }
    }

    /**
     * Getter for Customer Id of Credential
     *
     * @return mixed
     */
    public function getCustomerId() {
        return $this->customer_id;
    }

    /**
     * Setter for Customer Id of Credential
     *
     * @param $customerId
     */
    public function setCustomerId($customerId) {
        $this->customer_id = $customerId;
    }

    /**
     * Getter for Site Id of Credential
     *
     * @return mixed
     */
    public function getSiteId() {
        return $this->site_id;
    }

    /**
     * Setter for Site Id of Credential
     *
     * @param $siteId
     */
    public function setSiteId($siteId) {
        $this->site_id = $siteId;
    }

    /**
     * Getter for API User Name of Credential
     *
     * @return mixed
     */
    public function getApiUsername() {
        return $this->api_username;
    }

    /**
     * Setter for API User Name of Credential
     *
     * @param $apiUsername
     */
    public function setApiUsername($apiUsername) {
        $this->api_username = $apiUsername;
    }

    /**
     * Getter for API Password of Credential
     *
     * @return mixed
     */
    public function getApiPassword() {
        return $this->api_password;
    }

    /**
     * Setter for API Password of Credential
     *
     * @param $apiPassword
     */
    public function setApiPassword($apiPassword) {
        $this->api_password = $apiPassword;
    }
}

