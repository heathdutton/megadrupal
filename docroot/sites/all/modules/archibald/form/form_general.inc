<?php

/**
 * @file
 * lom resource add/edit form part: for general tab
 */

/**
 * part of lom resource add/edit form
 *  tab: general
 *
 * @param array $form
 * @param array $form_state
 * @param ArchibaldLom $lom
 * @param string $res_language
 */
function archibald_content_form_general(&$form, &$form_state, &$lom, $res_language) {
  global $language;

  $lom_general = $lom->getGeneral();
  if (!($lom_general instanceof ArchibaldLomDataGeneral)) {
    $lom_general = new ArchibaldLomDataGeneral();
  }

  $lom_life_cycle = $lom->getLifeCycle();
  if (!($lom_life_cycle instanceof ArchibaldLomDataLifeCycle)) {
    $lom_life_cycle = new ArchibaldLomDataLifeCycle();
  }

  $lom_technical = $lom->getTechnical();
  if (!($lom_technical instanceof ArchibaldLomDataTechnical)) {
    $lom_technical = new ArchibaldLomDataTechnical();
  }

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General information'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#group' => 'vt_main',
    '#tree' => TRUE,
    '#theme' => 'archibald_general',
  );

  $form_is_proposal = archibald_form_is_proposal(empty($form_state['values']['lom_id']));

  // Title
  $default_value = '';
  $lang_string = $lom_general->getTitle();
  if ($lang_string instanceof ArchibaldLomDataLangString) {
    $default_value = $lang_string->getString($res_language);
  }
  $title = archibald_get_field_label('general.title');
  $title .= archibald_get_require_type_value('general.title') . archibald_get_field_help('general.title');
  $form['general']['title'] = array(
    '#type' => 'textfield',
    '#title' => $title,
    '#default_value' => $default_value,
    '#maxlength' => 500,
    '#required' => FALSE,
    '#attributes' => array(
      'class' => array('lang_string_autocomplete', 'lang_string'),
      'archibald_help_ident' => 'general.title',
    ),
  );

  if (archibald_is_proposal($lom)) {
    if ($lom->getLomId() != '') {
      $lom_stats = archibald_get_lom_stats($lom->getLomId(), TRUE);

      $form['general']['proposal_message'] = array(
        '#type' => 'item',
        '#title' => t('Message from the proposer'),
        '#default_value' => $default_value,
        '#markup' => $lom_stats->proposal_message,
      );

    }
    else {
      $form['general']['proposal_message'] = array(
        '#type' => 'textarea',
        '#title' =>  archibald_get_field_label('general.proposalMessage') . archibald_get_require_type_value('general.proposalMessage') . archibald_get_field_help('general.proposalMessage'),
        '#default_value' => $default_value,
        '#required' => FALSE,
      );
    }
  }

  if (empty($default_value) || $lom->getLomId() == '') {
    $type = 'value';
  }
  else {
    $type = 'checkbox';
  }

  if (archibald_check_field_permission('general description')) {
    // Description
    $default_value = '';
    $lang_string = $lom_general->getDescription();
    if ($lang_string instanceof ArchibaldLomDataLangString) {
      $default_value = $lang_string->getString($res_language);
    }

    $form['general']['description'] = array(
      '#type' => 'textarea',
      '#title' => archibald_get_field_label('general.description') .
        archibald_get_require_type_value('general.description') .
        archibald_get_field_help('general.description'),
      '#default_value' => $default_value,
      '#rows' => 5,
      '#attributes' => array(
        'class' => array(
          'lang_string_autocomplete',
          'lang_string',
        ),
        'archibald_help_ident' => 'general.description',
      ),
    );
  }

  if (empty($default_value) || $lom->getLomId() == '') {
    $type = 'value';
  }
  else {
    $type = 'checkbox';
  }

  /*
   * START Identifier handling
   */

  $form['general']['identifier'] = array(
    // '#type' => 'fieldset',
    '#id' => 'general-identifier-fieldset',
    '#attributes' => array('archibald_help_ident' => 'general.identifier'),
    // '#subtitle' => archibald_get_field_label('general.identifier'),
    '#theme' => 'archibald_general_identifier',
    '#prefix' => '<div class="clearfix" ' .
    'id="archibald_general_identifier-wrapper">',
    '#suffix' => '</div>',
    '#lom_id' => $lom->getLomId(),
  );

  /** Load identfiers from lom object if nothing was commited * */
  $num_general_identifier = 0;
  if (!isset($form_state['num_general_identifier'])) {
    $form_state['num_general_identifier'] = 0;
  }

  $identifiers = $lom_general->getIdentifier();
  if (!empty($identifiers) && empty($form_state['values']['general']['identifier']['add_more_general_identifier'])) {
    foreach ($identifiers as $identifier) {
      $form['general']['identifier']['identifier_' . $num_general_identifier] = array(
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#tree' => TRUE,
        '#weight' => $num_general_identifier,
      );


      $default_value = '';
      $lang_string = $identifier->getTitle();
      if ($lang_string instanceof ArchibaldLomDataLangString) {
        $default_value = $lang_string->getString($res_language);
      }
      $form['general']['identifier']['identifier_' . $num_general_identifier]['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#default_value' => $default_value,
        '#attributes' => array('class' => array('general_identifier_title')),
      );

      $form['general']['identifier']['identifier_' . $num_general_identifier]['catalog'] = array(
        '#type' => 'select',
        '#title' => t('Catalogue'),
        '#default_value' => archibald_lom_vocab_default_val($identifier->getCatalog()),
        '#options' => archibald_get_identifier_catalog_options(),
        '#attributes' => array('class' => array('general_identifier_catalog')),
      );

      $default_value = $identifier->getEntry();
      $form['general']['identifier']['identifier_' . $num_general_identifier]['entity'] = array(
        '#type' => 'textfield',
        '#title' => t('Entity'),
        '#default_value' => $default_value,
        '#maxlength' => 450,
        '#attributes' => array('class' => array('general_identifier_entity')),
      );

      $form_state['num_general_identifier']++;
      $num_general_identifier++;
    }
  }
  if (empty($form_state['num_general_identifier'])) {
    $form_state['num_general_identifier'] = 1;
  }

  for ($i = $num_general_identifier; $i < $form_state['num_general_identifier']; $i++) {
    archibald_content_form_general_add_empty_identifier($form, $form_state);
  }

  $form['general']['identifier']['add_more_general_identifier'] = array(
    '#type' => 'submit',
    '#value' => t('Add an identifier'),
    '#weight' => 9999,
    '#limit_validation_errors' => array(),
    // #submit is required to use #limit_validation_errors
    '#submit' => array(
      'archibald_content_form_add_more_general_identifier_submit',
    ),
    '#ajax' => array(
      'callback' => 'archibald_ajax_content_form_general_add_empty_identifier',
      'wrapper' => 'archibald_general_identifier-wrapper',
      'method' => 'replace',
    ),
  );


  // Language
  if (archibald_check_field_permission('general language')) {
    $default_values = $lom_general->getLanguage();

    // Custom language order in Archibald's form
    $l = archibald_get_general_language_options($default_values, $language->language );

    $cl = $cs = array();
    foreach ($l as $lk => $lv) {
      switch ( $lk ) {
        case 'none':
          $cl[$lk] = $lv;
          $cs[$lk] = 1;
          unset( $l[$lk] );
        break;
        case 'de':
          $cl[$lk] = $lv;
          $cs[$lk] = 2;
          unset( $l[$lk] );
        break;
        case 'fr':
          $cl[$lk] = $lv;
          $cs[$lk] = 3;
          unset( $l[$lk] );
        break;
        case 'it':
          $cl[$lk] = $lv;
          $cs[$lk] = 4;
          unset( $l[$lk] );
        break;
        case 'rm':
          $cl[$lk] = $lv;
          $cs[$lk] = 5;
          unset( $l[$lk] );
        break;
        case 'rm2907':
          $cl[$lk] = $lv;
          $cs[$lk] = 6;
          unset( $l[$lk] );
        break;
        case 'rm2908':
          $cl[$lk] = $lv;
          $cs[$lk] = 7;
          unset( $l[$lk] );
        break;
        case 'rm13312':
          $cl[$lk] = $lv;
          $cs[$lk] = 8;
          unset( $l[$lk] );
        break;
        case 'rm16068':
          $cl[$lk] = $lv;
          $cs[$lk] = 9;
          unset( $l[$lk] );
        break;
        case 'rm16069':
          $cl[$lk] = $lv;
          $cs[$lk] = 10;
          unset( $l[$lk] );
        break;
        case 'en':
          $cl[$lk] = $lv;
          $cs[$lk] = 11;
          unset( $l[$lk] );
        break;
      }
    }

    array_multisort($cs, $cl);
    asort($l);
    $langs_ordered = array_merge($cl, $l);

    $form['general']['language'] = array(
      '#type' => 'checkboxes',
      '#title' => archibald_get_field_label('general.language') .
        archibald_get_require_type_value('general.language') .
        archibald_get_field_help('general.language'),
      '#required' => FALSE,
      '#options' => $langs_ordered,
      '#default_value' => $default_values,
      '#attributes' => array(
        'class' => array(
          'general_language',
        ),
        'archibald_help_selector' => 'for|edit-general-language',
        'archibald_help_ident' => 'general.live_cycle.language',
      ),
    );
  }

  // Preview Image + Copyright Description
  if (archibald_check_field_permission('technical preview_image')) {
    $i = $lom_technical->getPreviewImage();
    $default_value = $i->getImage();
    preg_match('/archibald_file\/([0-9]+)\//', $default_value, $old_fid);
    $default_value = isset($old_fid[1]) ? $old_fid[1] : NULL;
    $form['general']['preview_image'] = array(
      '#type' => 'managed_file',
      '#title' => archibald_get_field_label('technical.previewImage') .
        archibald_get_require_type_value('technical.previewImage') .
        archibald_get_field_help('technical.previewImage'),
      '#default_value' => $default_value,
      '#upload_validators' => array(
        'file_validate_extensions' => array(
          'png jpg jpeg gif',
        ),
      ),
      '#upload_location' => 'public://files/preview_image',
      '#field_suffix' => '<div class="archibald_preview_image" ' .
      'style_name="archibald_preview">&nbsp;</div>',
    );

    if (archibald_check_field_permission('technical preview_image_copyright_description')) {
      $default_value = $i->getCopyrightDescription();
      $form['general']['previewImageCopyrightDescription'] = array(
        '#type' => 'textfield',
        '#title' => archibald_get_field_label('technical.previewImageCopyrightDescription') .
          archibald_get_require_type_value('technical.previewImageCopyrightDescription') .
          archibald_get_field_help('technical.previewImageCopyrightDescription'),
        '#default_value' => $default_value,
        '#maxlength' => 200,
        '#attributes' => array('class' => array('general_copyright_description'))
      );
    }
  }

  // Version
  if (archibald_check_field_permission('general life_cycle version')) {
    $default_value = '';
    $lang_string = $lom_life_cycle->getVersion();
    if ($lang_string instanceof ArchibaldLomDataLangString) {
      $default_value = $lang_string->getString($res_language);

      if (empty($default_value)) {
        $tmp = $lang_string->getStrings();
        $default_value = reset($tmp);
      }
    }

    $form['general']['life_cycle']['version'] = array(
      '#type' => 'textfield',
      '#title' => archibald_get_field_label('lifecycle.version') .
        archibald_get_require_type_value('lifecycle.version') .
        archibald_get_field_help('lifecycle.version'),
      '#default_value' => $default_value,
      '#attributes' => array(
        'class' => array(
          'life_cycle_version',
          'lang_string',
        ),
        'archibald_help_ident' => 'live_cycle.version',
      ),
      '#size' => 20,
    );
  }

  $options = array_merge(array('' => ''), archibald_get_taxonomy_options('aggregation_level'));
  // Aggregation level
  if (archibald_check_field_permission('education aggregationLevel')) {
    $default_value = $lom_general->getAggregationLevel();
    if ($default_value instanceof ArchibaldLomDataVocabulary) {
      $default_value = $default_value->getSource() . '|' . $default_value->getValue();
    }

    $form['general']['aggregationLevel'] = array(
      '#type' => 'select',
      '#title' => archibald_get_field_label('education.aggregationLevel') .
        archibald_get_require_type_value('education.aggregationLevel') .
        archibald_get_field_help('education.aggregationLevel'),
      '#default_value' => $default_value,
      '#attributes' => array(
        'archibald_help_ident' => 'general.aggregationLevel',
        'style' => 'width:250px;'
      ),
      '#options' => $options
    );
  }
}

