<?php

/**
 * Class KalturaHelpers.
 */
class KalturaHelpers {

  function __construct() {
    if (function_exists('libraries_load')) {
      libraries_load('KalturaClient');
    }
    else {
      $status_report = l(t('Status report'), 'admin/reports/status');
      drupal_set_message(t("Kaltura module now requires Libraries module to be installed and enabled. Please follow instructions listed on the !status_report page.", array('!status_report' => $status_report)), 'error');
    }
  }

  function getSimpleEditorFlashVars($ks, $kshowId, $type, $partner_data, $uiConfId = NULL) {
    $session_user = $this->getSessionUser();
    $config = self::getServiceConfiguration();

    $flash_vars = array();

    if ($type == 'entry') {
      $flash_vars["entry_id"] 		= $kshowId;
      $flash_vars["kshow_id"] 		= 'entry-' . $kshowId;
    }
    else {
      $flash_vars["entry_id"] 		= -1;
      $flash_vars["kshow_id"] 		= $kshowId;
    }

    $flash_vars["partner_id"] 	= $config->partnerId;
    $flash_vars["partnerData"] 	= $partner_data;
    $flash_vars["subp_id"] 		= $config->subPartnerId;
    $flash_vars["uid"] 			= $session_user->id;
    $flash_vars["ks"] 			= $ks;
    $flash_vars["backF"] 		= "onSimpleEditorBackClick";
    $flash_vars["saveF"] 		= "onSimpleEditorSaveClick";
    if ($uiConfId) {
      $flash_vars["uiConfId"] = $uiConfId;
    }
    else {
      $flash_vars["uiConfId"] = KALTURASETTINGS_SE_UICONF_ID;
    }

    return $flash_vars;
  }

  function getAdvancedEditorFlashVars($ks, $kshowId, $type, $partner_data, $uiConfId = NULL) {
    $session_user = $this->getSessionUser();
    $config = self::getServiceConfiguration();

    $flash_vars = array();

    if ($type == 'entry') {
      $flash_vars["entry_id"] 		= $kshowId;
      $flash_vars["kshow_id"] 		= 'entry-' . $kshowId;
    }
    else {
      $flash_vars["entry_id"] 		= -1;
      $flash_vars["kshow_id"] 		= $kshowId;
    }

    $flash_vars["partner_id"] 	= $config->partnerId;
    $flash_vars["partnerData"] 	= $partner_data;
    $flash_vars["subp_id"] 		= $config->subPartnerId;
    $flash_vars["uid"] 			= $session_user->id;
    $flash_vars["ks"] 			= $ks;
    $flash_vars["backF"] 		= "onSimpleEditorBackClick";
    $flash_vars["saveF"] 		= "onSimpleEditorSaveClick";
    if ($uiConfId) {
      $flash_vars["uiConfId"] = $uiConfId;
    }
    else {
      $flash_vars["uiConfId"] = KALTURASETTINGS_AE_UICONF_ID;
    }

    return $flash_vars;
  }

  function getKalturaPlayerFlashVars($ks, $kshowId = -1, $entryId = -1) {
    $session_user = $this->getSessionUser();

    $flash_vars = array();
    $flash_vars["uid"] 			= $session_user->id;

    return $flash_vars;
  }

  function getSwfUrlForBaseWidget() {
    return $this->getSwfUrlForWidget(KALTURASETTINGS_BASE_WIDGET_ID);
  }

  function getSwfUrlForWidget($widgetId) {
    return self::getKalturaServerUrl() . "/kwidget/wid/" . $widgetId;
  }

  function getSimpleEditorUrl($uiConfId = NULL) {
    if ($uiConfId) {
      return self::getKalturaServerUrl() . "/kse/ui_conf_id/" . $uiConfId;
    }
    else {
      return self::getKalturaServerUrl() . "/kse/ui_conf_id/" . KALTURASETTINGS_SE_UICONF_ID;
    }
  }

  function getAdvancedEditorUrl($uiConfId = NULL) {
    if ($uiConfId) {
      return self::getKalturaServerUrl() . "/kae/ui_conf_id/" . $uiConfId;
    }
    else {
      return self::getKalturaServerUrl() . "/kae/ui_conf_id/" . KALTURASETTINGS_AE_UICONF_ID;
    }
  }

