<?php

/**
 * DreamObjects makes much use of Amazon S3's API, so much of the code here is
 * inspired by the StorageS3 class from the Storage module.
 */
class StorageDreamObjects extends StorageContainer implements StorageContainerInterface {

  private function signature($source) {
    return base64_encode(hash_hmac('sha1', utf8_encode($source), $this->settings['secret_access_key'], TRUE));
  }

  private function parseHeaders($source) {
    $headers = array();

    $source = explode("\r\n", trim($source));
    array_shift($source);  // Get rid of the HTTP result line.

    foreach ($source as $header) {
      list($header, $value) = explode(':', $header . ':', 3);
      $headers[$header] = trim($value);
    }

    return $headers;
  }

  private function request($op, $bucket = NULL, $object_name = NULL, Storage $storage = NULL,  $uri = NULL, array $query = array()) {
    $result = array();
    $date = gmdate(DATE_RFC1123, time());

    $components = array($op, '', '', $date);

    $headers[] = 'Date: ' . $date;

    $curl_options = array(
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_AUTOREFERER => TRUE,
      CURLOPT_FOLLOWLOCATION => TRUE,
      CURLOPT_USERAGENT => 'Storage API (http://drupal.org/project/storage_api)',
    );

    switch ($op) {

      case 'PUT':
        $curl_options[CURLOPT_PUT] = TRUE;

        if ($storage) {

          if (!$this->access_control) {
            $headers[] = 'x-amz-acl: public-read';
            $components[] = 'x-amz-acl:public-read';
          }

          if (isset($storage->copy_source)) {
            $headers[] = 'x-amz-copy-source: ' . $storage->copy_source;
            $components[] = 'x-amz-copy-source:' . $storage->copy_source;
          }

        }

        if ($uri) {
          $fp = fopen($uri, 'r');

          $components[1] = base64_encode($storage->md5);
          $components[2] = $storage->mimetype;

          $headers = array_merge($headers, $storage->HTTPHeaders());

          $curl_options[CURLOPT_INFILE] = $fp;
          $curl_options[CURLOPT_INFILESIZE] = $storage->size;
        }
        else {
          $curl_options[CURLOPT_INFILESIZE] = 0;
          $headers[] = 'Expect:';
        }
        break;

      case 'HEAD':
        $curl_options[CURLOPT_NOBODY] = TRUE;
        $curl_options[CURLOPT_HEADER] = TRUE;
        break;

      case 'GET':
        if ($object_name) {
          $result['temp_uri'] = new StorageTempURI();
          $fp = fopen($result['temp_uri'], 'w');
          $curl_options[CURLOPT_FILE] = $fp;
        }
        break;

      case 'DELETE':
        $curl_options[CURLOPT_CUSTOMREQUEST] = 'DELETE';
        break;
    }

    if ($bucket) {
      $object_name = '/' . $object_name;
    }

    $components[] = '/' . $bucket . drupal_encode_path($object_name);

    $signature = $this->signature(implode("\n", $components));
    $headers[] = 'Authorization: AWS ' . $this->settings['access_key_id'] . ':' . $signature;

    $curl_options[CURLOPT_HTTPHEADER] = $headers;

    $url = $this->settings['url_style'] == 'domain'
      ? 'http://' . $bucket . '.objects.dreamhost.com'
      : 'http://' . 'objects.dreamhost.com/' . $bucket;
    $ch = curl_init(url($url . drupal_encode_path($object_name), 
      array('query' => $query)));
    curl_setopt_array($ch, $curl_options);
    $transfer = curl_exec($ch);
    $result += curl_getinfo($ch);
    curl_close($ch);

    if (isset($fp)) {
      fclose($fp);
    }

    if (isset($result['temp_uri']) && $result['http_code'] != 200) {
      unset($result['temp_uri']);
    }

    if (is_string($transfer) && $transfer != '') {
      $result['transfer'] = $transfer;

      if ($op == 'HEAD') {
        $result['headers'] = $this->parseHeaders($transfer);
      }
      else {
        $result['simplexml'] = @simplexml_load_string($transfer);

        if ($result['simplexml']) {
          if ($result['simplexml']->getName() == 'Error') {

            $msg = (string) $result['simplexml']->Code;

            watchdog('storage_dreamobjects', 'DreamObjects error: @msg', array('@msg' => $msg), WATCHDOG_ERROR);
            if (user_access('administer storage'))
              drupal_set_message(t('DreamObjects error: @msg', array('@msg' => $msg)), 'error');
          }
        }
      }
    }

    return $result;
  }

