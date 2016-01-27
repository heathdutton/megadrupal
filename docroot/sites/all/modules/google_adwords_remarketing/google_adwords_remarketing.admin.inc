<?php

/**
 * @file
 * Admin page callbacks for the Google Adwords Remarketing module.
 * Google Adwords Remarketing Campaign aka GARC
 */

/**
 * Menu callback for admin/config/services/garc.
 */
function google_adwords_remarketing_admin_display() {

  // Fetch and sort GARCs
  $garcs = array();
  $result = db_query("SELECT * FROM {google_adwords_remarketing}");
  foreach ($result as $garc) {
    $garcs[$garc->garcid] = $garc;
  }
  if (!empty($garcs)) {
    usort($garcs, '_garc_compare');
  }
  return drupal_get_form('google_adwords_remarketing_admin_display_form', $garcs);
}

/**
 * Generate main GARCs administration form.
 */
function google_adwords_remarketing_admin_display_form($form, &$form_state, $garcs) {
  $form = array();

  // If non-default theme configuration has been selected, set the custom theme.
  // @TODO implement per theme, right now it's just the theme_default thats being done.
  // global $theme_key, $custom_theme;
  // $custom_theme = isset($theme) ? $theme : variable_get('theme_default', 'garland');

  // Weights range from -delta to +delta, so delta should be at least half
  // of the amount of GARCs present. This makes sure all GARCs in the same
  // region get an unique weight.
  $weight_delta = round(count($garcs) / 2);

  if (count($garcs) > 0) {
    $form['#tree'] = TRUE;

    foreach ($garcs as $garc) {
      $form['campaigns'][$garc->garcid]['garcid'] = array(
        '#type' => 'hidden',
        '#value' => $garc->garcid,
      );
      $form['campaigns'][$garc->garcid]['info'] = array(
        '#type' => 'markup',
        '#markup' => check_plain($garc->info)
      );
      $form['campaigns'][$garc->garcid]['status'] = array(
        '#type' => 'checkbox',
        '#default_value' => $garc->status,
      );
      $form['campaigns'][$garc->garcid]['weight'] = array(
        '#type' => 'weight',
        '#default_value' => $garc->weight,
        '#delta' => $weight_delta,
      );
      $form['campaigns'][$garc->garcid]['configure'] = array(
        '#type' => 'link',
        '#title' => t('configure'),
        '#href' => 'admin/config/services/garc/'. $garc->garcid .'/configure',
      );
      $form['campaigns'][$garc->garcid]['delete'] = array(
        '#type' => 'link',
        '#title' => t('delete'),
        '#href' => 'admin/config/services/garc/'. $garc->garcid .'/delete',
      );
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update campaigns'),
    );
  }

  return $form;
}

/**
 * Process main google adwords campaign administration form submission.
 */
function google_adwords_remarketing_admin_display_form_submit($form, &$form_state) {
  foreach ($form_state['values']['campaigns'] as $garc) {
    drupal_write_record('google_adwords_remarketing', $garc, 'garcid');
  }
  drupal_set_message(t('The campaign settings have been updated.'));
  cache_clear_all('google_adwords_remarketing_garc_list', 'cache');
}

/**
 * Process variables for google-adwords-remarketing-admin-display.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see google-adwords-remarketing-admin-display.tpl.php
 * @see theme_google_adwords_remarketing_admin_display()
 */
function theme_google_adwords_remarketing_admin_display_form(&$variables) {
  global $theme_key;

  $form = &$variables['form'];

  $rows = array();
  if (isset($form['campaigns'])) {
    foreach (element_children($form['campaigns']) as $garcid) {
      $element = &$form['campaigns'][$garcid];
      $element['weight']['#attributes']['class'][] = 'weight';
      $row = array();
      $row[] = render($element['info']);
      $row[] = render($element['status']);
      $row[] = render($element['weight']);
      $row[] = render($element['configure']);
      $row[] = render($element['delete']);
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
        'id' => 'program-list-row-'. $garcid,
      );
    }
  }

  $header = array(
    t('Campaign'),
    t('Enabled'),
    t('Weight'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No campaigns are defined.'), 'colspan' => '5'));
  }

  drupal_add_tabledrag('order-campaings', 'order', 'sibling', 'weight');
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'order-campaings',
      'style' => 'width: 100%',
    ),
  ));
  $output .= drupal_render_children($form);
  $output .= theme('pager');
  return $output;
}

