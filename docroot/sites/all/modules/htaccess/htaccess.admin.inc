<?php

/**
 * @file
 * Administration pages for the Htaccess module.
 */

/**
 * Admin settings.
 */
function htaccess_settings($form, $form_state) {
  $form['htaccess_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $htaccess_settings_url_prefix_redirection_options = array(
    'without_www' => t('Without WWW prefix'),
    'with_www' => t('With WWW prefix'),
  );

  $form['htaccess_settings']['htaccess_settings_url_prefix_redirection'] = array(
    '#type' => 'radios',
    '#title' => t('URL prefix redirection'),
    '#description' => t('Use one of the following options to redirect users to your preferred
    URL, either <strong>with</strong> or <strong>without</strong> the \'www.\' prefix. Default: without.'),
    '#options' => $htaccess_settings_url_prefix_redirection_options,
    '#default_value' => variable_get('htaccess_settings_url_prefix_redirection', 'without_www'),
  );

  $htaccess_settings_symlinks_options = array(
    'FollowSymLinks' => t('+FollowSymLinks'),
    'SymLinksifOwnerMatch' => t('+SymLinksifOwnerMatch'),
  );

  $form['htaccess_settings']['htaccess_settings_symlinks'] = array(
    '#type' => 'radios',
    '#title' => t('Symbolic links'),
    '#description' => t('Define the Apache\'s right options to access to parts of the filesystem. Default: +FollowSymLinks.<br />For more informations, see <a href="@link_apache" target="_blank">http://httpd.apache.org/docs/2.2/urlmapping.html#outside</a>.', array('@link_apache' => url('http://httpd.apache.org/docs/2.2/urlmapping.html#outside'))),
    '#options' => $htaccess_settings_symlinks_options,
    '#default_value' => variable_get('htaccess_settings_symlinks', 'FollowSymLinks'),
  );

  $form['htaccess_settings_custom'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['htaccess_settings_custom']['htaccess_settings_custom_settings'] = array(
    '#type' => 'textarea',
    '#description' => t('Copy/paste below your custom settings (redirections, rewrite rules etc..). These will be added before the Drupal rules.'),
    '#default_value' => variable_get('htaccess_settings_custom_settings', ''),
  );

  $form['htaccess_settings_boost_module'] = array(
    '#type' => 'fieldset',
    '#title' => t('Boost'),
    '#description' => t('The Boost module is a static file caching tool to improve performance.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $htaccess_settings_boost_module_readonly = (!module_exists('boost') ? TRUE : FALSE);
  $htaccess_settings_boost_module_description = ($htaccess_settings_boost_module_readonly ? t('Boost is currently not installed. You can download it at <a href="@link_dl_boost" target="_blank">https://drupal.org/project/boost</a>.', array('@link_dl_boost' => url('https://drupal.org/project/boost'))) : t('If enable, copy past the <a href="@link_boost">Boost rules</a>.', array('@link_boost' => url('admin/config/system/boost'))));

  $form['htaccess_settings_boost_module']['htaccess_settings_boost_module_rules'] = array(
    '#type' => 'textarea',
    '#title' => t('Rules'),
    '#description' => $htaccess_settings_boost_module_description,
    '#default_value' => variable_get('htaccess_settings_boost_module_rules', ''),
    '#disabled' => $htaccess_settings_boost_module_readonly,
  );

  return system_settings_form($form);
}

/**
 * Admin htaccess generate form.
 */
function htaccess_generate($form, $form_state) {

  $form['htaccess_settings_generate_settings'] = array(
  '#type' => 'fieldset',
  '#title' => t('Informations'),
  '#description' => t('The htaccess\' settings will be based on what you entered in the settings tab.'),
  );

  $form['htaccess_settings_generate_profile'] = array(
  '#type' => 'fieldset',
  '#title' => t('Profile'),
  );

  $form['htaccess_settings_generate_profile']['htaccess_settings_generate_name'] = array(
  '#type' => 'textfield',
  '#title' => t('Name'),
  '#description' => t('Name of the htaccess profile: must be lowercase and without any special character.'),
  '#default_value' => '',
  '#required' => TRUE,
  );

  $form['htaccess_settings_generate_profile']['htaccess_settings_generate_description'] = array(
  '#type' => 'textfield',
  '#title' => t('Description'),
  '#description' => t('A short description of the htaccess usage.'),
  '#default_value' => '',
  );

  $form['htaccess_settings_generate']['submit'] = array(
  '#type' => 'submit',
  '#value' => t('Generate htaccess'),
  '#validate' => array('htaccess_admin_settings_generate_validate'),
  '#submit' => array('htaccess_admin_settings_generate_submit'),
  );

  return $form;

}

/**
 * Admin htaccess deployment page.
 */
function htaccess_deployment($form, $form_state) {
  // Get the current htaccess deployed
  $htaccess_current = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('deployed', 1, '=')
    ->execute()
    ->fetchAssoc();

  if($htaccess_current){
    $current = $htaccess_current['name'];
  }
  else{
    $current = t('none');
  }

  $form['htaccess_settings_current'] = array(
  '#type' => 'fieldset',
  '#title' => t('Status'),
  '#description' => t('Current deployed profile: <b>@current</b>.', array('@current' => $current)),
  );


  $form['htaccess_settings_version'] = array(
    '#prefix' => '<table>',
    '#suffix' => '</table>',
    '#tree' => TRUE,
    '#weight' => '110',
  );

  $form['htaccess_settings_version']['htaccess_settings_version_header'] = array(
  '#markup' => '<thead>
    <tr>
      <th>'.t('ID').'</th>
      <th>'.t('Created date').'</th>
      <th>'.t('Name').'</th>
      <th>'.t('Description').'</th>
      <th>'.t('Operations').'</th>
    </tr>
  </thead>',
  );

  $htaccess = db_select('htaccess', 'h')
    ->fields('h')
    ->execute()
    ->fetchAll();

  $htaccess_count = count($htaccess);

  for ($i=0; $i<$htaccess_count; $i++) {
    $form['htaccess_settings_version']['row_' . $i] = array(
      '#prefix' => '<tr class="'.($i % 2 ? "odd" : "even").'">',
      '#suffix' => '</tr>',
    );
    $form['htaccess_settings_version']['row_' . $i]['htaccess_settings_version_number'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => $htaccess[$i]->id,
    );
    $form['htaccess_settings_version']['row_' . $i]['htaccess_settings_version_created'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => format_date($htaccess[$i]->created, 'short'),
    );
    $form['htaccess_settings_version']['row_' . $i]['htaccess_settings_version_name'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => $htaccess[$i]->name,
    );
    $form['htaccess_settings_version']['row_' . $i]['htaccess_settings_version_description'] = array(
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      '#markup' => $htaccess[$i]->description,
    );
    $form['htaccess_settings_version']['row_' . $i]['htaccess_settings_version_operation'] = array(
      '#markup' => l(t('View'), 'admin/config/system/htaccess/deployment/view/'.$htaccess[$i]->id.'') . ' ' . l(t('Deploy'), 'admin/config/system/htaccess/deployment/deploy/'.$htaccess[$i]->id.'') . ' ' . l(t('Download'), 'admin/config/system/htaccess/deployment/download/'.$htaccess[$i]->id.'') . ' ' . l(t('Delete'), 'admin/config/system/htaccess/deployment/delete/'.$htaccess[$i]->id.''),
      '#prefix' => '<td>',
      '#suffix' => '</td>',
    );
  }
  return $form;
}

/**
 * Admin htaccess confirm form
 */
function htaccess_confirm_form($form, $form_state, $action, $id) {
  $htaccess_data = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('id', array(':id' => $id),'IN')
    ->execute()
    ->fetchAssoc();

  $profile_name = $htaccess_data['name'];
  $profile_id = $id;

  $form['action'] = array(
    '#type' => 'value',
    '#value' => $action,
  );

  $form['profile_id'] = array(
    '#type' => 'value',
    '#value' => $profile_id,
  );

 switch ($action) {
  case 'deploy':
    $confirm_form = confirm_form($form,
    t('Are you sure you want to deploy the htaccess profile %profile_name?', array('%profile_name' => $profile_name)), 'admin/config/system/htaccess/deployment', t('The htaccess %profile_name will be deployed.', array('%profile_name' => $profile_name)),
    t('Deploy'),
    t('Cancel')
    );
     break;
  case 'delete':
    $confirm_form = confirm_form($form,
    t('Are you sure you want to delete the htaccess profile %profile_name?', array('%profile_name' => $profile_name)), 'admin/config/system/htaccess/deployment', t('The htaccess %profile_name will be deleted. This action cannot be undone.', array('%profile_name' => $profile_name)),
    t('Delete'),
    t('Cancel')
    );
     break;
 }
  return $confirm_form;
}

/**
 * Submit handler for confirm form
 */
function htaccess_confirm_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $action = $form_state['values']['action'];
    $profile_id = $form_state['values']['profile_id'];
    switch ($action) {
      case 'deploy':
        htaccess_deploy($profile_id);
        break;
      case 'delete':
        htaccess_delete($profile_id);
        break;
    }
  }
}

function htaccess_display($id){
  $htaccess_get = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('id', array(':id' => $id),'IN')
    ->execute()
    ->fetchAssoc();

  $htaccess = "<p>". t("Back to !deployment_page", array("!deployment_page" => l(t("deployment page"), "/admin/config/system/htaccess/deployment"))) ."</p>";
  $htaccess .= "<code>".str_replace("\n", "<br />", check_plain($htaccess_get['htaccess']))."</code>";

  $form['htaccess_settings_display'] = array(
  '#type' => 'fieldset',
  '#title' => $htaccess_get['name'],
  '#description' => $htaccess,
  );

  return $form;
}

function htaccess_deploy($id){
  $root_path = DRUPAL_ROOT;

  $htaccess_path =  $root_path   . '/.htaccess';

  $htaccess_get = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('id', array(':id' => $id))
    ->execute()
    ->fetchAssoc();

  $htaccess_content = $htaccess_get['htaccess'];

  // Remove utf8-BOM
  $htaccess_content = str_replace("\xEF\xBB\xBF",'', $htaccess_content);

  // Try to write to the .htaccess file
  if (file_put_contents($htaccess_path, $htaccess_content)) {
    drupal_chmod($htaccess_path, 0644);

    // Get the current htaccess deployed
    $htaccess_current = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('deployed', 1, '=')
    ->execute()
    ->fetchAssoc();

    // If any, set the status to 0
    if($htaccess_current){
      db_update('htaccess')->fields(array(
      'deployed' => 0))
      ->condition('id', $htaccess_current['id'], '=')
      ->execute();
    }

    // Set the status to 1
    db_update('htaccess')->fields(array(
    'deployed' => 1))
    ->condition('id', $htaccess_get['id'], '=')
    ->execute();

    drupal_set_message(t('Htaccess profile @profile has been deployed.', array('@profile' => $htaccess_get['name'])));

    drupal_goto("admin/config/system/htaccess/deployment");
  }
  else {
    $variables = array(
      '%directory' => $root_path,
      '!htaccess' => '<br />' . nl2br(check_plain($htaccess_content)),
    );

    watchdog('security', "Security warning: Couldn't write .htaccess file.", $variables, WATCHDOG_ERROR);

    drupal_set_message(t('Error during deployment: couldn\'t write .htaccess file. You have to download it and manually put it in the root of your Drupal installation.'), 'error');

    drupal_goto("admin/config/system/htaccess/deployment");
  }
}

function htaccess_download($id){
  $htaccess_get = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('id', array(':id' => $id),'IN')
    ->execute()
    ->fetchAssoc();

  $htaccess_content = $htaccess_get['htaccess'];

  // Remove utf8-BOM
  $htaccess_content = str_replace("\xEF\xBB\xBF",'', $htaccess_content);

  $file_name = $htaccess_get['name'].'.htaccess';

  $htaccess_folder = 'public://htaccess';

  if(file_prepare_directory($htaccess_folder, FILE_CREATE_DIRECTORY)) {

    file_create_htaccess($htaccess_folder, true, false);

    $htaccess_file = file_unmanaged_save_data($htaccess_content, $htaccess_folder. '/' .$file_name, FILE_EXISTS_REPLACE);

    file_transfer($htaccess_file, array(
      'Content-Type' => 'application/octet-stream',
      'Content-disposition' => 'attachment; filename='.$file_name));
  }
}

function htaccess_delete($id){
  // Check that the profile is not in use
  $htaccess_check = db_select('htaccess', 'h')
   ->fields('h')
   ->condition('deployed', 1, '=')
   ->condition('id', array(':id' => $id),'IN')
   ->execute()
   ->fetchAssoc();

  if($htaccess_check){
    drupal_set_message(t('This htaccess\'s profile is currently in use'), 'error');
   }
  else{
    $htaccess_get = db_delete('htaccess')
      ->condition('id', array(':id' => $id))
      ->execute();

    drupal_set_message(t('Htacces profile has been removed.'));
  }

  drupal_goto("admin/config/system/htaccess/deployment");
}

/**
 * Admin htaccess generate validation handler.
 */
function htaccess_admin_settings_generate_validate($form, &$form_state) {
  $profile_name = $form_state['values']['htaccess_settings_generate_name'];

  if(preg_match('/[^a-z0-9]/', $profile_name)) {
     form_error($form, t('The name of the profile must be lowercase and without any special character.'));
  }
  // The name of the profile must be unique
  $htaccess_name = db_select('htaccess', 'h')
    ->fields('h')
    ->condition('name', $profile_name, '=')
    ->execute()
    ->fetchAssoc();

  if($htaccess_name){
    form_error($form, t('The profile @profile already exists.', array('@profile' => $profile_name)));
  }
}

/**
 * Admin htaccess generate submit handler.
 */
function htaccess_admin_settings_generate_submit($form, &$form_state) {
  module_invoke_all("htaccess_generate_before");

  $htaccess_template = file_get_contents(HTACCESS_TEMPLATE_PATH);

  $rules_before_config = variable_get('htaccess_settings_custom_settings', '');

  $redirection_config = variable_get('htaccess_settings_url_prefix_redirection', 'without_www');

  $boot_rules = variable_get('htaccess_settings_boost_module_rules', '');

  switch ($redirection_config) {
    case 'without_www':
      $without_www_config = "RewriteCond %{HTTP_HOST} ^www\.(.+)$ [NC]\n";
      $without_www_config.= "RewriteRule ^ http%{ENV:protossl}://%1%{REQUEST_URI} [L,R=301]\n";
      $with_www_config = "#RewriteCond %{HTTP_HOST} .\n";
      $with_www_config .= "#RewriteCond %{HTTP_HOST} !^www\. [NC]\n";
      $with_www_config .= "#RewriteRule ^ http%{ENV:protossl}://www.%{HTTP_HOST}%{REQUEST_URI} [L,R=301]\n";
      break;
    case 'with_www':
      $without_www_config = "#RewriteCond %{HTTP_HOST} ^www\.(.+)$ [NC]\n";
      $without_www_config.= "#RewriteRule ^ http%{ENV:protossl}://%1%{REQUEST_URI} [L,R=301]\n";
      $with_www_config = "RewriteCond %{HTTP_HOST} .\n";
      $with_www_config .= "RewriteCond %{HTTP_HOST} !^www\. [NC]\n";
      $with_www_config .= "RewriteRule ^ http%{ENV:protossl}://www.%{HTTP_HOST}%{REQUEST_URI} [L,R=301]\n";
      break;
    default:
      $without_www_config = "#RewriteCond %{HTTP_HOST} ^www\.(.+)$ [NC] \n";
      $without_www_config.= "#RewriteRule ^ http%{ENV:protossl}://%1%{REQUEST_URI} [L,R=301]\n";
      $with_www_config = "#RewriteCond %{HTTP_HOST} .\n";
      $with_www_config .= "#RewriteCond %{HTTP_HOST} !^www\. [NC]\n";
      $with_www_config .= "#RewriteRule ^ http%{ENV:protossl}://www.%{HTTP_HOST}%{REQUEST_URI} [L,R=301]\n";
      break;
  }

  $symbolic_links_config = variable_get('htaccess_settings_symlinks', 'FollowSymLinks');

  switch ($symbolic_links_config) {
    case 'FollowSymLinks':
      $symbolic_links_config = "+FollowSymLinks";
      break;
    case 'SymLinksifOwnerMatch':
      $symbolic_links_config = "+SymLinksifOwnerMatch";
      break;
    default:
      $symbolic_links_config = "+FollowSymLinks";
      break;
  }

  $search = array("%%%rules_before%%%", "%%%symbolic_links%%%", "%%%with_www%%%", "%%%without_www%%%", "%%%boost_rules%%%");
  $replace = array($rules_before_config, $symbolic_links_config, $with_www_config, $without_www_config, $boot_rules);

  $htaccess = str_replace($search, $replace, $htaccess_template);

  $htaccess_profile_name = $form_state['values']['htaccess_settings_generate_name'];
  $htaccess_description = $form_state['values']['htaccess_settings_generate_description'];

  db_insert('htaccess')->fields(array(
    'name' => $htaccess_profile_name,
    'description' => $htaccess_description,
    'htaccess' => $htaccess,
    'created' => REQUEST_TIME,
  ))->execute();

  module_invoke_all("htaccess_generate_after", $htaccess);

  drupal_set_message(t('A new htaccess profile has been generated.'));
}
