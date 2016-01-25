<?php

/**
 * @file
 * Important administration functions for the exchange links module.
 */

/**
 * Implements of exchange_links_settings_form()
 */
function exchange_links_settings_form() {
  $form['rules'] = array(
    '#type' => 'fieldset',
    '#title' => 'Rules',
    '#collapsible' => FALSE,
  );
  $form['rules']['exchange_links_rules_desc'] = array(
    '#type' => 'textarea',
    '#title' => 'Rule',
    '#description' => 'Enter your link sharing rules.',
    '#required' => TRUE,
    '#default_value' => variable_get('exchange_links_rules_desc', 'Drupal7 related websites only! (Other none Drupal7 related websites will be removed) We are constantly looking for new perm Drupal7 link exchanges.'),
  );
  $form['rules']['exchange_links_rules_pagerank'] = array(
    '#type' => 'textfield',
    '#title' => 'Min. PageRank&trade;',
    '#required' => TRUE,
    '#description' => 'Enter Min. PageRank&trade; value here.',
    '#default_value' => variable_get('exchange_links_rules_pagerank', 5),
  );
  $form['your_linking_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'Your Linking details',
    '#collapsible' => FALSE,
  );
  $form['your_linking_details']['exchange_links_url'] = array(
    '#type' => 'textfield',
    '#title' => 'URL',
    '#required' => TRUE,
    '#description' => 'It should be like http://www.yoursite.com',
    '#default_value' => variable_get('exchange_links_url', 'http://www.yoursite.com'),
  );
  $form['your_linking_details']['exchange_links_site_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Site Title',
    '#required' => TRUE,
    '#description' => 'Enter your site title here.',
    '#default_value' => variable_get('exchange_links_site_title', 'Site Title.'),
  );
  $form['your_linking_details']['exchange_links_site_desc'] = array(
    '#type' => 'textarea',
    '#title' => 'Site Description',
    '#description' => 'Enter site descripton.',
    '#required' => TRUE,
    '#default_value' => variable_get('exchange_links_site_desc', 'Site Description.'),
  );
  $form['your_linking_details']['exchange_links_html_code'] = array(
    '#type' => 'textfield',
    '#title' => 'HTML Code',
    '#required' => TRUE,
    '#description' => 'Enter HTML code here. Example: &amp;lt;a href="http://www.yoursite.com" title="Site Title"&amp;gt;Site Title&amplt;/a&amp;gt;',
    '#default_value' => variable_get('exchange_links_html_code', '&lt;a href="http://www.yoursite.com" title="Site Title"&gt;Site Title&lt;/a&gt;'),
  );
  return system_settings_form($form); 
}
 
/**
 * Implementation of admin_links_form()
 */
function admin_links_form() {
  $vocabulary = taxonomy_vocabulary_machine_name_load('exchange_links');
  $categories = taxonomy_get_tree($vocabulary->vid);
  $link_category = array();
  foreach ($categories as $category) {
    $link_category[$category->tid] = $category->name;
  }
  $form['your_linking_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'Linking details',
    '#collapsible' => FALSE,
  );
  $form['your_linking_details']['site_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Site title',
    '#description' => 'Enter site title. Example: Video Sharing Platform',
    '#required' => TRUE,
  );
  $form['your_linking_details']['your_url'] = array(
    '#type' => 'textfield',
    '#title' => 'Your URL',
    '#description' => 'Enter your site URL. Example: http://www.example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['reciprocal_link'] = array(
    '#type' => 'textfield',
    '#title' => 'Reciprocal link URL',
    '#description' => 'Enter reciprocal link. Example: http://www.example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Your email',
    '#description' => 'Enter you email address. Example: username@example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['site_desc'] = array(
    '#type' => 'textarea',
    '#title' => 'Site description',
    '#description' => 'Enter site description.',
    '#required' => TRUE,
  );
  $form['your_linking_details']['category'] = array(
    '#type' => 'select',
    '#title' => 'Category',
    '#options' => $link_category,
    '#description' => 'Select link category.',
    '#required' => TRUE,
  );
  $form['add_link'] = array(
    '#type' => 'submit',
    '#value' => 'Add my link',
  );
  return $form;
}
/**
 * Implementation of admin_links_form_validate($form, &$form_state)
 */
