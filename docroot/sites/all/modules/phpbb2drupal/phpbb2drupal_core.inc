<?php

include_once('phpbb2drupal.inc');

/**
 * Migrate users from phpbb to Drupal
 *
 * @author DarkSnow.net
 * @file
 * Migrate all the core functionality of a PHPBB forum into Drupal Core
 *
 * @issue http://drupal.org/node/777168
 * On migration we'll get a warning:
 *  "Undefined property: stdClass::$uid in profile_user_presave()"
 *  for every user. This is linked to an issue with profile and should be resolved soon.
 */
class PhpbbUserMigration extends PhpbbMigration {
 static $phpbb_type = 'User';
  public function __construct($settings_machine_name) {
    parent::__construct($settings_machine_name);
    $this->description = t('Migrate phpbb users to Drupal users');
    $this->softDependencies = array($this->getMigrationName('Avatar'));

    // Select everything from the phpbb users table
    $this->source = new MigrateSourceSQL(
      $this->query()
    );

    // Built in destination for users
    $this->destination = new MigrateDestinationUserMerge(MigrateDestinationUser::options(
      $this->defaultLanguage, $this->importFormat, FALSE
    ));

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'user_id' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => TRUE)
      ),
      MigrateDestinationUser::getKeySchema(),  $this->migration_settings['machine_name']
    );

    $this->addFieldMapping('name', 'username');
