<?php

/**
 * @file
 * The settings formulars for archibald 2
 */
/**
 * Implements hook_settings_language().
 */
function archibald_admin_settings_language() {

  $form = array();

  $form['archibald'] = array(
    '#type' => 'fieldset',
    '#title' => t('Language settings'),
    '#collapsible' => TRUE,
  );

  include_once 'languages/en.php';

  $form['archibald'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Language settings'),
    '#default_value' => variable_get('archibald_language_settings', array()),
    '#options' => $languages,
    '#description' => t('These languages will be available for descriptions.'),
    '#theme' => 'archibald_language_settings',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  #$form['#validate'][] = 'archibald_admin_settings_language_validate';

  return $form;
}

/**
 * validate archibald_admin_settings admin settings form
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_admin_settings_language_validate(&$form, &$form_state) {
  $valid = FALSE;
  foreach ($form_state['values']['archibald'] as $v) {
    if ($v != "0") {
      $valid = TRUE;
      break;
    }
  }
  if (!$valid) {
    form_set_error('archibald', t('Please choose a minimum of 1 language'));
  }
}

/**
 * form submit handler for archibald_admin_settings admin settings form
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_admin_settings_language_submit($form, &$form_state) {
  $values = array();
  foreach ($form_state['values']['archibald'] as $v) {
    if ($v == '0') {
      continue;
    }
    $values[] = $v;
  }
  variable_set('archibald_language_settings', $values);

  drupal_set_message(t('The configuration has been saved.'));
}

/**
 * settings form for archibald solr search interface
 * Implements hook_settings().
 */
function archibald_admin_settings_fields_proposal() {
  return archibald_admin_settings_fields(TRUE);
}

/**
 * form submit handler for archibald_admin_settings_solr
 * archibald solr admin settings form
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_admin_settings_fields_proposal_submit($form, &$form_state) {
  archibald_admin_settings_fields_submit($form, $form_state);
}
/**
 * settings form for archibald solr search interface
 * Implements hook_settings().
 */
function archibald_admin_settings_fields($is_proposal = '') {
  $form = array();
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');
  archibald_fields_is_proposal(!empty($is_proposal));
  $form['is_proposal'] = array(
    '#type' => 'hidden',
    '#value' => archibald_fields_is_proposal(),
  );

  $form['options'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#tree' => FALSE,
  );

  $can_central_publish = archibald_can_publish_central();

  foreach (archibald_get_field_info() as $section_id => $section) {
    foreach ($section['fields'] AS $field_id => $field) {

      $name = $section_id . '_' . $field_id;

      $continue = FALSE;
      switch ($field_id) {
        case 'proposalMessage':
        case 'proposalContactName':
        case 'proposalContactEmail':
        case 'proposalContactPhone':
          if( empty($is_proposal) ) $continue = TRUE;
          break;
        case 'kind':
        case 'catalog':
        case 'value':
        case 'description':
          if( $section_id == 'relation' ) $continue = TRUE;
          break;
      }

      if ($continue) continue;

      $form['options'][$name] = array(
        '#title' => t('Field: @field', array('@field' => $section['label'] . ' > ' . $field['label'])),
        '#description' => t('Please choose the required option for each field.'),
        '#type' => 'radios',
        '#options' => array(
          ARCHIBALD_RES_FIELD_OPTIONAL => t('Optional'),
          ARCHIBALD_RES_FIELD_RECOMMEND => t('Recommend'),
          ARCHIBALD_RES_FIELD_REQUIRED => t('Required'),
        ),
        '#default_value' => archibald_get_field_requirements($name),
      );

      if (archibald_get_field_minimum_requirements($section_id . '.' . $field_id) > ARCHIBALD_RES_FIELD_RECOMMEND) {
        unset($form['options'][$name]['#options'][ARCHIBALD_RES_FIELD_OPTIONAL]);
        unset($form['options'][$name]['#options'][ARCHIBALD_RES_FIELD_RECOMMEND]);
      }

      if (!archibald_fields_is_proposal() && $can_central_publish && in_array($name, array('general_description', 'education_learningresourcetype', 'classification_context'))) {
        $form['options'][$name]['#disabled'] = TRUE;
        $form['options'][$name]['#default'] = ARCHIBALD_RES_FIELD_REQUIRED;
      }
    }
  }

  $form['#theme'] = 'archibald_admin_settings_fields';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * theme a archibald form admin settings field
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_admin_settings_fields($variables) {
  $form = reset($variables);

  $html = '';

  $table = array(
    'header' => array(
      t('Field'),
      array(
        'data' => t('Options'),
        'colspan' => 3,
      ),
    ),
    'rows' => array(),
    'class' => 'archibald_admin_fields',
  );

  foreach (element_children($form['options'], TRUE) as $k) {
    $field = $form['options'][$k];

    $row = array();
    $row[] = $field['#title'];

    for ($i=0;$i<=2;$i++) {
      if (empty($field[$i])) {
        $row[] = '-';
      }
      else {
        $row[] = drupal_render($field[$i]);
      }
    }

    $table['rows'][] = $row;
  }

  $html .= theme('table', $table);
  $html .= drupal_render($form['submit']);

  // prevent endless loop
  $form['#theme'] = '';
  unset($form['options']);
  $html .= drupal_render($form);

  return $html;
}

/**
 * form submit handler for archibald_admin_settings_solr
 * archibald solr admin settings form
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_admin_settings_fields_submit($form, &$form_state) {

  archibald_fields_is_proposal(!empty($form_state['values']['is_proposal']));

  $skip_keys = array(
    'submit' => TRUE,
    'form_build_id' => TRUE,
    'is_proposal' => TRUE,
    'form_token' => TRUE,
    'form_id' => TRUE,
    'op' => TRUE
  );

  foreach ($form_state['values'] as $field => $value) {
    if (isset($skip_keys[$field])) {
      continue;
    }

    variable_set('archibald_field_require_type_' . ((archibald_fields_is_proposal()) ? 'proposal_' : '') . drupal_strtolower($field), $value);
  }
  drupal_set_message(t('Field configuration saved successfully'));
}

/**
 * settings form for archibald solr search interface
 * Implements hook_settings().
 */
function archibald_admin_settings_solr() {
  $form = array();

  if (!module_exists('apachesolr')) {
    form_set_error(
      'archibald',
      t(
        'Sorry, !module module was not installed. Please install it first.',
        array(
          '!module' => '<a href="http://drupal.org/project/apachesolr" ' .
          'target="_blank">apachesolr</a>',
        )
      )
    );
    return array();
  }

  try {
    // Get the $solr object
    $solr = apachesolr_get_solr();
    // If there is no server available, don't continue.
    if (!$solr->ping(variable_get('apachesolr_ping_timeout', 4))) {
      throw new Exception(t('No Solr instance available.'));
    }
  }
  catch (Exception $e) {
    form_set_error(
      'archibald',
      t('Sorry, no Solr instance available. Please configurate apachesolr module first.') . '<br />' . l(t('configure'), 'admin/config/search/apachesolr')
    );
    return array();
  }

  if (variable_get('archibald_solr_search_activ') == 0) {
    drupal_set_message(t('Solr search is currently disabled, please save to activate it.'), 'warning');
  }

  $weights = drupal_map_assoc(
    array(
      '21.0',
      '16.0',
      '11.0',
      '7.0',
      '5.0',
      '4.0',
      '3.5',
      '3.0',
      '2.8',
      '2.6',
      '2.4',
      '2.2',
      '2.0',
      '1.9',
      '1.8',
      '1.7',
      '1.6',
      '1.5',
      '1.4',
      '1.3',
      '1.2',
      '1.1',
      '1.0',
      '0.9',
      '0.8',
      '0.7',
      '0.6',
      '0.5',
      '0.4',
      '0.3',
      '0.2',
      '0.1',
    )
  );
  $weights['0'] = t('Ignore');

  $form['archibald_solr_search_activ'] = array(
    '#title' => t('Activate solr search'),
    '#description' => t('If this option is disabled, the search function will work by default sql search system.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_solr_search_activ', 1),
  );

  $form['biases'] = array(
    '#type' => 'fieldset',
    '#title' => t('Result biasing'),
    '#description' => t('Give bias to certain properties when ordering the search results. Any value except "Ignore" will increase the score of the given type in search results. Choose "Ignore" to ignore any given property.'),
    '#collapsible' => TRUE,
  );

  $form['biases']['archibald_solr_boosting_normal'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main items'),
    '#collapsible' => FALSE,
  );

  $keys = array(
    'label' => 'Title',
    'content' => t('Description'),
    'tm_keywords' => t('Keywords'),
    'tm_coverage' => t('Coverage'),
    'ts_otherPlattformRequirements' => t('Technical requirements'),
    'ts_education_description' => t('Educational description'),
    'tm_contributor' => t('Contributors'),
    'tm_vocabular_text' => t('Vocabulary text'),
  );
  foreach ($keys as $key => $title) {
    $form['biases']['archibald_solr_boosting_normal']['archibald_solr_bias_' . $key] = array(
      '#type' => 'select',
      '#options' => $weights,
      '#title' => $title,
      '#default_value' => archibald_get_solr_bias_value($key),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * form submit handler for archibald_admin_settings_solr
 * archibald solr admin settings form
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_admin_settings_solr_submit($form, &$form_state) {
  $values = array();

  variable_set('archibald_solr_search_activ', $form_state['values']['archibald_solr_search_activ']);

  foreach ($form_state['values'] as $key => $val) {
    if (drupal_substr($key, 0, 15) == 'archibald_solr_bias_') {
      $org_val = archibald_get_solr_bias_value(drupal_substrsubstr($key, 15), TRUE);

      if ($org_val == $val) {
        // if val like educa preset dont save it
        variable_del(drupal_substr($key, 15));
      }
      else {
        variable_set($key, $val);
      }
    }
  }

  drupal_set_message(t('The configuration has been saved.'));
}

/**
 * Implements hook_settings().
 */
function archibald_admin_settings() {

  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_admin_settings.js');

  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 3
  );

  $form['general']['archibald_switch_language_while_add'] = array(
    '#title' => t('Switch to language of added description.'),
    '#description' => t('Before a new description is added the user must choose the main language of the description. The user interface will switch to the selected language.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_switch_language_while_add', 1),
  );

  $form['general']['archibald_resource_save_able_with_errors'] = array(
    '#title' => t('Allow saving a description even when required fields are missing.'),
    '#description' => t('In case this option is disabled the user is not allowed to save a description until all required fields are available.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_resource_save_able_with_errors', 1),
  );

  $form['general']['archibald_coverage_autocomplete_behaviour'] = array(
    '#title' => t('Search for keywords only within the selected language'),
    '#description' => t('Editing a description, keywords are only available in the selected language.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_coverage_autocomplete_behaviour', 1),
  );

  $form['general']['archibald_form_blur_message'] = array(
    '#title' => t('Display a warning when user leaves the description edit form without saving.'),
    '#description' => t('This option will only work with Internet Explorer 7 or newer and Firefox.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_form_blur_message', 0),
  );


  $form['general']['identifier_fileExtensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifer file extensions'),
    '#description' => t('Space separated list of file extensions
      which are allowed to upload as resource identifer.') . '<br />' .
      t('Default') . ': ' . ARCHIBALD_DEFAULT_IDENTIFIER_FILE_EXTENSIONS,
    '#default_value' => variable_get('archibald_identifier_fileExtensions', ''),
    '#size' => 50,
    '#maxlength' => 500,
  );

  $form['general']['archibald_revision_cleaner'] = array(
    '#title' => t('Enable revision cleaner'),
    '#description' => t('If this is enabled we have only a max. of revisions for a description, old ones will be deleted. The max count can be configured below.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_revision_cleaner', 1),
  );

  $form['general']['archibald_max_revisions_holded'] = array(
    '#type' => 'textfield',
    '#title' => t('Max. revisions held'),
    '#description' => t('How much revisions will be left within the revision list of each description.'),
    '#default_value' => variable_get('archibald_max_revisions_holded', 50),
    '#size' => 20,
    '#maxlength' => 10,
  );

  $form['general']['archibald_alternative_homepage_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Alternative homepage node id'),
    '#description' => t('The node id which is used for the alternative front page.'),
    '#default_value' => variable_get('archibald_alternative_homepage_node', ''),
    '#size' => 20,
    '#maxlength' => 10,
  );

  // save able with error
  $form['front_end_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Frontend search interface'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 4
  );

  $form['front_end_search']['archibald_search_in_current_language'] = array(
    '#title' => t('Search only for descriptions in selected language.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_search_in_current_language', 1),
  );

  $form['front_end_search']['archibald_search_order_latest'] = array(
    '#title' => t('Show last published description on top.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_search_order_latest', 1),
  );

  $form['archibald'] = array(
    '#type' => 'fieldset',
    '#title' => t('Connection to the Swiss national school library'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 1,
  );

  $content_partner = archibald_load_content_partner();
  $rows = array();
  $can_change_working_mode = TRUE;
  foreach ($content_partner as $content_partner_id => $data) {
    // Define all the Operations
    $ops = array();

    $content_partner_name = l($data['name'], 'admin/config/services/archibald/settings/edit/' . $data['content_partner_id'],
      array(
        'query' => array(
          'destination' => $_GET['q'],
        ),
      )
    );

    $ops['edit'] = array(
      'class' => 'operation',
      'data' => l(t('Edit'), 'admin/config/services/archibald/settings/edit/' . $data['content_partner_id']),
    );

    // Check if content partner is in use
    // and if it is the last one
    if (!empty($data['usage_count'])) {
      $can_change_working_mode = FALSE;
      $ops['delete'] = t('With this account there are @usage_count descriptions published.',
        array(
          '@usage_count' => $data['usage_count'],
        )
      );
    }
    elseif (count($content_partner) < 2) {
      $ops['delete'] = t('The last account, cannot be deleted');
    }
    else {
      $ops['delete'] = array(
        'class' => 'operation',
        'data' => l(
          t('Delete'), 'admin/config/services/archibald/settings/delete/' . $data['content_partner_id'],
          array(
            'query' => array(
              'destination' => current_path(),
            ),
          )
        ),
      );
    }

    $headers = array(
      t('Name'),
      t('Icon'),
      t('Username'),
      array(
        'data' => t('Operations'),
        'colspan' => count($ops),
      ),
    );

    // get partner icon and render it if possible
    $icon = '';
    if (!empty($data['icon_file']->uri)) {
      $icon = theme(
        'image_style', array(
          'style_name' => 'archibald_contributor',
          'path' => $data['icon_file']->uri,
          'getsize' => FALSE,
        )
      );
    }

    $rows[$content_partner_id] = array(
      'data' => array(
        // Cells
        array(
          'data' => $content_partner_name,
        ),
        $icon,
        check_plain($data['username']),
      ),
    );
    // Add the links to the page
    $rows[$content_partner_id]['data'] = array_merge($rows[$content_partner_id]['data'], $ops);
  }

  $form['archibald']['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => array_values($rows),
    '#attributes' => array('class' => array('admin-archibald')),
  );

  $form['archibald']['add_new_content_partner'] = array(
    '#markup' => l(t('Add new content partner'), 'admin/config/services/archibald/settings/add') . '<br /><br />'
  );

  $form['publication'] = array(
    '#type' => 'fieldset',
    '#title' => t('The publication of descriptions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 2,
  );

  if ( archibald_can_publish_central() != TRUE ) {
    $form['publication']['archibald_publish_type_info'] = array(
      '#markup' => t('As long no content partner is configured, descriptions can only be published local.')
    );
  }
  else {
    $form['publication']['archibald_publish_type'] = array(
      '#type' => 'radios',
      '#options' => array(
      ARCHIBALD_PUBLISH_SEPERATE => t('Descriptions can be published locally or nationally. Use this option if you want to publish  only selected descriptions to the national catalogue.'),
      ARCHIBALD_PUBLISH_TOGETHER => t('Descriptions must be published locally and nationally. Use this option if you want to publish all your descriptions to the national catalogue.'),
      ),
      '#default_value' => variable_get('archibald_publish_type', ARCHIBALD_PUBLISH_SEPERATE),
    );
  }



  $form['ontolog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ontology settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 5
  );

  $form['ontolog']['archibald_ontology_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('Set URL for the ontology service'),
    '#default_value' => variable_get('archibald_ontology_url', 'http://ontology.biblio.educa.ch'),
    '#size' => 50,
    '#maxlength' => 250,
    '#required' => FALSE,
  );

  // This configuration allows admins to synchronize the licences taxonomy with the ontology service by cron.
  // At the moment its not used because the licences taxonomy is synchronized during the installation only
  /* $form['ontolog']['archibald_ontology_sync_licences'] = array(
    '#title' => t('Synchronize the licences taxonomy with the ontology service.') .
      ' ' . l('View licences taxonomy', 'admin/structure/taxonomy/rights_licenses'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('archibald_ontology_sync_licences', 0)
  );
   */


  $form['email_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-Mails'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 6
  );

  $form['email_fieldset']['email'] = array(
    '#type' => 'vertical_tabs',
  );

  // These email tokens are shared for all settings, so just define
  // the list once to help ensure they stay in sync.
  $email_token_help_anonymous = t('Available variables are: [site:name], [site:url], [lom:title].');
  $email_token_help = t('Available variables are: [site:name], [site:url], [user:name], [user:mail], [lom:id], [lom:url], [lom:title], [overview:proposal-url], [overview:to-publish-url], [overview:own-url].');

  $roles_to_publish = array_map('check_plain', archibald_get_users_with_perm_options(array('publish archibald content central', 'publish archibald content local')));
  $roles_to_edit = array_map('check_plain', archibald_get_users_with_perm_options('edit archibald content'));

  $description = t('Edit the template for the notification E-Mail sent to anonymous users submitting a proposal.');
  $description .= ' ' . $email_token_help_anonymous;
  $description .= '<br /><h5>' . t('Write E-Mail templates in english. For translation use the "Translate" link.') . '</h5>';

  $form['email_fieldset']['email_new_proposal'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proposal has been submitted'),
    '#collapsible' => TRUE,
    '#description' => $description,
    '#group' => 'email',
  );

  $form['email_fieldset']['email_new_proposal']['archibald_mail_new_proposal_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => archibald_mail_text('new_proposal_subject', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('new_proposal_subject'),
    '#maxlength' => 180,
  );

  $form['email_fieldset']['email_new_proposal']['archibald_mail_new_proposal_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => archibald_mail_text('new_proposal_body', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('new_proposal_body'),
    '#rows' => 15,
  );

  $description = t('Edit the template for the notification E-Mail sent to anonymous users when a proposal is changed to draft.');
  $description .= ' ' . $email_token_help_anonymous;
  $description .= '<br /><h5>' . t('Write E-Mail templates in english. For translation use the "Translate" link.') . '</h5>';

  $form['email_fieldset']['email_proposal2draft'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proposal is being reviewed'),
    '#collapsible' => TRUE,
    '#description' => $description,
    '#group' => 'email',
  );

  $form['email_fieldset']['email_proposal2draft']['archibald_mail_proposal2draft_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => archibald_mail_text('proposal2draft_subject', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('proposal2draft_subject'),
    '#maxlength' => 180,
  );

  $form['email_fieldset']['email_proposal2draft']['archibald_mail_proposal2draft_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => archibald_mail_text('proposal2draft_body', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('proposal2draft_body'),
    '#rows' => 15,
  );

  $description = t('Edit the template for the notification E-Mail sent to anonymous users when a proposal is published.');
  $description .= ' ' . $email_token_help_anonymous;
  $description .= '<br /><h5>' . t('Write E-Mail templates in english. For translation use the "Translate" link.') . '</h5>';

  $form['email_fieldset']['email_proposal2publish'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proposal has been published'),
    '#collapsible' => TRUE,
    '#description' => $description,
    '#group' => 'email',
  );

  $form['email_fieldset']['email_proposal2publish']['archibald_mail_proposal2publish_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => archibald_mail_text('proposal2publish_subject', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('proposal2publish_subject'),
    '#maxlength' => 180,
  );

  $form['email_fieldset']['email_proposal2publish']['archibald_mail_proposal2publish_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => archibald_mail_text('proposal2publish_body', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('proposal2publish_body'),
    '#rows' => 15,
  );

  $description = t('Edit the template for the notification E-Mail sent to users with editor role, when a new description has been proposed.');
  $description .= ' ' . $email_token_help;
  $description .= '<br /><h5>' . t('Write E-Mail templates in english. For translation use the "Translate" link.') . '</h5>';

  $form['email_fieldset']['email_new_unavailable'] = array(
    '#type' => 'fieldset',
    '#title' => t('New resource has been proposed'),
    '#collapsible' => TRUE,
    '#description' => $description,
    '#group' => 'email',
  );

  $form['email_fieldset']['email_new_unavailable']['enable_email_new_unavailable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => (variable_get('archibald_send_mails_new_unavailable', TRUE)) ? 1 : 0
  );

  $form['email_fieldset']['email_new_unavailable']['archibald_mail_new_unavailable_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => archibald_mail_text('new_unavailable_subject', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('new_unavailable_subject'),
    '#maxlength' => 180,
  );

  $form['email_fieldset']['email_new_unavailable']['archibald_mail_new_unavailable_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => archibald_mail_text('new_unavailable_body', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('new_unavailable_body'),
    '#rows' => 15,
  );

  $new_unavailable_groups = variable_get('archibald_mail_new_unavailable_group', array(1));
  if (!is_array($new_unavailable_groups)) {
    $new_unavailable_groups = array($new_unavailable_groups);
  }
  $form['email_fieldset']['email_new_unavailable']['archibald_mail_new_unavailable_group'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Users'),
    '#default_value' => $new_unavailable_groups,
    '#options' => $roles_to_edit,
    '#description' => t('Define here which users should receive the E-Mail.'),
    '#theme' => 'archibald_two_column_checkbox',
  );

  $form['email_fieldset']['email_status2final'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status of description has been changed to final'),
    '#collapsible' => TRUE,
    '#description' => t('Edit the template for the notification E-Mail sent to users with publisher role, when the status of a description has been changed from draft to final.') . ' ' . $email_token_help,
    '#group' => 'email',
  );

  $form['email_fieldset']['email_status2final']['enable_email_status2final'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => (variable_get('archibald_send_mails_status2final', TRUE)) ? 1 : 0
  );

  $form['email_fieldset']['email_status2final']['archibald_mail_status2final_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => archibald_mail_text('status2final_subject', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('status2final_subject'),
    '#maxlength' => 180,
  );

  $form['email_fieldset']['email_status2final']['archibald_mail_status2final_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => archibald_mail_text('status2final_body', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('status2final_body'),
    '#rows' => 15,
  );

  $status2final_groups = variable_get('archibald_mail_status2final_group', array(1));
  if (!is_array($status2final_groups)) {
    $status2final_groups = array($status2final_groups);
  }
  $form['email_fieldset']['email_status2final']['archibald_mail_status2final_group'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Users'),
    '#default_value' => $status2final_groups,
    '#options' => $roles_to_publish,
    '#description' => t('Define here which users should receive the E-Mail.'),
    '#theme' => 'archibald_two_column_checkbox',
  );

  $form['email_fieldset']['email_status2draft'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status of description has been reset to draft'),
    '#collapsible' => TRUE,
    '#description' => t('Edit the template for the notification E-Mail sent to the responsible editor, when the status of a descritpion has been changed from final to draft.') . ' ' . $email_token_help,
    '#group' => 'email',
  );

  $form['email_fieldset']['email_status2draft']['enable_email_status2draft'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => (variable_get('archibald_send_mails_status2draft', TRUE)) ? 1 : 0
  );

  $form['email_fieldset']['email_status2draft']['archibald_mail_status2draft_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => archibald_mail_text('status2draft_subject', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('status2draft_subject'),
    '#maxlength' => 180,
  );

  $form['email_fieldset']['email_status2draft']['archibald_mail_status2draft_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => archibald_mail_text('status2draft_body', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('status2draft_body'),
    '#rows' => 15,
  );

  $status2draft_groups = variable_get('archibald_mail_status2draft_group', array(1));
  if (!is_array($status2draft_groups)) {
    $status2draft_groups = array($status2draft_groups);
  }
  $form['email_fieldset']['email_status2draft']['archibald_mail_status2draft_group'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Users'),
    '#default_value' => $status2draft_groups,
    '#options' => $roles_to_edit,
    '#description' => t('Define here which users should receive the E-Mail.'),
  );

  $form['email_fieldset']['email_set_responsible'] = array(
    '#type' => 'fieldset',
    '#title' => t('A description has been assigned to you'),
    '#collapsible' => TRUE,
    '#description' => t('Edit the template for the notification E-Mail sent to the new responsible editor, when the current responsible editor passes this task.') . ' ' . $email_token_help,
    '#group' => 'email',
  );

  $form['email_fieldset']['email_set_responsible']['enable_email_set_responsible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable'),
    '#default_value' => (variable_get('archibald_send_mails_set_responsible', TRUE)) ? 1 : 0
  );

  $form['email_fieldset']['email_set_responsible']['archibald_mail_set_responsible_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => archibald_mail_text('set_responsible_subject', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('set_responsible_subject'),
    '#maxlength' => 180,
  );

  $form['email_fieldset']['email_set_responsible']['archibald_mail_set_responsible_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => archibald_mail_text('set_responsible_body', 'en', array(), TRUE),
    '#description' => archibald_mail_translate_description('set_responsible_body'),
    '#rows' => 15,
  );

  $available_curriculums = archibald_get_available_curriculums();
  if (!empty($available_curriculums)) {

    $form['curriculum_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Curriculum'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 6,
      '#tree' => TRUE,
    );


    $form['curriculum_fieldset']['curriculum'] = array(
      '#type' => 'vertical_tabs',
    );

    foreach ($available_curriculums as $curriculum) {
      $classname = 'ArchibaldCurriculum' . drupal_ucfirst($curriculum);

      $curr_handler = new $classname();

      $form['curriculum_fieldset'][$curriculum] = array(
        '#type' => 'fieldset',
        '#title' => t('Settings for @name', array('@name' => $curr_handler->getName())),
        '#collapsible' => TRUE,
        '#group' => 'curriculum',
        '#tree' => TRUE,
      );

      $form['curriculum_fieldset'][$curriculum] += $curr_handler->getSettingsForm();
    }
  }


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 7
  );

  $form['#validate'][] = 'archibald_admin_settings_validate';

  return $form;
}

function theme_archibald_two_column_checkbox($variables) {

  $element = $variables['form'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  $attributes['class'][] = 'form-checkboxes';
  $attributes['class'][] = 'clearfix';
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = array_merge($attributes['class'], $element['#attributes']['class']);
  }
  if (isset($element['#attributes']['title'])) {
    $attributes['title'] = $element['#attributes']['title'];
  }

  $elements = "";
  $c = 0;
  foreach (element_children($element) AS $entry) {
    $elements .= '<div style="float: left;width: 40%;">' . drupal_render($element[$entry]) . '</div>';
    if ($c++%2 == 0) {
      $elements .= '<div style="clear: both;"></div>';
    }
  }

  #print_r($element['#children']);die();
  $html = '<div' . drupal_attributes($attributes) . '>' . $elements . '</div>';
  return $html;
}

/**
 * Returns the text which includes a translation link for the given mail key
 * @param string $key
 *  the email key
 * @return string
 */
function archibald_mail_translate_description($key) {
  return t('To translate the E-Mail text please click on the following link: !link', array(
    '!link' => l(t('Translate'), archibald_mail_get_translate_link($key), array('attributes' => array('target' => '_Blank')))
  ));
}

/**
 * Returns the path to the translation link
 *
 * @param string $en_text
 *  the english text
 * @return string
 *  the url to the translation
 */
function archibald_mail_get_translate_link($key) {

  $en_text = archibald_mail_text($key, NULL, array(), TRUE);

  $row = db_query("SELECT lid FROM {locales_source} WHERE textgroup = 'default' AND source = :source", array(
    ':source' => $en_text
  ))->fetchAssoc();

  return 'admin/config/regional/translate/edit/' . $row['lid'];
}
/**
 * form submit handler for archibald_admin_setting admin settings form
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_admin_settings_submit($form, &$form_state) {
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_admin_settings.js');

  variable_set('archibald_ontology_url', $form_state['values']['archibald_ontology_url']);
  //variable_set('archibald_ontology_sync_licences', $form_state['values']['archibald_ontology_sync_licences']);

  // If an upload error occured, reset all batch key op.
  if (variable_get('archibald_upload_error', 0) == 1) {
    db_update('archibald_lom_stats')->fields(array(
      'batch_op_key' => '',
      'batch_op_time' => 0,
    ))->execute();
  }

  // reset cron look after big processing error
  variable_set('archibald_upload_error', 0);


  // save archibald_identifier_fileExtensions
  variable_set('archibald_identifier_fileExtensions', $form_state['values']['identifier_fileExtensions']);

  // save archibald_identifier_fileExtensions
  if (!isset($form_state['values']['archibald_publish_type'])) {
    variable_set('archibald_publish_type', ARCHIBALD_PUBLISH_TOGETHER);
  }
  else {
    variable_set('archibald_publish_type', $form_state['values']['archibald_publish_type']);
  }

  // save archibald_search_order_latest
  variable_set('archibald_search_order_latest', $form_state['values']['archibald_search_order_latest']);

  // save archibald_alternative_homepage_node
  variable_set('archibald_alternative_homepage_node', $form_state['values']['archibald_alternative_homepage_node']);

  // save archibald_switch_language_while_add
  variable_set('archibald_switch_language_while_add', $form_state['values']['archibald_switch_language_while_add']);

  // save archibald_resource_save_able_with_errors
  variable_set('archibald_resource_save_able_with_errors', $form_state['values']['archibald_resource_save_able_with_errors']);

  // save archibald_coverage_autocomplete_behaviour
  variable_set('archibald_coverage_autocomplete_behaviour', $form_state['values']['archibald_coverage_autocomplete_behaviour']);

  // save archibald_form_blur_message
  variable_set('archibald_form_blur_message', $form_state['values']['archibald_form_blur_message']);

  // save archibald_max_revisions_holded
  variable_set('archibald_max_revisions_holded', $form_state['values']['archibald_max_revisions_holded']);

  // save archibald_max_revisions_holded
  variable_set('archibald_revision_cleaner', $form_state['values']['archibald_revision_cleaner']);

  // save archibald_search_in_current_language
  variable_set('archibald_search_in_current_language', $form_state['values']['archibald_search_in_current_language']);

  $archibald_mail_new_proposal_body = archibald_rmr($form_state['values']['archibald_mail_new_proposal_body']);
  $archibald_mail_proposal2draft_body = archibald_rmr($form_state['values']['archibald_mail_proposal2draft_body']);
  $archibald_mail_proposal2publish_body = archibald_rmr($form_state['values']['archibald_mail_proposal2publish_body']);
  $archibald_mail_new_unavailable_body = archibald_rmr($form_state['values']['archibald_mail_new_unavailable_body']);
  $archibald_mail_status2final_body = archibald_rmr($form_state['values']['archibald_mail_status2final_body']);
  $archibald_mail_status2draft_body = archibald_rmr($form_state['values']['archibald_mail_status2draft_body']);
  $archibald_mail_set_responsible_body = archibald_rmr($form_state['values']['archibald_mail_set_responsible_body']);

  // save mail configurations
  if ($form_state['values']['archibald_mail_new_proposal_subject'] == archibald_mail_text('new_proposal_subject', NULL, array(), TRUE, TRUE)) {
    $form_state['values']['archibald_mail_new_proposal_subject'] = NULL;
  }

  if ( $archibald_mail_proposal2draft_body == archibald_mail_text('proposal2draft_body', NULL, array(), TRUE, TRUE)) {
    $archibald_mail_proposal2draft_body = NULL;
  }

  if ($form_state['values']['archibald_mail_proposal2draft_subject'] == archibald_mail_text('proposal2draft_subject', NULL, array(), TRUE, TRUE)) {
    $form_state['values']['archibald_mail_proposal2draft_subject'] = NULL;
  }

  if ( $archibald_mail_new_proposal_body == archibald_mail_text('new_proposal_body', NULL, array(), TRUE, TRUE)) {
    $archibald_mail_new_proposal_body = NULL;
  }

  if ($form_state['values']['archibald_mail_proposal2publish_subject'] == archibald_mail_text('proposal2publish_subject', NULL, array(), TRUE, TRUE)) {
    $form_state['values']['archibald_mail_proposal2publish_subject'] = NULL;
  }

  if ( $archibald_mail_proposal2publish_body == archibald_mail_text('proposal2publish_body', NULL, array(), TRUE, TRUE)) {
    $archibald_mail_proposal2publish_body = NULL;
  }

  if ($form_state['values']['archibald_mail_new_unavailable_subject'] == archibald_mail_text('new_unavailable_subject', NULL, array(), TRUE, TRUE)) {
    $form_state['values']['archibald_mail_new_unavailable_subject'] = NULL;
  }

  if ( $archibald_mail_new_unavailable_body == archibald_mail_text('new_unavailable_body', NULL, array(), TRUE, TRUE)) {
    $archibald_mail_new_unavailable_body = NULL;
  }

  if ($form_state['values']['archibald_mail_status2final_subject'] == archibald_mail_text('status2final_subject', NULL, array(), TRUE, TRUE)) {
    $form_state['values']['archibald_mail_status2final_subject'] = NULL;
  }

  if ($archibald_mail_status2final_body == archibald_mail_text('status2final_body', NULL, array(), TRUE, TRUE)) {
    $archibald_mail_status2final_body = NULL;
  }

  if ($form_state['values']['archibald_mail_status2draft_subject'] == archibald_mail_text('status2draft_subject', NULL, array(), TRUE, TRUE)) {
    $form_state['values']['archibald_mail_status2draft_subject'] = NULL;
  }

  if ($archibald_mail_status2draft_body == archibald_mail_text('status2draft_body', NULL, array(), TRUE, TRUE)) {
    $archibald_mail_status2draft_body = NULL;
  }

  if ($form_state['values']['archibald_mail_set_responsible_subject'] == archibald_mail_text('set_responsible_subject', NULL, array(), TRUE, TRUE)) {
    $form_state['values']['archibald_mail_set_responsible_subject'] = NULL;
  }

  if ($archibald_mail_set_responsible_body == archibald_mail_text('set_responsible_body', NULL, array(), TRUE, TRUE)) {
    $archibald_mail_set_responsible_body = NULL;
  }


  variable_set('archibald_mail_new_proposal_subject', $form_state['values']['archibald_mail_new_proposal_subject']);
  variable_set('archibald_mail_new_proposal_body', $archibald_mail_new_proposal_body);

  variable_set('archibald_mail_proposal2draft_subject', $form_state['values']['archibald_mail_proposal2draft_subject']);
  variable_set('archibald_mail_proposal2draft_body', $archibald_mail_proposal2draft_body);

  variable_set('archibald_mail_proposal2publish_subject', $form_state['values']['archibald_mail_proposal2publish_subject']);
  variable_set('archibald_mail_proposal2publish_body', $archibald_mail_proposal2publish_body);

  variable_set('archibald_mail_new_unavailable_subject', $form_state['values']['archibald_mail_new_unavailable_subject']);
  variable_set('archibald_mail_new_unavailable_body', $archibald_mail_new_unavailable_body);
  variable_set('archibald_mail_new_unavailable_group', $form_state['values']['archibald_mail_new_unavailable_group']);
  variable_set('archibald_send_mails_new_unavailable', !empty($form_state['values']['enable_email_new_unavailable']));

  variable_set('archibald_mail_status2final_subject', $form_state['values']['archibald_mail_status2final_subject']);
  variable_set('archibald_mail_status2final_body', $archibald_mail_status2final_body);
  variable_set('archibald_mail_status2final_group', $form_state['values']['archibald_mail_status2final_group']);
  variable_set('archibald_send_mails_status2final', !empty($form_state['values']['enable_email_status2final']));

  variable_set('archibald_mail_status2draft_subject', $form_state['values']['archibald_mail_status2draft_subject']);
  variable_set('archibald_mail_status2draft_body', $archibald_mail_status2draft_body);
  variable_set('archibald_mail_status2draft_group', $form_state['values']['archibald_mail_status2draft_group']);
  variable_set('archibald_send_mails_status2draft', !empty($form_state['values']['enable_email_status2draft']));

  variable_set('archibald_mail_set_responsible_subject', $form_state['values']['archibald_mail_set_responsible_subject']);
  variable_set('archibald_mail_set_responsible_body', $archibald_mail_set_responsible_body);
  variable_set('archibald_send_mails_set_responsible', !empty($form_state['values']['enable_email_set_responsible']));

  foreach (archibald_get_available_curriculums() as $curriculum) {
    $classname = 'ArchibaldCurriculum' . drupal_ucfirst($curriculum);

    $curr_handler = new $classname();
    $curr_handler->submitSettingsForm($form_state['values']['curriculum_fieldset'][$curriculum], $form_state);
  }

  drupal_set_message(t('The configuration has been saved.'));

  // to rebuild the menu
  menu_rebuild();
}

/**
 * validate archibald_admin_settings admin settings form
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_admin_settings_validate(&$form, &$form_state) {
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_admin_settings.js');
  $form_values = $form_state['values'];

  if (!empty($form_values['archibald_alternative_homepage_node'])) {

    if (intval($form_values['archibald_alternative_homepage_node'])."" !== $form_values['archibald_alternative_homepage_node']."") {
      form_set_error('archibald_alternative_homepage_node', t('You have to provide a correct node id (only numbers).'));
    }
    else {
      $node = node_load((int)$form_values['archibald_alternative_homepage_node']);
      if (empty($node)) {
        form_set_error('archibald_alternative_homepage_node', t('You have to provide an existing node id'));
      }
    }

  }


  foreach (archibald_get_available_curriculums() as $curriculum) {
    $classname = 'ArchibaldCurriculum' . drupal_ucfirst($curriculum);

    $curr_handler = new $classname();
    $curr_handler->validateSettingsForm($form_state['values']['curriculum_fieldset'][$curriculum], $form_state);
  }

  if (!valid_url($form_values['archibald_ontology_url']) && !empty($form_values['archibald_ontology_url'])) {
    form_set_error('ontolog][archibald_ontology_url', t('Please enter a valid URL.'));
  }
}

/**
 * theme archibald language settings
 *
 * @param array $variables
 *
 * @return string
 */
function theme_archibald_language_settings($variables) {
  $html = '';
  $form = &$variables['form'];

  $table = array(
    'rows' => array(),
  );
  asort($form['#options']);
  $languag_count = count($form['#options']);
  $k2code        = array_keys($form['#options']);
  $cols          = 6;
  $max_rows      = ceil($languag_count / $cols);
  $i             = 0;

  for ($i = 0; $i <= $max_rows; $i++) {

    $col = array();
    for ($x = 0; $x < $cols; $x++) {
      $k = $i + ($x * $max_rows);
      if (!isset($k2code[$k])) {
        $col[] = '';
        continue;
      }
      $k = $k2code[$k];
      $col[] = "<div class='form-item form-type-checkbox " . "form-item-archibald-" . $k . "'>" . theme("checkbox", $form[$k]) . " <label class='option' for='" . $form[$k]['#id'] . "'>" . $form[$k]['#title'] . "</label></div>";
    }
    $table['rows'][] = $col;
  }


  $html = theme('table', $table);

  return $html;
}

/**
 * menu callback
 * show the content partner add / edit form
 *
 * @param array $form
 * @param array $form_state
 * @param integer $content_partner_id
 *
 * @return array
 *   a drupal form
 */
function archibald_content_partner_edit_form($form, $form_state, $content_partner_id = NULL) {
  $content_partner_id = (INT) $content_partner_id;

  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/jquery-ui-1.8.23.custom.min.js');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_admin_content_partner_settings.js');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald_content_partner_edit.css');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald_ui_dialog.css');
  $content_partner = archibald_load_content_partner($content_partner_id);

  drupal_add_js(
    array(
      'archibald' => array(
        'admin_settings_cancel_url' => request_uri(),
      ),
    ),
    'setting'
  );

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add an account to create/publish descriptions.'),
    '#collapsible' => FALSE,
  );


  $form['account']['content_partner_id'] = array(
    '#type' => 'hidden',
    '#value' => $content_partner['content_partner_id'],
  );

  $form['account']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Choose an arbitrary name for the account.'),
    '#default_value' => $content_partner['name'],
    '#size' => 50,
    '#maxlength' => 128,
    '#weight' => -15,
    '#required' => TRUE,
  );



  $is_content_partner_editable = TRUE;
  if ( !empty($content_partner['usage_count']) ) {
    $is_content_partner_editable = FALSE;

    $form['account']['user_info'] = array(
      '#markup' => t('Your are not allowed to change the username as long as descriptions
        are published under this account!'),
    );


  }

  $form['account']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Enter your username which was provided by educa.ch'),
    '#default_value' => $content_partner['username'],
    '#size' => 50,
    '#maxlength' => 128,
    '#weight' => -10,
    '#disabled' => !$is_content_partner_editable
  );

  $form['account']['key'] = array(
    '#type' => 'managed_file',
    '#title' => t('Private key file'),
    '#description' => t('Upload the private key file provided by educa.ch. This key enables Archibald to publish descriptions to the national catalogue.'),
    '#upload_validators' => array(
    'file_validate_extensions' => array(
      'pem key private',
      ),
    ),
    '#default_value' => $content_partner['key'],
    '#upload_location' => 'public://files/lom_client_private_key',
  );

  if (variable_get('file_private_path', FALSE)) {
    $form['account']['key']['#upload_location'] = 'private://files/lom_client_private_key';
  }

  $form['account']['password'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key file Password'),
    '#description' => t('Enter the password for the private key file which was provided by educa.ch'),
    '#default_value' => '',
    // we dont like to show the password  $content_partner['password'],
    '#size' => 50,
    '#maxlength' => 120,
  );

  if (!empty($content_partner['content_partner_id'])) {
    $form['account']['password']['#description'] .= '<br />' .
      t('Don\'t mind an empty field. You only need to enter the password to change it.');
  }




  $form['logo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Partner Logo'),
    '#collapsible' => FALSE,
  );


  $form['logo']['icon'] = array(
    '#type' => 'managed_file',
    '#title' => t('Content Partner Logo'),
    '#required' => TRUE,
    '#description' => t('Upload the logo of your institution. It will be uploaded with every published description. After changing the logo you need to republish all your descriptions!'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('png jpg jpeg gif'),
    ),
    '#default_value' => $content_partner['icon'],
    '#upload_location' => 'public://files/company_logo',
    '#field_suffix' => '<div class="archibald_preview_image" style_name="archibald_contributor">&nbsp;</div>',
  );
  /*
   * @cha, 26.04.2013
   * Removed file server types.

  $form['file_upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('File upload'),
    '#collapsible' => TRUE,
  );


  $file_upload_options = array('no' => t('Not permmited'));


  if (!empty($form_state['values']['file_server_type'])) {
    $content_partner['file_server_type'] = $form_state['values']['file_server_type'];
  }

  $dir = new ArchibaldDir('file_uploader', FALSE, dirname(__FILE__));
  $dir->justFiles();
  $dir->fileRegexp('.class.php');

  $selected_plugin = NULL;
  foreach ($dir as $entry) {
    require_once $entry->path;
    if (!preg_match("/^uploader\.(.*)\.class\.php/", $entry->filename, $matches)) {
      continue;
    }

    $classname = 'ArchibaldUploader' . drupal_ucfirst($matches[1]);
    $class = new $classname();
    $file_upload_options = array_merge($file_upload_options, $class->getUploaderName());
    if ($content_partner['file_server_type'] == $matches[1]) {
      $selected_plugin = $class;
    }
  }



  $form['file_upload']['file_server_type'] = array(
    '#type' => 'select',
    '#title' => t('Upload method for digital resources'),
    '#options' => $file_upload_options,
    '#default_value' => $content_partner['file_server_type'],
    '#description' =>
      t('Choose the upload method for digital resources') .
      '<br />' .
      t('IMPORTANT: In case you choose - FTP server - you must configure the settings of the FTP server!'),
    '#ajax' => array(
      // #ajax has two required keys: callback and wrapper.
      // 'callback' is a function that will be called when this element changes.
      'callback' => 'archibald_content_partner_change_plugin_callback',
      // 'wrapper' is the HTML id of the page element that will be replaced.
      'wrapper' => 'replace_plugin_config_div',
      // There are also several optional keys - see ajax_example_autocheckboxes
      // below for details on 'method', 'effect' and 'speed' and
      // ajax_example_dependent_dropdown for 'event'.
    ),
  );

  if (!empty($selected_plugin) || $content_partner['file_server_type'] == 'no') {
    $form['file_upload']['file_uploader_config'] = array(
      '#type' => 'fieldset',
      '#title' => t("Server configuration"),
      '#tree' => TRUE,
      '#prefix' => '<div id="replace_plugin_config_div">',
      '#suffix' => '</div>',
    );
    if ($content_partner['file_server_type'] != 'no') {
      $form['file_upload']['file_uploader_config'] = array_merge(
        $form['file_upload']['file_uploader_config'],
        $selected_plugin->getSettingsForm($form, $form_state, $content_partner_id)
      );
    }
  }
*/
  // Button for the corresponding actions
  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/services/archibald/settings',
  );

  return $form;
}

