<?php

/**
 * @file
 * admin page callbacks and form handlers for the Smart Tribune snippet module
 */

/**
 * Page callback for Smart Tribune snippet admin page.
 *
 * @return array
 *   the themed output for the admin overview page
 */
function smarttribune_snippet_admin_overview() {
  drupal_add_css(drupal_get_path('module', 'smarttribune_snippet') . '/css/smarttribune_snippet.css');
  drupal_add_js(drupal_get_path('module', 'smarttribune_snippet') . '/js/smarttribune_snippet.js');
  $output = drupal_get_form('smarttribune_snippet_overview_form');

  return $output;
}

/**
 * Theme function for the Smart Tribune snippet admin overview table.
 *
 * Make sure that the weight field is displayed as a form element.
 */
function theme_smarttribune_snippet_overview_table(&$variables) {
  if (isset($variables['form']['smarttribune_snippet'])) {
    $form = $variables['form']['smarttribune_snippet'];
    foreach (element_children($form) as $region) {
      $i = 0;
      foreach (element_children($variables['form']['smarttribune_snippet'][$region]['item']) as $tcid) {
        // We have to render the form item since D7 didn't handle the table form
        // concept.
        // Instead we display the output of the rendered field.
        $weight = drupal_render($variables['form']['smarttribune_snippet'][$region]['item'][$tcid]['weight']);
        $variables['form']['smarttribune_snippet'][$region]['#rows'][$i]['data']['weight'] = $weight;
        $i++;
      }
    }
    return drupal_render_children($variables['form']);
  }
}

/**
 * Form for Smart Tribune snippet overview table.
 *
 * @return array
 *   a renderable Form API array
 */
function smarttribune_snippet_overview_form() {
  $regions = _smarttribune_snippet_get_all_snippets();
  $nb_items = 0;
  $form = array(
    '#type' => 'form',
    '#theme' => 'smarttribune_snippet_overview_table',
    '#theme_wrappers' => array('form'),
  );

  // Check if we have any available snippet.
  foreach ($regions as $region) {
    $nb_items += count($region);
  }
  if ($nb_items == 0) {
    $form['message'] = array(
      '#type' => 'html_tag',
      '#tag' => 'p',
      '#value' => t('You have not configured any Smart Tribune snippet yet.'),
    );
  }
  else {
    $header = array(t('Name'), t('Code'), t('Actions'), t('Weight'));
    $form['smarttribune_snippet'] = array('#tree' => TRUE);
    foreach ($regions as $region => $snippets) {
      $rows = array();
      $form['smarttribune_snippet'][$region] = array(
        '#prefix' => '<h2>' . ucfirst($region) . '</h2>',
        '#theme' => 'table',
        '#header' => $header,
        '#attributes' => array(
          'id' => 'smarttribune-snippet-admin-table-' . $region,
        ),
      );
      foreach ($snippets as $id => $snippet) {
        $enabled = ($snippet->status) ? t('Disable') : t('Enable');
        $actions = array(
          l($enabled, 'admin/structure/smarttribune_snippet/', array(
            'attributes' => array(
              'class' => 'smarttribune-snippet-disable-link',
              'rel' => $snippet->tcid,
            ),
            'html' => TRUE,
          )),
          l(t('Configure'), 'admin/structure/smarttribune_snippet/' . $snippet->tcid . '/edit'),
          l(t('Delete'), 'admin/structure/smarttribune_snippet/' . $snippet->tcid . '/delete'),
        );
        // Build a form element for each weight attribute.
        $form['smarttribune_snippet'][$region]['item'][$snippet->tcid]['weight'] = array(
          '#title' => t('Weight'),
          '#title_display' => 'invisible',
          '#type' => 'textfield',
          '#size' => 3,
          '#default_value' => $snippet->weight,
          '#element_validate' => array('element_validate_integer'),
          '#attributes' => array('class' => array('snippet-weight')),
        );
        // Prepare the table content.
        $rows[] = array(
          'data' => array(
            'name' => array(
              'data' => check_plain($snippet->name),
              'class' => 'smarttribune-snippet-name',
            ),
            'code' => array(
              'data' => check_plain(truncate_utf8($snippet->code, 150, FALSE, '...')),
              'class' => 'smarttribune-snippet-code',
            ),
            'actions' => implode(' | ', $actions),
            'weight' => '',
          ),
          'class' => ($snippet->status) ? array('draggable') : array('draggable smarttribune-snippet-disabled'),
        );
      }
      $form['smarttribune_snippet'][$region]['#rows'] = $rows;
      drupal_add_tabledrag('smarttribune-snippet-admin-table-' . $region, 'order', 'siblings', 'snippet-weight');
    }
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }
  return $form;
}

/**
 * Form for adding codeblocks (admin/structure/smarttribune_snippet/add).
 *
 * @return array
 *   a renderable Form API array
 */
function smarttribune_snippet_add_form() {
  $form['smarttribune_snippet_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter a name to describe this Smart Tribune snippet'),
  );

  $form['smarttribune_snippet_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Code'),
    '#description' => t('Paste your Smart Tribune snippet here. You can fetch your snippet code by accessing the "!snippet_link" page.', array('!snippet_link' => l(t('Button settings'), 'https://smart-tribune.com/admin/visuals/button'))),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Snippet'),
  );

  return $form;
}

/**
 * Form for editing codeblocks (admin/structure/smarttribune_snippet/%/edit).
 *
 * @param array $snippet
 *   A loaded snippet.
 *
 * @return array
 *   a renderable Form API array
 */
