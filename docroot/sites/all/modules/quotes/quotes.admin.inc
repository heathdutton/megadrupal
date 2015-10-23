<?php
/**
 * @file
 * The quotes module allows users to maintain a list of quotes that
 * can be displayed in any number of administrator-defined quote
 * blocks.
 *
 * @copyright Copyright (c) 2003-2007 Jim Riggs.  All rights reserved.
 * @author Jim Riggs <drupal at jim and lissa dot com>
 */


/**
 * Displays the admin settings page.
 */
function quotes_admin_settings_page() {
  return drupal_get_form('quotes_admin_settings');
}

/**
 * Displays the general admin settings form.
 *
 * @return array
 *   An array containing the form elements to be displayed.
 */
function quotes_admin_settings($form, $form_state) {
  $form = array();
  $yesno = array(t('No'), t('Yes'));
  // Next 2 Lines are for Vertical-Tabs - comment out.
  // $myjs = drupal_get_path('module', 'quotes') . '/js/quotes.admin.js';
  // $vert_tabs = array('js' => array('vertical-tabs' => $myjs));
  $count = db_query("SELECT COUNT(*) FROM {node} n where n.type='quotes' AND n.status=1")->fetchField();

  $form['count'] = array(
    '#type' => 'item',
    '#markup' => t('There are currently !count published quotations.', array('!count' => $count)),
  );

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    // '#attached' => $vert_tabs,
    '#description' => t('These options are for general theming.'),
    // '#group' => 'display',
  );

  $form['display']['quotes_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Quotes per page'),
    '#size' => 6,
    '#default_value' => variable_get('quotes_per_page', 10),
    '#description' => t('The number of quotes included on a page.'),
    '#prefix' => '<div class="clear-block"></div>',
  );

  $form['display']['quotes_leader'] = array(
    '#type' => 'textfield',
    '#title' => t('Author leader'),
    '#default_value' => variable_get('quotes_leader', '&mdash;'),
    '#description' => t('The text placed before the author attribution (for example, "&amp;mdash;" for an em-dash or "&amp;#8226;" for a bullet).'),
  );

  $form['display']['quotes_author_link'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Make author a link'),
    // '#attributes' => array('class' => 'container-inline'),
    '#prefix' => '<div id="author-link">',
    '#suffix' => '</div>',
    '#default_value' => variable_get('quotes_author_link', FALSE),
    '#description' => t('If selected, the author text will be a link to show all quotes by that author.'),
  );

  $form['display']['quotes_author_bio'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t("Don't display"),
      1 => t('Include text'),
      2 => t('Include as a link'),
    ),
    '#title' => t("Include author's bio"),
    '#default_value' => variable_get('quotes_author_bio', FALSE),
    '#description' => t("If selected, the author's biography will be shown on the Quotes page."),
    // '#attributes' => array('class' => 'container-inline'),
    '#prefix' => '<div id="author-bio">',
    '#suffix' => '</div>',
  );

  // Get the quotes_format or set it to the default.
  $format_list = variable_get('quotes_format', 0);
  if (!$format_list) {
    variable_set('quotes_format', filter_default_format());
  }
  $format_list = filter_formats($GLOBALS['user']);
  foreach ($format_list as $number => $filter) {
    $formats[$number] = $filter->name;
  }
  $form['display']['quotes_format'] = array(
    '#type' => 'radios',
    '#options' => $formats,
    '#title' => t('Author, citation, and bio input format'),
    '#default_value' => variable_get('quotes_format', 0),
    '#description' => t('This will be used as the input format for the author, citation, and bio fields.'),
    // '#attributes' => array('class' => 'container-inline'),
    '#prefix' => '<div id="quotes-format">',
    '#suffix' => '</div>',
    '#group' => 'display',
  );

  $form['display']['quotes_edit_link'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Add an "edit" link'),
    '#default_value' => variable_get('quotes_edit_link', TRUE),
    '#description' => t('If selected, an "edit" link will be shown for each quote.'),
    // '#attributes' => array('class' => 'container-inline'),
    '#prefix' => '<div id="quotes-edit-link">',
    '#suffix' => '</div>',
    '#group' => 'display',
  );

  $form['display']['quotes_quick_nav_original'] = array(
    '#type' => 'value',
    '#value' => variable_get('quotes_quick_nav', TRUE),
  );

  $form['display']['quotes_quick_nav'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Show quick navigation tabs'),
    '#default_value' => variable_get('quotes_quick_nav', TRUE),
    '#description' => t('If selected, quick navigation tabs will be shown at the top of the quotes page.'),
    // '#attributes' => array('class' => 'container-inline'),
    '#prefix' => '<div id="quotes-quick-nav">',
    '#suffix' => '</div>',
    '#group' => 'display',
  );

  $form['myquotes'] = array(
    '#type' => 'fieldset',
    '#title' => t('My Quotes links'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['myquotes']['quotes_showlink'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t("Show link to users' quotes"),
    // '#attributes' => array('class' => 'container-inline'),
    '#prefix' => '<div id="quotes-user-link">',
    '#suffix' => '</div>',
    '#default_value' => variable_get('quotes_showlink', TRUE),
    '#description' => t('Uncheck this to disable the link to each users\' "my quotes" page when viewing a node.'),
  );

  $form['myquotes']['quotes_show_myquotes_original'] = array(
    '#type' => 'value',
    '#value' => variable_get('quotes_show_myquotes', TRUE),
  );

  $form['myquotes']['quotes_show_myquotes'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Show the "my quotes" menu item'),
    // '#attributes' => array('class' => 'container-inline'),
    '#prefix' => '<div id="quotes-myquotes">',
    '#suffix' => '</div>',
    '#default_value' => variable_get('quotes_show_myquotes', TRUE),
    '#description' => t('Uncheck this to disable the "My quotes" menu item for all users. Note, changing this setting will require the menu to be rebuilt on the next page load or short there after'),
  );

  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('These options are for users.'),
  );

  $form['user']['quotes_user_recent'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#title' => t('Add a "Quotes" tab on the "My account" page'),
    // '#attributes' => array('class' => 'container-inline'),
    '#prefix' => '<div id="myquotes-tab">',
    '#suffix' => '</div>',
    '#default_value' => variable_get('quotes_user_recent', 0),
  );

  // Add a submit handler to force a menu rebuild, if needed.
  $form['#submit'][] = '_quotes_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Check the menu settings for changes.
 */
function _quotes_settings_form_submit($form, $form_state) {
  if (($form_state['values']['quotes_show_myquotes'] != $form_state['values']['quotes_show_myquotes_original'])
    || ($form_state['values']['quotes_quick_nav'] != $form_state['values']['quotes_quick_nav_original'])) {
    // Force the menu to be rebuilt on next page load.
    variable_set('menu_rebuild_needed', TRUE);
    drupal_set_message(t('The menu will be rebuilt.'));
  }
}

/**
 * Export function page.
 *
 * @return array
 *   A form with a tab-delimited list of quotes.
 */
function quotes_export() {
  $form['intro'] = array(
    '#type' => 'item',
    '#title' => t('Copy and paste this list to the receiving site'),
  );

  $output = NULL;
  $count = 0;

  $sql = db_select('node', 'n');
  $nr_alias = $sql->join('node_revision', 'nr', 'nr.vid = n.vid');
  $sql->fields('nr', array('title'));
  $frb_alais = $sql->join('field_revision_body', 'frb', 'frb.revision_id = nr.vid');
  $sql->fields('frb', array('body_value'));
  $q_alais = $sql->join('quotes', 'q', 'q.vid=n.vid');
  $sql->fields('q', array('aid', 'citation'));

  $sql
    ->condition('n.status', 1)
    ->condition('n.type', 'quotes')
    ->orderby('q.aid');
  $result = $sql->execute();

  $author = '';
  $current_aid = 0;

  foreach ($result as $row) {
    if ($current_aid < $row->aid) {
      $current_aid = $row->aid;
      $author = db_query(
        "SELECT qa.name FROM {quotes_authors} qa where qa.aid = :current_aid",
        array(':current_aid' => $current_aid)
      )->fetchField();
    }
    $row->author = $author;
    ++$count;
    $output .= _quotes_escape_newlines($row->body_value) . "\t" .
      _quotes_escape_newlines($row->author) . "\t" .
      _quotes_escape_newlines($row->citation) . "\n";
  }
  drupal_set_message(t('Found @count quotes.', array('@count' => $count)));
  if ($count == 0) {
    $count = 1;
    $output = t('No quotes were found.');
  }

  $form['list'] = array(
    '#type' => 'textarea',
    '#value' => $output,
    '#rows' => min(30, $count),
  );

  return $form;
}

/**
 * Helper function to strip Windows newline and format Unix newlines.
 *
 * @param string $text
 *   The text to be scanned.
 *
 * @return string
 *   Text with newlines processed.
 */
function _quotes_escape_newlines($text) {
  // Get rid of Windows crap.
  $text = str_replace("\r", '', $text);
  return str_replace("\n", "\\\n", $text);
}

/**
 * Displays a list of currently-defined quote blocks.
 */
function quotes_blocks_settings($form, $form_state) {

  $header = array(t('Name'), t('Filters'), t('Operations'), '');

  $rows = array();
  $result = db_select('quotes_blocks', 'qb')->extend('PagerDefault')->extend('TableSort')
    ->fields('qb')
    ->limit(20)
    ->orderByHeader($header)
    ->execute();

  foreach ($result as $qblock) {

    $filters = array();

    if ($qblock->nid_filter) {
      $filters[] = t('node');
    }

    if ($qblock->rid_filter) {
      $filters[] = t('role');
    }

    if ($qblock->uid_filter) {
      $filters[] = t('user');
    }

    if ($qblock->tid_filter) {
      $filters[] = t('term');
    }

    $rows[] = array(
      // Cells.
      t($qblock->name),
      implode(', ', (count($filters) ? $filters : array(t('none')))),
      l(t('configure block'), "admin/structure/block/manage/quotes/$qblock->bid/configure"),
      l(t('delete block'), "admin/config/quotes/delete/$qblock->bid"),
    );
  }

  $form = array();
  $form['quotes_blocks_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No quotes blocks created.'),
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#size' => 32,
    '#default_value' => NULL,
    '#maxlength' => 64,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add block'),
  );

  $form['quotes_blocks_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No quotes blocks created.'),
  );

  $form['quotes_blocks_pager'] = array('#theme' => 'pager');
  return $form;
}