/**
 * Callback.
 *
 * On an ajax submit, the form builder function is called again, then the $form
 * and $form_state are passed to this callback function so it can select which
 * portion of the form to send on to the client.
 *
 * @return renderable array (the textfield element)
 */
function archibald_content_partner_change_plugin_callback($form, $form_state) {
  // The form has already been submitted and updated. We can return the replaced
  // item as it is.
  if (empty($form['file_upload']['file_uploader_config'])) {
    return array("#markup" => '');
  }
  return array(
    "#markup" => '<div id="replace_plugin_config_div">' . drupal_render($form['file_upload']['file_uploader_config']) . '</div>',
  );
  return $form['file_upload']['file_uploader_config'];
}

/**
 * validate archibald_content_partner_edit_form admin settings form for
 * content partner
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_partner_edit_form_validate(&$form, &$form_state) {
  //validate private file

  if (empty($form_state['values']['password']) && !empty($form_state['values']['content_partner_id'])) {
    $form_state['values']['content_partner_id'] = (INT) $form_state['values']['content_partner_id'];
    $content_partner = archibald_load_content_partner($form_state['values']['content_partner_id']);
    $form_state['values']['password'] = $content_partner['password'];
  }

  if (!empty($form_state['values']['key'])) {
    if (empty($form_state['values']['password'])) {
      $password = '';
    }
    else {
      $password = $form_state['values']['password'];
    }

    $file = file_load($form_state['values']['key']);
    $client_keyfile = file_get_contents($file->uri);
    if (openssl_pkey_get_private($client_keyfile, $password) === FALSE) {
      form_set_error('password', t('Private key password is wrong'));
    }
  }

  /*
   * @cha, 26.04.2013
   * Removed file server types.
  if (!empty($form_state['values']['file_server_type']) && !empty($form_state['values']['file_uploader_config']) && $form_state['values']['file_server_type'] != 'no') {
    require_once(dirname(__FILE__) . '/file_uploader/uploader.' . $form_state['values']['file_server_type'] . '.class.php');

    $classname = 'ArchibaldUploader' . drupal_ucfirst($form_state['values']['file_server_type']);
    $class = new $classname();
    $class->validateSettingsForm($form['file_upload']['file_uploader_config'], $form_state['values']['file_uploader_config']);
  }
   */
}