//    $this->addFieldMapping('pass', 'user_password'); // Password handled in complete()
    $this->addFieldMapping('mail', 'user_email');
    $this->addFieldMapping('signature', 'user_sig');
    $this->addFieldMapping('created', 'user_regdate');
    $this->addFieldMapping('access', 'user_lastvisit');
    $this->addFieldMapping('login', 'user_lastvisit');

    // User status handled in prepare()
    $this->addFieldMapping(NULL, 'user_type')->issueGroup(t('DNM'));
    $this->addFieldMapping(NULL, 'user_inactive_reason')->issueGroup(t('DNM'));

    $this->addFieldMapping('timezone', 'user_timezone');
    $this->addFieldMapping('init', 'user_email');
    $this->addFieldMapping('roles')->defaultValue(2); # Authenticated User
    $this->addFieldMapping('user_website', 'user_website');
    $this->addFieldMapping('user_from', 'user_from');
    $this->addFieldMapping('user_occ', 'user_occ');
    $this->addFieldMapping('user_interests', 'user_interests');
    $this->addFieldMapping('signature_format')->defaultValue($this->importFormat);
    $this->addFieldMapping('language')->defaultValue($this->defaultLanguage);

    // Add all IM fields to IM Contacts field, done in prepareRow
    $this->addFieldMapping(NULL, 'user_icq')->issueGroup(t('DNM'));
    $this->addFieldMapping(NULL, 'user_aim')->issueGroup(t('DNM'));
    $this->addFieldMapping(NULL, 'user_yim')->issueGroup(t('DNM'));
    $this->addFieldMapping(NULL, 'user_msnm')->issueGroup(t('DNM'));
    $this->addFieldMapping(NULL, 'user_jabber')->issueGroup(t('DNM'));
    $this->addFieldMapping('user_im', 'user_im')->separator(PHPBB_LIST_SEPARATOR);

    $this->addFieldMapping('picture', 'user_avatar')->sourceMigration($this->getMigrationName('Avatar'));

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'path', 'is_new', 'theme',
      'status', // Handled in prepare
    ));

  }
  
  /**
   * Method to return the query used in the import.
   * Can be called by sub classes and extended if needed.
   */
  protected function query() {
    $query =  Database::getConnection('default', $this->migration_settings['machine_name'])
      ->select('{users}', 'u')->fields('u', array(
       'user_id', 'user_type', 'username', 'user_password', 'user_email', 'user_sig', 'user_sig_bbcode_uid',
       'user_regdate', 'user_lastvisit', 'user_inactive_reason',
       'group_id', 'user_timezone', 'user_avatar', 'user_website', 'user_from', 'user_birthday',
       'user_icq', 'user_aim', 'user_yim', 'user_msnm', 'user_jabber',
       'user_occ', 'user_interests'))
     ->condition('user_type', 2, '<>'); // User type 2 is for anonymous and web crawlers
    // To ignor the admin users set the minimum user id to be 2.
    if ($this->migration_settings['admin_migrate_option'] == PHPBB2DRUPAL_ADMIN_USER_IGNORE) {
      $min_uid = 2;
    }
    else {
      $min_uid = 1;
    }
    $query->condition('u.user_id', $min_uid, '>'); // Skip users, depending on key.
    return $query;
  }

  /**
   * Preprocess import rows before any other processing occurs
   *
   * @param $user The current import row
   */
  public function prepareRow($user) {
    
    
    $user->user_aim = strtr($user->user_aim, array('+' => ' ')); # PHPBB stores spaces as +, replace with ' '
    $user->user_yim = strtr($user->user_yim, array('+' => ' '));

    // Concatenate all IM fields into the user_im field so the system will add them all to one field
    $im_contacts = array();
    if (!empty($user->user_icq)   ) array_push($im_contacts, 'ICQ: ' . $user->user_icq);
    if (!empty($user->user_aim)   ) array_push($im_contacts, 'AOL: ' . $user->user_aim);
    if (!empty($user->user_yim)   ) array_push($im_contacts, 'Yahoo: ' . $user->user_yim);
    if (!empty($user->user_msnm)  ) array_push($im_contacts, 'MSNM: ' . $user->user_msnm);
    if (!empty($user->user_jabber)) array_push($im_contacts, 'Jabber: ' . $user->user_jabber);
    $user->user_im = count($im_contacts) ? implode(PHPBB_LIST_SEPARATOR, $im_contacts) : NULL;

    // Drupal 7 stores timezones in a 'friendly way' rather than just offsets
    // like Drupal 6 or PHPBB ie Australia/Sydney instead of 36000
    $second_offset = $user->user_timezone * 60 * 60;
    $user->user_timezone = timezone_name_from_abbr("", $second_offset, 0);

    // remove the :bbcode_uid from signature
    if (!empty($user->user_sig)) {
      $user->user_sig = PhpbbContentHelpers::stripUid($user->user_sig, $user->user_sig_bbcode_uid);
    }
    $user->user_sig = PhpbbContentHelpers::textSanitise(PhpbbContentHelpers::stripBbcode($user->user_sig));
    if(!module_exists('signature_forum')) {
      $user->user_sig = drupal_substr($user->user_sig, 0, 255); # Limit signature to 255 characters
    }
    if (!empty($user->user_avatar) && strstr('http://', $avatar->user_avatar) === FALSE) {
      $user->user_avatar = PhpbbAvatarMigration::getRealFilename($user->user_avatar, $user->user_id);
    }
    
    // Check for duplicate users.
    $user_by_username = user_load_by_name($user->username);
    if($user->user_email != '') {
      $user_by_email = user_load_by_mail($user->user_email);
    }
    /**
     * If a user exsists with email addres then use that username no matter what the source is.
     */
    if($user->user_id == 2 && $this->migration_settings['admin_migrate_option'] == PHPBB2DRUPAL_ADMIN_USER_ADMIN) {
      $admin_user = user_load(1);
      $user->uid = 1;
      $user->name = $admin_user->name;
      $user->pass = $admin_user->pass;
      $user->is_new = FALSE;
      $this->rollbackAction = MigrateMap::ROLLBACK_PRESERVE;
    }
    else if ($user_by_email) {
      $dupe_option = $this->migration_settings['email_collision_option'];
      if ($dupe_option == PHPBB2DRUPAL_DUPE_EMAIL_IGNORE) {
        return FALSE;
      }
     $user->is_new = FALSE;
     $this->rollbackAction = MigrateMap::ROLLBACK_PRESERVE;
    }
    /**
     * If the username is taken by a user with a different email address then generate a user name.
     */
    else if ($user_by_username) {
      if ($this->migration_settings[''] == PHPBB2DRUPAL_DUPE_USERNAME_IGNORE) {
        return FALSE;
      }
    }
  }

  public function prepare($user, stdClass $row) {
    // If the user was marked as inactive for any reason, keep them inactive in Drupal
    $user->status = ($row->user_inactive_reason == 0 && $row->user_type <> 1) ? 1 : 0;

    if($this->migration_settings['admin_role_migrate'] == 1 && $row->user_type == 3) {
      $user->roles[3] = 3;
    }
    
    // Check for duplicate users.
    $user_by_username = user_load_by_name($row->username);
    if($row->user_email != '') {
      $user_by_email = user_load_by_mail($row->user_email);
    }
    if($row->user_id != 2 || $this->migration_settings['admin_migrate_option'] == PHPBB2DRUPAL_ADMIN_USER_NORMAL) {
      if (isset($user_by_email) && $user_by_email) {
        $dupe_option = $this->migration_settings['email_collision_option'];
        
        if ($dupe_option == PHPBB2DRUPAL_DUPE_EMAIL_TRACK) {
          $props = get_object_vars($user_by_email);
          foreach($props as $key => $prop) {
            $user->{$key} = $prop;
          }
          // Avatars are problamatic. 
          unset($user->picture);
        }
        else { // OVERWRITE
          $user->uid = $user_by_email->uid;
          $user->name = $user_by_email->name;
          $user->pass = $user_by_email->pass;
        }
        $user->is_new = FALSE;
        $this->rollbackAction = MigrateMap::ROLLBACK_PRESERVE;
      }
      /**
       * If the username is taken by a user with a different email address then generate a user name.
       */
      else if ($user_by_username) {
        // Should be PHPBB2DRUPAL_DUPE_USERNAME_GENERATE
          $old_username =  $row->username;
          $user->name = $row->username . '_' . rand(1000,9999);
          watchdog('phpbb2drupal_migtate','User with email $mail had username $orig_username which was taken so this has been changed to $new_username',
              array('$mail' => $row->user_email,
                  '$orig_username' => $old_username,
                  '$new_username' => $user->name)
          );
        }
      }
    }
  

  public function complete($user, stdClass $row) {
    // Copy the password directly into the use table so we can use existing passwords
    // UPDATE users SET pass = $row->user_password WHERE uid = $user->uid
    // Only for new users.
    if (!isset($user->is_new) || !$user->is_new) {
      db_update('{users}')
        ->condition('uid', $user->uid)
        ->fields(array('pass' => $row->user_password))
        ->execute();
    }
  }
  /**
   * Misuse create stub to create users when they have been deleted to preserve autor distiction.
   * @param unknown_type $migration
   * @param unknown_type $source_id
   */
  protected function createStub($migration, $source_id) {
    //TODO: provide setting for creation of deleted user accounts.
    $user = new stdClass();
    // Don't migrate anon users.
    if($source_id[0] < 2) {
     return FALSE;
    }
    //TODO: check that the user isn't deleted.
    
    switch($this->migration_settings['deleted_user_option']) {
      case PHPBB2DRUPAL_DELETED_USER_GENERATE_EACH:
        $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
        $random_string = '';
        for ($i = 0; $i < 10; $i++) {
          $random_string .= $characters[rand(0, strlen($characters) - 1)];
        }
        
        $user->name = 'deleted user #' . $source_id[0] . ' random ' .rand(1, 1000);
        $user->pass = $random_string;
        $user->signature='';
        $user->enabled = FALSE;
        $user = user_save($user);
        if($user) {
          return array($user->uid);
        }
        else {
          return ;
        }        
      break;
      case PHPBB2DRUPAL_DELETED_USER_GENERATE_ADMIN:
        return 1;
      break;
      case PHPBB2DRUPAL_DELETED_USER_GENERATE_ANON:
        return 0;
      break;
      // If no case is selected, or if ignore is selected return false to have no mapping.
      case PHPBB2DRUPAL_DELETED_USER_IGNORE:
      default:
        return ;
    }
  }
}

