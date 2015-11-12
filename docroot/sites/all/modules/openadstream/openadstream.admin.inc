<?php
/**
 * @file
 * Admin features for openadstream module.
 */

/**
 * Main settings page.
 */
function openadstream_admin_settings() {
  $form = array();

  $form['main_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("These settings should be provided by 24/7 RealMedia."),
  );

  $form['main_settings']['openadstream_oas_hostname'] = array(
    '#type' => 'textfield',
    '#title' => t('Open AdStream Hostname'),
    '#default_value' => variable_get('openadstream_oas_hostname', 'oascentral.247realmedia.com'),
    '#size' => 50,
    '#maxlength' => 100,
    '#description' => t('The hostname used to request ads, will be provided by 24/7 Real Media.  Do not include the initial "http://" nor any slashes, just the bare hostname.'),
    '#required' => TRUE,
  );

  $form['main_settings']['openadstream_hostname'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Hostname'),
    '#default_value' => variable_get('openadstream_hostname', ''),
    '#size' => 50,
    '#maxlength' => 100,
    '#description' => t('By default, we will be using the current hostname'),
  );

  // @see http://onlinehelp.247media.com/xaxis_publisher14/Default.htm#Ad_Tagging/Tag_Type_Comparison/Tag_Type_Comparison.htm
  $form['main_settings']['openadstream_tag_type'] = array(
    '#type' => 'select',
    '#title' => t('Tag type'),
    '#options' => array(
      'mjx' => t('MJX'),
      'dx' => t('DX'),
      // 'rx' => t('RX'),
      // 'nx' => t('NX'),
    ),
    '#default_value' => variable_get('openadstream_tag_type', 'mjx'),
    '#description' => t('Select the 24/7 Real Media tag type that you want to use; the module may present different options depending on the value selected.'),
  );

  // Optional $_GET variable to override the pagename.
  $form['pagename_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pagename Settings'),
    '#description' => t('The pagename settings are all optional.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['pagename_settings']['openadstream_pagename'] = array(
    '#type' => 'textfield',
    '#title' => t('Fixed Page Name'),
    '#default_value' => variable_get('openadstream_pagename', ''),
    '#size' => 50,
    '#maxlength' => 50,
    '#description' => t('All page requests will load the same pagename value. Only use this for testing, will produce unwanted results in a production environment.'),
  );
  $form['pagename_settings']['openadstream_add_basepath'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Base Path'),
    '#default_value' => variable_get('openadstream_add_basepath', TRUE),
    '#description' => t('By default the pagename variable will be prepended
      by the string provided by base_path().  In occasions when you are
      running the site within a subdirectory but do not want that directory
      name added, disable this setting.'),
  );
  $form['pagename_settings']['openadstream_querystring'] = array(
    '#type' => 'textfield',
    '#title' => t('URL String'),
    '#default_value' => variable_get('openadstream_querystring', ''),
    '#size' => 10,
    '#maxlength' => 20,
    '#description' => t('Override the configured pagename via a URL string.'),
  );

  return system_settings_form($form);
}

/**
 * Positions setting page.
 */
function openadstream_admin_positions() {

  $positions = _openadstream_positions();

  $rows = array();
  if (!empty($positions)) {
    foreach ($positions as $position) {
      $rows[] = array(
        $position['name'],
        $position['description'],
        theme('links', array(
          'attributes' => array(
            'class' => array('inline'),
          ),
          'links' => array(
            'edit' => array(
              'title' => 'edit',
              'href' => 'admin/config/services/openadstream/positions/edit/' . $position['position_id'],
            ),
            'delete' => array(
              'title' => 'delete',
              'href' => 'admin/config/services/openadstream/positions/delete/' . $position['position_id'],
            ),
          ),
        )),
      );
    }
  }

  return array(
    '#theme' => 'table',
    '#prefix' => '<p>' . t('Positions are created in the Open AdStream management interface. 24/7 Real Media will provide the names to you.') . '</p>',
    '#header' => array(
      t('Position Name'),
      t('Description'),
      t('Operations'),
    ),
    '#rows' => $rows,
  );
}
/**
 * Add/Edit form for OAS positions.
 */
function openadstream_admin_form_positions($form, &$form_state, $position_id) {
  $openadstream_tag_type = variable_get('openadstream_tag_type', 'mjx');

  $position = array(
    'position_id' => NULL,
    'name' => NULL,
    'description' => NULL,
  );

  if ($openadstream_tag_type == 'dx') {
    $position['dx']['height'] = NULL;
    $position['dx']['width'] = NULL;
  }

  if (!empty($position_id)) {
    $position = db_query("SELECT * FROM {openadstream_positions} WHERE position_id = :pid", array(':pid' => $position_id))->fetchAssoc();
    $data = unserialize($position['data']);
    if ($openadstream_tag_type == 'dx') {
      $position['dx']['height'] = (!empty($data['dx']['height']) ? $data['dx']['height'] : NULL);
      $position['dx']['width'] = (!empty($data['dx']['width']) ? $data['dx']['width'] : NULL);
    }
  }

  $form = array();

  $form['position'] = array(
    '#type' => 'fieldset',
    '#title' => t('Position Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['position']['position_id'] = array(
    '#type' => 'value',
    '#value' => $position['position_id'],
  );

  $form['position']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Position Name'),
    '#description' => t('The position name is set in the OAS management interface.  24/7 RealMedia will provide this value.'),
    '#size' => 30,
    '#maxlength' => 75,
    '#required' => TRUE,
    '#default_value' => $position['name'],
  );

  $form['position']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('An optional description to help remember the details of the advertisement position.'),
    '#size' => 30,
    '#maxlength' => 100,
    '#required' => FALSE,
    '#default_value' => $position['description'],
  );

  if ($openadstream_tag_type == 'dx') {
    $form['position']['dimensions'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('container-inline'),
      ),
    );

    $form['position']['dimensions']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#size' => 4,
      '#maxlength' => 4,
      '#required' => TRUE,
      '#default_value' => $position['dx']['width'],
      '#suffix' => 'px by',
    );

    $form['position']['dimensions']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#size' => 4,
      '#maxlength' => 4,
      '#required' => TRUE,
      '#default_value' => $position['dx']['height'],
      '#suffix' => 'px',
    );
  }

  $form['#validate'][] = 'openadstream_admin_form_positions_validate';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}