/**
 * save archibald_content_partner_edit_form admin settings form for
 * content partner
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_partner_edit_form_submit(&$form, &$form_state) {

  $form_state['values']['content_partner_id'] = (INT) $form_state['values']['content_partner_id'];
  $form_state['redirect'] = 'admin/config/services/archibald/settings';


  //Save company logo
  if (!empty($form_state['values']['icon'])) {
    $file         = file_load($form_state['values']['icon']);
    $file->status = FILE_STATUS_PERMANENT;
    $file         = file_save($file);
    file_usage_add($file, 'archibald', 'company_logo', 1);

    // In case company logo is updated, all published resource must be republished.
    $content_partner = archibald_load_content_partner($form_state['values']['content_partner_id']);
    $old_icon_fid = $content_partner['icon'];
    $content_partner_usage_count = $content_partner['usage_count'];

    if ( $form_state['values']['icon'] != $old_icon_fid) {

      $solr_enabled = variable_get('archibald_solr_search_activ', 0);

      $sql = db_query("SELECT lom_id FROM {archibald_lom_stats} WHERE republication_required = 0 AND (publication_version != '' OR (local_published != '' AND :solr_enabled = 1)) AND content_partner_id = :cpid", array(
        ':cpid' => $form_state['values']['content_partner_id'],
        ':solr_enabled' => $solr_enabled,
      ));

      while ($row = $sql->fetch()) {

        $row_check = db_select('archibald_change_log', 'acl')
          ->fields('acl', array('lom_id'))
          ->condition('lom_id', $row->lom_id)
          ->condition('language', '')
          ->condition('field', 'content partner logo')
          ->execute()
          ->fetchAssoc();

        if (empty($row_check)) {
          db_insert('archibald_change_log')
            ->fields(array(
              'lom_id' => $row->lom_id,
              'language' => '',
              'field' => 'content partner logo',
              'old_value' => '',
              'new_value' => 'CHANGED',
            ))
            ->execute();
        }
      }



      db_query("UPDATE {archibald_lom_stats} SET republication_required = 1 WHERE (publication_version != '' OR (local_published != '' AND :solr_enabled = 1)) AND content_partner_id = :cpid", array(
        ':cpid' => $form_state['values']['content_partner_id'],
        ':solr_enabled' => $solr_enabled,
      ));
    }
  }

  //Save private file
  if (!empty($form_state['values']['key'])) {
    $file         = file_load($form_state['values']['key']);
    $file->status = FILE_STATUS_PERMANENT;
    $file         = file_save($file);
    file_usage_add($file, 'archibald', 'lom_client_private_key', 1);
  }

  $content_partner = array(
    'content_partner_id' => $form_state['values']['content_partner_id'],
    'name' => $form_state['values']['name'],
    'icon_fid' => (INT) $form_state['values']['icon'],
    'file_server_type' => 'no',
  );

  if (isset($form_state['values']['username'])) {
    $content_partner['username'] = $form_state['values']['username'];
  }

  if (isset($form_state['values']['key'])) {
    $content_partner['key_fid'] = $form_state['values']['key'];
  }

  if (!empty($form_state['values']['password'])) {
    $content_partner['password'] = $form_state['values']['password'];
  }

  if (empty($content_partner['content_partner_id'])) {
    unset($content_partner['content_partner_id']);
    $partner_id = db_insert('archibald_settings_content_partner')->fields($content_partner)->execute();
  }
  else {
    db_update('archibald_settings_content_partner')->fields($content_partner)->condition('content_partner_id', $content_partner['content_partner_id'])->execute();
    $partner_id = $content_partner['content_partner_id'];
  }

  // reset cron look after big processing error
  variable_set('archibald_upload_error', 0);

  //Reset menu cache
  menu_rebuild();



}

/**
 * menu callback
 * show the content partner delte form
 *
 * @param array $form
 * @param array $form_state
 * @param integer $content_partner_id
 *
 * @return array
 *   a drupal form
 */
