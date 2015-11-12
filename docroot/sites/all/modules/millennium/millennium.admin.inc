<?php
/**
 * @file
 * Callback functions for settings forms.
 */

/**
 * Callback for drupal_get_form for general settings form
 */
function millennium_admin_settings_form($form, &$form_state) {
  $form = array();
  $sources = millennium_get_sources_count();
  if (sizeof($sources) == 0) {
    drupal_set_message(t('You have not yet set any sources.'), 'warning');
  }

  $node_types = node_type_get_types();
  $options = array();
  foreach ($node_types as $obj) {
    $options[$obj->type] = $obj->name;
  }

  $form['general'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t("General settings"),
  );

  $form['general']['millennium_marc_node_type'] = array(
    '#type' => 'select',
    '#title' => t("Content type to import records into"),
    '#description' => t('Select content type into which you want to import bibliographic records. It is recommended you first create a new content type at !url.', array('!url' => l(t('Add content type'), 'admin/structure/types/add', array('query' => array('destination' => current_path()) )))),
    '#options' => $options,
    '#default_value' => variable_get('millennium_marc_node_type', 'page'),
  );

  $form['general']['millennium_action_on_last_item_deleted'] = array(
    '#type' => 'radios',
    '#title' => t('Action when imported records are no longer found in WebOPAC'),
    '#options' => array(
      'delete' => t("Delete node"),
      'unpublish' => t("Unpublish node"),
    ),
    '#default_value' => variable_get('millennium_action_on_last_item_deleted', 'unpublish'),
  );
  $form['general']['millennium_availability_strings'] = array(
    '#type' => 'textfield',
    '#title' => t('Status messages that mean items are available'),
    '#description' => t("Enter the exact strings (separated by commas) that appear in items' holdings table that to you mean it is available. Case does not matter. For example: %example", array('%example' => MILLENNIUM_ITEM_AVAILABLE_STRINGS)),
    '#default_value' => variable_get('millennium_availability_strings', MILLENNIUM_ITEM_AVAILABLE_STRINGS),
  );

  // Sources fieldset
  $form['sources'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Source OPACs'),
    '#description' => t('You can enter OPAC sources that will be available for batch import and other operations.'),
  );

  // Show table for existing OPAC sources
  $rows = array();
  $headers = array(t('Action'), t('OPAC base URL'), t('OPAC name'), t('Import stats'));
  foreach ($sources as $base_url => $source_data) {
    // Element to remove this source
    $remove_element = array(
      '#id' => 'remove_source_' . $base_url,
      '#parents' => array(0 => NULL),
      '#value' => NULL,
      '#name' => 'remove_source_' . $base_url,
      '#type' => 'checkbox',
      '#title' => t('Remove'),
    );
    // Element to edit existing source name
    $edit_name_element = array(
      '#id' => 'edit_name_' . $base_url,
      '#parents' => array(0 => NULL),
      '#name' => 'edit_name_' . $base_url,
      '#type' => 'textfield',
      '#value' => $source_data["name"],
      '#size' => 30,
    );
    // Add row to table
    $count = (isset($sources[$base_url]['node_count'])) ? $sources[$base_url]['node_count'] : 0;
    $rows[] = array(
      drupal_render($remove_element),
      l($base_url, $base_url),
      drupal_render($edit_name_element),
      t('@count nodes', array('@count' => $count)),
    );
  }

  // Add a final table row to allow adding new sources
  $add_url_element = array(
    '#id' => 'add_url',
    '#value' => '',
    '#parents' => array(0 => NULL),
    '#name' => 'add_url',
    '#type' => 'textfield',
    '#size' => 30,
    '#required' => TRUE,
    '#description' => t('A WebOPAC base URL.') . '<br />' . t('Example: http://example.com/'),
  );
  $add_name_element = array(
    '#id' => 'add_name',
    '#value' => '',
    '#parents' => array(0 => NULL),
    '#name' => 'add_name',
    '#type' => 'textfield',
    '#size' => 30,
    '#description' => t('A name for this WebOPAC.') . '<br />' . t('Example: "Test University Libraries"'),
  );
  $rows[] = array(t('Add new source:'), drupal_render($add_url_element), drupal_render($add_name_element), '');
  // Add the themed table to the form
  $form['sources']['table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array('header' => $headers, 'rows' => $rows)),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  // Render as a system settings form.
  return $form; #return system_settings_form($form);
}

/**
 * Validate handler for settings form; handles special values in table.
 */
function millennium_admin_settings_form_validate($form, &$form_state) {
  $sources = variable_get('millennium_sources', array());

  // Validate sources to remove
  foreach ($sources as $base_url => $source_data) {
    $base_url_as_index = str_replace(".", "_", $base_url);
    if (isset($form_state['input']["remove_source_{$base_url_as_index}"])) {
      // Check if there are existing records for this base_url
      $count = (int) db_query("SELECT count(1) FROM {millennium_node_bib} WHERE base_url = :base_url", array(':base_url' => $base_url))->fetchField();
      if ($count) {
        form_set_error("remove_source_{$base_url_as_index}", t('The source %url cannot be removed since nodes that depend on it still exist.', array('%url' => $new_url)));
      }
    }
  }

  // Validate a new URL if given
  $new_url = trim($form_state['input']['add_url']);
  $new_name = trim($form_state['input']['add_name']);
  if ($new_url) {
    // Normalize the base url, eliminate markup or XSS
    $new_url = check_plain(millennium_get_real_baseurl($new_url));

    // Check for valid URL
    if (strpos($new_url, 'http://') !== 0) {
      form_set_error('add_url', t('The entered url %url is not valid.', array('%url' => $new_url)));
      return;
    }

    // Check for unique base URL
    if (isset($sources[$new_url])) {
      form_set_error('add_url', t('The url %url is already defined.', array('%url' => $new_url)));
      return;
    }

    // Ping the WebOPAC, alert if not pingable
    if (!millennium_ping($new_url)) {
      drupal_set_message(t('Warning: The WebOPAC at <a href="@url">@url</a> is not responding.', array('@url' => $new_url)));
    }
  }
  // All is OK, store the normalized URL for the submit function
  $form_state['storage']['add_url'] = $new_url;
}

/**
 * Submit handler for settings form; handles special values that are not handled
 * by system_settings_form_submit();
 */
function millennium_admin_settings_form_submit($form, &$form_state) {
  $sources = variable_get('millennium_sources', array());

  $vars = array('millennium_marc_node_type', 'millennium_action_on_last_item_deleted', 'millennium_availability_strings');
  // Set system variables
  foreach ($vars as $var) {
    variable_set($var, $form_state['input'][$var]);
  }

  // Update any changes to sources
  foreach ($sources as $base_url => $source_data) {
    $base_url_as_index = str_replace(".", "_", $base_url);
    // Removal checked?
    if (isset($form_state['input']["remove_source_{$base_url_as_index}"])) {
      drupal_set_message(t('Removed source %url', array('%url' => $base_url)));
      unset($sources[$base_url]);
      continue;
    }

    // Name change
    $sent_name = check_plain(trim($form_state['input']["edit_name_{$base_url_as_index}"]));
    if ($sent_name != $source_data["name"]) {
      // Update taxonomy term if mapping currently set
      $opacname_machine_name = variable_get('millennium_opacname_vid', -1);
      if ($opacname_machine_name != -1) {
        $opacname_vid = db_query('SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = :name', array(':name' => $opacname_machine_name))->fetchField();
        // Find and load existing term
        $tid = db_query('SELECT tid FROM {taxonomy_term_data} WHERE vid = :vid and name = :name', array(':vid' => $opacname_vid, ':name' => $source_data["name"]))->fetchField();
        $term = taxonomy_term_load($tid);
        $term->name = $sent_name;
        // Save term's new name
        taxonomy_term_save($term);
        if ($sent_name != '') {
          drupal_set_message(t('Updated taxonomy term for %opacname.', array('%opacname' => $sent_name)));
        }
      }
      // Set name in $sources array
      $sources[$base_url]["name"] = $sent_name;
    }
  }

  // If sent, add the new url and name to the sources
  $new_url = $form_state['storage']['add_url'];
  $new_name = trim($form_state['input']['add_name']);
  if ($new_url) {
    $sources[$new_url] = array('name' => check_plain($new_name));
    drupal_set_message(t('Added source %url', array('%url' => $new_url)));
  }

  // Store the new value for sources
  millennium_variable_set('millennium_sources', $sources);
}

/**
 * Callback for drupal_get_form for crawl settings form.
 */
function millennium_admin_crawl($form, &$form_state) {
  $form = array();
  $run_cron_url = l(t('run cron'), 'admin/reports/status/run-cron', array('query' => array('destination' => MILLENNIUM_SETTINGS_PATH . '/crawl')));
  $form['crawl'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t("Automatic crawl settings"),
    "#description" => t('These settings affect the import operations run during cron calls. You can !run_cron manually.', array('!run_cron' => $run_cron_url)),
  );

  $form['crawl']['millennium_crawl_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic crawl enabled'),
    '#description' => t('Enabling this will start (or continue) the automatic record import/update. Disabling this does not stop importing items queued manually for import.'),
    '#default_value' => variable_get('millennium_crawl_flag', 0),
  );
  $form['crawl']['millennium_newitems_author_name'] = array(
    '#type' => 'textfield',
    '#title' => t("User for new items"),
    '#description' => t('Only applies to new items; existing items retain their "authored by" user information.'),
    '#default_value' => variable_get('millennium_newitems_author_name', millennium_default_author()),
    '#size' => 20,
    '#autocomplete_path' => 'user/autocomplete',
  );
  $form['crawl']['millennium_crawl_baseurl'] = array(
    '#type' => 'select',
    '#title' => t('Source WebOPAC'),
    '#description' => t('Select one of the enabled sources.'),
    '#default_value' => variable_get('millennium_crawl_baseurl', ''),
    '#options' => drupal_map_assoc(array_merge(array(0 => t('<none>')), array_keys(variable_get('millennium_sources', array())))),
  );
  $form['crawl']['millennium_webopac_maxrecords'] = array(
    '#type' => 'select',
    '#title' => t("Items to import per cron run"),
    '#options' => array(10 => 10, 20 => 20, 50 => 50, 100 => 100, 200 => 200, 300 => 300, 400 => 400, 500 => 500),
    '#default_value' => variable_get("millennium_webopac_maxrecords", 50),
    '#description' => t('Items can be imported during cron calls. This number limits the maximum amount of items that will be imported each time cron is run. Set this number lower if your cron is timing out or if PHP is running out of memory.'),
  );
  $form['crawl']['millennium_crawl_type'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('millennium_crawl_type', 'b'),
    '#options' => array(
      'i' => t('Item records'),
      'b' => t('Bibliographic records'),
    ),
    '#title' => t('Record type to crawl'),
    '#description' => t('The recommended setting is "Bibliographic". Crawling item records allows gathering number of checkouts and other information but generally takes longer.'),
  );
  $form['crawl']['millennium_webopac_start_itemrecord'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting record number for crawl'),
    '#description' => t('WebOpac records normally start at record #10000. Record numbers below this are NOT imported.'),
    '#default_value' => variable_get('millennium_webopac_start_itemrecord', 10000),
    '#size' => 10,
  );
  $form['crawl']['millennium_webopac_end_itemrecord'] = array(
    '#type' => 'textfield',
    '#title' => t('APPROXIMATE maximum record number'),
    '#description' => t('This is an estimated maximum record number, which might change on its own when the crawler reaches it. Record numbers higher than this number MAY BE imported.'),
    '#default_value' => variable_get('millennium_webopac_end_itemrecord', 20000),
    '#size' => 10,
  );

  $form['crawl']['millennium_webopac_reset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Restart crawl from above starting record?'),
    '#description' => t('WARNING: Set ONLY if you want to start over from that number onwards.'),
    '#default_value' => variable_get('millennium_webopac_reset', FALSE),
  );
  return system_settings_form($form);
}

