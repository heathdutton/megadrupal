<?php namespace Ooyala;

/*

Copyright 2011 © Ooyala, Inc.  All rights reserved.

Ooyala, Inc. (“Ooyala”) hereby grants permission, free of charge, to any person or entity obtaining a copy of the software code provided in source code format via this webpage and direct links contained within this webpage and any associated documentation (collectively, the "Software"), to use, copy, modify, merge, and/or publish the Software and, subject to pass-through of all terms and conditions hereof, permission to transfer, distribute and sublicense the Software; all of the foregoing subject to the following terms and conditions:

1.  The above copyright notice and this permission notice shall be included in all copies or portions of the Software.

2.   For purposes of clarity, the Software does not include any APIs, but instead consists of code that may be used in conjunction with APIs that may be provided by Ooyala pursuant to a separate written agreement subject to fees.  

3.   Ooyala may in its sole discretion maintain and/or update the Software.  However, the Software is provided without any promise or obligation of support, maintenance or update.  

4.  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE, AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, RELATING TO, ARISING FROM, IN CONNECTION WITH, OR INCIDENTAL TO THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

5.   TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, (i) IN NO EVENT SHALL OOYALA BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, SPECIAL, PUNITIVE, OR OTHER DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) RELATING TO, ARISING FROM, IN CONNECTION WITH, OR INCIDENTAL TO THE SOFTWARE OR THE USE OF OR INABILITY TO USE THE SOFTWARE, EVEN IF OOYALA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES, AND (ii) OOYALA’S TOTAL AGGREGATE LIABILITY RELATING TO, ARISING FROM, IN CONNECTION WITH, OR INCIDENTAL TO THE SOFTWARE SHALL BE LIMITED TO THE ACTUAL DIRECT DAMAGES INCURRED UP TO MAXIMUM AMOUNT OF FIFTY DOLLARS ($50).

*/

define("GET", "GET");
define("POST", "POST");
define("PUT", "PUT");
define("DELETE", "DELETE");
define("PATCH", "PATCH");

/**
 *PHP implementation of the Ooyala Api v2
 * http://api.ooyala.com/docs/v2/
 * @author german
 * (English)Installation
 *
 * To use PHP's cURL support you must also compile PHP --with-curl[=DIR] where DIR is the location of the directory containing the lib and include directories. In the include directory there should be a folder named curl which should contain the easy.h and curl.h files. There should be a file named libcurl.a located in the lib directory. Beginning with PHP 4.3.0 you can configure PHP to use cURL for URL streams --with-curlwrappers .
 *
 * Note: Note to Win32 Users
 * In order to enable this module on a Windows environment, libeay32.dll and ssleay32.dll must be present in your PATH. You don't need libcurl.dll from the cURL site.
 *
 * (Spanish)Instalación
 *
 * Para que PHP soporte cURL debe compilar PHP con el parámetro --with-curl[=DIR] DIR es la ruta donde están los directorios lib y include. En el directorio include debería haber el directorio curl y dentro los ficheros easy.h y curl.h. En el directorio lib debe contener el fichero libcurl.a Desde PHP 4.3.0 se puede configurar PHP usando cURL para streams --with-curlwrappers .
 *
 * Note: Nota para usuarios de Win32
 * Para poder activar este módulo en un entorno Windows, los ficheros libeay32.dll y ssleay32.dll deben estar presentes en su PATH. No se require libcurl.dll de la web de cURL.
 *
 *
 * more information: http://www.php.net/manual/en/curl.installation.php
 */
class OoyalaApiV2 {

  /**
   * Secret key used to generate the signatures
   * @var string
   */
  private $secret_key;
  /**
   * Api key
   * @var string
   */
  private $api_key;
  /**
   * Window time for conections
   * @var int
   */
  public $connection_time;
  /**
   * Round up time for requests
   * @var int
   */
  public $round_up_time;
  /**
   * Base URL
   * @var string
   */
  public $base_url;

  /**
   *
   * Ooyala Api constructors. It stores the secret key and the api key in order to make all requests.
   * @param string $secret_key your backlot secret key
   * @param string $api_key your backlot api key
   * @param int $connection_time window time for connections
   * @throws Exception if the constructor is called with null parameters
   */
  function __construct($secret_key, $api_key, $connection_time=15)
  {
    if($secret_key == null || $api_key == null) throw new Exception("Invalid call to constructor: Null parameters");
    $this->connection_time = $connection_time;
    $this->secret_key = $secret_key;
    $this->api_key = $api_key;
    $this->base_url = "https://api.ooyala.com";
    $this->round_up_time = 300;
  }

