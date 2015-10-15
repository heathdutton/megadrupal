<?php

/**
 * @file
 * Settings admin file.
 *
 * Has two tabs for creating TARDIS blocks and pages.
 */

/**
 * Main page for the TARDIS admin section.
 *
 * Displays an overview of available TARDIS blocks and pages.
 */
function tardis_overview($form, &$form_state) {
  // Here we build a table of available TARDIS blocks.
  $tardis_element_type = 'block';

  // First we need to get all, if any, from the DB.
  $tardis_elements = db_select('tardis_blocks', 'tb')
    ->fields('tb')
    ->orderBy('name', 'ASC')
    ->execute()
    ->fetchAllAssoc('name');

  if (count($tardis_elements) > 0) {
    // Then we build the table to place them in:
    $tardis_elements_header = array(
      array('data' => t('Block name')),
      array('data' => t('Title')),
      array('data' => t('Node types')),
      array('data' => t('Link')),
    );

    // Create extra table headers when submodules are on.
    _tardis_submodules_extra_headers('block', $tardis_elements_header);

    $tardis_elements_header[] = array('data' => t('Operations'));

    foreach ($tardis_elements as $tardis_element) {
      // We build each row using data extracted from the db.
      $tardis_element_name = $tardis_element->name;
      $tardis_element_title = filter_xss($tardis_element->title);

      // Node types are stored as an array and, therefore, require some
      // processing. They're in serialized form, which must be unserialized.
      // Then, we remove the ones which are off (zero).
      $tardis_element_node_types = unserialize($tardis_element->node_types);
      foreach ($tardis_element_node_types as $key => $value) {
        if ($tardis_element_node_types[$key] === 0) {
          unset($tardis_element_node_types[$key]);
        }
      }
      $tardis_element_node_types_list = array(
        '#theme' => 'item_list',
        '#type' => 'ul',
        '#items' => $tardis_element_node_types,
      );

      $tardis_element_block_link = $tardis_element->block_link;

      $tardis_elements_row = array(
        array('data' => $tardis_element_name),
        array('data' => $tardis_element_title),
        array('data' => $tardis_element_node_types_list),
        array('data' => $tardis_element_block_link),
      );

      // Add extra table info when submodules are on.
      _tardis_submodules_extra_info('block', $tardis_elements_rows, $tardis_elements_row, $tardis_element);

    }
  }
  else {
    // If none exist, we'll display a message telling users about it.
    $tardis_elements_header = array(
      array('data' => t('TARDIS blocks')),
    );

    $tardis_elements_rows[] = array(
      array('data' => t("You haven't created a TARDIS block yet.")),
    );
  }

  $form['tardis_blocks_table'] = array(
    '#title' => t('TARDIS blocks'),
    '#markup' => theme('table', array('header' => $tardis_elements_header, 'rows' => $tardis_elements_rows)),
  );

  unset($tardis_elements_row);
  unset($tardis_elements_rows);

  // Here we build a table of available TARDIS pages.
  $tardis_element_type = 'page';

  // First we need to get all, if any, from the DB.
  $tardis_elements = db_select('tardis_pages', 'tp')
    ->fields('tp')
    ->orderBy('name', 'ASC')
    ->execute()
    ->fetchAllAssoc('name');

  if (count($tardis_elements) > 0) {
    // Then we build the table to place them in:
    $tardis_elements_header = array(
      array('data' => t('Page name')),
      array('data' => t('Title')),
      array('data' => t('Node types')),
      array('data' => t('Address')),
    );

    // If TARDIS Token is enabled, the "Title" table header
    // needs to change slightly.
    (module_exists('tardistoken')) ? $tardis_elements_header[1]['data'] = t('Title(s)') : '';

    _tardis_submodules_extra_headers('page', $tardis_elements_header);

    // Create extra table headers when submodules are on.
    $tardis_elements_header[] = array('data' => t('Operations'));

    foreach ($tardis_elements as $tardis_element) {
      // We build each row using data extracted from the db.
      $tardis_element_name = $tardis_element->name;

      // If TARDIS Token is enabled, the "Title" cell needs to change slightly.
      if (module_exists('tardistoken')) {
        $tardis_element_title = array(
          '#theme' => 'item_list',
          '#type' => 'ul',
          '#items' => array(
            '<strong>Regular title: </strong><br />' . filter_xss($tardis_element->title),
            '<strong>Title with year: </strong><br />' . filter_xss($tardis_element->tardistoken_year),
            '<strong>Title with month and year: </strong><br />' . filter_xss($tardis_element->tardistoken_year_month),
          ),
        );
      }
      else {
        $tardis_element_title = filter_xss($tardis_element->title);
      }

      // Node types are stored as an array and, therefore, require some
      // processing. They're in serialized form, which must be unserialized.
      // Then, we remove the ones which are off (zero).
      $tardis_element_node_types = unserialize($tardis_element->node_types);
      foreach ($tardis_element_node_types as $key => $value) {
        if ($tardis_element_node_types[$key] === 0) {
          unset($tardis_element_node_types[$key]);
        }
      }
      $tardis_element_node_types_list = array(
        '#theme' => 'item_list',
        '#type' => 'ul',
        '#items' => $tardis_element_node_types,
      );

      $tardis_element_address = $tardis_element->address;

      $tardis_elements_row = array(
        array('data' => $tardis_element_name),
        array('data' => $tardis_element_title),
        array('data' => $tardis_element_node_types_list),
        array('data' => $tardis_element_address),
      );

      // Add extra table info when submodules are on.
      _tardis_submodules_extra_info('page', $tardis_elements_rows, $tardis_elements_row, $tardis_element);

    }
  }
  else {
    // If none exist, we'll display a message telling users about it.
    $tardis_elements_header = array(
      array('data' => t('TARDIS pages')),
    );

    $tardis_elements_rows[] = array(
      array('data' => t("You haven't created a TARDIS page yet.")),
    );
  }

  $form['tardis_pages_table'] = array(
    '#title' => t('TARDIS Pages'),
    '#markup' => theme('table', array('header' => $tardis_elements_header, 'rows' => $tardis_elements_rows)),
  );

  return $form;
}

