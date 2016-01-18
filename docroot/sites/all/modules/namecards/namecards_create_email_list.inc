<?php

/**
 * @file
 * This file contains functions pertaining to creating email lists for module namecards
 *
 * Copyright 2010 Ben Gunn
 *
 * Licensed under the GNU Public License
 */

/**
 * Builds page for menu item 'namecards/email_list'
 */
/*function namecards_email_list() {
  $output = '';

  // Embed modalframe messages div.
  $output .= '<div class="namecards-modalframe-messages"></div>';

  // Embed email list form.
  $email_list_form = drupal_get_form('namecards_email_list_form');
  $output .= drupal_render($email_list_form);

  return $output;
}*/

/**
 * Create email list form.
 */
function namecards_email_list_form($form, &$form_state) {
  global $user;
  
  // Add js libraries required for ctools modal.
  _namecards_ctools_modal_parent();
  
  $show_create_email_list_form = (isset($form_state['namecards']['show_create_email_list_form']) ? $form_state['namecards']['show_create_email_list_form'] : TRUE);

  if ($show_create_email_list_form) {
    // Add search box.
    $form['namecards_live_search_textfield'] = array(
      '#type' => 'textfield',
      '#default_value' => (!empty($form_state['namecards']['email_list_form_search_terms'])) ? $form_state['namecards']['email_list_form_search_terms'] : '',
      '#size' => 30,
      '#maxlength' => 400,
    );
    $form['namecards_live_search_search_button'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
    );
    $form['namecards_live_search_clear_button'] = array(
      '#type' => 'submit',
      '#value' => t('Clear'),
    );

    // Build database query.
    $query = db_select('node', 'n');
    // Join tables.
    $tbl_given_name_alias = $query->leftJoin('field_data_namecards_namecard_given_name', 'given', 'n.nid = %alias.entity_id');
    $tbl_nickname_alias = $query->leftJoin('field_data_namecards_namecard_nickname', 'nick', 'n.nid = %alias.entity_id');
    $tbl_org_alias = $query->leftJoin('field_data_namecards_namecard_organization', 'org', 'n.nid = %alias.entity_id');
    $tbl_email_alias = $query->leftJoin('field_data_namecards_namecard_comp_email', 'email', 'n.nid = %alias.entity_id');
    $tbl_org_name_alias = $query->leftJoin('node', 'org_node', '%alias.nid = org.namecards_namecard_organization_nid');
    $tbl_full_content_alias = $query->leftJoin('field_data_namecards_namecard_full_content', 'full_content', 'n.nid = %alias.entity_id');
    $tbl_full_public_alias = $query->leftJoin('field_data_namecards_namecard_public', 'public', 'n.nid = %alias.entity_id');
    // Select fields.
    $query->addField('n', 'nid');
    $fld_surname_alias = $query->addField('n', 'title', 'surname');
    $fld_given_name_alias = $query->addField($tbl_given_name_alias, 'namecards_namecard_given_name_value', 'given_name');
    $fld_nickname_alias = $query->addField($tbl_nickname_alias, 'namecards_namecard_nickname_value', 'nickname');
    $fld_org_alias = $query->addField($tbl_org_alias, 'namecards_namecard_organization_nid', 'organization');
    $fld_org_name_alias = $query->addField($tbl_org_name_alias, 'title', 'organization_name');
    $fld_email_alias = $query->addField($tbl_email_alias, 'namecards_namecard_comp_email_value', 'email_address');
    $fld_full_content_alias = $query->addField($tbl_full_content_alias, 'namecards_namecard_full_content_value', 'full_content_fld');
    // Set conditionals.
    $query->where('n.type = :type', array(':type' => 'namecards_namecard'));
    $query->where("$tbl_email_alias.namecards_namecard_comp_email_value <> ''", array());
    $query->where(':fld_email_alias <> \'\'', array(':fld_email_alias' => $fld_email_alias));

    // Create where clause string in order to filter by search terms.
    $search_term_where_clause = ' ';
    if (!empty($form_state['namecards']['email_list_form_search_terms'])) {
      $search_terms = array();
      $search_phrase = array();
      $placeholders = array();
      // Extract search terms.
      $search_terms = explode(' ', trim($form_state['namecards']['email_list_form_search_terms']));

      $num = count($search_terms);
      for ($i = 0; $i < $num; $i++) {
        // Build the where clause.
        // TODO: There is a strange bug in which the search terms are not
        // converted to upper in the sql statement. Only seems to apply
        // to nodes which have been imported.  Nodes created by the add
        // contact form appear uneffected. As a temporary solution I convert
        // the search terms to upper case using php rather than relying on
        // database to do it.
        $placeholders[':search_term_' . $i] = '%' . strtoupper($search_terms[$i]) . '%';
        $conditional_statement[] = "UPPER($tbl_full_content_alias.namecards_namecard_full_content_value) LIKE UPPER(:search_term_$i)";
      }
      // Merge conditional statement fragments into a single string.
      $conditional_statement = implode(' AND ', $conditional_statement);
      $query->where($conditional_statement, $placeholders);
    }

    // Add filter to ensure only public contacts or own contacts are displayed.
    $query->condition(db_or()->condition($tbl_full_public_alias . '.namecards_namecard_public_value', 1)->condition('n.uid', $user->uid));
    
    // Execute.
    $results = $query->execute();

    $checkboxes = array();

    foreach ($results as $result) {
      $checkboxes[$result->nid] = '';
      $form[$result->nid]['surname'] = array(
        '#markup' => check_plain($result->{$fld_surname_alias}),
      );
      $form[$result->nid]['given_name'] = array(
        '#markup' => check_plain($result->{$fld_given_name_alias}),
      );
      $form[$result->nid]['nickname'] = array(
        '#markup' => check_plain($result->{$fld_nickname_alias}),
      );
      $form[$result->nid]['organization'] = array(
        '#markup' => check_plain($result->{$fld_org_name_alias}),
      );
      $form[$result->nid]['email'] = array(
        '#markup' => check_plain($result->{$fld_email_alias}),
      );
    }

    $form['checkboxes'] = array(
      '#type' => 'checkboxes',
      '#options' => $checkboxes,
      '#default_value' => (isset($form_state['namecards']['nids'])) ? $form_state['namecards']['nids'] : array(),
    );

    // Only display createlist widgets if there are contacts with email addresses.
    if (count($checkboxes) > 0) {
      $form['create_list'] = array(
        '#type' => 'fieldset',
        '#title' => t('Create mailing list'),
        '#collapsible' => FALSE,
        '#access' => TRUE,
      );
      $form['create_list']['email_client_type'] = array(
        '#type' => 'select',
        '#title' => t('Email program'),
        '#options' => array(
          1 => 'MS Outlook',
          2 => 'Thunder Bird',
        ),
        '#default_option' => 1,
        '#description' => t('Select the email program you will use to send this mailing list'),
      );
      $form['create_list']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Create'),
      );
    }

    // Add form themeing function which renders form as table.
    $form['#theme'] = 'namecards_create_email_list_theme';

    // Set session var for displaying search textbox.  This would be better placed
    // in form submit function but looks like session vars cannot be set from the
    // form submit function. So we place here instead.
    $_SESSION['namecards_email_list_show_search_box'] = 0;
  }
  else {
    // Get nids of selected contacts.
    $selected_nids = array_filter($form_state['values']['checkboxes'], '_namecards_create_email_list_get_nids');

    if (!empty($selected_nids)) {
      $results = db_query("SELECT fdnne.namecards_namecard_email_value AS email FROM {field_data_namecards_namecard_email} fdnne WHERE fdnne.entity_id IN (:placeholder)", array(':placeholder' => $selected_nids));
    }

    // Set email client name string and format of email address separation character
    switch ((int)$form_state['values']['email_client_type']) {
      case 1:
        // MS Outlook
        $email_client_name = "Microsoft Outlook"; //Set text to appear in email list title
        $sep_char = ';'; //Set separation char for email list
        break;
      case 2:
        // Thunder bird
        $email_client_name = "Thunderbird"; //Set text to appear in email list title
        $sep_char = ',';  //Set separation char for email list
        break;
    }

    // Process email addresses to create final email address list content formatted for specified email client type.
    $email_addresses = array();
    foreach ($results as $result) {
      $email_addresses[] = $result->email;
    }
    $email_list_content = '';
    $number_of_email_addresses = count($email_addresses);
    for ($i = 0; $i < $number_of_email_addresses; $i++) {
      if ($i!= $number_of_email_addresses - 1) { // Is not the final email address so add seperator
        $email_list_content .= $email_addresses[$i] . $sep_char .' ';
      }
      else { // Is final email address so don't add separator
        $email_list_content .= $email_addresses[$i];
      }
    }

    // Add form elements.
    $form['email_list'] = array(
      '#title' => t('Email mailing list'),
      '#type' => 'textarea',
      '#description' => t('Mailing list formatted for use with @email_client.  To use simply copy then paste this list into the recipient section of your email message.', array('@email_client' => $email_client_name)),
      '#default_value' => $email_list_content,
      '#resizable' => TRUE,
      '#rows' => 5,
      '#attributes' => array(
        'id' => 'namecards-email-list-content-textarea',
      ),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Back'),
    );

    // Store the nids of the selected nodes.  These will be used to rebuild the create email list form.
    $form_state['namecards']['nids'] = $selected_nids;

    // Set session var for displaying search textbox.  This would be better placed
    // in form submit function but looks like session vars cannot be set from the
    // form submit function. So we place here instead.
    $_SESSION['namecards_email_list_show_search_box'] = 1;
  }

  return $form;
}

