<?php

/**
 * @file
 * class that provides the connection to the central catalog of archibald
 */

/**
 * This is the parent class for all Publish-Classes.
 * Specially for
 *
 *  - ArchibaldLomPublish
 *  - CommentsOM_Pub_Publish
 *  - ArchibaldRatingsPublish
 *
 * @author cmueller <c.mueller@educa.ch>
 */
abstract class ArchibaldAbstractDsbApi {

  /**
   * array returned from converted to object
   * @see archibald_load_content_partner()
   * @var object
   */
  protected $contentPartner = NULL;

  /**
   * current processed object
   * @var mixed
   */
  protected $objectToPublish = NULL;

  /**
   * auth the client at the publich lom server
   *
   * @param $auth_token string
   *   called by reference
   *   current auth token
   *
   * @param $force boolean
   *   default FALSE
   *   TRUE: force refresh
   *
   * @return boolean
   *   return auth object on success, else FALSE
   */
  protected function lomApiAuth(&$auth_token = NULL, $force = FALSE) {

    if ($this->getContentPartnerCredential() === NULL) {
      return FALSE;
    }
    $auth_token = variable_get('archibald_auth_key_' . $this->getContentPartnerCredential()->username);
    $expire = variable_get('archibald_auth_expire' . $this->getContentPartnerCredential()->username);

    if ($force == TRUE) {
      $expire = 0;
    }

    watchdog('lom_auth', 'Authenticate with authtoken: @auth_token , expire=@expire',
      array(
        '@auth_token' => print_r($auth_token, 1),
        '@expire' => print_r($expire, 1)
      ),
      WATCHDOG_INFO
    );

    if (empty($auth_token) || time() >= $expire) {
      $client_private_key = $this->getClientPrivateKeyFile();
      $username           = $this->getContentPartnerCredential()->username;
      $vector           = $username . time();

      if (empty($client_private_key)) {
        return FALSE;
      }

      openssl_sign($vector, $signature, $client_private_key);
      openssl_free_key($client_private_key);

      $postarr = array(
        'user' => $username,
        'signature' => base64_encode($signature),
        'vector' => $vector,
      );

      list($result_raw, $info) = $this->openUrl(
        variable_get('archibald_api_url', 'https://dsb-api.educa.ch/v2') . '/auth',
        $postarr,
        'POST'
      );

      $auth = json_decode($result_raw);

      watchdog('lom_auth', 'Auth call result: @message',
        array(
          '@message' => $result_raw,
        ),
        WATCHDOG_INFO
      );

      if (empty($auth)) {
        watchdog('lom_auth', 'While executing auth command. The server throws "@message", please contact your software distributor.',
          array(
            '@message' => $result_raw,
          ),
          WATCHDOG_ERROR
        );

        if (function_exists('drush_log')) {
          drush_log('Auth: ' . $result_raw, 'error');
        }

        return FALSE;
      }

      if (empty($auth->token)) {
        watchdog('lom_auth', 'While executing auth command. The server throws "@message", please contact your software distributor.',
          array(
            '@message' => print_r($auth, TRUE),
          ),
          WATCHDOG_ERROR
        );

        if (function_exists('drush_log')) {
          drush_log('Auth: ' . print_r($auth, TRUE), 'error');
        }

        return FALSE;
      }

      $auth_token = $auth->token;

      variable_set('archibald_auth_key_' . $this->getContentPartnerCredential()->username, $auth->token);
      variable_set('archibald_auth_expire_' . $this->getContentPartnerCredential()->username, $auth->expire);

      if (function_exists('drush_log')) {
        drush_log('Auth: ' . $info->status . ' (' . $result_raw . ')', $info->status == 200 ? 'ok' : 'error');
      }
    }
    return !empty($auth);
  }

  /**
   * Make a request to the DSB API.
   *
   * @param string $url
   *   the url to open
   * @param array $params
   *   the request parameters
   * @param string $method
   *   the request method
   * @param array $headers
   *   the request headers
   * @param bool $ssl
   *   if TRUE, use ssl
   *
   * @return array
   *    the first key contains the raw result, the second key contains some
   *    request information (status, etc).
   */
  public function openUrl($url, $params = array(), $method = 'GET', $headers = array(), $ssl = FALSE) {
    $ch = curl_init();

    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, strtoupper($method));

