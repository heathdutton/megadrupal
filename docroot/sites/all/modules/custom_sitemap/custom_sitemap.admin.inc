<?php

/**
 * @file
 * Custom sitemap modules administrative page callbacks.
 */

/**
 * List all sitemap files uploaded with respect to each domain available.
 */
function custom_sitemap_files_list_form() {
  $options = array();
  $custom_sitemap_domains = _custom_sitemap_get_available_domains();

  if (count($custom_sitemap_domains) >= 1) {
    foreach ($custom_sitemap_domains as $key => $value) {
      foreach ($value as $index => $domain) {
        if ($index == 'n') {
          $clip = 'n_' . $key . '-' . $domain;
          $options[$clip] = t($domain);
        }
        else {
          $clip = $index . '-' . $domain;
          $options[$clip] = t($domain);
        }
      }
    }
  }
  $form['operations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload Sitemap'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  if (count($custom_sitemap_domains) >= 1) {
    $form['operations']['operation'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => '0',
    );
    $form['operations']['file'] = array(
      '#type' => 'file',
      '#title' => t('Sitemap'),
      '#description' => t('Allowed extension: xml'),
    );
    $form['operations']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Upload'),
    );
  }
  else {
    $form['operations']['operation'] = array(
      '#type' => 'item',
      '#title' => t("No domain/subdomain to add sitemap."),
    );
  }
  $domain_used = db_query("SELECT COUNT(name) FROM {system} WHERE type = :module 
  AND name = :name AND status = :status",
  array(':module' => 'module', ':name' => 'domain', ':status' => 1))->fetchField();
  $header = array();
  $header['sno'] = t('#');
  $header['domain'] = array('data' => t('Domain/Subdomain'), 'field' => 'domain');
  $header['file'] = array('data' => t('File'), 'field' => 'relative_filename');
  $header['uploadedon'] = array(
    'data' => t('Uploaded On'),
    'field' => 'uploaded_on',
    'sort' => 'asc',
  );
  $header['operations'] = array('data' => t('Operations'));
  $sitemap_added = db_query("SELECT COUNT(id) FROM {custom_sitemap}")->fetchField();
  if ($sitemap_added) {
    $query = db_select('custom_sitemap');
    $query->fields('custom_sitemap',
      array(
        'id',
        'domain',
        'relative_filename',
        'filename',
        'uploaded_on',
        'subdomain_id',
      )
    );
    $query->extend('TableSort')->orderByHeader($header);
    if (!$domain_used) {
      $query->condition('subdomain_id', 0);
    }
    $res = $query->execute();
    $option = array();
    $cnt = 0;
    while ($obj = $res->fetchObject()) {
      $counter = $obj->id;
      $rs_domain = $obj->domain;
      $rs_file = $obj->relative_filename;
      $rs_file = l(t($rs_file), 'admin/config/search/custom-sitemap/' . $counter . '/preview',
        array(
          'html' => TRUE,
          'attributes' => array(
            'target' => '_blank',
            'title' => t('preview sitemap'),
          ),
        )
      );
      $up_on = $obj->uploaded_on ? format_date($obj->uploaded_on, 'short') : '-';
      $operations = array();
      $operations['delete'] = custom_sitemap_operation_link('admin/config/search/custom-sitemap/' . $counter . '/delete/', array('title' => t('Delete'), 'modal' => TRUE));
      $option[$counter] = array(
        'sno' => ++$cnt,
        'domain' => t($rs_domain),
        'file' => t($rs_file),
        'uploadedon' => $up_on,
        'operations' => array(
          'data' => array(
            '#theme' => 'links',
            '#links' => $operations,
            '#attributes' => array('class' => array('links', 'inline')),
          ),
        ),
      );
    }
    $form['sitemap_files'] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom Sitemap List'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['sitemap_files']['custom_sitemaps'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $option,
    );
  }
  else {
    $form['sitemap_files'] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom Sitemap List'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['sitemap_files']['custom_sitemaps'] = array(
      '#type' => 'item',
      '#title' => t('No custom sitemap uploaded.'),
    );
  }
  return $form;
}

/**
 * Validate custom_sitemap_files_list_form form submission.
 */
