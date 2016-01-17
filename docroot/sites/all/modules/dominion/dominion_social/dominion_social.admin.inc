<?php

function dominion_social_admin_settings_page() {
  $output = '';

  $header = array(
    t('Icon'),
    t('Name'),
    t('Blueprints'),
    t('Operations'),
  );
  $rows = array();
  
  $query = db_select('dominion_social_site', 'ss');
  $fm = $query->leftJoin('file_managed', 'fm', 'ss.icon_fid = fm.fid');
  $query->fields('ss')->addField($fm, 'uri', 'icon_uri');
  $query->orderBy('ss.weight', 'asc');
  $sites = $query->execute()->fetchAll();
  foreach ($sites as $site) {
    $query = db_select('dominion_social_site_blueprint', 'ssb');
    $b = $query->join('dominion_blueprint', 'b', 'ssb.blueprint = b.blueprint');
    $blueprints = $query->fields($b, array('name'))
      ->condition('ssb.sid', $site->sid)
      ->orderBy("$b.is_default", 'desc')
      ->orderBy("$b.name", 'asc')
      ->execute()
      ->fetchCol();
    $edit = l(t('edit'), 'admin/structure/dominion/settings/social/edit/' . $site->sid);
    $delete = l(t('delete'), 'admin/structure/dominion/settings/social/delete/' . $site->sid);
    $rows[] = array(
      $site->icon_uri ? theme('image', array('path' => $site->icon_uri)) : t('none'),
      check_plain($site->name),
      $blueprints ? theme('item_list', array('items' => $blueprints)) : 'none',
      "$edit $delete",
    );
  }

  if ($rows) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('There are no sites created yet.') . '</p>';
  }

  return $output;
}

function dominion_social_site_form($form, &$form_state, $sid = NULL) {
  $site = new stdClass();
  if ($sid) {
    $query = db_select('dominion_social_site', 'ss');
    $fm = $query->leftJoin('file_managed', 'fm', 'ss.icon_fid = fm.fid');
    $query->fields('ss')->addField($fm, 'uri', 'icon_uri');
    $query->condition('ss.sid', $sid);
    $query->orderBy('ss.weight', 'asc');
    $site = $query->execute()->fetchObject();
    if (!$site) {
      drupal_not_found();
      exit;
    }
    $form['#sid'] = $sid;
    $form['#site'] = $site;
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $sid ? $site->name : '',
    '#required' => TRUE,
  );

  if ($sid && $site->icon_fid) {
    $form['icon'] = array(
      '#type' => 'fieldset',
      '#title' => t('Icon'),
      '#description' => t('There is an icon uploaded already. You may upload a new icon.'),
    );
    
    $form['icon']['current'] = array(
      '#markup' => theme('image', array('path' => $site->icon_uri)),
    );

    $form['icon']['icon_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete icon'),
    );

    $form['icon']['icon'] = array(
      '#type' => 'file',
      '#name' => 'files[icon]',
      '#title' => t('New icon'),
    );
  }
  else {
    $form['icon'] = array(
      '#type' => 'file',
      '#name' => 'files[icon]',
      '#title' => t('Icon'),
    );
  }

  if ($sid) {
    $form['weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Weight'),
      '#default_value' => $site ? $site->weight : 0,
      '#required' => TRUE,
      '#size' => 10,
    );
  }
  
  $form['inherit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inherit links'),
    '#default_value' => $sid ? $site->inherit : FALSE,
    '#description' => t('Inherit links from parent subsite if no link is provided.'),
  );
  
  $blueprints = db_select('dominion_blueprint', 'b')
    ->fields('b', array('blueprint', 'name'))
    ->orderBy('b.is_default', 'desc')
    ->orderBy('b.name', 'asc')
    ->execute()
    ->fetchAllKeyed();
  if ($sid) {
    $default = db_select('dominion_social_site_blueprint', 'ssb')
      ->fields('ssb', array('blueprint'))
      ->condition('ssb.sid', $sid)
      ->execute()
      ->fetchCol();
  }
  else {
    $default = db_select('dominion_blueprint', 'b')
      ->fields('b', array('blueprint'))
      ->condition('b.is_default', 1)
      ->execute()
      ->fetchCol();
  }
  $form['blueprints'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Blueprints'),
    '#options' => $blueprints,
    '#default_value' => drupal_map_assoc($default),
    '#description' => t('Check the blueprints on which this social site should be enabled.'),
  );

  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#attributes'] = array('enctype' => 'multipart/form-data');

  return $form;
}