/**
 * Validate the username entered in the form.
 * @see millennium_admin_crawl()
 */
function millennium_admin_crawl_validate($form, &$form_state) {
  $author_name = $form_state['values']['millennium_newitems_author_name'];
  $user = array_shift(user_load_multiple(array(), array('name' => $author_name)));
  if (!$user) {
    form_set_error('millennium_newitems_author_name', t('Please enter a valid username.'));
  }
}

/**
 * Callback for drupal_get_form for display settings form.
 */
function millennium_admin_display($form, &$form_state) {
  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t("General"),
  );
  $form['general']['millennium_display_biblio_data'] = array(
    '#type' => 'radios',
    '#title' => t('Display bibiographic data table in node'),
    '#options' => array(
      0 => t('No'),
      'full' => t('Full view only'),
      'teaser' => t('Teaser & Full view'),
    ),
    '#default_value' => variable_get('millennium_display_biblio_data', "teaser"),
    '#description' => t('You might want to switch this off if you are mapping values into CCK fields. Recommended setting is "Teaser & Full view".'),
  );
  $form['general']['millennium_coverimage_baseurl'] = array(
    '#type' => 'textfield',
    '#title' => '(' . t('Optional') . ') ' . t('URL for displaying cover images'),
    '#default_value' => variable_get('millennium_coverimage_baseurl', ''),
    '#description' => t('Use the placeholder !id in the URL where the ISBN or ISBN will be placed. You can also specify: !type (Biblio Type number), !tit (Title), !aut (Author(s))') . ' ' . t('For example') . ': http://example.com/getimage?isbn=!id&type=!type',
  );
  $form['general']['millennium_display_hold_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show link to place "request item" page at source OPAC'),
    '#description' => t('This link will take the user to the originating OPAC.'),
    '#default_value' => variable_get('millennium_display_hold_link', 1),
  );
  $form['general']['millennium_display_original_record_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show link to the original record at source OPAC'),
    '#description' => t('This link will take the user to the originating OPAC.'),
    '#default_value' => variable_get('millennium_display_original_record_link', 1),
  );

  $form['holdings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t("Holdings display"),
  );

  $form['holdings']['millennium_realtime_holdings'] = array(
    '#type' => 'radios',
    '#title' => t('Display real-time availability information'),
    '#options' => array(0 => t('No'), "full" => t('Full view only'), "teaser" => t('Teaser & Full view')),
    '#default_value' => variable_get('millennium_realtime_holdings', "full"),
    '#description' => t('WARNING: This option might slow down your page views, as availability information is fetched from the Millennium for each record (and kept in a Drupal cache for subsequent calls). Recommended setting is "Full view only".'),
  );

  $form['holdings']['millennium_holdings_opacname'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show OPAC name in record holdings table'),
    '#description' => t('Enabling this will show the OPAC name entered in the Sources tab next to each item in the holdings table.'),
    '#default_value' => variable_get('millennium_holdings_opacname', 1),
  );

  $form['holdings']['millennium_preferred_locations'] = array(
    '#type' => 'textfield',
    '#title' => '(' . t('Optional') . ') ' . t('Locations to show in teaser view availability (comma-separated)'),
    '#default_value' => variable_get('millennium_preferred_locations', ''),
    '#description' => t('Comma-separated location names, as they appear on the WebOpac, that will be given priority over other locations. For example, entering "CSU" will match locations like "CSU General stacks" and "CSU Storage", showing only those in teasers and showing those above others in full record views.'),
  );

  $form['gbooks'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t("Google Book Search integration"),
  );
  $form['gbooks']['millennium_googlebooks'] = array(
    '#type' => 'radios',
    '#title' => t('Add links to Google Book Search'),
    '#options' => array(
      0 => t('No'),
      'full' => t('Full view only'),
      'teaser' => t('Teaser & Full view'),
    ),
    '#default_value' => variable_get('millennium_googlebooks', "teaser"),
    '#description' => t('This will show a link to Google Book Search whenever a limited or full view version is found on Google. Note: This setting loads a small script hosted by Google for each page of items shown.'),
  );
  $form['gbooks']['millennium_googlebooks_covers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace empty cover images with Google Book Search covers'),
    '#default_value' => variable_get('millennium_googlebooks_covers', TRUE),
    '#description' => t('This will only work when the "Add links to Google Book Search" option is enabled.'),
  );

  $form['gbooks']['millennium_googlebooks_widget'] = array(
    '#type' => 'radios',
    '#title' => t('Show embedded Google Book Search preview in full node view'),
    '#options' => array(
      0 => t("No"),
      1 => t("Yes"),
    ),
    '#default_value' => variable_get('millennium_googlebooks_widget', 0),
    '#description' => t('This will show an embedded Google Books viewer when the item is found on Google Book Search. Note: Only works when "Add links to Google Book Search" option is enabled. This setting loads an additional script hosted by Google.'),
  );

  return system_settings_form($form);
}