  function getThumbnailUrl($widgetId = NULL, $entryId = NULL, $width = 240, $height= 180) {
    $config = self::getServiceConfiguration();
    $url = self::getKalturaServerUrl();
    $url .= "/p/" . $config->partnerId;
    $url .= "/sp/" . $config->subPartnerId;
    $url .= "/thumbnail";
    if ($widgetId) {
      $url .= "/widget_id/" . $widgetId;
    }
    elseif ($entryId) {
      $url .= "/entry_id/" . $entryId;
    }
    $url .= "/width/" . $width;
    $url .= "/height/" . $height;
    $url .= "/type/2";
    $url .= "/bgcolor/000000";
    return $url;
  }

  /**
   * Initialises variables for the config object.
   *
   * @return KalturaConfiguration
   */
  public static function getServiceConfiguration() {
    $partnerId = variable_get('kaltura_partner_id', 0);
    if ($partnerId == '') {
      $partnerId = 0;
    }

    $subPartnerId = variable_get('kaltura_subp_id', 0);
    if ($subPartnerId == '') {
      $subPartnerId = 0;
    }

    $config = new KalturaConfiguration();
    $config->serviceUrl = self::getKalturaServerUrl();
    $config->subPartnerId = $subPartnerId;
    $config->partnerId = $partnerId;
    $config->curlTimeout = 60;

    //$config->setLogger(new KalturaLogger());
    return $config;
  }

  /**
   * Gets the url of the server either from drupal or the settings file.
   *
   * @return string
   */
  public static function getKalturaServerUrl() {
    $url = variable_get('kaltura_server_url', KALTURASETTINGS_SERVER_URL);
    $url = $url ? rtrim($url, '/') : rtrim(KALTURASETTINGS_SERVER_URL, '/');
    if ($GLOBALS['is_https']) {
      $url = preg_replace('/^\/\//', 'https://', $url, 1);
    }
    else {
      $url = preg_replace('/^\/\//', 'http://', $url, 1);
    }
    return $url;
  }

  /**
   * CMAC
   * gets the username and id of the current drupal user
   * change: replaced KalturaSessionUser with KalturaUser object
   * TODO: add more variables to the kaltura user object
   * @return KalturaUser object
   */
  function getSessionUser() {
    global $user;

    $kaltura_user = new KalturaUser();

    if ($user->uid) {
      $kaltura_user->id= $user->uid;
      $kaltura_user->screenName = $user->name;
      $kaltura_user->email = $user->mail;
    }
    else {
      $kaltura_user->id = KALTURASETTINGS_ANONYMOUS_USER_ID;
    }

    return $kaltura_user;
  }

  /**
   * oferc
   * @return: the list of players defined for the account
   */
  function getSitePlayers(&$arr) {
    static $players;

    $arr['48501'] = array('name' => 'Light', 'width' => 0, 'height' => 0);
    $arr['48502'] = array('name' => 'Dark', 'width' => 0, 'height' => 0);

    if (empty($players)) {
      try {
        $players = array();
        $k_helpers = new KalturaHelpers();
        $client = $k_helpers->getKalturaClient(true);
        $listResponse = $client->uiConf->listAction();

        for ($i = 0; $i < $listResponse->totalCount; $i++) {
          if ($listResponse->objects[$i]->objType == KalturaUiConfObjType::PLAYER) {
            // Don't show playlist as regular player.
            if (stristr($listResponse->objects[$i]->tags, "playlist") != FALSE) {
              continue;
            }

            $arr[$listResponse->objects[$i]->id] = array(
              'name'   => $listResponse->objects[$i]->name,
              'width'  => $listResponse->objects[$i]->width,
              'height' => $listResponse->objects[$i]->height,
            );
            $players[$listResponse->objects[$i]->id] = array(
              'name'   => $listResponse->objects[$i]->name,
              'width'  => $listResponse->objects[$i]->width,
              'height' => $listResponse->objects[$i]->height,
            );
          }
        }
      }
      catch (Exception $e) {
        watchdog_exception('kaltura', $e);
      }
    }
    else {
      foreach ($players as $key => $sitePlayer) {
        $arr[$key] = $sitePlayer;
      }
    }
  }