/**
 * Import forum user avatars from phpbb to core picture field
 */

class PhpbbAvatarMigration extends PhpbbMigration {
 static $phpbb_type = 'Avatar';
 public function __construct($settings_machine_name) {
  parent::__construct($settings_machine_name);
  $this->description = t('Migrate user avatars from phpbb.');

  $this->map = new MigrateSQLMap($this->machineName,
    array('user_avatar' => array(
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
      'description' => 'Avatar File Name.'
    )
    ),
    MigrateDestinationFile::getKeySchema(), $this->migration_settings['machine_name']
  );

  $this->source = new MigrateSourceSQL($this->query());

  $this->destination = new MigrateDestinationFile('file', 'MigrateFileUri', array());

  $this->addFieldMapping('source_dir')->defaultValue($this->getPath('avatar'));
  $this->addFieldMapping('value', 'user_avatar');
  $this->addFieldMapping('destination_dir')->defaultValue(file_build_uri('users/avatar'));
  $this->addFieldMapping('destination_file', 'user_avatar');
  $this->addFieldMapping('file_replace')->defaultValue(FILE_EXISTS_RENAME);
  $this->addFieldMapping('preserve_files')->defaultValue(0);

  // Unmapped destination fields
  $this->addUnmigratedDestinations(array('uid', 'timestamp', 'path'));
 }
 