function smarttribune_snippet_edit_form($form, &$form_state, $snippet) {
  _smarttribune_snippet_set_breadcrumb();

  $form['delta'] = array(
    '#type' => 'hidden',
    '#value' => $snippet['tcid'],
  );

  $form['smarttribune_snippet_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Enter a name to describe this code block'),
    '#default_value' => $snippet['name'],
  );

  $form['smarttribune_snippet_status'] = array(
    '#type' => 'radios',
    '#title' => t('Enable Smart Tribune Snippet'),
    '#description' => t("Smart Tribune snippets are disabled by default so you won't accidentally make Smart Tribune snippet live if you didn't intend to."),
    '#default_value' => $snippet['status'],
    '#options' => array(1 => t('Active'), 0 => t('Disabled')),
  );

  $form['smarttribune_snippet_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Code'),
    '#description' => t('Enter your Smart Tribune snippet here'),
    '#default_value' => $snippet['code'],
  );

  $form['smarttribune_snippet_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility Settings'),
    '#collapsible' => TRUE,
  );

  $form['smarttribune_snippet_options']['smarttribune_snippet_region'] = array(
    '#type' => 'radios',
    '#title' => t('Render this code:'),
    '#options' => array(
      'header' => t('Header'),
      'footer' => t('Footer'),
    ),
    '#default_value' => $snippet['region'],
  );

  $form['smarttribune_snippet_options']['smarttribune_snippet_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Invoke this Smart Tribune snippet on specific pages:'),
    '#options' => array(
      BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
      BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
    ),
    '#default_value' => $snippet['visibility'],
  );

  $form['smarttribune_snippet_options']['smarttribune_snippet_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. <front> is the front page."),
    '#default_value' => $snippet['pages'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Snippet'),
  );

  return $form;
}

/**
 * Confirmation form for deleting snippets.
 *
 * @param array $snippet
 *   A loaded snippet.
 */
function smarttribune_snippet_delete_form($form, &$form_state, $snippet) {
  $form['delta'] = array('#type' => 'hidden', '#value' => $snippet['tcid']);
  $form['snippet_name'] = array('#type' => 'hidden', '#values' => $snippet['name']);

  return confirm_form($form, t('Are you sure you want to delete the Smart Tribune snippet %name?', array('%name' => $snippet['name'])), 'admin/structure/smarttribune_snippet', t('You are about to delete a snippet.'), t('Delete'), t('Cancel'));
}

/**
 * Page callback for AJAX enable/disable request on a codeblock.
 *
 * @param array $snippet
 *   Loaded snippet.
 *
 * @return string
 *   a JSON object containing the status and label replacement
 */
function smarttribune_snippet_ajax_disable($snippet) {
  $status = ($snippet['status']) ? 0 : 1;

  db_update('smarttribune_snippet')
    ->fields(array('status' => $status))
    ->condition('tcid', $snippet['tcid'], '=')
    ->execute();

  $response = array(
    'status' => $status,
    'label' => ($status ? t('Disable') : t('Enable')),
  );

  print drupal_json_output($response);
  drupal_exit();
}


/**
 * ### SUBMIT/VALIDATE FUNCTIONS ###
 */

/**
 * Submit function for smarttribune_snippet overview/weight table.
 */
function smarttribune_snippet_overview_form_submit($form, &$form_state) {
  foreach ($form_state['values']['smarttribune_snippet'] as $region_name => $region) {
    foreach ($region['item'] as $tcid => $data) {
      db_update('smarttribune_snippet')
        ->fields(array('weight' => $data['weight']))
        ->condition('tcid', $tcid, '=')
        ->execute();
    }
  }
  drupal_set_message(t('Smart Tribune snippet weights have been updated.'));
}

/**
 * Submit function for smarttribune_snippet add form.
 */
function smarttribune_snippet_add_form_submit($form, &$form_state) {
  $tcid = db_insert('smarttribune_snippet')
    ->fields(array(
      'name' => $form_state['values']['smarttribune_snippet_name'],
      'code' => $form_state['values']['smarttribune_snippet_code'],
      'status' => 0,
      'weight' => 0,
      'visibility' => 0,
      'pages' => '',
    ))
    ->execute();

  $form_state['redirect'] = 'admin/structure/smarttribune_snippet/' . $tcid . '/edit';
  drupal_set_message(t('Created new Smart Tribune snippet "%name."', array('%name' => $form_state['values']['smarttribune_snippet_name'])));
}

/**
 * Submit function for smarttribune_snippet edit form.
 */
function smarttribune_snippet_edit_form_submit($form, &$form_state) {
  db_update('smarttribune_snippet')
    ->fields(array(
      'name' => $form_state['values']['smarttribune_snippet_name'],
      'code' => $form_state['values']['smarttribune_snippet_code'],
      'region' => $form_state['values']['smarttribune_snippet_region'],
      'status' => $form_state['values']['smarttribune_snippet_status'],
      'visibility' => $form_state['values']['smarttribune_snippet_visibility'],
      'pages' => $form_state['values']['smarttribune_snippet_pages'],
    ))
    ->condition('tcid', $form_state['values']['delta'], '=')
    ->execute();

  $form_state['redirect'] = 'admin/structure/smarttribune_snippet';
  drupal_set_message(t('Updated Smart Tribune snippet "%name."', array('%name' => $form_state['values']['smarttribune_snippet_name'])));
}

/**
 * Submit function for smarttribune_snippet delete form.
 */
function smarttribune_snippet_delete_form_submit($form, &$form_state) {
  db_delete('smarttribune_snippet')
    ->condition('tcid', $form_state['values']['delta'])
    ->execute();

  $form_state['redirect'] = 'admin/structure/smarttribune_snippet';
  drupal_set_message(t('Deleted Smart Tribune snippet "%name."', array('%name' => $form_state['values']['snippet_name'])));
}
