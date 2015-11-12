<?php


/**
 * The class extendes Avatar to add World of Warcraft Functions.
 *
 * @see AvatarController::query()
 */
// Ensure that alter_ego module loads.  Needed if installing with dependancies and uninstalls.
//drupal_load('module', 'alter_ego');
class WoWToon extends Avatar {
  protected $locale;
  public $loaded_name;
  public $toon_key;
  public $update_status;

  public function __construct($values = array()) {
    parent::__construct($values, 'avatar');
    $this->locale = variable_get('wowguild_locale');
  }

  /**
   *
   * @see Entity::save()
   * @see entity_save()
   */
  public function save() {
    // Save properly capatilized name.
    /*
     print("presave()");

    if($this->loaded_name) {
    $this->name = $this->loaded_name;
    }
    die($this);
    $return = entity_get_controller($this->entityType)->save($this);
    //$this->setToonKey();
    $query = db_delete('wowtoon_owners');
    $query->condition('toon_key', $this->toon_key);
    $query->execute();
    if ($this->uid) {

    }
    return $return;
    */
    //return entity_get_controller($this->entityType)->save($this);
    return parent::save();
  }

  /**
   * Overridden.
   *
   * @see entity::delete()
   * @see entity_delete()
   */
  public function delete() {
    $id = $this->identifier();
    if (isset($id)) {
      entity_get_controller($this->entityType)->delete(array($id));

      $this->setToonKey();
      $query = db_delete('wowtoon_owners');
      $query->condition('toon_key', $this->toon_key);
      $query->execute();

    }
  }

  protected function defaultLabel() {
    return $this->fullname;
  }

  protected function defaultUri() {
    return array('path' => 'avatar/' . $this->aid);
  }