/**
 * System settings form for TARDIS blocks of recent months.
 *
 * Works for both creating and uptating.
 */
function tardis_block_settings($form, &$form_state) {
  // Due to a limitation in the way the menu system works,
  // we must change the title manually here.
  drupal_set_title('Create TARDIS block');

  // Are we creating a new block, or updating an existing one?
  $tardis_block_name = arg(5);

  // Check if the provided name is valid.
  if (!is_null($tardis_block_name)) {
    $query = db_select('tardis_blocks', 'tb')
      ->fields('tb')
      ->condition('name', $tardis_block_name)
      ->execute()
      ->fetchAssoc();
  }

  if (isset($query) && count($query) > 1) {
    // If we're editing an existing block, and users change its name,
    // that would create a new block. We must prevent this.
    $tardis_update = $tardis_block_name;

    // Extra precaution against XSS.
    $tardis_block_title = filter_xss($query['title']);
    drupal_set_title(t('Edit block <em>@name</em>', array('@name' => $tardis_block_name)), PASS_THROUGH);

    // Node types are stored as an array and must be unserialized.
    $tardis_block_node_types = unserialize($query['node_types']);

    // All other variables are pretty straightforward.
    $tardis_block_current_month = $query['current_month'];
    $tardis_block_past_months = $query['past_months'];
    $tardis_block_stop_year = $query['stop_year'];
    $tardis_block_link_nesting_months_years = $query['link_nesting_months_years'];
    $tardis_block_year_link = $query['year_link'];
    $tardis_block_month_link = $query['month_link'];
    $tardis_block_block_link = $query['block_link'];

    // Retrieve TARDIS submodules' values.
    _tardis_submodules_values('block', $query, $tardisdate_field, $tardisi18n_lang, $tardisrender_view_mode, $tardistoken_titles);
  }
  else {
    // If we're creating a new block, then we must
    // assign default values to fields.
    $tardis_update = 0;

    $tardis_block_name = 'tardis_block';
    $tardis_block_title = 'Recent nodes';
    $tardis_block_node_types = array('page');
    $tardis_block_current_month = 0;
    $tardis_block_past_months = 12;
    $tardis_block_stop_year = 2010;
    $tardis_block_link_nesting_months_years = TRUE;
    $tardis_block_year_link = 'normal';
    $tardis_block_month_link = 'names';
    $tardis_block_block_link = '';

    // Set default values for TARDIS submodules.
    _tardis_submodules_default_values('block', $tardisdate_field, $tardisi18n_lang, $tardisrender_view_mode, $tardistoken_titles);
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('TARDIS block administrative name'),
    '#description' => t('Give your TARDIS block a unique name. Use only lowercase letters and numbers.'),
    '#default_value' => $tardis_block_name,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('TARDIS block title'),
    '#description' => t('Give your TARDIS block a title'),
    '#default_value' => $tardis_block_title,
  );

  // Get all available node types and build an associative array
  // with the format machine_name => 'human-readable name'
  $node_types = node_type_get_types();
  foreach ($node_types as $node_type) {
    $options[$node_type->type] = $node_type->name;
  }
  // Now ask site admins which nodes they want displayed
  // on the TARDIS block.
  $form['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#description' => t('Which node types would you like displayed?'),
    '#options' => $options,
    '#default_value' => $tardis_block_node_types,
  );

  // Should the TARDIS include the current month?
  $form['current_month'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the current month'),
    '#description' => t('Check this box to have the TARDIS display results from the current month.'),
    '#default_value' => $tardis_block_current_month,
  );

  // How far back is the TARDIS supposed to go and fetch results?
  $form['past_months'] = array(
    '#type' => 'textfield',
    '#size' => 2,
    '#maxlength' => 2,
    '#title' => t('How far back?'),
    '#field_suffix' => t('months'),
    '#description' => t('Choose how many months you would like displayed on the TARDIS block.'),
    '#default_value' => $tardis_block_past_months,
  );

  // Define a certain point beyond which the TARDIS shouldn't look.
  $form['stop_year'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#maxlength' => 4,
    '#title' => t('Stop looking at the year'),
    '#description' => t("For increased performance, define a year (in the format YYYY) beyond which the TARDIS should stop looking.
    <br />If you don't, the TARDIS will keep looking until it reaches year zero, which means going over the db a LOT of times."),
    '#default_value' => $tardis_block_stop_year,
  );

  // Option to nest months within years.
  $form['link_nesting_months_years'] = array(
    '#type' => 'checkbox',
    '#title' => t('Month link nesting'),
    '#description' => t("Should months be nested under years?"),
    '#default_value' => $tardis_block_link_nesting_months_years,
  );

  // Lets users choose how year links will be styled.
  $form['year_link'] = array(
    '#type' => 'radios',
    '#title' => t('Year link style'),
    '#description' => t("How should TARDIS render years?
      <ul>
        <li><em>Accordion: </em> year links will render as collapsible blocks with nested months.</li>
        <li><em>Normal link: </em> year links will link to the TARDIS page, just like month links do.</li>
        <li><em>No link: </em> year links will be displayed, but without links.</li>
      </ul>
    "),
    '#options' => array(
      'accordion' => t('Accordion'),
      'normal' => t('Normal link'),
      'no_link' => t('No link'),
    ),
    '#default_value' => $tardis_block_year_link,
    '#states' => array(
      'visible' => array(
        ':input[name="link_nesting_months_years"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Lets users choose how month links will be styled.
  $form['month_link'] = array(
    '#type' => 'radios',
    '#title' => t('Month link style'),
    '#description' => t('Choose whether you want months displayed as numbers or names.'),
    '#options' => array(
      'names' => t('names'),
      'numbers' => t('numbers'),
    ),
    '#default_value' => $tardis_block_month_link,
  );

  // Allows users to set the TARDIS link.
  $form['block_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Block link'),
    '#description' => t("Choose an address for the TARDIS links. You can link to a view. <br />
    No beginning or trailing slashes, please.<br />
    <em>Note:</em> If you link to a custom view, it must take arguments in the form YYYY/MM just like TARDIS pages do."),
    '#default_value' => $tardis_block_block_link,
  );

  // Tell the module whether it's an update or not.
  $form['tardis_update'] = array(
    '#type' => 'hidden',
    '#value' => $tardis_update,
  );

  // The save function works for both TARDIS blocks and pages,
  // so we set the difference here.
  $form['table'] = array(
    '#type' => 'hidden',
    '#value' => 'tardis_blocks',
  );

  // Pass update information on.
  _tardis_submodules_update_info('block', $form, $tardisdate_field, $tardisi18n_lang, '', $tardistoken_titles);

  // Validate and submit functions.
  $form['#validate'][] = 'tardis_block_settings_validate';
  $form['#submit'][] = 'tardis_settings_submit';

  // Here's a group to place the TARDIS submodules.
  // This group will be rendered as a vertical tab group.
  $form['tardis_submodules'] = array(
    '#type' => 'vertical_tabs',
  );

  return (system_settings_form($form));
}

/**
 * Validates input for the TARDIS block settings.
 */
function tardis_block_settings_validate($form, &$form_state) {
  // Define the validation strings.
  $tardis_block_name = $form_state['values']['name'];
  $tardis_block_title = $form_state['values']['title'];
  $tardis_block_stop_year = $form_state['values']['stop_year'];
  $tardis_block_past_months = $form_state['values']['past_months'];

  // The "Name" field should contain only lowercase letters, numbers
  // and underscores, and begin with a letter.
  if (!preg_match('/^[a-z0-9_]+$/', $tardis_block_name)) {
    form_set_error('name', t('Please use only lowercase letters, numbers and underscores _ for the TARDIS block name.'));
  }

  // The "Name" field should not contain more than 64 characters.
  if (strlen($tardis_block_name) > 64) {
    form_set_error('name', t("TARDIS block name mustn't contain more than 64 characters."));
  }

  // The "Title" field should not contain more than 64 characters.
  if (strlen($tardis_block_title) > 64) {
    form_set_error('title', t("TARDIS block title mustn't contain more than 64 characters."));
  }

  // The "Stop looking at the year" field should obviously be a year.
  if (!preg_match('/^[0-9]{4}$/', $tardis_block_stop_year)) {
    form_set_error('stop_year', t('Please input a valid year with four digits.'));
  }

  // And the "How far back?" one should be a two-digit number.
  if (!preg_match('/^[0-9]{2}$/', $tardis_block_past_months) || $tardis_block_past_months <= 1) {
    form_set_error('past_months', t('Please input a valid number of months between 1 and 99.'));
  }
}

/**
 * System settings form for TARDIS pages of recent nodes.
 *
 * Works for both creating and uptating.
 */
function tardis_page_settings($form, &$form_state) {
  // Due to a limitation in the way the menu system works,
  // we must change the title manually here.
  drupal_set_title('Create TARDIS page');

  // Are we creating a new page, or updating an existing one?
  $tardis_page_name = arg(5);

  // Check if the provided name is valid.
  if (!is_null($tardis_page_name)) {
    $query = db_select('tardis_pages', 'tp')
      ->fields('tp')
      ->condition('name', $tardis_page_name)
      ->execute()
      ->fetchAssoc();
  }

  if (isset($query) && count($query) > 1) {
    // If we're editing an existing page, and users change its name,
    // that would create a new page. We must prevent this.
    $tardis_update = $tardis_page_name;

    // TARDIS page address.
    $tardis_page_address = $query['address'];

    // Extra precaution against XSS.
    $tardis_page_title = filter_xss($query['title']);
    drupal_set_title(t('Edit page <em>@name</em>', array('@name' => $tardis_page_name)), PASS_THROUGH);

    // Should TARDIS pages have dates appended to their titles?
    $tardis_page_title_date = $query['title_date'];

    // Should TARDIS pages begin with the current month or the past month?
    $tardis_page_current_month = $query['current_month'];

    // Node types are stored as an array and must be unserialized.
    $tardis_page_node_types = unserialize($query['node_types']);

    // How many nodes per TARDIS page.
    $tardis_page_nodes_per_page = $query['nodes_per_page'];

    // Retrieve TARDIS submodules' values.
    _tardis_submodules_values('page', $query, $tardisdate_field, $tardisi18n_lang, $tardisrender_view_mode, $tardistoken_titles);
  }
  else {
    // If we're creating a new page, then we must
    // assign default values to fields.
    $tardis_update = 0;

    $tardis_page_name = 'tardis_page';
    $tardis_page_address = 'recent/nodes';
    $tardis_page_title = 'Recent nodes';
    $tardis_page_title_date = 'no_date';
    $tardis_page_current_month = 0;
    $tardis_page_node_types = array('page');
    $tardis_page_nodes_per_page = 10;

    // Set default values for TARDIS submodules.
    _tardis_submodules_default_values('page', $tardisdate_field, $tardisi18n_lang, $tardisrender_view_mode, $tardistoken_titles);
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('TARDIS page administrative name'),
    '#description' => t('Give your TARDIS block a unique name. Use only lowercase letters and numbers.'),
    '#default_value' => $tardis_page_name,
  );

  // Where is the TARDIS parked? ;)
  $form['address'] = array(
    '#type' => 'textfield',
    '#title' => t('TARDIS page address'),
    '#description' => t('Choose an address for your TADIS page. <br />
      For example: <em>recent/nodes</em> leads to <em>http://yoursite.com/recent/nodes</em>. <br />
      No beginning or trailing slashes, please. <br />
      <em>Please note: </em>changing this setting requires that you clear your cache.'),
    '#default_value' => $tardis_page_address,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('TARDIS page title'),
    '#description' => t('Give your TARDIS page a title.<br />
      <em>Please note: </em>changing this setting requires that you clear your cache.'),
    '#default_value' => $tardis_page_title,
  );

  // Option to append a date to the page title.
  $form['title_date'] = array(
    '#type' => 'radios',
    '#title' => t('Append date to page title'),
    '#description' => t('Should TARDIS append the year and/or month being viewed to the page title?'),
    '#options' => array(
      'years_month_names' => t('Years and month names'),
      'years_month_numbers' => t('Years and month numbers'),
      'no_date' => t('No date on page title'),
    ),
    '#default_value' => $tardis_page_title_date,
  );

  // Should the TARDIS include the current month?
  $form['current_month'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the current month'),
    '#description' => t('Check this box to have the TARDIS display results from the current month.'),
    '#default_value' => $tardis_page_current_month,
  );

  // Get all available node types and build an associative array
  // with the format machine_name => 'human-readable name'
  $node_types = node_type_get_types();
  foreach ($node_types as $node_type) {
    $options[$node_type->type] = $node_type->name;
  }
  // Now ask site admins which nodes they want displayed
  // on the TARDIS page and block.
  $form['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#description' => t('Which node types would you like displayed?'),
    '#options' => $options,
    '#default_value' => $tardis_page_node_types,
  );

  $form['nodes_per_page'] = array(
    '#type' => 'textfield',
    '#size' => 2,
    '#maxlength' => 2,
    '#title' => t('Nodes per page'),
    '#field_suffix' => t('nodes on each page'),
    '#description' => t('Results are paged. How many nodes would you like to show on each page?'),
    '#default_value' => $tardis_page_nodes_per_page,
  );

  // Tell the module whether it's an update or not.
  $form['tardis_update'] = array(
    '#type' => 'hidden',
    '#value' => $tardis_update,
  );

  // The save function works for both TARDIS blocks and pages,
  // so we set the difference here.
  $form['table'] = array(
    '#type' => 'hidden',
    '#value' => 'tardis_pages',
  );

  // Pass update information on.
  _tardis_submodules_update_info('page', $form, $tardisdate_field, $tardisi18n_lang, $tardisrender_view_mode, $tardistoken_titles);

  // Here's a group to place the TARDIS submodules.
  // This group will be rendered as a vertical tab group.
  $form['tardis_submodules'] = array(
    '#type' => 'vertical_tabs',
  );

  // Validate and submit functions.
  $form['#validate'][] = 'tardis_page_settings_validate';
  $form['#submit'][] = 'tardis_settings_submit';

  return (system_settings_form($form));
}

/**
 * Validates input for the TARDIS page settings.
 */
function tardis_page_settings_validate($form, &$form_state) {
  // Define the validation strings.
  $tardis_page_name = $form_state['values']['name'];
  $tardis_page_address = $form_state['values']['address'];
  $tardis_page_title = $form_state['values']['title'];
  $tardis_page_nodes_per_page = $form_state['values']['nodes_per_page'];

  // The "Name" field should contain only lowercase letters, numbers
  // and underscores, and begin with a letter.
  if (!preg_match('/^[a-z0-9_]+$/', $tardis_page_name)) {
    form_set_error('name', t('Please use only lowercase letters, numbers and underscores _ for the TARDIS page name.'));
  }

  // The "Name" field should not contain more than 64 characters.
  if (strlen($tardis_page_name) > 64) {
    form_set_error('name', t("TARDIS page name mustn't contain more than 64 characters."));
  }

  // The "Title" field should not contain more than 64 characters.
  if (strlen($tardis_page_title) > 64) {
    form_set_error('title', t("TARDIS page title mustn't contain more than 64 characters."));
  }

  // Did the page address or title change? If so, we need to rebuild the menu.
  // But first, we must check which TARDIS page it is.
  $tardis_page = db_select('tardis_pages', 'tp')
    ->fields('tp', array(
      'name',
      'title',
      'address',
    ))
    ->condition('name', $tardis_page_name)
    ->execute()
    ->fetchAllAssoc('name');

  // Comparison between old and new values for page address and title.
  if (isset($tardis_page[$tardis_page_name])) {
    $tardis_page_new_title = $tardis_page[$tardis_page_name]->title;
    $tardis_page_new_address = $tardis_page[$tardis_page_name]->address;
  }
  else {
    $tardis_page_new_title = '';
    $tardis_page_new_address = '';
  }
  // If the page address or title are different, users must clear their cache.
  if ($tardis_page_address != $tardis_page_new_address || $tardis_page_title != $tardis_page_new_title) {
    drupal_set_message(t("Don't forget to <a href='/admin/config/development/performance'>clear your cache.</a>"));
  }

  // The "Nodes per page" field should be a two-digit number.
  if (!preg_match('/^[0-9]{2}$/', $tardis_page_nodes_per_page) || $tardis_page_nodes_per_page <= 1) {
    form_set_error('nodes_per_page', t('Please input a valid number of nodes per page between 1 and 99.'));
  }
}

/**
 * Submits input for the TARDIS block and page settings.
 */
function tardis_settings_submit($form, &$form_state) {
  // These are the values entered in the form.
  // But before we can save to the db, however, we must process the selected
  // node types - or else, they'll be saved as the word "Array".
  // We do that by serializing the array.
  $form_state['values']['node_types'] = serialize($form_state['values']['node_types']);

  if (module_exists('tardisi18n')) {
    // If TARDIS Internationalization is enabled,
    // serialize languages array before saving.
    $form_state['values']['tardisi18n_lang'] = serialize($form_state['values']['tardisi18n_lang']);
  }

  // We turn the values entered in the form into an object...
  $tardis_values = (object) $form_state['values'];

  // ... and we call the function that writes records to the db
  // specifying the correct table.
  $tardis_table = $form_state['values']['table'];
  $tardis_update = _tardis_save($tardis_table, $tardis_values);

  // But before we display a confirmation message, there's some XSS cleanup.
  $tardis_name = filter_xss($form_state['values']['name']);

  // Also check whether we're creating a new block/page or
  // updating an existing one.
  if ($tardis_update == 'update') {
    drupal_set_message(t('<em>@tardis_name</em> updated successfully.', array('@tardis_name' => $tardis_name)));
  }
  else {
    drupal_set_message(t('<em>@tardis_name</em> created successfully.', array('@tardis_name' => $tardis_name)));
  }

  // And back to the TARDIS admin page.
  $form_state['redirect'] = 'admin/structure/tardis';
}

/**
 * Deletion form.
 */
function tardis_delete_confirm($form, &$form_state) {
  // First off, are we deleting a block or a page?
  $tardis_table = 'tardis_' . arg(3) . 's';
  // And which one is it?
  $tardis_name = arg(5);

  // We must check if a name has been provided:
  if (!is_null($tardis_name)) {
    $query = db_select($tardis_table, 't')
      ->fields('t', array('name'))
      ->condition('name', $tardis_name)
      ->execute()
      ->fetchAssoc();
  }

  // And if a block or page with that name exists:
  if (count($query) > 0) {
    // Extra precaution against XSS.
    $tardis_name = filter_xss($query['name']);

    $form_state['table'] = $tardis_table;
    $form_state['name'] = $tardis_name;

    // The confirm_form function (part of Drupal Core) creates a
    // confirmation step before deletion.
    return confirm_form(
      $form,
      t('Are you sure you want to delete <em>@tardis_name?</em>', array('@tardis_name' => $tardis_name)),
      'admin/structure/tardis'
    );
  }
  else {
    return drupal_access_denied();
  }
}

/**
 * Deletion confirmation form.
 */
function tardis_delete_confirm_submit($form, &$form_state) {
  // These are values passed by the confirmation step:
  $tardis_table = $form_state['table'];
  $tardis_name = $form_state['name'];

  // And this is a helper function that removes records from the TARDIS tables.
  _tardis_delete($tardis_table, $tardis_name);

  // But before we display a confirmation message, there's some XSS cleanup.
  $tardis_target = filter_xss($tardis_name);
  drupal_set_message(t('<em>@tardis_name</em> deleted successfully.', array('@tardis_name' => $tardis_name)));

  // And back to the TARDIS admin page.
  drupal_goto('admin/structure/tardis');
}

/**
 * TARDIS submodules helper function.
 *
 * Creates extra table headers when submodules are on.
 */
function _tardis_submodules_extra_headers($tardis_element_type, &$tardis_elements_header) {
  // If TARDIS Token is enabled, add an extra column for TARDIS blocks.
  (module_exists('tardistoken') && $tardis_element_type == 'block') ? $tardis_elements_header[] = array('data' => t('Link titles')) : '';

  // If TARDIS Date is enabled, add an extra column.
  (module_exists('tardisdate')) ? $tardis_elements_header[] = array('data' => t('Date field')) : '';

  // If TARDIS Internationalization is enabled, add an extra column.
  (module_exists('tardisi18n')) ? $tardis_elements_header[] = array('data' => t('Languages')) : '';

  // If TARDIS Render is enabled, add an extra column for TARDIS pages.
  (module_exists('tardisrender') && $tardis_element_type == 'page') ? $tardis_elements_header[] = array('data' => t('View mode')) : '';
}

/**
 * TARDIS submodules helper function.
 *
 * Adds extra table info when submodules are on.
 */
function _tardis_submodules_extra_info($tardis_element_type, &$tardis_elements_rows, &$tardis_elements_row, $tardis_element) {
  // If TARDIS Token is enabled, add extra info to the TARDIS pages table.
  if (module_exists('tardistoken') && $tardis_element_type == 'block') {
    $tardis_element_link_titles = array(
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#items' => array(
        '<strong>Title for year and month: </strong><br />' . filter_xss($tardis_element->tardistoken_year_month),
        '<strong>Title for year: </strong><br />' . filter_xss($tardis_element->tardistoken_year),
        '<strong>Title for month: </strong><br />' . filter_xss($tardis_element->tardistoken_month),
      ),
    );

    $tardis_elements_row[] = array('data' => $tardis_element_link_titles);
  }

  // If TARDIS Date is enabled, add extra info.
  (module_exists('tardisdate')) ? $tardis_elements_row[] = array('data' => $tardis_element->tardisdate_field) : '';

  // If TARDIS Internationalization is enabled, add extra info.
  if (module_exists('tardisi18n')) {
    // First we retrieve all the languages for this TARDIS from the db,
    // if any.
    $tardis_element_languages = ($tardis_element->tardisi18n_lang != '') ? unserialize($tardis_element->tardisi18n_lang) : array('cur_lang' => 0);
    // Then we remove the ones which are off.
    foreach ($tardis_element_languages as $key => $value) {
      if ($tardis_element_languages[$key] === 0) {
        unset($tardis_element_languages[$key]);
      }
    }
    // And theme results as an HTML item list.
    $tardis_element_languages_list = array(
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#items' => $tardis_element_languages,
    );

    $tardis_elements_row[] = array('data' => $tardis_element_languages_list);
  }

  // If TARDIS Render is enabled, add extra info to the TARDIS pages table.
  if (module_exists('tardisrender') && $tardis_element_type == 'page') {
    // First we retrieve all the view modes for this TARDIS from the db,
    // if any.
    $tardis_element_view_mode = ($tardis_element->tardisrender_view_mode != '') ? $tardis_element->tardisrender_view_mode : 'teaser';
    $tardis_elements_row[] = array('data' => $tardis_element_view_mode);
  }

  $tardis_elements_row[] = array(
    'data' => t(
      '<a href="@edit_link">edit</a> | <a href="@delete_link">delete</a>',
      array(
        '@edit_link' => 'tardis/' . $tardis_element_type . '/edit/' . $tardis_element->name,
        '@delete_link' => 'tardis/' . $tardis_element_type . '/delete/' . $tardis_element->name,
      )
    ),
  );

  $tardis_elements_rows[] = $tardis_elements_row;
}

/**
 * TARDIS submodules helper function.
 *
 * Retrieves TARDIS submodules' values.
 */
function _tardis_submodules_values($tardis_type, $query, &$tardisdate_field, &$tardisi18n_lang, &$tardisrender_view_mode, &$tardistoken_titles) {
  if (module_exists('tardisdate')) {
    // If TARDIS Date is enabled, check for which date.module field
    // is in the db - if any.
    $tardisdate_field = ($query['tardisdate_field'] != '') ? $query['tardisdate_field'] : 'created';
  }

  if (module_exists('tardisi18n')) {
    // If TARDIS Internationalization is enabled, check for languages - which
    // are stored in serialized form and must be unserialized.
    $tardisi18n_lang = ($query['tardisi18n_lang'] != '') ? unserialize($query['tardisi18n_lang']) : array('cur_lang' => 'cur_lang');
  }

  if (module_exists('tardisrender') && $tardis_type == 'page') {
    // If TARDIS Render is enabled, check for a view mode in the db, if any.
    $tardisrender_view_mode = ($query['tardisrender_view_mode'] != '') ? $query['tardisrender_view_mode'] : 'teaser';
  }

  if (module_exists('tardistoken')) {
    // If TARDIS Token is enabled, check for tokenized titles, if any.
    // They're stored in separate db fields, but retrieved as an array:
    // in this way, there needs to be only a single hidden field for
    // settings updates.
    $tardistoken_titles = array();
    switch ($tardis_type) {
      case 'page':
        $tardistoken_titles['year'] = ($query['tardistoken_year'] != '') ? $query['tardistoken_year'] : '';
        $tardistoken_titles['year_month'] = ($query['tardistoken_year_month'] != '') ? $query['tardistoken_year_month'] : '';
        break;

      case 'block':
        $tardistoken_titles['year'] = ($query['tardistoken_year'] != '') ? $query['tardistoken_year'] : '';
        $tardistoken_titles['month'] = ($query['tardistoken_month'] != '') ? $query['tardistoken_month'] : '';
        $tardistoken_titles['year_month'] = ($query['tardistoken_year_month'] != '') ? $query['tardistoken_year_month'] : '';
        break;
    }
  }
}

/**
 * TARDIS submodules helper function.
 *
 * Sets default values for TARDIS submodules.
 */
function _tardis_submodules_default_values($tardis_type, &$tardisdate_field, &$tardisi18n_lang, &$tardisrender_view_mode, &$tardistoken_titles) {
  if (module_exists('tardisdate')) {
    // Default value for TARDIS Date field.
    $tardisdate_field = 'created';
  }

  if (module_exists('tardisi18n')) {
    // Default value for TARDIS Internationalization language.
    $tardisi18n_lang = array('cur_lang' => 'cur_lang');
  }

  if (module_exists('tardisrender') && $tardis_type == 'page') {
    // If TARDIS Render is enabled, check for a view mode in the db, if any.
    $tardisrender_view_mode = 'teaser';
  }

  if (module_exists('tardistoken')) {
    // If TARDIS Token is enabled, check for tokenized titles, if any.
    $tardistoken_titles = array();
    switch ($tardis_type) {
      case 'page':
        $tardistoken_titles['year'] = '';
        $tardistoken_titles['year_month'] = '';
        break;
    }
  }
}

/**
 * TARDIS submodules helper function.
 *
 * Passes update information on.
 */
function _tardis_submodules_update_info($tardis_type, &$form, $tardisdate_field, $tardisi18n_lang, $tardisrender_view_mode, $tardistoken_titles) {
  // If TARDIS Date module is enabled, pass the created or date.module field on.
  if (module_exists('tardisdate')) {
    $form['tardisdate_update'] = array(
      '#type' => 'hidden',
      '#value' => $tardisdate_field,
    );
  }

  // If TARDIS Internationalization module is enabled,
  // serialize and pass the languages on.
  if (module_exists('tardisi18n')) {
    $form['tardisi18n_update'] = array(
      '#type' => 'hidden',
      '#value' => $tardisi18n_lang,
    );
  }

  // If TARDIS Render module is enabled, pass the view mode on.
  if (module_exists('tardisrender') && $tardis_type == 'page') {
    $form['tardisrender_update'] = array(
      '#type' => 'hidden',
      '#value' => $tardisrender_view_mode,
    );
  }

  // If TARDIS Token module is enabled, store any tokenized titles as an array.
  if (module_exists('tardistoken')) {
    $form['tardistoken_update'] = array(
      '#type' => 'hidden',
      '#value' => $tardistoken_titles,
    );
  }
}
