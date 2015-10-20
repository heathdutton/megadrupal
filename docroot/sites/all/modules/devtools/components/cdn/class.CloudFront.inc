<?php
namespace devtools\components\cdn;

class CloudFront {
  protected $_cf_key = NULL;
  protected $_cf_sec = NULL;
  protected $_cf_did = NULL;

  protected $_date = NULL;
  protected $_request = '';
  protected $_request_target = '';

  public function __construct($cf_key, $cf_sec, $cf_did) {
    $this->_cf_key = $cf_key;
    $this->_cf_sec = $cf_sec;
    $this->_cf_did = $cf_did;

    $this->_date = gmdate('D, d M Y G:i:s T');

    $this->_request_target = '/2012-07-01/distribution/' . $this->_cf_did . '/invalidation';

    $this->_request .= 'Host: cloudfront.amazonaws.com' . "\r\n";
    $this->_request .= 'Date: ' . $this->_date . "\r\n";

    $signature = $this->build_signature($this->_date, $this->_cf_sec);
    $this->_request .= 'Authorization: AWS ' . $this->_cf_key . ':' . $signature . "\r\n";
  }

  /**
   * Sends invalidation request for specified paths.
   * @param  [type] $paths [description]
   * @return [type]        [description]
   */
  public function invalidate($paths) {
    if (!empty($paths)) {
      $paths_xml = '';
      foreach ($paths as &$path) {
        // CloudFront requires leading forward slash
        $path = '/' . $path;
        $paths_xml .= '<Path>' . $path . '</Path>' . "\r\n";
      }

      $xml =  '
                <?xml version="1.0" encoding="UTF-8"?>
                <InvalidationBatch xmlns="http://cloudfront.amazonaws.com/doc/2012-07-01/">
                  <Paths>
                    <Quantity>' . sizeof($paths) . '</Quantity>
                    <Items>
                      ' . $paths_xml . '
                    </Items>
                  </Paths>
                  <CallerReference>' . $this->_cf_did . date('U') . '</CallerReference>
                </InvalidationBatch>
              ';

      $request = 'POST ' . $this->_request_target . ' HTTP/1.0' . "\r\n" . $this->_request;
      $request .= 'Content-Type: text/xml' . "\r\n";
      $request .= 'Content-Length: ' . strlen($xml) . "\r\n\r\n";
      $request .= $xml;

      // send request & parse HTTP code from response
      if (!$response = $this->send_request($request)) {
        return;
      }

      if (!$http_code = $this->response_get_code($response)) {
        return;
      }

      // AWS returns a 201 on 'success'
      if ($http_code == '201') {
        drupal_set_message(t('Invalidation request created on CloudFront. Allow 15-20 minutes for files to get updated.'));
      }
      else {
        drupal_set_message(t('Invalidation request NOT created on CloudFront. Received response: %code', array('%code' => $http_code)), 'error');
      }
    }
  }

  /**
   * Returns items in the current queue.
   * @return [type] [description]
   */
  public function get_queue() {
    $items = array();

    $request = 'GET ' . $this->_request_target . '?MaxItems=100 HTTP/1.1' . "\r\n" . $this->_request;
    $request .= 'Content-Type: text/plain' . "\r\n";
    $request .= 'Connection: Close' . "\r\n\r\n";

    // send request & parse HTTP code from response
    if (!$response = $this->send_request($request)) {
      return;
    }

    if (!$content = $this->response_get_content($response)) {
      return;
    }

    if ($xml = simplexml_load_string($content)) {
      $items = $xml->Items->children();
    }

    return $items;
  }

  /**
   * Builds an AWS authentication signature.
   * @param  [type] $string [description]
   * @param  [type] $secret [description]
   * @return [type]         [description]
   */
  protected function build_signature($string, $secret) {
    return base64_encode(hash_hmac('sha1', $string, $secret, TRUE));
  }

  /**
   * Sends a request and returns the response.
   * @param  [type] $request [description]
   * @return [type]          [description]
   */
  protected function send_request($request) {
    $socket = fsockopen('ssl://cloudfront.amazonaws.com', 443, $error_num, $error, 30);

    if (!$socket) {
      drupal_set_message(t('Could not open connection to AWS: %error_num - %error', array(
        '%error_num' => $error_num,
        '%error' => $error,
      )), 'error');
    }
    else {
      fwrite($socket, $request);
    
      $response = '';

      while (!feof($socket)) {
        $response .= fgets($socket, 1024);
      }
      
      fclose($socket);

      return $response;
    }

    return FALSE;
  }

  /**
   * Parses the response and returns the HTTP code.
   * @param  [type] $response [description]
   * @return [type]           [description]
   */
  protected function response_get_code($response) {
    foreach (explode("\n", $response) as $line) {
      if (preg_match('#HTTP/\d+\.\d+ (\d+) (.+)#', $line, $matches)) {
        return $matches[1];
      }
    }

    return FALSE;
  }

  /**
   * Returns the response's content.
   * @param  [type] $response [description]
   * @return [type]           [description]
   */
  protected function response_get_content($response) {
    $lines = explode("\n", $response);

    return array_pop($lines);
  }
}



