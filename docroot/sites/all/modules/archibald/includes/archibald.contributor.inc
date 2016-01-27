<?php

/**
 * @file
 * handler for contributor / vCards
 * mange and choose contributor
 */
define('ARCHIBALD_CONRIBUTER_PER_PAGE', 14);

require 'archibald.contributor_edit.inc';

/**
 * contributor/vcard list/search
 *
 * @param string $operation_type
 *   (chooser|standalone)  default:chooser
 * @param string $js
 *   (nojs|ajax)
 *   default:nojs
 * @param string $query
 * @param integer $page
 *   default:0
 *
 * @return string html
 */
function archibald_choose_contributor($operation_type = 'chooser', $js = 'nojs', $query = '', $page = 0) {
  drupal_set_title(t('vCards'));
  archibald_set_header($operation_type);
  $_SESSION['archibald_contributor-last_query'] = $query;

  // pager
  $page = (INT) $page;
  if ($page >= 1) {
    $_SESSION['archibald_contributor-last_page'] = $page;
    $output = archibald_choose_contributor_search_page($query, $page, $operation_type);

    if ($js == 'ajax') {
      print $output;
      exit;
    }
    else {
      return $output;
    }
  }

  // normal ctools overlay
  $_SESSION['archibald_contributor-last_page'] = 1;
  $output = archibald_choose_contributor_search($query, $operation_type);
  if ($js == 'ajax') {
    $commands = array(ctools_modal_command_display(t('Choose vCard'), $output));

    drupal_add_http_header('Content-Type', 'application/json');
    print ajax_render($commands);
    exit;
  }

  return $output;
}

/**
 * include js and css files and set header vars
 *
 * @param string $operation_type
 *  (chooser|standalone)
 *   default:NULL
 */
function archibald_set_header($operation_type = '') {
  // Include the CTools tools that we need.
  ctools_include('ajax');
  ctools_include('modal');

  // Add CTools' javascript to the page.
  ctools_modal_add_js();

  // include standalone css + js
  drupal_add_js(drupal_get_path('module', 'archibald') . '/js/archibald.js');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald_form.css');

  // only set variable if all needen vars are set
  if (!empty($operation_type)) {
    drupal_add_js(
      array(
        'archibald' => array(
          'urls' => array(
            'contributor_list' => url('archibald_contributor/' . $operation_type . '/ajax'),
            'contributor_preview' => url('archibald_contributor/preview'),
          ),
        ),
      ),
      'setting'
    );
  }
}

/**
 * ajax callback to display vCard preview
 *
 * @param string $contributor_id
 *   the contributor id
 */
function archibald_contributor_preview($contributor_id, $return = FALSE) {
  $contributor = new ArchibaldLomContributor($contributor_id);
  $vcard       = $contributor->getVcardObject();
  $result      = theme('archibald_choose_contributor_preview', array('vcard' => $vcard));
  if ($return) {
    return $result;
  }
  echo $result;
  die();
}

/**
 * get the contributor and return it with ajax
 *
 * @param string $contributor_id the contributor id
 *
 * @return array result set
 */
function archibald_contributor_get($contributor_id) {
  $sh = db_select('archibald_contributors ', 'archibaldc')
    ->fields(
      'archibaldc',
      array(
        'lastname',
        'firstname',
        'organisation',
      )
    );

  if (!empty($contributor_id)) {
    $condition = db_or();
    $condition->condition('contributor_id', $contributor_id);
    $sh->condition($condition);
  }

  $db_result = $sh->execute()->fetchAssoc();
  if ($db_result) {
    return drupal_json_output($db_result);
  }

  return drupal_json_output('');
}

/**
 * performace a contributor search
 *
 * @param string $query
 * @param integer $page
 *   default=1
 * @param integer $per_page
 *   default=25
 * @param integer $pages
 *   default=1
 *   called by refference
 *   number of pages
 *
 * @return array
 *   result set
 */
