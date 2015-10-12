<?php

/**
 * @file
 * Creates forms for the SavetoFTP module
 * Returns an administration form, a form for uploading files to ftp, and a form for deleting files from ftp
 */

/**
 * An implementation of Form API form form() function.
 * Returns a form where user can enter FTP login credentials and upload a file.
 */

function savetoftp_publish_form() {
  $node = menu_get_object();
  $save_to_ftp_path = '/' . drupal_lookup_path('alias', 'node/' . $node->nid);
  $form['header'] = array(
    '#value' => t('<h2>FTP Server Settings for saving a file to the FTP server</h2><strong>You must save any changes FIRST before publishing to the FTP server or your changes might be lost.</strong>'),
  );
  $form['ftp_server'] = array(
    '#type' => 'textfield',
    '#default_value' => check_plain(variable_get('savetoftp_server')),
    '#maxlength' => 50,
    '#size' => 20,
    '#title' => t('FTP Server'),
    '#prefix' => '<div style="float:left;">',
    '#suffix' => '</div>',
  );
  $form['ftp_user'] = array(
    '#type' => 'textfield',
    '#default_value' => check_plain(variable_get('savetoftp_username', '')),
    '#maxlength' => 50,
    '#size' => 10,
    '#title' => t('FTP Username'),
    '#prefix' => '<div style="float:left;">',
    '#suffix' => '</div>',
  );
  $form['ftp_pass'] = array(
    '#type' => 'password',
    '#default_value' => check_plain(variable_get('savetoftp_pass', '')),
    '#maxlength' => 50,
    '#size' => 10,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('FTP Password'),
    '#prefix' => '<div style="float:left;">',
    '#suffix' => '</div>',
  );
  $form['ftp_direct'] = array(
    '#type' => 'textfield',
    '#default_value' => check_plain(variable_get('ftp_directory', $save_to_ftp_path)),
    '#maxlength' => 50,
    '#size' => 20,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('FTP Directory'),
    '#prefix' => '<div>',
    '#suffix' => '</div><div style="clear:both;"></div>',
  );
  $form['ftp_markup'] = array(
    '#value' => t('<p><strong>FTP Directory is Optional:</strong> By default this is taken from the URL Path. To put your html file in a specific directory on the FTP server, put the full directory here without a trailing slash, e.g., my/files/ or public_html/home, etc.</p>'),
  );
  $form['ftp_all'] = array(
    '#type' => 'checkbox',
    '#default_value' => check_plain(variable_get('ftp_all', 1)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Copy all images, javascript, and css to the ftp server'),
    '#description' => t('<strong>This may take a long time. If your server is unable to handle the length of time it takes to copy all the files via FTP, then you may receive a timeout</strong>. In addition, this will change the links to your css, js, and image files in the html to point to the new location.'),
  );
  $form['ftp_links'] = array(
    '#type' => 'checkbox',
    '#default_value' => check_plain(variable_get('ftp_links', 0)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Change the css, js, and image references to be the full link to this server.'),
    '#description' => t('Select this option if you have are running Drupal on a server that is publicly accessible. This way, the file you put on your FTP Server will link to the js, css, and images on this server.)'),
  );
  $form['ftp_anon'] = array(
    '#type' => 'checkbox',
    '#default_value' => check_plain(variable_get('ftp_anon', 0)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Retrieve the page as an anonymous user.'),
    '#description' => t('Select this option if you want to get the page as an anonymous user would see it as opposed to how you would see it.)'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Publish to FTP'),
  );
  $form['cancel'] = array(
    '#type' => 'markup',
    '#value' => '<button type="button" onclick="document.getElementById(\'ftp1\').style.display = \'none\'">Cancel</button>',
  );
  return $form;
}

/**
 * An implementation of Form API form form() function.
 * Returns a form where user can enter FTP login credentials and delete a file.
 */

function savetoftp_delete_form() {
  $form['#prefix'] = t('<h2>FTP Server Settings for deleting a file from the FTP server</h2>');
  $form['ftp_server1'] = array(
    '#type' => 'textfield',
    '#default_value' => check_plain(variable_get('savetoftp_server')),
    '#maxlength' => 50,
    '#title' => t('FTP Server'),
  );
  $form['ftp_user1'] = array(
    '#type' => 'textfield',
    '#default_value' => check_plain(variable_get('savetoftp_username')),
    '#maxlength' => 50,
    '#size' => 25,
    '#title' => t('FTP Username'),
  );
  $form['ftp_pass1'] = array(
    '#type' => 'password',
    '#default_value' => check_plain(variable_get('savetoftp_pass')),
    '#maxlength' => 50,
    '#size' => 15,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('FTP Password'),
  );
  $form['ftp_direct1'] = array(
    '#type' => 'textfield',
    '#default_value' => check_plain(variable_get('ftp_directory1')),
    '#maxlength' => 50,
    '#size' => 25,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => 'FTP Directory',
    '#description' => t('Enter your default directory, this will most likely be public_html or blank'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete file from FTP'),
  );
  $form['cancel'] = array(
    '#type' => 'markup',
    '#value' => '<button type="button" onclick="document.getElementById(\'ftp2\').style.display = \'none\'">Cancel</button>',
  );
  return $form;
}

/**
 * Admin settings form
 * Allows users to enter in the html fields
 * @param $form form object
 * @param $form_state form state object
 * @return $form object
 */

function savetoftp_admin_settings($form, &$form_state) {
  $form['form_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('FTP Server Defaults'),
    '#description' => t('Fill these values in to have them appear as defaults in the Publish to FTP forms'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['form_defaults']['savetoftp_username'] = array(
    '#type' => 'textfield',
    '#title' => t('FTP Username'),
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => variable_get('savetoftp_username', ''),
  );
  $form['form_defaults']['savetoftp_pass'] = array(
    '#type' => 'password',
    '#title' => t('FTP Password'),
    '#description' => t('Caution: This will be stored in plain text in the database'),
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => variable_get('savetoftp_pass', ''),
  );
  $form['form_defaults']['savetoftp_server'] = array(
    '#type' => 'textfield',
    '#title' => t('FTP Server'),
    '#description' => t('The FTP Server you will be connecting to, e.g., ftp.archive.org'),
    '#size' => 20,
    '#maxlength' => 255,
    '#default_value' => variable_get('savetoftp_server', ''),
  );
  $form['savetoftp_date'] = array(
    '#type' => 'checkbox',
    '#title' => t('Date'),
    '#description' => t('If checked, this will add a Last updated date at the bottom of the page'),
    '#default_value' => variable_get('savetoftp_date', '0'),
  );
  return system_settings_form($form);
}