/**
 * Callback for drupal_get_form for mapping settings form.
 */
function millennium_admin_mapping($form, &$form_state) {
  $form = array();
  $millennium_node_type = variable_get('millennium_marc_node_type', 'page');
  $field_termreference = field_info_instances('node', $millennium_node_type);
  $v_options[-1] = t("<No mapping>");
  foreach ($field_termreference as $fieldname => $fieldinfo) {
    if ($fieldinfo['widget']['module'] == 'taxonomy') {
      $v_options[$fieldname] = t('taxonomy field: @name', array('@name' => $fieldinfo['label']));
    }
  }

  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('If you want to browse your imported records via taxonomy, you must assign a mapping here.')
      . '<ul><li>'
      . t('You can only map to taxonomy vocabularies that have a "Tags" setting and are assigned to the destination node type (currently set to %type in the !sourceslink tab).', array('%type' => variable_get('millennium_marc_node_type', '<none>'), '!sourceslink' => l(t('sources'), MILLENNIUM_SETTINGS_PATH)))
      . '</li><li>'
      . t('NOTE: CHANGING these values after import does not change already-imported records! You can use the !link page to refresh mappings using already-imported data.', array('!link' => l(t('Millennium batch import/refresh'), 'admin/content/millennium')))
      . '</li></ul>',
  );
  $form['marc'] = array(
    '#type' => 'fieldset',
    '#title' => t("MARC to taxonomy mappings"),
    '#collapsible' => FALSE, // DUE to problems http://drupal.org/node/152431
  );

  $form['marc']['millennium_taxonomy_language'] = array(
    '#type' => 'radios',
    '#title' => t('Language used to create taxonomy terms and other additional information'),
    '#options' => array(
      'english' => t("English (recommended)"),
      'site_default' => t("Site default language, currently @name", array('@name' => language_default('name'))),
    ),
    '#default_value' => variable_get('millennium_taxonomy_language', 'english'),
    '#description' => t('Choose a language other than English only if your site is in a single
language. To use multiple languages with i18n taxonomy translation
enabled, choose English here.'),
    '#suffix' => '<table>',
  );

  // Issue warning when no applicable vocabularies are available
  if (sizeof($v_options) == 1) {
    $form['marc']['warning'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="warning">',
      '#suffix' => '</div>',
      '#markup' =>
        t('There are no applicable vocabularies to assign mappings to.')
        . ' '
        . t('You can only map to taxonomy vocabularies that have a "Tags" setting.')
        . ' '
        . t('You can add or configure vocabularies at the !link page.', array('!link' => l(t('Administer Taxonomy'), 'admin/structure/taxonomy'))),
    );
    return system_settings_form($form);
  }

  $vocs = array(
    'subject_kw' => t('Subject:main and general subdivision'),
    'subject_time' => t('Subject:chronological'),
    'subject_form' => t('Subject:form'),
    'subject_geo' => t('Subject:geographic'),
    'leader_item_type' => t('Item type'),
    'year' => t('Year published'),
    'language' => t('Item language'),
    'author' => t('Author'),
    'series' => t('Series Uniform Title'),
  );
  $descs = array(
    'subject_kw' => t('MARC 6xx, subfields a,x'),
    'subject_time' => t('MARC 6xx, subfield y'),
    'subject_form' => t('MARC 6xx, subfield v'),
    'subject_geo' => t('MARC 6xx, subfield z'),
    'leader_item_type' => t('MARC Leader, position 6'),
    'year' => t('MARC 008, position 6'),
    'language' => t('MARC 008, position 35'),
    'author' => t('MARC 100s, 110s, 700s, 710s (all subfields merged together as text)'),
    'series' => t('MARC 830, subfield a'),
  );

  foreach ($vocs as $code => $voc_name) {
    $name = "millennium_marc_vid_$code";
    $form['marc'][$name] = array(
      '#type' => 'select',
      '#prefix' => '<tr><td style="width:30%">' . $voc_name . '<div class=description>' . $descs[$code] . '</div></td><td>' . t("into") . '</td><td>',
      '#suffix' => '</td></tr>',
      '#options' => $v_options,
      '#default_value' => variable_get($name, -1)
    );
  }

  // Fieldset for OPAC Name
  $form['opacname'] = array(
    '#prefix' => '</table></fieldset>',
    '#type' => 'fieldset',
    '#title' => t("OPAC name to taxonomy mapping"),
    '#collapsible' => FALSE,
    '#description' => t("These settings map the originating OPAC's name (defined in the !sourceslink tab) into a taxonomy vocabulary.", array('!sourceslink' => l(t('sources'), MILLENNIUM_SETTINGS_PATH))),
  );
  $form['opacname']['millennium_opacname_vid'] = array(
    '#type' => 'select',
    '#title' => t('Map originating OPAC name into'),
    '#options' => $v_options,
    '#default_value' => variable_get('millennium_opacname_vid', -1),
  );

  $form['availability'] = array(
    '#type' => 'fieldset',
    '#title' => t("Holdings locations to taxonomy mapping"),
    '#collapsible' => FALSE,
    '#description' => t("These settings map location names from records' holdings tables into a taxonomy vocabulary."),
  );
  // Selector for item availability
  $form['availability']['millennium_availability_vid'] = array(
    '#type' => 'select',
    '#title' => t('Map item location into'),
    '#options' => $v_options,
    '#default_value' => variable_get('millennium_availability_vid', -1),
  );
  $form['availability']['millennium_map_available_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add location terms only when an item is available at that location'),
    '#description' => t('Enabling checkbox will only add the location terms to the node then items are available at the time of import. Recommended setting: off.'),
    '#default_value' => variable_get('millennium_map_available_only', FALSE),
  );
  return system_settings_form($form);
}

