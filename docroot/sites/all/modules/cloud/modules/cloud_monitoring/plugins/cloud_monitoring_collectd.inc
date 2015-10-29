<?php

/**
 * @file
 * Concrete Collectd Implementation.  This implementation
 * talks with a Drupal based RRD Rest Module.  In order
 * for this class to function properly, the monitoring server
 * at the $endpoint, must have Drupal 7 and the RRD Rest Module
 * enabled.
 */

class collectd_monitor extends monitor_system {

  protected function get_host() {

  }

  public function get_ip() {
    $ip = '';
    $result = $this->execute(array('q' => 'monitor-server/status'));
    if (count($result) && isset($result['server_ip'])) {
      $ip = $result['server_ip'];
    }
    return $ip;
  }

  public function is_active() {
    $active = TRUE;
    //ping the server to make sure it is up and running
    $result = $this->execute(array('q' => 'monitor-server/status'));
    if (count($result) == 0) {
      $active = FALSE;
    }
    elseif ($result['rrd_data_dir'] == 'Not Available' || $result['rrd_tool'] == 'Not Available') {
      $active = FALSE;
    }
    return $active;
  }

  protected function set_endpoint() {
    //locate the endpoint using the monitor_id
    if ($this->monitor_id) {
      $monitor = cloud_monitoring_load_monitor($this->monitor_id);
      if ($monitor) {
        $monitor_details = _aws_cloud_get_instance_details_by_id($monitor['cloud_type'], $monitor['instance_id']);
        $endpoint = isset($monitor_details['hostname']) ? $monitor_details['hostname'] : '';
        //if the endpoint doesn't have a transport, use a http default
        if (!preg_match("/^(http|https)/i", $endpoint)) {
          $endpoint = 'http://' . $endpoint;
        }
        $this->endpoint = $endpoint;
      }
    }
  }

  /**
   * Method generates a small graph url
   */
  public function get_small_graph($host, $seconds, array $params) {
    $url = '';
    if (count($params)) {
      $params['q'] = 'monitor-server/get-graph-by-plugin';
      $params['host'] = $host;
      $params['seconds'] = $seconds;
      $params['image_preset'] = 'medium';
      $url = $this->build_url($params);
    }
    $instance = cloud_monitoring_instance_by_dns($host);
    $detail_path = '';
    if ($instance) {
      $details_url = "clouds/{$instance['cloud_type']}/instances/describe/{$instance['instance_id']}/graph-details";
      //copy the array - and pass it to the details page
      $details_params = $params;
      unset($details_params['q']);
      $details_params['image_preset'] = 'large';
    }
    return l(t("<img src={$url} class='graph-image'>"), $details_url, array('html' => TRUE, 'query' => $details_params));
  }

  public function get_large_graph($host, $seconds, array $params) {
    $url = '';
    if (count($params)) {
      $params['q'] = 'monitor-server/get-graph-by-plugin';
      $params['host'] = $host;
      $params['seconds'] = $seconds;
      $params['image_preset'] = 'large';
      $url = $this->build_url($params);
    }
    return "<img src={$url} class='graph-image'>";
  }

  /**
   * Method gets plugins from collectd
   */
  public function get_plugin_details($host, $plugin = NULL, $details = 'true') {

    if (!isset($host)) {
      return FALSE;
    }
    $params = array(
      'host' => $host,
      'q' => 'monitor-server/get-plugins-by-host',
    );
    if (isset($plugin)) {
      $params['plugin'] = $plugin;
    }
    if (isset($details)) {
      $params['details'] = $details;
    }
    $data = $this->execute($params);

    return $data;
  }

  public function get_csv_data($host, $seconds, array $params) {
    $url = '';
    if (count($params)) {
      $params['q'] = 'monitor-server/get-data-by-plugin';
      $params['host'] = $host;
      $params['seconds'] = $seconds;
      $params['data_set'] = 'full';
      $params['format'] = 'csv';
      $url = $this->build_url($params);
    }
    if (!empty($url)) {
      return l(t('Download Dataset'), $url);
    }
    return '';
  }

  /**
   *
   */
  public function get_data($host, $seconds, array $params) {
    $data = array();
    if (isset($host)) {
      $params['q'] = 'monitor-server/get-data-by-plugin';
      $params['host'] = $host;
      $params['seconds'] = $seconds;
      $params['data_set'] = 'full';
      $result = $this->execute($params);
      //format it for usage
      if ($result) {
        $data['header'] = array('Date');
        foreach ($result['meta']['legend']['entry'] as $header) {
          $data['header'][] = ucwords($header);
        }
        foreach ($result['data']['row'] as $key => $row) {
          $time = date('m-d-Y G:i:s', $row['t']); //format time

          $data['row'][$key] = array($time);
          foreach ($row['v'] as $value) {
            $data['row'][$key][] = ($value != 'NaN') ? floatval($value) : $value;
          }
        }
      }
    }
    return $data;

  }

  /**
   * Get all the supported plugins
   */
  public function get_supported_plugins() {
    $data = $this->execute(array('q' => 'monitor-server/get-supported-plugins'));
    return $data;
  }