function custom_sitemap_files_list_form_validate($form, &$form_state) {
  if (!_custom_sitemap_drupal_is_writable(drupal_realpath(file_build_uri('custom_sitemap')))) {
    if (drupal_rmdir(file_build_uri('custom_sitemap'))) {
      drupal_mkdir(file_build_uri('custom_sitemap'));
      drupal_chmod(file_build_uri('custom_sitemap'), 0777);
    }
    else {
      form_set_error('', t('Please ensure that custom_sitemap folder is writable.'));
    }
  }
  db_delete('file_managed')
  ->condition('uri', array('%/custom_sitemap/%'), 'LIKE')
  ->execute();
  $file = file_save_upload('file', array(
    'file_validate_extensions' => array('xml'),
  ));
  $folder = custom_sitemap_get_folder();
  if ($file) {
    if ($file = file_move($file, $folder)) {
      $form_state['values']['file'] = $file;
    }
    else {
      form_set_error('file', t('Failed to write the uploaded file to sites file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file uploaded.'));
  }
}

/**
 * Process custom_sitemap_files_list_form form submission.
 */
function custom_sitemap_files_list_form_submit($form, &$form_state) {
  global $user;
  if ($form_state['values']['file']->filename != '') {
    $original_file_name = $form_state['values']['file']->filename;
    $fid = $form_state['values']['file']->fid;
    $tmp_domain = $form_state['values']['operation'];
    $cs_tmp = explode('-', $tmp_domain);
    $domain = $cs_tmp[1];
    $sub_domain_id = 0;
    if (strpos($tmp_domain, 'n_') === FALSE) {
      $sub_domain_id = explode('-', $form_state['values']['operation']);
      $tmp = $sub_domain_id[1];
      $sub_domain_id = db_select('domain')
        ->fields('domain', array('domain_id'))
        ->condition('subdomain', "$tmp")
        ->execute()
        ->fetchField();
    }
    $move_file_to_this_dir = file_build_uri('custom_sitemap');
    $tmp_fn = explode('.', $original_file_name);
    $original_filename = $tmp_fn[0];
    $original_file_extension = $tmp_fn[1];
    $new_file_name = $original_filename . ".$original_file_extension";
    $destination_of_file_with_name = $move_file_to_this_dir . '/' . $original_filename . ".$original_file_extension";
    $i = 0;
    while (file_exists($destination_of_file_with_name)) {
      $new_file_name = $original_filename . "_$i" . ".$original_file_extension";
      $fname_without_extension = $original_filename . "_$i";
      $i++;
      $destination_of_file_with_name = $move_file_to_this_dir . '/' . $new_file_name;
    }
    $file_source = $form_state['values']['file']->uri;
    $file = (object) array(
      'uid' => $user->uid,
      'uri' => $file_source,
      'filemime' => file_get_mimetype($file_source),
      'status' => 1,
    );
    if (file_copy($file, $destination_of_file_with_name)) {
      drupal_unlink($file_source);
      db_delete('file_managed')
      ->condition('fid', $fid)
      ->execute();
      $now = time();
      db_insert('custom_sitemap')
      ->fields(array(
        'domain' => $domain,
        'filename' => $new_file_name,
        'relative_filename' => $original_file_name,
        'uploaded_on' => $now,
        'subdomain_id' => $sub_domain_id,
      ))
      ->execute();
      drupal_set_message(t("Sitemap added successfully."));
    }
    else {
      drual_set_message(t("Failed to upload file."), 'error');
    }
  }
  else {
    drupal_set_message(t("Select a file to upload."), 'error');
    drupal_goto(drupal_get_destination());
  }
}

/**
 * Deletes custom sitemap file.
 */
function custom_sitemap_delete() {
  $cs_id = arg(4);
  $sitemap_exists = db_query("SELECT COUNT(id) FROM {custom_sitemap} WHERE id = :id", array(':id' => $cs_id))->fetchField();
  if ($sitemap_exists) {
    $rs = db_select('custom_sitemap')
      ->fields('custom_sitemap', array('filename', 'domain'))
      ->condition('id', $cs_id)
      ->execute();
    $res = $rs->fetchObject();
    $filename = $res->filename;
    $domain = $res->domain;
    $pth = drupal_realpath(custom_sitemap_get_folder()) . '/' . $filename;
    drupal_unlink($pth);
    db_delete('custom_sitemap')
      ->condition('id', $cs_id)
      ->execute();
    drupal_set_message(t("Sitemap for domain %domain deleted.", array('%domain' => $domain)));
  }
  else {
    drupal_set_message(t("Sitemap doesn't exists"));
  }
  drupal_goto($_SERVER['HTTP_REFERER']);
}

/**
 * Preview custom sitemap.
 */
function custom_sitemap_preview() {
  header('Content-type: text/xml');
  $cs_id = arg(4);
  $sitemap_exists = db_query("SELECT COUNT(id) FROM {custom_sitemap} WHERE id = :id", array(':id' => $cs_id))->fetchField();
  if ($sitemap_exists) {
    $rs = db_select('custom_sitemap')
    ->fields('custom_sitemap', array('filename', 'domain'))
    ->condition('id', $cs_id)
    ->execute();
    $res = $rs->fetchObject();
    $filename = $res->filename;
    $domain = $res->domain;
    $file_uri = drupal_realpath(file_build_uri('custom_sitemap') . '/' . $filename);
    echo file_get_contents($file_uri);
  }
}