/**
 * Form submit function
 */
function namecards_email_list_form_submit($form, &$form_state) {
  switch ($form_state['clicked_button']['#value']) {
    case t('Back'):
      // Rebuild to email selection form.
      $form_state['namecards']['show_create_email_list_form'] = TRUE;
      $form_state['rebuild'] = TRUE;
      break;
    case t('Search'):
      // Save search terms to form object.
      drupal_set_message('Save search terms in form object');
      $form_state['namecards']['email_list_form_search_terms'] = $form_state['values']['namecards_live_search_textfield'];
      $form_state['rebuild'] = TRUE;
      break;
    case t('Clear'):
      // Clear all search terms.
      $form_state['namecards']['email_list_form_search_terms'] = NULL;
      break;
    case t('Create'):
      // Create formatted email list.
      $form_state['namecards']['show_create_email_list_form'] = FALSE;
      $form_state['rebuild'] = TRUE;
      break;
    default:
      break;
  }
}

/**
 * Email list form validation
 */
function namecards_email_list_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Create')) {
    $selected_nids = array_values(array_filter($form_state['values']['checkboxes']));
    if (count($selected_nids) < 1) {
      form_set_error('checkboxes', t('You must select at least one contact from the provided list.'));
      drupal_add_js(drupal_get_path('module', 'namecards') . '/js/namecards_highlight_checkboxes.js');
    }
  }
}

