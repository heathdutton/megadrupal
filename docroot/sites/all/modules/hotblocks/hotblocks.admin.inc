<?php

/**
 * Form callback for main settings form
 */
function hotblocks_settings($form, $form_state, $delta = NULL) {
  $form['hotblocks'] = array(
    '#type' => 'fieldset',
    '#title' => 'Hotblocks',
  );

  // Build a table of hotblock names and admin links
  $rows = array();
  foreach (hotblocks_get_blocks() as $machine_name => $label) {
    $link_options = array('query' => drupal_get_destination());
    $rows[] = array(
      $label, // Block title
      $machine_name, // Machine name
      l('Edit', "admin/structure/hotblocks/list/$machine_name/edit", $link_options), // Edit link
      l('Delete', "admin/structure/hotblocks/list/$machine_name/delete", $link_options), // Delete link
      l('Configure block', "admin/structure/block/manage/hotblocks/$machine_name/configure", $link_options), // Config block link
    );
  }
  $form['hotblocks']['hotblocks_links'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => array('Label', 'Machine Name', 'Edit', 'Delete', 'Configure'),
  );

  // New hotblock form
  $form['hotblocks']['new'] = array('#type' => 'fieldset', '#title' => 'Create a new hotblock');
  $form['hotblocks']['new']['hotblocks_new_block_name'] = array(
    '#type'        => 'textfield',
    '#title'       => 'Block name',
    '#description' => 'The name of this block as seen in the block by the hotblock admin and in the administer blocks page.',
  );
  $form['hotblocks']['new']['hotblocks_new_block_machine_name'] = array(
    // The machine name Field which will take the value from Source field and convert it to machine friendly name.
    '#type'         => 'machine_name',
    '#title'        => t("Machine Name"),
    '#description'  => t("machine-friendly name."),
    '#required'     => FALSE,
    '#size'         => 128,
    '#maxlength'    => 128,
    '#machine_name' => array(
      // function that return 1 if the machine name is duplicated.
      'exists' => 'hotblocks_check_machine_name_if_exist',
      // the name of the source field that we will Take the User Friendly name from and convert it to Machine Friedly name
      'source' => array('hotblocks', 'new', 'hotblocks_new_block_name'),
      //'standalone' => TRUE,
      //'label' => '',
    ),
  );


  // CONTENT FILTERING////////////
  $form['hotblocks_filtering'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<label>' . t('Filtering content available for hotblocks') . '</label>',
  );

  // Build an option list from the content types available in the system
  $aContentTypes = node_type_get_names();
  $aOptions = array('hotblocks_any' => t('Any/all types'));
  foreach ($aContentTypes as $aType => $sTypeName) {
    $aOptions[$aType] = $sTypeName;
  }

  // Options for content types
  $form['hotblocks_filtering']['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Types'),
  );
  $form['hotblocks_filtering']['content']['hotblocks_allowed_types'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#size' => 15,
    '#title' => 'Allowed Content Types',
    '#description' => 'The content types that are allowed to be put inside a hotblock.',
    '#options' => $aOptions,
    '#default_value' => hotblocks_get_setting('hotblocks_allowed_types', $delta),
  );
  $form['hotblocks_filtering']['content']['hotblocks_show_all_nodes_of_hotblocks_item_type'] = array(
    '#type' => 'checkbox',
    '#title' => 'Make all nodes of the allowed content types available',
    '#description' => 'Uncheck this if you would like to specify which nodes are availble to hotblocks on a node-by-node
      basis.  Allowed content types filtering will still apply.',
    '#default_value' => hotblocks_get_setting('hotblocks_show_all_nodes_of_hotblocks_item_type', $delta),
  );

  // Options for entities:
  $form['hotblocks_filtering']['entities'] = array(
    '#type' => 'fieldset',
    '#title' => 'Entities',
  );
  $sorted_entity_info = entity_get_info();
  ksort($sorted_entity_info);
  foreach ($sorted_entity_info as $entity_type => $entity_info) {
    $options = array();

    // Grab each bundle type from the entity type and put it in the options array
    foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
      $options[$bundle] = $bundle_info['label'];
    }

    $form['hotblocks_filtering']['entities']['hotblocks_allowed_bundles_' .$entity_type] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#size' => 15,
      '#title' => "Allowed {$entity_info['label']} ($entity_type) Bundles",
      '#description' => 'The entity\'s bundles that are allowed to be put inside a hotblock.',
      '#options' => $options,
      '#default_value' => hotblocks_get_setting('hotblocks_allowed_bundles_' .$entity_type, $delta),
    );
  }

  // Options for blocks:
  $form['hotblocks_filtering']['blocks'] = array(
    '#type' => 'fieldset',
    '#title' => 'Blocks',
  );
  $form['hotblocks_filtering']['blocks']['hotblocks_allow_blocks'] = array(
    '#type' => 'checkbox',
    '#title' => 'Allow other Drupal blocks to be inserted into hotblocks.',
    '#default_value' => hotblocks_get_setting('hotblocks_allow_blocks', $delta),
  );

  $aBlockOptions = module_implements('block_info');
  sort($aBlockOptions);
  $aBlockOptions = array_combine($aBlockOptions, $aBlockOptions);
  $form['hotblocks_filtering']['blocks']['hotblocks_blocks_excluded_modules'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#size' => 15,
    '#title' => 'Exclude blocks from these modules',
    '#description' => 'If you don\'t want the blocks provided by one or more modules to be available to hotblocks,
      select them here.  Only applies if blocks are allowed.  Use shift+click or ctrl+click to help make your selections.',
    '#options' => $aBlockOptions,
    '#default_value' => hotblocks_get_setting('hotblocks_blocks_excluded_modules', $delta),
  );

  // Option for whitelisting certain blocks
  $available_blocks = hotblocks_get_assignable_blocks(FALSE);
  $whitelist_options = array();
  foreach ($available_blocks as $block) {
    $unique_id = $block->block['module'] . '_' . $block->block['delta'];
    $whitelist_options[$unique_id] = "({$block->block['module']}) - {$block->title}";
  }
  asort($whitelist_options);

  $form['hotblocks_filtering']['blocks']['hotblocks_block_only_whitelist'] = array(
    '#type' => 'checkbox',
    '#title' => 'Only allow whitelisted blocks to be added to hotblocks',
    '#default_value' => hotblocks_get_setting('hotblocks_block_only_whitelist', $delta),
  );
  $form['hotblocks_filtering']['blocks']['hotblocks_blocks_whitelist'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#size' => 50,
    '#title' => 'Whitelisted blocks',
    '#description' => 'This list is additionally filtered by modules excluded.',
    '#options' => $whitelist_options,
    '#default_value' => hotblocks_get_setting('hotblocks_blocks_whitelist', $delta),
  );


  // INTERFACE OPTIONS///////////
  $form['hotblocks_interface'] = array(
    '#type' => 'fieldset',
    '#title' => t('Interface'),
  );

  $form['hotblocks_interface']['hotblocks_use_icons'] = array(
    '#type' => 'checkbox',
    '#title' => 'Interface Icons',
    '#description' => 'Check this box if you want hotblocks to use icons instead of text links for its administrative controls.',
    '#default_value' => variable_get('hotblocks_use_icons', TRUE),
  );

  $files = file_scan_directory(drupal_get_path('module', 'hotblocks') . '/images', '/.*/', array('callback' => FALSE, 'recurse' => FALSE));
  foreach ($files as $oVal) {
    $options[$oVal->filename] = $oVal->filename;
  }
  $form['hotblocks_interface']['hotblocks_icons_set'] = array(
    '#type' => 'select',
    '#title' => 'Icons size',
    '#description' => 'If you\'re using icons for the admin UI, choose the set you\'d like to use',
    '#default_value' => variable_get('hotblocks_icons_set', 'flat'),
    '#options' => $options,
  );

  $form['hotblocks_interface']['hotblocks_floating_controls'] = array(
    '#type' => 'checkbox',
    '#title' => 'Floating Controls',
    '#description' => 'Check this box if you want hotblocks administrative controls to float over it\'s items when you
      mouse over them.  If unchecked, hotblocks controls will be visible all the time.',
    '#default_value' => variable_get('hotblocks_floating_controls', TRUE),
  );

  // TODO: some kind of radio button option group instead of checkboxes
  $form['hotblocks_interface']['hotblocks_contextual_links'] = array(
    '#type' => 'checkbox',
    '#title' => 'Controls as Contextual Links only',
    '#description' => 'Check this box if you want hotblocks administrative controls to only appear in the block\'s
      contextual links.  <strong>The contextual links module must be enabled, and permissions must be configured for
      this to work properly</strong>.',
    '#default_value' => variable_get('hotblocks_contextual_links', FALSE),
  );

  // Option to organize by taxonomy
  if (module_exists('taxonomy')) {
    $vocabularies = taxonomy_get_vocabularies();
    $voptions[0] = '--None--';
    foreach ((array) $vocabularies as $oVocab) {
      $voptions[$oVocab->vid] = $oVocab->name;
    }

    $form['hotblocks_interface']['taxonomy'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy'),
    );

    $form['hotblocks_interface']['taxonomy']['hotblocks_vocabulary'] = array(
      '#type' => 'select',
      '#title' => 'Organize by Taxonomy',
      '#description' => 'Provides an additional option to categorize assignable content by taxonomy when adding content
        to a hotblock. Tag your content with taxonomy terms from this vocabulary to use this feature.',
      '#options' => $voptions,
      '#default_value' => variable_get('hotblocks_vocabulary', 0),
    );
  }

  // MISC OPTIONS

  $form['hotblocks_misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneous Options'),
  );

  $form['hotblocks_misc']['hotblocks_reorder_titles'] = array(
    '#type' => 'checkbox',
    '#title' => 'Show titles only when reordering',
    '#description' => 'If this setting is checked, only the item titles will be displayed when reordering items inside a
      hotblock.  This is helpful when you have a large area of content and it becomes cumbersome to reorder such large
      pieces.  If you would like the same full, visual display of the hotblock items while reordering, turn this off.',
    '#default_value' => variable_get('hotblocks_reorder_titles', TRUE),
  );

  $form['hotblocks_misc']['hotblocks_extended_permissions'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable extended permissions',
    '#description' => 'If this setting is checked, the hotblocks module will provide granular permissions for each
      hotblock and all allowed hotblocks item types. E.g., "assign hotblocks of typex to blockx". Having a permission of
      "assign hotblocks" will negate the need for the more specific permission, and "administer hotblocks" eliminates
      the need for any other permissions.',
    '#default_value' => variable_get('hotblocks_extended_permissions', FALSE),
  );

  $form['hotblocks_misc']['hotblocks_terminology_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hotblocks Terminology'),
  );

  $form['hotblocks_misc']['hotblocks_terminology_fieldset']['hotblocks_terminology'] = array(
    '#type' => 'textfield',
    '#title' => t('Term for hotblocks item'),
    '#description' => t('Depending on what you are using this module for, you may want to use different language to
      describe hotblocks content to the user.  This is an easy way to accomplish this.  This option allows you to change
      the term that is displayed to the user to something else, like "promos", "blurb", "snippet", or whatever you might
      think of.  The new terminology will not be reflected on this settings page.'),
    '#default_value' => variable_get('hotblocks_terminology', HOTBLOCKS_DEFAULT_TERMINOLOGY),
  );
  $form['hotblocks_misc']['hotblocks_terminology_fieldset']['hotblocks_terminology_plural'] = array(
    '#type' => 'textfield',
    '#title' => t('Plural term'),
    '#description' => t('This should be the plural version of the term above, as in the case of "reorder hotblock items", etc.'),
    '#default_value' => variable_get('hotblocks_terminology_plural', HOTBLOCKS_DEFAULT_TERMINOLOGY_PLURAL),
  );

  $form['hotblocks_misc']['hotblocks_title_hiding'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node Title Hiding'),
  );

  $form['hotblocks_misc']['hotblocks_title_hiding']['hotblocks_title_hiding_helptext'] = array(
    '#type' => 'markup',
    '#value' => '<P>' . t('If you want to hide some or all of the titles of nodes within your hotblocks from being displayed to users, check one of the 2 checkboxes below.') . '</P>',
  );

  $form['hotblocks_misc']['hotblocks_title_hiding']['hotblocks_title_hiding_hide_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide all hotblocks node titles'),
    '#default_value' => hotblocks_get_setting('hotblocks_title_hiding_hide_all', $delta),
    '#description' => t('If this box is checked, all node titles of hotblocks will be hidden when the item is displayed in the hotblock.'),
  );

  $form['hotblocks_misc']['hotblocks_title_hiding']['hotblocks_title_hiding_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide hotblocks_item titles within delimiters'),
    '#description' => t('Check this box if you want hotblocks node titles inside the delimiters to be hidden when the
      item displays: e.g. if your delimiters are parentheses, then (My Title) will be hidden.  This can be useful to see
      adminstrator notes in the hotblocks picking screen that are hidden from the user.  For example "(the ad we just
      made last night) Super Product X".  You could also use this feature to hide some titles but not others.  E.g.
      "This title can be seen", "(This cannot because the node just contains an image)".'),
    '#default_value' => hotblocks_get_setting('hotblocks_title_hiding_enable', $delta),
  );

  $form['hotblocks_misc']['hotblocks_title_hiding']['hotblocks_title_hiding_delim_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting delimiter'),
    '#size' => 4,
    '#default_value' => variable_get('hotblocks_title_hiding_delim_start', HOTBLOCKS_TITLE_HIDING_START),
  );

  $form['hotblocks_misc']['hotblocks_title_hiding']['hotblocks_title_hiding_delim_end'] = array(
    '#type' => 'textfield',
    '#title' => t('Ending delimiter'),
    '#size' => 4,
    '#default_value' => variable_get('hotblocks_title_hiding_delim_end', HOTBLOCKS_TITLE_HIDING_END),
  );

  $form['hotblocks_misc']['hotblocks_respect_node_access'] = array(
    '#title' => 'Respect node access',
    '#type' => 'checkbox',
    '#description' => 'If this is unchecked, hotblocks permissions will allow users to add or remove nodes from
      hotblocks for which they wouldn\'t normally have view access to.  A user with "administer hotblocks" will still
      be able to view all hotblock content regardless of this setting, so delegate that permission with care.',
    '#default_value' => variable_get('hotblocks_respect_node_access', TRUE),
  );

  // FORM BUTTONS//////////
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );
  $form['#theme'] = 'system_settings_form';

  return $form;
}