function dominion_social_site_form_validate($form, &$form_state) {
  if (isset($form_state['values']['weight']) && !is_numeric($form_state['values']['weight'])) {
    form_set_error('weight', t('Weight must be numeric'));
  }
}

function dominion_social_site_form_submit($form, &$form_state) {
  $validators = array(
   'file_validate_is_image' => array(),
  );
  $file = file_save_upload('icon', $validators);
  if ($file) {
    $file = file_move($file, 'public://' . $file->filename, FILE_EXISTS_RENAME);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
  }
  if (!empty($form_state['values']['icon_delete'])) {
    if ($old = file_load($form['#site']->icon_fid)) {
      file_delete($old);
    }
  }
  
  if (isset($form_state['values']['weight'])) {
    $weight = $form_state['values']['weight'];
  }
  else {
    $sql = 'SELECT MAX(weight) FROM {dominion_social_site}';
    $weight = ((int) db_query($sql)->fetchField()) + 1;
  }

  $site = new stdClass();
  if (!empty($form['#sid'])) {
    $site->sid = $form['#sid'];
    db_delete('dominion_social_site_blueprint')->condition('sid', $site->sid)->execute();
  }
  $site->icon_fid = $file ? $file->fid : NULL;
  $site->name = $form_state['values']['name'];
  $site->inherit = !empty($form_state['values']['inherit']);
  $site->weight = $weight;
  drupal_write_record('dominion_social_site', $site, empty($site->sid) ? array() : array('sid'));

  foreach ($form_state['values']['blueprints'] as $blueprint => $checked) {
    if ($checked) {
      db_insert('dominion_social_site_blueprint')->fields(array(
        'sid' => $site->sid,
        'blueprint' => $blueprint,
      ))->execute();
    }
  }

  $form_state['redirect'] = 'admin/structure/dominion/settings/social';
}

function dominion_social_delete_form($form, &$form_state, $sid) {
  $sql = 'SELECT dss.sid, dss.name, dss.icon_fid, f.uri as icon_uri
  FROM {dominion_social_site} dss
  LEFT JOIN {file_managed} f ON dss.icon_fid = f.fid
  WHERE dss.sid = :sid';
  $res = db_query($sql, array(':sid' => $sid));
  if (!$site = $res->fetchObject()) {
    drupal_not_found();
    module_invoke_all('exit') & exit;
  }

  $form['#sid'] = $sid;
  $form['#icon_fid'] = $site->icon_fid;
  $form['#site'] = $site;

  $form['info'] = array(
    '#markup' => '<p>' . t('Are you sure you want to delete %site?', array('%site' => $site->name)) . '</p>',
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  $form['cancel'] = array(
    '#markup' => l(t('cancel'), 'admin/structure/dominion/settings/social'),
  );

  return $form;
}

function dominion_social_delete_form_submit($form, &$form_state) {
  if (!empty($form['#icon_fid'])) {
    $file = file_load($form['#icon_fid']);
    file_delete($file);
  }
  db_delete('dominion_social_site')->condition('sid', $form['#sid'])->execute();
  db_delete('dominion_social_site_blueprint')->condition('sid', $form['#sid'])->execute();
  drupal_set_message(t('Site %name has been deleted.', array('%name' => $form['#site']->name)));
  $form_state['redirect'] = 'admin/structure/dominion/settings/social';
}
