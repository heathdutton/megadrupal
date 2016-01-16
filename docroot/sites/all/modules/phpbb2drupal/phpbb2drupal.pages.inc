<?php

/**
 * @file
 *
 * Web UI for configuration of migration from PHPBB to Drupal.
 */

/**
 * Menu callback: Returns a page for configuring phpbb migrations.
 */
function phpbb2drupal_configure($machine_name = '') {
  drupal_set_title(t('Phpbb migrate configuration'));
  if($machine_name != '') {
     $migration = phpbb2drupal_get_migration_settings(array('machine_name' => $machine_name));
   if (!isset($migration['db']['password'])) {
     $migration['db']['password'] = '';
   }
  }
  else {
   global $databases;
   $migration = array(
     'machine_name' => '',
     'name' => '',
     'db' => array(
       'prefix' => 'phpbb_',
       'local' => 1,
       'driver' => db_driver(),
       'host' => '',
       'database' =>  $databases['default']['default']['database'],
       'username' => '',
       'password' => '',
       ),
     'text_format' => 'filtered_html',
     'files_domain' => '',
     'files_directory' => '',
     'settings' => array(),
     );
  }
  
  return drupal_get_form('phpbb2drupal_configure_form',$migration);
}

/**
 * Form for configuring phpbb migrations.
 */
