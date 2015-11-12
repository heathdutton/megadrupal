<?php
/**
 * Created by ConfidentTechnologies.
 * User: byao@confidenttech.com
 * Date: 8/26/12
 * Time: 8:24 PM
 * PHP ConfidentCaptcha Implementation
 */
require_once('ConfidentCaptchaCredentials.php');
require_once('ConfidentCaptchaProperties.php');
require_once('ConfidentCaptchaResponses.php');

/**
 * Confident CAPTCHA API Client
 * This is the entry point to call CAPTCHA API. The front end UI (JSP pages) should call
 * this Client to interact with CAPTCHA server.
 */
class ConfidentCaptchaClient
{
    /**
     * @var                             ConfidentCaptchaCredentials class instance
     */
    private $credentials;

    /**
     * @var                             ConfidentCaptchaProperties class instance
     */
    private $captchaProperties;

    /**
     * @var                             CAPTCHA Server URL
     */
    private $apiServerUrl;

    /**
     * @var                             CAPTCHA API Version
     */
    private $libraryVersion;

    /**
     * Public Constructor
     * usually you will want to set the SettingsXml, ApiServerUrl and LibraryVersion, however you can also set these
     * after the object is instanciated
     *
     * @param $settingsXmlFilePath              Configuration XML file containing credentials and properties
     * @param $apiServerUrl             CAPTCHA Server URL
     * @param $libraryVersion           Version of the captcha you want to request
     *
     * @return ConfidentCaptchaClient
     */
    public function __construct($apiServerUrl='https://captcha.confidenttechnologies.com',  $libraryVersion='20120625_PHP',  $settingsXmlFilePath=null)
    {

        if($settingsXmlFilePath != null){
            $this->credentials = ConfidentCaptchaCredentials::confidentCaptchaCredentialsFromXml($settingsXmlFilePath);
            $this->captchaProperties = new ConfidentCaptchaProperties($settingsXmlFilePath);
        }

        if (!empty($apiServerUrl))
        {
            $this->apiServerUrl = $apiServerUrl;
        }
        else{
            $serverUrl = $this->captchaProperties->getApiServerUrl();

            if (empty($serverUrl))
            {
                $this->apiServerUrl = $this->captchaProperties->getApiServerUrl();
            }
        }

        if(!empty($libraryVersion)){
            $this->libraryVersion = $libraryVersion;
        }
     }



    /**
     * Getter for Credentials
     *
     * @return ConfidentCaptchaCredentials
     */
    public function getCredentials()
    {
        return $this->credentials;
    }

    /**
     * Setter for Credentials
     *
     * @param $confidentCaptchaCredentials
     */
    public function setCredentials($confidentCaptchaCredentials)
    {
        $this->credentials = $confidentCaptchaCredentials;
    }

    /**
     * Getter for Properties
     *
     * @return ConfidentCaptchaProperties
     */
    public function getCaptchaProperties()
    {
        return $this->captchaProperties;
    }

    /**
     * Setter for Properties
     *
     * @param $confidentCaptchaProperties
     */
    public function setCaptchaProperties($confidentCaptchaProperties)
    {
        $this->captchaProperties = $confidentCaptchaProperties;
    }

    /**
     * Getter for CAPTCHA API Server URL
     *
     * @return
     */
    public function getApiServerUrl()
    {
        return $this->apiServerUrl;
    }

    /**
     * Setter for CAPTCHA API Server URL
     *
     * @param $apiServerUrl
     */
    public function setApiServerUrl($apiServerUrl)
    {
        $this->apiServerUrl = $apiServerUrl;
    }

    /**
     * Getter for CAPTCHA API library version
     *
     * @return
     */
    public function getLibraryVersion()
    {
        return $this->libraryVersion;
    }

    /**
     * Setter for CAPTCHA API library version
     *
     * @param $libraryVersion
     */
    public function setLibraryVersion($libraryVersion)
    {
        $this->libraryVersion = $libraryVersion;
    }

