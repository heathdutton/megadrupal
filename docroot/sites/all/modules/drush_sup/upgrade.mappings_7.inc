<?php

function drush_upgrade_mappings() {
  //
  // Project alternatives lists Drupal 7 contrib projects that
  // may be used in place of Drupal 6 contrib modules that may
  // or may not have versions in Drupal 7.  Each entry
  // in the alternatives list contains a 'module-remap',
  // which is an associative array that remaps the Drupal 6
  // module name to the corresponding module in the Drupal 7
  // project.  Note that the Drupal 6 project name does not
  // need to be specified; Drush site-upgrade can determine
  // that on its own.
  //
  // Example:
  //
  // The project http://drupal.org/project/redirect
  // provides a new module called "redirect" that replaces
  // the D6 contrib modules "globalredirect" and "path_redirect".
  // Its entry in project alternatives therefore looks like this:
  //
  //     'redirect' => array(
  //       'module-remap' => array(
  //         'globalredirect' => 'redirect',
  //         'path_redirect' => 'redirect',
  //       ),
  //     ),
  //
  // A 'description' entry may also be provided.
  //
  // Note that it is possible to have multiple project alternatives for
  // any given D6 contrib module.  The user will have to select the
  // desired upgrade path if this is the case.
  //
  $project_alternatives = array(
    'redirect' => array(
      'description' => 'The redirect project provides replacement functionality for the globalredirect and path_redirect projects.',
      'module-remap' => array(
        'globalredirect' => 'redirect',
        'path_redirect' => 'redirect',
      ),
    ),
    'bundle_copy' => array(
      'description' => 'The cck copy_content module was moved out into the bundle_copy module in D7.',
      'module-remap' => array(
        'content_copy' => 'bundle_copy',
      ),
    ),
    'field_permissions' => array(
      'description' => 'The cck content_permissions module moved to the field_permissions module in D7.',
      'module-remap' => array(
        'content_permissions' => 'field_permissions',
      ),
    ),
    'field_group' => array(
      'description' => 'The cck field_group module moved to the field_group module in D7.',
      'module-remap' => array(
        'fieldgroup' => 'field_group',
      ),
    ),
    'ctools' => array(
      'description' => 'The functionality of the views module views_export can now be handled by the ctools bulk export module.',
      'module-remap' => array(
        'views_export' => 'bulk_export',
      ),
    ),
    'references' => array(
      'description' => 'The cck node_reference and user_reference modules may be replaced by the references project in D7.',
      'module-remap' => array(
        'nodereference' => 'node_reference',
        'userreference' => 'user_reference',
      ),
    ),
    'entityreference' => array(
      'description' => 'The cck node_reference and user_reference modules may be replaced by the entityreference module in D7.',
      'module-remap' => array(
        'nodereference' => 'entityreference',
        'userreference' => 'entityreference',
      ),
    ),
    'calendar_ical' => array(
      'description' => 'calendar_ical has been replaced by date_ical in D7.',
      'module-remap' => array(
        'calendar_ical' => 'date_ical'
      ),
    ),
    'subpath_alias' => array(
      'description' => 'subpath_alias has been replaced by subpathauto in D7.',
      'module-remap' => array(
        'subpath_alias' => 'subpathauto'
      ),
    ),
  );
  //
  // Projects in core lists Drupal 6 contrib projects
  // that are now included in Drupal 7 core.
  //
  $projects_in_core = array(
    'abssrc',
    'admin_hover',
    'adminrole',
    'ahah_helper',
    'autoload',
    'automaticmenu',
    'automenu',
    'auto_menutitle',
    'block_edit',
    'block_node_visibility',
    'blocks404',
    'canonical_url',
    'checkbox_validate',
    'comment_display',
    'config_perms',
    'ctm',
    'dbtng',
    'documentation',
    'drupal_queue',
    'edit_term',
    'elements',
    'element_themehook',
    'filefield',
    'filter_default',
    'hook_file',
    'imageapi',
    'imagecache',
    'imagefield',
    'input_format_permissions',
    'jq',
    'jqp',
    'jquery_cookie',
    'jquery_form_update',
    'jquery_ui',
    'jsalter',
    'login_security',
    'menuless_nodetype',
    'nodepreview_by_type',
    'parallel',
    'permissions_api',
    'phpass',
    'plugin_manager',
    'plugins',
    'poormanscron',
    'preview',
    'rdf',
    'shortlink',
    'simplecdn',
    'simpletest',
    'storage_api',
    'tar',
    'taxidermy',
    'tinymce_ahah',
    'tinymce_dragdrop',
    'transaction',
    'upload_element',
    'upload_preview',
    'url_alter',
    'user_cancellation',
    'user_default_filter',
    'user_delete',
    'vertical_tabs',
    'view_unpublished',
    'vocabperms',
    'wysiwygcck',
  );
  $modules_in_core = array(
    // 'date' project is still contrib, but date_timezone, one of its modules, has moved to core. See http://drupal.org/node/868028.
    'date_timezone',
    // 'optionwidgets' has been replaced by the core module 'options'
    'optionwidgets',
  );
  $result = array(
    'source' => 'http://drupal.org/node/895314',
    'project-alternatives' => $project_alternatives,
    'projects-in-core' => $projects_in_core,
    'modules-in-core' => $modules_in_core,
    // If a project changes name, then an entry
    // 'oldprojectname' => 'newprojectname' should be placed in this map.
    // 'project-remap' should only be used if the extensions in the project
    // all exist with the same name in the new project name.  If modules change
    // name, hten 'module-remap' and 'module-project-relocation' should also be
    // used.
    // If an extension changes name, but stays in the same
    // project, then put an entry here from the original
    // module name to the new module name.
    'module-remap' => array(
      'content' => 'cck',
    ),
    // Drush site-upgrade will manage the module project relocation map
    // automatically, based on information stored in the project alternatives.
    'module-project-relocation' => array(),
    // Drush site-upgrade will manage the project remap automatically,
    // based on information stored in the project alternatives.
    'project-remap' => array(),
    'warning' => array(
      'token' => dt('!special  In Drupal 7, the contrib token module handles UI, as well as field and profile tokens; all other functionality has been migrated to core. You may encounter problems when enabling the token module during a major upgrade; see http://drupal.org/node/1477932.'),
      'cck' => TRUE,
      'content_taxonomy' => dt('!special  The project page for this module (!project_page) says that an upgrade path will be provided for this module at a future time.'),
      'date:date_api' => dt("!special  The d6 version of the date_api module in the date project defines a table called date_formats, which is also defined by system/system.install schema in d7.  If this problem has not been fixed yet, then the updatedb function will fail, and it will not be possible to upgrade to d7.  If this happens, disable and uninstall the date_api module before running site-upgrade (i.e. add '--uninstall=date_api' to site-upgrade call).  See http://drupal.org/node/1013034."),
      'features' => dt('IMPORTANT: If you are using features, you should import your features into the database using the features-import-all command.  See http://drupal.org/node/1014522#comment-5478110 for details.'),
      'field_taxonomy' => FALSE,
      'filefield' => dt('!special  Data migration for this module will be provided by the Content Migrate submodule of cck.  Enable content_migrate after upgrading; see http://drupal.org/node/781088.'),
      'imagefield' => dt('!special  Data migration for this module will be provided by the Content Migrate submodule of cck.  Enable content_migrate after upgrading; see http://drupal.org/node/781088.'),
      'taxonomy_delegate' => FALSE,
      'taxonomy_intro' => FALSE,
      'term_fields' => FALSE,
      'uuid' => dt('IMPORTANT: Bug in uuid module:  Apply the patch at http://drupal.org/node/1469942, or upgrade to the dev version prior to enabling the uuid module to fix.  If you see error messages after updating uuid, you must restore to the latest working backup before proceeding.'),
    ),
  );

  return $result;
}
