<?php



/**
 * The class extendes Avatar to add World of Warcraft Functions.
 *
 * @see AvatarController::query()
 */

class WoWGuild extends Entity {
  protected $locale;
  protected $language;

  public function __construct($values = array()) {

    parent::__construct($values, 'wowguild');

    $this->locale = variable_get('wowguild_locale');

    // Set initial values.
    foreach ($values as $key => $value) {
      $this->$key = $value;
    }
    if (!empty($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'])) {
      $this->zone = substr($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], strlen($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']) - 2);
      $this->realm = substr($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], 0, strlen($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']) - 3);
    }
    elseif (!empty($this->zone) && !empty($this->realm)) {
      $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'] = $this->realm . '-' . $this->zone;
    }
    if (!empty($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'])) {
      $this->setGuildKey();
    }
  }

  /**
   *
   * @see Entity::save()
   * @see entity_save()
   */
  public function save() {
    $this->setGuildKey();
    return parent::save();
  }
  /**
   * Set internal variables needed for most operations.
   *
   */
  public function setGuildKey() {
    $this->guild_key = wowguild_getkey($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], $this->name);
    $this->zone = substr($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], strlen($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']) - 2);
    $this->realm = substr($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], 0, strlen($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']) - 3);

    $this->language = variable_get('wowguild_default_language', 'en');
    if ($this->zone == 'us' && ($this->language == 'fr' || $this->language == 'de' || $this->language == 'ru')) {
      $this->language = 'en';
    }

    if ($this->zone == 'eu') {
      switch ($language) {
        case 'es': $this->locale = 'es_ES'; break;
        case 'fr': $this->locale = 'fr_FR'; break;
        case 'ru': $this->locale = 'ru_RU'; break;
        case 'de': $this->locale = 'de_DE'; break;
        default:
        case 'en': $this->locale = 'en_GB'; break;
      }
    }
    elseif($this->zone == 'us') {
      switch ($this->language) {
        case 'es': $this->locale = 'es_MX'; break;
        default:
        case 'en': $this->locale = 'en_US'; break;
      }
    }
  }
  /**
   * Load Toon Data and attach it to $this.  Only grabs from the database.
   */
  public function loadGuild() {
    $this->setGuildKey();

    $query = db_select('wowguild_cached_data', 'w');
    $query->fields('w');
    $query->condition('guild_key', $this->guild_key);
    $result = $query->execute()->fetch();
    if (!empty($result)) {
      foreach ($result as $key => $value) {
        // Don't overwrite existing values.
        if (!isset($this->{$key})) {
          $this->{$key} = $value;
        }
      }
      //$this->loaded_name = $result->name;
      //$this->fullrealm = $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'];

      $this->raid_progression = unserialize($this->raid_progression);
      $this->tabard = unserialize($this->tabard);
      $this->members = unserialize($this->members);
    }
  }

  /**
   * Return the URL to the rendered tabard.  Create if necessary.
   *
   * Much of the code/images sourced from: http://sourceforge.net/p/wowarmoryapi/home/Home/
   *
   */
  public function getTabardUrl() {

    $real_tabard_directory = variable_get('wowguild_tabards_cache_path', 'public://tabardcache');
    if (substr($real_tabard_directory, 0, 9) == 'public://') {
      $url_tabard_directory = file_create_url($real_tabard_directory);
      $real_tabard_directory = drupal_realpath($real_tabard_directory);
    }
    else {
      $url_tabard_directory = variable_get('wowguild_tabards_cache_url', file_create_url('public://tabardcache'));
    }
    if (!file_exists($real_tabard_directory)) {
      drupal_mkdir($real_tabard_directory, NULL, TRUE);
    }

    // Save the image filename as the unique parameters for creating this image.
    $imgfile = sprintf("%03.0f-%s-%03.0f-%s-%s-%s.png",
        $this->tabard['icon'], $this->tabard['iconColor'],
        $this->tabard['border'], $this->tabard['borderColor'],
        $this->tabard['backgroundColor'], $this->faction
    );


    if (!file_exists($real_tabard_directory . '/' . $imgfile)) {
      $width = 215;
      $height = 230;

      $ring = $this->faction;

      $imgOut = imagecreatetruecolor(215, 230);

      $emblemURL = dirname(__FILE__) . "/../images/emblems/emblem_" . sprintf("%02s",$this->tabard['icon']) . ".png";
      $borderURL = dirname(__FILE__) . "/../images/borders/border_" . sprintf("%02s",$this->tabard['border']) . ".png";
      $ringURL = dirname(__FILE__) . "/../images/static/ring-" . $ring . ".png";
      $shadowURL = dirname(__FILE__) . "/../images/static/shadow_00.png";
      $bgURL = dirname(__FILE__) . "/../images/static/bg_00.png";
      $overlayURL = dirname(__FILE__) . "/../images/static/overlay_00.png";
      $hooksURL = dirname(__FILE__) . "/../images/static/hooks.png";
      $levelURL = dirname(__FILE__) . "/../images/static/";

      imagesavealpha($imgOut,true);
      imagealphablending($imgOut, true);
      $trans_colour = imagecolorallocatealpha($imgOut, 0, 0, 0, 127);
      imagefill($imgOut, 0, 0, $trans_colour);

      $ring = imagecreatefrompng($ringURL);
      $ring_size = getimagesize($ringURL);

      $emblem = imagecreatefrompng($emblemURL);
      $emblem_size = getimagesize($emblemURL);
      $emblemcolor = preg_replace('/^ff/i','',$this->tabard['iconColor']);
      $color_r = hexdec(substr($emblemcolor,0,2));
      $color_g = hexdec(substr($emblemcolor,2,2));
      $color_b = hexdec(substr($emblemcolor,4,2));
      if (function_exists('imagelayereffect')) {
        imagelayereffect($emblem, IMG_EFFECT_OVERLAY);
        imagefilledrectangle($emblem,0,0,$emblem_size[0],$emblem_size[1],imagecolorallocatealpha($emblem, $color_r, $color_g, $color_b,0));
      }
      else {
        // Workaround for PHPs without imagecolorallocatealpha function (UBUNTU and other distros with GD not compiled into PHP)
        $this->imageColorize($emblem, $color_r, $color_g, $color_b);
      }

      $border = imagecreatefrompng($borderURL);
      $border_size = getimagesize($borderURL);
      $bordercolor = preg_replace('/^ff/i','',$this->tabard['borderColor']);
      $color_r = hexdec(substr($bordercolor,0,2));
      $color_g = hexdec(substr($bordercolor,2,2));
      $color_b = hexdec(substr($bordercolor,4,2));
      if (function_exists('imagelayereffect')) {
        imagelayereffect($border, IMG_EFFECT_OVERLAY);
        imagefilledrectangle($border,0,0,$border_size[0]+100,$border_size[0]+100,imagecolorallocatealpha($border, $color_r, $color_g, $color_b,0));
      }
      else {
        // Workaround for PHPs without imagecolorallocatealpha function (UBUNTU and other distros with GD not compiled into PHP)
        $this->imageColorize($border, $color_r, $color_g, $color_b);
      }

      $shadow = imagecreatefrompng($shadowURL);

      $bg = imagecreatefrompng($bgURL);
      $bg_size = getimagesize($bgURL);
      $bgcolor = preg_replace('/^ff/i','',$this->tabard['backgroundColor']);
      $color_r = hexdec(substr($bgcolor,0,2));
      $color_g = hexdec(substr($bgcolor,2,2));
      $color_b = hexdec(substr($bgcolor,4,2));
      if (function_exists('imagelayereffect')) {
        imagelayereffect($bg, IMG_EFFECT_OVERLAY);
        imagefilledrectangle($bg,0,0,$bg_size[0]+100,$bg_size[0]+100,imagecolorallocatealpha($bg, $color_r, $color_g, $color_b, 0));
      }
      else {
        // Workaround for PHPs without imagecolorallocatealpha function (UBUNTU and other distros with GD not compiled into PHP)
        $this->imageColorize($bg, $color_r, $color_g, $color_b);
      }

      $overlay = imagecreatefrompng($overlayURL);
      $hooks = imagecreatefrompng($hooksURL);

      $x = 20;
      $y = 23;

      imagecopy($imgOut,$ring,0,0,0,0, $ring_size[0],$ring_size[1]);

      $size = getimagesize($shadowURL);
      imagecopy($imgOut,$shadow,$x,$y,0,0, $size[0],$size[1]);
      imagecopy($imgOut,$bg,$x,$y,0,0, $bg_size[0],$bg_size[1]);
      imagecopy($imgOut,$emblem,$x+17,$y+30,0,0, $emblem_size[0],$emblem_size[1]);
      imagecopy($imgOut,$border,$x+13,$y+15,0,0, $border_size[0],$border_size[1]);

      $size = getimagesize($overlayURL);
      imagecopy($imgOut,$overlay,$x,$y+2,0,0, $size[0],$size[1]);
      $size = getimagesize($hooksURL);
      imagecopy($imgOut,$hooks,$x-2,$y,0,0, $size[0],$size[1]);

      $showlevel = false;
      if ($showlevel) {
        $level = $guild->level;
        if ($level < 10){
          $levelIMG = imagecreatefrompng($levelURL.$level.".png");
        } else {
          $digit[1] = substr($level,0,1);
          $digit[2] = substr($level,1,1);
          $digit1 = imagecreatefrompng($levelURL.$digit[1].".png");
          $digit2 = imagecreatefrompng($levelURL.$digit[2].".png");
          $digitwidth = imagesx($digit1);
          $digitheight = imagesy($digit1);
          $levelIMG = imagecreatetruecolor($digitwidth*2,$digitheight);
          $trans_colour = imagecolorallocatealpha($levelIMG, 0, 0, 0, 127);
          imagefill($levelIMG, 0, 0, $trans_colour);
          imagesavealpha($levelIMG,true);
          imagealphablending($levelIMG, true);
          // Last image added first because of the shadow need to be behind first digit
          imagecopy($levelIMG,$digit2,$digitwidth-12,0,0,0, $digitwidth, $digitheight);
          imagecopy($levelIMG,$digit1,12,0,0,0, $digitwidth, $digitheight);
        }
        $size[0] = imagesx($levelIMG);
        $size[1] = imagesy($levelIMG);
        $levelemblem = imagecreatefrompng($ringURL);
        imagesavealpha($levelemblem,true);
        imagealphablending($levelemblem, true);
        imagecopy($levelemblem,$levelIMG,(215/2)-($size[0]/2),(215/2)-($size[1]/2),0,0,$size[0],$size[1]);
        imagecopyresampled($imgOut, $levelemblem, 143, 150,0,0, 215/3, 215/3, 215, 215);
      }

      if ($width > 1 && $width < 215) {
        imagecopyresampled($finalimg, $imgOut, 0, 0, 0, 0, $width, $height, 215, 230);
      }
      else {
        $finalimg = $imgOut;
      }

      imagepng($finalimg, $real_tabard_directory . '/' . $imgfile);

      imagedestroy($imgOut);
      imagedestroy($ring);
      imagedestroy($shadow);
      imagedestroy($bg);
      imagedestroy($overlay);
      imagedestroy($border);
      imagedestroy($emblem);
      imagedestroy($hooks);
    }
    return $url_tabard_directory . '/' . $imgfile;

  }

  /**
   * Loop over image, and colorize non transparent pixels.
   *
   * Simulates the following two lines:  (see above)
   *   imagelayereffect($im, IMG_EFFECT_OVERLAY);
   *   imagefilledrectangle($im,0,0,$emblem_size[0],$emblem_size[1],imagecolorallocatealpha($im, $color_r, $color_g, $color_b,0));
   *
   * Source: http://drupal.org/node/1154970
   *
   * @param Image $im
   * @param int $r_overlay
   * @param int $g_overlay
   * @param int $b_overlay
   */
  private function imageColorize(&$im, $r_overlay, $g_overlay, $b_overlay) {
    $height = imagesy($im);
    $width = imagesx($im);

    for($y=0;$y<$height;$y++) {
      for($x=0;$x<$width;$x++) {
        $rgb = imagecolorat($im, $x, $y);
        $alpha = ($rgb >> 24) & 0xFF;
        $r_source = ($rgb >> 16) & 0xFF;
        $g_source = ($rgb >> 8) & 0xFF;
        $b_source = $rgb & 0xFF;

        // Tweak this number if overlay looks weird.  (0 = Fully transparent, 127 = No transparancy)
        if ($alpha < 50) {
          if($r_source <= 128) {
            $final_r = (2 * $r_source * $r_overlay) / 256;
          }
          else {
            $final_r = 255 - (((255 - (2 * ($r_source - 128))) * (255 - $r_overlay)) / 256);
          }
          if($g_source <= 128) {
            $final_g = (2 * $g_source * $g_overlay) / 256;
          }
          else {
            $final_g = 255 - (((255 - (2 * ($g_source - 128))) * (255 - $g_overlay)) / 256);
          }
          if ($b_source <= 128) {
            $final_b = (2 * $b_source * $b_overlay) / 256;
          }
          else {
            $final_b = 255 - (((255 - (2 * ($b_source - 128))) * (255 - $b_overlay)) / 256);
          }
          $final_colour = imagecolorallocate($im, $final_r, $final_g, $final_b);
          imagesetpixel($im, $x, $y, $final_colour);
        }
      }
    }
  }

  /**
   * Used during guild validation.  Checks the database then armory to see if a guild exists.
   *
   **/
  public function validateGuild() {
    // First check to see if guild is in the database.
    if (!wowguild_is_valid_realm($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'])) {
      $this->update_status = WOWTOON_UPDATE_INVALID_REALM;
      return WOWTOON_UPDATE_INVALID_REALM;
    }
    $this->loadGuild();


    if (isset($this->level)) {
      $this->update_status = WOWTOON_UPDATE_CHARACTER_AVAILABLE;
      return WOWTOON_UPDATE_CHARACTER_AVAILABLE;
    }
    else {
      return $this->loadFromArmory();
    }
  }

  /**
   * Loads information from the armory and saves cached_data.
   *
   */
  public function loadFromArmory($allow_header_cache = TRUE) {
    $this->setGuildKey();

    $all_realms = wowguild_get_all_realms();
    $realm_data = $all_realms[$this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']];

    $url = sprintf('http://%s/api/wow/guild/%s/%s?fields=achievements,members,news%s', $realm_data['server'], $realm_data['slug'], str_replace(' ', '%20', $this->name), empty($this->locale)?'':'&locale=' . $this->locale);
    //$url = sprintf('http://%s.battle.net/api/wow/guild/%s/%s?fields=achievements,members%s', strtolower($this->zone), str_replace(array("'", ' '), array('', '%20'), $this->realm), str_replace(' ', '%20', $this->name), empty($this->locale)?'':'&locale=' . $this->locale);
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
        'text' => sprintf('Guild Update: %s@%s', $this->name, $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'])
    ));

    if ($results['success'] == WOWTOON_UPDATE_CHARACTER_AVAILABLE) {
      $this->url = sprintf("http://%s.battle.net/wow/%s/guild/%s/%s/", $this->zone, $this->language, str_replace(array("'", ' '), array('', '%20'), $this->realm), str_replace(' ', '%20', $this->name));
      $this->parse_results($results['data']);

      // See if guild is cached in the database.
      $query = db_select('wowguild_cached_data', 'w');
      $query->fields('w', array('guild_key'));
      $query->condition('w.guild_key', $this->guild_key);
      $res = $query->execute()->fetchObject();
      $this->update_attempted = REQUEST_TIME;
      if (!empty($res->guild_key)) {
        $this->added_to_database = REQUEST_TIME;
        drupal_write_record('wowguild_cached_data', $this, 'guild_key');
      }
      else {
        drupal_write_record('wowguild_cached_data', $this);
      }
    }
    $this->update_status = $results['success'];
    return $results['success'];
  }
  /**
   * Apply ->members to WoWToon Entities.
   *
   * @param integer $index - If $index is passed, then just load one member.  Used to drupal batch processing.
   */
  public function reloadMembers($index = -999) {
    if ($index == -999) {
      $members = $this->members;
    }
    else {
      $members = array($this->members[$index]);
    }

    // Load existing WoWToons
    $query = db_select('wowtoon_cached_data', 'w');
    $query->fields('w', array('toon_key', 'guild_rank'));
    $query->leftJoin('wowtoon_owners', 'o', 'o.toon_key = w.toon_key');
    $query->fields('o');
    $query->leftJoin('avatar', 't', 't.aid = o.aid');
    $query->fields('t', array('gid'));
    if ($index != -999) {
      //$toon_key = wowguild_getkey($this->field_wowtoon_realm[LANGUAGE_NONE][0]['value'], $this->members[$index]['character']['name']);
      $toon_key = wowguild_getkey($this->members[$index]['character']['realm'], $this->members[$index]['character']['name']);
      $query->condition('w.toon_key', $toon_key);
    }
    $results = $query->execute()->fetchAllAssoc('toon_key');

    $updates = array();
    $inserts = array();
    $toon_keys = array();
    $aids = array();
    foreach ($members as $member) {
      // We cannot load character less than level 10, so ignore them.
      if ($member['character']['level'] >= 10) {
        $toon_key = wowguild_getkey($member['character']['realm'] . '-' . $this->zone, $member['character']['name']);
        $toon_keys[] = $toon_key;
        // If Toon exists in database, update guild_rank and level.
        if (array_key_exists($toon_key, $results)) {
          $updates[$toon_key] = array(
              'guild_rank' => $member['rank'],
              'level' => $member['character']['level'],
              'realm' => $member['character']['realm']
          );

          // If Toon had not specifically been added or removed by the guild admin, set to in guild.
          if ($results[$toon_key]->gid >= 0) {
            $updates[$toon_key]['aid'] = $results[$toon_key]->aid;
            $updates[$toon_key]['gid'] = $this->gid;
          }

          $aids[] = $results[$toon_key]->aid;
        }
        else {
          //$url = sprintf("http://%s.battle.net/wow/%s/character/%s/%s/", $this->zone, $this->language, str_replace(array("'", ' '), array('', '%20'), $this->realm), urlencode($this->name));
          $url = sprintf("http://%s.battle.net/wow/%s/character/%s/%s/", $this->zone, $this->language, str_replace(array("'", ' '), array('', '%20'), $member['character']['realm']), urlencode($member['character']['name']));
          $inserts[$toon_key] = array(
              'loaded_name' => $member['character']['name'],
              'type' => 'wowtoon',
              'realm' => $member['character']['realm'],
              'zone' => $this->zone,
              'url' => $url,
              'field_wowtoon_realm' => array(LANGUAGE_NONE => array(0 => array('value' => $member['character']['realm'] . '-' . $this->zone))),
              'gid' => $this->gid,
              'toon_key' => $toon_key,
              'guild_rank' => $member['rank'],
              'classId' => $member['character']['class'],
              'genderId' => $member['character']['gender'],
              'raceId' => $member['character']['race'],
              'level' => $member['character']['level'],
              'armory_render' => sprintf("http://%s.battle.net/static-render/%s/%s", strtolower($this->zone), strtolower($this->zone), substr($member['character']['thumbnail'], 0, strpos($member['character']['thumbnail'], '-avatar.jpg')))
          );
        }
      }
    }

    if (!empty($updates)) {
      foreach($updates as $toon_key => $update) {
        if (isset($update['gid'])) {
          db_update('avatar')
          ->fields(array('gid' => $update['gid']))
          ->condition('aid', $update['aid'])
          ->execute();
          unset($update['aid']);
          unset($update['gid']);
        }

        db_update('wowtoon_cached_data')
        ->fields($update)
        ->condition('toon_key', $toon_key)
        ->execute();
      }
    }

    if (!empty($inserts)) {
      foreach($inserts as $insert) {
        $wowtoon = avatar_create($insert);//new WoWToon($insert, 'avatar');
        avatar_save($wowtoon);

        $aids[] = $wowtoon->aid;

        // If we are loading one toon [usually from a drupal batch], load from armory, else insert what we know.
        if ($index == -999) {
          unset($insert['name']);
          unset($insert['gid']);
          unset($insert['type']);
          unset($insert['field_wowtoon_realm']);
          db_insert('wowtoon_cached_data')
          ->fields($insert)
          ->execute();
        }
        else {
          // Load from Armory
          $wowtoon->validateToon();
        }
      }
    }
    // If we are updating all, UNSET gid for non guild members.
    if ($index == -999 && !empty($aids)) {
      $query = db_update('avatar');
      $query->fields(array('gid' => 0));
      $query->condition('gid', $this->gid);
      $query->condition('aid', $aids, 'NOT IN');
      $query->execute();
    }
  }

  /**
   * Takes the results of a Blizzard API call and format for our database.
   *
   * @param String $data
   */
  private function parse_results($data = array()) {
    $this->setGuildKey();

    $all_realms = wowguild_get_all_realms();
    $realm_data = $all_realms[$this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']];


    $this->loaded_name = $data['name'];
    $this->factionId = $data['side'];
    // Grab race lookup to get alliance / name text.
    $lookup_url = sprintf('http://%s/api/wow/data/character/races', $realm_data['server']);
    $lookup = _wowguild_get_blizzard_data_resource_lookup($lookup_url, $this->locale);
    $this->factionId = $data['side'];
    if ($this->factionId == 0) {
      // 1 = human
      $this->faction = $lookup[1]['side'];
    }
    if ($this->faction == 1) {
      // 10 = blood elf
      $this->faction = $lookup[10]['side'];
    }
    $this->level = $data['level'];
    $this->achievementPoints = $data['achievementPoints'];

    $this->battlegroup = ''; //$data['battlegroup']; // Not supplied by API yet.
    $this->tabard = $data['emblem'];
    $this->members = $data['members'];

    // Grab current progression raid info.
    $progression_raids = wowguild_get_progression_raids();
    $progression = $progression_raids;

    for ($i=0;$i<count($progression_raids);$i++) {
      $raid = $progression_raids[$i];

      // See if we've completed this raid normal mode.
      if (in_array($raid['guild_achid'], $data['achievements']['achievementsCompleted'])) {
        $progression[$i]['normal_completed'] = 1;
      }
      else {
        $progression[$i]['normal_completed'] = 0;
      }

      if (in_array($raid['guild_mythic_achid'], $data['achievements']['achievementsCompleted'])) {
        $progression[$i]['mythic_completed'] = 1;
      }
      else {
        $progression[$i]['mythic_completed'] = 0;
      }
    }

    $this->raid_progression = $progression;

    // Load existing members if they exist.
    $query = db_select('avatar', 'a');
    $query->fields('a', array('aid', 'name'));
    $query->condition('type', 'wowtoon');
    $query->condition('gid', $this->gid);
    $query->join('wowtoon_owners', 'o', 'a.aid = o.aid');
    $query->join('wowtoon_cached_data', 't', 'o.toon_key = t.toon_key');
    $query->fields('t', array('classId'));
    $results = $query->execute();
    $namelookup = array();
    foreach ($results as $member) {
      $namelookup[$member->name] = $member;
    }

    // Are we using popups?
    $avatar_type = avatar_get_types('wowtoon');
    $usepopup = (is_array($avatar_type->data) && array_key_exists('use_popup', $avatar_type->data))?$avatar_type->data['use_popup']:1;
    if ($usepopup) {
      $popup_class = 'avatar-popup';
    }
    else {
      $popup_class = '';
    }

    // Loop over feed items and parse into feed_item_entity.
    $ach = array();
    foreach ($data['news'] as $key => $ele) {
      $newobj = array();
      $newobj['temp'] = $ele['type'];
      $newobj['dateCompleted'] = $ele['timestamp'] / 1000;

      switch ($ele['type']) {
        case "guildCreated":
          $newobj['feed_item_type'] = WOWTOON_FEED_GUILD_CREATED;
          $newobj['description'] = t("Guild created.");
          break;
        case "guildLevel":
          $newobj['feed_item_type'] = WOWTOON_FEED_GUILD_LEVEL_UP;
          $newobj['description'] = t("Guild reached level @level.", array('@level' => $ele['levelUp']));
          break;
        case "guildAchievement":
          $newobj['feed_item_type'] = WOWTOON_FEED_GUILD_ACHIEVEMENT;
          $newobj['icon'] = wowguild_get_icon_path($this->zone, $ele['achievement']['icon'], 18);
          $newobj['description'] = t("The guild earned the achievement !achievement_link for @points.", array("!achievement_link" => l($ele['achievement']['title'], WOWTOON_WOWHEAD_BASE_URL . '/?achievement=' . $ele['achievement']['id']), '@points' => $ele['achievement']['points']));
          break;
        case "playerAchievement":
          $newobj['feed_item_type'] = WOWTOON_FEED_GUILD_PLAYER_ACHIEVEMENT;
          $member = $ele['character'];
          if (array_key_exists($ele['character'], $namelookup)) {
            $a = avatar_load($namelookup[$ele['character']]->aid);
            $path = avatar_uri($a);
            $member = l($member, $path['path'], array("attributes" => array("rel" => $a->aid, "class" => array($popup_class, "shadow", "color-c" . $a->classId))));
          }
          $newobj['icon'] = wowguild_get_icon_path($this->zone, $ele['achievement']['icon'], 18);
          $newobj['description'] = t("!player earned the achievement !achievement_link for @points.", array('!player' => $member, '!achievement_link' => l($ele['achievement']['title'], WOWTOON_WOWHEAD_BASE_URL . '/?achievement=' . $ele['achievement']['id']), '@points' => $ele['achievement']['points']));
          break;
        case "itemCraft":
          $newobj['feed_item_type'] = WOWTOON_FEED_GUILD_ITEM_CRAFTED;
          $item = wowguild_fetch_item($ele['itemId'], $this->zone, $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']);
          if (!empty($item)) {
            $newobj['icon'] = wowguild_get_icon_path($this->zone, $item['icon'], 18);
            $member = $ele['character'];
            if (array_key_exists($ele['character'], $namelookup)) {
              $a = avatar_load($namelookup[$ele['character']]->aid);
              $path = avatar_uri($a);
              $member = l($member, $path['path'], array("attributes" => array("rel" => $a->aid, "class" => array($popup_class, "shadow", "color-c" . $a->classId))));
            }
            $newobj['description'] = t("!player crafted !item_link.", array('!item_link' => l($item['name'], WOWTOON_WOWHEAD_BASE_URL . '/?item=' . $item['id'], array('attributes' => array('class' => array('color-q' . $item['quality'])))), '!player' => $member));
          }
          break;
        case "itemPurchase":
          $newobj['feed_item_type'] = WOWTOON_FEED_GUILD_ITEM_PURCHASED;
          $item = wowguild_fetch_item($ele['itemId'], $this->zone, $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']);
          if (!empty($item)) {
            $newobj['icon'] = wowguild_get_icon_path($this->zone, $item['icon'], 18);
            $member = $ele['character'];
            if (array_key_exists($ele['character'], $namelookup)) {
              $a = avatar_load($namelookup[$ele['character']]->aid);
              $path = avatar_uri($a);
              $member = l($member, $path['path'], array("attributes" => array("rel" => $a->aid, "class" => array($popup_class, "shadow", "color-c" . $a->classId))));
            }
            $newobj['description'] = t("!player purchased !item_link.", array('!item_link' => l($item['name'], WOWTOON_WOWHEAD_BASE_URL . '/?item=' . $item['id'], array('attributes' => array('class' => array('color-q' . $item['quality'])))), '!player' => $member));
          }
          break;
        case "itemLoot":
          $newobj['feed_item_type'] = WOWTOON_FEED_GUILD_ITEM_LOOTED;
          $item = wowguild_fetch_item($ele['itemId'], $this->zone, $this->field_wowtoon_realm[LANGUAGE_NONE][0]['value']);
          if (!empty($item)) {
            $newobj['icon'] = wowguild_get_icon_path($this->zone, $item['icon'], 18);
            $member = $ele['character'];
            if (array_key_exists($ele['character'], $namelookup)) {
              $a = avatar_load($namelookup[$ele['character']]->aid);
              $path = avatar_uri($a);
              $member = l($member, $path['path'], array("attributes" => array("rel" => $a->aid, "class" => array($popup_class, "shadow", "color-c" . $a->classId))));
            }
            $newobj['description'] = t("!player looted !item_link.", array('!item_link' => l($item['name'], WOWTOON_WOWHEAD_BASE_URL . '/?item=' . $item['id'], array('attributes' => array('class' => array('color-q' . $item['quality'])))), '!player' => $member));
          }
          break;
      }

      if (!empty($newobj['description'])) {
        $ach[] = $newobj;
      }
    }
    if ($ach) {
      $oldaids = wowtoon_feed_item_load_achids_by_keyid($this->gid, 'guild_feed');
      wowtoon_feed_item_delete_multiple($oldaids);
      foreach ($ach as $newobj) {
        $newobj['type'] = 'guild_feed';
        $newobj['keyid'] = $this->gid;
        $newach = new WoWToonFeedItem($newobj);
        wowtoon_feed_item_save($newach);
      }
    }

    $this->updated = REQUEST_TIME;

  }


  /**
   * Load the guild activity feed from armory (NO API Available!)
   * Save as WoWToonFeedItem.
   *
   */
//   public function reloadActivityFeed() {
//     // This won't work unless guild has been saved once.
//     if (!empty($this->gid)) {
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

//       $url = $this->url . 'news';
//       $result = drupal_http_request($url, $options);

//       switch ($result->code) {
//         case 304: // Page not modified.  Armory does not seem to respect this.
//           return WOWTOON_UPDATE_CHARACTER_NOT_MODIFIED;
//           break;
//         case 500: // Error with Armory
//         case 404: // Page not found error
//         case 200: // Everythings OK!
//         default:  // unknown?
//           $namelookup = array();
//           /*
//            $query = new EntityFieldQuery();
//           $query->entityCondition('entity_type', 'avatar');
//           $query->entityCondition('bundle', 'wowtoon');
//           //->propertyCondition('status', 1)
//           //$query->fieldCondition('gid', 'value', $this->gid, '=');
//           $results = $query->execute();
//           */

//           $query = db_select('avatar', 'a');
//           $query->fields('a', array('aid', 'name'));
//           $query->condition('type', 'wowtoon');
//           $query->condition('gid', $this->gid);
//           $query->join('wowtoon_owners', 'o', 'a.aid = o.aid');
//           $query->join('wowtoon_cached_data', 't', 'o.toon_key = t.toon_key');
//           $query->fields('t', array('classId'));
//           $results = $query->execute();
//           foreach ($results as $member) {
//             $namelookup[$member->name] = $member;
//           }

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

//           $avatar_type = avatar_get_types('wowtoon');
//           $usepopup = (is_array($avatar_type->data) && array_key_exists('use_popup', $avatar_type->data))?$avatar_type->data['use_popup']:1;
//           if ($usepopup) {
//             $popup_class = 'avatar-popup';
//           }
//           else {
//             $popup_class = '';
//           }


//           $ach = array();

//           $qp->top()->find('.activity-feed')->find('a');
//           foreach ($qp as $link) {
//             $link->attr('rel', '');
//             if (strpos($link->attr('href'), 'achievement#') !== FALSE) {
//               $a = strpos($link->attr('href'), ':a') + 2;
//               $txt = trim(substr($link->attr('href'), $a));
//               $link->attr('href', WOWTOON_WOWHEAD_BASE_URL . "/?achievement=" . $txt);
//             }
//             elseif (strpos($link->attr('href'), '/item/') !== FALSE) {
//               $link->attr('href', WOWTOON_WOWHEAD_BASE_URL . "/?item=" . array_pop(explode('/', $link->attr('href'))));
//             }
//             elseif (strpos($link->attr('href'), '/character/') !== FALSE) {
//               if (array_key_exists($link->innerHTML(), $namelookup)) {
//                 $toon = $namelookup[$link->innerHTML()];
//                 // If we are doing first update, we might not know classId.
//                 if (isset($toon->classId)) {
//                   $link->attr('class', $popup_class . ' shadow color-c' . $toon->classId);
//                 }
//                 else {
//                   $link->attr('class', $popup_class);
//                 }
//                 $link->attr('href', url('avatar/' . $toon->aid));
//                 if ($usepopup) {
//                   $link->attr('rel', $toon->aid);
//                 }
//               }
//             }
//             $link->attr('onmouseover', '');
//           }

//           $qp->top(); $qp->find('.activity-feed')->children();
//           foreach ($qp as $item) {
//             unset($newobj);
//             $newobj->keyid = $this->gid;
//             $class = $item->attr('class');
//             $newobj->class = $class;
//             if (strpos($class, 'guild-levelup') !== FALSE) {
//               $newobj->type = WOWTOON_FEED_GUILD_LEVEL_UP;
//             }
//             elseif (strpos($class, 'player-ach') !== FALSE) {
//               $newobj->type = WOWTOON_FEED_GUILD_PLAYER_ACHIEVEMENT;
//             }
//             elseif (strpos($class, 'guild-ach') !== FALSE) {
//               $newobj->type = WOWTOON_FEED_GUILD_ACHIEVEMENT;
//             }
//             elseif (strpos($class, 'item-crafted') !== FALSE) {
//               $newobj->type = WOWTOON_FEED_GUILD_ITEM_CRAFTED;
//             }
//             elseif (strpos($class, 'item-purchased') !== FALSE) {
//               $newobj->type = WOWTOON_FEED_GUILD_ITEM_PURCHASED;
//             }
//             elseif (strpos($class, 'item-looted') !== FALSE) {
//               $newobj->type = WOWTOON_FEED_GUILD_ITEM_LOOTED;
//             }

//             switch ($newobj->type) {
//               case WOWTOON_FEED_GUILD_LEVEL_UP:
//                 $newobj->icon = '';
//                 $txt = strip_tags($item->firstChild()->firstChild()->innerHTML());
//                 break;
//               case WOWTOON_FEED_GUILD_PLAYER_ACHIEVEMENT:
//               case WOWTOON_FEED_GUILD_ACHIEVEMENT:
//                 $txt = $item->firstChild()->firstChild()->firstChild()->firstChild()->attr('style');
//                 $a = strpos($txt, 'url("') + 5;
//                 $b = strpos($txt, '"', $a);
//                 $newobj->icon = substr($txt, $a, $b - $a);
//                 $txt = $item->parent()->parent()->innerHTML();
//                 $a = strpos($txt, '</a>') + 5;
//                 $txt = trim(substr($txt, $a));
//                 break;
//               default:
//                 $txt = $item->firstChild()->firstChild()->firstChild()->firstChild()->attr('style');
//                 $a = strpos($txt, 'url("') + 5;
//                 $b = strpos($txt, '"', $a);
//                 $newobj->icon = substr($txt, $a, $b - $a);

//                 $txt = $item->parent()->parent()->innerHTML();
//                 $a = strpos($txt, '</a>') + 5;
//                 $txt = trim(substr($txt, $a));
//             }
//             /*
//              if (!isset($newobj->description)) {
//             $tmp = qp($txt);
//             $tmp->top()->find('p');
//             if ($tmp->html()) {
//             $newobj->description = trim($tmp->innerHTML());
//             } else {
//             $tmp->top()->find('body');
//             $newobj->description = trim($tmp->innerHTML());
//             }
//             }*/
//             $newobj->description = $txt;

//             $newobj->dateCompleted = $item->next()->innerHTML();

//             // depending on the format of the country, we can't yet know what represent each $date
//             //we'll have to specify in the switch, to get back to an iso date
//             switch ($language) {
//               case 'fr':
//                 if (strpos($newobj->dateCompleted, 'il y a') !== FALSE) {
//                   // replace 'il y a' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj->dateCompleted = preg_replace(
//                       array('/^il y a /', '/jour$/', '/jours$/', '/heure$/', '/heures$/', '/minute$/', '/minutes$/'),
//                       array('', 'day ago', 'days ago', 'hour ago', 'hours ago', 'minute ago', 'minutes ago'),
//                       $newobj->dateCompleted
//                   );
//                 }
//                 elseif (strpos($newobj->dateCompleted, 'dans') !== FALSE) {
//                   // replace 'dans' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj->dateCompleted = preg_replace(
//                       array('/^dans /', '/jour$/', '/jours$/', '/heure$/', '/heures$/', '/minute$/', '/minutes$/'),
//                       array('+', 'day', 'days', 'hour', 'hours', 'minute', 'minutes'),
//                       $newobj->dateCompleted
//                   );
//                 }
//                 else {
//                   // swap DD/MM/YYYY to MM/DD/YYYY and pass to strtotime().
//                   $tab = explode("/", $newobj->dateCompleted);
//                   $newobj->dateCompleted = $tab[1] . '/' . $tab[0] . '/' . $tab[2];
//                 }
//                 break;
//               case 'es':
//                 if (strpos($newobj->dateCompleted, 'hace') !== FALSE) {
//                   // replace 'il y a' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj->dateCompleted = preg_replace(
//                       array('/^hace /', '/día$/', '/días$/', '/hora$/', '/horas$/', '/minuto$/', '/minutos$/'),
//                       array('', 'day ago', 'days ago', 'hour ago', 'hours ago', 'minute ago', 'minutes ago'),
//                       $newobj->dateCompleted
//                   );
//                 }
//                 elseif (strpos($newobj->dateCompleted, 'en') !== FALSE) {
//                   // replace 'dans' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj->dateCompleted = preg_replace(
//                       array('/^en /', '/día$/', '/días$/', '/hora$/', '/horas$/', '/minuto$/', '/minutos$/'),
//                       array('+', 'day', 'days', 'hour', 'hours', 'minute', 'minutes'),
//                       $newobj->dateCompleted
//                   );
//                 }
//                 else {
//                   // swap DD/MM/YYYY to MM/DD/YYYY and pass to strtotime().
//                   $tab = explode("/", $newobj->dateCompleted);
//                   $newobj->dateCompleted = $tab[1] . '/' . $tab[0] . '/' . $tab[2];
//                 }
//                 break;
//               case 'de':
//                 if (strpos($newobj->dateCompleted, 'vor') !== FALSE) {
//                   // replace 'il y a' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj->dateCompleted = preg_replace(
//                       array('/^vor /', '/Tag$/', '/Tagen$/', '/Stunde$/', '/Stunden$/', '/Minute$/', '/Minuten$/'),
//                       array('', 'day ago', 'days ago', 'hour ago', 'hours ago', 'minute ago', 'minutes ago'),
//                       $newobj->dateCompleted
//                   );
//                 }
//                 elseif (strpos($newobj->dateCompleted, 'in') !== FALSE) {
//                   // replace 'dans' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj->dateCompleted = preg_replace(
//                       array('/^in /', '/Tag$/', '/Tagen$/', '/Stunde$/','/Stunden$/','/Minute$/', '/Minuten$/'),
//                       array('+', 'day', 'days', 'hour', 'hours', 'minute', 'minutes'),
//                       $newobj->dateCompleted
//                   );
//                 }
//                 else {
//                   // swap DD.MM.YYYY to MM/DD/YYYY and pass to strtotime().
//                   $tab = explode(".", $newobj->dateCompleted);
//                   $newobj->dateCompleted = $tab[1] . '/' . $tab[0] . '/' . $tab[2];
//                 }
//                 break;
//               case 'ru':
//                 if (strpos($newobj->dateCompleted, 'назад') !== FALSE) {
//                   // replace 'il y a' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj->dateCompleted = preg_replace(
//                       array('/дн/', '/ч/', '/мин/', '/ назад$/'),
//                       array('days ago', 'hours ago', 'minutes ago', ''),
//                       $newobj->dateCompleted
//                   );
//                 }
//                 elseif (strpos($newobj->dateCompleted, 'через') !== FALSE) {
//                   // replace 'dans' with '',and jours with days ago, heures with hours ago...  etc.
//                   $newobj->dateCompleted = preg_replace(
//                       array('/^через /', '/дн$/', '/дн.$/', '/ч$/', '/мин$/'),
//                       array('+', 'day', 'days', 'hours', 'minutes'),
//                       $newobj->dateCompleted
//                   );
//                 }
//                 else {
//                   // swap DD/MM/YYYY to MM/DD/YYYY and pass to strtotime().
//                   $tab = explode("/", $newobj->dateCompleted);
//                   $newobj->dateCompleted = $tab[1]  . '/'.$tab[0] . '/' . $tab[2];
//                 }
//                 break;
//               default:
//               case 'en':
//                 if (strpos($newobj->dateCompleted, 'ago') !== FALSE) {
//                   // do nothing, string is in the proper format.
//                 }
//                 elseif (strpos($newobj->dateCompleted, 'in') !== FALSE) {
//                   // replace 'in' with '+'
//                   $newobj->dateCompleted = preg_replace(
//                       '/^in /',
//                       '+',
//                       $newobj->dateCompleted
//                   );
//                 }
//                 else {
//                   // us.battle.net uses MM/DD/YYYY, eu.battle.net uses DD/MM/YYYY
//                   if (strtolower($zone) == 'eu') {
//                     $tab = explode("/", $newobj->dateCompleted);
//                     $newobj->dateCompleted = $tab[1] . '/' . $tab[0] . '/' . $tab[2];
//                   }
//                 }
//                 break;
//             } // END: Lnaguage Case

//             if (strpos($newobj->dateCompleted, 'ago') !== FALSE) {
//               $newobj->dateCompleted = strtotime($newobj->dateCompleted, REQUEST_TIME);
//             }
//             else {
//               $newobj->dateCompleted = strtotime($newobj->dateCompleted);
//             }
//             $ach[] = $newobj;
//           }

//           if ($ach) {
//             $oldaids = wowtoon_feed_item_load_achids_by_keyid($this->gid, 'guild_feed');
//             wowtoon_feed_item_delete_multiple($oldaids);
//             foreach ($ach as $newobj) {
//               $newobj->type = 'guild_feed';
//               $newobj->keyid = $this->gid;
//               //$newach = new WoWToonFeedItem($newobj);
//               $newach = wowtoon_feed_item_create((array)$newobj);
//               wowtoon_feed_item_save($newach);
//             }
//           }

//           $this->data['feed_updated'] = REQUEST_TIME;
//           $this->save();
//           break;
//       } // END: Result Code Case
//     }
//   }

  protected function defaultLabel() {
    return $this->name;
  }

  protected function defaultUri() {
    return array('path' => 'guild/' . $this->gid);
  }

}