function archibald_contributor_search($query, $page = 1, $per_page = 25, &$pages = 1) {

  $sh = db_select('archibald_contributors ', 'contrers')
    ->fields(
      'contrers',
      array(
        'contributor_id',
        'lastname',
        'firstname',
        'organisation',
        'country',
        'zip',
        'city',
        'address1',
        'address2',
        'email',
        'url',
        'tel',
      )
    );

  // Make sure that vCard List is sorted by organisation. When organisation is not defined, use lastname for sorting.
  $db_connection = Database::getConnection('default');
  if ($db_connection instanceof DatabaseConnection_mysql || $db_connection instanceof DatabaseConnection_pgsql) {
    $sh->addExpression('(organisation||lastname)', 'field_to_order');
  }
  else {
    $sh->addField('contrers', 'organisation', 'field_to_order');
  }

  $shc = db_select('archibald_contributors ', 'contrers');
  $shc->addExpression('COUNT(contributor_id)', 'count');

  if (!empty($query)) {
    $query = trim($query);
    $condition = db_or();
    $add_condition = FALSE;
    $words = preg_split('/[\s]+/', $query);
    foreach ($words as $word) {
      if (!empty($word)) {
        $add_condition = TRUE;
        $condition->condition('firstname', '%' . $word . '%', 'LIKE');
        $condition->condition('lastname', '%' . $word . '%', 'LIKE');
        $condition->condition('organisation', '%' . $word . '%', 'LIKE');
        $condition->condition('country', '%' . $word . '%', 'LIKE');
        $condition->condition('city', '%' . $word . '%', 'LIKE');
        $condition->condition('address1', '%' . $word . '%', 'LIKE');
        $condition->condition('email', '%' . $word . '%', 'LIKE');
        $condition->condition('url', '%' . $word . '%', 'LIKE');
      }
    }
    if ($add_condition) {
      $sh->condition($condition);
      $shc->condition($condition);
    }
  }

  $pages = ceil($shc->execute()->fetchField() / $per_page);
  if ($pages < 1) {
    $pages = 1;
  }

  if ($page > $pages) {
    $page = $pages;
  }

  if ($page < 1) {
    $page = 1;
  }

  $sh->orderBy('field_to_order', 'ASC')
    ->orderBy('lastname', 'ASC')
    ->orderBy('firstname', 'ASC')
    ->range(($page - 1) * $per_page, $per_page);

  $result = array();
  $db_result = $sh->execute();
  while ($row = $db_result->fetchAssoc()) {
    $result[] = $row;
  }

  return $result;
}

/**
 * Search for contributors.
 *
 * @param string $query
 *  search string
 * @param string $operation_type
 *  (chooser|standalone)  default:chooser
 * @param string $contributor_id
 *  to select
 *
 * @return string
 *   the html.
 */
function archibald_choose_contributor_search($query = '', $operation_type = 'chooser', $contributor_id = '') {

  $search_result = archibald_contributor_search($query, 1, ARCHIBALD_CONRIBUTER_PER_PAGE, $pages);

  return theme(
    'archibald_choose_contributor_search',
    array(
      'search_result' => $search_result,
      'pages' => $pages,
      'query' => (!empty($_POST['query']) ? check_plain($_POST['query']) : ''),
      'contributor_id' => $contributor_id,
      'operation_type' => $operation_type,
    )
  );
}

/**
 * search for contributors ajax request
 *
 * @param array $form
 * @param array $form_state
 *
 * @return string html
 */
function archibald_choose_contributor_search_ajax($form, $form_state) {
  $search_result = archibald_contributor_search($form_state['values']['query'], 1, ARCHIBALD_CONRIBUTER_PER_PAGE, $pages);

  return theme(
    'archibald_choose_contributor_search',
    array(
      'search_result' => $search_result,
      'pages' => $pages,
      'query' => $form_state['values']['query'],
      'ajax' => TRUE,
      'operation_type' => $form_state['values']['operation_type'],
    )
  );
}

/**
 * search for contributors ajax request
 *
 * @param string $query
 * @param integer $page
 * @param string $operation_type   (chooser|standalone)  default:chooser
 *
 * @return string html
 */
