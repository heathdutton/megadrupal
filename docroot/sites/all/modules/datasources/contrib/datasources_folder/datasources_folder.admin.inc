<?php

/**
 * Submit callback.
 */
function datasources_folder_admin_settings_submit($form, &$form_state) {

}

/**
 * Menu callback: displays the a datasource folder folders overview.
 *
 */
function datasources_folder_folders_overview() {
  return theme('datasources_folder_folders_overview', array('folders' => datasources_folder_load()));
}

/**
 * Returns HTML for the datasource inputs overview.
 *
 * @param array $variables
 *
 * @ingroup themeable
 */
function theme_datasources_folder_folders_overview($variables) {
  $folders=$variables['folders'];

  $rows = array();
  foreach ($folders as $folder) {
    $row = array();
    $row[] = $folder->name;
    $row[] = array('data' => l(t('edit'), "admin/config/content/datasources_folder/{$folder->dfid}"), 'class' => array('nowrap'));
    $row[] = array('data' => l(t('delete'), "admin/config/content/datasources_folder/{$folder->dfid}/delete"), 'class' => array('nowrap'));
    $rows[]=$row;
  }

  $header = array(
      t('Name'),
      array('data' => t('Operations'), 'colspan' => 2));

  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No datasource folders available.')));
}

/**
 * Form builder for the datasource folder delete form.
 *
 * @param $input_id
 *
 * @see datasources_folder_form_delete_folder_submit()
 * @ingroup forms
 */
function datasources_folder_form_delete_folder($form, $form_state, $dfid) {
  $folder=datasources_folder_load($dfid);
  return confirm_form(
      array(
          'source' => array(
              '#type' => 'value',
              '#value' => $folder->dfid,
          ),
      ),
      t('Are you sure you want to delete datasource folder %name?', array('%name' => $folder->name)),
      'admin/config/content/datasources_folder/' . $folder->dfid,
      t('This action cannot be undone.'),
      t('Delete datasource folder'),
      t('Cancel')
  );
}

/**
 * Submit callback.
 *
 * @see datasources_folder_form_delete_folder();
 */
function datasources_folder_form_delete_folder_submit($form, &$form_state) {
  db_delete('datasources_folder')
    ->condition('dfid', $form_state['values']['source'])
    ->execute();
  $form_state['redirect'] = 'admin/config/content/datasources_folder';
}

/**
 * Form builder for the datasource folder create form.
 *
 * @param $input_id
 *
 * @see datasources_folder_form_delete_folder_submit()
 * @ingroup forms
 */
function datasources_folder_form_add_folder($form, $form_state) {
  $form = array();
  
  $form['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Name'),
      '#default_value' => '',
      '#required' => TRUE,
      '#machine_name' => array(
          'exists' => 'datasources_folder_name_exists',
      ),
  );
  
  return confirm_form(
      $form,
      t('Are you sure you want to add a datasource folder'),
      'admin/config/content/datasources_folder',
      t('Add a new datasource folder.'),
      t('Add datasource folder'),
      t('Cancel')
  );
}

/**
 * Returns whether a folder name already exists.
 *
 * @see datasources_folder_form_add_folder()
 */
function datasources_folder_name_exists($value) {
  return db_query_range('SELECT 1 FROM {datasources_folder} WHERE name = :name', 0, 1, array(':name' => $value))->fetchField();
}

/**
 * Submit callback.
 *
 * @see datasources_folder_form_delete_folder();
 */
function datasources_folder_form_add_folder_submit($form, &$form_state) {
  $dfid = db_insert('datasources_folder')
    ->fields(array('name', 'options'))
    ->values(array(
          'name' => $form_state['values']['name'],
          'options' => serialize(array()),
    ))
    ->execute();
  
  $form_state['redirect'] = 'admin/config/content/datasources_folder/' . $dfid . '/edit';
}


/**
 * Form builder for the datasource folder edit form.
 *
 * @param $dfid
 *
 * @see datasources_folder_form_edit_folder_submit()
 * @ingroup forms
 */
function datasources_folder_form_edit_folder($form, $form_state, $dfid) {
  $folder=datasources_folder_load($dfid);

  drupal_set_title(t('Edit datasource folder !name', array('!name' => $folder->name)));

  $form = array();
  
  $form['dfid']=array(
      '#type' => 'value',
      '#value' => $folder->dfid,
  );

  $form['info'] = array(
      '#markup' => t('Enable or disable datasource input. Choose base entity type and bundle.'),
  );

  $form['name'] = array(
      '#title' => t('Name'),
      '#type' => 'item',
      '#markup' => $folder->name,
  );

  $form['options'] = array(
      '#tree' => TRUE,
  );
  
  $form['options']['root'] = array(
      '#type' => 'textfield',
      '#title' => t('Root import folder'),
      '#default_value' => $folder->options['root'],
      '#description' => 'System root folder.',
  );
  
  $form['options']['image_pattern'] = array(
      '#type' => 'textfield',
      '#title' => t('Image destination folder pattern'),
      '#default_value' => $folder->options['image_pattern'],
      '#description' => 'You can use strftime() formats, it will be used with image modification date.',
  );
  
  $form['options']['detect_caption'] = array(
      '#type' => 'checkbox',
      '#title' => t('Detect image caption'),
      '#default_value' => $folder->options['detect_caption'],
      '#description' => 'Detect if a text file has the same basename of an image and take it as the caption.',
  );
  
  $form['options']['multiple_text'] = array(
      '#type' => 'select',
      '#title' => t('Multiple text files mode'),
      '#options' => array('first' => t('First'), 'merge' => t('Merge')),
      '#default_value' => $folder->options['multiple_text'],
      '#description' => 'How to handle multiple text files.',
  );
  
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save settings'),
  );  
  
  $output = l(t('Configure datasource input text contents'), 'admin/config/content/datasource/datasource_folder_text_contents_' .$folder->name) . '<br/>' .
        l(t('Configure datasource input image contents'), 'admin/config/content/datasource/datasource_folder_image_contents_' .$folder->name);
  
  $output = '<div class="config-links">'.$output.'</div>';
  
  $form['configure'] = array(
      '#type' => 'markup',
      '#markup' => $output,
  );
  

  return $form;
}

/**
 * Submit callback.
 *
 * @see datasources_folder_form_edit_folder();
 */
function datasources_folder_form_edit_folder_submit($form, &$form_state) {
  db_update('datasources_folder')
    ->fields(array('options' => serialize($form_state['values']['options'])))
    ->condition('dfid', $form_state['values']['dfid'])
    ->execute();
  drupal_set_message(t('Settings saved.'));
}