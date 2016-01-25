<?php

/**
 * @file
 * This class creates a block of HTML that contains the most popular URLs in
 * your Chartbeat account.
 *
 * It displays the most popular pages found at Chartbeat for this domain.
 */

/**
 * Class MostPopularBlock
 *
 * This class represents the block of HTML created by this module.
 */
class MostPopularBlock {

  protected $listLength;
  protected $apiKey;
  protected $hostName;
  protected $debugFlag;
  protected $json;
  protected $frontpageTitle;
  protected $campaignName;
  protected $campaignSource;
  protected $campaignMedium;

  /**
   * Getter method for campaignName.
   *
   * @return string
   *   Returns the name of the Google campaign
   */
  public function getCampaignName() {
    return $this->campaignName;
  }

  /**
   * Setter method for campaignName.
   *
   * @param string $campaignname
   *   Sets the campaignName.
   */
  public function setCampaignName($campaignname) {
    $this->campaignName = $campaignname;
  }

  /**
   * Getter method for frontpageTitle.
   *
   * @return string
   *   Returns the front page title
   */
  public function getFrontpageTitle() {
    return $this->frontpageTitle;
  }

  /**
   * Setter method for frontpageTitle.
   *
   * @param string $frontpagetitle
   *   Sets the frontpageTitle.
   */
  public function setFrontpageTitle($frontpagetitle) {
    $this->frontpageTitle = $frontpagetitle;
  }

  /**
   * Getter method for listLength.
   *
   * @return int
   *   Returns the number of items that the block is supposed to output.
   */
  public function getListLength() {
    return $this->listLength;
  }

  /**
   * Setter method for listLength.
   *
   * @param int $listlength
   *   Sets the nubmer of items that the block is supposed to output.
   */
  public function setListLength($listlength) {
    $this->listLength = $listlength;
  }

  /**
   * Getter method for apiKey.
   *
   * @return string
   *   Gets the Chartbeat API key the module will use.
   */
  public function getApiKey() {
    return $this->apiKey;
  }

  /**
   * Setter method for apiKey.
   *
   * @param string $apikey
   *   Sets the Chartbeat API key the module will use.
   */
  public function setApiKey($apikey) {
    $this->apiKey = $apikey;
  }

  /**
   * Setter method for getHostName.
   *
   * @return string
   *   Sets the hostname that will be used as part of the Chartbeat API call.
   */
  public function getHostName() {
    return $this->hostName;
  }

  /**
   * Getter method for getHostName.
   *
   * @param mixed $hostname
   *   Gets the hostname that will be used as part of the Chartbeat API call.
   */
  public function setHostName($hostname) {
    $this->hostName = check_url($hostname);
  }

  /**
   * Getter method for getDebugFlag.
   *
   * @return bool
   *   Gets flag used to turn on database logging.
   */
  public function getDebugFlag() {
    return $this->debugFlag;
  }

  /**
   * Setter method for debugFlag.
   *
   * @param bool $debugflag
   *   Sets flag used to turn on database logging.
   */
  public function setDebugFlag($debugflag) {
    $this->debugFlag = $debugflag;
  }

  /**
   * Getter method for json.
   *
   * @return string
   *   Gets json retrieved from Chartbeat.
   */
  public function getJson() {
    return $this->json;
  }

  /**
   * Setter method for json.
   *
   * @param string $json
   *   Sets json with a string (should be JSON from Chartbeat).
   */
  public function setJson($json) {
    $this->json = $json;
  }

  /**
   * Getter method for campaignSource.
   *
   * @return string
   *   Gets string used for the source value in Google Analytics campaign code.
   */
  public function getCampaignSource() {
    return $this->campaignSource;
  }

  /**
   * Setter method for campaignSource.
   *
   * @param string $campaignsource
   *   Sets string used for the source value in Google Analytics campaign code.
   */
  public function setCampaignSource($campaignsource) {
    $this->campaignSource = check_plain($campaignsource);
  }

  /**
   * Getter method for campaignMedium.
   *
   * @return string
   *   Gets string used for the medium value in Google Analytics campaign code.
   */
  public function getCampaignMedium() {
    return $this->campaignMedium;
  }

  /**
   * Setter method for campaignMedium.
   *
   * @param string $campaignmedium
   *   Gets string used for the medium value in Google Analytics campaign code.
   */
  public function setCampaignMedium($campaignmedium) {
    $this->campaignMedium = check_plain($campaignmedium);
  }

