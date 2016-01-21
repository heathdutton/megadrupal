<?php

/**
 * @file
 * Admin page callbacks.
 */


/**
 * Admin settings form
 */
function booklists_settings() {
  $form = array();
  
  $form['booklists_catalog_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Polaris Catalog URL'),
    '#description' => t('e.g., http://catalog.polaris.com/polaris/view.aspx'),
    '#default_value' => variable_get('booklists_catalog_url'),
    '#size' => 100
  );
  $form['booklists_nytimes_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('New York Times API Key'),
    '#description' => t('If you plan to create blocks using the NYTimes API, you can <a href="http://developer.nytimes.com/apps/register" target="_blank">get an API key here</a>.<br /><strong>Note:</strong> After you save this form to initially record your API key, you\'ll be able to add NYTimes blocks.'),
    '#default_value' => variable_get('booklists_nytimes_api_key')
  );
  $form['booklists_amazon_associates_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Amazon Associates ID'),
    '#description' => t('If you want your lists to allow users to purchase materials on Amazon.com and get credit through your library\'s Assocates ID, you can <a href="https://affiliate-program.amazon.com/" target="_blank">get an Amazon Associates ID here</a>.'),
    '#default_value' => variable_get('booklists_amazon_associates_id'),
    '#size' => 10
  );
  $form['overdrive'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overdrive'),
    '#description' => t('If you plan to display one of the NYTimes lists of eBooks and wish to do single-click links into your Overdrive holdings, please fill out this section.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['overdrive']['booklists_overdrive_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Overdrive URL'),
    '#description' => t('e.g. http://example.lib.overdrive.com'),
    '#default_value' => variable_get('booklists_overdrive_url')
  );
  $form['overdrive']['booklists_overdrive_client_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Overdrive Client Key'),
    '#description' => t('Can be acquired on https://developer.overdrive.com'),
    '#default_value' => variable_get('booklists_overdrive_client_key')
  );
  $form['overdrive']['booklists_overdrive_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Overdrive Client Secret'),
    '#description' => t('Can be acquired on https://developer.overdrive.com'),
    '#default_value' => variable_get('booklists_overdrive_client_secret')
  );
  $form['overdrive']['booklists_overdrive_account_id_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Overdrive Account ID Number'),
    '#description' => t('Can be found on https://developer.overdrive.com in the "Library Search". Look for a 4 or 5-digit number.'),
    '#default_value' => variable_get('booklists_overdrive_account_id_number'),
    '#size' => 5
  );
  
  $form['amazon'] = array(
    '#type' => 'fieldset',
    '#title' => t('Amazon Blocks'),
    '#description' => t('Choose how many blocks related to Amazon RSS feeds you wish to have available when you visit <a href="/admin/structure/blocks">the blocks page</a>.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  for ($x=1; $x <= 10; $x++) {
    $collapsed = TRUE;
    if (variable_get('booklists_amazon_block_' . $x . '_title') && variable_get('booklists_amazon_block_' . $x . '_url')) {
      $collapsed = FALSE;
    }
    $form['amazon']['block_' . $x] = array(
      '#type' => 'fieldset',
      '#title' => t('Block ' . $x),
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed,
    );
    $form['amazon']['block_' . $x]['booklists_amazon_block_' . $x . '_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Amazon Block ' . $x . ' Title'),
      '#description' => t('e.g. Amazon RSS Bestsellers in Magazines'),
      '#default_value' => variable_get('booklists_amazon_block_' . $x . '_title')
    );
    $form['amazon']['block_' . $x]['booklists_amazon_block_' . $x . '_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Amazon Block ' . $x . ' RSS URL'),
      '#description' => t('e.g. http://www.amazon.com/gp/rss/bestsellers/magazines'),
      '#default_value' => variable_get('booklists_amazon_block_' . $x . '_url')
    );
  }

  // If we have the library's API key then we can check and see which lists are currently available from the API via a call to it.
  if (NYTIMES_API_KEY) {
    // Call for lists.
    $books = new NYTBooks(NYTIMES_API_KEY);
    // List all available types of lists.
    $list_types = json_decode($books->get_list_types());
    $list_options = array('' => '-SELECT-');
    if (isset($list_types->results)) {
      foreach ($list_types->results as $list) {
        $list_options[str_replace(' ', '-', $list->list_name)] = $list->list_name;
      }
    }

    // Options list for number of titles to display
    $count_options = array('' => '-SELECT-');
    for ($x=1; $x <= 20; $x++) {
      $count_options[$x] = $x;
    }

    $form['nytimes'] = array(
      '#type' => 'fieldset',
      '#title' => t('New York Times Blocks'),
      '#description' => t('Choose how many blocks related to the NYTimes you wish to have available when you visit <a href="/admin/structure/blocks">the blocks page</a>.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    for ($x=1; $x <= 10; $x++) {
      $collapsed = TRUE;
      if (variable_get('booklists_nyt_block_' . $x . '_list')) {
        $collapsed = FALSE;
      }
      $form['nytimes']['block_' . $x] = array(
        '#type' => 'fieldset',
        '#title' => t('Block ' . $x),
        '#collapsible' => TRUE,
        '#collapsed' => $collapsed,
      );
      $form['nytimes']['block_' . $x]['booklists_nyt_block_' . $x . '_list'] = array(
        '#type' => 'select',
        '#title' => t('Block ' . $x . ' List'),
        '#options' => $list_options,
        '#default_value' => variable_get('booklists_nyt_block_' . $x . '_list')
      );
      $form['nytimes']['block_' . $x]['booklists_nyt_block_' . $x . '_list_vertical'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display as a vertical list.'),
        '#default_value' => variable_get('booklists_nyt_block_' . $x . '_list_vertical')
      );
    }
  }
  
  $form['polaris'] = array(
    '#type' => 'fieldset',
    '#title' => t('Polaris Lists'),
    '#description' => t('If you have a Polaris catalog and have set up an ODBC connection that allows you to do direct SQL queries against the database to display lists like New Arrivals, Coming Soon, Just Checked In, and Most Popular, please fill out this section.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['polaris']['booklists_polaris_odbc_host'] = array(
    '#type' => 'textfield',
    '#title' => t('ODBC Host'),
    '#description' => t('e.g., catalog.library.com (Do not include http://)'),
    '#default_value' => variable_get('booklists_polaris_odbc_host')
  );
  $form['polaris']['booklists_polaris_odbc_dbname'] = array(
    '#type' => 'textfield',
    '#title' => t('Database name'),
    '#description' => t('The default is usually just "polaris" (Do not include quotation marks).'),
    '#default_value' => variable_get('booklists_polaris_odbc_dbname')
  );
  $form['polaris']['booklists_polaris_odbc_username'] = array(
    '#type' => 'textfield',
    '#title' => t('SQL username'),
    '#description' => t('The MSSQL user who will be making the queries'),
    '#default_value' => variable_get('booklists_polaris_odbc_username')
  );
  $form['polaris']['booklists_polaris_odbc_password'] = array(
    '#type' => 'password',
    '#title' => t('SQL password'),
    '#description' => t('The MSSQL user password'),
    '#default_value' => variable_get('booklists_polaris_odbc_password')
  );

  $form = system_settings_form($form);
  return $form;
}