  public function setToonKey() {
    $this->toon_key = wowguild_getkey($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], $this->name);
  }
  /**
   * Load Toon Data and attach it to $this.  Only grabs from the database.
   *
   * Return bool - TRUE if toon was found in the database.
   */
  public function loadToon() {
    $this->setToonKey();

    $query = db_select('wowtoon_cached_data', 'w');
    $query->fields('w');
    $query->leftJoin('wowtoon_owners', 'o', 'o.toon_key = w.toon_key');
    $query->fields('o', array('uid', 'aid'));
    $query->condition('w.toon_key', $this->toon_key);
    $result = $query->execute()->fetch();

    if (!empty($result)) {
      unset($this->is_new);
      foreach ($result as $key => $value) {
        // Don't overwrite existing values unless empty.
        if (!isset($this->{$key})) {
          $this->{$key} = $value;
        }
        else {
          if (empty($this->{$key}) && !empty($value)) {
            $this->{$key} = $value;
          }
        }
      }
      $this->fullrealm = $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'];
      $this->nameatrealm = $this->name . '@' . $this->fullrealm;

      if (!is_array( $this->progression)) {
        $this->progression = unserialize($this->progression);
      }
      if (!is_array( $this->professions)) {
        $this->professions = unserialize($this->professions);
      }

      $this->render = array();
      $this->render['profile'] = sprintf("%s-profilemain.jpg?alt=/wow/static/images/2d/profilemain/race/%d-%d.jpg", check_plain($this->armory_render), $this->raceId, $this->genderId);
      $this->render['inset'] = sprintf("%s-inset.jpg?alt=/wow/static/images/2d/card/inset/%d-%d.jpg", check_plain($this->armory_render), $this->raceId, $this->genderId);
      $this->render['card'] = sprintf("%s-card.jpg?alt=/wow/static/images/2d/card/race/%d-%d.jpg", check_plain($this->armory_render), $this->raceId, $this->genderId);
      $this->render['avatar'] = sprintf("%s-avatar.jpg?alt=/wow/static/images/2d/avatar/%d-%d.jpg", check_plain($this->armory_render), $this->raceId, $this->genderId);


      $query = db_select('wowtoon_cached_sets', 's');
      $query->fields('s');
      $query->condition('s.toon_key', $this->toon_key);
      $this->sets = array();
      $sets = $query->execute();
      $seaid = 1;
      foreach ($sets as $set) {
        $set->stats = unserialize($set->stats);
        $set->items = unserialize($set->items);
        if ($set->active == 1) {
          $this->active_set = $set;
        }
        $this->sets[] = $set;
      }
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Validate that this toon exists in database or on the armory.  Does not save it as an avatar.
   *
   * @param integer $slot_id - If this is passed, check to see if this item at this slot has been equiped/unequiped and return TRUE/FALSE
   * @param bool $equip
   */
  public function validateToon($slot_id = NULL, $equip = NULL) {
    // First check to see if toon is in the database.
    $this->loadToon();


    // See if we are validating this character.
    if ($slot_id !== NULL) {
      // Load from armory
      $this->loadFromArmory();
      if ($equip === TRUE) {
        return !empty($this->active_set->items[$slot_id]->id);
      }
      else {
        return empty($this->active_set->items[$slot_id]->id);
      }
    }

    if ($this->loaded_name) {
      return WOWTOON_UPDATE_CHARACTER_AVAILABLE;
    }
    else {
      return $this->loadFromArmory();
    }
  }

  public function loadFromArmory($allow_header_cache = TRUE) {

    $all_realms = wowguild_get_all_realms();
    $realm_data = $all_realms[$this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']];

    $zone = $realm_data['zone'];
    $realm = $realm_data['name'];

    //$zone = strtolower(substr($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], strlen($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']) - 2));
    //$realm = substr($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], 0, strlen($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']) - 3);


    $language = variable_get('wowguild_default_language', 'en');
    if ($zone == 'us' && ($language == 'fr' || $language == 'de' || $language == 'ru')) {
      $language = 'en';
    }

    if ($zone == 'eu') {
      switch ($language) {
        case 'es': $locale = 'es_ES'; break;
        case 'fr': $locale = 'fr_FR'; break;
        case 'ru': $locale = 'ru_RU'; break;
        case 'de': $locale = 'de_DE'; break;
        default:
        case 'en': $locale = 'en_GB'; break;
      }
    }
    elseif($zone == 'us') {
      switch ($language) {
        case 'es': $locale = 'es_MX'; break;
        default:
        case 'en': $locale = 'en_US'; break;
      }
    }
    $url = sprintf('http://%s/api/wow/character/%s/%s?fields=guild,stats,talents,items,titles,professions,progression,feed%s', $realm_data['server'], $realm_data['slug'], urlencode($this->name), empty($locale)?'':'&locale=' . $locale);
    //$url = sprintf('http://%s.battle.net/api/wow/character/%s/%s?fields=guild,stats,talents,items,titles,professions,progression%s', strtolower($zone), str_replace(array("'", ' '), array('', '%20'), $realm), urlencode($this->name), empty($locale)?'':'&locale=' . $locale);
// print("<pre>");
// print("$url\n");
// print_r($this);
    $headers = array();
    if ($allow_header_cache) {
      // If updated set and updated_attempted is not zero.
      if (!empty($this->updated) && !empty($this->update_attempted) && !isset($_GET['force'])) {
        $headers['If-Modified-Since'] = gmdate(DATE_RFC1123, $this->updated);
      }
    }
    $this->update_attempted = REQUEST_TIME;
    $results = _wowguild_fetch_json($url, $headers);

    _wowguild_armory_hit(array(
        'url' => $url,
        'success' => $results['success'],
        'text' => sprintf('Toon Update: %s@%s', $this->name, $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'])
    ));

    if ($results['success'] == WOWTOON_UPDATE_CHARACTER_AVAILABLE && !empty($results['data'])) {
      $this->zone = $zone;
      $this->realm = $realm;
      $this->url = sprintf("http://%s/wow/%s/character/%s/%s/", $realm_data['server'], $language, str_replace(array("'", ' '), array('', '%20'), $realm), urlencode($this->name));

      $this->parse_toon_results($results['data']);

      // See if toon is cached in the database.
      $query = db_select('wowtoon_cached_data', 'w');
      $query->fields('w', array('toon_key'));
      $query->condition('w.toon_key', $this->toon_key);
      $res = $query->execute()->fetchObject();
      if (!empty($res->toon_key)) {
        $this->added_to_database = REQUEST_TIME;
        drupal_write_record('wowtoon_cached_data', $this, 'toon_key');
      }
      else {
        drupal_write_record('wowtoon_cached_data', $this);
      }
    }
    else {
      db_update('wowtoon_cached_data')
      ->fields(array('update_attempted' => $this->update_attempted))
      ->condition('toon_key', $this->toon_key)
      ->execute();
    }
    $this->update_status = $results['success'];
    return $results['success'];
  }
  /**
   * Takes the results of a Blizzard API call and format for our database.
   *
   * @param String $data
   */
  private function parse_toon_results($data = array()) {

    // Don't accidentally blank name.
    if (empty($data['name'])) {
      die('PARSE_RESULTS: ERROR!');
    }

    $all_realms = wowguild_get_all_realms();
    $realm_data = $all_realms[$this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']];

    $this->name = $data['name'];
    $zone = strtolower($this->zone);
    $this->realm = $data['realm'];

    $language = variable_get('wowguild_default_language', 'en');
    if ($zone == 'us' && ($language == 'fr' || $language == 'de' || $language == 'ru')) {
      $language = 'en';
    }

    if ($zone == 'eu') {
      switch ($language) {
        case 'es': $locale = 'es_ES'; break;
        case 'fr': $locale = 'fr_FR'; break;
        case 'ru': $locale = 'ru_RU'; break;
        case 'de': $locale = 'de_DE'; break;
        default:
        case 'en': $locale = 'en_GB'; break;
      }
    }
    elseif($zone == 'us') {
      switch ($language) {
        case 'es': $locale = 'es_MX'; break;
        default:
        case 'en': $locale = 'en_US'; break;
      }
    }

    $this->loaded_name = $data['name'];
    $this->toon_key = wowguild_getkey($this->realm . '-' . $this->zone, $data['name']);
    // Loop over titles and find $title["selected"] = 1.
    $this->fullname = $this->name;
    foreach ($data['titles'] as $title) {
      if (!empty($title['selected'])) {
        $this->fullname = sprintf($title['name'], $this->name);
      }
    }

    $this->battlegroup = ''; // Not in API.
    $this->guild_name = !empty($data['guild']['name'])?$data['guild']['name']:'';

    // Place character progression in data structure.
    $progression_raids = wowguild_get_progression_raids();
    $progression = $progression_raids;


    for ($i=0;$i<count($progression_raids);$i++) {
      $raid = $progression_raids[$i];

      // Find this raid in my progression
      foreach ($data['progression']['raids'] as $myraid) {
        if ($myraid['id'] == $raid['id']) {
          // Save name if locale is not english.
          $progression[$i]['name'] = $myraid['name'];

          // Loop over bosses.
          $flex_kills = 0;
          $flex_max = 0;
          $normal_kills = 0;
          $normal_max = 0;
          $heroic_kills = 0;
          $heroic_max = 0;
          $mythic_kills = 0;
          $mythic_max = 0;
          for ($j=0;$j<count($raid['encounters']);$j++) {
            // Find this boss in my progression
            foreach ($myraid['bosses'] as $myboss) {
              if ($raid['encounters'][$j]['id'] == $myboss['id']) {
                // Save name if locale is not english.
                $progression[$i]['encounters'][$j]['name'] = $myboss['name'];

                if (isset($myboss['flexKills'])) {
                  if ($myboss['flexKills'] > 0) {
                    $flex_kills++;
                    $progression[$i]['encounters'][$j]['killed'] = 1;
                  }
                  $flex_max++;

                }


                if (array_key_exists('N', $raid['encounters'][$j]['modes'])) {
                  if ($myboss['normalKills'] > 0) {
                    $normal_kills++;
                    $progression[$i]['encounters'][$j]['killed'] = 1;
                  }
                  $normal_max++;
                }
                if (array_key_exists('H', $raid['encounters'][$j]['modes'])) {
                  if ($myboss['heroicKills'] > 0) {
                    $heroic_kills++;
                  }
                  $heroic_max++;
                }
                if (array_key_exists('M', $raid['encounters'][$j]['modes'])) {
                  if ($myboss['mythicKills'] > 0) {
                    $mythic_kills++;
                  }
                  $mythic_max++;
                }
              }
            }
          }

          $progression[$i]['flex_kills'] = $flex_kills;
          $progression[$i]['flex_max'] = $flex_max;
          if ($flex_max > 0 && $flex_kills == $flex_max) {
            $progression[$i]['flex_completed'] = 1;
          }
          else {
            $progression[$i]['flex_completed'] = 0;
          }
          $progression[$i]['normal_kills'] = $normal_kills;
          $progression[$i]['normal_max'] = $normal_max;
          if ($normal_max > 0 && $normal_kills == $normal_max) {
            $progression[$i]['normal_completed'] = 1;
          }
          else {
            $progression[$i]['normal_completed'] = 0;
          }
          if ($heroic_max > 0) {
            $progression[$i]['heroic_kills'] = $heroic_kills;
            $progression[$i]['heroic_max'] = $heroic_max;
            if ($heroic_kills == $heroic_max) {
              $progression[$i]['heroic_completed'] = 1;
            }
            else {
              $progression[$i]['heroic_completed'] = 0;
            }
          }
          if ($mythic_max > 0) {
            $progression[$i]['mythic_kills'] = $mythic_kills;
            $progression[$i]['mythic_max'] = $mythic_max;
            if ($mythic_kills == $mythic_max) {
              $progression[$i]['mythic_completed'] = 1;
            }
            else {
              $progression[$i]['mythic_completed'] = 0;
            }
          }
        }
      }
    }

    $this->progression = $progression;

    // Grab class lookup
    $lookup_url = sprintf('http://%s/api/wow/data/character/classes', $realm_data['server']);
    $lookup = _wowguild_get_blizzard_data_resource_lookup($lookup_url, $locale);
    $this->classId = $data['class'];
    $this->class = array_key_exists($this->classId, $lookup)?$lookup[$this->classId]['name']:$data['class'];

    // Grab race lookup
    $lookup_url = sprintf('http://%s/api/wow/data/character/races', $realm_data['server']);
    $lookup = _wowguild_get_blizzard_data_resource_lookup($lookup_url, $locale);
    $this->raceId = $data['race'];
    $this->race = array_key_exists($this->raceId, $lookup)?$lookup[$this->raceId]['name']:$data['race'];
    $this->faction = array_key_exists($this->raceId, $lookup)?$lookup[$this->raceId]['side']:$data['race'];

    // Hardcode factionId or NULL
    if ($this->faction == 'alliance') {
      $this->factionId = 0;
    }
    if ($this->faction == 'horde') {
      $this->factionId = 1;
    }

    // API not provided, hardcode gender.
    $this->genderId = $data['gender'];
    $this->gender = $this->genderId == 0?'male':'female';

    $this->level = $data['level'];

    $this->achievement_points = $data['achievementPoints'];

    $this->avg_ilevel = $data['items']['averageItemLevel'];
    $this->avg_ilevel_best = $data['items']['averageItemLevelEquipped'];

    $this->race_background = ''; // Not really using this...  Lookup from raceId/genderId?

    $this->armory_render = sprintf('http://%s/static-render/%s/%s', $realm_data['server'], strtolower($this->zone), substr($data['thumbnail'], 0, strpos($data['thumbnail'], '-avatar')));
    $this->render = array();
    $this->render['profile'] = sprintf("%s-profilemain.jpg?alt=/wow/static/images/2d/profilemain/race/%d-%d.jpg", check_plain($this->armory_render), $this->raceId, $this->genderId);
    $this->render['inset'] = sprintf("%s-inset.jpg?alt=/wow/static/images/2d/card/inset/%d-%d.jpg", check_plain($this->armory_render), $this->raceId, $this->genderId);
    $this->render['card'] = sprintf("%s-card.jpg?alt=/wow/static/images/2d/card/race/%d-%d.jpg", check_plain($this->armory_render), $this->raceId, $this->genderId);
    $this->render['avatar'] = sprintf("%s-avatar.jpg?alt=/wow/static/images/2d/avatar/%d-%d.jpg", check_plain($this->armory_render), $this->raceId, $this->genderId);


    $this->lifetimehonorablekills = 0; // Not in API.

    // TODO: create DB for and grab secondary professions
    $this->professions = array();
    foreach ($data['professions']['primary'] as $key => $ele) {
      $this->professions[$key] = array();
      $this->professions[$key]['name'] = $ele['name'];
      $this->professions[$key]['icon'] = $ele['icon'];
      $this->professions[$key]['value'] = $ele['rank'];
      $this->professions[$key]['max'] = $ele['max'];
    }

    // Loop over talents
    $activespec = 0;
    foreach ($data['talents'] as $key => $ele) {
      $specs[$key] = array();
      if(!empty($ele['selected'])) {
        $activespec = $key;
        $specs[$key]['active'] = 1;
      }
      else {
        $specs[$key]['active'] = 0;
      }
      if (!empty($ele['spec']['icon'])) {
        $specs[$key]['icon'] = $ele['spec']['icon'];
        $specs[$key]['name'] = $ele['spec']['name'];
        $specs[$key]['background'] = $ele['spec']['backgroundImage'];
        $specs[$key]['description'] = $ele['spec']['description'];
      }
      else {
        $specs[$key]['icon'] = 'inv_misc_questionmark';
        $specs[$key]['name'] = 'none';
        $specs[$key]['build'] = '';
        $specs[$key]['background'] = '';
        $specs[$key]['description'] ='';
      }
    }

    // Loop over items
    $items = array();
    $items_lookup = array('head' => 0, 'neck' => 1, 'shoulder' => 2, 'back' => 14, 'chest' => 4, 'shirt' => 3,'tabard' => 18,
        'wrist' => 8, 'hands' => 9, 'waist' => 5, 'legs' => 6, 'feet' => 7, 'finger1' => 10, 'finger2' => 11,
        'trinket1' => 12, 'trinket2' => 13, 'mainHand' => 15, 'offHand' => 16, 'ranged' => 17);
    // Create empty item array.
    foreach ($items_lookup as $key => $ele) {
      $items[$ele] = array(
          'slotname'=> $key,
          'data-id'=> $ele
      );
    }
    foreach ($data['items'] as $key => $ele) {
      if (array_key_exists($key, $items_lookup)) {
        $items[$items_lookup[$key]] = $ele;
        $items[$items_lookup[$key]]['slotname'] = $key;
        $items[$items_lookup[$key]]['data-id'] = $items_lookup[$key];
      }
    }
// kpr($data['feed']);
// die();
    // Loop over feed items and parse into feed_item_entity.
    $ach = array();
    foreach ($data['feed'] as $key => $ele) {
      $newobj = array();
      $newobj['dateCompleted'] = $ele['timestamp'] / 1000;
      switch ($ele['type']) {
        case "ACHIEVEMENT":
          $newobj['feed_item_type'] = WOWTOON_FEED_ACHIEVEMENT;
          $newobj['icon'] = wowguild_get_icon_path($this->zone, $ele['achievement']['icon'], 18);
          $newobj['description'] = t("Earned the achievement !achievement_link for @points points.", array('!achievement_link' => l($ele['achievement']['title'], WOWTOON_WOWHEAD_BASE_URL . '/?achievement=' . $ele['achievement']['id']), '@points' => $ele['achievement']['points']));
          break;
        case "BOSSKILL":
          $newobj['feed_item_type'] = WOWTOON_FEED_BOSS_KILL;
          $newobj['icon'] = wowguild_get_icon_path($this->zone, $ele['achievement']['icon'], 18);
          $newobj['description'] = t("@count @boss_kill.", array('@boss_kill' => $ele['achievement']['title'], '@count' => $ele['quantity']));
          break;
        case "CRITERIA":
          $newobj['feed_item_type'] = WOWTOON_FEED_CRITERIA;
          $newobj['description'] = t("Completed step !step of achievement !achievement_link.", array('!achievement_link' => l($ele['achievement']['title'], WOWTOON_WOWHEAD_BASE_URL . '/?achievement=' . $ele['achievement']['id']), '!step' => '<strong>' . $ele['criteria']['description'] . '</strong>'));
          break;
        case "LOOT":
          $newobj['feed_item_type'] = WOWTOON_FEED_OTHER;
          $item = wowguild_fetch_item($ele['itemId']);
          if (!empty($item)) {
            $newobj['description'] = t("Obtained !item_link.", array('!item_link' => l($item['name'], WOWTOON_WOWHEAD_BASE_URL . '/?item=' . $item['id'], array('attributes' => array('class' => array('color-q' . $item['quality']))))));
            $newobj['icon'] = wowguild_get_icon_path($this->zone, $item['icon'], 18);
          }
          break;
      }

      if (!empty($newobj['description'])) {
        $ach[] = $newobj;
      }
    }
    if ($ach) {
      $oldaids = wowtoon_feed_item_load_achids_by_keyid($this->aid, 'toon_feed');
      wowtoon_feed_item_delete_multiple($oldaids);
      foreach ($ach as $newobj) {
        $newobj['type'] = 'toon_feed';
        $newobj['keyid'] = $this->aid;
        $newach = new WoWToonFeedItem($newobj);
        wowtoon_feed_item_save($newach);
      }
    }

    // $data['lastModified'] is in milliseconds.
    $this->updated_on_armory = (int)$data['lastModified'] / 1000;

    $stats = $data['stats'];

    // Load current sets
    $results = db_query('SELECT * FROM {wowtoon_cached_sets} WHERE toon_key = :toon_key', array(':toon_key' => $this->toon_key));
    $sets = array();
    foreach ($results as $set) {
      $sets[$set->set_id] = $set;
    }

    // Check Spec and Sets
    for ($i=0;$i<count($specs);$i++) {
      if (!isset($sets[$i])) {
        $sets[$i] = new StdClass();
      }
      $sets[$i]->toon_key = $this->toon_key;
      $sets[$i]->set_id = $i;
      $sets[$i]->icon = $specs[$i]['icon'];
      //$sets[$i]->spec = sprintf("%s (%s)", $specs[$i]['name'], $specs[$i]['description']);
      $sets[$i]->spec = $specs[$i]['name'];
      $sets[$i]->description = $specs[$i]['description'];
      //$sets[$i]->background = $this->render['profile'];
      $sets[$i]->background = $specs[$i]['background'];

      if ($specs[$i]['active'] == 1) {
        $sets[$i]->active = 1;
        $sets[$i]->stats = $stats;
        $sets[$i]->allstats = '';
        $sets[$i]->items = $items;

        // See if we need to download background image
        /*
         if (variable_get('wowtoon_cache_backgrounds', 0)) {
        $full_profile_image = $toon->armory_render . '-profilemain.jpg'; //sprintf('http://%s.battle.net/static-render/us/%s-profilemain.jpg', $zone, $toon->armory_render);
        $real_background_directory = variable_get('wowtoon_cache_backgrounds_path', 'public://wowtoon_background_cache');
        if (substr($real_background_directory, 0, 9) == 'public://') {
        $url_background_directory = file_create_url($real_background_directory);
        $real_background_directory = drupal_realpath($real_background_directory);
        }
        else {
        $url_background_directory = variable_get('wowtoon_cache_backgrounds_url', file_create_url('public://wowtoon_background_cache'));
        }

        $real_filename = sprintf("%s/toon-%s-%d.jpg", $real_background_directory, base64_encode($toon->toon_key), $i);
        $url_filename = sprintf("%s/toon-%s-%d.jpg", $url_background_directory, base64_encode($toon->toon_key), $i);
        if (!file_exists($real_background_directory)) {
        drupal_mkdir($real_background_directory, NULL, TRUE);
        }
        if (copy($full_profile_image, $real_filename)) {
        $sets[$i]->background = $url_filename;
        }
        else {
        watchdog('error', 'Could not copy background image %from to %to directory', array('%from' => $full_profile_image, '%to' => $real_filename));
        $sets[$i]->background = $full_profile_image;
        }
        }
        */
      }
      else {
        // Unset these so we don't overwrite old values.
        unset($sets[$i]->stats);
        unset($sets[$i]->allstats);
        unset($sets[$i]->items);
        $sets[$i]->active = 0;
      }




      if (isset($set->id)) {
        drupal_write_record('wowtoon_cached_sets', $sets[$i], 'id');
      }
      else {
        drupal_write_record('wowtoon_cached_sets', $sets[$i]);
      }
      if($sets[$i]->active == 1) {
        $this->active_set = $sets[$i];
      }
    }

    $this->sets = $sets;
    $this->updated = REQUEST_TIME;
  }


//   public function reloadActivityFeed() {
//     // This won't work unless toon has been saved once.
//     if (!empty($this->aid)) {
//       // Generate conditional GET headers.
//       $options = array('headers' => array());

//       $options['headers']["User-Agent"] = "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.2) Gecko/20070219 Firefox/2.0.0.2";

//       // Does not seem to work.
//       if (!empty($this->data['feed_updated'])) {
//         $options['headers']['If-Modified-Since'] = gmdate(DATE_RFC1123, $this->data['feed_updated']) .' GMT';
//       }

//       $name = $this->name;
//       $realm = $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'];
//       $zone = strtolower(substr($realm, strlen($realm) - 2));

//       // wowhead uses www, the armory uses en
//       $language = variable_get('wowguild_default_language', 'en');
//       // Verify that this zone is available to the selected language.
//       if ($zone == 'us' && ($language == 'fr' || $language == 'de' || $language == 'ru')) {
//         $language = 'en';
//       }

//       $url = $this->url . 'feed';
//       $result = drupal_http_request($url, $options);

//       switch ($result->code) {
//         case 304: // Page not modified.  Armory does not seem to respect this.
//           return WOWTOON_UPDATE_CHARACTER_NOT_MODIFIED;
//           break;
//         case 500: // Error with Armory
//         case 404: // Page not found error
//         case 200: // Everythings OK!
//         default:  // unknown?
//           try {
//             $qp = qp($result->data, NULL, array('use_parser' => 'xml'));
//             $error = $qp->find('#server-error')->innerHTML();
//             if ($error) {
//               if (strpos($error, 'Character Not Available')) {
//                 return WOWTOON_UPDATE_CHARACTER_NOT_AVAILABLE;
//               }
//               else {
//                 return WOWTOON_UPDATE_INVALID_PAGE;
//               }
//             }
//           } catch (Exception $e) {
//             return WOWTOON_UPDATE_INVALID_PAGE;
//           }


//           $toon_key = wowguild_getkey($realm, $name);

//           $ach = array();

//           $snapshot_time = $this->updated_on_armory;
//           $qp->top()->find('.activity-feed')->children();
//           foreach ($qp as $item) {
//             $newobj = array();
//             $newobj['toon_key'] = $this->toon_key;
//             $class = $item->attr('class');
//             $newobj['class'] = $class;
//             if (strpos($class, 'ach') !== FALSE) {
//               $newobj['feed_item_type'] = WOWTOON_FEED_ACHIEVEMENT;
//               $txt = $item->firstChild()->firstChild()->firstChild()->firstChild()->attr('style');
//               $a = strpos($txt, 'url("') + 5;
//               $b = strpos($txt, '"', $a);
//               $newobj['icon'] = substr($txt, $a, $b - $a);
//               $txt = $item->parent()->parent()->innerHTML();
//               $a = strpos($txt, '</a>') + 5;
//               $txt = trim(substr($txt, $a));
//               $txt = preg_replace(array("/achievement#.*:a/", '/onmouseover=\".*\"/', '/rel=\".*\"/'), array(WOWTOON_WOWHEAD_BASE_URL . "/?achievement=", '', ''), $txt);
//               $newobj['description'] = $txt;
//               $newobj['created'] = $item->next()->innerHTML();
//             }
//             elseif (strpos($class, 'crit') !== FALSE) {
//               $newobj['feed_item_type'] = WOWTOON_FEED_CRITERIA;
//               $newobj['icon'] = '';
//               $txt = $item->firstChild()->firstChild()->innerHTML();
//               $a = strpos($txt, 'Completed');
//               $txt = trim(substr($txt, $a));
//               $txt = preg_replace(array("/achievement#.*:a/", '/onmouseover=\".*\"/', '/rel=\".*\"/'), array(WOWTOON_WOWHEAD_BASE_URL . "/?achievement=", '', ''), $txt);
//               $newobj['description'] = $txt;
//               $newobj['created'] = $item->next()->innerHTML();
//             }
//             elseif (strpos($class, 'bosskill') !== FALSE) {
//               $newobj['feed_item_type'] = WOWTOON_FEED_BOSS_KILL;
//               $newobj['icon'] = '';
//               $txt = $item->firstChild()->firstChild()->innerHTML();
//               $newobj['description'] = trim(strip_tags($txt));
//               $newobj['created'] = $item->next()->innerHTML();
//             }
//             else {
//               $newobj['feed_item_type'] = WOWTOON_FEED_OTHER;
//               $txt = $item->firstChild()->firstChild()->firstChild()->firstChild()->attr('style');
//               $a = strpos($txt, 'url("') + 5;
//               $b = strpos($txt, '"', $a);
//               $newobj['icon'] = substr($txt, $a, $b - $a);
//               $txt = $item->parent()->parent()->innerHTML();
//               $a = strpos($txt, '</a>') + 5;
//               $txt = trim(substr($txt, $a));
//               $newobj['description'] = str_replace('/wow/' . $language . '/item/', WOWTOON_WOWHEAD_BASE_URL . '/' . WOWTOON_WOWHEAD_SEARCH_ID, $txt);
//               $newobj['created'] = $item->next()->innerHTML();
//             }

//             // depending on the format of the country, we can't yet know what represent each $date
//             //we'll have to specify in the switch, to get back to an iso date
//             switch ($language) {
//               case 'fr':
//                 if (strpos($newobj['created'], 'il y a') !== FALSE) {
//                   // replace 'il y a' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj['created'] = preg_replace(
//                       array('/^il y a /', '/jour$/', '/jours$/', '/heure$/', '/heures$/', '/minute$/', '/minutes$/'),
//                       array('', 'day ago', 'days ago', 'hour ago', 'hours ago', 'minute ago', 'minutes ago'),
//                       $newobj['created']
//                   );
//                 }
//                 elseif (strpos($newobj['created'], 'dans') !== FALSE) {
//                   // replace 'dans' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj['created'] = preg_replace(
//                       array('/^dans /', '/jour$/', '/jours$/', '/heure$/', '/heures$/', '/minute$/', '/minutes$/'),
//                       array('+', 'day', 'days', 'hour', 'hours', 'minute', 'minutes'),
//                       $newobj['created']
//                   );
//                 }
//                 else {
//                   // swap DD/MM/YYYY to MM/DD/YYYY and pass to strtotime().
//                   $tab = explode("/", $newobj['created']);
//                   $newobj['created'] = $tab[1] . '/' . $tab[0] . '/' . $tab[2];
//                 }
//                 break;
//               case 'es':
//                 if (strpos($newobj['created'], 'hace') !== FALSE) {
//                   // replace 'il y a' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj['created'] = preg_replace(
//                       array('/^hace /', '/día$/', '/días$/', '/hora$/', '/horas$/', '/minuto$/', '/minutos$/'),
//                       array('', 'day ago', 'days ago', 'hour ago', 'hours ago', 'minute ago', 'minutes ago'),
//                       $newobj['created']
//                   );
//                 }
//                 elseif (strpos($newobj['created'], 'en') !== FALSE) {
//                   // replace 'dans' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj['created'] = preg_replace(
//                       array('/^en /', '/día$/', '/días$/', '/hora$/', '/horas$/', '/minuto$/', '/minutos$/'),
//                       array('+', 'day', 'days', 'hour', 'hours', 'minute', 'minutes'),
//                       $newobj['created']
//                   );
//                 }
//                 else {
//                   // swap DD/MM/YYYY to MM/DD/YYYY and pass to strtotime().
//                   $tab = explode("/", $newobj['created']);
//                   $newobj['created'] = $tab[1] . '/' . $tab[0] . '/' . $tab[2];
//                 }
//                 break;
//               case 'de':
//                 if (strpos($newobj['created'], 'vor') !== FALSE) {
//                   // replace 'il y a' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj['created'] = preg_replace(
//                       array('/^vor /', '/Tag$/', '/Tagen$/', '/Stunde$/', '/Stunden$/', '/Minute$/', '/Minuten$/'),
//                       array('', 'day ago', 'days ago', 'hour ago', 'hours ago', 'minute ago', 'minutes ago'),
//                       $newobj['created']
//                   );
//                 }
//                 elseif (strpos($newobj['created'], 'in') !== FALSE) {
//                   // replace 'dans' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj['created'] = preg_replace(
//                       array('/^in /', '/Tag$/', '/Tagen$/', '/Stunde$/','/Stunden$/','/Minute$/', '/Minuten$/'),
//                       array('+', 'day', 'days', 'hour', 'hours', 'minute', 'minutes'),
//                       $newobj['created']
//                   );
//                 }
//                 else {
//                   // swap DD.MM.YYYY to MM/DD/YYYY and pass to strtotime().
//                   $tab = explode(".", $newobj['created']);
//                   $newobj['created'] = $tab[1] . '/' . $tab[0] . '/' . $tab[2];
//                 }
//                 break;
//               case 'ru':
//                 if (strpos($newobj['created'], 'назад') !== FALSE) {
//                   // replace 'il y a' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj['created'] = preg_replace(
//                       array('/дн/', '/ч/', '/мин/', '/ назад$/'),
//                       array('days ago', 'hours ago', 'minutes ago', ''),
//                       $newobj['created']
//                   );
//                 }
//                 elseif (strpos($newobj['created'], 'через') !== FALSE) {
//                   // replace 'dans' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj['created'] = preg_replace(
//                       array('/^через /', '/дн$/', '/дн.$/', '/ч$/', '/мин$/'),
//                       array('+', 'day', 'days', 'hours', 'minutes'),
//                       $newobj['created']
//                   );
//                 }
//                 else {
//                   // swap DD/MM/YYYY to MM/DD/YYYY and pass to strtotime().
//                   $tab = explode("/", $newobj['created']);
//                   $newobj['created'] = $tab[1]  . '/'.$tab[0] . '/' . $tab[2];
//                 }
//                 break;
//               default:
//               case 'en':
//                 if (strpos($newobj['created'], 'ago') !== FALSE) {
//                   // do nothing, string is in the proper format.
//                 }
//                 elseif (strpos($newobj['created'], 'in') !== FALSE) {
//                   // replace 'in' with '+'
//                   $newobj['created'] = preg_replace(
//                       '/^in /',
//                       '+',
//                       $newobj['created']
//                   );
//                 }
//                 else {
//                   // us.battle.net uses MM/DD/YYYY, eu.battle.net uses DD/MM/YYYY
//                   if (strtolower($zone) == 'eu') {
//                     $tab = explode("/", $newobj['created']);
//                     $newobj['created'] = $tab[1] . '/' . $tab[0] . '/' . $tab[2];
//                   }
//                 }
//                 break;
//             }
//             $newobj['dateCompleted'] = strtotime($newobj['created']);

//             $ach[] = $newobj;
//           }
//           if ($ach) {
//             $oldaids = wowtoon_feed_item_load_achids_by_keyid($this->aid, 'toon_feed');
//             wowtoon_feed_item_delete_multiple($oldaids);
//             foreach ($ach as $newobj) {
//               $newobj['type'] = 'toon_feed';
//               $newobj['keyid'] = $this->aid;
//               $newach = new WoWToonFeedItem($newobj);
//               wowtoon_feed_item_save($newach);
//             }
//           }

//           $this->data['feed_updated'] = REQUEST_TIME;
//           $this->save();

//           /*
//            // TODO: Save old achievements.  Currently $achievement->completed can change, so cannot use that to dertermine duplicate achievements.
//           // Search for latest one?  And insert ones after?
//           $achievements = wowachievement_load_aid($toon->aid);
//           foreach ($achievements as $achievement) {
//           wowachievement_delete($achievement->aid);
//           }

//           foreach ($ach as $newobj) {
//           wowachievement_save($newobj);
//           }
//           //drupal_set_message(t('Updated @name achievements', array('@name' => $name)));
//           }
//           */
//           break;
//       }
//     }
//   }
}