/**
 * Validation of Add/Edit form for OAS positions.
 */
function openadstream_admin_form_positions_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (empty($values['position_id'])) {
    $exists = db_select('openadstream_positions', 'oasp')
      ->fields('oasp', array(
        'position_id',
      ))
      ->condition('oasp.name', $values['name'])
      ->execute()
      ->fetchCol();

    if (!empty($exists)) {
      form_set_error('name', t('The position name %name is already in use.', array('%name' => $values['name'])));
    }
  }
}
/**
 * Persistence for Add/Edit form for OAS positions.
 */
function openadstream_admin_form_positions_submit($form, &$form_state) {

  $errors = form_get_errors();
  if (!empty($errors)) {
    return;
  }

  $values = $form_state['values'];
  $mydata = array();
  $fields = array(
    'name' => $values['name'],
    'description' => $values['description'],
  );

  $openadstream_tag_type = variable_get('openadstream_tag_type', 'mjx');
  if ($openadstream_tag_type == 'dx') {
    $mydata['dx']['height'] = ($values['height']) ? $values['height'] : NULL;
    $mydata['dx']['width'] = ($values['width']) ? $values['width'] : NULL;
  }

  $action = 'created';

  if (!empty($values['position_id'])) {
    $action = 'updated';

    $position = db_query("SELECT * FROM {openadstream_positions} WHERE position_id = :pid", array(':pid' => $values['position_id']))->fetchAssoc();
    $data = unserialize($position['data']);
    $data = (is_array($data)) ? array_merge($data, $mydata) : $mydata;
    $fields['data'] = serialize($data);

    db_update('openadstream_positions')
      ->fields($fields)
      ->condition('position_id', $values['position_id'])
      ->execute();
  }
  else {
    $fields['data'] = serialize($mydata);
    db_insert('openadstream_positions')
      ->fields($fields)
      ->execute();
  }

  drupal_set_message(t('The position %name has been @action.', array(
    '%name' => $values['name'],
    '@action' => $action,
  )));

  $form_state['redirect'] = 'admin/config/services/openadstream/positions';
}
/**
 * TODO.
 */
function openadstream_admin_form_positions_delete($form, &$form_state, $position_id) {
  $position = db_query("SELECT * FROM {openadstream_positions} WHERE position_id = :pid", array(':pid' => $position_id))->fetchAssoc();

  drupal_set_title(t("Position '@name'", array('@name' => $position['name'])), PASS_THROUGH);

  $form = array(
    'position_id' => array(
      '#type' => 'value',
      '#value' => $position_id,
    ),
  );

  return confirm_form($form,
    t("Are you sure you want to delete position '%name'?", array('%name' => $position['name'])),
    'admin/config/services/openadstream/positions',
    t('This action cannot be undone'),
    t('Delete'), t('Cancel')
  );
}
/**
 * TODO.
 */
function openadstream_admin_form_positions_delete_submit($formid, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_delete('openadstream_positions')
      ->condition('position_id', $form_state['values']['position_id'])
      ->execute();

    drupal_set_message(t('The position has been deleted.'));
  }
  $form_state['redirect'] = 'admin/config/services/openadstream/positions';
  _block_rehash();
}