    if (!empty($params)) {
      switch ($method) {
        default:
        case 'GET':
          $url .= '?' . http_build_query($params);
          break;

        case 'POST':
          curl_setopt($ch, CURLOPT_POSTFIELDS, $params);
          break;

        case 'PUT':
        case 'DELETE':
          watchdog('lom_publish', '@message',
            array(
              '@message' => http_build_query($params),
            ),
            WATCHDOG_DEBUG
          );
          curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($params));
          break;
      }
    }

    curl_setopt($ch, CURLOPT_URL, $url);

    if ($ssl == TRUE) {
      curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
      curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
    }

    if (!empty($headers)) {
      $real_headers = array();
      foreach ($headers as $key => $value) {
        $real_headers[] = "$key: $value";
      }

      curl_setopt($ch, CURLOPT_HTTPHEADER, $real_headers);
    }

    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_USERAGENT, 'Archibald/2.0 (@' . @$_SERVER['HTTP_HOST'] . ')');

    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    $result = curl_exec($ch);
    if (empty($result)) {
      $result = curl_exec($ch);
    }

    $info = (object) array(
      'status' => curl_getinfo($ch, CURLINFO_HTTP_CODE),
      'effective_url' => curl_getinfo($ch, CURLINFO_EFFECTIVE_URL),
    );

    curl_close($ch);

    return array($result, $info);
  }

  /**
   * get open ssl ressource for private key
   *
   * @return resource
   */
  protected function getClientPrivateKeyFile() {
    $content_partner = $this->getContentPartnerCredential();

    if (empty($content_partner)) {
      return FALSE;
    }

    $private_key_file = $content_partner->key_file;
    if (empty($private_key_file) || !isset($private_key_file->uri)) {
      return FALSE;
    }

    return openssl_pkey_get_private(file_get_contents($private_key_file->uri), $content_partner->password);
  }

  /**
   * check if current content partner can access to central catalog
   *
   * @return bollean
   */
  public function contentPartnerCanCentral() {
    return ($this->getClientPrivateKeyFile() === FALSE) ? FALSE : TRUE;
  }

  /**
   * get api login credentials
   *
   * @return object
   *   username
   *   password
   *   key_fid
   *   key_file
   *   icon_fid
   *   icon_file
   *   file_server_type
   */
  protected function getContentPartnerCredential() {


    if (!empty($this->objectToPublish->lom_stats->content_partner)) {
      // watchdog('lom_auth3', 'OBJECT TO PUBLISH EXISTS',
      //   array(
      //     '@message' =>  ''
      //   ),
      //   WATCHDOG_INFO
      // );

      return (object) $this->objectToPublish->lom_stats->content_partner;
    } else {
      // watchdog('lom_auth3', 'OBJECT TO PUBLISH EMPTY',
      //   array(
      //     '@message' =>  ''
      //   ),
      //   WATCHDOG_INFO
      // );
    }

    if (!empty($this->contentPartner)) {
      // watchdog('lom_auth3', 'CONTENT PARTNER EXISTS',
      //   array(
      //     '@message' =>  ''
      //   ),
      //   WATCHDOG_INFO
      // );
      return $this->contentPartner;
    } else {
      // watchdog('lom_auth3', 'NO CONTENT PARTNER',
      //   array(
      //     '@message' =>  ''
      //   ),
      //   WATCHDOG_INFO
      // );
    }

    return NULL;
  }

  /**
   * init content_partner by lom_id
   *
   * @param string $lom_id
   */
  public function loadContentPartnerByLomId($lom_id) {

    $stats = db_query('SELECT content_partner_id FROM {archibald_lom_stats} WHERE lom_id = :lom_id', array(':lom_id' => $lom_id));
    $stats_values = $stats->fetchAssoc();
    $this->contentPartner = (object) archibald_load_content_partner($stats_values['content_partner_id']);
  }

  /**
   * init content_partner by content_partner_id
   *
   * @param integer $content_partner_id
   */
  public function loadContentPartnerByContentPartnerId($content_partner_id) {
    $this->contentPartner = (object) archibald_load_content_partner($content_partner_id);
  }

  /**
   * Each sub class must implement this method.
   */
  public abstract function cron();
}