/**
 * Callback for drupal_get_form that shows admin options for manually importing items.
 */
function millennium_admin_queue($form, &$form_state) {
  drupal_add_js(drupal_get_path("module", "millennium") . "/millennium_admin.js");

  $tot_imported_nodes = db_query("SELECT count(*) FROM {millennium_node_bib}")->fetchField();
  $tot_queued_items = db_query("SELECT count(*) FROM {millennium_import_queue}")->fetchField();
  $form['source'] = array(
    '#type' => 'radios',
    '#title' => t("Source of records to process"),
    '#options' => array(
      'list' => t('Record number listing'),
      'range' => t('Record number range'),
      'query' => t('Enter a keyword query'),
      'url' => t('Enter a URL for a result listing (only works for certain record display types)'),
      'existing' => t('All already-imported records (@num items)', array("@num" => $tot_imported_nodes)),
      'test' => t('Run predefined tests against some known OPACs'),
    ),
    '#default_value' => variable_get('millennium_import_form_source', 'list'),
  );
  if ($tot_queued_items > 0) {
    $form['source']['#options']['queued'] = t('All records awaiting cron (@num items). <em>Note: this will empty the cron queue!</em>', array("@num" => $tot_queued_items));
  }
  $form['millennium_import_form_baseurl'] = array(
    '#prefix' => '<div id="baseurl-table">',
    '#suffix' => '</div>',
    '#type' => 'select',
    '#title' => t('Source WebOPAC'),
    '#description' => t('Select one of the enabled sources.'),
    '#default_value' => variable_get('millennium_import_form_baseurl', ''),
    '#options' => drupal_map_assoc(array_merge(array(0 => t('<none>')), array_keys(variable_get('millennium_sources', array())))),
  );
  $form['list'] = array(
    '#type' => 'fieldset',
    '#title' => t('Record number listing'),
    '#attributes' => array('id' => 'list-fieldset'),
  );
  $form['list']['ignore_checkdigit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore check digit'),
    '#description' => t("Use when exported record numbers Millennium contain check digits or 'X' at the end. This is normally the case when exporting lists from Millennium."),
    '#default_value' => variable_get('millennium_list_form_checkdigit', FALSE),
  );
  $form['list']['queue'] = array(
    '#type' => 'textarea',
    '#title' => t('Record numbers to queue for import (one per line)'),
    '#description' => t('Record numbers are written like this: b123456, i123456. You can get bibliographic or item record numbers from Millennium Client\'s "Create Lists" option.'),
    '#default_value' => '',
  );
  $form['range'] = array(
    '#type' => 'fieldset',
    '#title' => t('Record number range'),
    '#attributes' => array('id' => 'range-fieldset'),
  );
  $form['range']["type"] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('millennium_range_form_type', 'b'),
    '#options' => array('i' => t('Item records'), 'b' => t('Bibliographic records')),
    '#title' => t('Record type'),
  );
  $form['range']["start"] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('millennium_range_form_start', '100000'),
    '#size' => 7,
    '#title' => t('Starting number'),
  );
  $form['range']["end"] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('millennium_range_form_end', '100010'),
    '#size' => 7,
    '#title' => t('Ending number'),
  );
  $form['url'] = array(
    '#type' => 'fieldset',
    '#title' => t('URL for a result listing'),
    '#attributes' => array('id' => 'url-fieldset'),
  );
  $form['url']['url'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('millennium_import_form_url', ''),
    '#size' => 80,
    '#maxlength' => 2047,
    '#title' => t('URL for listing'),
    '#description' => t('Copy a URL for a result listing from a WebOPAC. NOTE: This will only work when the links in the lists lead to a single record (like keyword searches and featured lists)'),
  );
  $form['url']['num_items2'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('millennium_import_form_num_items', 10),
    '#size' => 5,
    '#title' => t('Maximum number of items to import'),
    '#description' => t('Enter the number of items to import.'),
  );
  $form['query'] = array(
    '#type' => 'fieldset',
    '#title' => t('Keyword query'),
    '#attributes' => array('id' => 'query-fieldset'),
  );
  $form['query']['kw'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('millennium_import_form_kw', ''),
    '#size' => 80,
    '#maxlength' => 255,
    '#title' => t('Keywords to search for'),
    '#description' => t('Type in keywords as you would type them in the WebOPAC keyword search. Some lesser-known available fields are: mattype:mattype[letter] branch:branch[locationcode]'),
  );
  $form['query']['num_items'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('millennium_import_form_num_items', 10),
    '#size' => 5,
    '#title' => t('Maximum number of items to import'),
    '#description' => t('Enter the number of items to import.'),
  );
  $form['force_update'] = array(
    '#type' => 'radios',
    '#title' => t('Import action'),
    '#options' => array(
      1 => t("Create new items and update existing with current WebOPAC data"),
      0 => t("Create new items only, don't update existing"),
      2 => t("Do not create items, only update existing using local MARC data (will only update MARC->taxonomy and other mappings)"),
      3 => t("Do not create nor update (useful for testing crawler)"),
    ),
    '#default_value' => variable_get('millennium_import_form_force_update', 1),
    '#prefix' => '<div id="import-options">',
    '#suffix' => '</div>',
  );
  $form['priority'] = array(
    /*
    '#type' => 'radios',
    '#title' => t('Cron priority for these items'),
    '#options' => array(0 => t("Low"), 1 => t("High")),
    '#default_value' => 0,
    '#description' => t('All high-priority items will be processed before any of the low-priority items.'),
    */
    '#type' => 'hidden',
    '#value' => 0,
  );
  $form['submit']['using_batch'] = array(
    '#type' => 'submit',
    '#value' => t('Process these records now'),
  );
  $form['submit']['using_cron'] = array(
    '#type' => 'submit',
    '#value' => t('Process these records during cron runs'),
  );
  return $form;
}