/**
 * Menu callback; displays the google_adwords_remarketing configuration form.
 */
function google_adwords_remarketing_admin_configure($form, &$form_state, $garcid = NULL) {
  $custom_theme = variable_get('theme_default', 'garland');

  $form['garcid'] = array(
    '#type' => 'value',
    '#value' => $garcid,
  );

  $edit = db_query("SELECT * FROM {google_adwords_remarketing} WHERE garcid = :garcid", array(
    ':garcid' => $garcid,
  ))->fetchObject();

  $form['info'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Adminstrative Title'),
    '#maxlength' => 64,
    '#description' => ('A brief description of your Campaign. Used on the <a href="/admin/config/services/garc">overview page</a>.'),
    '#default_value' => empty($edit->info) ? '' : $edit->info,
    '#weight' => -18,
    '#required' => TRUE,
  );
  $form['status'] = array(
    '#title' => t('Enabled'),
    '#type' => 'checkbox',
    '#default_value' => !isset($edit->status) ? true : $edit->status,
  );

  $form['theme'] = array(
    '#type' => 'hidden',
    '#default_value' => empty($edit->theme) ? $custom_theme : $edit->theme,
  );

  // Add field for weight.
  // Fixed issue https://drupal.org/node/2223369
  $form['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#maxlength' => 5,
    '#default_value' => empty($edit->weight) ? '0' : $edit->weight,
    '#required' => TRUE,
  );

  $form['conversion_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Conversion ID'),
    '#maxlength' => 15,
    '#default_value' => empty($edit->conversion_id) ? '' : $edit->conversion_id,
    '#required' => TRUE,
  );

  $form['language'] = array(
    '#type' => 'textfield',
    '#title' => t('Conversion Language'),
    '#maxlength' => 15,
    '#size' => 5,
    '#default_value' => empty($edit->language) ? 'en' : $edit->language,
    '#required' => TRUE,
  );

  $form['format'] = array(
    '#type' => 'select',
    '#title' => t('Conversion Format'),
    '#default_value' => empty($edit->format) ? 2 : $edit->format,
    '#options' => array(
      1 => 'Single line',
      2 => 'Two lines',
      3 => 'Not displayed',
    ),
  );

  $form['color'] = array(
    '#type' => 'textfield',
    '#title' => t('Conversion Color'),
    '#default_value' => empty($edit->color) ? 'FFFFFF' : $edit->color,
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Conversion Label'),
    '#default_value' => empty($edit->label) ? '' : $edit->label,
    '#size' => 30,
    '#maxlength' => 255,
  );

  $form['params'] = array(
    '#type' => 'textfield',
    '#title' => t('Conversion Params'),
    '#default_value' => empty($edit->params) ? '' : $edit->params,
    '#size' => 30,
    '#maxlength' => 255,
  );

  $form['remarketing'] = array(
    '#type' => 'textfield',
    '#title' => t('Conversion Remarketing only'),
    '#default_value' => empty($edit->remarketing) ? '' : $edit->remarketing,
    '#size' => 10,
    '#maxlength' => 10,
  );

  $form['ext_js'] = array(
    '#type' => 'textfield',
    '#title' => t('External JavaScript'),
    '#default_value' => empty($edit->ext_js) ? GARC_DEFAULT_EXT_JS : $edit->ext_js,
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  // Role-based visibility settings
  $default_role_options = array();
  $result = db_query("SELECT rid FROM {google_adwords_remarketing_roles} WHERE garcid = :garcid", array(':garcid' =>$garcid));
   foreach ($result as $role) {
    $default_role_options[] = $role->rid;
  }
  $result = db_query('SELECT rid, name FROM {role} ORDER BY name');
  $role_options = array();
  foreach ($result as $role) {
    $role_options[$role->rid] = $role->name;
  }
  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific visibility settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['role_vis_settings']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show campaign for specific roles'),
    '#default_value' => $default_role_options,
    '#options' => $role_options,
    '#description' => t('Show campaign only for the selected role(s). If you select no roles, the campaign will be visible to all users.'),
  );

  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific visibility settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $access = user_access('administer garc');

  if (isset($edit->visibility) && $edit->visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array(
      '#type' => 'value',
      '#value' => 2,
    );
    $form['page_vis_settings']['pages'] = array(
      '#type' => 'value',
      '#value' => isset($edit->pages) ? $edit->pages : '',
    );
  }
  else {
    $options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if (module_exists('php') && $access) {
      $options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show campaign on specific pages'),
      '#options' => $options,
      '#default_value' => isset($edit->visibility) ? $edit->visibility : 0,
    );
    $form['page_vis_settings']['pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => empty($edit->pages) ? '' : $edit->pages,
      '#description' => $description,
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save campaign'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'markup',
    '#markup' => l(t('Cancel'), 'admin/config/services/garc'),
    '#limit_validation_errors' => array(),
  );

  return $form;
}

