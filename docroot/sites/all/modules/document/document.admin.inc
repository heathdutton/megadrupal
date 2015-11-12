<?php

require_once (drupal_get_path('module', 'document') . '/document.inc');

function document_settings_form($form, $form_state) {
  drupal_add_css(drupal_get_path('module', 'document') . '/document.css');

  drupal_add_js(drupal_get_path('module', 'document') . '/document.js');
  _document_register_validation_token();
  drupal_add_js('jQuery(document).ajaxSend(function(event, request, settings) {
     jQuery("#document_loading").show();
   });

   jQuery(document).ajaxComplete(function(event, request, settings) {
     jQuery("#document_loading").hide();
   });', 'inline');

  $form['document_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Documents Path'),
      '#description' => t('Path where the Documents should be stored. This path would be considered relative the Drupal\'s File System Path. Changing this path would not automatically move existing documents.'),
      '#default_value' => variable_get('document_path', ''));

  $form['document_allowed_extensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Allowed extensions'),
      '#description' => t('A space separated list of allowed extensions.'),
	  '#required' => TRUE,
      '#default_value' => _document_get_allowed_extensions());

  $form['document_allow_external'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('document_allow_external', FALSE),
      '#title' => t('Allow external document Urls'),
      '#description' => t('Check to allow users to specify an external Url for a document. Unchecking would force users to upload a file while creating a document.'));

  $form['document_allow_websearch'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('document_allow_websearch', FALSE),
      '#title' => t('Allow Web search'),
      '#description' => t('Check to add an extra option for searching web while searching for documents. Currently, Google would be used for web search. Future releases would allow you to choose your search provider.'));

  $form['document_types_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Document Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE);

  $form['document_types_fieldset']['document_types'] = array(
      '#type' => 'select',
      '#multiple' => true,
      '#title' => t('Document Types'),
      '#description' => t('Document Types.'),
      '#size' => 10,
      '#options' => document_get_types(TRUE, TRUE));

  $form['document_types_fieldset']['document_type'] = array(
      '#type' => 'textfield',
      '#title' => t('Add Document Type'),
      '#description' => t('Add Document Type.'));

  $form['document_types_fieldset']['document_type_add'] = array(
      '#type' => 'button',
      '#value' => t('Add Document Type'),
      '#prefix' => '<div style="float: left">',
      '#suffix' => theme_image(array(
            'path' => document_image_url('loading.gif'), 
            'alt' => t('Loading...'),
            'title' => t('Loading...'),
            'attributes' => array(
                'id' => 'document_loading',
                'style' => 'display: none',
                'width' => 16,
                'height' => 16,
                )
            )),
      '#attributes' => array(
          'onclick' => 'return(doc.add_type());'));

  $form['document_types_fieldset']['document_type_delete'] = array(
      '#type' => 'button',
      '#value' => t('Delete Selected Types'),
      '#suffix' => '</div>',
      '#attributes' => array(
          'onclick' => 'return(doc.delete_types());'));

  $email_token_help = t('Available variables are:') . ' !site, !doc_link, !node_title, !node_title_link, !node_link, !uri, !uri_brief, !username, !mailto, !date.';

  $form['document_email_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Email Settings'),
      '#description' => t('Customize e-mail message sent when Document uploaded by a User is published.') . ' ' . $email_token_help,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE);

  $form['document_email_fieldset']['document_publish_email'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('document_publish_email', TRUE),
      '#title' => t('Email on Publishing a Document'),
      '#description' => t('Email the Creator of the Document when it is published by administrators.'));

  $form['document_email_fieldset']['document_publish_subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Publish Email Subject'),
      '#description' => t('Subject template for the e-mail sent when a document is published.'),
      '#required' => TRUE,
      '#maxlength' => 180,
      '#default_value' => document_mail_text('publish_subject'));

  $form['document_email_fieldset']['document_publish_body'] = array(
      '#type' => 'textarea',
      '#title' => t('Publish Email Body'),
      '#description' => t('Body template for the e-mail sent when a document is published.'),
      '#required' => TRUE,
      '#rows' => 15,
      '#default_value' => document_mail_text('publish_body'));

  $form['#submit'][] = 'document_settings_submit_handler';

  return system_settings_form($form);
}

function document_settings_submit_handler($form, &$form_state) {
  $path = $form_state['values']['document_path'];
  variable_set('document_path', $path);
  variable_set('document_allowed_extensions', $form_state['values']['document_allowed_extensions']);
  variable_set('document_allow_external', $form_state['values']['document_allow_external']);
  variable_set('document_allow_websearch', $form_state['values']['document_allow_websearch']);

  variable_set('document_publish_email', $form_state['values']['document_publish_email']);
  variable_set('document_publish_subject', $form_state['values']['document_publish_subject']);
  variable_set('document_publish_body', $form_state['values']['document_publish_body']);

  $full = document_get_path();
  file_prepare_directory($full, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
}

function document_moderate_form($form, $form_state) {
  drupal_add_css(drupal_get_path('module', 'document') . '/document.css');

  drupal_add_js(drupal_get_path('module', 'document') . '/document.js');
  _document_register_validation_token();
  _document_register_status();

  $headers = array(
      '',
      array('data' => t('Type'), 'field' => 'd.type'),
      array(
          'data' => t('Title'),
          'field' => 'title',
          'sort' => 'asc'),
      array('data' => t('Author'), 'field' => 'author'),
      array(
          'data' => t('Year of Publication'),
          'field' => 'publish_year'),
      'Keywords',
      '');

  $query = db_select('node', 'n');
  $query->join('document', 'd', 'n.vid = d.vid');
  $query
        ->condition('n.status', DOCUMENT_STATUS_PUBLISHED, '<>')
        ->extend('PagerDefault') 	//Pager Extender
          ->limit(10)				//10 results per page
        ->extend('TableSort')		//Sorting Extender
          ->orderByHeader($headers)//Field to sort on is picked from $header
        ->fields ('n')
        ->fields('d');
  $results = $query
        ->execute();

  $imgPublish = theme_image(array(
      'path' => document_image_url('spacer.gif'), 
      'alt' => t('Publish'),
      'title' => t('Publish'),
      'attributes' => array(
          'onclick' => 'doc.changeDocStatus(this, %1$d, \'icon-publish\', true);',
          'class' => 'icon-publish',
          'width' => 16,
          'height' => 16,
          )
      ));
  $imgDelete = theme_image(array(
      'path' => document_image_url('spacer.gif'), 
      'alt' => t('Delete'),
      'title' => t('Delete'),
      'attributes' => array(
          'onclick' => 'doc.deleteDoc(this, %1$d, \'icon-delete\');',
          'class' => 'icon-delete',
          'width' => 16,
          'height' => 16,
          )
      ));

  $rows = array();
  foreach($results as $doc) {
    $row = array(
        sprintf($imgPublish . '&nbsp;&nbsp;&nbsp;' . $imgDelete, $doc->nid),
        check_plain($doc->type),
        l($doc->title, 'node/' . $doc->nid),
        check_plain($doc->author),
        $doc->publish_year,
        check_plain($doc->keywords),
        l('Download', $doc->url, array(
            'attributes' => array(
                'target' => '_blank'))));

    $rows[] = $row;
  }
  
  $table = theme('table', array(
      'header' => $headers, 
      'rows' => $rows
      )
    );

  $form['document_moderate_table'] = array(
      '#type' => 'markup',
      '#markup' => $table);

  return ($form);
}