/**
 * Pagenames settings page.
 */
function openadstream_admin_pagenames() {

  // Compile the custom pagenames.
  $pagenames = _openadstream_pagenames();

  $rows = array();
  if (!empty($pagenames)) {
    foreach ($pagenames as $pagename) {
      $rows[] = array(
        $pagename['pagename'],
        $pagename['weight'],
        htmlspecialchars($pagename['path']),
        theme('links', array(
          'attributes' => array(
            'class' => array('inline'),
          ),
          'links' => array(
            'edit' => array(
              'title' => 'edit',
              'href' => 'admin/config/services/openadstream/pagenames/edit/' . $pagename['pagename_id'],
            ),
            'delete' => array(
              'title' => 'delete',
              'href' => 'admin/config/services/openadstream/pagenames/delete/' . $pagename['pagename_id'],
            ),
          ),
        )),
      );
    }
  }

  return array(
    '#theme' => 'table',
    '#prefix' => '<p>' . t('Override the Open AdStream pagename variable based on the page path.') . '</p>',
    '#header' => array(
      t('Pagename'),
      t('Weight'),
      t('Path'),
      t('Operations'),
    ),
    '#rows' => $rows,
  );
}
/**
 * TODO.
 */
function openadstream_admin_form_pagenames($form, &$form_state, $pagename_id) {

  $pagename = array(
    'pagename_id' => NULL,
    'path' => NULL,
    'pagename' => NULL,
    'weight' => NULL,
  );

  if (!empty($pagename_id)) {
    $pagename = db_query("SELECT * FROM {openadstream_pagenames} WHERE pagename_id = :pid", array(':pid' => $pagename_id))->fetchAssoc();
  }

  $form = array();

  $form['pagename'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pagename Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['pagename']['pagename_id'] = array(
    '#type' => 'value',
    '#value' => $pagename['pagename_id'],
  );

  $form['pagename']['pagename'] = array(
    '#type' => 'textfield',
    '#title' => t('Pagename'),
    '#description' => t('The pagename will be overridden with this value when viewing a page with the specified path.'),
    '#size' => 30,
    '#maxlength' => 75,
    '#required' => TRUE,
    '#default_value' => $pagename['pagename'],
  );

  $form['pagename']['weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#description' => t('Pagenames are applied by order of weight. If a path matches multiple pagename entries then the entry with the highest weight wins.'),
    '#default_value' => $pagename['weight'],
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
  );

  $form['pagename']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t("Enter a drupal path. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog. &lt;front&gt; is the front page."),
    '#size' => 30,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $pagename['path'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}
/**
 * TODO.
 */
function openadstream_admin_form_pagenames_submit($form, &$form_state) {

  $errors = form_get_errors();
  if (!empty($errors)) {
    return;
  }

  $values = $form_state['values'];
  $action = 'created';

  if (!empty($values['pagename_id'])) {
    $action = 'updated';

    db_update('openadstream_pagenames')
      ->fields(array(
        'pagename' => $values['pagename'],
        'weight' => $values['weight'],
        'path' => $values['path'],
      ))
      ->condition('pagename_id', $values['pagename_id'])
      ->execute();
  }
  else {
    db_insert('openadstream_pagenames')
      ->fields(array(
        'pagename' => $values['pagename'],
        'weight' => $values['weight'],
        'path' => $values['path'],
      ))
      ->execute();
  }

  drupal_set_message(t('The pagename %name has been @action.', array(
    '%name' => $values['pagename'],
    '@action' => $action,
  )));

  $form_state['redirect'] = 'admin/config/services/openadstream/pagenames';
}
/**
 * TODO.
 */
function openadstream_admin_form_pagenames_delete($form, &$form_state, $pagename_id) {
  $pagename = db_query("SELECT * FROM {openadstream_pagenames} WHERE pagename_id = :pid", array(':pid' => $pagename_id))->fetchAssoc();

  drupal_set_title(t("Pagename '@name'", array('@name' => $pagename['pagename'])), PASS_THROUGH);

  $form = array(
    'pagename_id' => array(
      '#type' => 'value',
      '#value' => $pagename_id,
    ),
  );

  return confirm_form($form,
    t("Are you sure you want to delete pagename '%name'?", array('%name' => $pagename['pagename'])),
    'admin/config/services/openadstream/pagenames',
    t('This action cannot be undone'),
    t('Delete'), t('Cancel')
  );
}
/**
 * TODO.
 */
function openadstream_admin_form_pagenames_delete_submit($formid, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_delete('openadstream_pagenames')
      ->condition('pagename_id', $form_state['values']['pagename_id'])
      ->execute();

    drupal_set_message(t('The pagename has been deleted.'));
  }
  $form_state['redirect'] = 'admin/config/services/openadstream/pagenames';
}