/**
 * Validate function for millennium_admin_queue() form
 */
function millennium_admin_queue_validate($form, &$form_state) {
  drupal_add_js(drupal_get_path("module", "millennium") . "/millennium_admin.js");
  if ($form_state["values"]["source"] == "range" ||
    $form_state["values"]["source"] == "list" ||
    $form_state["values"]["source"] == "query") {
    $base_url = millennium_get_real_baseurl($form_state['values']['millennium_import_form_baseurl']);
    if (!$base_url) {
      form_set_error('millennium_import_form_baseurl', t("Not a valid base URL."));
    }
  }
  if ($form_state["values"]["source"] == "range") {
    foreach (array("start", "end") as $element) {
      if (!is_int($form_state["values"][$element] + 0)) {
        form_set_error($element, t("Must be a number"));
      }
      if ($form_state["values"][$element] + 0 < 10000) {
        form_set_error($element, t("Must be equal or higher than 10000"));
      }
    }
    if ($form_state["values"]["end"] < $form_state["values"]["start"]) {
      form_set_error("end", t("Ending number must be higher or equal than starting number."));
    }
  }

  if ($form_state["values"]["source"] == "list") {
    if (trim($form_state['values']['queue']) == "") {
      form_set_error("queue", t("You must enter at least one record number (e.g.: b123456, i123456) in the list."));
      return;
    }
    $lines = explode("\n", $form_state['values']['queue']);
    $errors = array();
    $regexp = $form_state["values"]["ignore_checkdigit"] ? MILLENNIUM_RECORD_PREG_CHECKDIGIT : MILLENNIUM_RECORD_PREG_NOCHECKDIGIT;
    foreach ($lines as $line) {
      $recnum = trim($line);
      if ($recnum) {
        if (!preg_match($regexp, $recnum)) {
          $errors[] = $recnum;
        }
      }
    }
    if ($errors) {
      form_set_error(
        "queue",
          t("These are not Millennium record numbers:")
        . theme('item_list', array('items' => $errors))
        . t('Record numbers begin with "i" or "b" and have 5 or more digits. If your record numbers end with a check digit (e.g. b123456x) then mark the "Ignore check digit" checkbox.')
        , "error"
        );
    }
  }

  if ($form_state["values"]["source"] == "query") {
    if (!$form_state['values']['kw']) {
      form_set_error('kw', t("You must enter a query."));
    }
    if (($form_state['values']['num_items'] + 0) <= 0) {
      form_set_error('num_items', t("You must enter a number greater than 0."));
    }
  }

  if ($form_state["values"]["source"] == "url") {
    if (!$form_state['values']['url']
         || !millennium_get_real_baseurl($form_state["values"]["url"])
         || !millennium_ping($form_state["values"]["url"])
      ) {
      form_set_error('url', t("You must enter a valid URL."));
    }
    if (($form_state['values']['num_items2'] + 0) <= 0) {
      form_set_error('num_items2', t("You must enter a number greater than 0."));
    }
  }
}

/**
 * Submit function for millennium_admin_queue() form
 */
