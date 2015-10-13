<?php

/**
 * @file
 * lom resource add/edit form part: for keywords tab
 */

/**
 * part of lom resource add/edit form
 *  tab: keywords
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 */
function archibald_content_form_keywords(&$form, &$form_state, &$lom, $language) {

  $lom_general = $lom->getGeneral();
  if (!($lom_general instanceof ArchibaldLomDataGeneral)) {
    $lom_general = new ArchibaldLomDataGeneral();
  }

  $keyword_access = archibald_check_field_permission('keywords keyword');
  $coverage_access = archibald_check_field_permission('keywords coverage');

  if ($keyword_access || $coverage_access) {
    $form['keywords'] = array(
      '#type' => 'fieldset',
      '#title' => '&nbsp;&nbsp;&nbsp;&nbsp;' . t('Tags'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#group' => 'vt_main',
      '#tree' => TRUE,
      '#theme' => 'archibald_keywords',
    );
  }

  if ($keyword_access) {

    /**
     * START Keyword handling
     */
    $form['keywords']['keyword'] = array(
      '#type' => 'fieldset',
      '#title' => '',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes' => array('archibald_help_ident' => 'general.keyword'),
      '#prefix' => '<div class="clearfix" id="archibald_general_keyword-wrapper">',
      '#suffix' => '</div>',
      '#theme' => 'archibald_general_keyword',
    );

    /** Load keywords submitted by add more button * */
    $num_general_keyword = 0;
    $keywords = $lom_general->getKeyword();

    if (!empty($keywords)) {

      foreach ($keywords as $keyword) {
        $default_value = '';
        if ($keyword instanceof ArchibaldLomDataLangString) {
          $default_value = $keyword->getString($language);
          if (empty($default_value)) {
            // If the string is not within the current language, skip it.
            continue;
          }
        }

        // print_r( $keyword );

        archibald_content_form_general_add_keyword($form, $form_state, $language, 'keyword_' . $keyword->getId(), $default_value);
        $num_general_keyword++;
      }
      // die();
    }
    if (!isset($form_state['num_general_keyword'])) {
      $form_state['num_general_keyword'] = $num_general_keyword + 3;
    }

    for ($i = $num_general_keyword; $i < $form_state['num_general_keyword']; $i++) {
      archibald_content_form_general_add_keyword($form, $form_state, $language);
    }

    $form['keywords']['keyword']['add_more_general_keyword'] = array(
      '#type' => 'submit',
      '#value' => t('Add 3 keywords'),
      '#weight' => 9999,
      '#limit_validation_errors' => array(),
      //submit is required to use #limit_validation_errors
      '#submit' => array(
        'archibald_content_form_add_more_general_keyword_submit',
      ),
      '#ajax' => array(
        'callback' => 'archibald_ajax_content_form_general_add_empty_keyword',
        'wrapper' => 'archibald_general_keyword-wrapper',
        'method' => 'replace',
        #'effect'   => 'fade',
      ),
    );
  }


  if ($coverage_access) {

    /**
     * START coverage handling
     */
    $form['keywords']['coverage'] = array(
      '#type' => 'fieldset',
      '#title' => '',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#attributes' => array('archibald_help_ident' => 'general.coverage'),
      '#prefix' => '<div class="clearfix" id="archibald_general_coverage-wrapper">',
      '#suffix' => '</div>',
      '#theme' => 'archibald_general_coverage',
    );

    /** Load coverage submitted by add more button * */
    $num_general_coverage = 0;

    $coverages = $lom_general->getCoverage();
    if (!empty($coverages)) {
      foreach ($coverages as $coverage) {
        $default_value = '';
        if ($coverage instanceof ArchibaldLomDataLangString) {
          $default_value = $coverage->getString($language);

          if (empty($default_value)) {
            // If the string is not within the current language, skip it.
            continue;
          }
        }

        archibald_content_form_general_add_coverage($form, $form_state, $language, 'coverage_' . $coverage->getId(), $default_value);
        $num_general_coverage++;
      }
    }

    if (!isset($form_state['num_general_coverage'])) {
      $form_state['num_general_coverage'] = $num_general_coverage + 3;
    }

    for ($i = $num_general_coverage; $i < $form_state['num_general_coverage']; $i++) {
      archibald_content_form_general_add_coverage($form, $form_state, $language);
    }

    $form['keywords']['coverage']['add_more_general_coverage'] = array(
      '#type' => 'submit',
      '#value' => t('Add 3 coverages'),
      '#weight' => 9999,
      '#limit_validation_errors' => array(),
      // #submit is required to use #limit_validation_errors
      '#submit' => array(
        'archibald_content_form_add_more_general_coverage_submit',
      ),
      '#ajax' => array(
        'callback' => 'archibald_ajax_content_form_general_add_empty_coverage',
        'wrapper' => 'archibald_general_coverage-wrapper',
        'method' => 'replace',
        #'effect'   => 'fade',
      ),
    );
  }
}

/**
 * add new general keyword, this could called by default form or ajax callback
 *
 * @param array $form
 * @param array $form_state
 * @param string $id
 *   the id of keyword
 * @param string $default_value
 *   default value
 *   is only used in cae of a untranslated langString
 */
function archibald_content_form_general_add_keyword(&$form, &$form_state, $language, $id = '', $default_value = '') {

  if (empty($id)) {
    $i = 0;
    foreach (array_keys($form['keywords']['keyword']) as $k) {
      if (drupal_substr($k, 0, 8) == 'keyword_') {
        $i++;
      }
    }
    $id = 'keyword_' . $i;
  }


  $form['keywords']['keyword'][$id] = array(
    '#type' => 'textfield',
    '#title' => archibald_get_field_label('general.keyword'),
    '#autocomplete_path' => 'archibald/autocomplete/keyword/' . $language,
    '#default_value' => $default_value,
    '#attributes' => array('class' => array('general_keyword')),
    '#size' => 40,
  );
}

/**
 * ajax menu callback
 * place new empty keywords fields on bottom of keywords list
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_ajax_content_form_general_add_empty_keyword($form, $form_state) {
  return $form['keywords']['keyword'];
}

$_archibald_keyword_id_count = 0;

/**
 * theme a archibald form element general keywords
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_general_keyword($form) {
  global $_archibald_keyword_id_count;
  $form = reset($form);

  $output = '<label for="edit-general-keyword-' . $_archibald_keyword_id_count++ . '">' . t('Keywords') .
    archibald_get_require_type_value('general.keyword') .
    archibald_get_field_help('general.keyword');

  $output .= '</label>';

  $output .= '<div id="archibald_general_keyword">';

  foreach ($form as $k => $element) {
    if (drupal_substr($k, 0, 8) == 'keyword_') {
      unset($element['#title']);
      $output .= drupal_render($element);
    }
  }

  $output .= '</div>';
  $output .= drupal_render($form['add_more_general_keyword']);

  return $output;
}

/**
 * add new general coverage, this could called by default form or ajax callback
 *
 * @param array $form
 * @param array $form_state
 * @param string $id
 *   the id of keyword
 * @param string $default_value
 *   default value
 *   is only used in cae of a untranslated langString
 */
function archibald_content_form_general_add_coverage(&$form, &$form_state, $language, $id = '', $default_value = '') {

  if (empty($id)) {
    $i = 0;
    foreach (array_keys($form['keywords']['coverage']) as $k) {
      if (drupal_substr($k, 0, 9) == 'coverage_') {
        $i++;
      }
    }
    $id = 'coverage_' . $i;
  }

  $form['keywords']['coverage'][$id] = array(
    '#type' => 'textfield',
    '#title' => archibald_get_field_label('general.coverage'),
    '#autocomplete_path' => 'archibald/autocomplete/coverage/' . $language,
    '#default_value' => $default_value,
    '#attributes' => array('class' => array('general_coverage')),
    '#size' => 40,
  );
}

/**
 * theme a archibald form element general coverage
 *
 * @param array $form
 *
 * @return string
 */
function archibald_ajax_content_form_general_add_empty_coverage($form, $form_state) {

  return $form['keywords']['coverage'];
}

$_archibald_coverage_id_count = 0;

/**
 * theme a archibald form element general coverages
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_general_coverage($form) {
  global $_archibald_coverage_id_count;
  $form = reset($form);

  $output = '<label for="edit-general-coverage-' . $_archibald_coverage_id_count++ . '">' . archibald_get_field_label('general.coverage') .
    archibald_get_require_type_value('general.coverage') .
    archibald_get_field_help('general.coverage');

  $output .= '</label>';
  $output .= '<div id="archibald_general_coverage">';

  foreach ($form as $k => $element) {
    if (drupal_substr($k, 0, 9) == 'coverage_') {
      unset($element['#title']);
      $output .= drupal_render($element);
    }
  }

  $output .= '</div>';
  $output .= drupal_render($form['add_more_general_coverage']);

  return $output;
}

/**
 * Submit handler for the "Add more" button.
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_add_more_general_keyword_submit($form, &$form_state) {
  if (!isset($form_state['num_general_keyword'])) {
    $form_state['num_general_keyword'] = 0;
  }
  $form_state['num_general_keyword'] += 3;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Add more" button.
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_add_more_general_coverage_submit($form, &$form_state) {
  if (!isset($form_state['num_general_coverage'])) {
    $form_state['num_general_coverage'] = 0;
  }
  $form_state['num_general_coverage'] += 3;
  $form_state['rebuild'] = TRUE;
}

/**
 * theme a archibald form block genera
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_keywords($form) {
  $form = reset($form);

  $output = '<label for="edit-keywords-title" class="htab_title">' . t('Tags') . '</label>';
  $output .= '<table id="archibald_general" border="0">';

  $html = array('left' => '', 'right' => '', 'bottom' => '');
  foreach (element_children($form, TRUE) as $key) {
    switch ($key) {
      case 'keyword':
        $html['left'][] = drupal_render($form[$key]);
        break;

      case 'coverage':
        $html['right'][] = drupal_render($form[$key]);
        break;

      default:
        $html['bottom'][] = drupal_render($form[$key]);
        break;
    }
  }

  if (empty($html['right']) || empty($html['left'])) {
    $output .= '<tr>';
    $output .= '<td class="archibald_general_td ' . 'archibald_general_td_middle" colspan=2>';
    $output .= (!empty($html['left'])) ? implode("\n", $html['left']) : '';
    $output .= (!empty($html['right'])) ? implode("\n", $html['right']) : '';
    $output .= '</td>';
    $output .= '</tr>';
  }
  else {
    $output .= '<tr>';
    $output .= '<td class="archibald_general_td ' . 'archibald_general_td_middle_left">';
    $output .= implode("\n", $html['left']);
    $output .= '</td>';
    $output .= '<td class="archibald_general_td ' . 'archibald_general_td_middle_right">';
    $output .= implode("\n", $html['right']);
    $output .= '</td>';
    $output .= '</tr>';
  }

  if (!empty($html['bottom'])) {
    $output .= '<tr>';
    $output .= '<td class="archibald_general_td ' . 'archibald_general_td_bottom" colspan=2>';
    $output .= implode("\n", $html['bottom']);
    $output .= '</td>';
    $output .= '</tr>';
  }

  $output .= '</table>';

  return $output;
}

/*
  ..######..##.....##.########..##.....##.####.########
  .##....##.##.....##.##.....##.###...###..##.....##...
  .##.......##.....##.##.....##.####.####..##.....##...
  ..######..##.....##.########..##.###.##..##.....##...
  .......##.##.....##.##.....##.##.....##..##.....##...
  .##....##.##.....##.##.....##.##.....##..##.....##...
  ..######...#######..########..##.....##.####....##...
*/

/**
 * save handler for lom general form part
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 */
function archibald_content_form_keywords_submit($form, &$form_state, &$lom) {
  $lom_general = $lom->getGeneral();
  if (!($lom_general instanceof ArchibaldLomDataGeneral)) {
    $lom_general = new ArchibaldLomDataGeneral();
  }

  $keyword_language = $form_state['values']['language'];

  ##### handle keyword
  if (archibald_check_field_permission('keywords keyword')) {

    $new_keywords = array();
    $t = array();

    $existing_keywords = array();
    $already_added = array();
    foreach ($lom_general->getKeyword() as $k => $keyword) {
      if (!($keyword instanceof ArchibaldLomDataLangString)) {
        // this shoud realy never happen
        continue;
      }

      $lang = array_keys($keyword->strings);
      $lang = $lang[0];
      $kw_id = $keyword->getId();
      if( $lang != $keyword_language ) {
        $lang_string = new ArchibaldLomDataLangString();
        $lang_string->setString($keyword->strings[$lang], $lang);
        $lang_string->setId($kw_id);
        $new_keywords[] = $lang_string;
      } else {
        $existing_keywords[$keyword->getString($lang)] = $kw_id;
      }
    }

    foreach ($form_state['values']['keywords']['keyword'] as $i => $keyword) {

      if (empty($keyword)) {
        continue;
      }

      // Prevents double inserting.
      if (isset($already_added[$keyword]) || $keyword == 'archibald_delete') {
        continue;
      }
      $already_added[$keyword] = TRUE;

      if (preg_match("/^keyword_(.*)$/", $i, $matches)) {
        $key = $matches[1];

        if (drupal_strlen($key) == 0) {
          continue;
        }
        $lang_string = new ArchibaldLomDataLangString();
        $lang_string->setString($keyword, $keyword_language);

        $within_db = db_select('archibald_keyword_list', 'kl')
          ->fields('kl', array('langstring_id'))
          ->condition('keyword', $keyword)
          ->condition('language', $keyword_language)
          ->execute()
          ->fetchAssoc();

        if (isset($existing_keywords[$keyword])) {
          $lang_string->setId($existing_keywords[$keyword]);

          if (empty($within_db)) {
            db_insert('archibald_keyword_list')
              ->fields(array(
                'keyword' => $keyword,
                'language' => $keyword_language,
                'langstring_id' => $lang_string->getId(),
              ))
            ->execute();
          }

          $new_keywords[] = $lang_string;
          unset($existing_keywords[$keyword]);
          continue;
        }

        if (!empty($within_db)) {
          $lang_string->setId($within_db['langstring_id']);
        }
        else {
          $lang_string = new ArchibaldLomDataLangString($keyword, $keyword_language);
          ArchibaldLomSaveHandler::saveOrInsertLangString($lang_string);
        }

        if (empty($within_db)) {
          db_insert('archibald_keyword_list')
            ->fields(array(
              'keyword' => $keyword,
              'language' => $keyword_language,
              'langstring_id' => $lang_string->getId(),
            ))
            ->execute();
        }

        $new_keywords[] = $lang_string;
      }
    }

    $lom_general->setKeyword($new_keywords);
  }

  ##### handle coverage
  if (archibald_check_field_permission('keywords coverage')) {

    $new_keywords = array();

    $existing_coverages = array();
    foreach ($lom_general->getCoverage() as $kw_id => $coverage) {
      if (!($coverage instanceof ArchibaldLomDataLangString)) {
        continue;
      }

      $lang = array_keys($coverage->strings);
      $lang = $lang[0];
      $kw_id = $coverage->getId();
      if( $lang != $keyword_language ) {
        $lang_string = new ArchibaldLomDataLangString();
        $lang_string->setString($coverage->strings[$lang], $lang);
        $lang_string->setId($kw_id);
        $new_keywords[] = $lang_string;
      } else {
        $existing_coverages[$coverage->getString($lang)] = $kw_id;
      }
    }

    $already_added = array();
    foreach ($form_state['values']['keywords']['coverage'] as $i => $coverage) {

      if (empty($coverage)) {
        continue;
      }

      // Prevents double inserting.
      if (isset($already_added[$coverage])) {
        continue;
      }
      $already_added[$coverage] = TRUE;

      if (preg_match("/^coverage_(.*)$/", $i, $matches)) {
        $key = $matches[1];

        if (drupal_strlen($key) == 0) {
          continue;
        }

        $within_db = db_select('archibald_coverage_list', 'kl')
          ->fields('kl', array('langstring_id'))
          ->condition('coverage', $coverage)
          ->condition('language', $keyword_language)
          ->execute()
          ->fetchAssoc();

        if (empty($within_db)) {
          continue;
        }

        $lang_string = new ArchibaldLomDataLangString();
        $lang_string->setString($coverage, $keyword_language);
        $lang_string->setId($within_db['langstring_id']);
        $new_keywords[] = $lang_string;
      }
    }

    $lom_general->setCoverage($new_keywords);
  }

  $lom->setGeneral($lom_general);
}