    /**
     * Check the environment to see if it meets with API requirements
     *
     * @return mixed
     */
    public function checkClientSetup()
    {
        // Local checks
        $localClientCheckpointsHolder = array(array("Item", "Value", "Required Value", "Acceptable?"));

        // Check PHP version 5.x
        $runningPhpVersion = phpversion();
        $minimumSupportedPhpVersion = "5.0.0";

        if (version_compare($runningPhpVersion, $minimumSupportedPhpVersion, '>=')) {
            $phpVersionSupported = 'Yes';
        }
        else {
            $phpVersionSupported = 'No';
        }

        $localClientCheckpointsHolder[] = array('PHP version', $runningPhpVersion, $minimumSupportedPhpVersion, $phpVersionSupported);

        // Check cURL extension
        if (extension_loaded('curl')) {
            $curlVersion = phpversion('cURL');
            if (empty($curlVersion)){
                $curlVersion = '(installed)';
            }
            $curlSupported = 'Yes';
        }
        else {
            $curlVersion = "(not installed)";
            $curlSupported = 'No';
        }

        $localClientCheckpointsHolder[] = array('cURL extension', $curlVersion, '(installed)', $curlSupported);

        // Check SimpleXML extension
        if (extension_loaded('SimpleXML')) {
            $simpleXmlVersion = phpversion('SimpleXML');
            if (empty($simpleXmlVersion)){
                $simpleXmlVersion = '(installed)';
            }
            $simpleXmlSupported = 'Yes';
        }
        else {
            $simpleXmlVersion = "(not installed)";
            $simpleXmlSupported = 'No';
        }
        $localClientCheckpointsHolder[] = array('SimpleXML extension', $simpleXmlVersion, '(installed)', $simpleXmlSupported);

        // Check CAPTCHA API server URL
        $notSet = '(NOT SET)';

        $apiServerUrl = $this->apiServerUrl;

        $expectedApiServerUrl = 'http://captcha.confidenttechnologies.com/';
        $sslExpectedApiServerUrl = 'https://captcha.confidenttechnologies.com/';
        $dispExpectedApiServerUrl  = 'http(s)://captcha.confidenttechnologies.com/';

        if ($apiServerUrl == $expectedApiServerUrl or $apiServerUrl == $sslExpectedApiServerUrl) {
            $ApiServerUrlSupported = 'Yes';
        }
        elseif (empty($apiServerUrl)) {
            $apiServerUrl = $notSet;
            $ApiServerUrlSupported = 'No';
        }
        elseif (0 == substr_compare($apiServerUrl, 'http', 0, 4)) {
            $ApiServerUrlSupported = 'Maybe';
        }
        else {
            $ApiServerUrlSupported = 'No';
        }

        $localClientCheckpointsHolder[] = array('ccap_server_url', $apiServerUrl, $dispExpectedApiServerUrl, $ApiServerUrlSupported);

        $credentials = $this->credentials;

        // Check API parameters
        $apiParameterSet = array('customer_id', 'site_id', 'api_username', 'api_password');

        foreach ($apiParameterSet as $apiParameterName) {
            switch ($apiParameterName)
            {
                case 'customer_id':
                    $parameterValue = $credentials->getCustomerId();
                    break;
                case 'site_id':
                    $parameterValue = $credentials->getSiteId();
                    break;
                case 'api_username':
                    $parameterValue = $credentials->getApiUsername();
                    break;
                case 'api_password':
                    $parameterValue = $credentials->getApiPassword();
                    break;
            }

            if (empty($parameterValue)) {
                $parameterValue = $notSet;
                $parameterValueOk = 'No';
            }
            else {
                $parameterValueOk = 'Yes';
            }

            $localClientCheckpointsHolder[] = array($apiParameterName, $parameterValue, '(some value)', $parameterValueOk);
        }

        $properties = $this->captchaProperties->getProperties();

        // Check CAPTCHA options
        $currentKnownProperties = array(
            'display_style'=> '(optional, default "lightbox")',
            'include_audio_form'=> '(optional, default FALSE)',
            'height'=> '(optional, default 3)',
            'width'=> '(optional, default 3)',
            'captcha_length'=> '(optional, default 4)',
            'image_code_color'=> '(optional, default "White")'
        );

        foreach ($currentKnownProperties as $propertyName => $expected) {
            if (isset($properties[$propertyName])) {
                $property = $properties[$propertyName];
            }
            else {
                $property = $this->$propertyName;
            }

            if (is_null($property)) {
                $property = $notSet;
                $required = ('(optional' != substr($expected, 0, 9));
                $propertyOk = ($required) ? 'No' : 'Yes';
            }
            else {
                // TODO: Should I validate the option?
                $propertyOk = 'Probably';
            }
            $localClientCheckpointsHolder[] = array($propertyName, $property, $expected, $propertyOk);
        }

        # Make local tables
        $localClientCheckpointsReturnHtml = "<h1>Local Configuration</h1>\n";
        $localClientCheckpointsReturnHtml.= "<table border=\"1\">\n<tr><th>";

        $headRow = array_shift($localClientCheckpointsHolder);
        $localClientCheckpointsReturnHtml.= implode('</th><th>', $headRow) . "</th></tr>\n";
        $localOk = TRUE;

        foreach($localClientCheckpointsHolder as $checkpoint) {
            $localClientCheckpointsReturnHtml .= '<tr><td>'.implode('</td><td>', $checkpoint)."</td></tr>\n";
            if (end($checkpoint) == 'No') $localOk = FALSE;
        }

        $localClientCheckpointsReturnHtml .= '</table><br/>';

        $localClientCheckpointsReturnHtml .= "\n<h1>Remote Configuration</h1>\n";

        // Check credentials with API server
        $checkCredentialsResponse = $this->checkCredentials();

        if ($checkCredentialsResponse->getStatus() == 200) {
            $checkCredentialsResponseHtml = $checkCredentialsResponse->getBody();
            $goodCredentials = (false === strstr($checkCredentialsResponseHtml, "api_failed='True'"));
        } else {
            $checkCredentialsResponseHtml  = "check_credentials call failed with status code: ";
            $checkCredentialsResponseHtml .= $checkCredentialsResponse->getStatus().'.';
            $checkCredentialsResponseHtml .= '<br />response body: <br />'.$checkCredentialsResponse->getBody();
            $goodCredentials = false;
        }

        $checkClientSetupResponse = new CheckClientSetupResponse($checkCredentialsResponse->getStatus(),
                $localClientCheckpointsReturnHtml . $checkCredentialsResponseHtml,
                $localOk and $goodCredentials);

        return $checkClientSetupResponse;
    }

