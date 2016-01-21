<?php

/**
 * @file
 * Core file of the migration, containing code helpful to all migrations
 */

/**
 * Used to build a multi value list
 */
define("PHPBB_LIST_SEPARATOR", '|');

abstract class PhpbbMigration extends DynamicMigration {

  protected $importFormat;
  protected $defaultLanguage;
  protected $migration_settings;
  static $phpbb_type = NULL;
  
  public function __construct($settings_machine_name) {
    $called_class = get_called_class();
    $settings = phpbb2drupal_get_migration_settings($settings_machine_name);
    $this->migration_settings = $settings;
    PhpbbConfig::createConnection($settings);
    //TODO: so that each set of migrations is in it's own group.
    parent::__construct(MigrateGroup::getInstance('phpbb_' . $this->migration_settings['machine_name']));
    $this->machineName = $this->getMigrationName($called_class::$phpbb_type);
    
    $this->importFormat = $settings['text_format'];
    if (drupal_multilingual()) {
      $languages = language_list('enabled');
      $languages = $languages[1];
      $forum_language = $this->getPhpbbDbVar('default_lang');
      if (!isset($languages[$forum_language])) {
        // Try a simpler version of the language code.
        $forum_language_array = explode('_', $this->getPhpbbDbVar('default_lang'));
        $forum_language = $forum_language_array[0];
        if (!isset($languages[$forum_language])) {
          drupal_set_message(t('The laguage code %langcode is used in the forum %name, but this language is not enabled on your site.',array('%langcode' => $forum_language,'%name' => $this->migration_settings['name'])),'warning',FALSE);
        } 
      }
      $this->defaultLanguage = $forum_language;
    }
    else {
      global $language;
      $this->defaultLanguage = $language->language;
    }
    $this->team = array(
      new MigrateTeamMember('Martin Fraser', 'mdf@darksnow.net', t('Implementor')),
      new MigrateTeamMember('Jeremy French', 'jeremy@jeremyfrench.co.uk', t('i18n and multi import'))
    );

    // Individual mappings in a migration can be linked to a ticket or issue
    // in an external tracking system. Define the URL pattern here in the shared
    // class with ':id:' representing the position of the issue number, then add
    // ->issueNumber(1234) to a mapping.
    $this->issuePattern = 'http://drupal.org/node/:id:';

  }


  /**
   * Migrations which are part of the same set will have common names, this is a function for
   * them to be able to generate their common names and reference each other for dependancies
   * etc.
   *
   * @param string $base_name the base type of the name for example user or privatemsg
   */
  protected function getMigrationName($base_name) {
   if($this->migration_settings['machine_name'] == 'phpbb2drupal') {
    return  'Phpbb' . $base_name;
   } 
   return 'phpbb_' . $this->migration_settings['machine_name'] . '_' . $base_name;
  }
  
  /**
   * Get a variable from the Phpbb config table, or NULL if not found.
   * This requires the database connection to have already been created.
   * @param $var
   */
  function getPhpbbDbVar($var) {
   $ret = NULL;
   try {
    $result = Database::getConnection('default', $this->migration_settings['machine_name'])
    ->select('{config}', 'c')
    ->fields('c', array('config_value'))
    ->condition('config_name', $var)
    ->execute()
    ->fetchAssoc();
    if ($result) {
     $ret = $result['config_value'];
    }
   }
   catch (Exception $ex) {
    // Assume an error means we won't find anything, so return the NULL
   }
   return $ret;
  }
  
  /**
   * Build a stream path for the request phpbb config variable
   * Valid paths are: avatar, icons, ranks, smilies, upload
   *
   * @param $var The variable requested, without the _path suffix
   * @return The complete URI of the requested path
   */
  function getPath($var) {
   /**
    * Get a variable from the Phpbb config table, or NULL if not found.
    * This requires the database connection to have already been created.
    * @param $var
    */
   if($this->migration_settings['files_domain'] != '') {
    $ret = $this->migration_settings['files_domain'];
   }
   else {
    $ret = file_build_uri($this->migration_settings['files_directory']);
   }
   return $ret . $this->getPhpbbDbVar("{$var}_path");
  }
}
/**
 * Static class for content helpers
 */
class PhpbbContentHelpers {
  /**
   * Strip the BBCode UID from the string
   *
   * @param unknown_type $text The source text to be changed
   * @param unknown_type $uid The BBCode UId to remove
   */
  static function stripUid($text, $uid) {
    return preg_replace("/(:[a-z])?:$uid\]/", ']', $text);
  }

