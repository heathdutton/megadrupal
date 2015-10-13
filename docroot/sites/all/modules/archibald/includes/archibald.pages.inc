<?php

require_once dirname(__FILE__) . '/archibald.transition.php';
use \Educa\DSB\LomDataSanitization;

/**
 * @file
 * here in are all thing which are shows some content
 * and are to small to have an own file / folder
 */

/**
 * Provide the change form for a keyword.
 *
 * @param string $id
 *   the langstring id for this keyword
 *
 * @return string
 *   html
 */
function archibald_admin_edit_keyword($id) {
  $output = "";
  if (empty($_POST)) {
    drupal_set_message(t('If you edit the keyword you have to republish all descriptions using this keyword.'), 'warning');
  }
  $f = drupal_get_form('archibald_admin_edit_keyword_form', $id);
  $output .= drupal_render($f);
  return $output;
}

/**
 * Returns the edit form for a keyword within the admin interface.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 * @param string $id
 *   the langstring id.
 *
 * @return array
 *   the form
 */
function archibald_admin_edit_keyword_form($form, $form_state, $id) {
  $form = array();

  $data = db_select('archibald_keyword_list', 'kl')
    ->fields('kl')
    ->condition('langstring_id', $id)
    ->execute()
    ->fetch();

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $id,
  );

  $form['keyword'] = array(
    '#type' => 'textfield',
    '#title' => t('Edit keyword'),
    '#default_value' => $data->keyword,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 9999,
  );

  return $form;
}


/**
 * Validate the new keyword.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_admin_edit_keyword_form_validate($form, &$form_state) {
  if (empty($form_state['values']['keyword']) && $form_state['values']['op'] != $form_state['values']['cancel']) {
    drupal_set_message(t('If you edit the keyword you have to republish all descriptions using this keyword.'), 'warning');
    form_set_error('keyword', t('!name field is required.', array(
      '!name' => $form['keyword']['#title'],
    )));
  }
}

/**
 * Save the new keyword.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_admin_edit_keyword_form_submit($form, &$form_state) {

  if ($form_state['values']['op'] == $form_state['values']['cancel']) {
    drupal_goto('admin/archibald/other/archibald_keywords');
    return;
  }

  $data = db_select('archibald_keyword_list', 'kl')
    ->fields('kl')
    ->condition('langstring_id', $form_state['values']['id'])
    ->execute()
    ->fetch();

  if ($data->keyword != $form_state['values']['keyword']) {

    $old_value = $data->keyword;
    $new_value = $form_state['values']['keyword'];

    try {
      db_update('archibald_keyword_list')
      ->fields(array(
        'keyword' => $new_value,
      ))
      ->condition('langstring_id', $form_state['values']['id'])
      ->execute();

      $langstring = ArchibaldLomSaveHandler::requestLangStringObj($form_state['values']['id']);
      $langstring->setString($new_value, $data->language);
      ArchibaldLomSaveHandler::saveOrInsertLangString($langstring);
      archibald_handle_new_revision_is_required(
          $form_state['values']['id'],
          new ArchibaldLomDataLangString($old_value, $data->language),
          new ArchibaldLomDataLangString($new_value, $data->language)
      );

      drupal_set_message(t('Keyword changed'));
      drupal_goto('admin/archibald/other/archibald_keywords');
      die();
    }
    catch (PDOException $e) {
      drupal_set_message(t('Keyword already exists'), 'error');
    }
  }
  else {
    drupal_set_message(t('Keyword unchanged'), 'info');
    drupal_goto('admin/archibald/other/archibald_keywords');
    die();
  }
}

/**
 * Provide the change form for a coverage.
 *
 * @param string $id
 *   the langstring id for this coverage
 *
 * @return string
 *   html
 */
function archibald_admin_edit_coverage($id) {
  $output = "";
  if (empty($_POST)) {
    drupal_set_message(t('If you edit the coverage you have to republish all descriptions using this coverage.'), 'warning');
  }
  $f = drupal_get_form('archibald_admin_edit_coverage_form', $id);
  $output .= drupal_render($f);
  return $output;
}

/**
 * Returns the edit form for a coverage within the admin interface.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 * @param string $id
 *   the langstring id.
 *
 * @return array
 *   the form
 */
function archibald_admin_edit_coverage_form($form, $form_state, $id) {
  $form = array();

  $data = db_select('archibald_coverage_list', 'cl')
    ->fields('cl')
    ->condition('langstring_id', $id)
    ->execute()
    ->fetch();

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $id,
  );

  $form['coverage'] = array(
    '#type' => 'textfield',
    '#title' => t('Edit coverage'),
    '#default_value' => $data->coverage,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 9999,
  );

  return $form;
}

/**
 * Validates the coverage edit form.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_admin_edit_coverage_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] != $form_state['values']['cancel'] && empty($form_state['values']['coverage'])) {
    drupal_set_message(t('If you edit the coverage you have to republish all descriptions using this coverage.'), 'warning');
    form_set_error('coverage', t('!name field is required.', array('!name' => $form['coverage']['#title'])));
  }
}

/**
 * Save the new coverage.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_admin_edit_coverage_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == $form_state['values']['cancel']) {
    drupal_goto('admin/archibald/other/archibald_coverages');
    return;
  }
  $data = db_select('archibald_coverage_list', 'cl')
    ->fields('cl')
    ->condition('langstring_id', $form_state['values']['id'])
    ->execute()
    ->fetch();

  if ($data->coverage != $form_state['values']['coverage']) {

    $old_value = $data->coverage;
    $new_value = $form_state['values']['coverage'];

    try {
      db_update('archibald_coverage_list')
        ->fields(array(
          'coverage' => $new_value,
        ))
        ->condition('langstring_id', $form_state['values']['id'])
        ->execute();

      $langstring = ArchibaldLomSaveHandler::requestLangStringObj($form_state['values']['id']);
      $langstring->setString($new_value, $data->language);
      ArchibaldLomSaveHandler::saveOrInsertLangString($langstring);
      archibald_handle_new_revision_is_required(
          $form_state['values']['id'],
          new ArchibaldLomDataLangString($old_value, $data->language),
          new ArchibaldLomDataLangString($new_value, $data->language)
      );

      drupal_set_message(t('Coverage changed'));
      drupal_goto('admin/archibald/other/archibald_coverages');
      die();
    }
    catch (PDOException $e) {
      drupal_set_message(t('Coverage already exists'), 'error');
    }
  }
  else {
    drupal_set_message(t('Coverage unchanged'), 'info');
    drupal_goto('admin/archibald/other/archibald_coverages');
    die();
  }
}

/**
 * Displays the managment interface for keywords.
 *
 * @param string $action
 *   the action
 * @param string $id
 *   the keyword id
 *
 * @return string
 *   the html code
 */
function archibald_admin_manage_keywords($action = "", $id = "") {

  $output = "";

  if ($action == 'delete') {
    $data = db_select('archibald_keyword_list', 'kl')
      ->fields('kl')
      ->condition('langstring_id', $id)
      ->execute()
      ->fetch();

    if (empty($_POST)) {
      drupal_set_message(t('If you delete the keyword you have to republish all descriptions using this keyword.'), 'warning');
    }

    $f = drupal_get_form('archibald_manage_keyword_delete', $id);
    $output .= t('Do you really want to delete the keyword: @keyword?', array('@keyword' => $data->keyword));
    $output .= '<div style="margin-top: 10px">' . drupal_render($f) . '</div>';
    $output .= '<br /><br />';
    return $output;
  }

  $f = drupal_get_form('archibald_manage_keyword_coverage_search');
  $output .= drupal_render($f);

  $results = db_select('archibald_keyword_list', 'kl')
    ->fields('kl');

  $results->orderBy('kl.keyword, kl.language');

  if (!empty($_SESSION['archibald_manage_keywords_search_value'])) {
    $results->condition('keyword', '%' . $_SESSION['archibald_manage_keywords_search_value'] . '%', 'LIKE');
  }

  if (!empty($_SESSION['archibald_manage_keywords_search_language'])) {
    $results->condition('language', $_SESSION['archibald_manage_keywords_search_language']);
  }

  $results = $results->execute();

  $table = array(
    'header' => array(
      t('Keyword'),
      array('data' => t('Language'), 'style' => 'text-align:right; width: 90px;'),
      array('data' => t('Options'), 'style' => 'text-align:right; width: 150px;'),
    ),
    'rows' => array(),
  );

  foreach ($results->fetchAll() AS $row) {

    $links = "";
    $links .= l(t('Edit'), 'admin/archibald/other/archibald_keyword/' . $row->langstring_id . '/edit') . ' | ';
    $links .= l(t('Delete'), 'admin/archibald/other/archibald_keyword/' . $row->langstring_id . '/delete');

    $table['rows'][] = array(
      $row->keyword,
      array('data' => $row->language, 'style' => 'text-align:right;'),
      array('data' => $links, 'style' => 'text-align:right;'),
    );
  }

  $output .= theme('table', $table);
  return $output;
}

