<?php

/**
 * @file
 * Administrative functions
 */

/**
 * Called from monster_menus_menu().
 */
function mm_admin_menu() {
  $items = array();
  $base = array('file' => 'mm_admin.inc');

  $items['admin/config/mm'] = $base + array(
    'title' => 'Monster Menus',
    'description' => 'Settings for Monster Menus',
    'page callback' => 'system_admin_menu_block_page',
    'access arguments' => array('access administration pages'),
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
  );
  $items['admin/config/mm/settings'] = $base + array(
    'title' => 'Monster Menus',
    'description' => "Various settings that affect Monster Menus' appearance and behavior",
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_config'),
    'access arguments' => array('access administration pages'),
    'file' => 'mm_admin.inc',
  );

  // Administer->Monster Menus
  $items['admin/mm'] = array(
    'title' => 'Monster Menus',
    'description' => 'Perform various administrative tasks in Monster Menus.',
    'page callback' => 'system_admin_menu_block_page',
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
    'access callback' => '_mm_menu_access_any_admin');
  $items['admin/mm/settings'] = $base + array(
    'title' => 'Settings',
    'description' => "Change various settings that affect Monster Menus' appearance and behavior",
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_config'),
    'access callback' => '_mm_menu_access_any_admin');
  $items['admin/mm/browse'] = $base + array(
    'title' => 'Browse tree',
    'description' => 'View sections of the Monster Menus tree',
    'page callback' => 'mm_admin_browse',
    'type' => MENU_NORMAL_ITEM,
    'access callback' => '_mm_menu_access_any_admin');
  $items['admin/mm/export'] = $base + array(
    'title' => 'Export tree',
    'description' => 'Export a section of the Monster Menus tree in a format the can be re-imported using "Import tree"',
    'page callback' => 'mm_admin_export',
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/mm/import'] = $base + array(
    'title' => 'Import tree',
    'description' => 'Import a section of the Monster Menus tree that was previously exported using "Export tree"',
    'page callback' => 'mm_admin_import',
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/mm/dump'] = $base + array(
    'title' => 'Export entire tree as CSV',
    'description' => 'Export the entire Monster Menus tree as a CSV file',
    'page callback' => 'mm_admin_dump',
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/mm/vgroups-mark'] = $base + array(
    'title' => 'Mark all virtual groups',
    'description' => 'Mark all virtual groups as "dirty", so that they are regenerated during the next cron run',
    'page callback' => 'mm_content_update_vgroup_view',
    'access callback' => 'user_access',
    'access arguments' => array('administer all groups'));
  $items['admin/mm/vgroups'] = $base + array(
    'title' => 'Update virtual groups',
    'description' => 'Update the membership of any "dirty" virtual groups immediately, instead of during cron',
    'page callback' => 'mm_regenerate_vgroup',
    'access callback' => 'user_access',
    'access arguments' => array('administer all groups'));
  $items['admin/mm/integrity'] = $base + array(
    'title' => 'Verify integrity of database relationships',
    'description' => "Check the relationships between Monster Menus' tables for missing records",
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_verify_integrity'),
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/mm/sort'] = $base + array(
    'title' => 'Verify sort index',
    'description' => 'Check for problems in the way menu entries are sorted and optionally correct them',
    'page callback' => 'mm_admin_validate_sort_index',
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/mm/reassign'] = $base + array(
    'title' => 'Reassign user content',
    'description' => 'Give ownership of all pages and content owned by a particular user to someone else',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_reassign_content'),
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/mm/fix-nodes'] = $base + array(
    'title' => 'Fix URLs in content',
    'description' => 'Fixes URLs contained in node bodies and certain other fields',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_fix_node_urls'),
    'access callback' => '_mm_menu_access_any_admin',
  );
  $items['admin/mm/regions'] = $base + array(
    'title' => 'Region settings',
    'description' => 'Control who can add content to, and what types are allowed in, each page region',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_regions'),
    'access callback' => '_mm_menu_access_any_admin',
  );

  // Administer->Monster Menus->Manage sites
  $items['admin/mm/sites'] = $base + array(
    'title' => 'Manage sites',
    'description' => 'Set a different homepage for each Drupal site',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_list_sites'),
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/mm/sites/list'] = $base + array(
    'title' => 'List',
    'weight' => -10,
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/mm/sites/add'] = $base + array(
    'title' => 'Add',
    'page callback' => 'mm_admin_edit_site',
    'type' => MENU_LOCAL_TASK,
    'access callback' => '_mm_menu_access_all_admin');

  // Administer->Structure->Blocks
  $items['admin/structure/block/mm'] = $base + array(
    'title' => 'MM blocks',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_list_blocks'),
    'type' => MENU_LOCAL_ACTION,
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/structure/block/mm/edit'] = $base + array(
    'title' => 'Configure block',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_edit_block'),
    'type' => MENU_LOCAL_ACTION,
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/structure/block/mm/add'] = $base + array(
    'title' => 'Add block',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_edit_block'),
    'type' => MENU_LOCAL_ACTION,
    'access callback' => '_mm_menu_access_all_admin');
  $items['admin/structure/block/mm/delete'] = $base + array(
    'title' => 'Delete block',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_delete_block'),
    'type' => MENU_LOCAL_ACTION,
    'access callback' => '_mm_menu_access_all_admin');

  // Administer->People
  $items['admin/people/by-uid'] = $base + array(
    'title' => 'Find by name',
    'type' => MENU_LOCAL_TASK,
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mm_admin_find_user'),
    'access arguments' => array('administer users'));

  return $items;
}