    /**
     * Check if valid for the Credentials in settings.xml
     *
     * @return CheckCredentialsResponse
     */
    public function checkCredentials()
    {
        $endPointUrl = $this->apiServerUrl . '/check_credentials';
        $httpMethod = 'GET';

        $apiResponse = $this->makeRequest($endPointUrl, $httpMethod);

        $checkCredentialsResponse = new CheckCredentialsResponse($apiResponse->getStatus(),$apiResponse->getBody(), $apiResponse->getMethod(), $apiResponse->getUrl(), $apiResponse->getForm());
        return  $checkCredentialsResponse;
    }

    /**
     * Crate a visual CAPTCHA
     * The number of images is defined in settings.xml by width times height
     * The length of CAPTCHA (or solution) is also defined in settings.xml (captcha_length)
     *
     * @return CreateCaptchaResponse
     */
    public function requestCaptcha()
    {
        $endPointUrl = $this->apiServerUrl . '/captcha';
        $httpMethod = 'POST';

        $apiResponse = $this->makeRequest($endPointUrl,  $httpMethod);

        if ($apiResponse->getStatus() == 200) {
            $apiResponse->setBody(str_replace('ajax_verify: true,', 'ajax_verify: false,', $apiResponse->getBody()));

        }
        else
        {
            $apiResponse->setBody( $this->createArithmeticCaptcha());
        }


        return $apiResponse;
    }

    /**
     * Create a visual CAPTCHA and parse out and return
     * only the HTML Body of the returned response object
     *
     * @return String of HTML Body to represent a CAPTCHA
     */
    public function createCaptcha()
    {
        $requestCaptchaResponse = $this->requestCaptcha();

        if ($requestCaptchaResponse != null)
        {
            return $requestCaptchaResponse->getBody();
        }
    }