function millennium_admin_queue_submit($form, &$form_state) {
  #dpm($form_state);
  #return;

  if ($form_state["submitted"] == TRUE) {

    // Store some values for later
    millennium_variable_set('millennium_import_form_source', $form_state["values"]["source"]);
    millennium_variable_set('millennium_import_form_baseurl', $form_state["values"]["millennium_import_form_baseurl"]);
    millennium_variable_set('millennium_import_form_force_update', $form_state["values"]["force_update"]);
    millennium_variable_set('millennium_list_form_checkdigit', $form_state["values"]["ignore_checkdigit"]);
    millennium_variable_set('millennium_range_form_type', $form_state["values"]["type"]);
    millennium_variable_set('millennium_range_form_start', $form_state["values"]["start"]);
    millennium_variable_set('millennium_range_form_end', $form_state["values"]["end"]);
    millennium_variable_set('millennium_import_form_kw', $form_state["values"]["kw"]);
    millennium_variable_set('millennium_import_form_url', $form_state["values"]["url"]);

    $base_url = millennium_get_real_baseurl($form_state['values']['millennium_import_form_baseurl']);

    if ($form_state["clicked_button"]["#value"] == t("Process these records during cron runs")) {

      // When? Cron runs
      if ($form_state["values"]["source"] == "list") {
        // Import from list
        $lines = explode("\n", $form_state['values']['queue']);
        $queued = 0;
        $regexp = $form_state["values"]["ignore_checkdigit"] ? MILLENNIUM_RECORD_PREG_CHECKDIGIT : MILLENNIUM_RECORD_PREG_NOCHECKDIGIT;
        foreach ($lines as $line) {
          $recnum = trim($line);
          if ($recnum == "") {
            continue;
          }
          if (!preg_match($regexp, $recnum)) {
            drupal_set_message(t("Ignoring '@recnum': not a valid Millennium record number (b123456, i123456)", array("@recnum" => $recnum)), "error");
          }
          else {
            if ($form_state["values"]["ignore_checkdigit"] == TRUE) {
              $recnum = drupal_substr($recnum, 0, drupal_strlen($recnum) - 1);
            }
            $ok = $id = db_insert('millennium_import_queue')
            ->fields(array(
              'recnum' => $recnum,
              'priority' => $form_state['values']['priority'],
              'force_update' => $form_state['values']['force_update'],
              'base_url' => $base_url,
            ))
            ->execute();
            if ($ok) {
              $queued++;
            }
          }
        }
      }

      if ($form_state["values"]["source"] == "range") {
        // Import from range
        $type = $form_state["values"]["type"];
        $start = $form_state["values"]["start"];
        $end = $form_state["values"]["end"];
        for ($num = $start; $num <= $end; $num++) {
          $ok = db_insert('millennium_import_queue')
          ->fields(array(
            'recnum' => $type . $num,
            'priority' => $form_state['values']['priority'],
            'force_update' => $form_state['values']['force_update'],
            'base_url' => $base_url,
          ))
          ->execute();
          if ($ok) {
            $queued++;
          }
        }
      }

      if ($form_state["values"]["source"] == "existing") {
        $result = db_query("SELECT bib_recnum, base_url FROM {millennium_node_bib}");
        foreach ($result as $data) {
          $ok = db_insert('millennium_import_queue')
          ->fields(array(
            'recnum' => $data->item_recnum,
            'priority' => $form_state['values']['priority'],
            'force_update' => $form_state['values']['force_update'],
            'base_url' => $data->base_url,
          ))
          ->execute();
          if ($ok) {
            $queued++;
          }
        }
      }
      drupal_set_message( t('@count items queued successfully. They will be processed during subsecuent cron runs.', array("@count" => $queued)) . ' ' . l(t('You can run cron now.'), 'admin/reports/status/run-cron', array('query' => array('destination' => 'admin/content/millennium'))));

    }
    else {
      // When? Right now!
      $records_to_crawl = array();

      if ($form_state["values"]["source"] == "test") {
        millennium_mass_fetch_test();
        $form_state['#redirect'] = MILLENNIUM_SETTINGS_PATH . "/batch_import";
        return;
      }

      if ($form_state["values"]["source"] == "list") {
        // Import from list
        $lines = explode("\n", $form_state['values']['queue']);
        $queued = 0;
        foreach ($lines as $line) {
          $recnum = trim($line);
          if ($recnum) {
            if ($form_state["values"]["ignore_checkdigit"] == TRUE) {
              $recnum = drupal_substr($recnum, 0, drupal_strlen($recnum) - 1);
            }
            $records_to_crawl[$base_url][$recnum] = $recnum;
          }
        }
      }

      if ($form_state["values"]["source"] == "range") {
        // Import from range
        $type = $form_state["values"]["type"];
        $start = $form_state["values"]["start"];
        $end = $form_state["values"]["end"];
        for ($num = $start; $num <= $end; $num++) {
          $records_to_crawl[$base_url]["{$type}{$num}"] = "{$type}{$num}";
        }
      }

      if ($form_state["values"]["source"] == "query") {
        module_load_include('inc', 'millennium', 'millennium.import');
        millennium_variable_set('millennium_import_form_num_items', $form_state["values"]["num_items"]);
        $records = millennium_query_scrape(
          $form_state["values"]["kw"],
          $base_url,
          $form_state["values"]["num_items"]
        );
        foreach ($records as $recnum) {
          $records_to_crawl[$base_url][$recnum] = $recnum;
        }
      }

      if ($form_state["values"]["source"] == "url") {
        module_load_include('inc', 'millennium', 'millennium.import');
        millennium_variable_set('millennium_import_form_num_items', $form_state["values"]["num_items2"]);
        $records = millennium_result_list_scrape(
          $form_state["values"]["url"],
          $form_state["values"]["num_items2"]
        );
        $base_url = millennium_get_real_baseurl($form_state["values"]["url"]);
        foreach ($records as $recnum) {
          $records_to_crawl[$base_url][$recnum] = $recnum;
        }
      }

      if ($form_state["values"]["source"] == "existing") {
        $result = db_query("SELECT bib_recnum, base_url FROM {millennium_node_bib} ORDER BY base_url");
        foreach ($result as $data) {
          $records_to_crawl[$data->base_url][$data->bib_recnum] = $data->bib_recnum;
        }
      }

      if ($form_state["values"]["source"] == "queued") {
        $result = db_query("SELECT recnum, base_url FROM {millennium_import_queue} ORDER BY base_url");
        foreach ($result as $data) {
          $records_to_crawl[$data->base_url][$data->recnum] = $data->recnum;
        }
        db_delete('millennium_import_queue')->execute();
      }

      if (sizeof($records_to_crawl) > 0) {
        millennium_batch_import($records_to_crawl, $form_state['values']['force_update']);
        $form_state['#redirect'] = MILLENNIUM_SETTINGS_PATH . "/batch_import";
      }
    }
  }

}

/**
 * Batch API implementation.
 *
 * @param $records_to_crawl
 *   A keyed array of arrays where the key is the base_url of the OPAC to crawl,
 *   and each element is an array of bib or item records to get from the OPAC.
 */
function millennium_batch_import($records_to_crawl, $force_update = TRUE) {
  $session_id = sprintf("%.0f", microtime(TRUE) * 100);
  $operations = array();

  #dpm($records_to_crawl);
  foreach ($records_to_crawl as $base_url => $recnums) {
    // Chunk records into groups
    $chunks = array_chunk($recnums, 1000);
    foreach ($chunks as $chunk) {
      if ($force_update == 0 || $force_update == 1 || $force_update == 3) {
        // Do crawl
        $operations[] = array('millennium_batch_import_crawl_operation', array($session_id, $chunk, $base_url));
      }
      if ($force_update == 2) {
        // Should skip crawl if refreshing mappings from local MARC
        // Load into database for node processing
        $operations[] = array('millennium_batch_import_refresh_operation', array($session_id, $chunk, $base_url));
      }

      if ($force_update != 3) {
        // Operation to convert data from database created in previous operations.
        $operations[] = array('millennium_batch_import_node_operation', array($session_id, $force_update, $base_url));
      }
      else {
        // Empty the cache table, as it was not used
        $operations[] = array('millennium_batch_import_cleanup_operation', array($session_id));
      }
    }
  }

  // Let other modules alter the operations.
  // NOTE: You can only add functions defined in .module files.
  drupal_alter("millennium_batch_import", $operations, $session_id, $force_update);

  // Define the batch
  $batch = array(
    'operations' => $operations,
    'finished' => 'millennium_batch_import_finished',
    'title' => t('Millennium module is processing items...'),
    'init_message' => t('Batch processing is starting.'),
    'file' => drupal_get_path('module', 'millennium') . '/millennium.admin.inc',
    //'progress_message' => t('Reindexed @current out of @total.'),
    'error_message' => t('Batch importing has encountered an error.'),
  );
  batch_set($batch);
}

/**
 * Batch Operation Callback
 */