function google_adwords_remarketing_admin_configure_validate($form, &$form_state) {
  $garc = db_query("SELECT * FROM {google_adwords_remarketing} WHERE garcid != :garcid AND info = :info", array(
    ':garcid' => $form_state['values']['garcid'],
    ':info' => $form_state['values']['info'],
  ))->fetchObject();

  if (empty($form_state['values']['info']) || $garc !== FALSE) {
    form_set_error('info', t('Please ensure that each campaign description is unique.'));
  }
}

function google_adwords_remarketing_admin_configure_submit($form, &$form_state) {
  if (!form_get_errors()) {
    $garc = array (
      'garcid' => $form_state['values']['garcid'],
      'info' => $form_state['values']['info'],
      'status' => $form_state['values']['status'],
      'theme' => $form_state['values']['theme'],
      'weight' => $form_state['values']['weight'],
      'visibility' => $form_state['values']['visibility'],
      'pages' => $form_state['values']['pages'],
      'conversion_id' => $form_state['values']['conversion_id'],
      'language' => $form_state['values']['language'],
      'format' => $form_state['values']['format'],
      'color' => $form_state['values']['color'],
      'label' => $form_state['values']['label'],
      'params' => $form_state['values']['params'],
      'remarketing' => $form_state['values']['remarketing'],
      'ext_js' => $form_state['values']['ext_js'],
    );
    drupal_write_record('google_adwords_remarketing', $garc, 'garcid');
    db_delete('google_adwords_remarketing_roles')
      ->condition('garcid', $form_state['values']['garcid'])
      ->execute();
    foreach (array_filter($form_state['values']['roles']) as $rid) {
      $role = array (
        'garcid' => $form_state['values']['garcid'],
        'rid' => $rid,
      );
      drupal_write_record('google_adwords_remarketing_roles', $role);
    }
    drupal_set_message(t('Configuration for the campaign %name has been saved.', array('%name' => $form_state['values']['info'])));
    cache_clear_all('google_adwords_remarketing_garc_list', 'cache');
    $form_state['redirect'] = 'admin/config/services/garc';
    return;
  }
}

/**
 * Menu callback: display the custom google_adwords_remarketing addition form.
 */
function google_adwords_remarketing_add_campaign_form($form, &$form_state) {
  return google_adwords_remarketing_admin_configure($form, $form_state, NULL);
}

