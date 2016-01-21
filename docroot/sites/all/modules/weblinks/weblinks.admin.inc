<?php

/**
 * @file
 * Settings form.
 */

/**
 * Module settings page.
 */
function weblinks_settings_page() {
  return drupal_get_form('weblinks_admin_settings');
}

/**
 * Module settings form.
 */
function weblinks_admin_settings() {
  drupal_add_js(drupal_get_path('module', 'weblinks') . '/weblinks.admin.js');
  drupal_add_css(drupal_get_path('module', 'weblinks') . '/weblinks.admin.css');
  $form = array('#attributes' => array('class' => array('weblinks-settings')));
  $taxo_avail = module_exists('taxonomy');

/**
 * Overall settings.
 */
  $yesno = array(1 => t('Yes'), 0 => t('No'));

  $vocabs = $taxo_avail ? taxonomy_get_vocabularies() : array();
  $vocab_field_info = _weblinks_vocabulary_field_info();
  $weblinks_fields = field_read_fields(array('bundle' => 'weblinks'));
  if (count($vocabs) > 1) {
    $avail_vocs = array();
    $item_options = array();
    $disabled_warning = '';
    // Create a set of radio buttons for choosing the vocabulary.
    foreach ($vocabs as $vid => $vocabulary) {
      $avail_vocs[$vid] = check_plain($vocabulary->name);
      // Add a wrapper so that vocabulary description stays with the button.
      $item_options[$vid] = array(
        '#description' => check_plain($vocabulary->description),
        '#prefix' => '<span class="weblinks-radios-wrapper">',
        '#suffix' => '</span>',
      );
      // If there is no field for this vocabulary then disable the radio button.
      if (!isset($vocab_field_info['all_vocab_fields'][$vocabulary->machine_name])) {
        $item_options[$vid]['#disabled'] = TRUE;
        $item_options[$vid]['#prefix'] = '<span class="weblinks-radios-wrapper weblinks-disabled">';
        $disabled_warning = t('To use a vocabulary which is disabled above add the appropriate term reference field via the <a href="@link">Weblinks Content Type</a> page.', array(
          '@link' => url('admin/structure/types/manage/weblinks/fields'),
        ));
      }
    }
    $description = t('Select the vocabulary to use for grouping Web Links.') . ' ' . $disabled_warning;
    $default = variable_get('weblinks_nav_vocabulary', 0);
    if (!empty($item_options[$default]['#disabled'])) {
      // If the field for the current weblinks vocabulary has been deleted then
      // remove the default, to force another vocabuary to be selected.
      drupal_set_message(t('Please select a new navigation vocabulary because the field for %field does not exist in the <a href="@link">Weblinks Content Type</a>.', array(
        '%field' => $avail_vocs[$default],
        '@link' => url('admin/structure/types/manage/weblinks/fields'),
      )), 'warning');
      $default = NULL;
    }
    $form['weblinks_nav_vocabulary'] = array(
      '#type' => 'radios',
      '#title' => t('Vocabulary to use for Weblinks'),
      '#description' => $description,
      '#default_value' => $default,
      '#options' => $avail_vocs,
      '#required' => TRUE,
      '#prefix' => '<div class="weblinks-radios">',
      '#suffix' => '</div><div class="clearfix"></div>',
      );
    $form['weblinks_nav_vocabulary'] += $item_options;
  }
  else {
    if (count($vocabs) < 1 && $taxo_avail) {
      variable_del('weblinks_nav_vocabulary');
      _weblinks_get_vocid();
      drupal_set_message(t('No vocabulary for the weblinks content type was found. A new vocabulary was created.'));
    }
  }

  $len_opts = drupal_map_assoc(array(0, 10, 15, 20, 30, 40, 50, 60, 70, 80, 128, 256));

  $form['weblinks_trim_block'] = array(
    '#type' => 'select',
    '#options' => $len_opts,
    '#title' => t('Maximum block title length'),
    '#default_value' => variable_get('weblinks_trim_block', 0),
    '#description' => t('This controls how much of a node title will be shown in a block to conserve space. Zero (0) means unlimited.'),
    );

  $form['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links page settings'),
    '#description' => t('These settings control the main links page display.'),
    '#attributes' => array('id' => 'weblinks-page-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

/**
 * Settings for the links page.
 */
  $form['page']['weblinks_new_redirect'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Redirect to link page after new link'),
    '#default_value' => (int) variable_get('weblinks_new_redirect', 1),
    '#description' => t('Enable this if you want to redirect the user to the main links page after they enter a new link.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['page']['weblinks_collapsible'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Make groups collapsible'),
    '#default_value' => (int) variable_get('weblinks_collapsible', TRUE),
    '#description' => t('Enable this if you want the groups to be collapsible on the main links page. Note that if the "Unclassified" title is empty, that group will not be collapsible.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['page']['weblinks_collapsed'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Collapse all groups'),
    '#default_value' => (int) variable_get('weblinks_collapsed', FALSE),
    '#description' => t('Enable this if you want all the groups to be collapsed on the main links page.')
      . ' ' . t('For mixed settings, select "No" here then edit the individual group and select "Yes" to collapse that group.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks_collapse_hide weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['page']['weblinks_linktitle'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Link group title to group'),
    '#default_value' => (int) variable_get('weblinks_linktitle', TRUE),
    '#description' => t('Enable this if you want the group titles to be a link to show only that group.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks-link-title weblinks-radios">',
    '#suffix' => '</div>',
    );

  $sort_options = array(
    'standard' => t('Normal Drupal - by weight, sticky, created date.'),
    'title' => t('Legacy - by weight, sticky, title.'),
    );
  $sort_description_links = array(
    '!weblinks_content_type' => url('admin/structure/types/manage/weblinks'),
    '!weight_project_page' => 'http://www.drupal.org/project/weight',
  );
  $sort_description = t('This determines how the links page will sort the links.') . '<br>';
  if (module_exists('weight')) {
    if (in_array('weblinks', _weight_get_types())) {
      $sort_description .= t('The <a href="!weight_project_page">weight module</a> is enabled for Web Links. You can adjust the settings via the <a href="!weblinks_content_type">web links content type</a> page.', $sort_description_links);
    }
    else {
      $sort_description .= t('The <a href="!weight_project_page">weight module</a> is available, but is not turned on for Web Links. Enable it via the <a href="!weblinks_content_type">web links content type</a> page.', $sort_description_links);
    }
  }
  else {
    $sort_description .= t('The Weight module can be used to further refine the link order. Download it from the <a href="!weight_project_page">weight project page</a>', $sort_description_links);
  }
  $form['page']['weblinks_page_sort'] = array(
    '#type' => 'radios',
    '#options' => $sort_options,
    '#title' => t('Page Sort order'),
    '#default_value' => variable_get('weblinks_page_sort', 'title'),
    '#description' => $sort_description,
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $form['page']['weblinks_pagedesc'] = array(
    '#type' => 'textarea',
    '#title' => t('Page information'),
    '#rows' => 3,
    '#default_value' => variable_get('weblinks_pagedesc', NULL),
    '#description' => t('This description will appear at the top of the links page.  It will be filtered by your default input format.'),
    '#required' => FALSE,
    );

  $form['page']['weblinks_maxfrontdepth'] = array(
    '#type' => 'select',
    '#title' => t('Maximum group depth (main links page)'),
    '#options' => array(100 => 'Unlimited') + drupal_map_assoc(range(1, 20)),
    '#default_value' => variable_get('weblinks_maxfrontdepth', 100),
    '#description' => t('This controls how many group levels will be displayed on the main Web Links page.'),
  );

  $form['page']['weblinks_maxdepth'] = array(
    '#type' => 'select',
    '#title' => t('Maximum group depth (group pages)'),
    '#options' => array(100 => 'Unlimited') + drupal_map_assoc(range(1, 20)),
    '#default_value' => variable_get('weblinks_maxdepth', 100),
    '#description' => t('This controls how many group levels will be displayed beneath the starting term on group pages.'),
  );

  $form['page']['weblinks_admin_override'] = array(
    '#type' => 'radios',
    '#title' => t('Override behavior for Web Links administrators'),
    '#options' => array(
      'expand' => t('Expanded group'),
      'collapse' => t('Collapsed group'),
      'hide' => t('Hidden (as for ordinary users)'),
    ),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#default_value' => variable_get('weblinks_admin_override', 'collapse'),
    '#description' => t('For groups that exceed the "Maximum Depth" or are set not to be shown on the front page, this option allows Admin users to see the groups.'),
  );

  // Add "rel=nofollow."
  $form['page']['weblinks_nofollow'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Spam link deterrent'),
    '#default_value' => (int) variable_get('weblinks_nofollow', FALSE),
    '#description' => t('If enabled, Drupal will add rel="nofollow" to all links, as a measure to reduce the effectiveness of spam links. Note: this will also prevent valid links from being followed by search engines, therefore it is likely most effective when enabled for anonymous users.'),
    );

/**
 * Settings for group display.
 */
  $form['group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Group settings'),
    '#description' => t('These settings control the way the groups display on the main links page.'),
    '#attributes' => array('id' => 'weblinks-group-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['group']['weblinks_catdesc'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Display group descriptions'),
    '#default_value' => (int) variable_get('weblinks_catdesc', TRUE),
    '#description' => t('Generally recommended.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['group']['weblinks_empty_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Empty container text'),
    '#rows' => 2,
    '#default_value' => variable_get('weblinks_empty_text', NULL),
    '#description' => t('This text will appear when a container is empty. Leave it blank to not show the group at all. It will be filtered by your default input format.'),
    );

  $form['group']['weblinks_unclassified_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for unclassified links'),
    '#default_value' => variable_get('weblinks_unclassified_title', t('Unclassified')),
    '#description' => t('This will be used as the group title for unclassified links. You may leave it blank for no title.'),
    );

  $form['group']['weblinks_unclassified_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Description for unclassified links'),
    '#rows' => 1,
    '#default_value' => variable_get('weblinks_unclassified_desc', t('These links have not been assigned a group.')),
    '#description' => t('This will be used as the description for the unclassified links group. It may be left blank.'),
    );

  $form['group']['weblinks_unpublished_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for unpublished links'),
    '#default_value' => variable_get('weblinks_unpublished_title', t('Unpublished')),
    '#description' => t('This will be used to describe the unpublished links. If you leave it blank, there will be no separate "unpublished" group on the links page; this can be desirable if your site\'s theme identifies unpublished content.'),
    );

  $form['group']['weblinks_unpublished_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Description for unpublished links'),
    '#rows' => 1,
    '#default_value' => variable_get('weblinks_unpublished_desc', t('These links are not published and need to be reviewed.')),
    '#description' => t('This will be used as the description for the unpublished links group.  It may be left blank.'),
    );