 /**
  * Method to return the query used in the import.
  * Can be called by sub classes and extended if needed.
  */
 protected function query() {
   return Database::getConnection('default', $this->migration_settings['machine_name'])
   ->select('{users}', 'u')
   ->fields('u', array('user_id', 'user_avatar'))
   ->isNotNull('user_avatar')
   ->condition('user_avatar', "", '<>');
 }
 
 /**
  * Preprocess import rows before any other processing occurs
  *
  * @param $user The current import row
  */
 public function prepareRow($avatar) {
  if (!empty($avatar->user_avatar)) {
   $avatar->user_avatar = $this->getRealFilename($avatar->user_avatar, $avatar->user_id);
  }
 }

 public function getRealFilename($dbFileName, $dbUserID) {
  $name = pathinfo($dbFileName);
  $filename = $this->getPhpbbDbVar('avatar_salt') . '_' . $dbUserID . '.' . $name['extension'];
  return $filename;
 }
}

/**
 * Import hierarchy of forums from phpbb to core forum.
 * Core forum uses vocabulary terms, under the Forums vocabulary, to
 * create the hierarchy of forums.
 */
class PhpbbForumsMigration extends PhpbbMigration {
  static $phpbb_type = 'Forums';
  protected $forum_weight = 0;
  public function __construct($settings_machine_name) {
    parent::__construct($settings_machine_name);
    $this->description = t('Migrate forum structure from phpbb.');

    /**
     * Get the bits we need, the sort order ensures root level parents appear
     * first and that we get the children in the correct order.
     */
    $this->source = new MigrateSourceSQL($this->query());

    // Built in destination for Taxonomy Terms
    $this->destination = new MigrateDestinationTerm('forums');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'forum_id' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => TRUE)
      ),
      MigrateDestinationTerm::getKeySchema(), $this->migration_settings['machine_name']
    );
    $this->addFieldMapping('name', 'forum_name');
    $this->addFieldMapping('description', 'forum_desc');
    $this->addFieldMapping('parent', 'parent_id')->sourceMigration($this->machineName);
    $this->addFieldMapping('format')->defaultValue($this->importFormat);
    $this->addFieldMapping('language')->defaultValue($this->defaultLanguage);
    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('weight', 'parent_name', 'path'));
  }
  
  /**
   * Method to return the query used in the import.
   * Can be called by sub classes and extended if needed.
   */
  protected function query() {
   return Database::getConnection('default', $this->migration_settings['machine_name'])
    ->select('{forums}', 'f')
    ->fields('f', array('forum_id', 'parent_id', 'forum_name', 'forum_desc', 'forum_type'))
    ->condition('forum_type', 2, '<>')
    ->orderBy('parent_id')
    ->orderBy('right_id')
    ->orderBy('left_id');
  }

  /**
   * Ensure root level (parent == 0) forums don't get a stub parent created
   *
   * @param Migration $migration The current running migration
   */
  protected function createStub($migration, $term) {
    if ($migration->sourceValues->parent_id == 0) {
      return array(0);
    }

    $tax = new stdClass;
    $tax->name = t('Stub');
    $tax->description = $migration->sourceValues->forum_desc;
    $tax->language = $this->defaultLanguage;
//  TODO Find out how the destination knows the vid instead of getting the variable
    $tax->vid = variable_get('forum_nav_vocabulary', 0);
    taxonomy_term_save($tax);
    if (isset($tax->tid)) {
      return array($tax->tid);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Increment and use the objects weight variable so the ordering
   * of the query will set the weight, and thus order, of the taxonomy
   *
   * @param TaxonomyTerm $forum The Destination taxonomy term
   * @param stdClass     $row   The current source row
   */
  public function prepare($forum, stdClass $row) {
    $forum->weight = ++$this->forum_weight;
    $forum->name = html_entity_decode($forum->name);
  }

  /**
   * Once the forum term has been imported, if it's a container, which is
   * forum_type == 0 in phpbb, mark it as such in Drupal
   *
   * @param TaxonomyTerm $forum The newly imported forum
   * @param stdClass     $row   The current source row
   */
  public function complete($forum, stdClass $row) {
    if ($row->forum_type == 0) {
      $containers = variable_get('forum_containers', array());
      array_push($containers, $forum->tid);
      variable_set('forum_containers', $containers);
    }
    if(module_exists('phpbb_redirect')) {
      phpbb_redirect_add_forum($forum->tid,$row->forum_id,$this->migration_settings['machine_name']);
    }
  }
  
  public function completeRollback($forum_id) {
   if(module_exists('phpbb_redirect')) {
    phpbb_redirect_remove_forum($forum_id, $this->migration_settings['machine_name']);
   }
  }
}

/**
 * Import forum topics from phpbb to core forum nodes
 */
class PhpbbTopicMigration extends PhpbbMigration {
  static $phpbb_type = 'Topic';
  public function __construct($settings_machine_name) {
    parent::__construct($settings_machine_name);
    $this->description = t('Migrate individual forum topics from phpbb.');
    $this->dependencies = array($this->getMigrationName('User'), $this->getMigrationName('Forums'));
    $this->source = new MigrateSourceSQL($this->query());
    
    $this->destination = new MigrateDestinationNode('forum',
      array('text_format' => $this->importFormat)
    );

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'post_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'alias' => 'p'
        )
      ),
      MigrateDestinationNode::getKeySchema(), $this->migration_settings['machine_name']
    );

    // Put NULL in here due to error http://drupal.org/node/839770
    $this->addFieldMapping('nid')->defaultValue(NULL);
    $this->addFieldMapping('uid', 'poster_id')
      ->sourceMigration($this->getMigrationName('User'))->defaultValue(1);
    $this->addFieldMapping('taxonomy_forums', 'forum_id')
      ->sourceMigration($this->getMigrationName('Forums'))
      ->arguments(array('source_type' => 'tid'));