    /**
     * Validate a created CAPTCHA
     * You may parse out the CAPTCHA ID, CODE and Click Coordinates
     * from REQUEST object instead of passing them in.
     * This function is designed in a way that it can be used to
     * validate CAPTCHA when you have the CAPTCHA ID, CODE and
     * Click Coordinates in some other routes.
     *
     * @param $request                      HTTP Request
     * @return CheckCaptchaResponse
     */
    public function checkCaptcha($request, $captchaId=null, $code=null, $clickCoordinates=null )
    {       
        if(is_null($captchaId)){
            if(empty($request['confidentcaptcha_captcha_id']) == false){
                $captchaId=$request['confidentcaptcha_captcha_id'];
            }
        }

        if(is_null($code)){
           if(empty($request['confidentcaptcha_code']) == false){
              $code=$request['confidentcaptcha_code'];
           }
        }

        if(is_null($clickCoordinates)){
           if(empty($request['confidentcaptcha_click_coordinates']) == false){
              $clickCoordinates=$request['confidentcaptcha_click_coordinates'];
           }
        }
        if (empty($request['confidentcaptcha_captcha_id']) && empty($request['confidentcaptcha_code']))
        {
            // Don't bother to validate against server
            // Use arithmetic captcha instead
            $status = 404;
            $body = $this->validateArithmeticCaptcha($request);
            if ($body === True)
            {
                $status = 200;
				$body = "true";
            }

            return new CheckCaptchaResponse($status, $body);
        }

        $endPointUrl = $this->apiServerUrl . "/captcha/" . $captchaId;

        $httpParameters = $this->captchaProperties->getProperties();
        $httpParameters['captcha_id']           = $captchaId;
        $httpParameters['code']                 = $code;
        $httpParameters['click_coordinates']    = $clickCoordinates;

        $httpMethod = 'POST';

        $apiResponse = $this->makeRequest($endPointUrl, $httpMethod, $httpParameters);

        return new CheckCaptchaResponse($apiResponse->getStatus(),$apiResponse->getBody(), $apiResponse->getMethod(), $apiResponse->getUrl(), $apiResponse->getForm());
    }

    /**
     * An Arithmetic CAPTCHA is created when creating a CAPTCHA and CAPTCHA Server is not reachable,
     * or when validating a CAPTCHA and CAPTCHA Server is not reachable.
     *
     * @return string
     */
    private function createArithmeticCaptcha()
    {
        $captcha = <<<CAPTCHA
            <script type="text/javascript">
                var a = Math.ceil(Math.random() * 10);
                var b = Math.ceil(Math.random() * 10);
                function createArithmeticCaptcha(){
                    document.write("What is "+ a + " + " + b +"? ");
                    document.write("<input name='arithmeticCaptchaUserInput' id='arithmeticCaptchaUserInput' type='text' maxlength='2' size='2'/>");
                    document.write("<input name='arithmeticCaptchaNumberA' id='arithmeticCaptchaNumberA' type='hidden' value='var a'/>");
                    document.write("<input name='arithmeticCaptchaNumberB' id='arithmeticCaptchaNumberB' type='hidden' value='var b'/>");
                    document.getElementById('arithmeticCaptchaNumberA').value = a;
                    document.getElementById('arithmeticCaptchaNumberB').value = b;
                }
                createArithmeticCaptcha();
            </script>
CAPTCHA;

        return $captcha;
    }

    /**
     * Validating the user's response to the Arithmetic CAPTCHA challenge
     *
     * @param $request                      HTTP Request
     * @return bool
     */
    private function validateArithmeticCaptcha($request)
    {

        // init our return value
        $matchCaptchaRequestPassed = false;

        if (! is_null($request) && ! empty($request))
        {
            $numberA = $request['arithmeticCaptchaNumberA'];
            $numberB = $request['arithmeticCaptchaNumberB'];
            $userGivenAnswer = $request['arithmeticCaptchaUserInput'];
        }

        if (isset($numberA) && isset($numberB) && isset($userGivenAnswer))
        {
            // see if they got the answer correct
            if (intval($numberA) + intval($numberB) == intval($userGivenAnswer))
            {
                $matchCaptchaRequestPassed = true;
            }
        }

        return $matchCaptchaRequestPassed;
    }