  /**
   * oferc
   * this method is defined just for clearence, acctualy it is the same as regular players
   * @return: the list of players defined for the account
   */
  function getSitePlaylistPlayers(&$arr) {
    $arr['1292302'] = array('name' => 'Playlist', 'width' => 0, 'height' => 0);
    $this->getSitePlayers($arr);
  }

  function getKalturaClient($isAdmin = FALSE, $privileges = NULL) {
    // Get the configuration to use the kaltura client.
    $kalturaConfig = self::getServiceConfiguration();

    if (!$privileges) {
      $privileges = 'edit:*';
    }
    // Inititialize the kaltura client using the above configurations.
    $kalturaClient = new KalturaClient($kalturaConfig);
    // Get the current logged in user.
    $session_user = $this->getSessionUser();

    // Get the variables required to start a session.
    $partnerId = variable_get('kaltura_partner_id', '');
    $secret = variable_get('kaltura_secret', '');
    $adminSecret = variable_get('kaltura_admin_secret', '');

    if ($isAdmin) {
      $result = $kalturaClient->session->start($adminSecret, $session_user->id, KalturaSessionType::ADMIN, $partnerId, 86400, $privileges);
    }
    else {
      $result = $kalturaClient->session->start($secret, $session_user->id, KalturaSessionType::USER, $partnerId, 86400, $privileges);
    }
    $len = strlen($result);
    /** proper method for error checking please
    if ($len!=116)
    {
      watchdog("kaltura", $result );
      return null;
    }else {
     */
    // set the session so we can use other service methods
    $kalturaClient->setKs($result);
    //}

    return $kalturaClient;
  }

  function uploadFile($isAdmin, $file, $name) {
    try {
      $kaltura_client = $this->getKalturaClient($isAdmin);

      $token = $kaltura_client->baseEntry->upload($file);
      $entry = new KalturaBaseEntry();
      $entry ->name = $name;
      $res = $kaltura_client->baseEntry->addFromUploadedFile($entry, $token, NULL);
    }
    catch(Exception $ex) {
      $res = $ex->getMessage();
    }
    return $res;
  }

  /**
   * Retrieves custom metadata for entry.
   *
   * @param string $entry_id
   *   Kaltura media entry ID.
   *
   * @return array
   *   Each array's element corresponds to metadata from one profile: key is the
   *   profile ID and value is an array with the following elements:
   *   - fields: Array with metadata fields, where each key is the field's
   *     system name and each value is an indexed array of field's values.
   *   - metadata: KalturaMetadata object as returned from the service.
   */
  public function getEntryMetadata($entry_id) {
    $data = array();

    try {
      $client = $this->getKalturaClient(TRUE);
      $plugin = KalturaMetadataClientPlugin::get($client);

      $filter = new KalturaMetadataFilter();
      $filter->metadataObjectTypeEqual = KalturaMetadataObjectType::ENTRY;
      $filter->objectIdEqual = $entry_id;

      $response = $plugin->metadata->listAction($filter);

      foreach ($response->objects as $object) {
        $fields = array();
        $xml = simplexml_load_string($object->xml);
        foreach ($xml->children() as $xml_node) {
          $fields[$xml_node->getName()][] = (string) $xml_node;
        }

        $data[$object->metadataProfileId] = array(
          'fields' => $fields,
          'metadata' => $object,
        );
      }
    }
    catch (Exception $e) {
      watchdog_exception('kaltura', $e);
    }

    return $data;
  }

  /**
   * Retrieves metadata profile.
   *
   * @param int $profile_id
   *   Profile ID.
   *
   * @return \KalturaMetadataProfile|null
   *   Metadata profile object.
   */
  public function getMetadataProfile($profile_id) {
    $profiles = &drupal_static('KalturaHelpers::getMetadataProfile', array());

    if (!isset($profiles[$profile_id])) {
      $cid = 'kaltura:metadata_profile:' . $profile_id;

      if ($cache = cache_get($cid)) {
        $profiles[$profile_id] = $cache->data;
      }
      else {
        try {
          $client = $this->getKalturaClient(TRUE);
          $plugin = KalturaMetadataClientPlugin::get($client);

          $profiles[$profile_id] = $plugin->metadataProfile->get($profile_id);
        }
        catch (Exception $e) {
          watchdog_exception('kaltura', $e);
        }

        if (!empty($profiles[$profile_id])) {
          cache_set($cid, $profiles[$profile_id]);
        }
      }
    }

    return !empty($profiles[$profile_id]) ? $profiles[$profile_id] : NULL;
  }