//    $this->addFieldMapping('hostname', 'poster_ip');
    $this->addFieldMapping('status', 'post_approved');
    $this->addFieldMapping('title', 'post_subject');
    $this->addFieldMapping('body', 'post_text');
    $this->addFieldMapping('created', 'post_time');
    $this->addFieldMapping('changed', 'post_edit_time');
    $this->addFieldMapping('language')->defaultValue($this->defaultLanguage);
    $this->addFieldMapping('sticky', 'topic_type');
    $this->addFieldMapping('totalcount','topic_views');   
    $this->addFieldMapping(NULL, 'bbcode_uid')->issueGroup(t('DNM')); // Used to santise body text

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'is_new', 'revision', 'revision_uid', 'promote', 'comment', 'path', 'pathauto'
    ));
  }
  
  /**
   * Method to return the query used in the import.
   * Can be called by sub classes and extended if needed.
   */
  protected function query() {
   $query = Database::getConnection('default', $this->migration_settings['machine_name'])->select('{posts}', 'p');
   $query->leftJoin('{topics}', 't', 'p.topic_id = t.topic_id');
   $query->fields('p', array(
       'forum_id', 'post_id', 'poster_id', //'poster_ip',
       'post_time', 'post_edit_time', 'post_approved',
       'post_subject', 'post_text', 'bbcode_uid',
       'post_edit_reason', 'post_edit_user', 'post_edit_count'));
   $query->fields('t', array('topic_type','topic_views','topic_id'));
   $query->condition('post_approved', 1)
     ->where('p.post_id = t.topic_first_post_id')
     ->orderBy('post_time');
   return $query;
  }

  function prepareRow($post) {
    // remove the :bbcode_uid from post_text
    if (!empty($post->bbcode_uid)) {
      $post->post_text = PhpbbContentHelpers::stripUid($post->post_text, $post->bbcode_uid);
    }
    $post->post_text = PhpbbContentHelpers::stripBbcode($post->post_text);
    $post->post_text = PhpbbContentHelpers::textSanitise($post->post_text);
    $post->post_subject = PhpbbContentHelpers::textSanitise($post->post_subject);
    if ($post->post_edit_time == 0) {
      $post->post_edit_time = $post->post_time;
    }
    $post->topic_type = ($post->topic_type > 0) ? 1 : 0;
    $post->forum_id = ($post->forum_id == 0) ? 1 : $post->forum_id;
  }
  
  public function complete($node, stdClass $row) {
    if(module_exists('phpbb_redirect')) {
      phpbb_redirect_add_node($node->nid, $row->topic_id, $this->migration_settings['machine_name']);
    }
    
    // Save a new revision if the post had been edited
    if ($row->post_edit_count > 0) {
      $node->revision = TRUE;
      $node->log = $row->post_edit_reason;
      $original_user = $node->uid;
      node_save($node);
      // fixup the node object, modified should be from migrate. 
      $node_query = db_update('node');
      $node_query->condition('nid',$node->nid);
      $node_query->fields(array('changed' => $row->post_edit_time,'uid' => $original_user));
      $node_query->execute();
      
      $node_revision_query = db_update('node_revision');
      $node_revision_query->condition('vid',$node->vid);
      $fields = array('timestamp' => $row->post_edit_time);
      // Apply the correct user to the revsion. If not the orignal user.
      $account_id = $original_user;
      if ($row->post_edit_user != $row->poster_id) {
        $user_migration_name = $this->getMigrationName('User');
        $user_migration = MigrationBase::getInstance($user_migration_name);
        $source_row = $user_migration->map->getRowBySource(array($row->post_edit_user));
        $account_id = $source_row['destid1'];
      }
      $fields['uid'] = $account_id;
      $node_revision_query->fields($fields);
      $node_revision_query->execute();
    } 
  }
  
  public function completeRollback($nid) {
    if(module_exists('phpbb_redirect')) {
      phpbb_redirect_remove_node($nid, $this->migration_settings['machine_name']);
    }
  }
}