/**
 * Submit callback for hotblocks_settings form
 */
function hotblocks_settings_submit($form, &$form_state) {
  // A new hotblock is being created, write it to the DB
  if(!empty($form_state['values']['hotblocks_new_block_name']) && !empty($form_state['values']['hotblocks_new_block_machine_name'])) {
    $record = array(
      'machine_name' => $form_state['values']['hotblocks_new_block_machine_name'],
      'label' => $form_state['values']['hotblocks_new_block_name'],
      'data' => '',
    );
    drupal_write_record('hotblocks_settings', $record);
    drupal_set_message('New hotblock created.');
  }

  // The menu titles will change when the terminology option is changed, so menu needs to be rebuilt.
  menu_rebuild();

  system_settings_form_submit($form, $form_state);
}

/**
 * settings page form callback for the configuration of hotblock nodes
 * view modes
 */
function hotblocks_settings_view_modes($form, $form_state) {
  $aForm = array();

  // Build an options list of node view modes usable by FAPI.
  $options = array();
  $entity_info = entity_get_info('node');
  $view_modes = $entity_info['view modes'];
  foreach ($view_modes as $view_mode_name => $view_mode_info) {
    $options[$view_mode_name] = $view_mode_info['label'];
  }

  // The 'default for all' setting
  $aForm['node-type-default'] = array(
    '#type' => 'fieldset',
    '#title' => "Default for all content types",
  );
  $aForm['node-type-default']['hotblocks_view_mode_default'] = array(
    '#type' => 'select',
    '#title' => t('View Mode'),
    '#options' => $options,
    '#default_value' => variable_get('hotblocks_view_mode_default', 'full'),
  );

  // Show view mode options for each content type
  $allContentTypes = node_type_get_names();
  $aOptions = array();
  foreach (hotblocks_get_allowed_node_types() as $sTypeName) {
    $aForm[$sTypeName.'-modes'] = array(
      '#type' => "fieldset",
      '#title' => "Settings for ".$allContentTypes[$sTypeName]
    );
    $aForm[$sTypeName.'-modes']['hotblocks_view_mode_'.$sTypeName] = array(
      '#type' => 'select',
      '#title' => t('Use custom display settings for the following view modes'),
      '#options' => $options,
      '#default_value' => variable_get('hotblocks_view_mode_'.$sTypeName, 'full'),
    );
    $aOptions[$sTypeName] = $sTypeName;
  }

  // Submit button
  $aForm['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 50,
  );

  return $aForm;
}

