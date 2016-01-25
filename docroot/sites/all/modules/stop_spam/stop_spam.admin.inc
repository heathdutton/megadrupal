<?php

function stop_spam_settings_form() {
  $forms_options = array();
  $types_options = array();
  
  foreach (node_type_get_types() as $type) {
    $form_id = $type->type . '_node_form';
    $forms_options[$form_id] = t('@name form', array('@name' => $type->name));
    
    $types_options[$type->type] = $type->name;
  }
  
  uasort($forms_options, 'strcasecmp');
  uasort($types_options, 'strcasecmp');
  
  if (module_exists('comment')) {
    $forms_options['comment_form'] = t('Comment form');
  }

  $form = array(
    'forms' => array(
      '#type' => 'fieldset',
      '#title' => t('Protected forms'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,

      'stop_spam_forms' => array(
        '#type' => 'checkboxes',
        '#title' => t('Forms to perform spam checks on'),
        '#options' => $forms_options,
        '#default_value' => variable_get('stop_spam_forms', array()),
      ),
    ),
    'protection_bypass' => array(
      '#type' => 'fieldset',
      '#title' => t('Protection bypass'),
      '#collapsible' => TRUE,

      'stop_spam_skip_existing_on_edit' => array(
        '#type' => 'checkbox',
        '#title' => t('Skip existing external links when editing node/comment'),
        '#default_value' => variable_get('stop_spam_skip_existing_on_edit', TRUE),
      ),

      'stop_spam_allow_ext_links' => array(
        '#type' => 'checkboxes',
        '#title' => t('Conditions to allow external links posting'),
        '#options' => array(
          'authmap' => t('Existing linked account on 3rd party website'),
          'posts' => t('Minimal number of published posts reached'),
        ),
        '#default_value' => variable_get('stop_spam_allow_ext_links', array()),
      ),
      'stop_spam_min_posts_num' => array(
        '#type' => 'textfield',
        '#title' => t('Minimal number of posts to submit external links'),
        '#default_value' => variable_get('stop_spam_min_posts_num', 5),
        '#states' => array(
          'visible' => array(
            '#edit-stop-spam-allow-ext-links-posts' => array('checked' => TRUE),
          ),
        ),
      ),
      'stop_spam_local_hosts' => array(
        '#type' => 'textarea',
        '#title' => t('Allowed external hosts'),
        '#description' => t('List of hosts to allow links to in addition to current website host. All specified hosts will be checked with and without "www" prefix.'),
        '#default_value' => implode("\n", variable_get('stop_spam_local_hosts', array())),
      ),
    ),
    'external_links' => array(
      '#type' => 'fieldset',
      '#title' => t('External links index'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    
      'stop_spam_links_index_types' => array(
        '#type' => 'checkboxes',
        '#title' => t('Node types to index'),
        '#description' => t("<strong>It is strongly recommended to rebuild index after changing this setting.</strong>"),
        '#options' => $types_options,
        '#default_value' => variable_get('stop_spam_links_index_types', array()),
      ),
    ),
  );

  $form = system_settings_form($form);
  
  $form['#validate'][] = '_stop_spam_settings_form_validate';
  
  array_unshift($form['#submit'], '_stop_spam_settings_form_cleanup');
  
  return $form;
}

function _stop_spam_settings_form_validate($form, &$state) {
  $values = $state['values'];
  
  if (!empty($values['stop_spam_allow_ext_links']['posts'])) {
    $posts_num = $values['stop_spam_min_posts_num'];
    
    if (empty($posts_num) || !is_numeric($posts_num) || intval($posts_num) <= 0) {
      form_set_error('stop_spam_min_posts_num', t('Specify correct minimal number of posts.'));
    }
  }
}

function _stop_spam_settings_form_cleanup($form, &$state) {
  $filter = array(
    'stop_spam_forms',
    'stop_spam_allow_ext_links',
    'stop_spam_links_index_types',
  );
  
  foreach ($filter as $field) {
    $state['values'][$field] = array_filter($state['values'][$field]);
  }
  
  $hosts = preg_split('/\s*[\n\r]+\s*/', $state['values']['stop_spam_local_hosts'], NULL, PREG_SPLIT_NO_EMPTY);

  foreach ($hosts as $index => $host) {
    $host = _stop_spam_remove_www($host);
    
    if ($host != '') {
      $hosts[$index] = $host;
    }
    else {
      unset($hosts[$index]);
    }
  }
  
  $state['values']['stop_spam_local_hosts'] = $hosts;
}