/**
 * Import forum posts from phpbb to core forum comments
 */
class PhpbbPostMigration extends PhpbbMigration {
  static $phpbb_type = 'Post';
  public function __construct($settings_machine_name) {
    parent::__construct($settings_machine_name);
    $this->description = t('Migrate individual forum posts from phpbb.');
    $this->dependencies =  array($this->getMigrationName('Topic'));
    $this->source = new MigrateSourceSQL($this->query());

    $this->destination = new MigrateDestinationComment('comment_node_forum',
      array('text_format' => $this->importFormat)
    );

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'post_id' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => TRUE)
      ),
      MigrateDestinationComment::getKeySchema(), $this->migration_settings['machine_name']
    );

    $this->addFieldMapping('uid', 'poster_id')
      ->sourceMigration($this->getMigrationName('User'))->defaultValue(1);
    $this->addFieldMapping('nid', 'topic_first_post_id')
      ->sourceMigration($this->getMigrationName('Topic'));
    $this->addFieldMapping('hostname', 'poster_ip');
    $this->addFieldMapping('status', 'post_approved');
    $this->addFieldMapping('subject', 'post_subject');
    $this->addFieldMapping('comment_body', 'post_text');
    $this->addFieldMapping('created', 'post_time');
    $this->addFieldMapping('changed', 'post_edit_time');
    $this->addFieldMapping('language')->defaultValue($this->defaultLanguage);

    $this->addFieldMapping(NULL, 'bbcode_uid')->issueGroup(t('DNM')); // Used to santise body text

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'pid', 'path', 'thread', 'name', 'mail', 'homepage'
    ));
  }

  /**
   * Method to return the query used in the import.
   * Can be called by sub classes and extended if needed.
   */
  protected function query() {
   $query = Database::getConnection('default', $this->migration_settings['machine_name'])->select('{posts}', 'p');
   $query->leftJoin('{topics}', 't', 'p.topic_id = t.topic_id');
   $query->addField('t', 'topic_first_post_id');
   $query->fields('p', array(
     'post_id', 'poster_id', 'poster_ip',
     'post_time', 'post_edit_time', 'post_approved',
     'post_subject', 'post_text', 'bbcode_uid',))
     ->condition('post_approved', 1)
     ->where('p.post_id <> t.topic_first_post_id')
     ->orderBy('post_time');   
   return $query;
  }
  
  function prepareRow($post) {
    // remove the :bbcode_uid from post_text
    if (!empty($post->bbcode_uid)) {
      $post->post_text = PhpbbContentHelpers::stripUid($post->post_text, $post->bbcode_uid);
    }
    $post->post_text = PhpbbContentHelpers::stripBbcode($post->post_text);
    $post->post_text = PhpbbContentHelpers::textSanitise($post->post_text);
    $post->post_subject = PhpbbContentHelpers::textSanitise($post->post_subject);  
    if ($post->post_edit_time == 0) {
      $post->post_edit_time = $post->post_time;
    }
  }
  
  public function complete($comment, stdClass $row) {
    if(module_exists('phpbb_redirect')) {
      phpbb_redirect_add_comment($comment->cid, $row->post_id, $this->migration_settings['machine_name']);
    }
  }
  
  public function completeRollback($cid) {
   if(module_exists('phpbb_redirect')) {
    phpbb_redirect_remove_comment($cid, $this->migration_settings['machine_name']);
   }
  }

}