/**
 * Form callback for the submission of the view modes settings form
 * Pass to standard system settings processing
 * @param $form - the form
 * @param $form_state - reference to form state
 */
function hotblocks_settings_view_modes_submit($form, &$form_state) {
  system_settings_form_submit($form, $form_state);
}



/**
 * Page/form callback for admin/structure/hotblocks/%/delete
 * Confirmation form to delete a hotblock
 */
function hotblocks_delete_form($form, &$form_state, $delta) {
  $form['delta'] = array (
    '#type' => 'value',
    '#value' => $delta,
  );

  return confirm_form($form, 'Are you sure you want to delete the hotblock "<strong>'.hotblocks_get_block_label($delta).'</strong>"?', drupal_get_destination());
}

/**
 * Form submit callback for hotblocks_delete_form
 * Deletes all entries from all hotblocks tables related to the diven block machine name
 */
function hotblocks_delete_form_submit($form, &$form_state) {
  $delta = $form_state['values']['delta'];
  $label = hotblocks_get_block_label($delta);

  // Delete all remnants of this block from the database
  db_query("DELETE FROM {hotblocks} WHERE delta = :delta", array(':delta' => $delta));
  db_query("DELETE FROM {hotblocks_weight} WHERE delta = :delta", array(':delta' => $delta));
  db_query("DELETE FROM {hotblocks_settings} WHERE machine_name = :delta", array(':delta' => $delta));

  drupal_set_message("Deleted hotblock: $label");
}