/**
 * Validates that the new block name is valid.
 */
function quotes_blocks_settings_validate($form, &$form_state) {
  $name = trim($form_state['values']['name']);
  if (!$name) {
    form_set_error('name', t('You must specify a valid block name.'));
  }
  else {
    $result = db_select('quotes_blocks', 'qb')
      ->fields('qb', array('name'))
      ->condition('qb.name', $name)
      ->execute()->fetchField();
    if ($result) {
      form_set_error('name', t('The block name %name already exists. Please choose another block name.', array('%name' => $name)));
    }
  }
}

/**
 * Creates the new quote block.
 *
 * @param array $form
 *   The form that was submitted.
 * @param array $form_state
 *   The array specifying the form values.
 */
function quotes_blocks_settings_submit($form, &$form_state) {
  db_insert('quotes_blocks')
    ->fields(array(
      'name' => trim($form_state['values']['name']),
      'block_type' => '0',
      'nid_filter' => '',
      'aid_filter' => '',
      'rid_filter' => '',
      'uid_filter' => '',
      'tid_filter' => '',
      'cron_interval' => 0,
      'cron_step' => 0,
      'cron_last' => 0,
      'vid' => 0,
    ))
    ->execute();
}

/**
 * Confirms the deletion a quote block.
 *
 * @param array $bid
 *   The block values of the block being deleted.
 *
 * @return string
 *   A string containing the confirmation form displayed to the user.
 */