  private $errorStrings = array(
    400 => "400 (Bad request): A 400 response code indicates that a request was poorly formed or contained invalid data. For example, the API would return a 400 response code if your request contained malformed JSON or if you tried to submit a start_time of \"25:00:00\" for a publishing rule since the hour field of time must be between 0 and 23.",
    401 => "401 (Not authorized): A 401 response code indicates that a request was not properly signed, or that the API key supplied was invalid.",
    403 => "403 (Forbidden): A 403 response code indicates that you have submitted a request that is not properly authenticated for the requested operation.",
    404 => "404 (Not found): A 404 response code indicates that you have tried to modify a resource that does not exist. For example, you would receive a 404 response code if you tried to delete a player but you specified the wrong URL for the player."
  );

  public function get($path, $parameters)
  {
    return $this->request(GET, $path, "", $parameters);
  }
  public function post($path, $parameters, $body)
  {
    return $this->request(POST, $path, $body, $parameters);
  }
  public function put($path, $parameters, $body)
  {
    return $this->request(PUT, $path, $body, $parameters);
  }
  public function patch($path, $parameters, $body)
  {
    return $this->request(PATCH, $path, $body, $parameters);
  }
  public function delete($path, $parameters)
  {
    return $this->request(DELETE, $path, "", $parameters);
  }

  /**
   *
   * Makes a request to the given path
   * eg: to request an asset=> request(GET, "/v2/assets/$embed_code")
   *
   * calling this function is the same than calling:
   * get( generateURL(GET, ...)) in case of get. and the same for the other http methods.
   *
   *
   * @param string $HTTPMethod one of GET, POST, PATCH, PUT, DELETE
   * @param string $requestPath the path of the backlot resource
   * @param string $requestBody the body of the request
   * @param array $parameters aditional parameters
   * @throws Exception in case of a bad httpMethod or in case of error with the request
   */
  public function request($HTTPMethod, $requestPath, $requestBody = "", $parameters = array(), $content_type = "application/json")
  {
    $url = $this->generateURL( $HTTPMethod, $requestPath, $requestBody, $parameters);
    switch ($HTTPMethod) {
    case GET:
      return $this->do_get($url);
    case POST:
      return $this->do_post($url, $requestBody, $content_type);
    case PUT:
      return $this->do_put($url, $requestBody, $content_type);
    case DELETE:
      return $this->do_delete($url);
    case PATCH:
      return $this->do_patch($url, $requestBody);
    default:
      throw new Exception("Supported HTTP methods are: ".GET.", ".POST.", ".PUT.", ".DELETE.", ".PATCH);
    }

  }
  /**
   *
   * Generates the url for the request
   * @param string $HTTPMethod GET, POST, PUT, PATCH, DELETE
   * @param string $requestPath the path of the object to request
   * @param array $parameters array of paramaters [key => value]
   * @param string $requestBody body for the request
   * @return string theUrl generated
   */
  public function generateURL($HTTPMethod, $requestPath, $requestBody = "", $parameters=array())
  {

    $parameters["api_key"] = $this->api_key;
    $parameters["expires"] = $this->connection_time + time();
    if (!array_key_exists('expires', $parameters)) {
      $params['expires'] = floor(time()/$this->round_up_time)*$this->round_up_time + $this->connection_time;
    }
    $signature = $this->generateSignature($HTTPMethod, $requestPath, $parameters, $requestBody);
    $url = $this->base_url.$requestPath."?";
    foreach ($parameters as $key => $value) {
      $url .=  $key . "=" . urlencode($value) . "&";
    }
    $url .= "signature=" . $signature;
    return $url;
  }

