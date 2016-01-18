<?php

/**
 * @file
 * Administratin page callbacks for the cmis views module.
 */

/**
 * Form builder. Configure cmis_views.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function cmis_views_admin_overview() {

  $header = array();
  $header[] = array('data' => t('View name'), 'field' => 'cmisviewname', 'sort' => 'asc');
  $header[] = array('data' => t('Query Type'), 'field' => 'querytype');
  $header[] = array('data' => t('CMIS Query'), 'field' => 'cmisquery');
  $header[] = array('data' => t('File Path'), 'field' => 'filepath');
  $header[] = array('data' => t('Operations'), 'field' => 'operations');

  $query = db_select('cmis_views')->extend('PagerDefault')->extend('TableSort');
  $result = $query
    ->fields('cmis_views')
    ->orderByHeader($header)
    ->limit(50)
    ->execute();

  $rows = array();
  $destination = drupal_get_destination();
  foreach ($result as $data) {
    $row = array();
    $row['data']['cmisviewname '] = $data->cmisviewname;
    if ($data->querytype == 1) {
      $row['data']['querytype'] = 'CMIS';
      $row['data']['cmisquery'] = $data->cmisquery;
      $row['data']['filepath'] = '';
    }
    else {
      $row['data']['querytype'] = 'File Path';
      $row['data']['cmisquery'] = '';
      $row['data']['filepath'] = $data->filepath;
    }
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => "admin/config/cmis/views/edit/$data->cmid",
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => "admin/config/cmis/views/delete/" . $data->cmid,
      'query' => $destination,
    );
    $row['data']['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );

    $rows[] = $row;
  }

  $build['cmis_views_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('To add CMIS Views, go to Structure > Blocks > Add a CMIS View'),
  );
  $build['cmis_views_pager'] = array('#theme' => 'pager');
  return $build;
}
/**
 * Settings for CMIS views
 */
function cmis_views_admin_settings() {

  module_load_include('utils.inc', 'cmis_views');

  $dftext = '';
  if (variable_get('cmis_views_noresultstext') != '') {
    $dftext = variable_get('cmis_views_noresultstext');
  }
  else {
    $dftext = t('Sorry, there are no results to match your query');
  }

  $dftheme = '';
  if (variable_get('cmis_views_theme') != '') {
    $dftheme = variable_get('cmis_views_theme');
  }
  
  if (variable_get('cmis_views_thumbnails') != '') {
    $thumbnails = variable_get('cmis_views_thumbnails');
  }

  $themes = _cmis_views_theme_types();

  $form['cmis_views_settings']['cmis_views_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Select a default theme for this CMIS views:'),
    '#options' => $themes,
    '#default_value' => $dftheme,
  );
  
  $form['cmis_views_settings']['cmis_views_thumbnails'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cmis:thumbnail for thumbnails whenever available'),
    '#description' => t('Thumbnails will be the first page of actual document if selected, otherwise they will default to generic filetype icons'),
    '#default_value' => $thumbnails,
  );

  return system_settings_form($form);
}

/**
 * Menu callback; handles pages for creating and editing CMIS Views
 */
function cmis_views_admin_edit($cmisinfo = array()) {

  if ($cmisinfo) {
    drupal_set_title($cmisinfo['cmisviewname']);
    $output = drupal_get_form('cmis_views_admin_form', $cmisinfo);
  }
  else {
    $output = drupal_get_form('cmis_views_admin_form');
  }
  return $output;
}

/**
 * Return a form for editing or creating an individual CMIS Views
 *
 * @ingroup forms
 * @see cmis_views_admin_form_submit()
 */