function archibald_choose_contributor_search_page($query = '', $page = 1, $operation_type = 'chooser') {

  $search_result = archibald_contributor_search($query, $page, ARCHIBALD_CONRIBUTER_PER_PAGE, $pages);

  return theme(
    'archibald_choose_contributor_search',
    array(
      'search_result' => $search_result,
      'pages' => $pages,
      'page' => $page,
      'query' => $query,
      'ajax' => TRUE,
      'operation_type' => $operation_type,
    )
  );
}

/**
 * theme archibald contributor search
 *
 * @param array $variables
 *
 * @return string
 */
function theme_archibald_choose_contributor_search($variables) {
  ctools_include('ajax');

  if (!isset($variables['contributor_id'])) {
    $variables['contributor_id'] = '';
  }

  if (!isset($variables['page'])) {
    $variables['page'] = 1;
  }

  // if we came from the add form and saved this success full,
  // the new item needs to be in list to be choosen
  if (!empty($variables['contributor_id'])) {
    $res_row = db_select('archibald_contributors ', 'contrers')
      ->fields(
        'contrers',
        array(
          'contributor_id',
          'lastname',
          'firstname',
          'organisation',
          'country',
          'zip',
          'city',
          'address1',
          'address2',
          'email',
          'url',
          'tel',
        )
      )
      ->condition('contributor_id', $variables['contributor_id'], '=')
      ->execute()
      ->fetchAssoc();

    $variables['search_result'][] = $res_row;
  }


  $html = '';
  $vq = $variables['query'];
  $vo = $variables['operation_type'];
  $render = drupal_get_form(
    'archibald_choose_contributor_search_form',
    $vq,
    $vo
  );
  if (empty($variables['ajax'])) {
    $html .= drupal_render(
      $render
    );
  }

  $table = array(
    'header' => array(
      t('Person'),
      t('Options'),
    ),
    'rows' => array(),
  );

  // generate table from result set
  foreach ($variables['search_result'] as $res_row) {
    $edit_url = 'archibald_contributor/edit/' . $variables['operation_type'] . '/nojs/' . $res_row['contributor_id'] . '/0/' . (!empty($_POST['self_lom_id']) ? check_plain($_POST['self_lom_id']) : '');
    $delete_url = 'archibald_contributor/delete/' . $res_row['contributor_id'];
    // "/nojs" will replaced by ctools to "/ajax" if possible

    $row = array(0 => '', 1 => array());

    $row_labels = array($res_row['organisation'], $res_row['lastname'] . ' ' . $res_row['firstname']);
    $row_label = implode(', ', array_filter($row_labels, 'archibald_is_empty'));

    if ($variables['operation_type'] == 'standalone') {
      $row[0] = ctools_ajax_text_button(
        trim($row_label), $edit_url, t('Edit contributor')
      );
    }
    else {
      $row[0] = array(
        'data' => trim($row_label),
        'class' => array(
          'archibald_contributor_list_name archibald_contributor_choose',
        ),
        'contributor_id' => $res_row['contributor_id'],
      );
    }

    if (!empty($_POST['entity']) && $_POST['entity'] == $res_row['contributor_id']) {
      $row[0]['class'][] = 'archibald_contributor_list_current_activ';
    }

    $attributes = array(
      'class' => array(
        'archibald_contributor_choose',
      ),
      'contributor_id' => $res_row['contributor_id'],
    );

    if ($variables['contributor_id'] == $res_row['contributor_id']) {
      $attributes['class'][] = ' archibald_contributor_choose_this';
    }

    if ($variables['operation_type'] == 'chooser') {
      $row[1][] = l(
        t('Choose'), 'archibald_contributor/choose/' . $res_row['contributor_id'], array('attributes' => $attributes));
      if (archibald_can_change_vcards()) {
        $row[1][] = ctools_ajax_text_button(t('Edit'), $edit_url, t('Edit vCard'));
      }
    }
    else if (archibald_can_change_vcards()) {
      $row[1][] = ctools_ajax_text_button(t('Edit'), $edit_url, t('Edit vCard'));
      $row[1][] = l(t('Delete'), $delete_url);
    }

    $row[1] = implode(' | ', $row[1]);

    $table['rows'][] = $row;
  }

  $content = theme('table', $table);

  // Start of Pager
  $items = array();
  for ($p = 1; $p <= $variables['pages']; $p++) {
    if ($p == $variables['page']) {
      $items[] = array(
        'class' => array(
          'pager-current',
        ),
        'data' => $p,
      );
    }
    else {
      $items[] = array(
        'class' => array(
          'pager-item',
        ),
        'data' => l(
          $p,
          'archibald_contributor/' . $variables['operation_type'] . '/nojs/' . urlencode($variables['query']) . '/' . $p,
          array(
            'attributes' => array(
              'class' => array(
                'archibald_contriberter_page',
              ),
            ),
          )
        ),
      );
    }
  }

  $content .= '<h2 class="element-invisible">' . t('Pages') . '</h2>' .
    theme(
      'item_list',
      array(
        'items' => $items,
        'attributes' => array(
          'class' => array(
            'pager',
          ),
        ),
      )
    );

  $html .= '<div id="archibald_choose_contributor_search_result">' . $content . '</div>';

  return $html;
}