  /**
   *
   * Generates the signature for a request
   * @param string $HTTPMethod GET, POST, PUT, PATCH, DELETE
   * @param string $requestPath the path of the object to request
   * @param array $parameters array of paramaters [key => value]
   * @param string $requestBody body for the request
   */
  private function generateSignature($HTTPMethod, $requestPath, $parameters, $requestBody = "")
  {
    $to_sign = $this->secret_key . $HTTPMethod . $requestPath;
    $keys = $this->sortKeys($parameters);
    foreach ($keys as $key) {
      $to_sign .= $key . "=" . $parameters[$key];
    }
    $to_sign .= $requestBody;
    $hash = hash("sha256", $to_sign,true);
    $base = base64_encode($hash);
    $base = substr($base,0,43);
    $base = urlencode($base);
    $base = rtrim($base, "=");
    return $base;

  }

  private function sortKeys($array)
  {
    $keys = array();$ind=0;
    foreach ($array as $key => $val) {
      $keys[$ind++]=$key;
    }
    sort($keys);
    return $keys;
  }


  /**
   *
   * Makes an http GET request to the url
   * returns the response or the error of the request
   * @param string $url
   * @throws Exception in case of conection error or error response from backlot
   */
  public function do_get($url)
  {
    $ch = curl_init($url);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    try {
      return $this->httpRequest($ch);
    } catch (OoyalaException $e) {
      throw $e;
    }
  }

  /**
   *
   * Makes an http PATCH request to the url
   * returns the response or the error of the request
   * @param string $url
   * @param string $body the body of the request
   */
  public function do_patch($url, $body = "")
  {
    $ch = curl_init($url);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt ($ch, CURLOPT_HTTPHEADER, Array("Content-Type: application/json"));
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_setopt($ch, CURLOPT_POSTFIELDS, $body);

    try {
      return $this->httpRequest($ch);
    } catch (Exception $e) {
      throw $e;
    }
  }

  /**
   *
   * Makes an http PUT request to the url
   * returns the response or the error of the request
   * @param string $url
   * @param string $body  the body of the request
   * @param string $content_type the content type of the body
   * @throws Exception in case of conection error or error response from backlot
   */
  public function do_put($url, $body = "", $content_type = "application/json")
  {
    $ch = curl_init($url);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt ($ch, CURLOPT_HTTPHEADER, Array("Content-Type: $content_type"));
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_setopt($ch, CURLOPT_POSTFIELDS, $body);

    try {
      return $this->httpRequest($ch);
    } catch (Exception $e) {
      throw $e;
    }
  }

  /**
   *
   * Makes an http POST request to the url
   * returns the response or the error of the request
   * @param string $url
   * @param string $body  the body of the request
   * @param string $content_type the content type of the body
   * @throws Exception in case of conection error or error response from backlot
   */
  public function do_post($url, $body = "", $content_type = "application/json")
  {
    $ch = curl_init($url);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt ($ch, CURLOPT_HTTPHEADER, Array("Content-Type: $content_type"));
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
    curl_setopt($ch, CURLOPT_POSTFIELDS, $body);

    try {
      return $this->httpRequest($ch);
    } catch (Exception $e) {
      throw $e;
    }
  }



  /**
   *
   * Makes an http DELETE request to the url
   * returns the response or the error of the request
   * @param string $url
   * @param string $body the body of the request
   * @throws Exception in case of conection error or error response from backlot
   */
  public function do_delete($url)
  {
    $ch = curl_init($url);
    curl_setopt ($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt ($ch, CURLOPT_HTTPHEADER, Array("Content-Type: application/json"));
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");

    try {
      return $this->httpRequest($ch);
    } catch (Exception $e) {
      throw $e;
    }
  }

  /**
   *
   * Makes the http with the curl parameter
   * you need to invoke curl_init before. this function closes the curl session
   * returns the response or the error of the request
   * @throws BacklotException in case of curl_error or response codes >=300
   * @param curl $ch the curl_init element
   */
  private function httpRequest($ch)
  {
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
    $response = curl_exec($ch);

    if(curl_error($ch)){
      curl_close($ch);
      return curl_error($ch);
    }

    $head=curl_getinfo($ch);
    $content = $head["content_type"];//application/json
    $code = $head["http_code"];//200
    curl_close($ch);
    if($code>=200 && $code <300)//server sometimes responds 204
    {
      return $response;
    }

    $error = "HTTP Error ($code), Response: $response.";
    if(array_key_exists($code, $this->errorStrings))
    {
      $error .= ' ' . $this->errorStrings[$code];
    }
    throw new OoyalaException($error, $code);

  }


}
class OoyalaException extends \Exception {}

?>