function phpbb2drupal_configure_form($form, &$form_state,$migration) {
  $form['overview'] = array(
    '#prefix' => '<div>',
    '#markup' => t('The import of phpbb forums into Drupal can be configured using
      this form.<br />
      Once the configuration has been saved here, the various migrations will appear
      in the Migrate tab.<br />
      You may either provide the necessary credentials for Drupal to access
      your phpbb database and files directly, or the data must be in the current
      Drupal database and all files should have been copied into public storage.'),
    '#suffix' => '</div>',
  );

  // Select default text format for bodies etc.
  $options = array();
  foreach (filter_formats() as $format_id => $format) {
    $options[$format_id] = $format->name;
  }
  
  $form['phpbb2drupal_migration_machine_name'] = array(
    '#type' => 'hidden',
    '#default_value' => $migration['machine_name'],
    );
  
  $form['phpbb2drupal_migration_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Migration Name'),
    '#default_value' => $migration['name'],
    '#required' => 'true',
    '#description' => t('Name of the PHPBB install that you are migrating, used to help you identify it.'),
  );
    ;
  
  $form['phpbb2drupal_text_format'] = array(
    '#type' => 'select',
    '#title' => t('Format for text fields'),
    '#default_value' => $migration['text_format'],
    '#options' => $options,
    '#description' => t('Which input format to use for all content'),
  );

/*
  // Might need this bit for import of attachments etc
  if (module_exists('media') && !module_exists('migrate_extras')) {
    $form['need_extras'] = array(
      '#prefix' => '<div>',
      '#markup' => t('You have the <a href="@media">Media module</a> enabled - to
        take advantage of Media features, you need to also install and enable the
        <a href="@extras">Migrate Extras module</a>.',
        array('@media' => url('http://drupal.org/project/media'),
          '@extras' => url('http://drupal.org/project/migrate_extras'))),
      '#suffix' => '</div>',
    );
  }
 */
  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Phpbb files location'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['files']['description'] = array(
    '#prefix' => '<div>',
    '#markup' => t('To import your forum files (avatars, smilies, attachments,
                    etc) from phpbb, enter the location here. You can point the
                    migration at the location of the website directly, or pick one
                    of the directories in the Drupal public file system. The
                    migration will look for the relevant directories, as determined
                    from the phpbb configuration, under this directory, only if you
                    have not set a domain.'),
    '#suffix' => '</div>',
  );

  $form['files']['phpbb2drupal_files_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain of your forum'),
    '#default_value' => $migration['files_domain'],
    '#description' => t('Enter the domain of the forum to import, with path if
                         necessary (e.g., http://example.phpbb.com/forum).'),
  );

  $dirs = array('' => '<none>');
  $path = drupal_realpath('public://');
  foreach (scandir($path) as $dir) {
    $dir_path = drupal_realpath('public://' . $dir);
    if (drupal_substr($dir, 0, 1) != '.' && is_dir($dir_path)) {
      $dirs[$dir] = $dir;
    }
  }
  $form['files']['phpbb2drupal_files_directory'] = array(
    '#type' => 'select',
    '#title' => t('Directory of phpbb files'),
    '#default_value' => $migration['files_directory'],
    '#options' => $dirs,
    '#description' => t('Enter the directory name of the forum files in local storage.'),
  );

  $form['db'] = array(
    '#type' => 'fieldset',
    '#title' => t('phpbb database location'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  global $databases;

  $form['db']['phpbb2drupal_db_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('PHPBB Table prefix'),
    '#default_value' => $migration['db']['prefix'],
    '#description' => t('The prefix on all table names in the phpbb database'),
  );

  $form['db']['phpbb2drupal_db_local'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use local database'),
    '#default_value' =>  $migration['db']['local'],
    '#description' => t('The PHPBB data has been imported into the Drupal database, ignore credentials below'),
  );

  $form['db']['phpbb2drupal_db_driver'] = array(
    '#type' => 'select',
    '#title' => t('DB Driver'),
    '#default_value' => $migration['db']['driver'],
    '#options' => array('mysql' => 'MySQL', 'pgsql' => 'PostgreSQL', 'sqlite' => 'SQL Lite'),
    '#description' => t('The type of database server to connect to'),
  );

  $form['db']['phpbb2drupal_db_host'] = array(
    '#type' => 'textfield',
    '#title' => t('DB Hostname'),
    '#default_value' =>  $migration['db']['host'],
    '#description' => t('The hostname of the PHPBB database server'),
  );

  $form['db']['phpbb2drupal_db_dbname'] = array(
    '#type' => 'textfield',
    '#title' => t('Database name'),
    '#default_value' => $migration['db']['database'],
    '#description' => t('Name of the database on host server'),
  );

  $form['db']['phpbb2drupal_db_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => $migration['db']['username'],
    '#description' => t('Username used to access database'),
  );

  $form['db']['phpbb2drupal_db_password'] = array(
    '#type' => 'password',
    '#default_value' =>  $migration['db']['password'],
    '#title' => t('Password'),
    '#description' => t('Password to your phpbb database'),
  );
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  

  
  $form['settings']['email_collision_option'] = array(
      '#type' => 'select',
      '#title' => t('How to handle duplicate email addresses'),
      '#options' => array(PHPBB2DRUPAL_DUPE_EMAIL_IGNORE => t('ignore user'),
                          PHPBB2DRUPAL_DUPE_EMAIL_TRACK => t('track'),
                          PHPBB2DRUPAL_DUPE_EMAIL_OVERWRITE => t('overwrite')),
      '#default_value' => (isset($migration['email_collision_option'])) ? $migration['email_collision_option'] : PHPBB2DRUPAL_DUPE_EMAIL_IGNORE,
      '#description' => t('If you are migrating from multiple sources, or you have an existing user base there may be cases where the email address of an imported user matches that of an existing user, you can either overwrite the user, track but keep existing details or not import the user.')
      );
  
  $form['settings']['username_collision_option'] = array(
      '#type' => 'select',
      '#options' => array(
          PHPBB2DRUPAL_DUPE_USERNAME_IGNORE => t('ignore'),
          PHPBB2DRUPAL_DUPE_USERNAME_GENERATE => t('generate'),
          ),
      '#default_value' => (isset($migration['username_collision_option'])) ? $migration['username_collision_option'] : PHPBB2DRUPAL_DUPE_USERNAME_IGNORE,
      '#title' => t('How to handle duplicate user names'),
      '#description' => t('If the username matches that of an existing user you can either ignore the user or generate a new name, merging is not allowed as the users may be different.'),
      );
  
  $form['settings']['deleted_user_option'] = array(
      '#type' => 'select',
      '#options' => array(
          PHPBB2DRUPAL_DELETED_USER_IGNORE => t('ignore content'),
          PHPBB2DRUPAL_DELETED_USER_GENERATE_ADMIN => t('Assign to admin user'),
          PHPBB2DRUPAL_DELETED_USER_GENERATE_ANON => t('Assign to anon user'),
          PHPBB2DRUPAL_DELETED_USER_GENERATE_EACH => t('Generate unique users'),
          ),
       '#default_value' => (isset($migration['deleted_user_option'])) ? $migration['deleted_user_option'] : PHPBB2DRUPAL_DELETED_USER_IGNORE,
       '#title' => t('how to handle deleted users and their content?'),
       '#description' => t('phpbb may contain content where the users have been deleted. You may still import this content but will need to decide how to handle users which are deleted. Ignore will ignore content from deleted users.'),
      );
  
  $form['settings']['admin_migrate_option'] = array(
      '#type' => 'select',
      '#default_value' => (isset($migration['admin_migrate_option'])) ? $migration['admin_migrate_option'] : PHPBB2DRUPAL_DELETED_USER_IGNORE,
      '#options' => array(
        PHPBB2DRUPAL_ADMIN_USER_IGNORE => t('Ignore this user'),
        PHPBB2DRUPAL_ADMIN_USER_ADMIN => t('Import as UID 1 in drupal'),
        PHPBB2DRUPAL_ADMIN_USER_NORMAL => t('Import as a normal user'),
      ),
      '#title' => 'How to handle the phpbb admin user',
      '#description' => t('Sometimes the phpbb admin user (userid =2) is used as a live account, if content is created by this user you may need to decide how to handle. If you don\'t ignore here, the behavour will be as for deleted users above'),
      );
  
  $form['settings']['admin_role_migrate'] = array(
      '#type' => 'checkbox',
      '#default_value' => (isset($migration['admin_role_migrate']) && $migration['admin_role_migrate']),
      '#return_value' => 1,
      '#title' => t('give adminstrator role to admins in phpbb'),
      '#description' => t('Some users in phpbb may have FOUNDER user type, check the box if you wish to make these administrators in drupal.'),      
      );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Validate callback for the phpbb configure form.
 */
function phpbb2drupal_configure_form_validate($form, &$form_state) {

  if ($form_state['values']['phpbb2drupal_files_domain'] == '' &&
      $form_state['values']['phpbb2drupal_files_directory'] == '') {
    form_set_error('', t('You must set a domain or directory to find forum files.'));
  }

  if ($form_state['values']['phpbb2drupal_db_local'] == 0 &&
      $form_state['values']['phpbb2drupal_db_host'] == '' &&
      $form_state['values']['phpbb2drupal_db_dbname'] == '' &&
      $form_state['values']['phpbb2drupal_db_username'] == '' &&
      $form_state['values']['phpbb2drupal_db_password'] == '') {
    form_set_error('', t('All external database credentials must be set if
                          you are not using the local database.'));
  }

  $domain = $form_state['values']['phpbb2drupal_files_domain'];
  if (drupal_substr($domain, -1) != '/') {
      $domain .= '/';
  }

  if (drupal_strlen($domain) > 0 && !preg_match('/^http(s)?:\/\//i', $domain)) {
    $domain = "http://{$domain}";
  }
  $form_state['values']['phpbb2drupal_files_domain'] = $domain;
  
}

/**
 * Submit callback for the phpbb configure form.
 */
function phpbb2drupal_configure_form_submit($form, &$form_state) {
 
 $migration = array();
 $migration['machine_name'] = $form_state['values']['phpbb2drupal_migration_machine_name'];
 $migration['name'] = $form_state['values']['phpbb2drupal_migration_name'];
 $migration['text_format']= $form_state['values']['phpbb2drupal_text_format'];
 
 $migration['db']['prefix'] = $form_state['values']['phpbb2drupal_db_prefix'];
 $migration['db']['local'] = $form_state['values']['phpbb2drupal_db_local'];
 $migration['db']['driver'] = $form_state['values']['phpbb2drupal_db_driver'];
 $migration['db']['host'] = $form_state['values']['phpbb2drupal_db_host'];
 $migration['db']['database'] = $form_state['values']['phpbb2drupal_db_dbname'];
 $migration['db']['username'] = $form_state['values']['phpbb2drupal_db_username'];
 $migration['files_domain'] = $form_state['values']['phpbb2drupal_files_domain'];
 $migration['files_directory'] = $form_state['values']['phpbb2drupal_files_directory'];
 if ($form_state['values']['phpbb2drupal_db_password']) {
  $migration['db']['password'] = $form_state['values']['phpbb2drupal_db_password'];
 }
 $migration['db'] = serialize($migration['db']);
 
 // Save advanced setting data.
 $migration['settings']['email_collision_option'] = $form_state['values']['email_collision_option'];
 $migration['settings']['username_collision_option'] = $form_state['values']['username_collision_option'];
 $migration['settings']['deleted_user_option'] = $form_state['values']['deleted_user_option'];
 $migration['settings']['admin_migrate_option'] = $form_state['values']['admin_migrate_option'];
 $migration['settings']['admin_role_migrate'] = $form_state['values']['admin_role_migrate'];
 
  
 $migration['settings'] = serialize($migration['settings']);
  
 // If machine name is blank asume this is a new migration.
 if($migration['machine_name'] == '') {
   $migration['machine_name'] = preg_replace('/[^A-Za-z0-9]/', '_', $migration['name']); //. '_' . $settings['language'];
   // Magic name is for upgrade path only so should not save new migration with it.
   if($migration['machine_name'] == PHPBB2DRUPAL_UPDATE_MAGIC_NAME) {
    $migration['machine_name'] == 'phpbb2drupal_not_update'; 
   }
   // Check for uniqueness of machine name
   $results = db_select('phpbb2drupal_instances','i')->fields('i', array('machine_name'))->condition('machine_name', db_like($migration['machine_name']) . '%', 'LIKE')->execute()->fetchCol();
   if(sizeof($results)!=0) {
     $migration['machine_name'] = $migration['machine_name'] . '_' . sizeof($results);
   }
   // TODO: error checking about db write.
   PhpbbConfig::registerMigrations($migration);
   drupal_write_record('phpbb2drupal_instances', $migration);
   
 }
 else {
  drupal_write_record('phpbb2drupal_instances', $migration, 'machine_name');
 }
 // Save the machine name to the form state so other modules can identify it.
 $form_state['saved_machine_name'] = $migration['machine_name'];
 drupal_set_message(t('PHPBB configuration changes saved.'));
 $form_state['redirect'] = 'admin/content/phpbb/list';
}


/**
 * List saved migrations
 */
function phpbb2drupal_list() {
 // Setup Header
 $header['machine_name'] = array('data' => t('Machine Name'));
 $header['name'] = array('data' => t('Name'), 'field' => 'name', 'sort' => 'asc');
 $header['files_domain'] = array('data' => t('Domain'), 'field' => 'files_domain');
 $header['files_directory'] = array('data' => t('Directory'), 'field' => 'files_directory');
 $header['edit'] = array();
 $header['delete'] =  array();
 // TODO: get details from migration about status.
 
 
 $migrations = db_select('phpbb2drupal_instances','i')
 ->extend('TableSort')
 ->extend('PagerDefault')
 ->fields('i',array('machine_name','name','files_domain','files_directory'))
 ->limit(50)
 ->orderByHeader($header)
 ->execute()
 ->fetchAllAssoc('machine_name',PDO::FETCH_ASSOC);
 
 foreach($migrations as $migration) {
  $migrations[$migration['machine_name']]['edit'] = l('Edit','admin/content/phpbb/' . $migration['machine_name'] . '/edit');
  $migrations[$migration['machine_name']]['delete'] = l('Delete', 'admin/content/phpbb/' . $migration['machine_name'] . '/delete');
}
 
 $content = array();
 $content['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $migrations,
        '#empty' => t('No PHPBB instances added, maby you want to !create one',array('!create' => l('create','admin/content/phpbb/add'))),
    );
    // Create paging element

  $content['pager'] = array(
                            '#theme' => 'pager',
                           );
 return $content;
}

function phpbb2drupal_confirm_delete($form, &$form_state, $machine_name) {
  $form = array();
  $form['#submit'][] = 'phpbb2drupal_delete';
  $form['machine_name'] = array(
        '#type' => 'value',
        '#value' => $machine_name,
      );
 return  confirm_form($form,
     t('Are you sure you want to delete this migration?'),
     '/admin/content/phpbb',
     t('Content that was migrated will NOT be deleted and you will not be able to update or roll back in future. To delete the content do a roll back.'));
  }

function phpbb2drupal_delete($form,&$form_state) {
  $machine_name = $form_state['values']['machine_name'];
  
  // De register migrations
  PhpbbConfig::deregisterMigrationsByName($machine_name);
  // Remove from phpbb migrations tables
  db_delete('phpbb2drupal_instances')->condition('machine_name',$machine_name)->execute();
  
  drupal_set_message(t('%migration_name has been deleted',array('%migration_name' => $machine_name)));
  drupal_goto('admin/content/phpbb');
}