  /**
   * Retrieves the list of metadata structure (custom fields).
   *
   * @return array
   *   Each array's element corresponds to one metadata profile: key is the
   *   profile ID and the value is an array with following elements:
   *   - field_info: Array with fields structure, where each key is the field's
   *     system name and each value is a SimpleXMLElement object describing
   *     the field as returned from the service.
   *   - metadata_profile: KalturaMetadataProfile object as returned from
   *     the service.
   */
  public function getMetadataFieldList() {
    $data = array();

    try {
      $client = $this->getKalturaClient(TRUE);
      $plugin = KalturaMetadataClientPlugin::get($client);

      $filter = new KalturaMetadataProfileFilter();
      $filter->metadataObjectTypeEqual = KalturaMetadataObjectType::ENTRY;

      $response = $plugin->metadataProfile->listAction($filter);

      foreach ($response->objects as $profile) {
        $fields = array();
        $xml = simplexml_load_string($profile->xsd, NULL, 0, 'xsd', TRUE);

        foreach ($xml->element->complexType->sequence->element as $element) {
          $name = (string) $element->attributes()->name;
          $fields[$name] = $element;
        }

        $data[$profile->id] = array(
          'field_info' => $fields,
          'metadata_profile' => $profile,
        );

        // Refresh cached profiles.
        cache_set('kaltura:metadata_profile:' . $profile->id, $profile);
      }
    }
    catch (Exception $e) {
      watchdog_exception('kaltura', $e);
    }

    return $data;
  }

  /**
   * Filters up-to-date entries of specific timestamp.
   *
   * @param int $timestamp
   *   Timestamp to check.
   *
   * @return array
   *   Entry IDs that need to be updated.
   *
   * @throws \Exception
   *   If number of entries was changed remotely while fetching entries.
   */
  public function filterOutUpToDateEntries($timestamp) {
    $client = $this->getKalturaClient(TRUE);

    $filter = new KalturaMediaEntryFilter();
    $filter->orderBy = KalturaMediaEntryOrderBy::CREATED_AT_ASC;
    $filter->updatedAtGreaterThanOrEqual = $timestamp;
    $filter->updatedAtLessThanOrEqual = $timestamp;

    $pager = new KalturaFilterPager();
    $pager->pageSize = 100;

    $entry_ids = array();
    $total = NULL;

    while (TRUE) {
      ++$pager->pageIndex;
      $result = $client->media->listAction($filter, $pager);

      if (!isset($total)) {
        $total = $result->totalCount;
      }
      elseif ($result->totalCount != $total) {
        throw new Exception(t('Number of entries was changed remotely.'));
      }

      if (empty($result->objects)) {
        break;
      }

      foreach ($result->objects as $entry) {
        $entry_ids[] = $entry->id;
      }

      if (count($entry_ids) == $total) {
        break;
      }
    }

    if ($entry_ids) {
      $query = new EntityFieldQuery();
      $result = $query
        ->entityCondition('entity_type', 'kaltura_entry')
        ->propertyCondition('kaltura_entryid', $entry_ids)
        ->propertyCondition('kaltura_updated_date', $timestamp)
        ->execute();

      if (!empty($result['kaltura_entry'])) {
        $entities = entity_load('kaltura_entry', array_keys($result['kaltura_entry']));
        $up_to_date = array();

        foreach ($entities as $entity) {
          $up_to_date[] = $entity->kaltura_entryid;
        }

        $entry_ids = array_diff($entry_ids, $up_to_date);
      }
    }

    return $entry_ids;
  }
}

/**
 * Class KalturaContentCategories.
 */
class KalturaContentCategories {

  public $categories = array(
    'Arts & Literature',
    'Automotive',
    'Business',
    'Comedy',
    'Education',
    'Entertainment',
    'Film & Animation',
    'Gaming',
    'Howto & Style',
    'Lifestyle',
    'Men',
    'Music',
    'News & Politics',
    'Nonprofits & Activism',
    'People & Blogs',
    'Pets & Animals',
    'Science & Technology',
    'Sports',
    'Travel & Events',
    'Women',
  );
}