function cmis_views_admin_form($form, &$form_state, $cmisinfo = array('cmisviewname' => '', 'themetype' => '', 'querytype' => '', 'cmisquery' => '', 'createblock' => '', 'blocktitle' => '', 'filepath' => '', 'cmid' => NULL)) {

  ctools_include('dependent');
  module_load_include('utils.inc', 'cmis_views');
  drupal_add_library('system', 'ui.dialog');
  drupal_add_js(drupal_get_path('module', 'cmis_views') . '/cmis_browser.js');

  $types = node_type_get_types();
  $options[0] = t('Directory');
  $options[1] = t('CMIS');

  $themes = _cmis_views_theme_types();

  $themedefault = '';
  if ($cmisinfo['themetype'] != '') {
    $themedefault = $cmisinfo['themetype'];
  }
  elseif (variable_get('cmis_views_theme') != '') {
    $themedefault = variable_get('cmis_views_theme');
  }

  $form['cmis_views_add']['#attributes'] = array(
    'class' => array(
      'search-form',
    ),
  );

  $form['cmis_views_add']['queryinfo'] = array(
    '#type' => 'fieldset',
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['cmis_views_add']['queryinfo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manage CMIS Views'),
    '#attributes' => array(
      'class' => array(
        'container-inline',
      ),
    ),
  );

  $form['cmis_views_add']['queryinfo']['cmisviewname'] = array(
    '#type' => 'textfield',
    '#title' => t('View name:'),
    '#description' => t('Enter the title of your CMIS view - this will be the page URL in the address bar'),
    '#default_value' => $cmisinfo['cmisviewname'],
    '#required' => TRUE,
    '#size' => 50,
    '#maxlength' => 50,
  );
  $form['cmis_views_add']['queryinfo']['#attributes'] = array('class' => array('search-form'));

  $form['cmis_views_add']['queryinfo']['querytype'] = array(
    '#type' => 'radios',
    '#title' => t('Query the repository by:'),
    '#options' => $options,
    '#default_value' => $cmisinfo['querytype'],
    '#description' => t('The CMIS views module will switch to a file path based query or a CMIS 1.0 SQL like query'),
    '#id' => 'cmis_querytype',
  );
  $form['cmis_views_add']['queryinfo']['pathholder'] = array(
    '#type' => 'fieldset',
    '#title' => t('Folder Location'),
    '#input' => TRUE,
    '#dependency' => array('radio:querytype' => array('0')),
  );

  $form['cmis_views_add']['queryinfo']['pathholder']['filepath'] = array(
    '#type' => 'textfield',
    '#title' => t('File Path location'),
    '#description' => t('Enter the file path location of the folder or file type that you want to display <br /><em>For example: Sites/docs/documentLibrary/casestudies</em>'),
    '#default_value' => $cmisinfo['filepath'],
    '#size' => 250,
    '#maxlength' => 250,
    '#dependency' => array('radio:querytype' => array('0')),
  );

  $form['cmis_views_add']['queryinfo']['pathholder']['browse'] = array(
    '#type' => 'button',
    '#value' => t('Browse CMIS repository'),
    '#submit' => array(),
    '#ajax' => array(
      'path' => 'admin/config/cmis/views/ajax/',
    ),
    '#limit_validation_errors' => array(),
  );

  cmis_views_admin_ajax_browse('');

  $form['cmis_views_add']['queryinfo']['cmisquery'] = array(
    '#type' => 'textfield',
    '#title' => t('CMIS SQL Query'),
    '#description' => t('Enter your CMIS 1.0 query'),
    '#default_value' => $cmisinfo['cmisquery'],
    '#size' => 300,
    '#maxlength' => 510,
    '#dependency' => array('radio:querytype' => array('1')),
  );

  $form['cmis_views_add']['cmid'] = array(
    '#type' => 'value',
    '#value' => $cmisinfo['cmid'],
  );

  $form['cmis_views_add']['queryinfo']['themetype'] = array(
    '#type' => 'radios',
    '#title' => t('Select a theme for this CMIS view:'),
    '#options' => $themes,
    '#default_value' => $themedefault,
  );

  $form['cmis_views_add']['queryinfo']['createblock'] = array(
    '#title' => t('Create a block'),
    '#type' => 'hidden',
    '#attributes' => array('class' => array('strong')),
    '#id' => 'edit-block-create',
    '#default_value' => 1,
    '#description' => t('To use this CMIS content in a page you must select "create block"'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Provides form to confirm deletion of CMIS View
 */
function cmis_views_admin_delete_form($form, &$form_state, $cmisinfo = array('cmisviewname' => '', 'querytype' => '', 'themetype' => '', 'cmisquery' => '', 'pagetitle' => '', 'createblock' => '', 'blocktitle' => '', 'filepath' => '', 'cmid' => NULL)) {

  $form['cmis_views'] = array(
    '#type' => 'value',
    '#value' => $cmisinfo,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return confirm_form($form,
  t('Delete CMIS View'),
  isset($_GET['destination']) ? $_GET['destination'] : "",
  t('Delete the view "') . $cmisinfo['cmisviewname'] . '" ?',
  t('Delete'),
  t('Cancel'));
}

/**
 * Deletes cmis_view record and cmis_view block delta
 */
function cmis_views_admin_delete_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];

  if ($form_state['values']['confirm']) {
    $cmisview = $form_state['values']['cmis_views'];

    db_delete('cmis_views')
    ->condition('cmid', $cmisview)
    ->execute();
    db_delete('block')
    ->condition('delta', $cmisview)
    ->condition('module', 'cmis_views')
    ->execute();
    $form_state['redirect'] = 'admin/structure/block/';

    drupal_set_message(t('CMIS View has been deleted successfully.'));
  }
}

/**
 * Cleans form values and creates default CMIS query to be saved in DB
 * Creates new block and new CMIS Views record
 */
function cmis_views_admin_form_submit($form, &$form_state) {
  module_load_include('utils.inc', 'cmis_views');
  module_load_include('api.inc', 'cmis');

  $repository = cmisapi_getRepositoryInfo();

  form_state_values_clean($form_state);

  $table = 'cmis_views';

  if ($form_state['values']['querytype'] == 0) {
    $form_state['values']['cmisquery'] = "select d.*, t.* from cmis:document as d join cm:titled as t on d.cmis:objectid = t.cmis:objectid where in_folder(d, '" . _cmis_views_content_object_from_path($repository, $form_state['values']['filepath']) . "')";
    $form_state['values']['uuid'] = _cmis_views_content_object_from_path($repository, $form_state['values']['filepath']);
  }

  if (empty($form_state['values']['cmid'])) {
    drupal_write_record($table, $form_state['values']);
    drupal_set_message(t('The CMIS view has been saved.'));

    $theme = variable_get('theme_default', NULL);

    // We also need to write a record to the block table.
    db_insert('block')
    ->fields(array(
      'module' => 'cmis_views',
      'delta' => $form_state['values']['cmid'],
      'pages' => '',
      'theme' => $theme,
      'status' => 1,
    ))
    ->execute();
  }
  else {
    drupal_write_record($table, $form_state['values'], array('cmid'));
    drupal_set_message(t('The CMIS view has been updated.'));
  }
  $form_state['redirect'] = 'admin/structure/block/manage/cmis_views/' . $form_state['values']['cmid'] . '/configure';
}

/**
 * Browse a CMIS reposiotry with ajax commands.
 * string $foldier_id the id of the folder to browse.
 */
function cmis_views_admin_ajax_browse($folder_id = '') {
  module_load_include('api.inc', 'cmis');
  module_load_include('utils.inc', 'cmis_browser');
  $commands = array();

  // Invoke CMIS service.
  try {
    $repository = cmis_get_repository();
    $repoid = !empty($repository->repositoryId) ? $repository->repositoryId : 'default';

    module_load_include('utils.inc', 'cmis_views');
    module_load_include('api.inc', 'cmis');
    $repository = cmis_get_repository();
    if ($folder_id == '') {
      if (array_key_exists('browser_default_folderId', $repository->settings)) {
        // Grab default folderId from repository's settings.
        $folder_id = $repository->settings['browser_default_folderId'];
      }elseif(variable_get('cmis_browser_root') != NULL){
        $repoid = !empty($repository->repositoryId) ? $repository->repositoryId : 'default';
        $folder_id = $repository->info->repositoryInfo['cmis:rootFolderId'];
        $bcarray = explode('/', variable_get('cmis_browser_root'));
        if (!empty($bcarray)) {
          $object_path = drupal_encode_path('/' . implode('/', $bcarray));
          $object = cmisapi_getObjectByPath($repoid, $object_path);
        }
        $folder_id = $object->properties['cmis:objectId'];
      }else {
        // Fallback to repository's root folderId.
        $folder_id = $repository->info->repositoryInfo['cmis:rootFolderId'];
      }
    }
    $object = cmisapi_getProperties($repository->repositoryId, $folder_id);
    $children = cmisapi_getChildren($repository->repositoryId, $folder_id);
    $content = array();
    if (isset($object->properties['cmis:parentId'])) {
      $items[] = l(t('Parent Folder'), 'admin/config/cmis/views/nojs/' . urlencode($object->properties['cmis:parentId']),
        array(
          'attributes' => array(
            'class' => array(
              'use-ajax', 'parent',
            ),
          ),
        ));
    }
    foreach ($children->objectList as $cmis_item) {
      if ($cmis_item->properties['cmis:objectTypeId'] == 'cmis:folder' || preg_match('/^F:/', $cmis_item->properties['cmis:objectTypeId'])) {
        $items[] = l($cmis_item->properties['cmis:name'],
        'admin/config/cmis/views/nojs/' . urlencode($cmis_item->id),
          array(
            'attributes' => array(
              'class' => array(
                'use-ajax',
              ),
            ),
          ));
      }
      else {
        // Show a name so that users have an idea what is in the folder.
        $items[] = '<span class="file">' . $cmis_item->properties['cmis:name'] . '</span>';
      }
    }

    $content['list'] = array(
      '#theme_wrappers' => array('container'),
      '#attributes' => array(
        'class' => array(
          'cmis_views_browse_folder',
         ),
      ),
      'child' => array(
        '#theme' => 'item_list',
        '#title' => t('Browse folder'),
        '#items' => $items,
      ),
    );
    $folder_path = $object->properties['cmis:path'];
    $commands[] = ajax_command_remove('.cmis_views_browse_folder');
    $commands[] = ajax_command_append('html', drupal_render($content));
    $commands[] = ajax_command_invoke('.cmis_views_browse_folder', 'dialog', array(array('modal' => "true", 'title' => $folder_path)));
    // Fire attach behaviors for the dialog by doing a dummy insert.
    $commands[] = ajax_command_append('html', '');
    drupal_add_css(drupal_get_path('module', 'cmis_views') . '/cmis_views.css');

  }
  catch (Exception $error) {
    $commands[] = ajax_command_alert(t('Cmis Error, %error', array('%error' => $error->getMessage())));
  }

  $page = array();
  $page['#type'] = 'ajax';
  $page['#commands'] = $commands;
  return $page;
}
