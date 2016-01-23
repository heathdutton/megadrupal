<?php

$plugin = array(
    'name' => 'Varnish',
    'module' => 'steroids',
    'admin_settings_form' => 'steroids_varnish_admin_settings_form',
    'hook_steroids_purge' => 'steroids_varnish_purge',
);

/**
 * Implements the former hook_settings().
 */
function steroids_varnish_admin_settings_form() {
  $settings = variable_get('steroids_varnish', array());
  $varnish = new steroids_varnish();

  $form['steroids_varnish'] = array(
      '#type' => 'fieldset',
      '#title' => t('Varnish'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
  );

  $form['steroids_varnish']['servers'] = array(
      '#type' => 'textarea',
      '#title' => t('Backend servers'),
      '#default_value' => isset($settings['servers']) ? $settings['servers'] : '',
      '#size' => 60,
      '#required' => TRUE,
      '#description' => 'A list of Varnish backend servers, specified as ip:port',
  );

  $form['steroids_varnish']['status'] = array(
      '#markup' => $varnish->_get_status(),
  );

  $form['steroids_varnish']['timeout'] = array(
      '#type' => 'select',
      '#title' => t('Timeout'),
      '#suffix' => t('seconds'),
      '#default_value' => isset($settings['timeout']) ? $settings['timeout'] : '',
      '#required' => TRUE,
      '#options' => drupal_map_assoc(array(5, 10, 20, 30, 40, 50, 60, 80, 100, 200, 300, 400, 500)),
      '#description' => t('Timeout timespan when socket is not responding.'),
  );

  $form['steroids_varnish']['version'] = array(
      '#type' => 'select',
      '#title' => t('Varnish version'),
      '#default_value' => isset($settings['version']) ? $settings['version'] : '',
      '#description' => t('Select the servers varnish version.'),
      '#options' => array(
          '2' => '2.0.x',
          '2.1' => '2.1.x',
          '3' => '3.x',
      ),
  );

  return $form;
}

/**
 * Invalidate all given urls in the Varnish cache.
 *
 * @param type $urls
 *   Array of urls.
 */
function steroids_varnish_purge($urls, $wildcard = FALSE, $hostnames = array()) {
  $varnish = new steroids_varnish();
  $varnish->purge($urls, $wildcard, $hostnames);
  watchdog('steroids', format_plural(count($urls),
    'Varnish purged 1 url: @urls',
    'Varnish purged @count urls: @urls',
     array('@urls' => implode(';', $urls))));
}

class steroids_varnish {

  /**
   * Get servers
   */
  private function _get_servers() {
    static $servers;

    if (isset($servers)) {
      return $servers;
    }

    $servers = array();

    $server_settings = $this->_get_settings('servers');
    if (!empty($server_settings)) {
      $server_settings = preg_split("[\n]", $server_settings);

      foreach ((array) $server_settings as $server) {
        $split = preg_split('[:]', $server);
        $servers[] = array(
            'host' => $split[0],
            'port' => $split[1],
        );
      }
      return $servers;
    }

    return;
  }

  /**
   * Get status table
   */
  public function _get_status() {
    $status = $this->command('status');

    $header = array(
        t('Host'),
        t('Port'),
        t('Code'),
        t('Response'),
    );

    $rows = array();

    foreach ((array) $status as $state) {
      $rows[] = array(
          $state['host'],
          $state['port'],
          $state['code'],
          $state['response'],
      );
    }

    return theme('table', array('header' => $header, 'rows' => $rows, 'caption' => t('Server status'), 'empty' => t('No running servers found.')));
  }

  /**
   * Perform command
   */
  public function command($cmd, $ok = 200) {
    $servers = $this->_get_servers();
    $response = array();

    if (count($servers)) {
      foreach ($servers as $server) {
        $this->host = $server['host'];
        $this->port = $server['port'];

        if ($this->_connect()) {
          $this->_write($cmd);
          $this->_write("\n");

          try {
            $response[] = array(
                'response' => $this->_read($code),
                'code' => $code,
                'host' => $server['host'],
                'port' => $server['port'],
                'cmd' => $cmd,
            );
          }
          catch (Exception $e) {
            drupal_set_message('Read failed', 'error');
          }

          if ($code !== $ok) {
            $this->_set_error(t('Varnish command error.'));
          }

          fclose($this->socket);
        }
      }
    }
    return $response;
  }

  private function _connect() {

    if (!extension_loaded('sockets')) {
      return FALSE;
    }

    $timeout = $this->_get_settings('timeout');

    if (empty($this->host)) {
      return FALSE;
    }

    $this->socket = @fsockopen($this->host, $this->port, $errno, $errstr, $timeout);

    // Failed connection
    if (!is_resource($this->socket)) {
      $this->_set_error($errstr);
      return FALSE;
    }

    stream_set_blocking($this->socket, TRUE);
    stream_set_timeout($this->socket, $timeout);

    // @TODO varnish auth required connections

    try {
      $response = $this->_read($code);
    }
    catch (Exception $e) {
      drupal_set_message(t('Error connecting to varnish server on @host:@port', array('@host' => $this->host, '@port' => $this->port)), 'error');
    }

    if ($code !== 200) {
      $this->_set_error(t('Bad response from varnishadm'));
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Send a list of url's as a purge request to Varnish
   *
   * @param $urls
   *   An array of url's to be purged from the cache
   */
  public function purge($urls, $wildcard = FALSE, $hostnames = array()) {
    $settings = variable_get('steroids_varnish');

    // urls to purge
    $purge_list = '';
    if ($wildcard) {
      $purge_list = '^' . implode('|^', $urls);
    }
    else {
      $purge_list = '^' . implode('$|^',$urls) . '$';
    }

    // hostnames
    $hostnames_list = '';
    if (!empty($hostnames)) {
      $hostnames_list = '^' . implode('$|^', $hostnames) . '$';
    }

    // varnish 3.x
    if ($settings['version'] == '3') {
      if (!empty($hostnames_list)) {
        $this->command('ban req.http.host ~ "' . $hostnames_list . '" && req.url ~ "' . $purge_list . '"');
      }
      else {
        $this->command('ban req.url ~ "' . $purge_list . '"');
      }
    }
    // varnish 2.x
    else {
      if (!empty($hostnames_list)) {
        $this->command('purge req.http.host ~ "' . $hostnames_list . '" && req.url ~ "' . $purge_list . '"');
      }
      else {
        $this->command('purge req.url ~ "' . $purge_list . '"');
      }
    }
  }

  private function _set_error($errstr) {
    watchdog('steroids', 'Varnish: Error on socket @server:@port: @error', array(
        '@host' => $this->host,
        '@port' => $this->port,
        '@error' => $errstr), WATCHDOG_ERROR
    );
  }

  private function _read(&$code) {
    $code = 0;
    // get bytes until we have either a response code and message length or an end of file
    // code should be on first line, so we should get it in one chunk
    while (!feof($this->socket)) {
      $response = fgets($this->socket, 1024);
      if (!$response) {
        $meta = stream_get_meta_data($this->socket);

        if ($meta['timed_out']) {
          throw new Exception(sprintf('Timed out reading from socket %s:%s', $this->host, $this->port));
        }
      }
      if (preg_match('/^(\d{3}) (\d+)/', $response, $r)) {
        $code = (int) $r[1];
        $len = (int) $r[2];
        break;
      }
    }

    if (!isset($code)) {
      throw new Exception(t('Failed to get numeric code in response'));
    }

    $response = '';
    while (!feof($this->socket) && strlen($response) < $len) {
      $response .= fgets($this->socket, 1024);
    }
    return $response;
  }

  private function _write($data) {
    $bytes = fputs($this->socket, $data);
    if ($bytes !== strlen($data)) {
      $this->_set_error(t('Failed to write to varnishadm'));
    }
    return TRUE;
  }

  /**
   * Get settings
   */
  private function _get_settings($name = NULL) {
    static $settings;

    if (!isset($settings)) {
      $settings = variable_get('steroids_varnish', array());
    }

    if (!isset($name)) {
      return $settings;
    }

    if (isset($settings[$name])) {
      return $settings[$name];
    }

    return;
  }
}