  /**
   * Strips text of extra phpbb markup and if requested, also strips all bbcode from text.
   */
  static function stripBbcode($text) {
    // Strip the text of extra markup - regular expressions taken
    // from phpbb includes/function.php, function get_preg_expression().
    $match = array(
      '#<!\-\- e \-\-><a href="mailto:(.*?)">.*?</a><!\-\- e \-\->#',
      '#<!\-\- l \-\-><a (?:class="[\w-]+" )?href="(.*?)(?:(&amp;|\?)sid=[0-9a-f]{32})?">.*?</a><!\-\- l \-\->#',
      '#<!\-\- ([mw]) \-\-><a (?:class="[\w-]+" )?href="(.*?)">.*?</a><!\-\- \1 \-\->#',
      '#<!\-\- s(.*?) \-\-><img src="\{SMILIES_PATH\}\/.*? \/><!\-\- s\1 \-\->#',
      '#<!\-\- .*? \-\->#s',
      '#<.*?>#s',
    );
    $replace = array('$1', '$1', '$2', '$1', '', '');
    $text = preg_replace($match, $replace, $text);

    return $text;
  }

  /**
   * function to properly encode strings.
   */
  static function textSanitise($text) {
    $text = html_entity_decode($text, ENT_QUOTES, 'utf-8');
    return $text;
  }

  

  
}

/**
 * Class to simplify access to the configuration of PHPBB and this migration
 */
class PhpbbConfig {
 
 /**
  * Create a phpbb database connection if it's not already there.
  */
 static function createConnection($settings) {
  global $databases;
  if ($settings['db']['local']) {
   $prefix = $settings['db']['prefix'];
   $settings['db'] = $databases['default']['default'];
   $settings['db']['prefix'] = $prefix;
  }
  if (empty($databases['default'][$settings['machine_name']])) {
   Database::addConnectionInfo($settings['machine_name'], 'default', $settings['db']);
  }
 }

 
  /**
   * Register all migrations.
   * Call this AFTER the configuration has been done!
   */
  static function registerMigrations($settings, $module = '') {
   // Create machine name for settings
   if(!isset($settings['machine_name'])) {
     $settings['machine_name'] = preg_replace('/[^A-Za-z0-9]/', '_', $settings['name']);
   }
   foreach (self::getMigrations() as $class => $mod) {
      if (module_exists($mod) && ($module == $mod || $module == '')) {
       $type = $class::$phpbb_type;
       // Old migrations need special treatment.
       if($settings['machine_name'] == PHPBB2DRUPAL_UPDATE_MAGIC_NAME) {
          $machine_name = 'phpbb' . $type;
        }
        else {
          $machine_name = 'phpbb_' . $settings['machine_name'] . '_' . $type;
        }
        Migration::registerMigration($class, $machine_name, array('machine_name' => $settings['machine_name']));
      }
    }
  }

  /**
   * Deregister all migrations.
   * Called when the module is disabled
   */
  static function deregisterMigrations($module) {
    foreach (self::getMigrations() as $class_name => $mod) {
      if ($mod == $module) {
        
        // TODO: magic for upgraded migrations.
        // See if this migration is already registered
        $migration_query = db_select('migrate_status', 'ms')
          ->fields('ms', array('machine_name', 'arguments'))
          ->condition('class_name', $class_name)
          ->execute();
        while ($migration = $migration_query->fetchObject()) {
          Migration::deregisterMigration($migration->machine_name);
        }
      }
    }
  }
  
  /**
   * Remove migrations for a given name, done when a migration is deleted.
   * @param string $machine_name
   */
  static function deregisterMigrationsByName($machine_name) {
    if($machine_name == 'phpbb2drupal') {
      $base = 'Phpbb';
    }
    else {
      $base = 'phpbb_' . $machine_name . '_';
    }
    foreach (self::getMigrations() as $class_name => $mod) {
      Migration::deregisterMigration($base . $class_name::$phpbb_type);
    }
    
  }

  /**
   * Get a list of all migrations supported by this class.
   * Also provides a list of the sub modules that implement a given migration.
   */
  static function getMigrations() {
    $migrations =  array(
      'PhpbbAvatarMigration' => 'phpbb2drupal',
      'PhpbbForumsMigration' => 'phpbb2drupal',
      'PhpbbUserMigration' => 'phpbb2drupal',
      'PhpbbTopicMigration' => 'phpbb2drupal',
      'PhpbbPostMigration' => 'phpbb2drupal',
      'PhpbbAttachmentMigration' => 'phpbb2drupal',
     );
    drupal_alter('phpbbmigrations', $migrations);
    return $migrations;
  } 
}