/**
 * Settings for link display.
 */
  $form['link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link settings'),
    '#description' => t('These settings control the way individual links display on the main links page.'),
    '#attributes' => array('id' => 'weblinks-link-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['link']['weblinks_show_title'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Show the node title'),
    '#default_value' => (int) variable_get('weblinks_show_title', TRUE),
    '#description' => t('If your theme automatically provides a node title, set this to "No." If you see duplicated titles, then set this to "No" and see if that resolves the problem.'),
    );

  $form['link']['weblinks_title_link'] = array(
    '#type' => 'radios',
    '#options' => array(1 => t('Node'), 0 => t('Site')),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Link the title to the node'),
    '#default_value' => (int) variable_get('weblinks_title_link', TRUE),
//    '#description' => t('This makes the node title link to the node. For some themes, this may not work without also making some simple theme changes.'),
    '#description' => t('When clicking the node title on the links page, "Node" takes the user to the standard node display; "Site" takes the user to the site (using click tracking if it is enabled). Note that "open in a new page" and "spam deterrent" are not available to the title link without theme changes.'),
    );

  $desc_opts = array(
    'none' => t('No description'),
    'teaser' => t('Teaser length'),
    'body' => t('Full length'),
    );
  $form['link']['weblinks_linkdesc'] = array(
    '#type' => 'radios',
    '#options' => $desc_opts,
    '#title' => t('Link description display'),
    '#default_value' => variable_get('weblinks_linkdesc', 'teaser'),
    '#description' => t('This sets how the description will be displayed on the main page and group pages.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  // Get the entity info and make a selection list.
  $entity_info = entity_get_info('node');
  $bundle_settings = variable_get('field_bundle_settings_node__weblinks');
  $view_modes = array('default' => t('Default'));
  if (isset($bundle_settings['view_modes'])) {
    foreach ($bundle_settings['view_modes'] as $name => $data) {
      // The view mode tabs on /admin/structure/types/manage/weblinks/display
      // are those which have custom_settings TRUE in the bundle settings array
      // and also are present in the $entity_info 'view_modes' array.
      if ($data['custom_settings'] && isset($entity_info['view modes'][$name])) {
        $view_modes[$name] = $entity_info['view modes'][$name]['label'];
      }
    }
  }
  $form['link']['weblinks_viewmode'] = array(
    '#type' => 'select',
    '#title' => t('View Mode'),
    '#options' => $view_modes,
    '#default_value' => variable_get('weblinks_viewmode', 'teaser'),
    '#description' => t("The view mode determines how the node contents are displayed on the main page and group pages. You can customise the display at <a href='@link'>Content Type - Manage Display</a>", array('@link' => url('admin/structure/types/manage/weblinks/display'))),
    '#multiple' => FALSE,
  );

  $form['link']['weblinks_redirect'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Enable click-tracking'),
    '#default_value' => (int) variable_get('weblinks_redirect', FALSE),
    '#description' => t('This turns on the feature to record the number of times a link has been clicked upon. It is a requirement for the "Popular" block.'),
    );

  $form['link']['weblinks_external'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t('Open links in a new window'),
    '#default_value' => (int) variable_get('weblinks_external', TRUE),
    '#description' => t('Recommended to keep your vistors on your site. Note that this will cause HTML "Strict" validation errors if used.'),
    );

  $form['link']['weblinks_user_links'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t("Display a link to other users' links"),
    '#default_value' => (int) variable_get('weblinks_user_links', FALSE),
    '#description' => t("Shows a link to another user's links."),
    );

  $form['link']['weblinks_extra_classes'] = array(
    '#type' => 'textfield',
    '#title' => t("Additional link classes"),
    '#default_value' => variable_get('weblinks_extra_classes', ''),
    '#description' => t("These are extra classes that are added to the link. Separate them with spaces (as you would in HTML)"),
    );

  $form['link']['weblinks_show_url'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    '#title' => t("Display URL/Visit on links page"),
    '#default_value' => (int) variable_get('weblinks_show_url', TRUE),
    '#description' => t("Determines whether the URL or invitational link (next setting) will be used on the links page. It will always be shown on a node view page."),
    );

  $form['link']['weblinks_view_as'] = array(
    '#type' => 'radios',
    '#title' => t('Link displays as'),
    '#options' => array('url' => t('URL'), 'visit' => t('Visit')),
    '#default_value' => variable_get('weblinks_view_as', 'url'),
    '#description' => t('This option sets whether the URL itself will be displayed or will hide behind an invitational link, such as "Visit <em>title</em>".'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $token = function_exists('token_replace');
  $form['link']['weblinks_visit_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Visit" text'),
    '#default_value' => variable_get('weblinks_visit_text', t('Visit [title]')),
    '#description' => t('This is the text that will be used with the node title (indicated by "[title]") to invite a click.'),
    '#prefix' => '<div class="weblinks_visit_hide">',
    );

  if ($token) {
    $form['link']['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#theme' => 'token_tree',
      '#token_types' => array('node', 'weblinks'),
      '#global_types' => FALSE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      );
  }

  // The end div here ends the "weblinks_visit_hide" above.
  $form['link']['weblinks_trim'] = array(
    '#type' => 'select',
    '#options' => $len_opts,
    '#title' => t('Maximum link title length'),
    '#default_value' => variable_get('weblinks_trim', 0),
    '#description' => t('This controls how much of long URLs will be shown in a link in full node view. Zero (0) means unlimited.'),
    '#prefix' => '</div><!-- /weblinks_visit_hide --><div class="weblinks_trim_hide">',
    );

  $form['link']['weblinks_strip'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="weblinks-radios">',
//    '#suffix' => '</div>',
    '#title' => t('Strip protocol and "www"'),
    '#default_value' => (int) variable_get('weblinks_strip', FALSE),
    '#description' => t('If enabled, the protocol ("http://") and "www" (if present) will be removed before displaying the URL.'),
    '#suffix' => '</div></div><!-- /weblinks_trim_hide -->',
    );

/**
 * Settings for field defaults.
 */
  $form['field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field defaults'),
    '#description' => t("These settings provide various defaults for fields that may be entered. Be careful when changing these as they can interfere with a user's input."),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  $form['field']['weblinks_url_stub'] = array(
    '#type' => 'textfield',
    '#title' => t('Default for URL'),
    '#default_value' => variable_get('weblinks_url_stub', 'http://'),
    '#description' => t('This will be used as a stub in the URL field.'),
    );

  $form['field']['weblinks_body_stub'] = array(
    '#type' => 'textfield',
    '#title' => t('Default for description'),
    '#default_value' => variable_get('weblinks_body_stub', ''),
    '#description' => t('This will be used as a stub in the "Link description" field.'),
    );

  // If the user cannot administer filters then they cannot change them here.
  if (user_access('administer filters')) {
    $format_list = filter_formats();
    $formats = array(filter_fallback_format() => '-default-');
    foreach ($format_list as $number => $filter) {
      $formats[$number] = $filter->name;
    }
    $form['field']['weblinks_format'] = array(
      '#type' => 'radios',
      '#options' => $formats,
      '#title' => t('Default input format'),
      '#default_value' => variable_get('weblinks_format', filter_fallback_format()),
      '#description' => t('This will be used as the default input format.'),
      '#prefix' => '<div class="weblinks-radios">',
      '#suffix' => '</div>',
      );
  }

//  $form['#redirect'] = 'weblinks';
  return system_settings_form($form);
}

