<?php


/**
 * @file
 * Abstract class for cloud monitoring.  To add a
 * new monitoring system into Clanavi, extend monitor_system
 * and implement its abstract methods.
 */

abstract class monitor_system {

  protected $endpoint;
  protected $monitor_id;

  /**
   * Default Constructor
   */
  function __construct($monitor_id) {
    $this->monitor_id = $monitor_id;
    $this->set_endpoint($monitor_id);
  }

  /**
   * Set the end point of the monitoring system.
   * Extending classes are responsible for setting
   * the monitoring endpoint.  The reason is that
   * different monitoring servers can exist on
   * different clouds.  Different clouds in Clanavi
   * can have different data structures. The use case
   * for this abstracted method is XCP vs Amazon based
   * clouds
   */
  protected abstract function set_endpoint();

  /**
   * Retrieve the server list from the monitoring
   * server
   */
  public abstract function get_hosts();

  /**
   * Define the supported plugins the implementing class
   * supports. Plugins are defined as the service being
   * monitored, such as interface, cpu, df, load...etc
   *
   * @return
   *   An array containing the plugins.
   */
  public abstract function get_supported_plugins();

  /**
   * Ping the server to make sure it is active
   * @return
   *   boolean indicating the server responded or not
   */
  public abstract function is_active();

  /**
   * Get the IP address for this monitoring server.
   * @return
   *   ip address of this monitoring server
   */
  public abstract function get_ip();

  /**
   * Define any details a particular plugin might have. The return
   * needs to be an array.
   *
   * @return
   *   An array of objects containing plugin details.
   */
  public abstract function get_plugin_details($host, $plugin = NULL, $details = 'true');

  /**
   * Define how the small graph img src is retrieved.
   *
   * @return
   *   A fully formed image url string
   */
  public abstract function get_small_graph($host, $seconds, array $params);

  /**
   * Define how the large graph img src is retrieved
   *
   * @return
   *   A fully formed image url string
   */
  public abstract function get_large_graph($host, $seconds, array $params);

  /**
   * Define how data exports are retrieved.
   *
   * @return
   *   An array of data points
   */
  public abstract function get_data($host, $seconds, array $params);

  /**
   * Define how CSV data is retrieved
   *
   * @return
   *   A fully formed url string for downloading csv data
   */
  public abstract function get_csv_data($host, $seconds, array $params);

  /**
   * Retrieve the servers being monitored by the monitoring server.
   * This method is called on the Monitor details UI.
   *
   * @return
   *   An associative array of servers containing Name, Id, Monitoring Status
   */
  public abstract function get_server_list();

  /**
   * Retrieve the details about a cloud monitor system given the monitor_id
   * @return
   *   Return an object of monitor information
   */
  public abstract function get_monitor_details();

  /**
   * Install monitoring software on a cloud instance.
   *
   * @return
   *   Return false if the extending class does not have an installer script
   */
  public abstract function install_monitor_script(array $params);

  /**
   * Stop the monitor software on a particular instance
   *
   *  @return
   *    Return boolean depending on script status
   */
  public abstract function stop_monitor(array $params);

  /**
   * Start the monitor software on a particular instance
   */
  public abstract function start_monitor(array $params);

  /**
   * Delete the monitr software on a particular instance
   */
  public abstract function delete_monitor(array $params);

}
