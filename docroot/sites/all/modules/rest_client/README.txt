INTRODUCTION
============
REST Client is a robust HTTP request module to consume REST style services.

Why use REST Client instead of drupal_http_request():
1. Streams large files
2. Uses Expect 100 header before sending data in case of redirection
3. Fully customizable HTTP request
4. REST utility functions: HMAC, binary SHA1, binary MD5


TARGET
======
End Users:
This is for module developers only. Do NOT install unless required by another 
module.

Module Developers:
Please use drupal_http_request() included in Drupal core. If drupal_http_request() 
does not fit your needs this is your module. 

Please post an issue for any REST service this does not work for along with a 
link to the service's REST API in this projects issue queue.


REQUEST API
===========
rest_client_request
This is the only function you call to send a HTTP request. All other functions 
except utility functions are private funcitons used by rest_client_request

PARAMS
$request = array()
The request parameter is an associative array holding parts of the HTTP request 
that make up the first line of the request.
Ex. $request = array( // defaults
    'method'      => 'GET',  // ['PUT', 'GET', 'DELETE', 'HEAD'] or any other method
    'resource'    => '/',    // the path of the request not including the host
    'port'        => '80',   // port of the request
    'httpversion' => '1.1',  // HTTP version '1.0' or '1.1'
    'scheme'      => 'http'  // scheme of request ['http', 'https'] or other scheme
  );

$headers = array()
This is an associative array of request headers. The key is the header name and 
the values is the header value. Any headers can be used here. Date, Content-Length, 
and Content-Md5 are determined for you at the time of the request.
Ex. $headers = array(
    'host' => 'example.com',
    'content-type' => 'text/plain'
  );

$data = null
This holds the request body information. If there is no body to be sent then set 
$data to null. If a body is to be sent then set $data to an assosative array.
Ex. $data = array(
    'type'  => 'file',             // ['file', 'string']
    'value' => '/path/to/file.mpg' // if type = file then value is the path to the file
  );                                 // if type = string then value contains the string

$retry = 3
This is the number of times to retry the request if redirected.

RETURN
rest_client_request returns a std class object holding the parsed response headers 
and the raw response body.
Ex. $response->headers   // an associative array containing all the response headers
  $response->code      // contains the HTTP response code
  $response->text      // contains the HTTP response text following the code
  $response->codeText  // contains the standard HTTP response text that the code matches
  $response->data      // contains the raw response body  
  $response->errorCode // contains the error code if the socket could not be opened
  $response->errorText // contains the error text if the socket could not be opened
  
You should always check if $resopnse->errorCode has been set before attempting 
to process the response.


UTILITY FUNCTIONS API
=====================
rest_client_hmac
This function takes a string and encryptes it with a HMAC SHA1 encryption. A key 
must be given encrypt with. This is usually the secret key in many REST services.

rest_client_binarySha1
This function takes a string and encrypts it with sha1 returns the result in 
binary form. This function is PHP4 & 5 safe. Many times you will need to wrap 
this with base64_encode().

rest_client_binaryMd5
This function takes a data array as sent to the request function and returns the 
$data->value in binary form. This function is PHP4 & 5 safe. Many times you will 
need to wrap this with base64_encode().