function _quotes_block_delete($form_stuff, $bid) {

  $bid_id = $bid['build_info']['args'][0];

  $qbname = db_select('quotes_blocks', 'qb')
    ->fields('qb', array('name'))
    ->condition('qb.bid', $bid_id)
    ->execute()
    ->fetchField();

  if (isset($qbname) && (!empty($qbname))) {
    $form = array();
    $form['bid'] = array(
      '#type' => 'value',
      '#value' => $bid_id,
    );
    $form['block_name'] = array(
      '#type' => 'value',
      '#value' => $qbname,
    );

    return confirm_form($form, t('Are you sure you want to delete the block %name?', array('%name' => $qbname)), 'admin/config/quotes/blocks', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
  }
  else {
    drupal_set_message(t('There are no quotes configuration blocks to delete .'));
  }
}

/**
 * Deletes the specified block.
 *
 * @param array $form
 *   The form that was submitted.
 * @param array $form_state
 *   The array specifying the form values.
 *
 * @result string
 *   A string specifying the page to which the user should be
 *   redirected (admin/settings/quotes/blocks).
 */
function _quotes_block_delete_submit($form, &$form_state) {
  db_delete('quotes_blocks')
    ->condition('bid', $form_state['values']['bid'])
    ->execute();
  // Now delete the block(s) from Drupals block table.
  db_delete('block')
    ->condition('delta', $form_state['values']['bid'])
    ->condition('module', 'quotes')
    ->execute();
  drupal_set_message(t('The block %name has been removed.', array('%name' => $form_state['values']['block_name'])));
  cache_clear_all();

  $form_state['redirect'] = 'admin/config/quotes/blocks';
}

/**
 * Bios maintenance page.
 *
 * @param array $form
 *   The form that was submitted.
 * @param array $form_state
 *   The array specifying the form values.
 * @param int $aid
 *   The author Id of the bio.
 *
 * @return array
 *   A form with a tab-delimited list of quotes.
 */
function quotes_bios($form, &$form_state, $aid = NULL) {
  $form = array();
  $first_pass = is_null($aid);
  global $_quotes_auths;
  if ($first_pass) {
    $_quotes_auths = quotes_get_authors();
    $data = array('name' => NULL, 'bio' => NULL);
  }
  else {
    // Get the data.
    $data = db_select('quotes_authors', 'qa')
      ->fields('qa')
      ->condition('qa.aid', $aid)
      ->execute()->fetchAssoc();

  }
  $count = count($_quotes_auths);

  $form['aid'] = array(
    '#type' => 'hidden',
    '#value' => $aid,
  );

  $form['author'] = array(
    '#type' => 'select',
    '#options' => $_quotes_auths,
    '#size' => min(20, $count),
    '#description' => t('Pick the author whose biography you wish to update.'),
  );

  $form['name'] = array(
    '#type' => 'markup',
    '#value' => '<h3>' . t('Biography for %name', array('%name' => $data['name'])) . '</h3>',
  );

  $form['bio'] = array(
    '#type' => 'textarea',
    '#description' => t("This is %name's biography.", array('%name' => $data['name'])),
    '#default_value' => $data['bio'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $first_pass ? t('Get biography') : t('Update'),
  );

  // Show the fields at the right time.
  if ($first_pass) {
    $form['name']['#type'] = 'hidden';
    $form['bio']['#type'] = 'hidden';
  }
  else {
    $form['author']['#type'] = 'hidden';
  }

  return $form;
}

/**
 * Submit the authors name for bio modification.
 *
 * After Modification of the bio submit update the db tables.
 *
 * @param array $form
 *   The form that was submitted.
 * @param array $form_state
 *   The array specifying the form values.
 */
function quotes_bios_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'Update') {
    $vals = array($form_state['values']['bio'], $form_state['values']['aid']);
    $upd = db_update('quotes_authors')
      ->fields(array('bio' => $form_state['values']['bio']))
      ->condition('aid', $form_state['values']['aid'])
      ->execute();
    $form_state['redirect'] = BIOS_PATH;
  }
  else {
    $form_state['redirect'] = BIOS_PATH . '/' . $form_state['values']['author'];
  }
}