  public function serviceSettingsDefault() {
    return array(
      'access_key_id' => '',
      'bucket' => '',
      'url_lifetime' => 60 * 60,
    );
  }

  public function serviceSettingsForm($already_exists) {
    $items['credentials_info'] = array(
      '#markup' => t('Get your credentials !link.', array('!link' => l(t('here'), 
        'https://panel.dreamhost.com', 
        array('attributes' => array('target' => '_blank')))))
    );

    $items['access_key_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Access key ID'),
      '#required' => TRUE,
      '#validated' => TRUE,
      '#default_value' => isset($this->settings['access_key_id']) ? $this->settings['access_key_id'] : '',
    );

    $items['secret_access_key'] = array(
      '#type' => 'password',
      '#title' => t('Secret access key'),
      '#required' => TRUE,
      '#validated' => TRUE,
    );

    $items['bucket'] = array(
      '#type' => 'textfield',
      '#title' => t('Bucket name'),
      '#required' => TRUE,
      '#validated' => TRUE,
      '#default_value' => isset($this->settings['bucket']) ? $this->settings['bucket'] : '',
      '#maxlength' => 63,
    );

    $items['url_style'] = array(
      '#type' => 'radios',
      '#title' => t('Bucket name placement within URL'),
      '#default_value' => isset($this->settings['url_style']) ? $this->settings['url_style'] : 'path',
      '#options' => array(
        'domain' => t('Prepend to domain'),
        'path' => t('Prepend to path (recommended)'),
      ),
      '#description' => t('DreamHost gives you a choice of where the bucket name should appear in the URLs generated for your DreamObjects.  You are free to choose either format, however DreamHost recommends that you choose "Prepend to path" as it is less resource intensive and can result in faster serving of data.'),
    );

    if ($this->access_control) {
      $description = t('(seconds)<br />This container will produce cryptographically signed URLs that will cease to function once their lifetime is over.');

      $items['url_lifetime'] = array(
        '#type' => 'textfield',
        '#title' => t('URL lifetime'),
        '#required' => TRUE,
        '#validated' => TRUE,
        '#default_value' => isset($this->settings['url_lifetime']) ? $this->settings['url_lifetime'] : '3600',
        '#description' => $description,
        '#size' => 8,
        '#maxlength' => 8,
      );
    }