function mm_admin_config($form, &$form_state) {
  if (user_access('administer all menus')) {
    $form['general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['general']['mm_recycle_auto_empty'] = array(
      '#type' => 'select',
      '#title' => t('Automatic recycle bin deletion interval'),
      '#description' => t('Automatically delete content in recycle bins that has been there longer than this amount of time'),
      '#default_value' => variable_get('mm_recycle_auto_empty', 0),
      '#options' => array(
          -1                       => t('(don\'t use recycle bins)'),
          0                        => t('(never auto delete)'),
          30*60                    => t('30 minutes'),
          60*60                    => t('1 hour'),
          2*60*60                  => t('2 hours'),
          6*60*60                  => t('6 hours'),
          12*60*60                 => t('12 hours'),
          24*60*60                 => t('1 day'),
          2*24*60*60               => t('2 days'),
          3*24*60*60               => t('3 days'),
          7*24*60*60               => t('1 week'),
          2*7*24*60*60             => t('2 weeks'),
          30*24*60*60              => t('30 days'),
          60*24*60*60              => t('60 days'),
          intval(365/4*24*60*60)   => t('3 months'),
          intval(365/2*24*60*60)   => t('6 months'),
          intval(365/4*3*24*60*60) => t('9 months'),
          365*24*60*60             => t('1 year'),
      )
    );
    $form['general']['mm_access_cache_time'] = array(
      '#type' => 'select',
      '#options' => array(0 => t('(disabled)'), 30 => t('30 seconds'), 60 => t('1 minute'), 120 => t('2 minutes'), 180 => t('3 minutes'), 240 => t('4 minutes'), 300 => t('5 minutes'), 600 => t('10 minutes'), 900 => t('15 minutes'), 1200 => t('20 minutes'), 1800 => t('30 minutes'), 2700 => t('45 minutes'), 3600 => t('1 hour')),
      '#title' => t('Permissions cache time'),
      '#description' => t("Save time by caching the data needed to determine if a given user has access to a page or piece of content. The cache is automatically cleared whenever a piece content or any of its parent pages' permissions are modified. Setting this value too high can lead to lots of data being stored in the cache table."),
      '#default_value' => variable_get('mm_access_cache_time', 0),
    );
    $form['general']['mm_prevent_showpage_removal'] = array(
      '#type' => 'select',
      '#options' => array(
        MM_PREVENT_SHOWPAGE_REMOVAL_NONE => t('Do not check'),
        MM_PREVENT_SHOWPAGE_REMOVAL_WARN => t('Show a warning'),
        MM_PREVENT_SHOWPAGE_REMOVAL_HALT => t('Prevent non-admins from moving or renaming'),
      ),
      '#title' => t('Protect dynamic content (hook_mm_showpage_routing) from removal'),
      '#description' => t('This option detects when the user is about to either move or rename a page that is referred to (or is the parent of a page referred to) in a hook_mm_showpage_routing() implemention. If the "Prevent" option is chosen, users with the "administer all menus" permission will only see a warning.'),
      '#default_value' => variable_get('mm_prevent_showpage_removal', MM_PREVENT_SHOWPAGE_REMOVAL_WARN),
    );

    $form['vgroup'] = array(
      '#type' => 'fieldset',
      '#title' => t('Virtual Groups'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['vgroup']['mm_vgroup_regen_chunk'] = array(
      '#type' => 'textfield',
      '#title' => t('Chunk size used to split virtual group regeneration queries'),
      '#size' => 10, '#default_value' => variable_get('mm_vgroup_regen_chunk', 15),
      '#description' => t('To gain speed when regenerating dirty virtual groups, the separate queries are concatenated into one big query with a UNION. If the maximum SQL query buffer length is being exceeded or the large queries are taking too much memory, this value should be reduced.'),
    );
    $form['vgroup']['mm_vgroup_regen_chunks_per_run'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of concatenated virtual group queries per cron run'),
      '#size' => 10, '#default_value' => variable_get('mm_vgroup_regen_chunks_per_run', 50),
      '#description' => t('If cron is taking too long to complete, this value should be reduced. The total number of dirty virtual groups updated per cron run is this number times the chunk size.'),
    );
    $form['vgroup']['mm_vgroup_errors_email'] = array(
      '#type' => 'textfield',
      '#title' => t('Where to send virtual group warnings'),
      '#default_value' => variable_get('mm_vgroup_errors_email', variable_get('site_mail', ini_get('sendmail_from'))),
      '#description' => t('Warnings are generated for any virtual groups that decrease in size too rapidly. This is the e-mail address where warnings are sent. If left blank, the site e-mail address is used.'),
      '#maxlength' => 1024,
      '#size' => 100,
    );
    $form['vgroup']['mm_group_info_message_vgroup'] = array(
      '#type' => 'textfield',
      '#title' => t('Message displayed in group information for virtual groups'),
      '#default_value' => variable_get('mm_group_info_message_vgroup', 'This group is automatically built from data. The group ID is @gid.'),
      '#description' => t('Two variables are available for substitution: @gid is the group ID and !owner is the themed owner of the group.'),
      '#maxlength' => 1024,
      '#size' => 100,
    );
    $form['vgroup']['mm_group_info_message_regular_group'] = array(
      '#type' => 'textfield',
      '#title' => t('Message displayed in group information for regular groups'),
      '#default_value' => variable_get('mm_group_info_message_regular_group', 'This group is editable by other users. The owner is !owner. The group ID is @gid.'),
      '#description' => t('Two variables are available for substitution: @gid is the group ID and !owner is the themed owner of the group.'),
      '#maxlength' => 1024,
      '#size' => 100,
    );

    $form['mm_page'] = array(
      '#type' => 'fieldset',
      '#title' => t('Page Display'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['mm_page']['mm_hide_empty_pages'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide empty pages in menus'),
      '#default_value' => variable_get('mm_hide_empty_pages', FALSE),
    );
    $form['mm_page']['mm_enable_rss'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow content creators to control the availability of RSS feeds on a per-page basis'),
      '#default_value' => variable_get('mm_enable_rss', FALSE),
    );

    $form['mm_node'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node Display'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['mm_node']['mm_node_alert_frequency'] = array(
      '#type' => 'select',
      '#title' => t('Frequency of system alerts'),
      '#default_value' => variable_get('mm_node_alert_frequency', 'once'),
      '#options' => array(
        'once' => t('Show only once'),
        'login' => t('Show at every login'),
        'constantly' => t('Show at every page load')
      ),
    );
    if (module_exists('comment')) {
      $form['mm_node']['comments'] = array(
        '#type' => 'fieldset',
        '#title' => t('Comments'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE
      );
      $form['mm_node']['comments']['mm_show_count_instead_of_comments'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show comment count instead of full comments'),
        '#description' => t('This option takes effect when viewing pages. When checked, nodes having comments show a link with the number of comments. Clicking on the link displays the node by itself, with the comments. If unchecked, all comments are displayed under their nodes, on the same page.'),
        '#default_value' => variable_get('mm_show_count_instead_of_comments', FALSE),
      );
      $form['mm_node']['comments']['mm_finegrain_comment_readability'] = array(
        '#type' => 'checkbox',
        '#title' => t('Control comment readability at the node level'),
        '#description' => t('This option lets users say who can read comments posted to each node on an individual basis. A default value for new nodes can also be set at the page level.'),
        '#default_value' => variable_get('mm_finegrain_comment_readability', FALSE),
      );
      $labels = variable_get('mm_comments_readable_labels', array());
      $labels[] = array();
      if (count($labels) == 1) $labels[] = array();
      $form['mm_node']['comments']['mm_comments_readable_labels'] = array(
        '#type' => 'markup',
        '#prefix' => '<table><tr><th>' . t('Permission') . '</th><th>' . t('Description') . '</th></tr>',
        '#suffix' => '</table>',
        '#description' => 'foo',
        '#tree' => TRUE
      );
      $i = 0;
      foreach ($labels as $label) {
        $form['mm_node']['comments']['mm_comments_readable_labels'][$i]['perm'] = array(
          '#type' => 'textfield',
          '#prefix' => '<tr><td>',
          '#default_value' => isset($label['perm']) ? $label['perm'] : '',
          '#size' => 30,
          '#suffix' => '</td><td>',
        );
        $form['mm_node']['comments']['mm_comments_readable_labels'][$i]['desc'] = array(
          '#type' => 'textfield',
          '#suffix' => '</td></tr>',
          '#default_value' => isset($label['desc']) ? $label['desc'] : '',
          '#size' => 30,
        );
        $i++;
      }
      $form['mm_node']['comments']['desc'] = array(
        '#type' => 'item',
        '#description' => t('<p><em>Permission</em> is the label appearing on the !url page; these are effectively ANDed with the <em>access comments</em> permission. Example: <em>comments readable by everyone</em></p><p><em>Description</em> is what users see in the list of choices for setting readability at the page/node level, it answers the question, "Who can read comments?" Example: <em>everyone</em></p><p>To remove a row, clear either value. <strong>Changing data in the Permission column or removing rows may affect the readability of comments in existing nodes!</strong> Don\'t forget to update the permissions after making changes here.</p>', array('!url' => l(t('Permissions'), 'admin/people/permissions', array('fragment' => 'module-monster_menus'))))
      );
    }

    $form['mm_userhome'] = array(
      '#type' => 'fieldset',
      '#title' => t('User Home Pages'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['mm_userhome']['mm_use_user_dir'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use user home directories'),
      '#description' => t('When enabled, each newly-added user gets a personal home page, starting at !url. Note: If you disable and then re-enable this option, any users created during the time it was disabled will not have home pages.', array('!url' => l(t('users'), 'users'))),
      '#default_value' => variable_get('mm_use_user_dir', TRUE),
    );
    $form['mm_userhome']['mm_use_virtual_user_dir'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use virtual user directories'),
      '#description' => t('If you have many users, the entire user list at !url can get very long. This feature will split the users into smaller chunks, based on the letter of the alphabet with which their name begins.', array('!url' => l(t('users'), 'users'))),
      '#default_value' => variable_get('mm_use_virtual_user_dir', TRUE),
    );
    $form['mm_userhome']['mm_default_homepage'] = array(
      '#type' => 'textarea',
      '#title' => t('Default personal homepage message'),
      '#description' => t('What users see when viewing their own, empty homepage. Provide some instructions telling them how to create content.'),
      '#default_value' => variable_get('mm_default_homepage', ''),
    );

    $form['mm_username'] = array(
      '#type' => 'fieldset',
      '#title' => t('User Names'),
      '#description' => t('These names are displayed in content attribution lines and group membership lists.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );
    $form['mm_username']['mm_username_anon'] = array(
      '#type' => 'textfield',
      '#title' => t('Long name of the Anonymous user'),
      '#default_value' => variable_get('mm_username_anon', t('Anonymous user')),
    );
    $form['mm_username']['mm_username_admin'] = array(
      '#type' => 'textfield',
      '#title' => t('Long name of the Administrator user'),
      '#default_value' => variable_get('mm_username_admin', t('Administrator')),
    );
    $form['mm_username']['mm_username_disabled'] = array(
      '#type' => 'textfield',
      '#title' => t('Long name for all disabled users'),
      '#default_value' => variable_get('mm_username_disabled', t('Inactive user')),
    );

    $form['mm_nodelist'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node Chooser'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['mm_nodelist']['mm_nodelist_pager_limit'] = array(
      '#type' => 'select',
      '#title' => t('Number of nodes to show per page in the node chooser'),
      '#options' => array(
        10 => 10,
        20 => 20,
        50 => 50,
        100 => 100
      ),
      '#default_value' => variable_get('mm_nodelist_pager_limit', 20),
    );

    $form['mm_sitemap'] = array(
      '#type' => 'fieldset',
      '#title' => t('Site Map'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['mm_sitemap']['help'] = array(
      '#markup'=> t('<p>Monster Menus will respond to a request for <code>/-mm-sitemap</code> by generating a standard <code>/sitemap.xml</code> file. You should call this URL periodically, in the same way you do cron.php, but less frequently. Once it has been generated, the <code>sitemap.xml</code> file contains links to any pages that are publicly readable, and not hidden or recycled.</p>'),
    );
    $form['mm_sitemap']['mm_sitemap_exclude_list'] = array(
      '#type' => 'textarea',
      '#wysiwyg' => FALSE,
      '#title' => t('Paths to exclude from the sitemap'),
      '#default_value' => join("\n", variable_get('mm_sitemap_exclude_list', array())),
      '#description' => t('A list of paths, one per line, which should not be part of the <code>sitemap.xml</code>. Do not include leading or trailing slashes. Example: <code>foo/bar/baz</code>'),
    );
    $form['mm_sitemap']['mm_sitemap_max_level'] = array(
      '#type' => 'select',
      '#options' => array(-1 => t('(disabled)'), 0 => t('(Home only)'), 1 => t('1 level'), 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8, 9 => 9, 10 => 10, 11 => 11, 12 => 12, 13 => 13, 14 => 14, 15 => 15, 1000 => t('(unlimited)')),
      '#title' => t('Number of levels to generate'),
      '#default_value' => variable_get('mm_sitemap_max_level', MM_SITEMAP_MAX_LEVEL_DEFAULT),
      '#description' => t('The maximum depth in the tree to use for the sitemap. Set this too high and, on a large site, your sitemap.xml file may become too large to be useful.'),
    );

    $form['#submit'][] = 'mm_admin_config_submit';
  }

  mm_module_invoke_all_array('mm_config_alter', array(&$form));

  if ($form) return system_settings_form($form);

  $form['msg'] = array('#markup' => t('You are not allowed to change any of the settings.'));
  return $form;
}

function mm_admin_config_validate($form, &$form_state) {
  foreach ($form_state['values']['mm_comments_readable_labels'] as $index => $row)
    if (empty($row['perm']) || empty($row['desc']))
      unset($form_state['values']['mm_comments_readable_labels'][$index]);

  foreach (explode(',', $form_state['values']['mm_vgroup_errors_email']) as $email) {
    if ($error = user_validate_mail(trim($email))) {
      form_set_error('mm_vgroup_errors_email', $error);
    }
  }

  $form_state['values']['mm_sitemap_exclude_list'] = preg_split('{/*\s*[\r\n]+\s*/*}', trim($form_state['values']['mm_sitemap_exclude_list'], " \r\n/"), -1, PREG_SPLIT_NO_EMPTY);
}

function mm_admin_config_submit($form, &$form_state) {
  // If mm_finegrain_comment_readability has never been set before, create the
  // default settings using the current 'access comments' setting.
  if ($form_state['values']['mm_finegrain_comment_readability'] && !variable_get('mm_finegrain_comment_readability_ever_set', FALSE)) {
    variable_set('mm_finegrain_comment_readability_ever_set', TRUE);

    $select = db_select('role_permission', 'p');
    $select->condition('p.permission', 'access_comments');
    $select->addField('p', 'rid');
    $select->addExpression(':comment_read', 'permission', array(':comment_read' => MM_COMMENT_READABILITY_DEFAULT));
    $select->addField('p', 'module');
    db_insert('role_permission')->from($select)->execute();

    $select = db_select('role', 'r');
    $select->leftJoin('role_permission', 'rp', 'rp.rid = r.rid AND permission = :permission', array(':permission' => 'access comments'));
    $select->isNull('rp.permission');
    $select->addField('r', 'rid');
    $select->addExpression(':comment_read', 'permission', array(':comment_read' => 'access comments'));
    $select->addExpression(':module', 'module', array(':module' => 'comment'));
    db_insert('role_permission')->from($select)->execute();

    drupal_set_message(t('Because you enabled the <em>Control comment readability at the node level</em> setting for the first time, the <em>access comments</em> permission has been enabled for all roles. This is necessary in order for this feature to work.'));
    drupal_set_message(t('You should now go to !url to set the roles for each permission you just created.', array('!url' => l(t('Permissions'), 'admin/people/permissions', array('fragment' => 'module-monster_menus')))));
  }

  // If changing to/from disabled sitemap, we need to rebuild the menu.
  if ($form_state['values']['mm_sitemap_max_level'] != $form['mm_sitemap']['mm_sitemap_max_level']['#default_value'] && ($form_state['values']['mm_sitemap_max_level'] < 0 || $form['mm_sitemap']['mm_sitemap_max_level']['#default_value'] < 0)) {
    variable_set('mm_sitemap_max_level', $form_state['values']['mm_sitemap_max_level']);
    menu_rebuild();
  }
}

/**
 * Verify database relationships
 */
function mm_admin_verify_integrity($form, &$form_state) {
  foreach (module_list() as $module)
    module_load_include('install', $module);

  $tests = module_invoke_all('mm_verify_integrity');
  if (!$tests) return t('There are no tests to perform.');

  $i = 0;
  foreach ($tests as $heading => $list) {
    $fieldset = "fs$i";
    $form[$fieldset] = array(
      '#type' => 'fieldset',
      '#title' => $heading,
      '#collapsible' => TRUE,
    );
    $form[$fieldset]['header'] = array(
      '#type' => 'value',
      '#value' => array(
        array('data' => t('Test')),
        array('data' => t('Result')),
      ),
    );

    foreach ($list as $description => $test) {
      $error = '';
      try {
        $result = db_query("SELECT COUNT(*) FROM $test")->fetchField();
      } catch (PDOException $e) {
        $result = $e->getCode();
        $error = $e->getMessage();
      }
      $form[$fieldset][] = array(
        'test' => array('#markup' => check_plain(Database::getConnection()->prefixTables($test))),
        'description' => array('#markup' => check_plain($description)),
        'result' => array('#markup' => $result),
        'error' => array('#markup' => check_plain($error)),
      );
    }
    $i++;
  }
  return $form;
}

function mm_admin_list_sites($form, &$form_state) {
  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array('data' => t('Name'), 'field' => 'name'),
      array('data' => t('Current Homepage')),
      array('data' => t('Operations'), 'colspan' => '2'),
    ),
  );
  $result = db_select('mm_tree', 't')
    ->fields('t', array('mmtid', 'name'))
    ->where("t.parent = 1 AND t.name NOT LIKE '.%'")
    ->extend('PagerDefault')
    ->limit(20)
    ->execute();

  $destination = drupal_get_destination();
  $page_mmtids = array();
  foreach ($result as $site) {
    $form['name'][$site->mmtid] = array('#markup' => l(mm_content_get_name($site->mmtid), "mm/$site->mmtid"));
    $form['operations'][$site->mmtid][] = array('#markup' => l(t('edit'), 'mm/'. $site->mmtid . '/settings', array('query' => $destination)));
    if ($site->mmtid != mm_home_mmtid()) {
      $form['operations'][$site->mmtid][] = array('#markup' => l(t('delete'), 'mm/'. $site->mmtid . '/settings/delete', array('query' => $destination)));
    }
    $page_mmtids[$site->mmtid] = '';
  }
  $form['current'] = array(
    '#type' => 'radios',
    '#options' => $page_mmtids,
    '#default_value' => mm_home_mmtid(),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set the current homepage'),
  );
  $form['pager'] = array('#markup' => theme('pager', array('tags' => NULL, 'element' => 0)));

  return $form;
}

function mm_admin_list_sites_submit($form, &$form_state) {
  $conf_path = conf_path();
  $list = variable_get('mm_home_mmtid', MM_HOME_MMTID_DEFAULT);
  if (!is_array($list)) {
    $list = array($conf_path => $list);
  }
  $list[$conf_path] = $form_state['values']['current'];
  variable_set('mm_home_mmtid', $list);
  drupal_set_message(t('The current site homepage has been changed.'));
}

function mm_admin_edit_site() {
  module_load_include('inc', 'monster_menus', 'mm_ui_content');
  return mm_ui_content(1, 'sub');
}

function mm_admin_list_blocks($form, &$form_state) {
  $form['header'] = array(
    '#type' => 'value',
    '#value' => array(
      array('data' => t('Name'), 'field' => 'name'),
      array('data' => t('Title'), 'field' => 'title'),
      array('data' => t('Flags')),
      array('data' => t('Operations'), 'colspan' => '2'),
    ),
  );
  $result = db_select('mm_block', 'b')
    ->fields('b')
    ->extend('PagerDefault')
    ->limit(20)
    ->execute();

  $destination = drupal_get_destination();
  foreach ($result as $block) {
    $form['name'][$block->bid] = array('#markup' => check_plain($block->name));
    $form['title'][$block->bid] = array('#markup' => check_plain($block->title));
    $flags = array();
    $fields = array(
      'title_is_cat' => t('Use page as title'),
      'allow_rss' => t('RSS feed'),
      'show_node_contents' => t('Show node contents'),
      'admin_only' => t('Admin. only')
    );
    foreach ($fields as $field => $label) {
      if ($block->$field) $flags[] = $label;
    }
    $form['flags'][$block->bid] = array('#markup' => join(', ', $flags));
    $form['operations'][$block->bid][] = array(
      '#type' => 'link',
      '#title' => t('edit'),
      '#href' => 'admin/structure/block/mm/edit/' . $block->bid,
      '#options' => array('query' => $destination),
    );
    $form['operations'][$block->bid][] = array(
      '#type' => 'link',
      '#title' => t('delete'),
      '#href' => 'admin/structure/block/mm/delete/' . $block->bid,
      '#options' => array('query' => $destination),
    );
  }
  $form['pager'] = array('#markup' => theme('pager', array('tags' => NULL, 'element' => 0)));

  return $form;
}

function mm_admin_edit_block($form, &$form_state, $bid = NULL) {
  $blocks = mm_content_get_blocks();
  if (isset($bid) && isset($blocks[$bid])) {
    $form['bid'] = array('#type' => 'value', '#value' => $bid);
    $block = $blocks[$bid];
  }
  else {
    $block = array(
      'info' => '',
      'title' => '',
      'help' => '',
      'title_is_cat' => FALSE,
      'allow_rss' => FALSE,
      'show_node_contents' => FALSE,
      'admin_only' => FALSE,
    );
  }

  $x = mm_ui_strings(FALSE);
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $block['info'],
    '#size' => 40, '#maxlength' => 128,
    '#required' => TRUE,
    '#description' => t('The name visible to users in the Appearance settings for a @thing', $x)
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $block['title'],
    '#size' => 40, '#maxlength' => 256,
    '#description' => t('An optional title to be placed above the block')
  );
  $form['help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#default_value' => $block['help'],
    '#rows' => 4,
    '#description' => t('The text which appears in the tooltip, describing when to use this block')
  );
  $form['title_is_cat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use @thing as title', $x),
    '#default_value' => $block['title_is_cat'],
    '#description' => t('If checked, the title of the block\'s @thing is used as the block title instead of the Title field, above.', $x)
  );
  $form['allow_rss'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow RSS feed'),
    '#default_value' => $block['allow_rss'],
    '#description' => t('If checked, automatically generate an additional RSS feed <code>&lt;link&gt;</code> tag for this block.')
  );
  $form['show_node_contents'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show node contents'),
    '#default_value' => $block['show_node_contents'],
    '#description' => t('If checked, show the contents of all nodes in this @thing instead of links.', $x)
  );
  $form['admin_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Admin. only'),
    '#default_value' => $block['admin_only'],
    '#description' => t('If checked, only users with the "administer all menus" permission can assign @things to this block.', $x)
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($block) ? t('Save settings') : t('Add block')
  );
  return $form;
}

function mm_admin_edit_block_submit($form, &$form_state) {
  $form_vals =& $form_state['values'];
  $fields = array(
    'name' => $form_vals['name'],
    'title' => $form_vals['title'],
    'help' => $form_vals['help'],
    'title_is_cat' => $form_vals['title_is_cat'],
    'allow_rss' => $form_vals['allow_rss'],
    'show_node_contents' => $form_vals['show_node_contents'],
    'admin_only' => $form_vals['admin_only'],
  );
  if (isset($form_vals['bid'])) {
    db_update('mm_block')
      ->fields($fields)
      ->condition('bid', $form_vals['bid'])
      ->execute();
    if (drupal_get_destination()) return;
  }
  else {
    db_insert('mm_block')
      ->fields($fields)
      ->execute();
    drupal_set_message(t('The new block has been created.'));
  }
  $form_state['redirect'] = 'admin/structure/block/mm';
}

function mm_admin_delete_block($form, &$form_state, $bid = NULL) {
  $blocks = mm_content_get_blocks();
  if (isset($bid) && isset($blocks[$bid])) {
    $form['bid'] = array('#type' => 'value', '#value' => $bid);
    $block = $blocks[$bid];

    $msg = t('Are you sure you want to delete the block %name?', array('%name' => $block['info']));
    $desc = '';
    $count = db_select('mm_tree_block', 'b')
      ->condition('bid', $bid)
      ->countQuery()->execute()->fetchField();
    if ($count) {
      $desc = format_plural($count, ' <font color="red">There is a @thing using this block.</font>', ' <font color="red">There are @count @things using this block.</font>', mm_ui_strings(FALSE));
    }

    module_load_include('inc', 'monster_menus', 'mm_ui_content_delete');
    return confirm_form($form, $msg, 'admin/structure/block/mm', $desc, t('Delete'), NULL);
  }
  else {
    drupal_set_message(t('Block not found'));
  }
}

function mm_admin_delete_block_submit($form, &$form_state) {
  db_delete('mm_block')
    ->condition('bid', $form_state['values']['bid'])
    ->execute();
  db_delete('mm_tree_block')
    ->condition('bid', $form_state['values']['bid'])
    ->execute();
  drupal_set_message(t('The block has been deleted.'));
  if (drupal_get_destination()) return;
  $form_state['redirect'] = 'admin/structure/block/mm';
}

/**
 * Display a list of links to popup tree browsers
 *
 * @return
 *   The HTML code for the links
 */
function mm_admin_browse() {
  mm_static('admin_browse', TRUE);
  $links = array();
  $list = array(
    t('Permission groups') => array(MM_BROWSER_MODE_ADMIN_GROUP, user_access('administer all groups'), mm_content_groups_mmtid()),
    t('Top-level menus') => array(MM_BROWSER_MODE_ADMIN_PAGE, user_access('administer all menus'), mm_home_mmtid()),
    t('User menus') => array(MM_BROWSER_MODE_ADMIN_USER, user_access('administer all users'), mm_content_users_mmtid()),
    t('Entire tree') => array(MM_BROWSER_MODE_ADMIN_PAGE, _mm_menu_access_all_admin(), 1),
  );

  foreach ($list as $text => $item) {
    if ($item[1]) {
      $links[] = array(
        'title' => $text,
        'href' => '#',
        'external' => TRUE,
        'attributes' => array(
          'onclick' => "return Drupal.MMAdminBrowse($item[2], '$item[0]', $item[2], '$text')",
        ),
      );
    }
  }

  return theme('links', array('links' => $links, 'attributes' => array()));
}

/**
 * Print a CSV dump of the entire MM tree
 */
function mm_admin_dump($start = 1) {
  class dumpIter {
    protected $fp;

    public function __construct() {
      $this->fp = fopen('php://output', 'w');
      fputcsv($this->fp, array(
        'visname', 'mmtid', 'level', 'name', 'alias', 'default_mode', 'owner', 'theme',
        'alw_theme', 'alw_type', 'hidden', 'groups_w', 'users_w', 'groups_a',
        'users_a', 'groups_u', 'users_u', 'groups_r', 'users_r', 'flags', 'block'));
    }

    public function iterate($item) {
      $visname = str_repeat('>', $item->level) . ' ' . $item->name;

      $allowed_themes = array();
      $allowed_node_types = array();
      $result = db_select('mm_cascaded_settings', 's')
        ->fields('s')
        ->condition('mmtid', $item->mmtid)
        ->condition('data_type', array('allowed_themes', 'allowed_node_types'), 'IN')
        ->execute();
      foreach ($result as $r) {
        if ($r->data_type == 'allowed_themes') {
          $allowed_themes[] = $r->data;
        }
        elseif ($r->data_type == 'allowed_node_types') {
          $allowed_node_types[] = $r->data;
        }
      }

      $groups = $users = array(
        MM_PERMS_READ  => array(),
        MM_PERMS_WRITE => array(),
        MM_PERMS_SUB   => array(),
        MM_PERMS_APPLY => array(),
      );
      $select = db_select('mm_tree', 't');
      $select->join('mm_tree_access', 'a', 'a.mmtid=t.mmtid');
      $select->leftJoin('mm_tree', 't2', 'a.gid=t2.mmtid');
      $result = $select->fields('t2', array('mmtid', 'name'))
        ->fields('a', array('mode'))
        ->condition('t2.mmtid', '0', '>=')
        ->condition('a.mmtid', $item->mmtid)
        ->orderBy('t2.name')
        ->execute();
      foreach ($result as $r) {
        $groups[$r->mode][$r->mmtid] = $r->name;
      }

      $select = db_select('mm_tree', 't');
      $select->join('mm_tree_access', 'a', 'a.mmtid=t.mmtid');
      $result = $select->fields('a', array('mode', 'gid'))
        ->condition('a.gid', '0', '<')
        ->condition('a.mmtid', $item->mmtid)
        ->execute();
      foreach ($result as $r) {
        $u = mm_content_get_users_in_group($r->gid, NULL, TRUE, 5);
        if (!is_null($u)) $users[$r->mode] = $u;
      }

      fputcsv($this->fp, array($visname, $item->mmtid, $item->level, $item->name,
        $item->alias, $item->default_mode, $item->uid, $item->theme,
        join(', ', $allowed_themes), join(', ', $allowed_node_types), $item->hidden,
        $this->dump($groups[MM_PERMS_WRITE]), $this->dump($users[MM_PERMS_WRITE]),
        $this->dump($groups[MM_PERMS_SUB]), $this->dump($users[MM_PERMS_SUB]),
        $this->dump($groups[MM_PERMS_APPLY]), $this->dump($users[MM_PERMS_APPLY]),
        $this->dump($groups[MM_PERMS_READ]), $this->dump($users[MM_PERMS_READ]),
        join(',', array_keys($item->flags)), $item->bid));

      return 1;
    }

    protected function dump($arr) {
      $out = array();
      foreach ($arr as $id => $name) {
        $out[] = $id ? "$name [$id]" : $name;
      }
      return join(', ', $out);
    }
  }

  if (!drupal_is_cli()) {
    $GLOBALS['devel_shutdown'] = TRUE;    // prevent the devel module from outputting
    header('Content-type: text/plain');
  }
  $params = array(
    MM_GET_TREE_ITERATOR =>      new dumpIter(),
    MM_GET_TREE_RETURN_BLOCK =>  TRUE,
    MM_GET_TREE_RETURN_FLAGS =>  TRUE,
  );
  mm_content_get_tree($start, $params);
  if (!drupal_is_cli()) {
    exit();
  }
}

function mm_admin_find_user($form, &$form_state) {
  _mm_ui_userlist_setup(array(0 => ''), $form, 'userlist', t("User's name:"), TRUE, '');
  $form['userlist-choose']['#title'] = '';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('View/edit user')
  );
  return $form;
}

function mm_admin_find_user_submit($form, &$form_state) {
  $uid = mm_ui_mmlist_key0($form_state['values']['userlist']);

  if (isset($uid)) {
    $form_state['redirect'] = array("user/$uid/edit", array('query' => array('destination' => 'admin/people/by-uid')));
  }
}

/**
 * Check all mm_tree.sort_idx entries and optionally fix any that seem to be
 * incorrect.
 *
 * @param $fix
 *   If TRUE, queue any wrong entries to be fixed during monster_menus_exit().
 * @param $output_mode
 *   Sets the method for outputting any error messages. Currently, these methods
 *   are supported:
 *   - watchdog:  The Drupal watchdog() function (suitable for cron)
 *   - message:   The drupal_set_message() function, which sends output to the
 *                current web page (do not use in a batch script). This is the
 *                default method, and is used by the menu entry.
 *   - print:     Print the messages to standard i/o
 *   - drush_log: Use when called by drush.
 * @return
 *   If $output_mode is 'message', a count of the number of errors, otherwise,
 *   nothing.
 */
function mm_admin_validate_sort_index($fix = FALSE, $output_mode = 'message') {
  function _mm_admin_validate_sort_index_output($output_mode, $fix, $message, $strings) {
    static $errors;

    if (is_null($message)) {
      if ($output_mode == 'message') {
        if ($errors) {
          if ($fix) {
            return format_plural($errors, '1 error was fixed.', '@count errors were fixed.');
          }
          return array(
            '#type' => 'link',
            '#title' => t('Click here to fix all errors'),
            '#href' => 'admin/mm/sort/1',
          );
        }
        return t('No errors found.');
      }
      return;
    }

    if ($errors++ == MM_ADMIN_VALIDATE_SORT_INDEX_MAX) {
      $message = 'Only the first !num messages are shown.';
      $strings = array('!num' => MM_ADMIN_VALIDATE_SORT_INDEX_MAX);
    }
    elseif ($errors > MM_ADMIN_VALIDATE_SORT_INDEX_MAX) return;

    switch ($output_mode) {
      case 'watchdog':
        watchdog('mm', $message, $strings, WATCHDOG_ERROR);
        break;

      case 'message':
        if (!$fix) {
          drupal_set_message(t($message, $strings), 'error');
        }
        break;

      case 'print':
        print t($message, $strings) . "\n";
        break;

      case 'drush_log':
        drush_log(t($message, $strings), $fix ? 'fixed' : 'ok');
        break;
    }
  }

  function _mm_admin_validate_sort_index_mmtid_link($output_mode, $mmtid) {
    if ($output_mode == 'print' || $output_mode == 'drush_log') return $mmtid;
    return l($mmtid, "mm/$mmtid");
  }

  $parents = $parent_mmtids = array();
  $skip_bad = '';
  $sibling = NULL;

  $q = db_query("SELECT t.*, (SELECT GROUP_CONCAT(p.parent ORDER BY p.depth) FROM {mm_tree_parents} p WHERE p.mmtid = t.mmtid) AS tree_parents FROM (SELECT :mmtid1 AS mmtid UNION SELECT p.mmtid FROM {mm_tree_parents} p WHERE p.parent = :mmtid2) x INNER JOIN {mm_tree} t ON t.mmtid = x.mmtid ORDER BY sort_idx", array(':mmtid1' => 1, ':mmtid2' => 1));
  foreach ($q as $r) {
    if ($r->sort_idx_dirty) {
      $skip_bad = $r->sort_idx;
      _mm_admin_validate_sort_index_output($output_mode, $fix, 'mmtid=!m (parent=!par) is marked as dirty. Updating now.', array('!m' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!par' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->parent)));
      // An update was previously missed, so definitely do it now
      mm_content_update_sort_queue($r->parent);
    }

    if ($r->mmtid == 1) {
      $parent = $r;
    }
    else {
      $parent_mmtids[$r->mmtid] = $r->parent;
      $is_bad = FALSE;
      if ($r->parent == $prev->mmtid || isset($sibling) && $r->parent == $sibling->mmtid) {
        // Going deeper in tree
        $parents[] = isset($parent) ? $parent : FALSE;
        $parent = $r->parent == $prev->mmtid ? $prev : $sibling;
        $sibling = NULL;
      }
      else {
        if (strncmp($skip_bad, $r->sort_idx, strlen($skip_bad))) $skip_bad = '';

        $old_sib = isset($sibling) ? $sibling : NULL;
        $old_parents = $parents;
        $old_parent = $parent;
        // See if we are going back up the tree
        while (count($parents) && $parent->mmtid != $r->parent) {
          $sibling = $parent;
          $parent = array_pop($parents);
        }

        // The parent was never found
        if (!is_object($parent) || $parent->mmtid != $r->parent) {
          // If the item's parent exists, this means the sort index is off
          if (mm_content_get($r->parent)) {
            // The index is bad, but don't complain here; do it later on
            $sibling = $old_sib;
            $parents = $old_parents;
            $parent = $old_parent;
            $is_bad = TRUE;
          }
          else {
            _mm_admin_validate_sort_index_output($output_mode, FALSE, 'MM Tree entry !mmtid has missing parent !parent. Testing halted.', array('!mmtid' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!parent' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->parent)));
            return _mm_admin_validate_sort_index_output($output_mode, FALSE, NULL, NULL);
          }
        }
      }

      if (empty($skip_bad)) {
        $parents_str = '';
        $fixed_parents = array();
        $m = $r->mmtid;
        while (isset($parent_mmtids[$m])) {
          $m = $parent_mmtids[$m];
          $fixed_parents[] = $m;
          $parents_str = $m . ($parents_str ? ',' : '') . $parents_str;
        }
        if ($m != 1) {
          _mm_admin_validate_sort_index_output($output_mode, $fix, 'Entry at mmtid=!m has impossible parents [...!pars]. mm_tree_parents says they should be [!pars2]. This condition cannot be fixed automatically.', array('!m' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!pars' => $parents_str, '!pars2' => $r->tree_parents));
          $is_bad = TRUE;
        }
        elseif ($parents_str != $r->tree_parents) {
          _mm_admin_validate_sort_index_output($output_mode, $fix, 'Entry at mmtid=!m has parents [!pars] which is inconsistent with mm_tree_parents [!pars2].', array('!m' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!pars' => $parents_str, '!pars2' => $r->tree_parents));
          if ($fix) mm_content_update_parents($r->mmtid, array_reverse($fixed_parents));
          // Intentionally don't set $is_bad, since we don't want to skip kids
        }
        elseif ($r->parent != $parent->mmtid) {
          _mm_admin_validate_sort_index_output($output_mode, $fix, 'Entry at mmtid=!m has a parent=!par that is inconsistent with its sort order.', array('!m' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!par' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->parent)));
        }
        elseif (strlen($r->sort_idx) - strlen($parent->sort_idx) != MM_CONTENT_BTOA_CHARS) {
          // The length of this entry's index is not correct, relative to its parent
          $msg = strlen($r->sort_idx) > strlen($parent->sort_idx) ? 'Sort index at mmtid=!m (parent=!par) is too long.' : 'Sort index at mmtid=!m (parent=!par) is too short.';
          _mm_admin_validate_sort_index_output($output_mode, $fix, $msg, array('!m' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!par' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->parent)));
          $is_bad = TRUE;
        }
        else {
          if ($r->sort_idx == $prev->sort_idx) {
            // Entry has same index as its predecessor
            _mm_admin_validate_sort_index_output($output_mode, $fix, 'mmtid=!m1 (parent=!par1) and mmtid=!m2 (parent=!par2) have the same sort index.', array('!m1' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!par1' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->parent), '!m2' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $prev->mmtid), '!par2' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $prev->parent)));
            $is_bad = TRUE;
          }
          elseif (strncmp($r->sort_idx, $parent->sort_idx, strlen($parent->sort_idx))) {
            // The indices should match, up to the parent's length
            _mm_admin_validate_sort_index_output($output_mode, $fix, 'Sort indices of child and parent do not match at mmtid=!m (parent=!par).', array('!m' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!par' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->parent)));
            $is_bad = TRUE;
          }
          elseif (isset($sibling)) {
            if (substr($r->sort_idx, -MM_CONTENT_BTOA_CHARS) == substr($sibling->sort_idx, -MM_CONTENT_BTOA_CHARS)) {
              // Entry has same index as its last sibling
              _mm_admin_validate_sort_index_output($output_mode, $fix, 'Siblings mmtid=!m1 and mmtid=!m2 have the same sort index (their parent=!par).', array('!m1' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!m2' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $sibling->mmtid), '!par' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->parent)));
              $is_bad = TRUE;
            }
            else {
              $msg = '';
              if ($sibling->name == MM_ENTRY_NAME_RECYCLE) {
                $msg = 'Entry mmtid=!m1 comes after mmtid=!m2, which is a recycle bin (their parent=!par).';
              }
              elseif ($r->name != MM_ENTRY_NAME_RECYCLE && !$r->hidden) {
                if ($sibling->hidden) {
                  $msg = 'Entry mmtid=!m1 is not hidden, but it comes after mmtid=!m2 which is (their parent=!par).';
                }
                elseif ($r->weight < $sibling->weight) {
                  $msg = 'Entry mmtid=!m1 has a weight that is lower than mmtid=!m2 (their parent=!par).';
                }
                elseif ($r->weight == $sibling->weight && strcasecmp($r->name, $sibling->name) < 0) {
                  // The simple cases are tested above, but strcasecmp() is not
                  // the same as collation-based comparisons in the DB, so upon
                  // failure, ask the DB if the sort is correct
                  $query = db_select('mm_tree', 't1');
                  $query->join('mm_tree', 't2', 't2.parent = t1.parent');
                  $query->condition('t1.mmtid', $r->mmtid)
                    ->condition('t2.mmtid', $sibling->mmtid)
                    ->addExpression('t1.name < t2.name');
                  if ($query->execute()->fetchField()) {
                    $msg = 'Entry mmtid=!m1 comes after mmtid=!m2, even though it has a name that is earlier alphabetically (their parent=!par).';
                  }
                }
              }

              if ($msg) {
                _mm_admin_validate_sort_index_output($output_mode, $fix, $msg, array('!m1' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->mmtid), '!m2' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $sibling->mmtid), '!par' => _mm_admin_validate_sort_index_mmtid_link($output_mode, $r->parent)));
                $is_bad = TRUE;
              }
            }
          }
        }
      }

      if ($is_bad) {
        if ($fix) mm_content_update_sort_queue($r->parent, NULL, TRUE);
        if (empty($skip_bad)) $skip_bad = $r->sort_idx;
      }
      $sibling = $r;
    }
    $prev = $r;
  }

  return _mm_admin_validate_sort_index_output($output_mode, $fix, NULL, NULL);
}

function mm_admin_reassign_content($form, &$form_state) {
  $form['old_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Old username'),
  );
  $form['new_user'] = array(
    '#type' => 'textfield',
    '#title' => t('New username'),
  );
  $form['migrate_user_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Migrate content in personal user space'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function mm_admin_reassign_content_validate($form, &$form_state) {
  $old = user_load_by_name($form_state['values']['old_user']);
  $new = user_load_by_name($form_state['values']['new_user']);
  if (empty($old)) form_set_error('old_user', t('Could not find a match for old user.'));
  if (empty($new)) form_set_error('new_user', t('Could not find a match for new user.'));
  if ($form_state['values']['migrate_user_content'] == 1) {
    if (empty($old->user_mmtid)) {
      form_set_error('migrate_user_content', t('Unable to find the current user\'s home directory.  No further processing was done.'));
    }
  }
}

function mm_admin_reassign_content_submit($form, &$form_state) {
  $old = user_load_by_name($form_state['values']['old_user']);
  $new = user_load_by_name($form_state['values']['new_user']);

  $node_update = db_update('node')
    ->fields(array('uid' => $new->uid))
    ->condition('uid', $old->uid);

  $mmtids = array();
  if (empty($form_state['values']['migrate_user_content']) && !empty($old->user_mmtid)) {
    $result = db_select('mm_tree_parents', 'p')
      ->fields('p', array('mmtid'))
      ->distinct()
      ->condition('p.parent', $old->user_mmtid)
      ->execute();
    $mmtids[] = $old->user_mmtid;
    foreach ($result as $item) {
      $mmtids[] = $item->mmtid;
    }
    $node_update->condition(
      'nid',
      db_select('mm_node2tree', 'm')
        ->fields('m', array('nid'))
        ->condition('m.mmtid', $mmtids, 'NOT IN'),
      'IN');
  }

  $query = db_select('mm_tree', 't')
    ->fields('t', array('mmtid'))
    ->condition('t.uid', $old->uid);
  if ($mmtids) {
    $query->condition('t.mmtid', $mmtids, 'NOT IN');
  }
  $result = $query->execute();
  $i = 0;
  foreach ($result as $item) {
    mm_content_update_quick(array('uid' => $new->uid), array('mmtid' => $item->mmtid));
    $i++;
  }
  drupal_set_message(t('Groups and pages that have switched owners: @count', array('@count' => $i)));

  $num_updated = $node_update->execute();
  drupal_set_message(t('Nodes that have switched users: @count', array('@count' => $num_updated)));

  // Find "individuals" groups that are about to become empty, so they can be
  // removed from mm_node_write.
  $empty_groups = db_query(
    'SELECT g.gid FROM {mm_node_write} nw ' .
    'INNER JOIN {mm_group} g ON g.gid = nw.gid ' .
    'WHERE g.gid < 0 AND g.uid = :uid AND (SELECT COUNT(*) FROM {mm_group} WHERE gid = g.gid) = 1',
    array(':uid' => $old->uid))
    ->fetchCol();
  $num_deleted = db_delete('mm_group')
    ->condition('uid', $old->uid)
    ->execute();
  drupal_set_message(t('Number of groups the user has been removed from: @count', array('@count' => $num_deleted)));
  if ($empty_groups) {
    db_delete('mm_node_write')
      ->condition('gid', $empty_groups)
      ->execute();
  }

  $vgroups = '';
  foreach (mm_content_get_uids_in_group(NULL, $old->uid, FALSE, TRUE, FALSE) as $gid) {
    $vgroups .= '<br />' . l(mm_content_get_name($gid), "mm/$gid");
  }
  if ($vgroups) {
    drupal_set_message(t('The user is still a member of these virtual group(s):') . $vgroups);
  }
}

function mm_admin_export() {
  module_load_include('inc', 'monster_menus', 'mm_import_export');
  return drupal_get_form('mm_export_form');
}

function mm_admin_import() {
  module_load_include('inc', 'monster_menus', 'mm_import_export');
  return drupal_get_form('mm_import_form');
}

function mm_admin_fix_node_urls($form, &$form_state) {
  $form['old'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 120,
    '#maxlength' => 1024,
    '#title' => t('Old path'),
    '#default_value' => isset($_SESSION['mm_fix_node_urls']['old']) ? $_SESSION['mm_fix_node_urls']['old'] : base_path() . 'foo/bar',
    '#description' => t('URLs starting at this location (with or without the host name, http:, or https:) will be rewritten'),
  );
  $form['new'] = array(
    '#type' => 'mm_catlist',
    '#required' => TRUE,
    '#mm_list_min' => 1,
    '#mm_list_max' => 1,
    '#mm_list_selectable' => '',
    '#title' => t('New location of the above path:'),
    '#default_value' => isset($_SESSION['mm_fix_node_urls']['new']) ? $_SESSION['mm_fix_node_urls']['new'] : array(),
    '#description' => t('Rewrite starting at this location.'),
  );
  $form['advanced'] = array(
    '#title' => t('Advanced options'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These settings are saved as the default for all future uses.'),
    'hostname_regex' => array(
      '#type' => 'textfield',
      '#size' => 120,
      '#maxlength' => 1024,
      '#title' => t('Regular expression to match host name'),
      '#description' => t('When searching for URLs to change, this regular expression is used to match host names. If your web server might be referred to by multiple host names in absolute URLs, this can be used to rewrite all of them to a single value. If left blank, the current host name will be used. Any dots must be escaped as: <code>\\.</code>'),
      '#default_value' => variable_get('mm_fix_node_urls_hostname_regex', preg_quote($_SERVER['HTTP_HOST'])),
    ),
    'chunksize' => array(
      '#type' => 'textfield',
      '#required' => TRUE,
      '#size' => 5,
      '#title' => t('Number of nodes to scan per run'),
      '#description' => t('The number of nodes to search per AJAX call. Increasing this number can improve efficiency, but setting it too high can result in timeouts, especially when the <em>Fix URLs</em> button is used.'),
      '#default_value' => variable_get('mm_fix_node_urls_chunksize', 50),
    ),
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'test' => array(
      '#type' => 'submit',
      '#value' => t('Test Fixing of URLs'),
    ),
    'go' => array(
      '#type' => 'submit',
      '#value' => t('Fix URLs'),
    ),
  );

  if (isset($_SESSION['mm_fix_node_urls']['result'])) {
    $form['result'] = array(
      '#type' => 'fieldset',
      '#title' => t('Result'),
      $_SESSION['mm_fix_node_urls']['result'],
    );
    unset($_SESSION['mm_fix_node_urls']['result']);
  }

  return $form;
}

function mm_admin_fix_node_urls_validate($form, &$form_state) {
  if (!empty($form_state['values']['hostname_regex'])) {
    $had_exception = FALSE;
    set_error_handler(function() use (&$had_exception) {
      $had_exception = TRUE;
    });
    try {
      preg_match('{' . $form_state['values']['hostname_regex'] . '}', 'x');
    }
    catch (Exception $e) {
      $had_exception = TRUE;
    }
    restore_error_handler();
    if (!empty($had_exception) || preg_last_error() != PREG_NO_ERROR) {
      form_set_error('hostname_regex', t('There was an error in the regular expression you entered.'));
    }
  }
}

function mm_admin_fix_node_urls_submit($form, &$form_state) {
  $vals = $form_state['values'];
  $_SESSION['mm_fix_node_urls']['result'] = array();

  $_SESSION['mm_fix_node_urls']['old'] = $vals['old'];
  $old = preg_quote(trim($vals['old'], '/'));
  $_SESSION['mm_fix_node_urls']['new'] = $vals['new'];
  variable_set('mm_fix_node_urls_hostname_regex', $vals['hostname_regex']);
  $host = $vals['hostname_regex'];
  if (empty($host)) {
    $host = preg_quote($_SERVER['HTTP_HOST']);
  }
  // Look for: "URL" or 'URL' with optional hostname. For unquoted URL, hostname is required.
  $re = "{([\"'])(?:https?://{$host})?/{$old}(?:/.*?)\\1|https?://{$host}/{$old}(?:/[-.\\w]*)*}";

  variable_set('mm_fix_node_urls_chunksize', $vals['chunksize']);

  reset($vals['new']);
  $new = 'mm/' . key($vals['new']);
  $write = $vals['op'] == $vals['go'];

  $batch = array(
    'title' => $write ? t('Updating') : t('Searching'),
    'progress_message' => '@elapsed elapsed ... @estimate remaining',
    'operations' => array(
      array('mm_admin_fix_node_urls_batch', array($re, $vals['chunksize'], $old, $new, $write)),
    ),
    'finished' => 'mm_admin_fix_node_urls_finished',
    'file' => drupal_get_path('module', 'monster_menus') . '/mm_admin.inc',
  );
  batch_set($batch);
}

function mm_admin_fix_node_urls_batch($re, $chunksize, $old, $new, $write, &$context) {
  if (!function_exists('mm_admin_fix_node_urls_default_get')) {
    // Default getter which works with most simple fields created in the usual
    // Drupal manner
    function mm_admin_fix_node_urls_default_get($node, $field, $language) {
      return isset($node->{$field}[$language][0]['value']) ? $node->{$field}[$language][0]['value'] : NULL;
    }

    // Default setter which works with most simple fields created in the usual
    // Drupal manner
    function mm_admin_fix_node_urls_default_set($value, $node, $field, $language) {
      $node->{$field}[$language][0]['value'] = $value;
    }

    // Body-specific getter
    function _mm_admin_fix_node_urls_batch_body_get($node, $field, $language) {
      return isset($node->body[$language][0][$field]) ? $node->body[$language][0][$field] : NULL;
    }

    // Body-specific setter
    function _mm_admin_fix_node_urls_batch_body_set($value, $node, $field, $language) {
      $node->body[$language][0][$field] = $value;
    }
  }

  // The minimum length of the source text must be strlen($old + two quotes + initial slash)
  $sql_params = array(':length' => strlen($old) + 3);
  if (empty($context['sandbox'])) {
    $context['sandbox']['fields'] = array_merge(array(
      'value' => array(
        'table' => 'field_data_body',
        'join on' => '%alias.revision_id = node.vid',
        'table field' => 'body_value',
        'get' => '_mm_admin_fix_node_urls_batch_body_get',
        'set' => '_mm_admin_fix_node_urls_batch_body_set',
      ),
      'summary' => array(
        'table' => 'field_data_body',
        'join on' => '%alias.revision_id = node.vid',
        'table field' => 'body_summary',
        'get' => '_mm_admin_fix_node_urls_batch_body_get',
        'set' => '_mm_admin_fix_node_urls_batch_body_set',
      ),
    ), mm_module_invoke_all_array('mm_fix_node_urls_info', array()));
    $context['sandbox']['query'] = db_select('node')
      ->fields('node', array('nid', 'title'));
    $joined = array('node' => 'node');
    $or = db_or();
    foreach ($context['sandbox']['fields'] as $field_name => $field_def) {
      if (!empty($field_def['table'])) {
        $join_key = $field_def['table'] . ':' . $field_def['join on'];
        if (empty($joined[$join_key])) {
          $alias = $joined[$join_key] = 't' . count($joined);
          $context['sandbox']['query']->leftJoin($field_def['table'], $alias, $field_def['join on']);
        }
        else {
          $alias = $joined[$join_key];
        }
        $aliased_field = $alias . '.' . $field_def['table field'];
        $context['sandbox']['query']->addExpression($aliased_field, $field_name);
        $or->where("LENGTH($aliased_field) >= :length", $sql_params);
        if (empty($field_def['get'])) {
          $context['sandbox']['fields'][$field_name]['get'] = 'mm_admin_fix_node_urls_default_get';
        }
        if (empty($field_def['set'])) {
          $context['sandbox']['fields'][$field_name]['set'] = 'mm_admin_fix_node_urls_default_set';
        }
      }
    }
    $context['sandbox']['query']->condition($or);
    $context['sandbox']['chunkpage'] = 0;
    $context['sandbox']['max'] = $context['sandbox']['query']
      ->countQuery()
      ->execute()
      ->fetchField();
    $context['results']['title'] = array();
    $context['results']['out'] = array();
    $context['results']['matched_nodes'] = 0;
    $context['results']['total_matches'] = 0;
  }
  $thischunk = 0;
  // While a REGEXP could be used here, it turns out to be much faster to read
  // all nodes with a non-empty body and do the matching in PHP.
  $query = $context['sandbox']['query']
    ->range($chunksize * $context['sandbox']['chunkpage'], $chunksize)
    ->execute();
  foreach ($query as $result) {
    $thischunk++;

    $matches = array_combine(array_keys($context['sandbox']['fields']), array_fill(0, count($context['sandbox']['fields']), array()));
    $have_match = FALSE;
    foreach (array_keys($context['sandbox']['fields']) as $field) {
      if (!empty($result->$field)) {
        $have_match |= preg_match_all($re, $result->$field, $matches[$field], PREG_SET_ORDER);
      }
    }

    if ($have_match) {
      $context['results']['matched_nodes']++;
      foreach (array_keys($context['sandbox']['fields']) as $field) {
        foreach ($matches[$field] as $n => $match) {
          if ($context['results']['total_matches'] + $n < MM_ADMIN_NODE_URL_PREVIEW_COUNT) {
            $from = !empty($match[1]) ? trim($match[0], $match[1]) : $match[0];
            $is_abs = empty($match[1]) && preg_match('{^https?:}', $from);
            $key = "$from => " . url(preg_replace("{^.*?$old}", $new, $from), array('absolute' => $is_abs));
            $context['results']['out'][$result->nid][$key] = isset($context['results']['out'][$result->nid][$key]) ? $context['results']['out'][$result->nid][$key] + 1 : 1;
            if (!isset($context['results']['title'][$result->nid])) {
              $context['results']['title'][$result->nid] = empty($result->title) ? t('(untitled)') : strip_tags($result->title);
            }
          }
          else {
            break;
          }
        }
        $context['results']['total_matches'] += count($matches[$field]);
      }

      if ($write) {
        if ($node = node_load($result->nid)) {
          $lang = !empty($node->language) && isset($node->body[$node->language]) ? $node->language : LANGUAGE_NONE;
          $changed = FALSE;
          foreach ($context['sandbox']['fields'] as $name => $field_def) {
            if ($old_value = $field_def['get']($node, $name, $lang)) {
              $new_value = preg_replace_callback($re, function ($match) use ($new, $old) {
                $from = !empty($match[1]) ? trim($match[0], $match[1]) : $match[0];
                $is_abs = empty($match[1]) && preg_match('{^https?:}', $from);
                $url = url(preg_replace("{^.*?$old}", $new, $from), array('absolute' => $is_abs));
                if (!empty($match[1])) {
                  return $match[1] . $url . $match[1];
                }
                return $url;
              }, $old_value);

              if ($new_value != $old_value) {
                $field_def['set']($new_value, $node, $name, $lang);
                $changed = TRUE;
              }
            }
          }

          if ($changed) {
            $subst = array(
              '@type' => $node->type,
              '%title' => $node->title,
              '@old' => $old,
              '@new' => url($new),
            );
            $node->revision = TRUE;
            $node->log = t('admin/mm/fix-nodes: Updated URLs from /@old to @new.', $subst);
            $node->keep_changed_date = TRUE;
            watchdog('content', 'Updated URLs in %title from /@old to @new.', $subst, WATCHDOG_NOTICE, l(t('view'), 'node/' . $node->nid));
            node_save($node);
          }
        }
      }
    }
  }

  $node_count = $context['sandbox']['chunkpage'] * $chunksize + $thischunk;
  $context['finished'] = $thischunk != $chunksize ? 1.0 : $node_count / $context['sandbox']['max'];
  $context['message'] = format_plural($context['results']['total_matches'], 'Found 1 match in @nodes of @total nodes', 'Found @count matches in @nodes of @total nodes', array('@nodes' => $node_count, '@total' => $context['sandbox']['max']));
  $context['sandbox']['chunkpage']++;
}

function mm_admin_fix_node_urls_finished($success, $results, $operations) {
  if ($success) {
    $over = '';
    $counter = format_plural($results['matched_nodes'], '@matches in 1 node.', '@matches in @count nodes.', array('@matches' => format_plural($results['total_matches'], '1 match', '@count matches')));
    if ($results['total_matches'] > MM_ADMIN_NODE_URL_PREVIEW_COUNT) {
      $over = t('Only the first @count matches are shown.', array('@count' => MM_ADMIN_NODE_URL_PREVIEW_COUNT));
      $counter .= " $over";
    }
    $items = array();
    foreach ($results['out'] as $nid => $replaced) {
      $items[$nid] = array(
        'data' => l($results['title'][$nid], "node/$nid"),
        'children' => array()
      );
      foreach ($replaced as $url => $count) {
        $item = check_plain($url);
        if ($count > 1) {
          $item .= " (x$count)";
        }
        $items[$nid]['children'][] = $item;
      }
    }
    $_SESSION['mm_fix_node_urls']['result'] = array(
      array('#markup' => $counter),
      array(
        '#theme' => 'item_list',
        '#items' => $items,
        '#title' => '',
        '#type' => 'ul',
      ),
      array('#markup' => $over),
    );
  }
}

function mm_admin_regions($form, &$form_state) {
  $all_types = array();
  foreach (node_type_get_types() as $t) {
    $all_types[$t->type] = $t->name;
  }
  natcasesort($all_types);

  $form['regions'] = array('#type' => 'vertical_tabs');
  _mm_ui_get_regions($regions, $select, FALSE);
  $perms = mm_content_get_perms_for_region();
  foreach ($regions as $region => $data) {
    $form[$region] = array(
      '#type' => 'fieldset',
      '#title' => $data['long_name'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => $data['message'],
      '#group' => 'regions',
      '#tree' => TRUE,
    );
    $types = mm_content_get_allowed_types_for_region($region);
    $form[$region]['types'] = array(
      '#type' => 'fieldset',
      '#title' => t('Content types allowed in this region'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    if ($region == MM_UI_REGION_CONTENT) {
      $form[$region]['types']['#description'] = t('All types are always available in the <em>Content</em> region.');
    }
    else {
      $form[$region]['types']['allowed_all'] = array(
        '#type' => 'checkbox',
        '#title' => t('Allow all types'),
        '#default_value' => $types === 'all',
      );
      $form[$region]['types']['allowed_types'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#size' => 10,
        '#default_value' => is_array($types) ? $types : array(),
        '#options' => $all_types,
        '#states' => array(
          'invisible' => array('#' . drupal_clean_css_identifier("edit-$region-types-allowed-all") => array('checked' => TRUE)),
        ),
      );
    }
    $form[$region]['perms'] = array(
      '#type' => 'fieldset',
      '#title' => t('Who can add content to this region'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $users = array();
    if (isset($perms[$region]['users'])) {
      foreach ($perms[$region]['users'] as $mode => $data) {
        foreach ($data as $uid) {
          $users[$uid]['name'] = mm_content_uid2name($uid);
          $users[$uid]['modes'][] = $mode;
        }
      }
    }

    $groups = array();
    if (isset($perms[$region]['groups'])) {
      foreach ($perms[$region]['groups'] as $mode => $data) {
        foreach ($data as $gid) {
          $members = mm_content_get_users_in_group($gid, '<br />', FALSE, 20, TRUE);
          if ($members == '') {
            $members = t('(none)');
          }
          $groups[$gid]['name'] = mm_content_get_name($gid);
          $groups[$gid]['members'] = $members;
          $groups[$gid]['modes'][] = $mode;
        }
      }
    }

    $types = array(
      MM_PERMS_WRITE => array(
        t('Use region'),
        'If checked, !class can put content into this region.',
      ),
    );

    module_load_include('inc', 'monster_menus', 'mm_ui_content_edit');
    mm_ui_content_edit_permissions($form[$region]['perms'], $types, !empty($perms[$region]['everyone']) ? array(MM_PERMS_WRITE) : array(), $groups, $users);
  }

  $form['_actions'] = array('#type' => 'actions');
  $form['_actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  mm_static('settings_perms', TRUE, 'mm-admin-regions');
  return $form;
}

function mm_admin_regions_submit($form, &$form_state) {
  $allowed_types = array();
  $vals =& $form_state['values'];
  $perms = array();
  foreach (array_keys(mm_content_get_perms_for_region()) as $region) {
    list($perms[$region]['groups'], $perms[$region]['users']) = _mm_ui_form_parse_perms($vals[$region]['perms'], TRUE);
    $perms[$region]['everyone'] = !empty($vals[$region]['perms']['table']['everyone'][0]['group-w-everyone']);
    $allowed_types[$region] = $region == MM_UI_REGION_CONTENT || !empty($vals[$region]['types']['allowed_all']) ? 'all' : array_values($vals[$region]['types']['allowed_types']);
  }
  variable_set('mm_allowed_region_node_types', $allowed_types);
  variable_set('mm_allowed_region_perms', $perms);
}