    /**
     * Interface to call CAPTCHA API
     *
     * @param $endPointUrl
     * @param $httpParameters
     * @param $httpMethod
     * @return ConfidentCaptchaResponse
     */
    public function makeRequest($endPointUrl, $httpMethod, $httpParameters=null )
    {
        $serverRequestUrl = $endPointUrl;

        $mandatoryRequestParameters = $this->buildMandatoryRequestParameters($httpParameters);
        $configurableParameters = $this->captchaProperties->getProperties();
        $requestParameters =   $mandatoryRequestParameters + $configurableParameters;

        if($httpParameters != null){
            $requestParameters = $requestParameters + $httpParameters;
        }

        $form = NULL;
        if (strtoupper($httpMethod) == 'GET') {
            $serverRequestUrl .= '?' . http_build_query($requestParameters, '', '&');
        } elseif (strtoupper($httpMethod) == 'POST' and $requestParameters) {
            $form = http_build_query($requestParameters, '', '&');
        }

        $curlHandle = curl_init();

        if (strtoupper($httpMethod) == 'POST') {
            curl_setopt($curlHandle, CURLOPT_POST, TRUE);

            if ($form) {
                curl_setopt($curlHandle, CURLOPT_POSTFIELDS, $form);
            }
        }

        # Set SSL verification options if we are requesting a ssl endpoint
        $sslProtocolUrl = "https://";
        $secureServerRequestUrl = substr($serverRequestUrl, 0, strlen($sslProtocolUrl));
        if (strcmp($secureServerRequestUrl, $sslProtocolUrl) == 0) {
                curl_setopt($curlHandle, CURLOPT_SSL_VERIFYPEER, FALSE);
                curl_setopt($curlHandle, CURLOPT_SSL_VERIFYHOST, 2);
        }

        curl_setopt($curlHandle, CURLOPT_URL, $serverRequestUrl);

        curl_setopt($curlHandle, CURLOPT_RETURNTRANSFER, TRUE);
        $body = curl_exec($curlHandle);

        $httpResponseCode = curl_getinfo($curlHandle, CURLINFO_HTTP_CODE);
        if ($body === FALSE || strtolower($body) === "false") {
            $response = new ConfidentCaptchaResponse($httpResponseCode, curl_error($curlHandle), strtoupper($httpMethod), $serverRequestUrl, $form);
        }
        else {
            $response = new ConfidentCaptchaResponse($httpResponseCode, $body, strtoupper($httpMethod), $serverRequestUrl, $form);
        }

        curl_close($curlHandle);

        return $response;
    }

    /**
     * Helper function to prepare the request parameters
     * It will add some dynamic properties such as IP Address and User Agent
     * Each of these are mandatory and pulled out of the environment/generated
     *
     * @param $httpParameters
     * @return array
     */
    private function buildMandatoryRequestParameters()
    {
        $mandatoryParameters['customer_id']  = $this->credentials->getCustomerId();
        $mandatoryParameters['site_id']      = $this->credentials->getSiteId();
        $mandatoryParameters['api_username'] = $this->credentials->getApiUsername();
        $mandatoryParameters['api_password'] = $this->credentials->getApiPassword();
        $mandatoryParameters['ip_addr']      = $this->getRealIpAddr();
        $mandatoryParameters['user_agent']   = $_SERVER['HTTP_USER_AGENT'];
        if (isset($_SERVER['HTTP_ACCEPT_LANGUAGE'])) {
            $mandatoryParameters['language'] = $_SERVER['HTTP_ACCEPT_LANGUAGE'];
        } else {
            $mandatoryParameters['language'] = "en";
        }
        $mandatoryParameters['library_version'] = $this->getLibraryVersion();        
        $mandatoryParameters['local_server_name'] = $this->getLocalServerName();
        $mandatoryParameters['local_server_address'] = $_SERVER['REMOTE_ADDR'];

        return $mandatoryParameters;
    }

    /**
     * Helper function to get client IP Address
     *
     * @return mixed
     */
    private function getRealIpAddr()
    {
        if (!empty($_SERVER['HTTP_CLIENT_IP']))             // to check ip from share internet
        {
            $ip = $_SERVER['HTTP_CLIENT_IP'];
        }
        elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR']))   // to check ip is pass from proxy
        {
            $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
        }
        else
        {
            $ip = $_SERVER['REMOTE_ADDR'];
        }

        return $ip;
    }

    /**
     * Helper function to get local server name
     *
     * @return string
     */
    private function getLocalServerName()
    {
        $protocol = 'http';

        if (isset($_SERVER['SERVER_PORT']) && $_SERVER['SERVER_PORT'] == '443')
        {
            $protocol = 'https';
        }

        $host = $_SERVER['HTTP_HOST'];
        $baseUrl = $protocol . '://' . $host;

        if (substr($baseUrl, -1)=='/') {
            $baseUrl = substr($baseUrl, 0, strlen($baseUrl)-1);
        }

        return $baseUrl;
    }
}