function archibald_content_partner_delete_form($form, $form_state, $content_partner_id = NULL) {
  $content_partner = archibald_load_content_partner((INT) $content_partner_id);

  $content_partners = archibald_load_content_partner();


  $form['content_partner_id'] = array(
    '#type' => 'value',
    '#value' => $content_partner['content_partner_id'],
  );

  $form['name'] = array(
    '#markup' => t(
      'Do you really want to delete "@name"?',
      array(
        '@name' => $content_partner['name'],
      )
    ),
  );

  // Button for the corresponding actions
  $form['actions'] = array(
    '#type' => 'actions',
  );

  if (!empty($content_partner['usage_count'])) {
    $form['actions']['delete'] = array(
      '#markup' => t(
        'With this account there are @usage_count descriptions published.',
        array(
          '@usage_count' => $data['usage_count'],
        )
      ),
    );
  }
  elseif (count($content_partners) < 2) {
    $form['actions']['delete'] = array(
      '#markup' => t('The last account, cannot be deleted'),
    );
  }
  else {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/services/archibald/settings',
  );

  return $form;
}

/**
 * save archibald_content_partner_delete_form admin settings form for
 * content partner
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_partner_delete_form_submit(&$form, &$form_state) {

  if (!empty($form_state['values']['content_partner_id'])) {
    db_delete('archibald_settings_content_partner')
      ->condition('content_partner_id', $form_state['values']['content_partner_id'])
      ->execute();
  }

  $form_state['redirect'] = 'admin/config/services/archibald/settings';
}