function millennium_batch_import_crawl_operation($session_id, $item_recnums, $base_url, &$context) {
  module_load_include('inc', 'millennium', 'millennium.import');

  // We can safely process this limit without a timeout.
  $limit = variable_get('millennium_webopac_maxrecords', 50);
  $chunks = array_chunk($item_recnums, $limit);

  if (!isset($context['sandbox']['progress'])) {
    if (empty($context['results']['operation_id'])) {
      $context['results']['operation_id'] = 0;
      $context['results']['tot_elapsed'] = 0;
      $context['results']['elapsed_node'] = 0;
    }
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_chunk'] = 0;
    $context['sandbox']['tot_items'] = sizeof($item_recnums);
    $context['sandbox']['tot_attempted'] = 0;
    $context['sandbox']['tot_notfound'] = 0;
    $context['sandbox']['tot_found'] = 0;
    $context['sandbox']['tot_fail'] = 0;
    $context['sandbox']['elapsed'] = 0;
    $context['sandbox']['max'] = sizeof($chunks);
  }

  // Here we actually perform our processing on the current chunk.
  $chunk = $chunks[$context['sandbox']['current_chunk']];
  $context['sandbox']['tot_attempted'] += sizeof($chunk);
  $fetched = millennium_mass_fetch($chunk, $base_url);
  #file_put_contents('c:\temp\final_results.txt', var_export($fetched, TRUE)); // TODO debug remove
  $context['sandbox']['tot_notfound'] += sizeof($fetched['not_found']);
  $context['sandbox']['tot_found'] += sizeof($fetched['found']);

  // Store records for next step in batch
  if (sizeof($fetched['found']) > 0) {
    foreach ($fetched['found'] as $data) {
      $id = db_insert('millennium_import_cache')
      ->fields(array(
        'data' => serialize($data),
        'session' => $session_id,
      ))
      ->execute();
    }
  }

  // Update our progress information.
  $context['sandbox']['progress']++;
  $context['sandbox']['current_chunk']++;
  $context['sandbox']['elapsed'] += $fetched['elapsed'];
  $context['sandbox']['crawlspeed'] = sprintf("%2.1f", $context['sandbox']['tot_attempted'] / $context['sandbox']['elapsed']);
  $progress_msg = t("Crawl @baseurl: @crawled crawled, @pending pending (@found found, @notfound not found) @crawlspeed items per second",
    array(
      "@baseurl" => $base_url,
      "@crawled" => $context['sandbox']['tot_attempted'],
      "@pending" => $context['sandbox']['tot_items'] - $context['sandbox']['tot_attempted'],
      "@found" => $context['sandbox']['tot_found'],
      "@notfound" => $context['sandbox']['tot_notfound'],
      "@crawlspeed" => $context['sandbox']['crawlspeed']
      )
  );
  // Build message to show
  $context['results']['messages']["crawl_{$context['results']['operation_id']}_{$base_url}"] = $progress_msg;
  $context['message'] = theme('item_list', array('items' => $context['results']['messages']));

  // Update results data for millennium_batch_import_finished()
  foreach (array('elapsed', 'tot_items', 'tot_found', 'tot_attempted', 'tot_notfound', 'crawlspeed') as $index) {
    $context['results'][$index] = $context['sandbox'][$index];
  }
  $context['results']['tot_elapsed'] += $fetched['elapsed'];

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    // Increment the operation_id
    $context['results']['operation_id']++;
  }
}

/**
 * Simple batch operation that just loads data into the cache table for later processing.
 */
