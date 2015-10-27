<?php

/**
 * @file
 *   Implements content forms integration on behalf of core modules.
 */

/**
 * Implements hook_contextual_forms_info().
 */
function contextual_forms_contextual_forms_info() {
  // Aggregator module
  if (module_exists('aggregator')) {
    $forms['aggregator_form_feed'] = array(
      'title' => t('Add feed'),
      'description' => t('Form constructor for adding and editing feed sources.'),
      'module' => 'aggregator',
      'include' => array('name' => 'aggregator.admin'),
    );

    $forms['aggregator_form_category'] = array(
      'title' => t('Add category'),
      'description' => t('Form constructor to add aggregator categories.'),
      'module' => 'aggregator',
      'include' => array('name' => 'aggregator.admin'),
    );

    $forms['aggregator_form_opml'] = array(
      'title' => t('Import OPML'),
      'description' => t('Form constructor for importing feeds from OPML.'),
      'module' => 'aggregator',
      'include' => array('name' => 'aggregator.admin'),
    );
  }


  // Block module
  if (module_exists('block')) {
    $forms['block_admin_configure'] = array(
      'title' => t('Configure block'),
      'description' => t('Form constructor for the block configuration form.'),
      'module' => 'block',
      'include' => array('name' => 'block.admin'),
      'contexts' => array(
        new ctools_context_required(t('Module'), 'string'),
        new ctools_context_required(t('Delta'), 'string'),
      ),
    );

    $forms['block_add_block_form'] = array(
      'title' => t('Add block'),
      'description' => t('Form constructor for the add block form.'),
      'module' => 'block',
      'include' => array('name' => 'block.admin'),
    );
  }


  // Book module
  if (module_exists('book')) {
    $forms['book_admin_edit'] = array(
      'title' => t('Re-order book pages and change titles'),
      'description' => t('Build the form to administrate the hierarchy of a single book.'),
      'module' => 'book',
      'include' => array('name' => 'book.admin'),
      'contexts' => array(
        new ctools_context_required(t('Node'), 'node'),
      ),
    );

    $forms['book_outline_form'] = array(
      'title' => t('Outline'),
      'description' => t('Show the outline form for a single node.'),
      'module' => 'book',
      'include' => array('name' => 'book.admin'),
      'contexts' => array(
        new ctools_context_required(t('Node'), 'node'),
      ),
    );
  }


  // Comment module
  if (module_exists('comment')) {
    $forms['comment_admin_overview'] = array(
      'title' => t('Comment Admin Overview'),
      'description' => t('Form builder for the comment overview administration form.'),
      'module' => 'comment',
      'include' => array('name' => 'comment.admin'),
      'contexts' => array(
        new ctools_context_required(t('Type'), 'string'),
      ),
    );
  }


  // Contact module
  if (module_exists('contact')) {
    $forms['contact_site_form'] = array(
      'title' => t('Site Contact'),
      'description' => t('Form constructor for the site-wide contact form.'),
      'module' => 'contact',
      'include' => array('name' => 'contact.pages'),
    );

    $forms['contact_personal_form'] = array(
      'title' => t('Personal Contact'),
      'description' => t('Form constructor for the personal contact form.'),
      'include' => array('name' => 'contact.pages'),
      'contexts' => array(
        new ctools_context_required(t('Recipient'), 'user'),
      ),
    );
  }


  // Filter module
  if (module_exists('filter')) {
    $forms['filter_admin_overview'] = array(
      'title' => t('Text formats'),
      'description' => t('Displays a list of all text formats and allows them to be rearranged.'),
      'module' => 'filter',
      'include' => array('name' => 'filter.admin'),
    );
  }


  // Forum module
  if (module_exists('forum')) {
    $forms['forum_overview'] = array(
      'title' => t('Forums'),
      'description' => t('Returns an overview list of existing forums and containers.'),
      'module' => 'forum',
      'include' => array('name' => 'forum.admin'),
    );
  }


  // Image module
  if (module_exists('image')) {
    $forms['image_style_add_form'] = array(
      'title' => t('Add style'),
      'description' => t('Form for adding a new image style.'),
      'module' => 'image',
      'include' => array('name' => 'image.admin'),
    );
  }

  // Locale module
  if (module_exists('locale')) {
    // @todo
    // $forms['locale_date_format_form'] = array();
    // $forms['locale_date_format_reset_form'] = array();
    // $forms['locale_languages_custom_form'] = array();
    // $forms['locale_languages_edit_form'] = array();
    // $forms['locale_languages_overview_form'] = array();
    // $forms['locale_languages_predefined_form'] = array();
    // $forms['locale_language_providers_session_form'] = array();
    // $forms['locale_language_providers_url_form'] = array();
    // $forms['locale_translate_edit_form'] = array();
    // $forms['locale_translate_import_form'] = array();
    // $forms['locale_translation_filter_form'] = array();
  }


  // Node module
  $forms['node_admin_content'] = array(
    'title' => t('Node Administration'),
    'description' => t('Find and manage content.'),
    'module' => 'node',
    'include' => array('name' => 'node.admin'),
  );

  $forms['node_type_form'] = array(
    'title' => t('Add content type'),
    'description' => t('Form constructor for the node type add form.'),
    'module' => 'node',
    'include' => array('name' => 'content_types'),
  );

  foreach (node_type_get_types() as $type) {
    $forms[$type->type . '_node_form'] = array(
      'title' => $type->name . ' Node Form',
      'description' => $type->description,
      'module' => 'node',
      'include' => array('name' => 'node.pages'),
      'contexts' => array(
        new ctools_context_required(t('Node'), 'node', array('type' => $type->type)),
      ),
    );
  }


  // Poll module
  if (module_exists('poll')) {
    $forms['poll_view_voting'] = array(
      'title' => t('Poll View Voting'),
      'description' => t('Generates the voting form for a poll.'),
      'module' => 'poll',
      'contexts' => array(
        new ctools_context_required(t('Node'), 'node', array('type' => 'poll')),
      ),
    );
  }


  // Shortcut module
  if (module_exists('shortcut')) {
    $forms['shortcut_set_add_form'] = array(
      'title' => t('Add shortcut set'),
      'description' => t('Builds the form for adding a shortcut set.'),
      'module' => 'shortcut',
      'include' => array('name' => 'shortcut.admin'),
    );
  }


  // System module
  $forms['system_ip_blocking_form'] = array(
    'title' => t('System IP Blocking'),
    'description' => t('Define the form for blocking IP addresses.'),
    'module' => 'system',
    'include' => array('name' => 'system.admin'),
    'contexts' => array(
      new ctools_context_required(t('Default IP'), 'string'),
    ),

  );

  $forms['system_theme_settings'] = array(
    'title' => t('System Theme Settings'),
    'description' => t('Display theme configuration for entire site and individual themes.'),
    'module' => 'system',
    'include' => array('name' => 'system.admin'),
    'contexts' => array(
      new ctools_context_required(t('Key'), 'string'),
    ),
  );

  $forms['system_modules'] = array(
    'title' => t('System Modules'),
    'description' => t('Provides module enable/disable interface.'),
    'module' => 'system',
    'include' => array('name' => 'system.admin'),
  );

  $forms['system_site_maintenance_mode'] = array(
    'title' => t('System Site Maintenance Mode'),
    'description' => t('Configure the site\'s maintenance status.'),
    'module' => 'system',
    'include' => array('name' => 'system.admin'),
  );


  // Taxonomy module
  if (module_exists('taxonomy')) {
    $forms['taxonomy_overview_vocabularies'] = array(
      'title' => t('Taxonomy Overview Vocabularies'),
      'description' => t('Form builder to list and manage vocabularies.'),
      'module' => 'taxonomy',
      'include' => array('name' => 'taxonomy.admin'),
    );

    $forms['taxonomy_form_vocabulary'] = array(
      'title' => t('Taxonomy Add/Edit Vocabularies'),
      'description' => t('Form builder to list and manage vocabularies.'),
      'module' => 'taxonomy',
      'include' => array('name' => 'taxonomy.admin'),
      'contexts' => array(
        new ctools_context_optional(t('Vocabulary'), 'vocabulary'),
      ),
    );

    $forms['taxonomy_form_term'] = array(
      'title' => t('Taxonomy Add/Edit Term'),
      'description' => t('Form function for the term edit form.'),
      'module' => 'taxonomy',
      'include' => array('name' => 'taxonomy.admin'),
      'contexts' => array(
        new ctools_context_optional(t('Taxonomy Term'), 'term'),
        new ctools_context_optional(t('Vocabulary'), 'vocabulary'),
      ),
    );

    $forms['taxonomy_overview_terms'] = array(
      'title' => t('Taxonomy Term Overview'),
      'description' => t('Form builder for the taxonomy terms overview.'),
      'module' => 'taxonomy',
      'include' => array('name' => 'taxonomy.admin'),
      'contexts' => array(
        new ctools_context_optional(t('Vocabulary'), 'vocabulary'),
      ),
    );
  }


  // User module
  $forms['user_filter_form'] = array(
    'title' => t('User Filter Form'),
    'description' => t('Return form for user administration filters.'),
    'module' => 'user',
    'include' => array('name' => 'user.admin'),
  );

  $forms['user_admin_account'] = array(
    'title' => t('User administration page.'),
    'description' => t('User administration page.'),
    'module' => 'user',
    'include' => array('name' => 'user.admin'),
  );

  $forms['user_pass'] = array(
    'title' => t('User Password Reset.'),
    'description' => t('Request a password reset.'),
    'module' => 'user',
    'include' => array('name' => 'user.pages'),
  );

  $forms['user_admin_permissions'] = array(
    'title' => t('User Permissions page.'),
    'description' => t('Administer permissions.'),
    'module' => 'user',
    'include' => array('name' => 'user.admin'),
  );

  $forms['user_admin_roles'] = array(
    'title' => t('User Roles.'),
    'description' => t('Form to re-order roles or add a new one.'),
    'module' => 'user',
    'include' => array('name' => 'user.admin'),
  );

  $forms['user_login'] = array(
    'title' => t('User Login'),
    'description' => t('The main user login form.'),
    'module' => 'user',
    'include' => array('name' => 'user.pages'),
  );

  $forms['user_register_form'] = array(
    'title' => t('User Register Form'),
    'description' => t('The user registration form.'),
    'module' => 'user',
  );

  $forms['user_profile_form'] = array(
    'title' => t('User Profile Form'),
    'description' => t('Edit a user account or one of their profile categories.'),
    'module' => 'user',
    'include' => array('name' => 'user.pages'),
    'contexts' => array(
      new ctools_context_required(t('User'), 'user'),
      new ctools_context_optional(t('Category'), 'string'),
    ),
  );

  return $forms;
}