/**
 * Provides a confirmation form to delete the choosen langstring.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 * @param string $id
 *   the langstring id
 */
function archibald_manage_keyword_delete($form, $form_state, $id) {
  $form = array();

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $id,
  );

  $form['confirm'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Deletes the keyword if confirm is clicked.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_manage_keyword_delete_submit($form, &$form_state) {
  if ($form_state['values']['op'] != $form_state['values']['cancel']) {
    $data = db_select('archibald_keyword_list', 'kl')
      ->fields('kl')
      ->condition('langstring_id', $form_state['values']['id'])
      ->execute()
      ->fetch();


    db_delete('archibald_keyword_list')
      ->condition('langstring_id', $form_state['values']['id'])
      ->execute();

    $langstring = new ArchibaldLomDataLangString();
    $langstring->setId($form_state['values']['id']);
    ArchibaldLomSaveHandler::saveOrInsertLangString($langstring);
    archibald_handle_new_revision_is_required(
        $form_state['values']['id'],
        new ArchibaldLomDataLangString($data->keyword, $data->language),
        new ArchibaldLomDataLangString()
    );

    db_delete('archibald_general_keywords')
      ->condition('keyword', $form_state['values']['id'])
      ->execute();

    drupal_set_message(t('Keyword deleted'), 'info');
  }
  drupal_goto('admin/archibald/other/archibald_keywords');
  die();
}

/**
 * Displays the managment interface for coverages.
 *
 * @param string $action
 *   the action
 * @param string $id
 *   the keyword id
 *
 * @return string
 *   the html code
 */
function archibald_admin_manage_coverages($action = "", $id = "") {

  $output = "";

  if ($action == 'delete') {
    $data = db_select('archibald_coverage_list', 'kl')
      ->fields('kl')
      ->condition('langstring_id', $id)
      ->execute()
      ->fetch();

    if (empty($_POST)) {
      drupal_set_message(t('If you delete the coverage you have to republish all descriptions using this coverage.'), 'warning');
    }
    $f = drupal_get_form('archibald_manage_coverage_delete', $id);
    $output .= t('Do you really want to delete the coverage: @coverage?', array('@coverage' => $data->coverage));
    $output .= '<div style="margin-top: 10px">' . drupal_render($f) . '</div>';
    $output .= '<br /><br />';
    return $output;
  }

  $output .= l(t('Add new coverage'), 'admin/archibald/other/archibald_coverages/add');

  $f = drupal_get_form('archibald_manage_keyword_coverage_search');
  $output .= drupal_render($f);

  $results = db_select('archibald_coverage_list', 'cl')
    ->fields('cl');

  $results->orderBy('cl.coverage, cl.language');


  if (!empty($_SESSION['archibald_manage_keywords_search_value'])) {
    $results->condition('coverage', '%' . $_SESSION['archibald_manage_keywords_search_value'] . '%', 'LIKE');
  }

  if (!empty($_SESSION['archibald_manage_keywords_search_language'])) {
    $results->condition('language', $_SESSION['archibald_manage_keywords_search_language']);
  }

  $results = $results->execute();

  $table = array(
    'header' => array(
      t('Coverage'),
      array('data' => t('Language'), 'style' => 'text-align: right;width: 90px;'),
      array('data' => t('Options'), 'style' => 'text-align: right; width: 150px;'),
    ),
    'rows' => array(),
  );

  foreach ($results->fetchAll() AS $row) {

    $links = "";
    $links .= l(t('Edit'), 'admin/archibald/other/archibald_coverage/' . $row->langstring_id . '/edit') . ' | ';
    $links .= l(t('Delete'), 'admin/archibald/other/archibald_coverage/' . $row->langstring_id . '/delete');

    $table['rows'][] = array(
      $row->coverage,
      array('data' => $row->language, 'style' => 'text-align: right;'),
      array('data' => $links, 'style' => 'text-align: right;'),
    );
  }

  $output .= theme('table', $table);
  return $output;
}

/**
 * Add a coverage.
 *
 * @return string
 *   html
 */
function archibald_admin_add_coverages() {
  $f = drupal_get_form('archibald_admin_add_coverages_form');
  return drupal_render($f);
}

/**
 * Returns the add coverage form.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_admin_add_coverages_form($form, $form_state) {
  global $language;
  $form = array();

  $form['coverage'] = array(
    '#type' => 'textfield',
    '#title' => t('New coverage'),
  );

  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => archibald_get_general_language_options(),
    '#default_value' => $language->language,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Validates the coverage add form.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_admin_add_coverages_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] != $form_state['values']['cancel']) {

    if (empty($form_state['values']['coverage'])) {
      form_set_error('coverage', t('!name field is required.', array('!name' => $form['coverage']['#title'])));
    }
    else {
      $row = db_select('archibald_coverage_list', 'cl')
        ->fields('cl')
        ->condition('coverage', $form_state['values']['coverage'])
        ->condition('language', $form_state['values']['language'])
        ->execute()
        ->fetchAssoc();

      if (!empty($row)) {
        form_set_error('coverage', t('Within this language the entered coverage already exists, please choose another one.'));
      }
    }
  }
}

/**
 * Adds the coverage
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_admin_add_coverages_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] != $form_state['values']['cancel']) {
    $langstring = new ArchibaldLomDataLangString($form_state['values']['coverage'], $form_state['values']['language']);
    db_insert('archibald_coverage_list')
      ->fields(array(
        'coverage' => $form_state['values']['coverage'],
        'language' => $form_state['values']['language'],
        'langstring_id' => ArchibaldLomSaveHandler::saveOrInsertLangString($langstring),
      ))
      ->execute();
    drupal_set_message(t('Coverage added'));
  }

  drupal_goto('admin/archibald/other/archibald_coverages');
  die();
}

/**
 * Provides a confirmation form to delete the choosen langstring.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 * @param string $id
 *   the langstring id
 */
function archibald_manage_coverage_delete($form, $form_state, $id) {
  $form = array();

  $form['id'] = array(
    '#type' => 'hidden',
    '#value' => $id,
  );

  $form['confirm'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Deletes the coverage if confirm is clicked.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_manage_coverage_delete_submit($form, &$form_state) {
  if ($form_state['values']['op'] != $form_state['values']['cancel']) {
    $data = db_select('archibald_coverage_list', 'kl')
      ->fields('kl')
      ->condition('langstring_id', $form_state['values']['id'])
      ->execute()
      ->fetch();


    db_delete('archibald_coverage_list')
      ->condition('langstring_id', $form_state['values']['id'])
      ->execute();

    $langstring = new ArchibaldLomDataLangString();
    $langstring->setId($form_state['values']['id']);
    ArchibaldLomSaveHandler::saveOrInsertLangString($langstring);
    archibald_handle_new_revision_is_required(
        $form_state['values']['id'],
        new ArchibaldLomDataLangString($data->coverage, $data->language),
        new ArchibaldLomDataLangString()
    );

    db_delete('archibald_general_coverage')
      ->condition('coverage', $form_state['values']['id'])
      ->execute();

    drupal_set_message(t('Coverage deleted'), 'info');
  }
  drupal_goto('admin/archibald/other/archibald_coverages');
  die();
}

/**
 * Returns the search form for keywords/coverages.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 *
 * @return array
 *   the form
 */
function archibald_manage_keyword_coverage_search($form, $form_state) {
  $form = array();

  if (!isset($_SESSION['archibald_manage_keywords_search_value'])) {
    $_SESSION['archibald_manage_keywords_search_value'] = "";
  }

  if (!isset($_SESSION['archibald_manage_keywords_search_language'])) {
    $_SESSION['archibald_manage_keywords_search_language'] = "";
  }

  $form['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#default_value' => $_SESSION['archibald_manage_keywords_search_value'],
  );

  $languages = array_merge(
      array('' => t('All')) ,
      archibald_get_general_language_options()
  );


  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => $languages,
    '#default_value' => $_SESSION['archibald_manage_keywords_search_language'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * Setup the session value on which we search.
 *
 * @param array $form
 *   the form array
 * @param array $form_state
 *   the form state
 */
function archibald_manage_keyword_coverage_search_submit($form, &$form_state) {
  $_SESSION['archibald_manage_keywords_search_value'] = $form_state['values']['search'];
  $_SESSION['archibald_manage_keywords_search_language'] = $form_state['values']['language'];
}

/**
 * display lom object
 *
 * @param string $lom_id
 * @param string $version_id
 */
function archibald_content_view($lom_id, $version_id = '') {
  global $language;

  if (empty($version_id)) {
    // get local published version if no version was given
    $status = archibald_get_lom_stats($lom_id, TRUE);
    if (!empty($status->local_published)) {
      $version_id = $status->local_published;
    }
  }
  $lom = archibald_load($lom_id, $version_id);

  // Make sure that only local published descriptions are displayed, except user has permission to view revisions
  if (empty($lom) || (empty($status->local_published) && !user_access('view archibald content revisions'))) {
      return t('No description available');
  }

  $additional          = array();
  $additional['first'] = array();
  $res                 = db_query('
    SELECT save_time, uid
    FROM {archibald_lom}
    WHERE lom_id=:lom_id
    ORDER BY save_time ASC
    LIMIT 1', array(
      'lom_id' => $lom_id,
    )
  );
  $return = $res->fetchAssoc();

  $additional['first']['save_time'] = date("d.m.Y H:i", $return['save_time']);
  $res_user = db_query(
    'SELECT name FROM {users} WHERE uid=:uid',
    array(
      'uid' => $return['uid'],
    )
  );
  $additional['first']['username'] = $res_user->fetchColumn(0);

  $res = db_query(
    'SELECT save_time FROM {archibald_lom} WHERE lom_id=:lom_id ORDER BY save_time DESC',
    array(
      'lom_id' => $lom_id,
    )
  );
  $additional['last'] = array(
    'save_time' => date('d.m.Y H:i', $res->fetchColumn(0)),
  );

  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');
  drupal_add_js(
    array(
      'archibald' => array(
        'lom' => array(
          'lom_id' => $lom->getLomId(),
          'version' => $lom->getVersion(),
        ),
      ),
    ), 'setting'
  );

  // Force title to none, to simplify the HTML layout
  drupal_set_title('');
  // $lom_general = $lom->getGeneral();
  // $lang_string = $lom_general->getTitle();
  // if ($lang_string instanceof ArchibaldLomDataLangString) {
  //   $title = ArchibaldLom::determinTitle($lang_string, $language->language);
  //   drupal_set_title(filter_xss($title));
  // }

  return theme(
    'archibald_content_view',
    array(
      'lom' => $lom,
      'additional' => $additional,
    )
  );
}

/**
 * returns a list with all revisions listed for given lom ressource
 *
 * @param string $lom_id the lom_id
 *
 * @return string html
 */
function archibald_revision_overview($lom_id) {
  global $language;

  drupal_set_title(t('Revisions'));

  $table = array(
    'header' => array(
      t('Date - Time'),
      t('Title of description'),
      t('Published'),
      t('User'),
      t('Options'),
    ),
    'rows' => array(),
  );

  $sql_result = db_query('
    SELECT archibaldl.version, archibaldg.title, archibaldl.save_time, usr.name as username
    FROM archibald_lom as archibaldl
    JOIN archibald_general as archibaldg ON (archibaldl.general_id = archibaldg.general_id)
    JOIN users as usr ON (archibaldl.uid = usr.uid)
    WHERE archibaldl.lom_id = :lom_id
     ORDER BY save_time DESC', array(
      ':lom_id' => $lom_id,
    )
  );

  while ($row = $sql_result->fetchAssoc()) {
    $lom_langstring = ArchibaldLomSaveHandler::requestLangStringObj($row['title']);

    $published = '-';

    switch (archibald_lom_is_published($lom_id, $row['version'])) {
      case ARCHIBALD_PUBLISH_STATUS_LOCAL:
        $published = t('Local');
        break;
      case ARCHIBALD_PUBLISH_STATUS_CENTRAL:
        $published = t('National');
        break;
      case ARCHIBALD_PUBLISH_STATUS_LOCAL_CENTRAL:
        $published = t('Local and national');
        break;
    }

    $links = array();
    $links[] = l(t('View'), 'archibald/' . $lom_id . '/revisions/' . $row['version'] . '/view');

    if (user_access('edit archibald content')) {
      $links[] = l(t('Edit'), 'archibald/' . $lom_id . '/edit/empty/' . $row['version']);
    }

    $row = array(
      format_date($row['save_time'], 'short'),
      ArchibaldLom::determinTitle($lom_langstring, $language->language),
      $published,
      $row['username'],
      implode(' | ', $links),
    );
    $table['rows'][] = $row;
  }

  return theme('table', $table);
}


function archibald_json_export($lom_id) {
  global $language;

  drupal_set_title(t('JSON export'));

  $status = archibald_get_lom_stats($lom_id, TRUE);

  $lom = archibald_load($lom_id, '', FALSE);
  $lom_general = $lom->getGeneral();
  $lang_string = $lom_general->getTitle();
  if ($lang_string instanceof ArchibaldLomDataLangString) {
    $lom_title = ArchibaldLom::determinTitle($lang_string, $language->language);
  }

  $json = LomDataSanitization::jsonExport($lom, TRUE);

  $table = array(
    'header' => array(
      'LOM ID : ' . $lom_id,
    ),
    'rows' => array(),
  );

  $table['rows'] = array(
    array(t('Title') . ' : ' . $lom_title),
    array('<pre>' . $json . '</pre>'),
  );

  return theme('table', $table);
}


/**
 * generate form to add / edit archibald resource
 *
 * @param string $lom_id
 * @param string $language
 * @param string $version
 *
 * @return string html
 */
function archibald_content_edit($lom_id = 'add', $language = '', $version = '', $copy_version = '') {

  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald_form.css');

  $content_partner_id = 0;

  drupal_add_js(
    array(
      'archibald' =>
      array(
        'urls' => array(
          'classification' => url('admin/archibald/classification'),
          'contributor_list' => url('archibald_contributor/chooser/ajax'),
          'contributor_preview' => url('archibald_contributor/preview/ajax'),
        ),
      ),
    ), 'setting'
  );

  ctools_include('ajax');
  ctools_include('modal');

  // Add CTools' javascript to the page.
  ctools_modal_add_js();

  if ($language == 'empty') {
    $language = '';
  }

  $lang_list = array();
  $languages = language_list('enabled');
  if (!empty($lom_id) && drupal_strtolower($lom_id) != 'add') {
    $lom = archibald_load($lom_id, $version, FALSE);
    if (empty($lom)) {
      drupal_set_message(t("No description available"), 'error');
      return "";
    }
    // try to load original language from meta meta data record
    if (empty($language)) {
      $meta_metadata = $lom->getMetaMetadata();
      if ($meta_metadata instanceof ArchibaldLomDataMetaMetadata) {
        $language = $meta_metadata->getLanguage();
      }
    }

    if (empty($language)) {
      // if metametadata.language is not set,
      // try to get it from title langstring and
      // set it to metametadata.language if it is unique

      $possible_languages = array_keys(
        $lom->getGeneral()->getTitle()->getStrings()
      );

      if (count($possible_languages) == 1) {
        $language = reset($possible_languages);
        $meta_metadata->setLanguage($language);
        $lom->setMetaMetadata($meta_metadata);
      }
      elseif (count($possible_languages) > 1) {
        // display language chooser to user
        $lang_list = array();
        foreach ($possible_languages as $lang) {
          $lang_list[] = $languages[1][$lang];
        }
      }
      else {
        // ooops here goes something horrible wrong,
        // show all languages to user
        $lang_list = array();
        foreach ($languages[1] as $lang) {
          $lang_list[] = $lang;
        }
      }
    }

    $lom_general = $lom->getGeneral();
    $lang_string = $lom_general->getTitle();
    if ($lang_string instanceof ArchibaldLomDataLangString) {
      $lom_title = ArchibaldLom::determinTitle($lang_string, $language);
      $title = t(
        'Edit "!title" (!language)',
        array(
          '!title' => $lom_title,
          '!language' => $language,
        )
      );
    }
  }
  else {
    $title = (user_access('edit archibald content')) ? t('Add new description') : t('Description proposal');

    if (!empty($version)) {
      // use an existing lom object as template to create a new one
      $preload_lomid = $version;
      $lom           = archibald_load($preload_lomid, $copy_version, FALSE);
      $lom->lom_id   = '';
      $lom->version  = '';
      // try to load original language from meta meta data record
      if (empty($language)) {
        $meta_metadata = $lom->getMetaMetadata();
        if ($meta_metadata instanceof ArchibaldLomDataMetaMetadata) {
          $language = $meta_metadata->getLanguage();
        }
      }

      if (empty($language)) {
        foreach ($lom->general->getLanguage() as $lang) {
          $lang_list[] = $languages[1][$lang];
        }
      }

      $lom_general = $lom->getGeneral();
      $lang_string = $lom_general->getTitle();
      if ($lang_string instanceof ArchibaldLomDataLangString) {
        $lom_title = ArchibaldLom::determinTitle($lang_string, $language);
        $title = t(
          'Add "!title" (!language)',
          array(
            '!title' => $lom_title,
            '!language' => $language,
          )
        );
      }

      /// unset title
      $lom_general->setTitle(NULL);
      $lom->setGeneral($lom_general);
    }
    else {
      $lom = new ArchibaldLom();
    }

    $lang_list = array();
    foreach ($languages[1] as $lang) {
      $lang_list[] = $lang;
    }
  }

  if (empty($language)) {
    if (count($lang_list) == 1) {
      $language = $lang_list[0]->language;
    }
    else {
      drupal_set_title(t('Choose the language in which you want to add the new description'));
      return theme(
        'archibald_content_choose_language',
        array(
          'lang_list' => $lang_list,
          'lom_id' => $lom_id,
        )
      );
    }
  }

  drupal_set_title($title);

  if ((empty($lom_id) || drupal_strtolower($lom_id) == 'add') && !empty($language) && (empty($_POST) || $_POST['form_id'] == 'archibald_content_edit_select_content_partner')) {

    $content_partner_list = archibald_can_publish_for_content_partner();
    if (empty($content_partner_list)) {
      $content_partner_list = 0;
    }
    elseif (count($content_partner_list) == 1) {
      $content_partner_id = key($content_partner_list);
    }
    else {
      if (empty($_SESSION['archibald_content_partner_id'])) {
        return drupal_get_form('archibald_content_edit_select_content_partner');
      }
      else {
        $content_partner_id = $_SESSION['archibald_content_partner_id'];
        unset($_SESSION['archibald_content_partner_id']);
      }
    }
  }

  return drupal_get_form('archibald_content_form', $lom, $language, $content_partner_id);
}

/**
 * delete (mark as deleted) a lom object
 *
 * @param string $lom_id
 */
function archibald_content_delete($lom_id) {
  $lom = archibald_load($lom_id);
  $archibald_lom = new ArchibaldLom();
  $title = $archibald_lom->determinTitle($lom);

  $status = archibald_get_lom_stats($lom_id);

  if ((!empty($status->publication_version) && $status->publication_log->lom_status == 'success') || !empty($status->local_published)) {
    drupal_set_title(filter_xss($title));
    drupal_set_message(t('This description cannot be deleted, unpublish first.'), 'notice');
    if ((!empty($status->publication_version) && $status->publication_log->lom_status == 'success')) {
      drupal_set_message(t('This description is published nationally.'), 'notice');
    }
    if (!empty($status->local_published)) {
      drupal_set_message(t('This description is published locally.'), 'notice');
    }
    return '';
  }

  $f = drupal_get_form('archibald_content_delete_form', $lom_id, strlen($title) ? $title : 'No title set');
  return drupal_render($f);
}

/**
 * delete formular for archibald resources
 *
 * @param array $form
 * @param array $form_state
 * @param string $title
 *   title of resource which is to delete
 *
 * @return array
 *   form
 */
function archibald_content_delete_form($form, &$form_state, $lom_id, $title) {
  $form['ressource'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE,
  );
  $form['ressource'][$lom_id] = array(
    '#type' => 'hidden',
    '#value' => $lom_id,
    '#prefix' => '<li>',
    '#suffix' => check_plain($title) . "</li>\n",
  );
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'archibald_content_delete_form_confirm_submit';

  return confirm_form(
    $form, t('Are you sure you want to delete this description?'), 'admin/archibald', t('This action cannot be undone.'), t('Yes'), t('No')
  );
}

/**
 * Delete one or multiple lom ressources after confimation
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_delete_form_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach (array_keys($form_state['values']['ressource']) as $lom_id) {
      $lom = archibald_load($lom_id);
      ArchibaldLomSaveHandler::delete($lom);
      $title = ArchibaldLom::determinTitle($lom);
      $title = strlen($title) ? $title : 'No title set';
      watchdog(
        'Content', 'Description "!title" has been deleted',
        array(
          '!title' => $title,
          '@lom_id' => $lom_id,
        ), WATCHDOG_NOTICE, 'archibald/' . $lom_id
      );

      drupal_set_message(
        t(
          'Description "!title" has been deleted',
          array(
            '!title' => $title,
            '@lom_id' => $lom_id,
          )
        )
      );
    }
  }
  $form_state['redirect'] = 'admin/archibald';
}

/**
 * the language chooser
 *
 * @param array $variables
 *
 * @return string
 */
function theme_archibald_content_choose_language($variables) {
  if (!empty($variables['lom_id']) && $variables['lom_id'] != 'add') {
    $type = 'edit';
  }
  else {
    $type = 'add';
  }

  $output = '';
  foreach ($variables['lang_list'] as $lang) {
    $link_params = array();
    if (variable_get('archibald_switch_language_while_add', 1) == 1) {
      $link_params = array(
        'language' => $lang
      );
    }

    if ($type == 'add') {
      $url = 'archibald/add/' . $lang->language;
    }
    else {
      $url = 'archibald/' . $variables['lom_id'] . '/edit/' . $lang->language;
    }
    $output .= l(t($lang->name), $url, $link_params) . '<br />';
  }

  return $output;
}

/**
 * display the content overview and perform the search
 *
 * @param string $type
 *   type to display
 *   unavailable
 *   own
 *   draft
 *   published
 *   published_local
 *   to_publish_local
 *   to_publish
 * @param $get_only_table boolean
 *   for ajax, set it to TRUE, to get only content
 *
 * @return string
 */
function archibald_admin_content_overview($type = 'all', $get_only_table = FALSE) {

  // If a publisher (which has no permission to edit archibald content) is logged on
  // the "In progress" menu item is not displayed. In that case go to "to publish" page.
  // Otherwise the publisher logs on and sees an empty page.
  if (!archibald_multi_access('manage archibald content', 'edit archibald content') && current_path() == 'admin/archibald') {
    drupal_goto('admin/archibald/to_publish');
  }

  global $user;

  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');
  drupal_add_library('system', 'ui.sortable');

  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');

  // Add CTools' javascript to the page.
  ctools_modal_add_js();

  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_admin_content_overview.js');

  if (isset($_POST['op']) && $_POST['op'] == t('Reset')) {
    $_POST['query'] = '';
    $_POST['own'] = '';
    archibald_admin_content_overview_form_submit();
  }

  if (isset($_POST['query'])) {
    archibald_admin_content_overview_form_submit();
    // print_r($_POST);
  }


  $form_params = array(
    'show_status_filter' => FALSE,
    'show_own_filter' => TRUE,
  );

  $publish_type = variable_get("archibald_publish_type", 0);
  $search = new ArchibaldLomSQLSearch();
  $is_published = FALSE;
  switch ($type) {
    case 'unavailable':
      $search->setStatusFilter('unavailable');
      break;

    case 'own':
      $search->setResponsibleFilter($user->uid);
      $search->setLocalPublishedFilter(FALSE);
      break;

    case 'draft':
      $search->setStatusFilter('draft');
      break;

    case 'published':
      $search->addWasPublishedFilter(($publish_type == 1) ? FALSE : TRUE);
      $is_published = TRUE;
      break;

    case 'published_local':
      $search->setLocalPublishedFilter(TRUE);
      $is_published = TRUE;
      break;

    case 'to_publish_local':
      $search->setLocalPublishedFilter(FALSE);
      break;

    case 'to_publish':
      $search->addCanPublishedFilter();
      $search->setStatusFilter('final');
      // $search->setLocalPublishedFilter(TRUE);
      break;

    default:
      if (!empty($_SESSION['archibald_search_status'])) {
        if ($_SESSION['archibald_search_status'] == 'deleted') {
          $search->showDeleted();
        }
        else {
          $search->setStatusFilter(archibald_select_options_default_value_filter_splitter($_SESSION['archibald_search_status']));
        }
      }
      $form_params['show_status_filter'] = TRUE;
      break;
  }

  if (!empty($_SESSION['archibald_search_own'])) {
    $search->setResponsibleFilter($user->uid);
  }
  $search_form = drupal_get_form('archibald_admin_content_overview_form', $form_params);


  $batch_ops_form = drupal_get_form('archibald_admin_content_batch_ops_form', array(
    'filter_view' => $type
  ));

  $num_per_page = variable_get('archibald_admin_content_num_per_page', 25);

  $search->setLimit($num_per_page);
  if (!empty($_SESSION['archibald_search_query'])) {
    $search->setQuery($_SESSION['archibald_search_query']);
  }
  $page = pager_default_initialize($search->getTotalRecordCount(), $num_per_page);
  $search->setOffset($page * $num_per_page);

  // echo '<pre>';
  // print_r( $search );
  // echo '</pre>';
  // exit;

  return theme('archibald_admin_content_overview', array(
    'published' => $is_published,
    'search_result' => $search->search(),
    'search_form' => $search_form,
    'search_result_count' => $search->getTotalRecordCount(),
    'type' => $type,
    'get_only_table' => (!empty($get_only_table)) ? 1 : 0,
    'batch_ops_form' => $batch_ops_form,
  ));
}

/**
 * admin search formular for archibald resources
 *
 * @param array $form
 * @param array $form_state
 * @param array $form_params
 *
 * @return array
 */
function archibald_admin_content_overview_form($form, &$form_state, $form_params) {

  $query = '';
  if (!empty($_SESSION['archibald_search_query'])) {
    $query = $_SESSION['archibald_search_query'];
  }

  $form['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Term'),
    '#default_value' => $query,
    '#size' => 40,
  );

  if ($form_params['show_own_filter'] == TRUE) {
    $archibald_search_own = '';
    if (!empty($_SESSION['archibald_search_own'])) {
      $archibald_search_own = $_SESSION['archibald_search_own'];
    }

    $form['own'] = array(
      '#type' => 'checkbox',
      '#title' => t('Description owner'),
      '#default_value' => $archibald_search_own,
    );
  }

  if ($form_params['show_status_filter'] == TRUE) {
    $archibald_search_status = '';
    if (!empty($_SESSION['archibald_search_status'])) {
      $archibald_search_status = $_SESSION['archibald_search_status'];
    }

    $form['status'] = array(
      '#type' => 'select',
      '#title' => t('Meta data status'),
      '#default_value' => $archibald_search_status,
      '#options' =>
      array('' => t('All')) + archibald_get_life_cycle_status_options() + array('deleted' => t('Deleted')),
    );
  }

  $form['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#weight' => 9999,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#weight' => 9999,
  );
  $form['#theme'] = 'archibald_admin_content_overview_form';

  return $form;
}

/**
 * submit handler for
 * archibald_admin_content_overview_form()
 */
function archibald_admin_content_overview_form_submit() {
  $_SESSION['archibald_search_own'] = '';
  if (!empty($_POST['own'])) {
    $_SESSION['archibald_search_own'] = filter_xss($_POST['own']);
  }

  $_SESSION['archibald_search_status'] = '';
  if (!empty($_POST['status'])) {
    $_SESSION['archibald_search_status'] = filter_xss($_POST['status']);
  }

  $_SESSION['archibald_search_query'] = '';
  if (!empty($_POST['query'])) {
    $_SESSION['archibald_search_query'] = filter_xss($_POST['query']);
  }
}

/**
 * Admin search formular for archibald resources.
 *
 * @param array $form
 * @param array $form_state
 * @param array $form_params
 *
 * @return array
 */
function archibald_admin_content_batch_ops_form($form, &$form_state, $form_params) {

  // Don't show the batch operation form if the current user do not have any valid permission for batch operations
  if (!archibald_check_batch_operation_access()) {
    return array();
  }

  $publish_type = variable_get('archibald_publish_type', ARCHIBALD_PUBLISH_SEPERATE);

  $batch_op_options = array('' => t('Choose an action for selected descriptions'));

  if (user_access('change archibald content status')) {
    switch ($form_params['filter_view']) {
      case 'unavailable':
      case 'to_publish':
      case 'to_publish_local':
      case 'published':
      case 'published_local':
        $batch_op_options['status2draft'] = t('Status to draft');
      break;
      case 'draft':
        $batch_op_options['status2final'] = t('Status to final');
      break;
    }
  }

  switch ($form_params['filter_view']) {
    case 'to_publish':
    case 'to_publish_local':
      if ($publish_type == ARCHIBALD_PUBLISH_SEPERATE) {
        if ($form_params['filter_view'] == 'to_publish_local' && user_access('publish archibald content local')) {
          $batch_op_options['publish_local'] =  t('Publish locally');
        }
        if ($form_params['filter_view'] == 'to_publish' && user_access('publish archibald content central')) {
          $batch_op_options['publish_national'] =  t('Publish nationally');
        }
      }
      if (user_access('publish archibald content local') && user_access('publish archibald content central')) {
        $batch_op_options['publish_both'] = ($publish_type == ARCHIBALD_PUBLISH_SEPERATE) ? t('Publish locally and nationally') : t('Publish');
      }
    break;
    case 'published':
    case 'published_local':
      if ($publish_type == ARCHIBALD_PUBLISH_SEPERATE) {
        if ($form_params['filter_view'] == 'published_local' && user_access('publish archibald content local')) {
          $batch_op_options['unpublish_local'] =  t('Unpublish locally');
        }
        if ($form_params['filter_view'] == 'published' && user_access('publish archibald content central')) {
          $batch_op_options['unpublish_national'] =  t('Unpublish nationally');
        }
      }
      if (user_access('publish archibald content local') && user_access('publish archibald content central')) {
        $batch_op_options['unpublish_both'] =  ($publish_type == ARCHIBALD_PUBLISH_SEPERATE) ? t('Unpublish locally and nationally') : t('Unpublish');
      }
    break;
  }

  // One operation is always present, the empty one which tells you to select something.
  if (count($batch_op_options) <= 1) {
    return array();
  }

  $form = array();

  $form['#action'] = url('admin/archibald/batch_ops');
  $form['#attributes']['onsubmit'] = "return false;";

  $form['batch_ops'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('batch_operation_form')),
  );

  foreach ($batch_op_options AS $k => &$v) {
    if (empty($k)) {
      continue;
    }
    $v = '> ' . $v;
  }

  $form['batch_ops']['batch_op_select'] = array(
    '#type' => 'select',
    '#title' => t('Batch operation'),
    '#options' => $batch_op_options,
  );

  $notification_markup = '';
  $notification_markup .= '<input type="hidden" name="batch_op" value="">';
  $notification_markup .= '<input type="hidden" name="batch_op_ids" value="">';
  $notification_markup .= '<input type="hidden" name="redirect" value="">';
  $notification_markup .= '<div id="batch_op_notification">';
  $notification_markup .= '<h3 class="checking hide">' . t('Checking selection. Please wait...') . '</h3>';
  $notification_markup .= '<h3 class="error hide"></h3>';
  $notification_markup .= '<h4 class="error_explain hide">' . t('Please find an explanation below.') . '</h4>';
  $notification_markup .= '</div>';

  $form['batch_ops']['notification'] = array(
    '#markup' => $notification_markup,
    '#weight' => 10000,
  );

  return $form;
}

/**
 * theme archibald content overview
 *
 * @param array $variables
 *
 * @return string
 */
function theme_archibald_admin_content_overview($variables) {
  global $user;
  global $language;

  // Display warning message if archibald is configured to publish to the central catalog and is not within live mode.
  // if (archibald_can_publish_central() && variable_get('archibald_working_mode', ARCHIBALD_WORKING_MODE_DEMO) != ARCHIBALD_WORKING_MODE_LIVE) {
  //   drupal_set_message(filter_xss(t('Notice: You are not working within the live publication modus. All published descriptions are not available within the national catalogue until you switch to the live mode and (re-)publish the description(s). To do this please go to !link and open the connection tab.', array(
  //     '!link' => l(t('Config page'), 'admin/config/services/archibald'),
  //   ))), 'warning', FALSE);
  // }

  $content_partners = archibald_load_content_partner();

  $html = '<input type="hidden" id="archibald_admin_content_overview_type" value="' . $variables['type'] . '" />';

  $action_links = "";
  if (archibald_content_add_access()) {
    $action_links .= '<div id="action_link_add">' . l(t('Add'), 'archibald/add') . '</div>';
  }

  if (archibald_check_publish_access()) {
    $row = db_query_range("SELECT 1 FROM {archibald_lom_stats} AS ls INNER JOIN {archibald_lom} as l ON(l.lom_id=ls.lom_id AND l.version=ls.publication_version) WHERE ls.republication_required = 1 AND l.deleted IS NULL", 0, 1)->fetchAssoc();
    if (!empty($row)) {
      $action_links .= '<div>' . l(t('Some descriptions need to be republished.'), 'admin/archibald/to_republish') . '</div>';
    }
  }

  if (!empty($action_links)) {
    $html .= '<div id="archibald_admin_content_overview_action_links">' . $action_links . '</div>';
  }

  $html .= drupal_render($variables['search_form']);
  $html .= '<div class="clearfix"></div>';

  $html .= drupal_render($variables['batch_ops_form']);
  $html .= '<div class="clearfix"></div>';

  drupal_add_js(array(
    'archibald_batch_ajax' => url('admin/archibald/batch_ops/ajax'),
  ), 'setting');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald_batch_ops.js');

  $table = array(
    'header' =>
    array(
      '<input type="checkbox" name="header_checkbox" id="archibald_batch_multicheck">',
      t('Link to description'),
      array(
        'data' => t('Status'),
        'colspan' => 2,
      ),
      t('Person responsible'),
    ),
    'rows' => array(),
    'attributes' => array('id' => 'archibald_admin_content_overview'),
  );

  $is_editor = (
    user_access('edit archibald content') ||
    user_access('delete archibald content') ||
    user_access('publish archibald content local') ||
    user_access('publish archibald content central')
  );

  $can_change_cp = (count(archibald_can_publish_for_content_partner()) > 1 && archibald_check_publish_access());

  if ($is_editor) {
    if (count($content_partners) > 1) {
      $table['header'][] = t('Content partner');
    }

    $table['header'][] = t('Options');
  }

  // generate table from result set
  foreach ($variables['search_result'] as $res_row) {
    $row = array();

    if (empty($res_row->status)) {
      $res_row->status = 'unavailable';
    }

    $row[0] = '<input type="checkbox" class="archibald_batch_chk" value="' . $res_row->lom_id . '|' . $res_row->version . '">';

    // Determine title.
    $archibald_lom = new ArchibaldLom();
    $title = $archibald_lom->determinTitle($res_row->title, $res_row->default_language);
    if (empty($title)) $title = t('No title');
    $row[1] = l(
      $title,
      'archibald/' . $res_row->lom_id . '/revisions/' . $res_row->version . '/view'
    );
    $in_translation = "";

    if ($res_row->status == ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_UNAVAILABLE) {
      $lom_status = t('Proposal');
    }
    else {
      $lom_status = t( ucfirst($res_row->status) );
    }
    if ( user_access('change archibald content status')) {
      $row[] = '<span class="lifecycle_status" lom_id="' . $res_row->lom_id . '" status="' . $res_row->status . '" url="' . url('archibald/' . $res_row->lom_id . '/set_status') . '">' . $lom_status . '</span>' . $in_translation;
    }
    else {
      $row[] = $lom_status;
    }


    $row[] = theme(
      'archibald_admin_content_publish_status',
      array(
        'res_row' => $res_row,
      )
    );

    $row[] = theme(
      'archibald_admin_content_overview_reponsible',
      array(
        'uid' => $res_row->responsible_uid,
        'name' => $res_row->user_name,
        'lom_id' => $res_row->lom_id,
      )
    );

    // if user is allowed to edit show options for this
    if ($is_editor) {
      if (count($content_partners) > 1) {
        if (empty($content_partners[$res_row->content_partner_id])) {
          $cp_name = t('Not set');
        }
        else {
          $cp_name = $content_partners[$res_row->content_partner_id]['name'];
        }

        if (!empty($res_row->publication_version) || !user_access('change archibald content partner')) {
          // if user has permission und ressource is not published
          $cp_class = 'content_partner_fixed';
        }
        else {
          $cp_class = 'content_partner';
        }

        if (!$can_change_cp) {
          $row[] = array(
            'data' =>  $cp_name ,
          );
        }
        else {
          $row[] = array(
            'data' => '<span class="content_partner" url="' . url('archibald/' . $res_row->lom_id . '/set_content_partner') . '">' . $cp_name . '</span>',
            'class' => array($cp_class),
          );
        }
      }

      $options = array();
      if (user_access('edit archibald content')) {
        $edit_label = t('Edit');
        $options[] = l(
          $edit_label,
          'archibald/' . $res_row->lom_id . '/edit',
          array(
            'attributes' => array(
              'type' => 'edit',
            ),
          )
        );
      }

      if (user_access('view archibald content revisions')) {
        $options[] = l(
          t('Revisions'),
          'archibald/' . $res_row->lom_id . '/revisions',
          array(
            'attributes' => array(
              'type' => 'revisions',
            ),
          )
        );
      }

      if (user_access('translate archibald content')) {
        $options[] = l(
          t('Translate'),
          'archibald/' . $res_row->lom_id . '/translate',
          array(
            'attributes' => array(
              'type' => 'translate',
            ),
          )
        );
      }
      if (user_access('delete archibald content')) {
        $options[] = l(
          t('Delete'),
          'archibald/' . $res_row->lom_id . '/delete',
          array(
            'attributes' => array(
              'type' => 'delete',
            ),
          )
        );
      }

      if (
        archibald_check_publish_access() &&
        $res_row->status == 'final'
      ) {
        /*
         *  Dont need to check here for can check central this will done later
         *  and we like to show the button all times, because we like to publish local
         *   !empty($content_partners[$res_row->content_partner_id]) &&
         *   !empty($content_partners[$res_row->content_partner_id]['key_fid']
         */

        $options[] = l(
          t('Publish'),
          'archibald/' . $res_row->lom_id . '/publish',
          array(
            'attributes' => array(
              'type' => 'publish',
              'class' => 'publish_link',
            ),
          )
        );
      }

      if (user_access('edit archibald content')) {
        $options[] = l(
          t('Publication log'),
          'archibald/' . $res_row->lom_id . '/log',
          array(
            'attributes' => array(
              'type' => 'log',
            ),
          )
        );
      }
      if ($res_row->status != ArchibaldLomDataLifeCycle::LIFECYCLE_STATUS_UNAVAILABLE) {
        if (user_access('add archibald content')) {
          $options[] = l(
            t('Use as template'),
            'archibald/add/' . $res_row->default_language . '/' . $res_row->lom_id,
            array(
              'attributes' => array(
                'type' => 'template',
              ),
            )
          );
        }
      }

      if (user_access('export archibald content')) {
        $options[] = l(
          t('JSON export'),
          'archibald/' . $res_row->lom_id . '/json_export',
          array(
            'attributes' => array(
              'type' => 'json_export',
            ),
          )
        );
      }

      $row[] = array(
        'data' => implode(' | ', $options),
        'class' => array('options_links'),
      );
    }

    $table['rows'][] = $row;
  }

  if (!empty($variables['get_only_table'])) {
    return theme('table', $table);
  }
  else {
    $html .= theme('table', $table);
  }

  $html .= theme('pager');

  return $html;
}

/**
 * theme the search form
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_admin_content_overview_form($form) {
  $form = reset($form);
  unset($form['query']['#title']);

  $html = '<div id="archibald_admin_content_overview_search_form">';
  $html .= '<div>' . theme('textfield', $form['query']) . '</div>';

  if (!empty($form['own'])) {
    $html .= '<div>' . t('My descriptions') . ' ' . theme('checkbox', $form['own']) . '</div>';
  }

  if (!empty($form['status'])) {
    $html .= '<div>' . theme('select', $form['status']) . '</div>';
  }

  $html .= '<div>' . drupal_render($form['search']) . '</div>';
  $html .= '<div>' . drupal_render($form['reset']) . '</div>';
  $html .= '</div>';
  return $html;
}

/**
 * return a list of users with can edit archibald content
 * @global object $user current logged in user
 *
 * @param $with_email
 *   Whether to include the email from the user in bracet or not (optional, default = FALSE).
 *
 * @return array
 *   A list with all archibald users.
 */
function archibald_get_all_user_editor($with_email = FALSE) {
  $required_user_access = 'edit archibald content';

  $sql = 'SELECT u.uid, u.name, u.mail FROM {users} u
    LEFT JOIN {users_roles} ur ON (u.uid=ur.uid)
    LEFT JOIN {role_permission} p ON (ur.rid=p.rid)
    WHERE p.permission = :required_user_access';
  // LEFT JOIN {role} r ON (ur.rid=r.rid)

  $users = array();
  $result = db_query(
    $sql, array(
      ':required_user_access' => $required_user_access,
    )
  );

  foreach ($result as $account) {
    $users[$account->uid] = $account->name . (($with_email) ? ' (' . $account->mail . ')' : '');
  }
  return $users;
}

/**
 * theme the responsible link item
 *
 * @param array $variables
 *
 * @return string
 */
function theme_archibald_admin_content_overview_reponsible($variables) {
  global $user;

  // Make sure that logged on user is only allowed to set new person responsible in case
  // at least one editor is available.
  $available_editors = archibald_get_all_user_editor();
  unset($available_editors[$user->uid]);
  if (count($available_editors) > 0) {
    $span = '<span class="responsible_editor" lom_id="' . $variables['lom_id'] . '" url="' .
      url('archibald/' . $variables['lom_id'] . '/set_new_responsible') .
      '" title=\'' . t('Assign to another editor') . '\'>';
  }
  else {
    $span = '<span>';
  }

  if (!empty($variables['uid']) && $variables['uid'] == $user->uid) {
    return $span . t('Me') . '</span>';
  }
  else {
    $account = (object)array(
      'uid' => $variables['uid'],
      'name' => $variables['name'],
    );

    $attributes_array = array();
    if (empty($variables['uid'])) {
      return $span . format_username($account) . '</span>';
    }
    else {
      return '<span>' . format_username($account) . '</span>';
    }
  }
}

/**
 * Menu callback; displays a listing of log messages.
 *
 * Messages are truncated at 56 chars. Full-length message could be viewed at
 * the message details page.
 *
 * @param string $lom_id
 * @param string $type
 *   comma seperated list of watchdog types
 *   example:
 *    file_upload
 *    file_replacing
 *    lom_upload
 *    lom_auth
 *    solr_index
 *    comment add
 *    comment delete
 *    .....
 */
function archibald_dblog_overview($lom_id, $type = '') {
  $rows = array();
  $classes = array(
    WATCHDOG_DEBUG => 'dblog-debug',
    WATCHDOG_INFO => 'dblog-info',
    WATCHDOG_NOTICE => 'dblog-notice',
    WATCHDOG_WARNING => 'dblog-warning',
    WATCHDOG_ERROR => 'dblog-error',
    WATCHDOG_CRITICAL => 'dblog-critical',
    WATCHDOG_ALERT => 'dblog-alert',
    WATCHDOG_EMERGENCY => 'dblog-emerg',
  );

  $filter = array(
    'where' => 'link = :link',
    'args' => array(
      ':link' => 'archibald/' . $lom_id,
    ),
  );

  if (!empty($type)) {
    $type = explode(',', $type);
    $filter['where'] .= " AND type IN ('" . implode("', '", $type) . "')";
  }

  $rows[] = array(
    t('Date - Time'),
    t('Message'),
    t('User'),
  );

  $query = db_select('watchdog', 'w')->extend('PagerDefault')->extend('TableSort');

  $query->leftJoin('users', 'u', 'w.uid = u.uid');
  $query->fields(
    'w', array(
      'wid',
      'uid',
      'severity',
      'type',
      'timestamp',
      'message',
      'variables',
      'link',
    )
  )
  ->orderBy('timestamp', 'DESC')
  ->addField('u', 'name');

  if (!empty($filter['where'])) {
    $query->where($filter['where'], $filter['args']);
  }
  $result = $query->limit(50)->execute();

  foreach ($result as $dblog) {
    $rows[] = array('data' =>
      array(
        // Cells
        format_date($dblog->timestamp, 'short'),
        theme(
          'archibald_dblog_message',
          array(
            'event' => $dblog,
            'link' => TRUE,
          )
        ),
        theme('username', array('account' => $dblog)),
      ),
      // Attributes for tr
      'class' => array(
        drupal_html_class('dblog-' . $dblog->type),
        $classes[$dblog->severity],
      ),
    );
  }

  $build['dblog_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#attributes' => array('id' => 'admin-dblog'),
    '#empty' => t('No log messages available.')
  );
  $build['dblog_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Generates a diff between two lom objects.
 *
 * @param string $lom_id
 *   The lom id
 * @param string $revision_a
 *   The old revision
 * @param string $revision_b
 *   The new revision, if an empty string is provided (default) the last
 *   revision will be used
 *   (optiona, default = '')
 */
function archibald_lom_diff($lom_id, $revision_a, $revision_b = "") {
  $json_a = json_decode(ArchibaldLomSaveHandler::jsonExport(ArchibaldLomSaveHandler::load($lom_id, $revision_a)), TRUE);
  $json_b = json_decode(ArchibaldLomSaveHandler::jsonExport(ArchibaldLomSaveHandler::load($lom_id, $revision_b)), TRUE);

  $diff = array();
  archibald_lom_diff_recrusive($json_a, $json_b, $diff);

}

/**
 * Recrusive function which found out the diff's for the 2 lom resource arrays.
 *
 * @param array $lom_a
 *   The first lom object
 * @param array $lom_b
 *   The second lom object
 */
function archibald_lom_diff_recrusive(array $lom_a, array $lom_b, &$diff, $key = "") {
  foreach ($lom_a AS $k => $v) {
    if (!empty($key)) {
      $new_key = $key . "." . $k;
    }
    else {
      $new_key = $k;
    }

    if (!isset($lom_b[$k])) {
      $diff[$new_key] = t("deleted entry: @entry", array('@entry' => $k));
      continue;
    }

    if (is_array($v)) {
      archibald_lom_diff_recrusive($lom_a[$k], $lom_b[$k], $diff, $new_key);
      continue;
    }

    if ($lom_a[$k] != $lom_b[$k]) {
      $diff[$new_key] = t("@key changed from @a to @b", array(
        '@key' => $new_key,
        '@a' => $lom_a[$k],
        '@b' => $lom_b[$k],
      ));
    }

    unset($lom_b[$k]);
  }

  foreach ($lom_b AS $k => $v) {
    if (!empty($key)) {
      $new_key = $key . "." . $k;
    }
    else {
      $new_key = $k;
    }
    if (is_array($v)) {
      archibald_lom_diff_recrusive(array(), $v, $diff, $new_key);
      continue;
    }
    $diff[$new_key] = t("New value for @key added: @val", array(
      '@key' => $new_key,
      '@val' => $v,
    ));
  }
}

/**
 * Returns HTML for a log message.
 *
 * @param $variables
 *   An associative array containing:
 *   - event: An object with at least the message and variables properties.
 *   - link: (optional) Format message as link, event->wid is required.
 *
 * @ingroup themeable
 */
function theme_archibald_dblog_message($variables) {
  $output = '';
  #print_r($event);
  $event = $variables['event'];
  // Check for required properties.
  if (isset($event->message) && isset($event->variables)) {
    // Messages without variables or user specified text.
    if ($event->variables === 'N;') {
      $output = $event->message;
    }
    // Message to translate with injected variables.
    else {
      $output = t($event->message, unserialize($event->variables));
    }
    if ($variables['link'] && isset($event->wid)) {
      // Truncate message to 56 chars.
      $output = truncate_utf8(
        filter_xss(
          $output,
          array()
        ),
        95,
        TRUE,
        TRUE
      );

      $output = l(
        $output,
        'admin/reports/event/' . $event->wid,
        array(
          'html' => TRUE,
        )
      );
    }
  }
  return $output;
}

/**
 *
 * This is a page callback function displaying a table with all descriptions which need to be republished.
 * The only indicator for the republication is the 'republication required' flag in the archibald_lom_stats table.
 *
 * @return string HTML
 */
function archibald_admin_to_republish() {

  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');

  $search = new ArchibaldLomSQLSearch();
  $search->setRepublicationRequiredFilter();
  $num_per_page = variable_get('archibald_admin_content_num_per_page', 25);
  $search->setLimit($num_per_page);
  $page = pager_default_initialize($search->getTotalRecordCount(), $num_per_page);
  $search->setOffset($page * $num_per_page);

  $confirmation_form = drupal_get_form('archibald_admin_to_republish_form');

  return theme('archibald_admin_to_republish', array(
    'search_result' => $search->search(),
    'confirmation_form' => $confirmation_form,
    'search_result_count' => $search->getTotalRecordCount()
  ));
}

/**
 * Implement hook_form. Showing two buttons to confirm or cancel the republication.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array drupal form API
 */
function archibald_admin_to_republish_form($form, &$form_state) {

  $form['hint'] = array(
    '#markup' => t('Keywords, coverages, vCards or the content partner logo were changed in the following nationally published descriptons.') .
      '<br/>' . t('For that reason the descriptions need to be republished.') . '<br/><br/>'
  );

  $form['confirmation'] = array(
    '#type' => 'submit',
    '#value' => t('Republish all descriptions'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Handles the republication form submit. In case user clicks 'cancel' go back to the main description list.
 * In case user clicks 'republish all descriptions' call archibald_reset_republication_required() and go to
 * page showing the republication process of all descriptions.
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_admin_to_republish_form_submit($form, &$form_state) {

  if ($form_state['clicked_button']['#id'] == 'edit-confirmation') {
    archibald_reset_republication_required();
    archibald_goto_republish_all();
  }
  elseif ($form_state['clicked_button']['#id'] == 'edit-cancel') {
    drupal_goto('admin/archibald');
  }
}


/**
 * Theme the table containting all descriptions which need to be republished.
 *
 * @param array $variables all variables needed to build the theme
 *
 * @return string HTML
 */
function theme_archibald_admin_to_republish($variables) {

  $html = drupal_render($variables['confirmation_form']);

  drupal_add_js(
    array(
      'archibald' => array(
        'urls' => array(
          'archibald_change_log' => url('archibald/change_log/'),
        ),
      ),
    ),
    'setting'
  );

  $table = array(
    'header' =>
    array(
      t('Link to description'),
      t('Change log'),
      t('Status'),
      t('Person responsible'),
    ),
    'rows' => array(),
    'attributes' => array('id' => 'archibald_admin_to_republish'),
  );

  // generate table from result set
  foreach ($variables['search_result'] as $res_row) {
    $row = array(0 => '');

    if (empty($res_row->status)) {
      $res_row->status = 'unavailable';
    }

    // Determine title.
    $title = ArchibaldLom::determinTitle($res_row->title, $res_row->default_language);
    $row[0] = l($title, 'archibald/' . $res_row->lom_id . '/revisions/' . $res_row->version . '/view');

    $row[] = "<span class='archibald_hover_change_log' lom_id='" . $res_row->lom_id . "'>" . t('View change log') . "</span>";

    $row[] = theme(
      'archibald_admin_content_publish_status',
      array(
        'res_row' => $res_row,
      )
    );

    $row[] = theme(
      'archibald_admin_content_overview_reponsible',
      array(
        'uid' => $res_row->responsible_uid,
        'name' => $res_row->user_name,
        'lom_id' => $res_row->lom_id,
      )
    );

    $table['rows'][] = $row;
  }

  $html .= theme('table', $table);

  $html .= theme('pager');

  return $html;

}

/**
 * Clear the cache for all lom objects including all revision where the
 * given langstring id is found within keywords or coverages.
 *
 * @param string $langstring_id
 *   the langstring id.
 */
function archibald_reset_lom_cache($langstring_id) {
  // Get all resource which includes the given langstring id as keyword or coverage.
  // Only those must be cleared from cache.
  $sql = "
    SELECT
      l.lom_id, l.version
    FROM {archibald_lom} l
    LEFT JOIN {archibald_general_keywords} gk ON (l.general_id = gk.general_id)
    LEFT JOIN {archibald_general_coverage} gc ON (l.general_id = gc.general_id)
    WHERE
      gk.keyword = :langstring_id OR
      gc.coverage = :langstring_id
    GROUP BY
      l.lom_id, l.version
  ";

  $params = array(
    ':langstring_id' => $langstring_id
  );

  $res = db_query($sql, $params);
  while ($row = $res->fetchAssoc()) {
    cache_clear_all('lom:' . $row['lom_id'] . '__version:' . $row['version'], 'cache');
  }
}