function admin_links_form_validate($form, &$form_state) {
  if (!valid_url($form_state['values']['reciprocal_link'], TRUE)) {
    form_set_error('reciprocal_link', 'Please enter valid reciprocal link.');
  }
  elseif (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', 'Please enter a valid email address.');
  }
}
/**
 * Implementation of admin_links_form_submit($form, &$form_state)
 */
function admin_links_form_submit($form, &$form_state) {
  $site_title = $form_state['values']['site_title'];
  $site_url = $form_state['values']['your_url'];
  $reciprocal_url = $form_state['values']['reciprocal_link'];
  $email = $form_state['values']['email'];
  $description = $form_state['values']['site_desc'];
  $category = $form_state['values']['category'];
  $fields = array('id' => NULL, 'site_title' => $site_title, 'site_url' => $site_url, 'reciprocal_url' => $reciprocal_url, 'email' => $email, 'description' => $description, 'category' => $category, 'status' => 1);
  $flag = db_insert('exchange_links')->fields($fields)->execute();
  if ($flag) {
    drupal_set_message(t('You link has been submitted successfully.'), 'status');
  }
}
 
/**
 * Implementation of exchange_links_overview()
 */
function exchange_links_overview() {
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'el.site_title', 'sort' => 'asc'),
    'site_url' => array('data' => t('Site URL'), 'field' => 'el.site_url'),
    'reciprocal_url' => array('data' => t('Reciprocal URL'), 'field' => 'el.reciprocal_url'),
    'status' => array('data' => t('Status'), 'field' => 'el.status'),
    'category' => array('data' => t('Category'), 'field' => 'el.category'),
    'operations' => array('data' => t('Operations')),
  );
  $query = db_select('exchange_links', 'el');
  user_build_filter_query($query);
  $count_query = clone $query;
  $count_query->addExpression('COUNT(el.id)');
  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('el', array('id', 'site_title', 'site_url', 'reciprocal_url', 'email', 'description', 'category', 'status'))
    ->limit(50)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $options = array();
  foreach (module_invoke_all('exchange_links_operations') as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'unblock',
  );
  $options = array();
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );
  $destination = drupal_get_destination();
  $status = array(t('blocked'), t('active'));
  $roles = array_map('check_plain', user_roles(TRUE));
  $accounts = array();
  foreach ($result as $links) {
    $options[$links->id] = array(
      'title' => $links->site_title,
      'site_url' =>  $links->site_url,
      'reciprocal_url' => $links->reciprocal_url,
      'status' => ($links->status)?'Published':'Unpublished',
      'category' => taxonomy_get_term_by_tid($links->category),
      'operations' => array('data' => array('#type' => 'link', '#title' => t('edit'), '#href' => "admin/config/exchange-links/$links->id/view", '#options' => array('query' => $destination))),
    );
  }
  $form['links'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No links available.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}
/**
 * Submit the exchange_links_overview administration update form.
 */
function exchange_links_overview_submit($form, &$form_state) {
  $operations = module_invoke_all('exchange_links_operations', $form, $form_state);
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked links.
  $links = array_filter($form_state['values']['links']);
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($links), $operation['callback arguments']);
    }
    else {
      $args = array($links);
    }
    call_user_func_array($function, $args);
    drupal_set_message(t('The update has been performed.'));
  }
}

/**
 * Implementation of exchange_links_overview_validate.
 */
function exchange_links_overview_validate($form, &$form_state) {
  $form_state['values']['links'] = array_filter($form_state['values']['links']);
  if (count($form_state['values']['links']) == 0) {
    form_set_error('', t('No links selected.'));
  }
}

/**
 * Implementation of view_links_form()
 */