/**
 * Import attachemtns from phpbb to Drupal file fields
 */
class PhpbbAttachmentMigration extends PhpbbMigration {
  static $phpbb_type = 'Attachment';
  // TODO: Automatically rename the files on disk before starting. Currently done by drush command.

  /**
   * Are we using the media module?
   */
  private $media = FALSE;

  public function __construct($settings_machine_name) {
    parent::__construct($settings_machine_name);
    $this->description = t('Migrate all phpbb attachments to a file field on the relavant D7 entity.');

    $this->dependencies = array($this->getMigrationName('Post'), $this->getMigrationName('User'));

    $this->source = new MigrateSourceSQL($this->query());

    $dest_options = array('copy_file' => TRUE);
    // Make use of the media module if present
    if (module_exists('media') && module_exists('migrate_extras')) {
      $this->media = TRUE;
    }
    // TODO: Strip full phpbb path from the filename so files are copied into attachments directory
    // TODO: Get this directory from the Attachments field settings
    $dest_options['copy_destination'] = file_build_uri('attachments') . '/';
    $this->destination = new MigrateDestinationFile('file','MigrateFileUri',$dest_options);
    $keySchema = MigrateDestinationFile::getKeySchema();
    $this->addFieldMapping('value', 'physical_filename');
    $this->addFieldMapping('destination_file','real_filename');
    $this->addFieldMapping('file_replace')->defaultValue(FILE_EXISTS_RENAME);
    $this->addFieldMapping('description', 'attach_comment');
    $this->addFieldMapping('display')->defaultValue(1);
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'attach_id' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => TRUE)
      ),
      $keySchema, $this->migration_settings['machine_name']
    );

    
    // Just map the incoming URL to the destination's 'uri'
    $this->addFieldMapping('uri', 'physical_filename');
    $this->addFieldMapping('uid', 'poster_id')->sourceMigration($this->getMigrationName('User'));
    $this->addFieldMapping('timestamp', 'filetime');
    $this->addFieldMapping('topic_id', 'post_msg_id')->sourceMigration($this->getMigrationName('Topic'))->issueGroup(t('DNM'));
    $this->addFieldMapping('comment_id', 'post_msg_id')->sourceMigration($this->getMigrationName('Post'))->issueGroup(t('DNM'));
    $this->addFieldMapping(NULL, 'topic_first_post_id')->issueGroup(t('DNM'));
    $this->addFieldMapping('filemime','mimetype');
    
    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('filename', 'contents', 'status', 'path'));

  }
  
  /**
   * Method to return the query used in the import.
   * Can be called by sub classes and extended if needed.
   */
  protected function query() {
   $query = Database::getConnection('default', $this->migration_settings['machine_name'])
     ->select('{attachments}', 'a');
   $query->innerJoin('{topics}', 't', 'a.topic_id = t.topic_id');
   $query->addField('t', 'topic_first_post_id');
   $query->fields('a', array(
     'attach_id', 'post_msg_id', 'poster_id',
     'real_filename', 'physical_filename',
     'attach_comment', 'mimetype',
     'filetime'));
   return $query;
  }
  
  /**
   * Preprocess import rows before any other processing occurs
   *
   * @param $user The current import row
   */
  public function prepareRow($att) {
    //$att->real_filename = $this->getPath('upload') . "/{$att->real_filename}";
    $att->physical_filename =  $this->getPath('upload') . '/' . $att->physical_filename;
  }

  /**
   * Once the file has been imported, rename it and
   * associate it with the appropriate node
   */
  public function complete($file, stdClass $row) {
      // Pick only MIME types that media supports
      $mediaMime = FALSE;
      switch (substr($file->filemime, 0, 5)) {
      case "image":
      case "audio":
      case "video":
        $mediaMime = TRUE;
        break;
      default:
        $mediaMime = FALSE;
        break;
      }
      
      // Replace the tags and save the object as an attachment.
      $i = 0;
      if ($row->topic_first_post_id == $row->post_msg_id) {
        $node = TRUE;
        $object = node_load($this->destinationValues->topic_id);
        $body = $object->body[LANGUAGE_NONE][0]['value'];
      }
      else {
        $object =  comment_load($this->destinationValues->comment_id);
        $body = $object->comment_body[LANGUAGE_NONE][0]['value'];
        $node = FALSE;
      }
      $i = 0;
      if (count($object->field_attach)) {
        $i = count($object->field_attach[LANGUAGE_NONE]);
      }
      
      // If media module add media tags.
      if ($mediaMime && $this->media) {
        $tag_data = array('type' => 'media',
                      'view_mode' => 'media_large',
                      'fid' => $file->fid,
                      'attributes' => array(
                          'alt' => '',
                          'class' => 'media-image',
                          'typeof' => 'foaf:Image'
                          ));
        $new_tag = '[[' . json_encode($tag_data) . ']]';
        $new_body = $this->replaceContent($body, $row->real_filename, $new_tag);
      }

      $object->field_attach[LANGUAGE_NONE][$i] = (array)$file;
  
      // GET LAST CHANGED DATE
      $old_modified_date = $object->changed;
      $old_created_date  = $object->created;
    
      // SAVE OBJECT
      if ($node) {
        $object->body[LANGUAGE_NONE][0]['value'] = (isset($new_body))? $new_body: $body;
        node_submit($object);
        $object->created = $old_created_date;
        node_save($object);
        $query = db_update('node');
        $query->condition('nid',$object->nid);
      }
      else {
        $object->comment_body[LANGUAGE_NONE][0]['value'] = (isset($new_body))? $new_body: $body;
        $object->is_anonymous = FALSE;
        comment_submit($object);
        $object->created = $old_created_date;
        comment_save($object);
        $query = db_update('comment');
        $query->condition('cid',$object->cid);   
      }

      // UPDATE LAST CHANGED DATE
      $query->fields(array('changed' => $old_modified_date));
      $query->execute();
  }
  
  

  /**
   * Replace the attachment tag for this file with the [media] tag.
   * If no matches are found, append tag to field.
   * 
   * @param $field The field to search
   * @param $filename The file name to search for
   * @param $tag The media tag to replace any matches with
   * 
   */
  public function replaceContent($field, $filename, $tag) {
      $count = 0;
      // [attachment=1:1e5rw1bo]<!-- ia1 -->P7.jpg<!-- ia1 -->[/attachment:1e5rw1bo]
      // [literal=id:bbcode_uid]comment $filename close[/literal:bbcode_uid]
      // BBCode UIDs will have already been stripped, leaving us:
      // [attachment=x]$filename[/attachment]
      $pattern = '/\[attachment=\d+\]' . basename($filename) . '\[\/attachment\]/';

      // We should only find one match so limit to 1 replacement
      $result = preg_replace($pattern, $tag, $field, 1, $count);

      // If no replacements were done, append tag to end of field
      return $count ? $result : $field . "\n{$tag}";
  }
}

class MigrateDestinationUserMerge extends MigrateDestinationUser {
  public function import(stdClass $account, stdClass $row) {
    if(isset($row->uid)) {
      $account->uid = $row->uid;
    }
  return parent::import($account,$row);
  }
}