function google_adwords_remarketing_add_campaign_form_validate($form, &$form_state) {
  $garc = db_query("SELECT * FROM {google_adwords_remarketing} WHERE info = :info", array(
    ':info' => $form_state['values']['info'],
  ))->fetchObject();

  if (empty($form_state['values']['info']) || $garc !== FALSE) {
    form_set_error('info', t('Please ensure that each campaign description is unique.'));
  }
}

/**
 * Save the new google_adwords_remarketing campaign aka garc
 */
function google_adwords_remarketing_add_campaign_form_submit($form, &$form_state) {
  $garc = array (
    'info' => $form_state['values']['info'],
    'status' => $form_state['values']['status'],
    'theme' => $form_state['values']['theme'],
    'weight' => $form_state['values']['weight'],
    'visibility' => $form_state['values']['visibility'],
    'pages' => $form_state['values']['pages'],
    'conversion_id' => $form_state['values']['conversion_id'],
    'language' => $form_state['values']['language'],
    'format' => $form_state['values']['format'],
    'color' => $form_state['values']['color'],
    'label' => $form_state['values']['label'],
    'params' => $form_state['values']['params'],
    'remarketing' => $form_state['values']['remarketing'],
    'ext_js' => $form_state['values']['ext_js'],
  );
  if (drupal_write_record('google_adwords_remarketing', $garc) !== FALSE) {
    // @TODO apply multi theme, is the approach i want?
    /*
    foreach (list_themes() as $key => $theme) {
      if ($theme->status) {
        db_query("INSERT INTO {google_adwords_remarketings} (field(s),theme) VALUES(%d)", $theme->name);
      }
    }
    */
    foreach (array_filter($form_state['values']['roles']) as $rid) {
      $role = array (
        'garcid' => $garc['garcid'],
        'rid' => $rid,
      );
      drupal_write_record('google_adwords_remarketing_roles', $role);
    }
    drupal_set_message(t('A campaign has been created.'));
    cache_clear_all('google_adwords_remarketing_garc_list', 'cache');
  }

  $form_state['redirect'] = 'admin/config/services/garc';
  return;
}

/**
 * Menu callback; confirm deletion of a garc record.
 */
function google_adwords_remarketing_campaign_delete($form, &$form_state, $garcid = 0) {
  $garc = google_adwords_remarketing_campaign_get($garcid);
  $form['info'] = array(
    '#type' => 'hidden',
    '#value' => $garc->info,
  );
  $form['garcid'] = array(
    '#type' => 'hidden',
    '#value' => $garcid,
  );
  return confirm_form($form, t('Are you sure you want to delete campaign %name?', array('%name' => $garc->info)), 'admin/config/services/garc', '', t('Delete'), t('Cancel'));
}

/**
 * Deletion of a garc record
 */
function google_adwords_remarketing_campaign_delete_submit($form, &$form_state) {
  db_delete('google_adwords_remarketing')
    ->condition('garcid', $form_state['values']['garcid'])
    ->execute();
  db_delete('google_adwords_remarketing_roles')
    ->condition('garcid', $form_state['values']['garcid'])
    ->execute();
  drupal_set_message(t('Campaign %name has been removed.', array('%name' => $form_state['values']['info'])));
  cache_clear_all('google_adwords_remarketing_garc_list', 'cache');
  $form_state['redirect'] = 'admin/config/services/garc';
  return;
}

/**
 * Helper function for sorting garcs on admin/config/services/garc.
 *
 * Active GARCS are sorted by status, then by weight.
 * Disabled garcs are sorted by name.
 */
function _garc_compare($a, $b) {
  // Separate enabled from disabled.
  $status = $b->status - $a->status;
  if ($status) {
    return $status;
  }
  // Sort by weight.
  $weight = $a->weight - $b->weight;
  if ($weight) {
    return $weight;
  }
  // Sort by title.
  return strcmp($a->info, $b->info);
}