    return $items;
  }

  public function serviceSettingsValidate($already_exists) {

    if (!$this->settings['access_key_id']) {
      form_set_error('settings][access_key_id', t('Access Key ID field is required.'));
    }

    if (!$this->settings['secret_access_key']) {
      form_set_error('settings][secret_access_key', t('Secret Access Key field is required.'));
    }

    if (!$this->settings['bucket']) {
      form_set_error('settings][bucket', t('Bucket name field is required.'));
    }

    if (form_get_errors()) {
      return;
    }

    // Test account credentials.
    $result = $this->request('GET');   // HEAD doesn't work

    if ($result['http_code'] != 200) {
      form_set_error('settings][bucket');
      return;
    }

    // Check if bucket is already owned by this account.
    $this->result = $this->request('HEAD', $this->settings['bucket']);

    if (!$already_exists && $this->result['http_code'] == 200) {
      $result = $this->request('GET', $this->settings['bucket']);

      if (count($result['simplexml']->Contents) > 0) {
        form_set_error('settings][bucket', t("Bucket %name is already owned by this account, but is not empty.", 
          array('%name' => $this->settings['bucket'])));
      }
    }

    if ($already_exists && $this->result['http_code'] != 200) {
      form_set_error('settings][bucket', t("Bucket not owned by this account."));
    }
  }

  public function serviceCreate() {
    if ($this->result['http_code'] != 200) {
      $result = $this->request('PUT', $this->settings['bucket']);

      if ($result['http_code'] != 200) {
        throw new StorageException();
      }

      $message = 'Acquired DreamObject bucket <i>' . $this->settings['bucket'] . '</i>.';
      drupal_set_message($message);
      watchdog('storage_dreamobjects', $message, NULL, WATCHDOG_NOTICE);
    }
  }

  public function serviceInfo() {
    $styles = array(
      'domain' => t('Prepend to domain'),
      'path' => t('Prepend to path'),
    );

    $info = array(
      t('Bucket') => $this->settings['bucket'],
      t('Bucket placement within URL') => $styles[$this->settings['url_style']],
    );

    if ($this->access_control) {
      $info[t('URL lifetime')] = format_interval($this->settings['url_lifetime']);
    }

    return $info;
  }

  public function serviceDestroy() {
  }

  public function serviceInstanceCreate(Storage $storage, $uri) {
    $reference = $this->uniqueFilename($storage->filename);

    $result = $this->request('PUT', $this->settings['bucket'], $reference, $storage, $uri);

    if ($result['http_code'] == 200) {
      return $reference;
    }
    else {
      throw new StorageException();
    }
  }

  public function serviceInstanceCopy(Storage $storage, StorageContainerInterface $source_container) {
    $reference = $this->uniqueFilename($storage->filename);

    $storage->copy_source = '/' . $source_container->settings['bucket'] . '/' . $source_container->reference;
    $result = $this->request('PUT', $this->settings['bucket'], $reference, $storage);
    unset($storage->copy_source);

    if ($result['http_code'] == 200 && $result['simplexml']) {

      if ($result['simplexml']->getName() == 'CopyObjectResult')
        return $reference;
    }

    throw new StorageException();
  }

  private function eTag2md5($etag) {
    return pack('H*', substr($etag, 1, 32));
  }

  public function serviceManifest(array &$sandbox, &$finished) {

    $query = array(
      'max-keys' => 100,
    );

    if (isset($sandbox['marker'])) {
      $query['marker'] = $sandbox['marker'];
    }

    $result = $this->request('GET', $this->settings['bucket'], NULL, NULL, NULL, $query);

    foreach ($result['simplexml']->Contents as $object) {

      $manifest[(string) $object->Key] = array(
        'size' => (string) $object->Size,
        'md5' => $this->eTag2md5($object->ETag),
      );
    }

    $sandbox['marker'] = (string) $object->Key;
    $finished = $result['simplexml']->IsTruncated == 'false';

    return $manifest;
  }

  public function serviceInstanceExists($reference) {

    $result = $this->request('HEAD', $this->settings['bucket'], $reference);

    return $result['http_code'] == 200;
  }

  public function serviceInstanceCheck($reference) {

    $result = $this->request('HEAD', $this->settings['bucket'], $reference);

    if ($result['http_code'] != 200) {
      throw new StorageException();
    }

    return array(
      'size' => $result['headers']['Content-Length'],
      'md5' => $this->eTag2md5($result['headers']['ETag']),
    );
  }

  public function serviceInstanceGetUri($reference) {

    $result = $this->request('GET', $this->settings['bucket'], $reference);

    return $result['temp_uri'];
  }

  public function serviceInstanceDestroy($reference) {

    $result = $this->request('DELETE', $this->settings['bucket'], $reference);

    if ($result['http_code'] != 204) {
      throw new StorageException();
    }
  }
  
  public function serviceInstanceServe($reference, $https) {
    $dreamobjects_object = '/' . drupal_encode_path($reference);
    $base_url = 'http' . ($https ? 's' : '') . '://';
    $query = array();

    $base_url = $this->settings['url_style'] == 'domain'
      ? $base_url . $this->settings['bucket'] . '.objects.dreamhost.com'
      : $base_url . 'objects.dreamhost.com/' . $this->settings['bucket'];

    if ($this->access_control) {
      $query['AWSAccessKeyId'] = $this->settings['access_key_id'];
      $query['Expires'] = time() + $this->settings['url_lifetime'];

      $components = array(
        'GET',
        '',
        '',
        $query['Expires'],
        '/' . $this->settings['bucket'] . $dreamobjects_object,
      );

      $query['Signature'] = $this->signature(implode("\n", $components));
    }

    return url($base_url . $dreamobjects_object, array('query' => $query));
  }
}