/**
 * search formular for archibald contributor search
 *
 * @param array $form
 * @param array $form_state
 * @param string $query
 * @param string $operation_type
 *  (chooser|standalone)  default:chooser
 *
 * @return array
 */
function archibald_choose_contributor_search_form($form, &$form_state, $query, $operation_type = 'chooser') {

  $form['operation_type'] = array(
    '#type' => 'hidden',
    '#value' => $operation_type,
  );

  $self_lom_id = '';
  if (!empty($_POST['self_lom_id'])) {
    $self_lom_id = check_plain($_POST['self_lom_id']);
  }

  $form['self_lom_id'] = array(
    '#type' => 'hidden',
    '#value' => $self_lom_id,
  );

  $current_entity = '';
  if (!empty($_POST['entity'])) {
    $current_entity = check_plain($_POST['entity']);
  }

  $form['entity'] = array(
    '#type' => 'hidden',
    '#value' => $current_entity,
  );


  $form['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Term'),
    '#size' => 40,
    '#default_value' => $query,
    '#attributes' => array(
      'class' => array(
        'archibald_choose_contributor_query',
      ),
    ),
    '#ajax' => array(
      'callback' => 'archibald_choose_contributor_search_ajax',
      'wrapper' => 'archibald_choose_contributor_search_result',
      'method' => 'replace',
      'effect' => 'fade',
      'keypress' => TRUE,
    ),
  );

  $form['search'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#weight' => 9999,
    '#ajax' => array(
      'callback' => 'archibald_choose_contributor_search_ajax',
      'wrapper' => 'archibald_choose_contributor_search_result',
      'method' => 'replace',
      'effect' => 'fade'
    ),
  );

  $form['#theme'] = 'archibald_choose_contributor_search_form';

  return $form;
}

/**
 * theme the search form
 *
 * @param array $variables
 *
 * @return string
 */
function theme_archibald_choose_contributor_search_form($variables) {
  ctools_include('ajax');

  $form = reset($variables);

  $add_url = 'archibald_contributor/add/' . $form['operation_type']['#value'] . '/nojs';
  // "/nojs" will replaced by ctools to "/ajax" if possible

  if ($form['operation_type']['#value'] == 'standalone') {
    $width = 450;
  }
  else {
    $width = 550;
  }

  unset($form['query']['#title']);
  $html = '';
  if (isset($_POST['entity']) && !empty($_POST['entity'])) {
    $html .= '<input type="button" class="archibald_contributor_reset form-submit" style="float:left" value="' . t('Reset') . '">';
  }
  $html .= '<div style="text-align:right; float:right;">';
  $html .= drupal_render($form['query']) . drupal_render($form['search']);
  $form['query']['#printed'] = TRUE;
  $html .= '</div>';

  $button = ctools_ajax_text_button(t('Add vCard'), $add_url, t('Add vCard'));
  $html .= '<ul class="action-links"><li>' . $button . '</li></ul>';
  $html .= drupal_render_children($form);

  return $html;
}