function archibald_get_identifier_catalog_options() {
  return archibald_get_taxonomy_options('general_catalog');
}

/**
 * add new general identifier item,
 * this could called by default form or ajax callback
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_general_add_empty_identifier(&$form, &$form_state) {

  $i = 0;
  foreach (array_keys($form['general']['identifier']) as $k) {
    if (drupal_substr($k, 0, 11) == 'identifier_') {
      $i++;
    }
  }

  $form['general']['identifier']['identifier_' . $i] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#weight' => $i,
  );

  $form['general']['identifier']['identifier_' . $i]['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#attributes' => array('class' => array('general_identifier_title')),
  );

  $form['general']['identifier']['identifier_' . $i]['catalog'] = array(
    '#type' => 'select',
    '#title' => t('Catalogue'),
    '#default_value' => '',
    '#options' => archibald_get_identifier_catalog_options(),
    '#attributes' => array('class' => array('general_identifier_catalog')),
  );

  $form['general']['identifier']['identifier_' . $i]['entity'] = array(
    '#type' => 'textfield',
    '#title' => t('entity'),
    '#default_value' => '',
    '#maxlength' => 450,
    '#attributes' => array('class' => array('general_identifier_entity')),
  );

  // $extensions = variable_get('archibald_identifier_file_extensions', '');
  // if (empty($extensions)) {
  //   $extensions = ARCHIBALD_DEFAULT_IDENTIFIER_FILE_EXTENSIONS;
  // }
  // $form['general']['identifier']['identifier_' . $i]['fid'] = array(
  //   '#title' => t('Upload'),
  //   '#type' => 'managed_file',
  //   '#upload_location' => 'public://files/archibald/',
  //   '#default_value' => NULL,
  //   '#progress_indicator' => 'throbber',
  //   '#upload_validators' => array(
  //     'file_validate_extensions' => array(
  //       $extensions,
  //     ),
  //   ),
  // );
}

/**
 * add new general identifier item,
 * this could called by default form or ajax callback
 *
 * The real work will done by:
 * @see archibald_content_form_general_add_empty_identifier()
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function archibald_ajax_content_form_general_add_empty_identifier($form, $form_state) {
  return $form['general']['identifier'];
}

$_archibald_identifier_id_count = 0;

/**
 * theme a archibald form element general identifier
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_general_identifier($form) {
  global $_archibald_identifier_id_count, $user;
  $form = reset($form);

  if (!user_is_logged_in()) {
    $output .= '<div class="archibald_help">' . t('Please provide at least one valid URL (incl. http://...)') . '</div>';
  }

  // echo '<PRE>';
  // print_r( $form );
  // die();

  $output = '<label>';
  $output .= archibald_get_field_label('general.identifier') . archibald_get_require_type_value('general.identifier') . archibald_get_field_help('general.identifier');
  $output .= '</label><fieldset>';
  $output .= '<div id="archibald_general_identifier">';

  foreach ($form as $k => $element) {
    if (drupal_substr($k, 0, 11) == 'identifier_') {
      $title_of_title = $element['title']['#title'];
      unset($element['title']['#title']);
      unset($element['catalog']['#title']);
      unset($element['entity']['#title']);
      unset($element['fid']['#title']);

      // at some where here the MetaTMeta Date Object Key
      // to load it in modal frame
      $output .= '<table class="archibald_general_identifier_item">';
      $output .= '<tr class="tr_upper">';
      $output .= '<td align="right">' . $title_of_title . '</td><td align="left">';

      $output .= theme('textfield', $element['title']);
      $output .= '</td></tr>';
      $output .= '<tr class="tr_upper"><td align="right">';
      $output .= drupal_render($element['catalog']);
      $output .= '</td><td align="left">';
      $output .= drupal_render($element['entity']);
      $output .= '</td></tr>';
      if (!empty($element['fid']) && archibald_can_upload_general_identifer($form['#lom_id'])) {
        $output .= '<tr class="tr_lower"><td>&nbsp;</td><td align="left">';
        $output .= drupal_render($element['fid']);
        $output .= '</td></tr>';
      }
      $output .= '</table>';
    }
  }

  $output .= '</div></fieldset>';
  $output .= drupal_render($form['add_more_general_identifier']);

  return $output;
}

/**
 * Submit handler for the "Add more" button.
 *
 * @param array $form
 * @param array $form_state
 */