  /**
   * This initializes the block and readies it contact Chartbeat.
   *
   * It also checks the Drupal vars to make sure that the block has been
   * properly configured.
   */
  public function initialize() {
    $this->setApiKey(variable_get('chartbeat_mostpopular_apikey'));
    $this->setListLength(variable_get('chartbeat_mostpopular_list_length', 3));
    $this->setHostName(variable_get('chartbeat_mostpopular_host'));
    $this->setFrontpageTitle(variable_get('chartbeat_mostpopular_frontpage_title'));
    $this->setCampaignName(variable_get('chartbeat_mostpopular_campaign_name'));
    $this->setDebugFlag(variable_get('chartbeat_mostpopular_debugging'));
    $this->setCampaignSource(variable_get('chartbeat_mostpopular_campaign_source'));

    if (drupal_strlen($this->getApiKey()) == 0) {
      watchdog("chartbeat_mostpopular", "API Key too short --
      not properly configured");
      trigger_error('API Key too short -- not properly configured');
    }

    if (drupal_strlen($this->getListLength()) == 0) {
      watchdog("chartbeat_mostpopular", "Items list is too short --
      not properly configured");
      trigger_error("Items list is too short -- not properly configured");
    }

    if (drupal_strlen($this->getHostName()) == 0) {
      watchdog("chartbeat_mostpopular", "Host name is too short --
        not properly configured");
      trigger_error("Host name is too short --  not properly configured");
    }
    // If the Front Page title is not set, it will use the hostname instead.
    if (drupal_strlen($this->getFrontpageTitle()) == 0) {
      $this->setFrontpageTitle($this->getHostName());
    }
  }

  /**
   * Uses block properties to get Chartbeat data.
   *
   * @return string
   *   This string is made of JSON from Chartbeat.
   */
  public function retrieveChartbeatData() {
    // Pull charts from Chartbeat.
    $url = 'http://api.chartbeat.com/live/toppages/v3';
    $data = array(
      'apikey' => $this->getApiKey(),
      'host' => $this->getHostName(),
      'limit' => $this->getListLength(),
    );

    $full_url = url($url, array('query' => $data));

    $json = drupal_http_request($full_url);

    $jsonfeed = drupal_json_decode($json->data, TRUE);

    if ($this->debugFlag) {
      watchdog('chartbeat_mostpopular', '<pre>' . $json->data . '</pre>');
    }

    $this->setJson($jsonfeed);
  }

  /**
   * This function creates the HTML for the Chartbeat - Most Popular block.
   *
   * @return string
   *   This returns a large block of HTML for presentation to the user.
   *   Caching code has been added so that it only pulls data from Chartbeat
   *   once an hour.
   */
  public function display() {

    $output = &drupal_static(__FUNCTION__);

    if (!isset($output)) {
      $cache = cache_get('cache_chartbeat_mostpopular');

      if ($cache) {
        $output = $cache->data;
        if ($this->getDebugFlag()) {
          watchdog('chartbeat_mostpopular',
            'Cache was retrieved'
          );
        }
        return $output;
      }

      else {

        $jsonfeed = $this->getJson();

        $output = "";

        // Start building the HTML divs for the block.
        $output .= '<ul>';

        $limit = count($jsonfeed['pages']);

        if ($this->getDebugFlag()) {
          watchdog('chartbeat_most_popular',
            'number of items in jsonfeed[pages]:<pre>' . $limit . '</pre>'
          );
        }

        if ($limit == 0) {
          $output = '';
          return $output;
        }

        // Go through the Chartbeat data and traverse the list based on user
        // configuration of $list_length.
        for ($i = 0; $i < $limit; $i++) {
          if ($i > $this->getListLength() + 1) {
            return;
          }
          // Now you know the url path and page title.
          $urlpath[$i] = $jsonfeed['pages'][$i]['path'];
          $pagetitle[$i] = $jsonfeed['pages'][$i]['title'];

          // Extract the page title (and not the other section and site stuff).
          $exploded_title = explode("|", $pagetitle[$i]);

          // Convert any possible HTML entities in the page title.
          $current_title = html_entity_decode($exploded_title[0],
            ENT_QUOTES | ENT_IGNORE, "UTF-8"
          );
          $current_title = check_plain($current_title);
          $ga_string = '';

          // If there is a campaign name go ahead and build the GA link.
          if (drupal_strlen($this->getCampaignName()) > 0) {

            // Build campaign string for Google analytics.
            $utm_source = $this->getCampaignSource();
            $utm_medium = "text-link";
            $utm_content = trim($current_title);
            $utm_campaign = $this->getCampaignName();
            $ga_string = "?utm_medium=text-link&utm_content=" . $utm_content . "&utm_campaign=" . $utm_campaign . "&utm_source=" . $utm_source;
          }
          // This is the url we will attach to every HREF in the link.
          $proposed_url = 'http://www.' . $urlpath[$i] . $ga_string;

          $item_title = "";

          // If the current url being processed is empty, don't get the node
          // title.
          if (!empty($urlpath[$i])) {
            $item_title = $this->getNodeTitle($urlpath[$i]);
          }

          // If it's not a node, then we have to fall back on the page title
          // that Chartbeat reported.
          if (empty($item_title)) {
            $item_title = $pagetitle[$i];
          }

          // <li><a><img><h3 class="title"></h3></a></li>
          $style_prefix = '<li>';
          $style_postfix = '</li>';

          // Using the proposed URL, do checks on it to build the correct link.
          $title = '<h3 class="title">' . $item_title . '</h3>';
          $href = l($title, $proposed_url, array('html' => 'true'));
          $output .= $style_prefix . $href . $style_postfix;
        }
        $output .= '</ul>';

        $current_timestamp = time();
        $in1hourstime = strtotime("+1 hours", $current_timestamp);
        cache_set('cache_chartbeat_mostpopular', $output, 'cache', $in1hourstime);

        if ($this->getDebugFlag()) {
          watchdog('chartbeat_mostpopular',
            'Cache was set'
          );
        }

      }
      return $output;
    }
  }

  /**
   * Used only by display method.
   *
   * This method retrieves proper node titles for items found in Chartbeat. This
   * only works on items that are nodes -- term pages and user pages don't have
   * titles.
   *
   * @param string $url
   *   The url passed only contains URI. The host name is not included.
   *
   * @return string
   *   This contains just the title of the node.
   */
  protected function getNodeTitle($url) {
    // Check to see if the URL passed by Chartbeat is only the home page URL.
    $hostname = $this->getHostName() . "/";
    $url = str_replace($hostname, "", $url);

    if (drupal_strlen($url) == 0) {
      $title = $this->getFrontpageTitle();
    }
    // This is where we load the node to get its title.
    else {
      $drupal_path = drupal_parse_url($url);
      $url = $drupal_path['path'];
      $node_path = drupal_lookup_path("source", $url);

      $node = menu_get_object("node", 1, $node_path);

      if ($node->title) {
        $title = $node->title;
      }

    }
    return $title;
  }

}
