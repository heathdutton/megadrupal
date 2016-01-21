<?php
// $Id: webform_protected_downloads.form.inc,v 1.1.2.4 2010/11/03 00:21:43 berliner Exp $

/**
 * Form callback for the webform configuration subpage
 *
 * @param array $form_state 
 * @param object $node 
 * @return void
 */

function webform_protected_downloads_configuration_form($form, $form_state, $node) {
  $mandatory_field_name = webform_protected_downloads_get_mandatory_field_name();

  $mail_fields = array();
  foreach ($node->webform['components'] as $cid => $component) {
    if ($component['type'] == 'email' && $component[$mandatory_field_name]) {
      $mail_fields[$cid] = $component['name'];
    }
  }
  
  $private_files = $node->wpd['private_files'];
  $private_file_fields = webform_protected_downloads_node_get_private_file_fields($node);
  
  if (!count($mail_fields) || !count($private_files) || !count($private_file_fields)) {
    $checklist = array();
    $checklist[1] =  array(
      'title' => t('You need to add at least one mandatory email component to this webform.'),
      'scope' => t('Webform'),
      'href' => 'node/' . $node->nid . '/webform',
      'status' => count($mail_fields) > 0,
    );
    $checklist[2] =  array(
      'title' => t('You need to a add a file or image field to this content type and configure it to be private.'),
      'scope' => t('Fields'),
      'href' => 'admin/structure/types/manage/' . $node->type . '/fields',
      'status' => count($private_file_fields) > 0,
    );
    $checklist[3] =  array(
      'title' => t('You need to upload private files to this node.'),
      'scope' => t('Node %title', array('%title' => check_plain($node->title))),
      'href' => 'node/' . $node->nid . '/edit',
      'status' => count($private_files) > 0,
      'depends' => 2,
    );

    $header = array(NULL, '#', t('Scope'), t('Description'), t('Status'));
    $rows = array();
    foreach ($checklist as $i => $item) {
      if ($item['status']) {
        $status = t('OK');
      }
      elseif (isset($item['depends']) && !$checklist[$item['depends']]['status']) {
        $status = t('Blocked by step !step', array('!step' => $item['depends']));
      }
      else {
        $status = l(t('Correct now'), $item['href']);
      }
      $icon_path = $item['status'] ? 'misc/message-16-ok.png' : 'misc/message-16-error.png';
      $icon = theme('image', array('path' => $icon_path));
      $row = array(
        $icon,
        $i,
        $item['scope'],
        $item['title'],
        $status,
      );
      $rows[] = array(
        'data' => $row,
        'class' => array($item['status'] ? NULL : 'warning'),
      );
    }
    
    $form['not_available'] = array(
      '#type' => 'markup',
      '#markup' => t('There is a problem with you current setup. Please refer to the following checklist.<br /><br />!table', array(
        '!table' => theme('table', array(
          'header' => $header,
          'rows' => $rows,
          'attributes' => array('width' => '400px'))
      ))),
    );
        
    return $form;
  }
 
  // get existing configuration for this node if any
  $conf = webform_protected_downloads_get_configuration($node->nid);
  
  $form['#tree'] = TRUE;
  
  $form['node'] = array('#type' => 'value', '#value' => $node);
  
  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files'),
    '#theme' => 'webform_protected_downloads_configuration_form_file_list',
  );
  
  foreach ($private_files as $fid => $file) {
    $form['files'][$fid] = array(
      '#type' => 'checkbox',
      '#title' => $file->filename,
      '#file_object' => $file,
      '#default_value' => webform_protected_downloads_file_is_protected($node->nid, $file->fid),
    );
  }
  
  $form['access_verification'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access verification'),
  );
  
  $form['access_verification']['mail_field_cid'] = array(
    '#type' => 'select',
    '#title' => t('Mail confirmation to'),
    '#description' => t('Webform component that will be used for the confirmation mail'),
    '#options' => $mail_fields,
    '#default_value' => $conf ? $conf->mail_field_cid : NULL,
    '#required' => TRUE,
  );
  
  $form['access_verification']['mail_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From'),
    '#default_value' => $conf ? $conf->mail_from : '',
    '#description' => t('The <em>From</em> address for the confirmation mail. Leave empty to use the sites email address (%site_mail).', array('%site_mail' => variable_get('site_mail', NULL))),
  );
  
  $form['access_verification']['mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $conf ? $conf->mail_subject : t('Download link for [site:url]'),
    '#required' => TRUE,
  );
  
  $form['access_verification']['mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $conf ? $conf->mail_body : t("Dear visitor,\n\nThank you for your interest.\nPlease use the following link to download the files: [protected-downloads:download-url]\n\nThis link will be accessible until [protected-downloads:download-expires]. If you need access after the link expires, don't hesitate to revisit the download page on [site:url]"),
    '#rows' => 10,
    '#required' => TRUE,
  );
  
  if (module_exists('token')) {
    $form['access_verification']['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    if (webform_protected_downloads_get_webform_version() != '7.4') {
      $form['access_verification']['token_help']['#description'] = t('Besides the listed replacement patterns you can insert any placeholder of the form <em>%value[key]</em> to access values that the user has submitted in the webform.');
    }
    
    $form['access_verification']['token_help']['help'] = array(
      '#type' => 'markup',
      '#markup' => theme('token_tree', array('token_types' => array('webform_protected_downloads', 'node', 'submission'))),
    );
    
  }
  
  $form['access_verification']['access_type'] = array(
    '#type' => 'radios',
    '#title' => t('Access type'),
    '#options' => array(
      WEBFORM_PROTECTED_DOWNLOADS_ACCESS_TYPE_SINGLE => t('One-time access only'),
      WEBFORM_PROTECTED_DOWNLOADS_ACCESS_TYPE_EXPIRES => t('Access expires')
    ),
    '#default_value' => $conf ? $conf->access_type : WEBFORM_PROTECTED_DOWNLOADS_DEFAULT_ACCESS_TYPE,
    '#description' => t('<em>One-time access only</em> means that the download page will only stay accessible for the duration of the session expiration period that you must configure below. Also you should update the mail body to indicate that this is a one-time access link. Note that you can safely set the downloads expiration to <em>0</em> for <em>One-time access only</em> but you need to set the session expiration to a value that is high enough that the user has time to view / download the file(s).<br /><em>Access expires</em> means that the link stays accessible until the download expires and can be accessed from different computers and persons as long as they know the link.'),
  );
  
  $form['access_verification']['expiration_download'] = array(
    '#type' => 'textfield',
    '#title' => t('Downloads expire after (seconds)'),
    '#default_value' => $conf ? $conf->expiration_download : WEBFORM_PROTECTED_DOWNLOADS_DEFAULT_EXPIRATION_DOWNLOAD,
    '#size' => 10,
    '#description' => t('The time in seconds that the download should be available to the user, counting from form submission. Set to 0 for no limit.'),
  );
  
  $form['access_verification']['expiration_session'] = array(
    '#type' => 'textfield',
    '#title' => t('Session expires after (seconds)'),
    '#default_value' => $conf ? $conf->expiration_session : WEBFORM_PROTECTED_DOWNLOADS_DEFAULT_EXPIRATION_SESSION,
    '#size' => 10,
    '#description' => t('The time in seconds that the session should be left open for a user when he has accessed the download page. The result is that the protected file stays accessible using the direct file path. Set to 0 for no limit.'),
  );
  
  $form['access_verification']['retroactive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Retroactive'),
    '#description' => t("Allow access to files that have been protected after the user has initially submitted the form. If unchecked, users who have received the mail with the link to the download page won't be able to access files that have been protected after the mail has been send, unless they resubmit the form."),
    '#default_value' => $conf ? $conf->retroactive : WEBFORM_PROTECTED_DOWNLOADS_DEFAULT_RETROACTIVE,
  );
  
  $form['access_verification']['redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect to downloads page after form submission'),
    '#description' => t("Check this, if you want to redirect the user to the downloads page, directly after the webform has been submitted. This circumvents the mail verification, thus allowing <strong>any</strong> user to access the files simply by submitting the form. Use with care!"),
    '#default_value' => $conf ? $conf->redirect : WEBFORM_PROTECTED_DOWNLOADS_DEFAULT_REDIRECT,
  );
  
  $form['protected_download_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protected download page'),
  );
  
  $form['protected_download_page']['text_download_access'] = array(
    '#type' => 'text_format',
    '#title' => t('Success'),
    '#description' => t('This text will be shown above the file listing on the protected downloads page.'),
    '#default_value' => $conf ? $conf->text_download_access : t('Thank you for your interest. Below you will find the requested files.'),
    '#format' => $conf ? $conf->text_download_access_format : filter_default_format(),
    '#parents' => array('protected_download_page', 'text_download_access'),
    '#required' => TRUE,
  );
  
  if (module_exists('token')) {
    $form['protected_download_page']['token_help_text_download_access'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    if (webform_protected_downloads_get_webform_version() != '7.4') {
      $form['access_verification']['token_help']['#description'] = t('Besides the listed replacement patterns you can insert any placeholder of the form <em>%value[key]</em> to access values that the user has submitted in the webform.');
    }
    
    $form['protected_download_page']['token_help_text_download_access']['help'] = array(
      '#type' => 'markup',
      '#markup' => theme('token_tree', array('token_types' => array('webform_protected_downloads', 'node', 'submission'))),
    );
  }
  
  $form['protected_download_page']['text_download_noaccess'] = array(
    '#type' => 'text_format',
    '#title' => t('Access denied'),
    '#description' => t('This text will be shown if the access is denied, e.g. if the download link expired.'),
    '#default_value' => $conf ? $conf->text_download_noaccess : t('This link is not valid. If you think that this is an error, please contact the <a href="mailto:[site:mail]">site administrator</a>.'),
    '#format' => $conf ? $conf->text_download_noaccess_format : filter_default_format(),
    '#parents' => array('protected_download_page', 'text_download_noaccess'),
    '#required' => TRUE,
  );
  
  if (module_exists('token')) {
    $form['protected_download_page']['token_help_text_download_noaccess'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form['protected_download_page']['token_help_text_download_noaccess']['help'] = array(
      '#type' => 'markup',
      '#markup' => theme('token_tree', array('token_types' => array('node'))),
    );
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}

/**
 * Implementation of hook_submit().
 *
 * @param array $form 
 * @param array $form_state 
 * @return void
 */

function webform_protected_downloads_configuration_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $node = $values['node'];
  
  foreach ($values['files'] as $fid => $protected) {
    $protected = (boolean) $protected;
    webform_protected_downloads_file_set_protected(
      $node->nid,
      $fid,
      $protected
    );
  }
  
  webform_protected_downloads_set_configuration(
    $node->nid,
    array(
      'mail_field_cid' => $values['access_verification']['mail_field_cid'],
      'mail_from' => $values['access_verification']['mail_from'],
      'mail_subject' => $values['access_verification']['mail_subject'],
      'mail_body' => $values['access_verification']['mail_body'],
      'access_type' => $values['access_verification']['access_type'],
      'expiration_download' => $values['access_verification']['expiration_download'],
      'expiration_session' => $values['access_verification']['expiration_session'],
      'retroactive' => $values['access_verification']['retroactive'],
      'redirect' => $values['access_verification']['redirect'],
      'text_download_access' => $values['protected_download_page']['text_download_access']['value'],
      'text_download_access_format' => $values['protected_download_page']['text_download_access']['format'],
      'text_download_noaccess' => $values['protected_download_page']['text_download_noaccess']['value'],
      'text_download_noaccess_format' => $values['protected_download_page']['text_download_noaccess']['format'],
    )
  );
  drupal_set_message('Your changes have been saved');
}

/**
 * Theme function for the configuration form
 *
 * @param array $form 
 * @return void
 */

function theme_webform_protected_downloads_configuration_form_file_list($variables) {
  
  $header = array(
    t('Filename'),
    t('Size'),
    t('Protected')
  );
  $rows = array();
  
  foreach ($variables['element'] as $key => $element) {
    if (!empty($key) && (is_numeric($key) || $key[0] != '#')) {
      $file = $element['#file_object'];
      unset($element['#title']);
      $rows[] = array(
        theme('file_link', array('file' => $file)),
        format_size($file->filesize),
        array('data' => $element),
      );
    }
  }
  return theme('table', array('header' => $header, 'rows' => $rows)) . drupal_render($element);
}
