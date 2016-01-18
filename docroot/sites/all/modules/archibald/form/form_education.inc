<?php

/**
 * @file
 * lom resource add/edit form part: for education tab
 */

/**
 * part of lom resource add/edit form
 *  tab: education
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $language
 */
function archibald_content_form_education(&$form, &$form_state, &$lom, $language) {

  if (
    archibald_check_field_permission('education learningResourceTypeDocumentary') ||
    archibald_check_field_permission('education learningResourceTypePedagogical') ||
    archibald_check_field_permission('education intendedEndUserRole') ||
    archibald_check_field_permission('education typicalAgeRange') ||
    archibald_check_field_permission('education typicalLearningTime') ||
    archibald_check_field_permission('education difficulty') ||
    archibald_check_field_permission('education description') ||
    archibald_check_field_permission('technical duration')
  ) {
    $lom_education = $lom->getEducation();
    if (!($lom_education instanceof ArchibaldLomDataEducation)) {
      $lom_education = new ArchibaldLomDataEducation();
    }

    $lom_general = $lom->getGeneral();
    if (!($lom_general instanceof ArchibaldLomDataGeneral)) {
      $lom_general = new ArchibaldLomDataGeneral();
    }

    $lom_technical = $lom->getTechnical();
    if (!($lom_technical instanceof ArchibaldLomDataTechnical)) {
      $lom_technical = new ArchibaldLomDataTechnical();
    }

    $form['education'] = array(
      '#type' => 'fieldset',
      '#title' => archibald_get_field_label('education'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#group' => 'vt_main',
      '#tree' => TRUE,
      '#theme' => 'archibald_education',
    );

    /*
      @learningResourceType
      .##.......########..########
      .##.......##.....##....##...
      .##.......##.....##....##...
      .##.......########.....##...
      .##.......##...##......##...
      .##.......##....##.....##...
      .########.##.....##....##...
    */
    if (archibald_check_field_permission('education learningResourceTypeDocumentary')) {
      $default_value = archibald_lom_vocab_default_val($lom_education->getLearningResourceType('documentary'));
      $options = archibald_get_learning_resource_type_options($default_value);

      $form['education']['learningResourceType_documentary'] = array(
        '#type' => 'checkboxes',
        '#attributes' => array(
          'archibald_help_selector' => 'for|edit-education-learningresourcetype',
          'archibald_help_ident' => 'education.learning_resource_type',
        ),
        '#options' => $options['documentary'],
        '#default_value' => $default_value,
      );
    }
    if (archibald_check_field_permission('education learningResourceTypePedagogical')) {
      $default_value = archibald_lom_vocab_default_val($lom_education->getLearningResourceType('pedagogical'));
      $form['education']['learningResourceType_pedagogical'] = array(
        '#type' => 'checkboxes',
        '#attributes' => array(
          'archibald_help_selector' => 'for|edit-education-learningresourcetype',
          'archibald_help_ident' => 'education.learning_resource_type',
        ),
        '#options' => $options['pedagogical'],
        '#default_value' => $default_value,
      );
    }

    // intendedEndUserRole
    if (archibald_check_field_permission('education intendedEndUserRole')) {
      $default_value = archibald_lom_vocab_default_val($lom_education->getIntendedEndUserRole());

      $form['education']['intendedEndUserRole'] = array(
        '#type' => 'checkboxes',
        '#title' => archibald_get_field_label('education.intendedEndUserRole') .
          archibald_get_require_type_value('education.intendedEndUserRole') .
          archibald_get_field_help('education.intendedEndUserRole'),
        '#attributes' => array(
          'archibald_help_selector' => 'for|edit-education-intendedenduserrole',
          'archibald_help_ident' => 'education.intended_end_user_role',
        ),
        '#options' => archibald_get_intended_end_user_role_options($default_value),
        '#default_value' => $default_value,
      );

      // typicalAgeRange
      $default_value_from = 0;
      $default_value_to   = 0;
      $default_value      = '';
      $lang_string        = $lom_education->getTypicalAgeRange(0);
      if ($lang_string instanceof ArchibaldLomDataLangString) {
        $default_value = ArchibaldLom::getLangstringText($lang_string, 'en');
      }

      if (preg_match('/^([\-U0-9]{0,3})\-([\-U0-9]{0,3})$/i', (string) $default_value, $hits)) {
        $default_value_from = (INT) $hits[1];
        $default_value_to = (INT) $hits[2];
      }
    }

    // typicalAgeRange
    if (archibald_check_field_permission('education typicalAgeRange')) {
      $form['education']['typicalAgeRange'] = array(
        '#type' => 'markup',
        '#title' => archibald_get_field_label('education.typicalAgeRange') .
          archibald_get_require_type_value('education.typicalAgeRange') .
          archibald_get_field_help('education.typicalAgeRange'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
        '#theme' => 'archibald_education_typical_age_range',
      );
      $options = range(0, 99);
      $options = array_combine($options, $options);
      $options[0] = t('Unknown');
      $form['education']['typicalAgeRange']['from'] = array(
        '#type' => 'select',
        '#title' => t('from'),
        '#default_value' => $default_value_from,
        '#options' => $options,
      );
      $form['education']['typicalAgeRange']['to'] = array(
        '#type' => 'select',
        '#title' => t('to'),
        '#default_value' => $default_value_to,
        '#options' => $options,
      );
    }

    // typicalLearningTime
    if (archibald_check_field_permission('education typicalLearningTime')) {
      $default_value = archibald_lom_vocab_default_val(
        $lom_education->getTypicalLearningTime()
      );
      $form['education']['typicalLearningTime'] = array(
        '#type' => 'select',
        '#title' => archibald_get_field_label('education.typicalLearningTime') .
          archibald_get_require_type_value('education.typicalLearningTime') .
          archibald_get_field_help('education.typicalLearningTime'),
        '#default_value' => $default_value,
        '#options' => archibald_get_education_typical_learning_time_options(),
      );
    }

    // Difficulty
    if (archibald_check_field_permission('education difficulty')) {
      $default_value = archibald_lom_vocab_default_val(
        $lom_education->getDifficult()
      );
      $form['education']['difficulty'] = array(
        '#type' => 'select',
        '#title' => archibald_get_field_label('education.difficulty') .
          archibald_get_require_type_value('education.difficulty') .
          archibald_get_field_help('education.difficulty'),
        '#default_value' => $default_value,
        '#attributes' => array('archibald_help_ident' => 'education.difficulty'),
        '#options' => array_merge(array('' => t('Unknown')), archibald_get_education_difficulty_options()),
      );
    }

    // Description
    if (archibald_check_field_permission('education description')) {
      $default_value = '';
      $lang_string = $lom_education->getDescription();
      if ($lang_string instanceof ArchibaldLomDataLangString) {
        $default_value = $lang_string->getString($language);
      }
      $form['education']['description'] = array(
        '#type' => 'textarea',
        '#title' => archibald_get_field_label('education.description') .
          archibald_get_require_type_value('education.description') .
          archibald_get_field_help('education.description'),
        '#default_value' => $default_value,
        '#rows' => 5,
        '#attributes' => array(
          'class' => array(
            'lang_string_autocomplete',
            'lang_string',
          ),
          'archibald_help_ident' => 'education.description',
        ),
      );
    }

    if (archibald_check_field_permission('technical duration')) {
      $duration_time = $lom_technical->duration->getDuration(TRUE);
      $form['education']['duration'] = array(
        '#type' => 'markup',
        '#title' => archibald_get_field_label('technical.duration') .
          archibald_get_require_type_value('technical.duration') .
          archibald_get_field_help('technical.duration'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
        '#tree' => TRUE,
        '#theme' => 'archibald_technical_duration',
      );

      $options = range(0, 23);
      array_walk($options, 'archibald_content_form_technical_zero_pad');
      $def = (isset($duration_time['hours']) ? $duration_time['hours'] : 0);
      $form['education']['duration']['hours'] = array(
        '#type' => 'select',
        '#default_value' => $def,
        '#options' => $options,
      );

      $options = range(0, 59);
      array_walk($options, 'archibald_content_form_technical_zero_pad');
      $def = (isset($duration_time['minutes']) ? $duration_time['minutes'] : 0);
      $form['education']['duration']['minutes'] = array(
        '#type' => 'select',
        '#default_value' => $def,
        '#options' => $options,
      );

      $options = range(0, 59);
      array_walk($options, 'archibald_content_form_technical_zero_pad');
      $def = (isset($duration_time['seconds']) ? $duration_time['seconds'] : 0);
      $form['education']['duration']['seconds'] = array(
        '#type' => 'select',
        '#default_value' => $def,
        '#options' => $options,
      );
    }
  }
}

/**
 * generate options list for drupal form api select
 *
 * @param string $default_value
 *
 * @return array
 */
function archibald_get_learning_resource_type_options(&$default_value) {
  $options = array();
  $options['pedagogical'] = archibald_get_taxonomy_options(
    'learning_resourcetype', 'pedagogical'
  );

  $options['documentary'] = archibald_get_taxonomy_options(
    'learning_resourcetype', 'documentary'
  );

  // extract vocabulars
  $default_value_assoc = array();
  foreach ($default_value as $value) {
    $k = archibald_select_options_default_value_filter_splitter($value);
    $default_value_assoc[$k] = $value;
  }

  // extract vocabulars
  $options_assoc = array();
  foreach ($options as $type => $values) {
    foreach ($values as $key => $value) {
      $k = archibald_select_options_default_value_filter_splitter($key);
      $options_assoc[$k] = $key;
    }
  }

  // echo "<pre>";
  // print_r($default_value);
  // print_r($default_value_assoc);
  // print_r($options_assoc);

  // find default_values within current existing options
  $default_value_new = array();
  foreach ($default_value_assoc as $value => $vocabular) {
    if (!empty($options_assoc[$value])) {
      $default_value_new[] = $options_assoc[$value];
      unset($default_value_assoc[$value]);
    }
  }
  // print_r($default_value_new);
  // die();

  // add non-existing default_values to options lists
  if (!empty($default_value_assoc)) {
    foreach ($default_value_assoc as $value => $vocabular) {
      $options['pedagogical'][$vocabular] = t($value);
      $default_value_new[] = $vocabular;
    }
  }
  $default_value = $default_value_new;

  return $options;
}

/**
 * get option array
 * for a checkboxes / select form item
 * with all possible user_role taxonomy terms
 *
 * @param string $default_value
 *
 * @return array
 */
function archibald_get_intended_end_user_role_options($default_value) {
  $options = archibald_get_taxonomy_options('intended_enduserrole');
  archibald_select_options_default_value_filter($options, $default_value);

  return $options;
}

/**
 * get option array
 * for a checkboxes / select form item
 * with all possible difficulty taxonomy terms
 *
 * @return array
 */
function archibald_get_education_difficulty_options() {
  return archibald_get_taxonomy_options('difficulty');
}

/**
 * get option array
 * for a checkboxes / select form item
 * with all possible learning_time taxonomy terms
 *
 * @return array
 */
function archibald_get_education_typical_learning_time_options() {
  $ret = archibald_get_taxonomy_options('typical_learning_time');
  foreach ($ret as $key => $value) {
    $testk = explode('|', $key);
    $testk = array_pop($testk);
    if ($testk == 'unknown') {
      unset( $ret[$key] );
      array_unshift($ret, $value);
    }
  }
  return $ret;
}

/**
 * theme a archibald form element typical age range
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_education_typical_age_range($form) {
  $form = reset($form);

  $output  = '<label for="edit-education_typical_age_range" archibald_help_ident="education.typical_age_range" archibald_help_selector="for|edit-education_typical_age_range">';
  $output .= $form['#title'];
  $output .= '</label>';
  $output .= '<div class="archibald_education_typical_age_range">';
  $output .= theme($form['from']['#type'], $form['from']) . ' - ' . theme($form['to']['#type'], $form['to']);
  $output .= '</div>';

  return $output;
}

/**
 * theme a archibald form block education
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_education($form) {
  $form = reset($form);

  // echo '<PRE>';
  // echo htmlspecialchars(drupal_render($form['learningResourceType_pedagogical']));
  // die();

  $html = array(
    'top' => array(),
    'left' => array(),
    'right' => array(),
    'bottom' => array(),
  );
  foreach (element_children($form, TRUE) as $key) {
    // echo '<PRE>';
    // print_r( $key );
    // echo "\n";
    switch ($key) {
      case 'learningResourceType_pedagogical':
      case 'learningResourceType_documentary':
        if ($key == 'learningResourceType_pedagogical') {
          $lang_resource_type = archibald_get_term_by_key('pedagogical', 'learning_resourcetype') . archibald_get_require_type_value('education.learningResourceTypePedagogical');
        }
        else {
          $lang_resource_type = archibald_get_term_by_key('documentary', 'learning_resourcetype') . archibald_get_require_type_value('education.learningResourceTypeDocumentary');
        }
        $html['left'][] = '<div class="' . $key . '"><strong>' . $lang_resource_type . '</strong>' . drupal_render($form[$key]) . '</div>';
        break;

      case 'description':
        $html['top'][] = drupal_render($form[$key]);
        break;

      default:
        $html['right'][] = drupal_render($form[$key]);
        break;
    }
  }

  $output = '<label for="edit-education-title" class="htab_title">' . $form['#title'] . '</label>';
  $output .= '<table id="archibald_education" border="0">';

  $top = implode("\n", $html['top']);
  if (!empty($top)) {
    $output .= '<tr>';
    $output .= '<td class="archibald_education_td archibald_education_td_top" colspan=2>';
    $output .= $top;
    $output .= '</td>';
    $output .= '</tr>';
  }

  $left = implode("\n", $html['left']);
  $right = implode("\n", $html['right']);

  if (!empty($left) || !empty($right)) {
    $output .= '<tr>';
    if (!empty($left)) {
      $output .= '<td class="archibald_education_td archibald_education_td_left">';

      $output .= ' <label for="edit-education-learningresourcetype-pedagogical" style="display: block;">' .
        t('Learning resource type') .
        archibald_get_field_help('education.learningResourceTypeDocumentary');

      $output .= '</label>';

      $output .= $left;
      $output .= '<div class="clearfix"></div>';
      $output .= '</td>';
    }
    if (!empty($right)) {
      $output .= '<td class="archibald_education_td archibald_education_td_right">';
      $output .= $right;
      $output .= '</td>';
      $output .= '</tr>';
    }
  }
  $output .= '</table>';

  return $output;
}

/**
 * theme Archibald form element duration
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_technical_duration($form) {
  $form = reset($form);

  $output  = '<label for="edit-technical_duration" archibald_help_ident="education.typical_age_range" archibald_help_selector="for|edit-technical_duration">';
  $output .= $form['#title'];
  $output .= '</label>';
  $output .= '<div class="archibald_technical_duration">';
  $output .= '<div>';
  $output .= '<span class="legend">HH</span>';
  $output .= '<span class="legend">MM</span>';
  $output .= '<span class="legend">SS</span>';
  $output .= '</div>';
  $output .= '<div><span>' . theme($form['hours']['#type'], $form['hours']) . '<span class="sep">:</span></span><span>' . theme($form['hours']['#type'], $form['minutes']) . '<span class="sep">:</span></span><span>' . theme($form['hours']['#type'], $form['seconds']) . '</span></div>';
  $output .= '</div>';

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
 * save handler for lom education form part
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 */
function archibald_content_form_education_submit($form, &$form_state, &$lom) {
  $lom_education = $lom->getEducation();
  if (!($lom_education instanceof ArchibaldLomDataEducation)) {
    $lom_education = new ArchibaldLomDataEducation();
  }

  $lom_general = $lom->getGeneral();
  if (!($lom_general instanceof ArchibaldLomDataGeneral)) {
    $lom_general = new ArchibaldLomDataGeneral();
  }


  ##### handle learningResourceType
  /*
    .##.......########..########
    .##.......##.....##....##...
    .##.......##.....##....##...
    .##.......########.....##...
    .##.......##...##......##...
    .##.......##....##.....##...
    .########.##.....##....##...
  */

  if (archibald_check_field_permission('education learningResourceTypeDocumentary') || archibald_check_field_permission('education learningResourceTypePedagogical')) {
    $opts = array();

    if( archibald_check_field_permission('education learningResourceTypeDocumentary') ) $opts[] = 'documentary';
    if( archibald_check_field_permission('education learningResourceTypePedagogical') ) $opts[] = 'pedagogical';

    foreach ($opts as $key => $type) {
      $learning_resource_types = array();

      $selected_options = $form_state['values']['education']['learningResourceType_' . $type];

      foreach ($selected_options as $i => $learning_resource_type) {
        if (empty($learning_resource_type)) {
          continue;
        }

        $key = drupal_strtolower(trim($learning_resource_type));
        if (strpos($key, '|') !== FALSE) {
          list($tmp_source, $key) = explode('|', $key, 2);
        }

        if (empty($key) || !preg_match('/(.+)\|(.+)/', $learning_resource_type, $parts)) {
          continue;
        }

        $vocabulary = new ArchibaldLomDataVocabulary();
        $vocabulary->setSource($parts[1]);
        $vocabulary->setValue($parts[2]);
        $learning_resource_types[] = $vocabulary;
      }
      $lom_education->setLearningResourceType($type, $learning_resource_types);
    }
  }

  ##### handle intendedEndUserRole
  if (archibald_check_field_permission('education intendedEndUserRole')) {
    $existing_intended_end_user_roles = array();
    foreach ($lom_education->getIntendedEndUserRole() as $intended_end_user_role) {
      if ($intended_end_user_role instanceof ArchibaldLomDataVocabulary) {
        $key = $intended_end_user_role->getSource() . '|' . $intended_end_user_role->getValue();
      }
      else {
        // this shoud really never happen
        continue;
      }

      $existing_intended_end_user_roles[$key] = $intended_end_user_role;
    }

    foreach ($form_state['values']['education']['intendedEndUserRole'] as $i => $intended_end_user_role) {
      $key = drupal_strtolower(trim($intended_end_user_role));

      if (empty($key) || !preg_match('/(.+)\|(.+)/', $intended_end_user_role, $parts)) {
        continue;
      }

      if (isset($existing_intended_end_user_roles[$key])) {
        unset($existing_intended_end_user_roles[$key]);
      }
      else {
        $vocabulary = new ArchibaldLomDataVocabulary();
        $vocabulary->setSource($parts[1]);
        $vocabulary->setValue($parts[2]);
        $lom_education->addIntendedEndUserRole($vocabulary);
      }
    }

    foreach ($existing_intended_end_user_roles as $key => $intended_end_user_role) {
      $lom_education->removeIntendedEndUserRole($intended_end_user_role);
    }
  }

  ##### handle typicalAgeRange
  if (archibald_check_field_permission('education typicalAgeRange')) {
    $string = $form_state['values']['education']['typicalAgeRange']['from'] . '-' . $form_state['values']['education']['typicalAgeRange']['to'];
    $lang_string = new ArchibaldLomDataLangString();
    $lang_string->setString($string, 'en');
    $lom_education->setTypicalAgeRange($lang_string, 0);
  }

  ##### handle typicalLearningTime
  if (archibald_check_field_permission('education typicalLearningTime')) {
    if (empty($form_state['values']['education']['typicalLearningTime'])) {
      $lom_education->setTypicalLearningTime(new ArchibaldLomDataVocabulary());
    } elseif (preg_match('/(.+)\|(.+)/', $form_state['values']['education']['typicalLearningTime'], $parts)) {
      $vocabulary = new ArchibaldLomDataVocabulary();
      $vocabulary->setSource($parts[1]);
      $vocabulary->setValue($parts[2]);
      $lom_education->setTypicalLearningTime($vocabulary);
    } else {
      $lom_education->setTypicalLearningTime(new ArchibaldLomDataVocabulary());
    }
  }

  ##### handle description
  if (archibald_check_field_permission('education description')) {
    $lom_education->setDescription(
      archibald_langstring_submit($lom_education->getDescription(), $form_state['values']['language'], $form_state['values']['education']['description'], FALSE)
    );
  }

  ##### handle difficulty
  if (archibald_check_field_permission('education difficulty')) {
    if (isset($form_state['values']['education']['difficulty'])) {
      if (empty($form_state['values']['education']['difficulty'])) {
        $lom_education->setDifficult(new ArchibaldLomDataVocabulary());
      }
      elseif (preg_match('/(.+)\|(.+)/', $form_state['values']['education']['difficulty'], $parts)) {
        $vocabulary = new ArchibaldLomDataVocabulary();
        $vocabulary->setSource($parts[1]);
        $vocabulary->setValue($parts[2]);
        $lom_education->setDifficult($vocabulary);
      }
    }
  }

  if (archibald_check_field_permission('technical duration')) {
    $lom_technical = $lom->getTechnical();
    if (!($lom_technical instanceof ArchibaldLomDataTechnical)) {
      $lom_technical = new ArchibaldLomDataTechnical();
    }

    ##### handle Duration
    $v = $form_state['values']['education']['duration'];
    $duration_time = '';
    if( $v['hours'] ) $duration_time .= $v['hours'] . 'H';
    if( $v['minutes'] ) $duration_time .= $v['minutes'] . 'M';
    if( $v['seconds'] ) $duration_time .= $v['seconds'] . 'S';
    if( strlen($duration_time) ) $duration_time = "PT" . $duration_time;
    $lom_technical->duration->duration = $duration_time;
    $lom->setTechnical($lom_technical);
  }

  $lom->setGeneral($lom_general);
  $lom->setEducation($lom_education);
}