  /**
   * Get all the hosts from a monitoring system
   */
  public function get_hosts() {
    $data = $this->execute(array('q' => 'monitor-server/get-all-hosts'));
    $hosts = array();
    if (count($data)) {
      foreach ($data as $value) {
        $instance = cloud_monitoring_monitor_instance_by_dns($value);
        if ($instance != FALSE ) {
          $hosts[$value] = cloud_monitoring_nickname_by_dns($value);
        }
      }
    }
    return $hosts;
  }

  /**
   * Expected return is Name, Id, Status
   */
  public function get_server_list() {
    $servers = array();
    if (isset($this->monitor_id)) {
      
      $monitorId = $this->monitor_id ;
      $query = "SELECT cmi.instance_id, cmi.status, ci.cloud_type, ci.instance_nickname from
                {cloud_monitor_instance} cmi INNER JOIN {cloud_aws_instances} ci on ci.instance_id = cmi.instance_id
                where monitor_id = $monitorId and ci.instance_state_name in ('running', 'pending') order by ci.instance_nickname";

      $result = db_query($query);

      foreach ($result as $obj) {
        $action = '';
        if ($obj->status) {
          $action = cloud_display_action('images/action_stop', t('Stop'), "design/cloud_monitoring/{$this->monitor_id}/stop/{$obj->cloud_type}/{$obj->instance_id}", array('html' => TRUE));
        }
        else {
          $action = cloud_display_action('images/resume', t('Resume'), "design/cloud_monitoring/{$this->monitor_id}/start/{$obj->cloud_type}/{$obj->instance_id}", array('html' => TRUE));
        }
        //add delete action
        $action .= cloud_display_action('images/icn_delete', t('Delete'), "design/cloud_monitoring/{$this->monitor_id}/delete/{$obj->cloud_type}/{$obj->instance_id}", array('html' => TRUE));

        $servers[] = array(
          array(
            'data' => l($obj->instance_nickname, "clouds/{$obj->cloud_type}/instances/describe/{$obj->instance_id}/monitor"),
            'class' => array('nickname-column'),
          ),
          $obj->instance_id,
          ($obj->status) ? t('Enabled') : t('Disabled'),
          array(
            'data' => $action,
            'class' => 'action-column',
          ),
        );
      }
    }
    return $servers;
  }

  public function get_monitor_details() {
    
    $monitorId = $this->monitor_id;
    $query = "SELECT cloud_monitor.monitor_id, cloud_monitor.monitor_name, cloud_monitor.monitor_type, cloud_monitor.instance_id, cloud_aws_instances.instance_nickname, cloud_aws_instances.dns_name, cloud_aws_instances.key_name
                        FROM {cloud_monitor} INNER JOIN {cloud_aws_instances} on cloud_monitor.instance_id = cloud_aws_instances.instance_id
                        and cloud_aws_instances.instance_state_name in ('running', 'pending') and cloud_monitor.monitor_id = $monitorId ";
    $result = db_query($query)->fetchAssoc();
    return $result;
  }

  /**
   * Helper method to build the full endpoint url
   */
  protected function build_url(array $params) {
    return url($this->endpoint . '/', array('query' => $params));
  }

  /**
   * Execute a http request against a given
   * REST path.  The endpoint is a drupal server
   * so the url is built as such.  The urls are not
   * built with the clean url style because not all
   * servers have that enabled.
   */
  protected function execute(array $params) {
    $decoded_result = array();
    if (count($params) && isset($params['q'])) {
      $url = $this->build_url($params);
      if (!empty($url)) {
        $result = drupal_http_request($url);
        if ($result->code == '200' && isset($result->data)) {
          $decoded_result = json_decode($result->data, TRUE);
        }
      }
    }
    return $decoded_result;
  }

  /**
   * Install monitoring script on a cloud instance.  This will
   * let the cloud instance communicate with the monitoring server.
   */
  public function install_monitor_script(array $params) {
    $script = _cloud_monitoring_collectd_instance_script();
    $ip = $this->get_ip();
    if (!empty($ip)) {
      $script = str_replace('MONITOR_IP', $ip, $script);
      $return = _cloud_scripting_exec_one_off_script($params['cloud_context'], $params['instance_id'], $script, 'cloud_instance_install', TRUE);
      if ($return == 0) {
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }

  public function stop_monitor(array $params) {
    $script = _cloud_monitoring_collectd_instance_stop();
    $return = _cloud_scripting_exec_one_off_script($params['cloud_context'], $params['instance_id'], $script, 'cloud_instance_monitor_stop', TRUE);
  }

  public function start_monitor(array $params) {
    $script = _cloud_monitoring_collectd_instance_start();
    $return = _cloud_scripting_exec_one_off_script($params['cloud_context'], $params['instance_id'], $script, 'cloud_instance_monitor_start', TRUE);
  }

  public function delete_monitor(array $params) {
    $script = _cloud_monitoring_collectd_instance_delete();
    $return = _cloud_scripting_exec_one_off_script($params['cloud_context'], $params['instance_id'], $script, 'cloud_instance_monitor_delete', TRUE);
  }

}