function view_links_form() {
  $vocabulary = taxonomy_vocabulary_machine_name_load('exchange_links');
  $categories = taxonomy_get_tree($vocabulary->vid);
  $link_category = array();
  foreach ($categories as $category) {
    $link_category[$category->tid] = $category->name;
  }
  $id = arg(3);
  $query = db_select('exchange_links', 'el');
  $query->fields('el', array('id', 'site_title', 'site_url', 'reciprocal_url', 'email', 'description', 'category', 'status'));
  $query->condition('el.id', $id, '=');
  $resu = $query->execute()->fetchObject();
  $form['your_linking_details'] = array(
    '#type' => 'fieldset',
    '#title' => 'Linking details',
    '#collapsible' => FALSE,
  );
  $form['your_linking_details']['site_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Site title',
    '#default_value' => $resu->site_title,
    '#description' => 'Enter site title. Example: Video Sharing Platform',
    '#required' => TRUE,
  );
  $form['your_linking_details']['your_url'] = array(
    '#type' => 'textfield',
    '#title' => 'Your URL',
    '#default_value' => $resu->site_url,
    '#description' => 'Enter your site URL. Example: http://www.example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['reciprocal_link'] = array(
    '#type' => 'textfield',
    '#title' => 'Reciprocal link URL',
    '#default_value' => $resu->reciprocal_url,
    '#description' => 'Enter reciprocal link. Must be on the same domain as above. Example: http://www.example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Your email',
    '#default_value' => $resu->email,
    '#description' => 'Enter you email address. Example: username@example.com',
    '#required' => TRUE,
  );
  $form['your_linking_details']['site_desc'] = array(
    '#type' => 'textarea',
    '#title' => 'Site description',
    '#default_value' => $resu->description,
    '#description' => 'Enter site description.',
    '#required' => TRUE,
  );
  $form['your_linking_details']['category'] = array(
    '#type' => 'select',
    '#title' => 'Category',
    '#options' => $link_category,
    '#default_value' => $resu->category,
    '#description' => 'Select link category.',
    '#required' => TRUE,
  );
  $form['your_linking_details']['status'] = array(
    '#type' => 'select',
    '#title' => 'Status',
    '#options' => array(
      '0' => 'Unpublished',
      '1' => 'Published',
    ),
   '#default_value' => $resu->status,
   '#description' => 'Select publishing option.',
   '#required' => TRUE,
  );
  $form['id'] = array(
    '#type' => 'hidden',
    '#default_value' => $id,
  );
  $form['edit_link'] = array(
    '#type' => 'submit',
    '#value' => 'Update',
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('delete_link')
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('cancel_link')
  );
  return $form;
}
/**
 * Implementation view_links_form_submit($form, &$form_state)
 */
function view_links_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $site_title = $form_state['values']['site_title'];
  $site_url = $form_state['values']['your_url'];
  $reciprocal_link = $form_state['values']['reciprocal_link'];
  $mail = $form_state['values']['email'];
  $site_desc = $form_state['values']['site_desc'];
  $category = $form_state['values']['category'];
  $status = $form_state['values']['status'];
  $flag = db_update('exchange_links')
    ->fields(
      array(
        'site_title' => $site_title,
        'site_url' => $site_url,
        'reciprocal_url' => $reciprocal_link,
        'email' => $mail,
        'description' => $site_desc,
        'category' => $category,
        'status' => $status,
      )
    )
    ->condition('id', $id, '=')
    ->execute();
  if ($flag) {
    drupal_set_message(t('Link updated successfully.'), 'status');
  }
}
/**
 * Implementation delete_link($form, &$form_state)
 */
function delete_link($form, &$form_state) {
  $id = $form_state['values']['id'];
  db_delete('exchange_links')
    ->condition('id', $id, '=')
    ->execute();
  drupal_set_message(t('The link has been deleted.'));
  drupal_goto('admin/config/exchange-links/list');
}

/**
 * Implementation delete_link($form, &$form_state)
 */
function cancel_link($form, &$form_state) {
  drupal_goto('admin/config/exchange-links/list');
}