function millennium_batch_import_refresh_operation($session_id, $bib_recnums, $base_url, &$context) {
  #dpm($item_recnums);
  #drupal_set_message("SELECT bib_recnum, biblio_data FROM {millennium_node_bib} WHERE bib_recnum IN (".
  #  db_placeholders($item_recnums, 'char')  . ") AND base_url = '%s'");
  #return;
  #drupal_set_message("base_url = $base_url, session_id = $session_id, biblio_data_query = $biblio_data_query");
  $handle = db_query("SELECT bib_recnum, biblio_data FROM {millennium_node_bib} 
    WHERE bib_recnum IN (:bib_recnums) AND base_url = :base_url", array(':bib_recnums' => $bib_recnums, ':base_url' => $base_url));

  foreach ($handle as $rec) {
    $biblio_data = unserialize($rec->biblio_data);
    $data = array(
      'bib_recnum' => $rec->bib_recnum,
      'marc' => $biblio_data['marc'],
      'base_url' => $base_url,
    );
    $id = db_insert('millennium_import_cache')
    ->fields(array(
      'data' => serialize($data),
      'session' => $session_id,
    ))
    ->execute();
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function millennium_batch_import_node_operation($session_id, $force_update, $base_url, &$context) {
  #dpm($context);
  timer_start("millennium_batch_import_node_operation");
  if (!isset($context['sandbox']['progress'])) {
    if (empty($context['results']['operation_id'])) {
      $context['results']['operation_id'] = 0;
      $context['results']['tot_elapsed'] = 0;
      $context['results']['elapsed_node'] = 0;
      // These are just to keep the time reporting happy.
      $context['results']['tot_attempted'] = 0;
      $context['results']['tot_notfound'] = 0;
      $context['results']['tot_found'] = 0;
    }
    $context['results']['session_id'] = $session_id;
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['tot_imported'] = 0;
    $context['sandbox']['tot_fail'] = 0;
    $context['sandbox']['max'] = db_query("SELECT COUNT(id) FROM {millennium_import_cache} WHERE session = :session", array(':session' => $session_id))->fetchField();
    $context['sandbox']['current_id'] = 0;
    $context['sandbox']['status']['bib_created'] = 0;
    $context['sandbox']['status']['bib_updated'] = 0;
    $context['sandbox']['status']['bib_untouched'] = 0;
    $context['sandbox']['elapsed_node'] = 0;
  }
  #drupal_set_message(sprintf("SELECT COUNT(id) FROM {millennium_import_cache} WHERE session = '%s'", $session_id));
  #dpm($context);
  #return;

  // Get the next N items to process from the database
  $limit = variable_get('millennium_webopac_maxrecords', 50);
  $query = db_query_range("SELECT * FROM {millennium_import_cache} WHERE id > :id AND session = :session ORDER BY id ASC", 0, $limit, array(':id' => $context['sandbox']['current_id'], ':session' => $session_id));
  foreach ($query as $row) {
    $data = unserialize($row->data);
    if (!$data['bib_recnum'] && !$data['item_recnum']) {
      watchdog("Millennium", "Batch import error: no record number given in row: @data", array('@data' => var_export($row, TRUE)));
      $context['sandbox']['tot_fail']++;
    }
    else {
      $result = millennium_process_record($data, $force_update);
      if ($result["success"] !== FALSE) {
        $context['sandbox']['tot_imported']++;
        $context['sandbox']['status'][$result['status']]++;
      }
      else {
        $context['sandbox']['tot_fail']++;
      }
    }

    // Delete just-processed item
    db_delete('millennium_import_cache')
    ->condition('id', $row->id)
    ->execute();

    $context['sandbox']['progress']++;
    $context['sandbox']['current_id'] = $row->id;
  }
  $timer = timer_stop("millennium_batch_import_node_operation");
  $elapsed = $timer['time'] / 1000;
  $context['sandbox']['elapsed_node'] += $elapsed;
  $context['sandbox']['importspeed'] = sprintf("%2.1f", $context['sandbox']['progress'] / $context['sandbox']['elapsed_node']);

  $progress_msg = t("Node import @id: @imported processed, @pending pending (@created created, @updated updated, @untouched untouched, @failed errors) @importspeed items per second",
    array(
      "@id" => $base_url,
      "@imported" => $context['sandbox']['tot_imported'],
      "@failed" => $context['sandbox']['tot_fail'],
      "@pending" => $context['sandbox']['max'] - $context['sandbox']['progress'],
      "@created" => $context['sandbox']['status']['bib_created'],
      "@updated" => $context['sandbox']['status']['bib_updated'],
      "@untouched" => $context['sandbox']['status']['bib_untouched'],
      "@importspeed" => $context['sandbox']['importspeed']
      )
  );
  // Build message to show
  $context['results']['messages']["nodeimport_{$context['results']['operation_id']}_{$base_url}"] = $progress_msg;
  $context['message'] = theme('item_list', array('items' => $context['results']['messages']));

  // Update results data for millennium_batch_import_finished()
  foreach (array('tot_imported', 'tot_fail', 'importspeed') as $index) {
    $context['results'][$index] = $context['sandbox'][$index];
  }
  $context['results']['tot_elapsed'] += $elapsed;
  $context['results']['elapsed_node'] += $elapsed;

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    // Increment the operation_id
    $context['results']['operation_id']++;
  }
}

/**
 * Simple batch operation that empties the cache table.
 */
function millennium_batch_import_cleanup_operation($session_id, &$context) {
  db_delete('millennium_import_cache')
  ->condition('session', $session_id)
  ->execute();
  $context['results']['messages']['cleanup'] = t('Import cache table clean up complete.');
}

/**
 * Batch 'finished' callback
 */
function millennium_batch_import_finished($success, $results, $operations) {
  $message_type = '';
  if ($success) {
    $results['messages'][] = t("Total time elapsed: @elapsed seconds", array('@elapsed' => sprintf("%2.1f", $results['tot_elapsed'])));
    $message = theme('item_list', array('items' => $results['messages']));
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing @num with arguments @args', array('@num' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE)));
    $message_type = 'error';
  }
  millennium_time_history($results['elapsed_node'], $results['tot_elapsed'], $results['tot_attempted'], $results['tot_notfound'], $results['tot_found']);
  drupal_set_message($message, $message_type);
}

/**
 * Run tests on different WebOPACs available on the web.
 */
function millennium_mass_fetch_test($num_records = 100) {
  // Some test cases
  // array('http://millenium.itesm.mx', 'b', 1580000), // Many not-found records should be here

  // Array of base urls, record type (i or b), and starting record number.
  $test_subjects = array(
    // Item record tests
    #array('http://catnyp.nypl.org', 'i', 17820000),
    #array('http://www.consuls.org', 'i', 1580000),
    #array('http://irma.aadl.org', 'i', 1500000),
    #array('http://elibrary.mel.org', 'i', 13500000),
    #array('http://millenium.itesm.mx', 'i', 100000),
    #array('http://library.dcccd.edu', 'i', 1100000),
    #array('http://catalogue.gvpl.ca' , 'i', 1100000),
    #array('http://eleanor.lib.gla.ac.uk', 'i', 2661000),
    #array('http://library.hku.hk', 'i', 1100000),

    // Bib record tests
    array('http://millennium.itesm.mx', 'b', 1179000),
    array('http://library.dcccd.edu', 'b', 1100000),
    #array('http://irma.aadl.org', 'b', 1321000),
    #array('http://catalog.nypl.org', 'b', 1100000),
    array('http://www.consuls.org', 'b', 1580000),
    array('http://www.helmet.fi', 'b', 1612818),
    array('http://eleanor.lib.gla.ac.uk', 'b', 2661000),
    #array('http://elibrary.mel.org', 'b', 10340000), // SLOW!
    #array('http://catalogue.gvpl.ca' , 'b', 1100000), // SLOW!
    #array('http://library.hku.hk', 'b', 1100000), // SLOW
  );
  $session_id = sprintf("%.0f", microtime(TRUE) * 100);
  $operations = array();
  foreach ($test_subjects as $subject) {
    // Create array of records
    $base_url = millennium_get_real_baseurl($subject[0]);
    $recnums = array();
    for ($i = 0; $i < $num_records; $i++) {
      $recnums[] = $subject[1] . ($subject[2] + $i);
    }
    $operations[] = array('millennium_batch_import_crawl_operation', array($session_id, $recnums, $base_url));
  }
  $operations[] = array('millennium_batch_import_cleanup_operation', array($session_id));

  // Define the batch
  $batch = array(
    'operations' => $operations,
    'finished' => 'millennium_mass_fetch_test_finished',
    'title' => t('Millennium module is processing items...'),
    'init_message' => t('Batch processing is starting.'),
    'file' => drupal_get_path('module', 'millennium') . '/millennium.admin.inc',
    //'progress_message' => t('Reindexed @current out of @total.'),
    'error_message' => t('Batch importing has encountered an error.'),
  );
  // Run the batch
  batch_set($batch);

  /*

  foreach ($test_subjects as $subject) {
    // Create array of records
    $recnums = array();
    $parsed_url = parse_url($subject[0]);
    $base_url = millennium_get_real_baseurl($subject[0]);

    $ok = millennium_ping($base_url);
    if (!$ok) {
      $dataa[] = array($base_url, "Not responding");
      continue;
    }

    for ($i = 0; $i < $num_records; $i++) {
      $recnums[] = $subject[1] . ($subject[2] + $i);
    }

    // Run test for a single URL
    timer_start("millennium_mass_fetch_test");
    $fetched = millennium_mass_fetch($recnums, $base_url);

    // Report
    $elapsed = sprintf("%2.2f", timer_read("millennium_mass_fetch_test") / 1000);
    $recs_per_sec = sprintf("%2.2f", $num_records / (timer_read("millennium_mass_fetch_test")/1000));
    $pct_found = sprintf("%2.1f", (sizeof($fetched["found"]) / $num_records) * 100);
    $report_rows[] = array($base_url, $num_records, $pct_found, $elapsed, $recs_per_sec);
    #drupal_set_message("{$base_url} : {$num_records} recs ({$pct_found}% found) in {$elapsed} seconds ({$recs_per_sec} rec/sec)");
    #dpm($fetched);
  }

  echo "<table>";
  echo "<tr><th>URL<th>Attempted<th>Pct found<th>Elapsed<th>Recs/sec\n";
  foreach ($report_rows as $row) {
    echo "<tr><td>". implode("<td>", $row) . "\n";
  }
  echo "</table>";
  */
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function millennium_mass_fetch_test_finished($success, $results, $operations) {
  $message_type = 'status';
  if ($success) {
    $results['messages'][] = t("Total time elapsed: @elapsed seconds", array('@elapsed' => sprintf("%2.1f", $results['tot_elapsed'])));
    $message = theme('item_list', array('items' => $results['messages']));
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing @num with arguments @args', array('@num' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE)));
    $message_type = 'error';
  }
  drupal_set_message($message, $message_type);
}