/**
 * Callback function used in call to filter_array().
 *
 * Check if form checkbox is selected by evaluating whether checkbox
 * value is an integer greater than zero (i.e. is selected).
 * @see namecards_email_list_form().
 */
function _namecards_create_email_list_get_nids($var) {
  if (is_numeric($var) && (int)$var > 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Theme create email list form
 */
function theme_namecards_create_email_list_theme($variables) {
  $form = $variables['form'];
  $rows = array();
  foreach (element_children($form['checkboxes']) as $nid) {
    $row = array();
    $row[] = drupal_render($form['checkboxes'][$nid]);
    $row[] = drupal_render($form[$nid]['surname']);
    $row[] = drupal_render($form[$nid]['given_name']);
    $row[] = drupal_render($form[$nid]['nickname']);
    $row[] = drupal_render($form[$nid]['organization']);
    $row[] = drupal_render($form[$nid]['email']);
    $rows[] = $row;
  }

  if (count($rows) > 0) {
    $header = array(theme('namecards_select_table_column_checkbox'), t('Surname'), t('Given name'), t('Nickname'), t('Organization'), t('Email'));
  }
  else {
    $header = array(t('Surname'), t('Given name'), t('Nickname'), t('Email'));
    $row = array();
    $row[] = array(
      'data' => '<strong>' . t('No contacts with email addresses were found') . '</strong>',
      'colspan' => 4,
      'style' => 'text-align:center'
    );
    $rows[] = $row;
  }

  // Set table tag attributes
  $attributes = array(
    'class' => array('namecards-create-email-list-table'),
  );

  $output = '<div class="namecards-create-email-list-wrapper">';
  $output .= drupal_render($form['namecards_live_search_textfield']);
  $output .= drupal_render($form['namecards_live_search_search_button']);
  $output .= drupal_render($form['namecards_live_search_clear_button']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => $attributes));
  $output .= drupal_render_children($form);
  $output .= '</div>';
  return $output;
}

/*
 * Highlights checkboxes in email_list_form.  Used if user submits
 * form without selecting any checkboxes.
 */
function theme_namecards_highlight_checkboxes_javascript() {
  drupal_add_js(drupal_get_path('module', 'namecards') . '/js/namecards_highlight_checkboxes.js');
}