function archibald_content_form_add_more_general_identifier_submit($form, &$form_state) {
  $form_state['num_general_identifier']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * theme a archibald form block general
 *
 * @param array $form
 *
 * @return string
 */
function theme_archibald_general($form) {
  $form = reset($form);

  $output = '<label for="edit-general-title" class="htab_title">' . $form['#title'] . '</label>';
  $output .= '<table id="archibald_general" border="0">';

  $html = array(
    'bottom right' => array(),
    'bottom left' => array(),
    'bottom middle' => array(),
    'bottom bottom' => array(),
  );

  foreach (element_children($form, TRUE) as $key) {
    switch ($key) {
      case 'life_cycle':
        $html['bottom right'][] = drupal_render($form[$key]['version']);
        break;

      case 'technical':
        $html['bottom right'][] = drupal_render($form[$key]);
        break;

      case 'language':
        $html['bottom left'][] = drupal_render($form[$key]);
        break;

      case 'preview_image':
        $html['bottom middle'][] = drupal_render($form[$key]);
        break;

      case 'previewImageCopyrightDescription':
        $html['bottom middle'][] = drupal_render($form[$key]);
        break;
      case 'aggregationLevel':
        $html['bottom bottom'][] = drupal_render($form[$key]);
        break;

      default:
        $html['top'][] = drupal_render($form[$key]);
        break;
    }
  }

  $output .= '<tr>';
  $output .= '<td class="archibald_general_td ' . 'archibald_general_td_top" colspan=3>';
  $output .= implode("\n", $html['top']);
  $output .= '</td>';
  $output .= '</tr>';

  $output .= '<tr>';
  $output .= '<td class="archibald_general_td ' . 'archibald_general_td_bottom_left">';
  $output .= implode("\n", $html['bottom left']);
  $output .= '</td>';
  $output .= '<td class="archibald_general_td ' . 'archibald_general_td_bottom_middle">';
  $output .= implode("\n", $html['bottom middle']);
  $output .= '</td>';
  $output .= '<td class="archibald_general_td ' . 'archibald_general_td_bottom_right">';
  $output .= implode("\n", $html['bottom right']);
  $output .= '</td>';
  $output .= '</tr>';

  $output .= '<tr>';
  $output .= '<td class="archibald_general_td ' . 'archibald_general_td_bottom_bottom" colspan=3>';
  $output .= implode("\n", $html['bottom bottom']);
  $output .= '</td>';
  $output .= '</tr>';

  $output .= '</table>';

  return $output;
}

/**
 * handle file uploads for identifier
 *
 * @param array $form_state
 */
function archibald_general_handle_file_upload(&$form_state) {
  // When using the #managed_file form element the file is automatically
  // uploaded an saved to the {file} table. The value of the corresponding
  // form element is set to the {file}.fid of the new file.

  foreach ($form_state['values']['general']['identifier'] as $id => $identifier) {
    // If fid is not 0 we have a valid file.
    if ($identifier['fid'] != 0) {
      // The new file's status is set to 0 or temporary and in order to ensure
      // that the file is not removed after 6 hours
      // we need to change it's status
      //
      // to 1. Save the ID of the uploaded image for later use.
      $file = file_load($identifier['fid']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);

      // When a module is managing a file, it must manage the usage count.
      // Here we increment the usage count with file_usage_add().
      file_usage_add($file, 'archibald', 'identifier_file', 1);

      $identifier['catalog'] = 'URL';
      $identifier['entity'] = archibald_file_get_url($file);
    }
    // If the file was removed we need to remove the module's reference to the
    // removed file's fid, and remove the file.
    if ($identifier['fid'] == 0 &&  preg_match('/archibald_file\/([0-9]+)\//', $identifier['entity'], $old_fid)) {
      // Retrieve the old file's id.
      $fid = $old_fid[1];
      $file = $fid ? file_load($fid) : FALSE;
      if ($file) {
        // When a module is managing a file, it must manage the usage count.
        // Here we decrement the usage count with file_usage_delete().
        file_usage_delete($file, 'archibald', 'identifier_file', 1);

        // The file_delete() function takes a file object and checks to see if
        // the file is being used by any other modules. If it is the delete
        // operation is cancelled, otherwise the file is deleted.
        file_delete($file);
      }
    }

    $form_state['values']['general']['identifier'][$id] = $identifier;
  }
}

/**
 * Return or set the identifier cache.
 *
 * @staticvar array $cache
 *   The cached values
 *
 * @param array $value
 *   The array (optional, default = NULL)
 *
 * @return mixed
 *   if we are in return mode ($value = NULL) returns the posted identifiers
 *   as an array, else we return nothing
 */
function archibald_general_identifier_cache($value = NULL) {
  $cache = &archibald_static(__FUNCTION__, NULL);
  if ($value != NULL) {
    $cache = $value;
    return;
  }

  return $cache;
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
function archibald_content_form_general_submit($form, &$form_state, &$lom) {

  // Need this to use within usort anonymous function.
  archibald_general_identifier_cache($form_state['values']['general']['identifier']);

  $lom_general = $lom->getGeneral();

  if (!($lom_general instanceof ArchibaldLomDataGeneral)) {
    $lom_general = new ArchibaldLomDataGeneral();
  }

  $lom_life_cycle = $lom->getLifeCycle();
  if (!($lom_life_cycle instanceof ArchibaldLomDataLifeCycle)) {
    $lom_life_cycle = new ArchibaldLomDataLifeCycle();
  }

  $lom_technical = $lom->getTechnical();
  if (!($lom_technical instanceof ArchibaldLomDataTechnical)) {
    $lom_technical = new ArchibaldLomDataTechnical();
  }

  /**
   * @cha, 26.06.2012
   * Disabled spellcheck
   *
   * if (!isset($form_state['values']['general']['description_spell_check'])) {
   *   $form_state['values']['general']['description_spell_check'] = 0;
   * }
   *
   * $form_state['values']['general']['title_spell_check']
   * $form_state['values']['general']['description_spell_check']
   * $form_state['values']['general']['otherPlattformRequirements_spell_check']
   */

  ##### handle title
  $lom_general->setTitle(
    archibald_langstring_submit(
      $lom_general->getTitle(),
      $form_state['values']['language'],
      $form_state['values']['general']['title'],
      FALSE
    )
  );
  ##### handle description
  if (isset($form_state['values']['general']['description'])) {
    $lom_general->setDescription(
      archibald_langstring_submit(
        $lom_general->getDescription(),
        $form_state['values']['language'],
        $form_state['values']['general']['description'],
        FALSE
      )
    );
  }

  ##### handle identifiers
  $existing_identifiers = array();
  foreach ($lom_general->getIdentifier() as $identifier) {
    $key = $identifier->getCatalog() . '_____' . $identifier->getEntry();
    $existing_identifiers[$key] = $identifier;
  }
  $technical_autofilled = FALSE;
  foreach ($form_state['values']['general']['identifier'] as $i => $identifier) {

    if (is_array($identifier) && !empty($identifier['entity']) && !empty($identifier['catalog'])) {

      $key = $identifier['catalog'] . '_____' . $identifier['entity'];

      if (isset($existing_identifiers[$key])) {
        // updating title and mark as exisiting with unset();
        $title = NULL;
        if (!empty($identifier['title'])) {
          $title = archibald_langstring_submit(
            $existing_identifiers[$key]->getTitle(),
            $form_state['values']['language'], $identifier['title']
          );
        }

        $existing_identifiers[$key]->setTitle($title);

        $lom_general->setIdentifier(
          $existing_identifiers[$key],
          $existing_identifiers[$key]->getId()
        );

        unset($existing_identifiers[$key]);
      }
      else {
        $title = NULL;
        if (!empty($identifier['title'])) {
          $title = archibald_langstring_submit(
            '',
            $form_state['values']['language'],
            $identifier['title']
          );
        }

        $lom_general->addIdentifier(
          new ArchibaldLomDataIdentifier(
            $identifier['catalog'],
            $identifier['entity'],
            $title
          )
        );
      }

      // if ($technical_autofilled == FALSE && $identifier['fid'] != 0) {
      //   $file = file_load($identifier['fid']);
      //   $lom_technical->addFormat($file->filemime);
      //   $lom_technical->setSize($file->filesize);
      //   $technical_autofilled = TRUE;
      // }
    }
  }

  foreach ($existing_identifiers as $key => $identifier) {
    $lom_general->removeIdentifier($identifier);
  }

  //Sort our identifier
  uasort($lom_general->identifier, function($a, $b) {

    // Get our stored identifiers.
    $identifiers = archibald_general_identifier_cache();

    // Build up our check keys.
    $a_key = $a->catalog . '_____' . $a->entry;
    $b_key = $b->catalog . '_____' . $b->entry;

    // Index check.
    $a_index = 0;
    $b_index = 0;

    // Loop through all identifiers and determine the original index for the
    // provided identifiers.
    foreach ($identifiers AS $i => $vals) {

      // Get the original key check.
      $key = $vals['catalog'] . '_____' . $vals['entity'];

      if ($key == $a_key) {
        $a_index = $i;
      }

      if ($key == $b_key) {
        $b_index = $i;
      }

    }

    // Get the sort index.
    $a_index = (INT) str_replace("identifier_", "", $a_index);
    $b_index = (INT) str_replace("identifier_", "", $b_index);

    // Return the sort values.
    if ($a_index < $b_index) {
      return -1;
    }
    elseif ($a_index > $b_index) {
      return 1;
    }
    return 0;
  });

  ##### handle lifeCycle version
  if (archibald_check_field_permission('general life_cycle version')) {
    $lang_string = new ArchibaldLomDataLangString();
    $lang_string->setString($form_state['values']['general']['life_cycle']['version'], $form_state['values']['language']);
    $lom_life_cycle->setVersion($lang_string);
  }

  ##### handle general language
  if (archibald_check_field_permission('general language')) {
    if (!empty($form_state['values']['general']['language'])) {
      $langs = array();
      foreach ($form_state['values']['general']['language'] as $lang) {
        if (!empty($lang)) {
          $langs[] = $lang;
        }
      }
      $lom_general->setLanguage($langs);
    }
  }

  ##### handle technical location
  if (archibald_check_field_permission('technical location') && isset($form_state['values']['general']['technical'])) {
    $lom_technical->setLocation($form_state['values']['general']['technical']['location'], 0);
  }

  ##### handle technical preview_image
  if (archibald_check_field_permission('technical preview_image')) {
    $preview_image = new ArchibaldLomDataPreviewImage();
    if (!empty($form_state['values']['general']['preview_image'])) {
      // The new file's status is set to 0 or temporary and in order to ensure
      // that the file is not removed after 6 hours we need to change it's status
      // to 1. Save the ID of the uploaded image for later use.
      $file = file_load($form_state['values']['general']['preview_image']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);

      // When a module is managing a file, it must manage the usage count.
      // Here we increment the usage count with file_usage_add().
      file_usage_add($file, 'archibald', 'preview_image', 1);

      $preview_image->setImage(archibald_file_get_url($file));

      if (archibald_check_field_permission('technical preview_image_copyright_description') && !empty($form_state['values']['general']['previewImageCopyrightDescription'])) {
        $preview_image->setCopyrightDescription($form_state['values']['general']['previewImageCopyrightDescription']);
      }
    }


    $lom_technical->setPreviewImage($preview_image);
  }

  ##### handle Aggregation level
  if (archibald_check_field_permission('education aggregationLevel')) {
    if (isset($form_state['values']['general']['aggregationLevel'])) {
      if (empty($form_state['values']['general']['aggregationLevel'])) {
        $lom_general->setAggregationLevel(new ArchibaldLomDataVocabulary());
      }
      else {
        $vocabulary = new ArchibaldLomDataVocabulary();
        if (!preg_match('/(.+)\|(.+)/', $form_state['values']['general']['aggregationLevel'], $parts)) {
          continue;
        }
        $vocabulary->setSource($parts[1]);
        $vocabulary->setValue($parts[2]);
        $lom_general->setAggregationLevel($vocabulary);
      }
    }
  }

  // save subobject back
  $lom->setTechnical($lom_technical);
  $lom->setGeneral($lom_general);
  $lom->setLifeCycle($lom_life_cycle);
}