/**
 * menu callback for choosing a menu item
 *
 * @param string $contributor_id
 *
 * @return string html
 */
function archibald_contributor_choose($contributor_id) {
  // display search result and mark edited items as choosen
  // cause of this by js the modal frame will dismissed
  return archibald_choose_contributor_search($_SESSION['archibald_contributor-last_query'], 'chooser', $contributor_id);
}

/**
 * clean up telefonumber and try to normalize it
 *
 * @param string $org_nr
 *
 * @return string
 */
function archibald_contributor_clean_tel($org_nr) {
  if (empty($org_nr)) {
    return '';
  }

  $nr = preg_replace('~[^0-9]+~i', '', $org_nr);

  if (drupal_substr($org_nr, 0, 1) == '+') {
    return '+' . $nr;
  }

  $nr = preg_replace('~[^0-9]+~i', '', $nr);

  $x1 = drupal_substr($nr, 0, 1);
  $x2 = drupal_substr($nr, 1, 2);

  if ($x1 == '0' && $x2 == '0') {
    return '+' . drupal_substr($nr, 2);
  }
  elseif ($x1 == '0' && $x2 > 0) {
    return '+41' . drupal_substr($nr, 1);
  }
  else {
    return $org_nr;
  }
}

/**
 * theming function for contributor preview ajax caller
 *
 * @param $variables array(vcard=>ArchibaldAppDataVcard)
 *
 * @return string html
 */
function theme_archibald_choose_contributor_preview($variables) {
    #'header' => array(t('Title'), t('Value')),
  $table = array(
    'colgroups' => array(array(array('width' => '40%'), array())),
    'rows' => array(),
  );

  if (!isset($variables['vcard'])) {
    return '';
  }
  $table['rows'][] = array(t('First name'), $variables['vcard']->firstname);
  $table['rows'][] = array(t('Last name'), $variables['vcard']->lastname);
  $table['rows'][] = array(t('Organisation'), $variables['vcard']->organization);

  $address = $variables['vcard']->address;
  if (!empty($address[0])) {
    $table['rows'][] = array(t('Country'), @$address[0]['country']);
    $table['rows'][] = array(t('ZIP'), @$address[0]['zip']);
    $table['rows'][] = array(t('City'), @$address[0]['city']);
    $table['rows'][] = array(t('Address'), @$address[0]['street']);
    $table['rows'][] = array(t('Additional address'), @$address[0]['extendedaddress']);
  }

  $email = $variables['vcard']->email;
  if (!empty($email[0]['value'])) {
    $table['rows'][] = array(t('E-Mail'), @$email[0]['value']);
  }

  $url = $variables['vcard']->url;
  if (!empty($url[0]['value'])) {
    $table['rows'][] = array(t('URL'), @$url[0]['value']);
  }

  $telephone = $variables['vcard']->telephone;
  if (!empty($telephone[0]['value'])) {
    $table['rows'][] = array(t('Work telephone'), @$telephone[0]['value']);
  }

  /**
   * HH 2012-05-04 vcard icons are not longer supported
   * CHA 2012-06-15 vcard enabled again
   */
  $default_value = NULL;
  if (isset($variables['vcard']->logo->uri)) {
    $default_value = $variables['vcard']->logo->uri;
  }
  preg_match('/archibald_file\/([0-9]+)\//', $default_value, $old_fid);
  if (!empty($old_fid[1])) {
    $file            = file_load($old_fid[1]);
    $style_name      = 'thumbnail';
    $table['rows'][] = array(
      t('Logo'),
      theme('image_style',
        array(
          'style_name' => (empty($style_name)) ? 'thumbnail' : $style_name,
          'path' => $file->uri,
          'getsize' => FALSE,
          'attributes' => array(
            'fid' => $file->fid
          ),
        )
      ),
    );
  }

  return theme('table', $table);
}

/**
 * Callback function for array_filter function.
 *
 * @param mixed $array_element the array element
 *
 * @return if element is empty or contains only spaces, return FALSE, otherwise return TRUE
 */
function archibald_is_empty($array_element) {
  $array_element = str_replace(' ' , '', $array_element);
  if (empty($array_element)) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}