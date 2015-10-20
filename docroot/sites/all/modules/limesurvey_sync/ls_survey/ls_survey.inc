<?php
/**
 * @file
 * functions for the ls_survey module
 */

/**
 * Provide default values for new answers content type.
 *
 * This function provide default values for new answers content type, $type
 * value is provide via the associated survey node, name and description are
 * provided from the LimeSurvey website.
 *
 * @param $node
 *   A object. The survey node associated to the new answers content type
 *   (optional). default is NULL.
 *
 * @return
 *   An object corresponding to the new answers content type properties.
 */
function ls_survey_default_node_type_answ($node = NULL) {
  // The answer node type is new : load default values from the LimeSurvey
  // website database.
  // Load the Survey properties to extract Name and Description first.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = (!empty($node->ls_sid)) ? limesurvey_sync_survey_properties($node->ls_sid) : array();
  $nid = (isset($node->nid)) ? $node->nid : '';
  $info = array(
    'type' => (!empty($node->ls_orig_type)) ? $node->ls_orig_type : '',
    'name' => ls_survey_type_name_unique((!empty($survey_properties)) ? $survey_properties['#lang'][$node->ls_lang]['title'] : t('Name for the respond content type #@nid', array('@nid' => $nid))),
    'base' => 'ls_answ',
    'module' => 'ls_answ',
    'description' => ((!empty($survey_properties)) ? $survey_properties['#lang'][$node->ls_lang]['description'] : ''),
    'custom' => FALSE, // Disallow to delete the node type.
    'locked' => FALSE,
    'title_label' => t('Respond title'),
    // The content type is create outside the module, don't save it twice.
    'is_new' => 0,
  );
  // Include content_types.inc file for the _node_type_set_defaults() function
  module_load_include('inc', 'node', 'content_types');
  $ls_answ_type_node_info = (object) node_type_set_defaults($info);

  return $ls_answ_type_node_info;
}

/**
 * Check the answers content type name and edit it if needed.
 *
 * This function makes sure the associated answers content default name is
 * unique. In the LimeSurvey Website, name field is unique, but conflicts may
 * occure for non ls_answ module content types. If the name already exists,
 * concatenate a random number to make it unique. The user will edit it on
 * the next page.
 *
 * @param $name
 *   A string. the default survey name on the LimeSurvey website.
 *
 * @return
 *   An string corresponding to a unique name for the answers content type.
 */
function ls_survey_type_name_unique($name) {
  $all_names = node_type_get_names();
  $unique_name = $name;
  $i = 0;
  while (in_array($unique_name, $all_names)) {
    $unique_name = $name . ' (' . mt_rand(0, 1000) . ')';
    // Avoid infinite loop.
    $i++;
    if ($i > 1000) {
      break;
    }
  }
  return $unique_name;
}

/**
 * Provide the answers content type object associated the orig_type value.
 *
 * Usefull for loading the associated answers content type of a specific node
 * survey, or checking if the associated answers content type exists. Note that
 * the ls_orig_type value may be different of the type value of the associated
 * answsers content type. That why this function parses all types, checking
 * for the matching orig_type value.
 *
 * @param $ls_orig_type
 *   A string. The orig_type value of an associated answsers content type. It
 *   corresponds to the ls_orig_type value of the associated survey node.
 *
 * @return
 *   The object type properties of the associated answers content type or
 *   FALSE if there is no associated answers content type.
 */
function ls_survey_find_answ_type($ls_orig_type) {
  if (empty($ls_orig_type)) {
    return FALSE;
  }
  module_load_include('inc', 'node', 'content_types');
  $all_types = node_type_get_types();
  foreach ($all_types as $type_name => $info) {
    if ($info->module == 'ls_answ' && $info->orig_type == $ls_orig_type && !$info->disabled) {
      return $info;
    }
  }
  return FALSE;
}

/**
 * Return the survey node associated to the content type value provided.
 *
 * The survey node associated to the answers content type provides default
 * values for new answer nodes.
 *
 * @param $ls_orig_type
 *   A string. The orig_type value of an associated answsers content type. It
 *   corresponds to the ls_orig_type value of the associated survey node.
 * @param $reset
 *   A boolean. to reset the cache. Default is FALSE.
 *
 * @return
 *   An object. The survey node associated to this answers content type or
 *   FALSE if there is no associated survey.
 */
function ls_survey_find_survey($ls_orig_type, $reset = FALSE) {
  static $surveys = array();

  if (empty($surveys) || $reset) {
    if ($reset) {
      $surveys = array();
    }

    // Build a cache, storing all survey nodes keyed by ls_orig_type.
    $survey_query = db_query('SELECT n.nid AS node_nid, ls_orig_type
    FROM {ls_survey} lss INNER JOIN {node} n ON n.nid = lss.nid AND n.vid = lss.vid
    ORDER BY ls_orig_type');
    while ($survey_row = $survey_query->fetchAssoc()) {
      $nid = $survey_row['node_nid'];
      $orig_type = $survey_row['ls_orig_type'];
      if (!empty($nid) && !empty($ls_orig_type)) {
        $surveys[$orig_type] = $nid;
      }
    }
  }

  // Deal with request:
  if (empty($ls_orig_type)) {
    return FALSE;
  }
  elseif (isset($surveys[$ls_orig_type])) {
    if (!function_exists('ls_survey_load')) {
      module_load_include('module', 'ls_survey');
    }
    return node_load($surveys[$ls_orig_type]);
  }
  else {
    return FALSE;
  }
}

/**
 * Validate the ls_orig_type value when editing a node survey.
 *
 * This function is an adapted version of the node_type_form_validate()
 * function, including the fix for unique orig_type.
 *
 * @param $node
 *   The survey node as an object, from the node edit form.
 * @param $form
 *   An array. The survey node edit form.
 */
function ls_survey_ls_orig_type_validate($node, $form) {

  if (!isset($node->ls_orig_type)) {
    form_set_error('ls_anw_type', t('The respond content type "type" value is not valid.'));
    return;
  }

  $ls_orig_type = trim($node->ls_orig_type);

  // Check if the answ type is available from de {ls_survey} table (usefull
  // for survey nodes with no associated answers content type).
  if (db_query("SELECT 1 FROM {ls_survey} lss JOIN {node} n ON n.nid = lss.nid AND n.vid = lss.vid WHERE ls_orig_type = :ls_orig_type AND n.nid <> :nnid", array(':ls_orig_type' => $ls_orig_type, ':nnid' => $node->nid))->fetchField()) {
    form_set_error('ls_orig_type', t('The machine-readable name %type is already taken.', array('%type' => $ls_orig_type)));
  }

  $previous_node = $form['#node'];
  if ($ls_orig_type != $previous_node->ls_orig_type) {
    // If the ls_orig_type has changed : check the new value.

    // Copy part of code from the node_type_form_validate() core function,
    // changing the target in the form_set_error() function.
    $types = node_type_get_names();
    if (isset($types[$ls_orig_type])) {
      form_set_error('ls_orig_type', t('The machine-readable name %type is already taken.', array('%type' => $ls_orig_type)));
    }
    if (!preg_match('!^[a-z0-9_]+$!', $ls_orig_type)) {
      form_set_error('ls_orig_type', t('The machine-readable name of this content type must contain only lowercase letters, numbers, and underscores.'));
    }
    $invalid_type = FALSE;
    $all_types = node_type_get_types();
    foreach ($all_types as $each_type) {
      if ($ls_orig_type == $each_type->orig_type) {
        $invalid_type = TRUE;
        break;
      }
    }
    // 'theme' conflicts with theme_node_form().
    // '0' is invalid, since elsewhere we check it using empty().
    if (in_array($ls_orig_type, array('0', 'theme')) || $invalid_type) {
      form_set_error('ls_orig_type', t("Invalid machine-readable name. Please enter a name other than %invalid.", array('%invalid' => $ls_orig_type)));
    }
  }
}

/**
 * Validate the ls_sid and ls_lang values when editing a node survey.
 *
 * This function makes sure the survey ID reference and langage are valid.
 * However, display warning message instead of error message if
 * synchronization fails, for archive raisons.
 *
 * @param $node
 *   The survey node as an object, from the node edit form.
 * @param $form
 *   An array. The survey node edit form.
 */
function ls_survey_ls_sid_ls_lang_validate($node, $form) {

  $previous_node = $form['#node'];

  $sid = $node->ls_sid;
  $new_node = (!isset($previous_node->nid) || !$previous_node->nid);

  // Make sure the ls_module is enable.
  if (!module_exists('ls_answ')) {
    $message = t("The '%module' module needs to be installed", array('%module' => 'LimeSurvey Sync Responses'));
    ls_survey_error_or_message($new_node, '', $message);
  }

  if (empty($sid) || ! is_numeric($sid)) {
    form_set_error('ls_sid', t('The Survey ID is not valid.'));
  }
  else {
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    $this_survey = limesurvey_sync_survey_properties($sid);
    if (empty($this_survey)) {
      $message = t('This survey ID does not exist on your LimeSurvey site.');
      ls_survey_error_or_message($new_node, 'ls_sid', $message);
    }
    else {
      // Load ls_lang and test it.
      $ls_lang = $node->ls_lang;
      if (empty($ls_lang) || ! is_string($ls_lang)) {
        form_set_error('ls_lang', t('The survey language is not valid.'));
      }
      else {
        if (!isset($this_survey['#lang'][$ls_lang])) {
          $message = t('This survey language does not exist on your LimeSurvey site.');
          ls_survey_error_or_message($new_node, 'ls_lang', $message);
        }
        else {
          // Load file for the limesurvey_sync_link_to_ls() function.
          module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
          $nid = (!empty($node->nid)) ? $node->nid : 0;
          // A survey node should be a unique sid + lang.
          // Check if this sid with this associated lang does already exist.
          $query_sid_doubloon = db_query("SELECT n.nid
          FROM {ls_survey} lss
          JOIN {node} n ON n.vid = lss.vid AND n.nid = lss.nid
          WHERE ls_sid = :ls_sid AND ls_lang = :ls_lang AND n.nid <> :nnid
          ", array(':ls_sid' => $sid, ':ls_lang' => $ls_lang, ':nnid' => $nid));
          $row_sid_doubloon = $query_sid_doubloon->fetchAssoc();
          if ($query_sid_doubloon->rowCount() >= 1) {
            // If we create a new survey node, prevent for doobloon.
            $node_doubloon = node_load($row_sid_doubloon['nid']);
            $doubloon_link = (node_access('view', $node_doubloon)) ? t('the !node survey', array('!node' => l($node_doubloon->title, 'node/' . $node_doubloon->nid))) : t('an unauthorized access survey');
            form_set_error('ls_sid', t('This survey ID, associated to this language, is already taken by !survey. An alternative way is !copy from the LimeSurvey user interface.',
            array('!survey' => $doubloon_link, '!copy' => limesurvey_sync_link_to_ls(t('to copy this survey'), NULL, 'newsurvey', 'copy'))));
          }
          else {
            // Test for compatibility, active and tokens table.
            // Call error for new node and display only message for existing
            // node, it will allow node edition on archive survey nodes.
            if (!$this_survey['active']) {
              $message = t('This survey is not currently active, you can !activate on the LimeSurvey user interface.', array('!activate' => limesurvey_sync_link_to_ls(t('activate it'), $sid, 'activate')));
              ls_survey_error_or_message($new_node, 'ls_sid', $message);
            }
          }
        }
      }
    }
  }
}

/**
 * Build form for selecting the ls_sid and ls_lang values on the node edit form.
 *
 * This function displays information from the LimeSurvey database, allowing
 * the user to pick a survey and the appropriate language. Results will be
 * themed by the theme_ls_survey_sid() function.
 *
 * @param $form_state
 *   The editing form.
 * @param $node
 *   The survey node object.
 *
 * @return
 *   An array. The drupal formated form.
 */
function ls_survey_sid(&$form_state, $node) {

  // Find default ls_sid and ls_lang :
  $ls_sid_default = $ls_lang_default = '';
  // Find ls_sid first, retreive the provided user value or the default node
  // value if empty.
  if (isset($form_state['input']['surveys']['ls_sid']) && trim($form_state['input']['surveys']['ls_sid']) != '' && is_numeric(trim($form_state['input']['surveys']['ls_sid']))) {
    $ls_sid_default = $form_state['input']['surveys']['ls_sid'];
  }
  elseif (!empty($node->ls_sid) && is_numeric(trim($node->ls_sid))) {
    $ls_sid_default = $node->ls_sid;
  }
  // Then find the ls_lang, retreive the provided user value or the default
  // node value if empty.
  if (!empty($ls_sid_default)) {
    if (isset($form_state['input']['surveys']['ls_lang'][$ls_sid_default]) && !empty($form_state['input']['surveys']['ls_lang'][$ls_sid_default])) {
      $ls_lang_default = $form_state['input']['surveys']['ls_lang'][$ls_sid_default];
    }
    elseif (!empty($node->ls_lang) && $node->ls_sid == $ls_sid_default) {
      $ls_lang_default = $node->ls_lang;
    }
  }

  // Load file for displaying boolean images.
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
  $form = array();

  // Add a css file for the compatibility table.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'ls_survey') . '/ls_survey.css',
  );

  $form['surveys'] = array(
    '#type' => 'fieldset',
    '#title' => t('Surveys from your LimeSurvey site'),
    '#description' => t('Choose a survey and a language'),
    '#weight' => -1,
    '#element_validate' => array('ls_survey_sid_validate'),
    '#tree' => 1,
  );
  // The menu fiedset #weight is -2, the survey fieldset will be upper the
  // menu fieldset.
  $form['surveys']['admin'] = array(
    '#type' => 'markup',
    '#prefix' => '<p>',
    '#markup' => t('Go to the !link to change those options', array('!link' => limesurvey_sync_link_to_ls(t('administration pages of your LimeSurvey site'), $ls_sid_default))),
    '#suffix' => '</p>',
    '#weight' => -3,
  );
  $form['surveys']['compatibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help on LimeSurvey settings for specific features on this site'),
    '#weight' => -1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['surveys']['compatibility']['help']['datas'] = array(
    '#type' => 'markup',
    '#prefix' => '<div>',
    '#markup' => ls_survey_compatibilityhelp($ls_sid_default),
    '#suffix' => '</div>',
  );
  $form['surveys']['compatibility']['table'] = array(
    '#type' => 'fieldset',
    '#title' => t('For custom use'),
    '#collapsible' => 1,
    '#collapsed' => 1,
  );
  $form['surveys']['compatibility']['table']['display_table'] = array(
    '#type' => 'markup',
    '#prefix' => '<div>',
    '#markup' => ls_survey_compatibility_table($node),
    '#suffix' => '</div>',
  );

  // Print all surveys availables from the LimeSurvey website.
  $k = 0;
  $js_tab = '';
  $radios = array();
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $all_surveys = limesurvey_sync_survey_properties('all');

  if ($all_surveys === FALSE) {
    // Avoid error message.
    $all_surveys = array();
  }
  $form_type = array('#type' => 'markup');
  foreach ($all_surveys as $ls_sid => $survey) {
    // Adapted version of the expand_radios() core function, in
    // include/form.inc file, line 1753.
    $disabled = (!$survey['active']);
    $ls_sid_parents = array('surveys', 'ls_sid');
    $parents_for_id = array_merge($ls_sid_parents, array($ls_sid));
    $id = drupal_clean_css_identifier('edit-' . implode('-', $parents_for_id));
    $labelfor = array(
      '#prefix' => '<label class="option" for="' . $id . '">',
      '#suffix' => '</label>',
    );

    $js_lang = array();
    $js_function = 'select_lang(\'' . (((! $disabled) && count(array_keys($survey['#lang'])) > 1) ? $ls_sid : '') . '\');';
    $radios[$ls_sid] = array(
      '#type' => 'radio',
      '#title' => '', // The value will be dispayed by the "display" fieldset.
      '#return_value' => $ls_sid,
      '#default_value' => (!empty($ls_sid_default)) ? $ls_sid_default : NULL,
      '#parents' => $ls_sid_parents,
      '#id' => $id,
      // Custom :
      // Only authorized survey can be checked.
      '#disabled' => ($disabled) ? 1 : 0,
      // Available surveys are on the top.
      '#weight' => ($disabled) ? 1 : 0,
      // Add JS function to enable or disable selecting lang.
      '#attributes' => array('onclick' => $js_function),
    );

    // Add form elements.
    $form['surveys']['display'][$ls_sid]['sid'] = array_merge($form_type, array('#markup' => ls_survey_display_sync_fields('ls_sid', $ls_sid)), $labelfor);
    $form['surveys']['display'][$ls_sid]['active'] = array_merge($form_type, array('#markup' => limesurvey_sync_display_boolean_img($survey['active'])), $labelfor);

    $nbr_lang[$ls_sid] = count($survey['#lang']);
    $radios_lang = array();
    foreach ($survey['#lang'] as $lg => $field) {

      $ls_lang_parents = array('surveys', 'ls_lang', $ls_sid);
      $parents_for_id_lang = array_merge($ls_lang_parents, array($lg));
      $id_lang = drupal_clean_css_identifier('edit-' . implode('-', $parents_for_id_lang));
      $labelfor_lang = array(
        '#prefix' => '<label class="option" for="' . $id_lang . '">',
        '#suffix' => '</label>',
      );
      if ($nbr_lang[$ls_sid] <= 1) {
        $form['surveys']['ls_lang'][$ls_sid][$lg] = array(
          '#type' => 'hidden',
          '#value' => $lg,
          '#weight' => -2,
        );
        $labelfor_lang = $labelfor;
      }
      else {
        $ls_lang_default_value = $ls_lang_default;
        $disabled_lang = ($disabled) ? 1 : (($radios[$ls_sid]['#default_value'] && $ls_sid == $radios[$ls_sid]['#default_value']) ? 0 : 1);
        if ($ls_sid == $ls_sid_default && $lg == $ls_lang_default) {
          $ls_lang_default_value = $ls_lang_default;
        }
        elseif ($lg == $survey['default_language']) {
          $ls_lang_default_value =  $lg;
        }
        else {
          $ls_lang_default_value = NULL;
        }

        $radios_lang[$lg] = array(
          '#type' => 'radio',
          '#title' => '', //
          '#return_value' => $lg,
          '#parents' => $ls_lang_parents,
          '#id' => $id_lang,
          '#default_value' => (!empty($ls_lang_default_value)) ? $ls_lang_default_value : NULL,
          '#weight' => ($lg == $survey['default_language']) ? -2 : 0,
          // custom :
          // Only active survey can be checked.
          '#disabled' => ($disabled_lang) ? 1 : 0,
        );
        $js_lang[] = $lg;
      }

      // Add form elements.
      $form['surveys']['display'][$ls_sid]['lang_surveys'][$lg]['lang_survey'] = array_merge($form_type, array('#markup' => ls_survey_display_sync_fields('ls_lang', $lg)), $labelfor_lang);
      $form['surveys']['display'][$ls_sid]['lang_surveys'][$lg]['title_survey'] = array_merge($form_type, array('#markup' => $field['title']), $labelfor);
      $form['surveys']['display'][$ls_sid]['lang_surveys'][$lg]['description_survey'] = array_merge($form_type, array('#markup' => $field['description']), $labelfor);
      // Node rights will be check below.
      $nid_lang = db_query("SELECT n.nid
      FROM {ls_survey} lss
      INNER JOIN {node} n ON n.vid = lss.vid AND n.nid = lss.nid
      WHERE ls_sid = :ls_sid AND ls_lang = :ls_lang", array(':ls_sid' => $ls_sid, ':ls_lang' => $lg))->fetchField();
      $node_lang = ($nid_lang) ? node_load($nid_lang) : FALSE;
      if ($node_lang) {
        // This survey already exists in the Drupal website.
        // Print it only if allowed.
        $affected = (node_access('view', $node_lang)) ? l($node_lang->title, 'node/' . $nid_lang ) : t('Not available');
      }
      else {
        $affected = ($disabled) ? t('Not available') : t('Available');
      }
      $form['surveys']['display'][$ls_sid]['lang_surveys'][$lg]['affected'] = array_merge($form_type, array('#markup' => $affected), $labelfor_lang);
    }


    if ($nbr_lang[$ls_sid] > 1) {
      $form['surveys']['ls_lang'][$ls_sid] = $radios_lang;
      uasort($form['surveys']['ls_lang'][$ls_sid], 'element_sort');
      if (!$disabled) {
        $js_tab .= "\n" . 'data[' . $k . '] = new Array(\'' . $ls_sid . '\', [\'' . implode('\' , \'', $js_lang) . '\']);';
        $k++;
      }
    }
  }

//  JS function js to activate only radio button of the checked sid.
  $js = 'function select_lang(ls_sid) {'  //  ."\n".' alert(\'test1\');'
   . "\n" . '  var data = new Array();'
  . $js_tab  //  ."\n".' alert(\'test2\');'
  . "\n" . '  for(var i= 0; i < data.length; i++) {'
  . "\n" . '    if (ls_sid != \'\' && ls_sid == data[i][0]) {'
  . "\n" . '      statut = \'\';'
  . "\n" . '    }else{'
  . "\n" . '      statut = \'disabled\';'
  . "\n" . '    };'
  . "\n" . '    for(var j= 0; j < data[i][1].length; j++) {'
  . "\n" . '      lg_id = \'edit-surveys-ls-lang-\' + data[i][0] + \'-\' + data[i][1][j];' //  ."\n".' alert(\'lg_id = \' + lg_id + \', disable = \' + statut);'// + \' \' + statut);'
  . "\n" . '      document.getElementById(lg_id).disabled = statut;'
  . "\n" . '    }'
  . "\n" . '  }'
  . "\n" . '}';

  $form['surveys']['js'] = array(
    '#type' => 'value',
    '#value' => $js,
  );

  $form['surveys']['ls_sid'] = $radios;

  $form['surveys']['#theme'] = 'ls_survey_sid';

  return $form;
}

/**
 * Display properties of all surveys as an html table.
 */
function theme_ls_survey_sid($variables) {
  $form = $variables['form'];

  $js = $form['js']['#value'];
  drupal_add_js($js, array('type' => 'inline'));

  $rows = array();
  foreach (element_children($form['ls_sid'], FALSE) as $ls_sid) {
    $nb_lang = count(element_children($form['ls_lang'][$ls_sid]));
    $adapt_rowspan = ($nb_lang <= 1 ) ? array() : array('rowspan' => $nb_lang);

    $row = array();
    $row[0] = array_merge(array('data' => drupal_render($form['ls_sid'][$ls_sid])), $adapt_rowspan);
    $row[1] = array_merge(array('data' => drupal_render($form['display'][$ls_sid]['sid'])), $adapt_rowspan);
    $row[10] = array_merge(array('data' => drupal_render($form['display'][$ls_sid]['active'])), $adapt_rowspan);
    foreach (element_children($form['ls_lang'][$ls_sid], TRUE) as $lg) {
      $row[2] = '<strong>' . drupal_render($form['display'][$ls_sid]['lang_surveys'][$lg]['title_survey']) . '</strong>';
      $row[3] = drupal_render($form['ls_lang'][$ls_sid][$lg]);
      $row[4] = drupal_render($form['display'][$ls_sid]['lang_surveys'][$lg]['lang_survey']);
      $row[5] = drupal_render($form['display'][$ls_sid]['lang_surveys'][$lg]['affected']);
      $row[6] = drupal_render($form['display'][$ls_sid]['lang_surveys'][$lg]['description_survey']);
      ksort($row); // Sort by keys
      $rows[] = $row;
      $row = array();
    }
  }
  $header_titles[0] = array(
    'data' => t('Survey ID'),
    'colspan' => 2, // Sid radio button.
  );
  $header_titles[2] = array('data' => t('Title'));
  $header_titles[3] = array(
    'data' => t('Default language'),
    'colspan' => 2, // Language radio button.
  );
  $header_titles[5] = array('data' => t('Affected to'));
  $header_titles[6] = array('data' => t('Description'));
  $header_titles[10] = array('data' => t('Activated'));

  $header = $header_titles;
  if (!count($rows)) {
    // Load file for the limesurvey_sync_link_to_ls() function.
    module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');

    $row = array();
    $row[] = array(
      'data' => t('No survey was found : !link_create or check the !link_settings', array(
        '!link_create' => limesurvey_sync_link_to_ls(t('create surveys'), NULL, 'newsurvey'),
        '!link_settings' => l(t('synchronization settings of your LimeSurvey site'), 'admin/config/media/ls_sync'),
      )),
      'colspan' => 9,
      'style' => 'text-align:center',
    );
    $rows[] = $row;
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output . drupal_render_children($form);
}

/**
 * Display the node_survey view page.
 *
 * Display table including informations about this survey from the LimeSurvey
 * website and this site.
 *
 * @param $node
 *   The survey node object.
 *
 * @return
 *   An html table with all datas.
 */
function ls_survey_view_properties($node) {
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
  $survey_properties = limesurvey_sync_survey_properties($node->ls_sid);
  // Load file for displaying boolean images.
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
  $survey_valid = (!empty($survey_properties));
  $lang_valid = (!empty($survey_properties['#lang'][$node->ls_lang]));
  $answ_type = ls_survey_find_answ_type($node->ls_orig_type);
  $row = $rows = $headers = array();

  // Drupal additional languages:
  $drupal_add_lang = array();
  if ($survey_valid) {
    foreach ($survey_properties['#lang'] as $lang => $data_lang) {
      $node_survey = ($data_lang['nid_survey']) ? node_load($data_lang['nid_survey']) : FALSE;
      if ($node_survey && $lang != $node->ls_lang) {
        $lang_name = ls_survey_display_sync_fields('ls_lang', $lang);
        $drupal_add_lang[] = (node_access('view', $node_survey)) ? l($lang_name, 'node/' . $node_survey->nid, array('attributes' => array('title' => $node_survey->title))) : $lang_name;
      }
    }
    sort($drupal_add_lang);
  }
  $drupal_add_lg = (empty($drupal_add_lang)) ? '' : theme('item_list', array('items' => $drupal_add_lang));

  // LimeSurvey additional language
  $ls_add_lang_ls = ($survey_valid && !empty($survey_properties['additional_languages'])) ? explode(' ', $survey_properties['additional_languages']) : array();
  $ls_add_lang_drupal_name = array();
  foreach ($ls_add_lang_ls as $ls_add_lang_ls_id) {
    $ls_add_lang_drupal_name[] = ls_survey_display_sync_fields('ls_lang', $ls_add_lang_ls_id);
  }
  sort($ls_add_lang_drupal_name);
  $ls_add_lg = (empty($ls_add_lang_drupal_name)) ? '' : theme('item_list', array('items' => $ls_add_lang_drupal_name));

  // Set some texts used several times.
  $no_more_survey = t('The survey does not exist anymore !');
  $no_more_lang = t('The survey or the language does not exist anymore !');
  $edit = t('edit');

  $introduction = '<p>' . t('All LimeSurvey functionalities may not be available from this Drupal site, depending on specific survey settings from your LimeSurvey admin interface.')
  . '<br /><strong>' . t('The table below shows which features are availables and how making then availables, setting each LimeSurvey parameter using the [edit] link into the right column.') . '</strong></p>';
  $headers[] = '';
  $headers[] = array(
    'data' => t('On this site'),
    'colspan' => 2,
  );
  $link = ($survey_valid) ? ' [' . limesurvey_sync_link_to_ls($edit, $node->ls_sid) . ']' : '';
  $headers[] = array(
    'data' => t('On the LimeSurvey admin interface') . $link,
    'colspan' => 3,
  );

  // Survey ID.
  $row[] = array(
    'data' => ls_survey_display_sync_headers('ls_sid'),
    'rowspan' => 2,
  );
  $row[] =  array(
    'data' => '<b>' . ls_survey_display_sync_fields('ls_sid', $node->ls_sid) . '</b>',
    'colspan' => 5,
    'align' => 'center',
  );
  $rows[] = $row;
  $row = array();
  $row[] = array('data' => ls_survey_display_sync_headers('ls_orig_type'));
  $row[] = array('data' => ($answ_type) ? $answ_type->name : t('Not associated to an answers content type'));
  $row[] = array(
    'data' => '',
    'colspan' => 3,
  );
  $rows[] = $row;
  $row = array();

  $row[] = t('Title');
  $row[] = array(
    'data' => $node->title,
    'colspan' => 2,
  );
  $row[] = array(
    'data' => ($lang_valid) ? $survey_properties['#lang'][$node->ls_lang]['title'] : $no_more_lang,
    'colspan' => 2,
  );
  $row[] = array(
    'data' => ($lang_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveylocalesettings', limesurvey_sync_localsettings_anchor($node->ls_sid, $node->ls_lang)) . ']' : '',
    'rowspan' => 2,
  );
  $rows[] = $row;
  $row = array();

  $row[] = t('Description');
  $row[] = array(
    'data' => ($answ_type) ? $answ_type->description : '',
    'colspan' => 2,
  );
  $row[] = array(
    'data' => ($lang_valid) ? $survey_properties['#lang'][$node->ls_lang]['description'] : $no_more_lang,
    'colspan' => 2,
  );
  $rows[] = $row;
  $row = array();

  // Default language.
  $row[] = ls_survey_display_sync_headers('ls_lang');
  $row[] = array(
    'data' => ls_survey_display_sync_fields('ls_lang', $node->ls_lang),
    'colspan' => 2,
  );
  $row[] = array(
    'data' => ($survey_valid) ? ls_survey_display_sync_fields('ls_lang', $survey_properties['default_language']) : $no_more_survey,
    'colspan' => 2,
  );
  $row[] = array(
    'data' => ($survey_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveysettings', 'general') . ']' : '',
    'rowspan' => 2,
  );
  $rows[] = $row;
  $row = array();

  $row[] = t('Additional languages');
  $row[] = array(
    'data' => (!empty($drupal_add_lg)) ? $drupal_add_lg : t('none'),
    'colspan' => 2,
  );
  $row[] = array(
    'data' => (!empty($ls_add_lg)) ? $ls_add_lg : t('none'),
    'colspan' => 2,
  );
  $rows[] = $row;
  $row = array();

  $answers_creatable = ($survey_valid && $survey_properties['is_available']);
  $row[] = array(
    'data' => $answers_creatable ? t('Answers can be <strong>created</strong>') : t('Answers <strong>can not be created</strong>'),
    'rowspan' => 3,
  );
  $row[] = array(
    'data' => limesurvey_sync_display_boolean_img($answers_creatable, FALSE),
    'colspan' => 2,
    'rowspan' => 3,
  );
  $row[] = ($survey_properties['active']) ? t('The survey is activated') : '<strong>' . t('Because the survey is not activated') . '</strong>';
  $row[] = ($survey_valid) ? limesurvey_sync_display_boolean_img($survey_properties['active'], FALSE) : $no_more_survey;
  $row[] = ($survey_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveysettings', 'general') . ']' : '';
  $rows[] = $row;
  $row = array();
  $startdate_status = (empty($survey_properties['startdate']) || $survey_properties['startdate'] < REQUEST_TIME);
  $row[] = ($startdate_status) ? t('Start date is valid') : '<strong>' . t('Because the start date is not valid') . '</strong>';
  $row[] = limesurvey_sync_display_boolean_img($startdate_status, FALSE) . ' ' . ((!empty($survey_properties['startdate'])) ? format_date($survey_properties['startdate'], 'short') : '-');
  $row[] = array(
    'data' => ($survey_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveysettings', 'publication') . ']' : '',
    'rowspan' => 2,
  );
  $rows[] = $row;
  $row = array();
  $expiresdate_status = (empty($survey_properties['expires']) || $survey_properties['expires'] > REQUEST_TIME);
  $row[] = ($expiresdate_status) ? t('Expiry date is valid') : '<strong>' . t('Because the expiry date is not valid') . '</strong>';
  $row[] = limesurvey_sync_display_boolean_img($expiresdate_status, FALSE) . ' ' . ((!empty($survey_properties['expires'])) ? format_date($survey_properties['expires'], 'short') : '-');
  $rows[] = $row;
  $row = array();

  $backurl_status = ($survey_valid && $survey_properties['#lang'][$node->ls_lang]['back_to_drupal']);
  $row[] = array(
    'data' => t('LimeSurvey end URL, redirecting to this site after completion'),
    'rowspan' => 2,
  );
  $row[] = t('Correct end url');
  $row[] = ($survey_valid) ? $survey_properties['#lang'][$node->ls_lang]['correct_back_url'] : '';
  $row[] = array(
    'data' => ($lang_valid) ? t('actual end url') : $no_more_lang,
    'rowspan' => 2,
  );
  $row[] = array(
    'data' => ($lang_valid) ? ((!empty($survey_properties['#lang'][$node->ls_lang]['back_url'])) ? $survey_properties['#lang'][$node->ls_lang]['back_url'] : t('none')) : $no_more_lang,
    'rowspan' => 2,
  );
  $row[] = array(
    'data' => ($lang_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveylocalesettings', limesurvey_sync_localsettings_anchor($node->ls_sid, $node->ls_lang)) . ']' : '',
    'rowspan' => 2,
  );
  $rows[] = $row;
  $row = array();
  $row[] = $backurl_status ? t('The end url is correctly set for advanced features') : '<strong>' . t('The end url is not correctly set for advanced features') . '</strong>';
  $row[] = limesurvey_sync_display_boolean_img($backurl_status, FALSE);
  $rows[] = $row;
  $row = array();

  $answers_editable = ($survey_valid && $survey_properties['can_edit_answer']);
  $row[] = array(
    'data' => $answers_editable ? t('Answers can be <strong>edited</strong>') : t('Answers <strong>can not be edited</strong>'),
    'rowspan' => 3,
  );
  $link = ($survey_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveysettings', 'tokens') . ']' : '';
  $row[] = array(
    'data' => limesurvey_sync_display_boolean_img($answers_editable, FALSE),
    'colspan' => 2,
    'rowspan' => 3,
  );

  $arg = array(
    '%parameter' => t('Enable token-based response persistence'),
    '%value' => t('Yes'),
  );
  $row[] = ($survey_properties['tokenanswerspersistence']) ? t('%parameter is set to %value', $arg) : '<strong>' . t('Because %parameter is not set to %value', $arg) . '</strong>';
  $row[] = ($survey_valid) ? limesurvey_sync_display_boolean_img($survey_properties['tokenanswerspersistence'], FALSE) : $no_more_survey;
  $row[] = array(
    'data' => ($survey_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveysettings', 'tokens') . ']' : '',
    'rowspan' => 2,
  );
  $rows[] = $row;
  $arg = array(
    '%parameter' => t('Allow editing answers after completion'),
    '%value' => t('Yes'),
  );
  $row = array();
  $row[] = ($survey_properties['alloweditaftercompletion']) ? t('%parameter is set to %value', $arg) : '<strong>' . t('Because %parameter is not set to %value', $arg) . '</strong>';
  $row[] = ($survey_valid) ? limesurvey_sync_display_boolean_img($survey_properties['alloweditaftercompletion'], FALSE) : $no_more_survey;
  $rows[] = $row;
  $row = array();
  $row[] = ($survey_properties['tokens_table']) ? t('The token table has been created') : '<strong>' . t('Because the token table has not been created') . '</strong>';
  $row[] = ($survey_valid) ? limesurvey_sync_display_boolean_img($survey_properties['tokens_table'], FALSE) : $no_more_survey;
  $row[] = array('data' => ($survey_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'tokens') . ']' : '');
  $rows[] = $row;
  $row = array();

  $answers_importable = ($survey_valid && $survey_properties['date_answer'] && !$survey_properties['anonymized']);
  $row[] = array(
    'data' => $answers_importable ? t('Answers can be automatically <strong>imported</strong>') : t('Answers <strong>can not be automatically imported</strong>'),
    'rowspan' => 2,
  );
  $row[] = array(
    'data' => limesurvey_sync_display_boolean_img($answers_importable, FALSE),
    'colspan' => 2,
    'rowspan' => 2,
  );
  $arg = array(
    '%parameter' => t('Anonymized responses'),
    '%value' => t('No'),
  );
  $row[] = (!$survey_properties['anonymized']) ? t('%parameter is set to %value', $arg) : '<strong>' . t('Because %parameter is not set to %value', $arg) . '</strong>';
  $row[] = ($survey_valid) ? limesurvey_sync_display_boolean_img(!$survey_properties['anonymized'], FALSE) : $no_more_survey;
  $row[] = array('data' => ($survey_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveysettings', 'tokens') . ']' : '');
  $rows[] = $row;
  $row = array();
  $arg = array(
    '%parameter' => t('Date stamp'),
    '%value' => t('Yes'),
  );
  $row[] = ($survey_properties['date_answer']) ? t('%parameter is set to %value', $arg) : '<strong>' . t('Because %parameter is not set to %value', $arg) . '</strong>';
  $row[] = ($survey_valid) ? limesurvey_sync_display_boolean_img($survey_properties['date_answer'], FALSE) : $no_more_survey;
  $row[] = ($survey_valid) ? '[' . limesurvey_sync_link_to_ls($edit, $node->ls_sid, 'editsurveysettings', 'notification') . ']' : '';
  $rows[] = $row;
  $row = array();


  $output = theme('table', array('header' => $headers, 'rows' => $rows));
  return $introduction . $output;
}

/**
 * Display an html help text for compatibility with the ls_survey module.
 *
 * Provide correct values the survey language depending on the survey ID
 * reference provided and the authorized value available.
 *
 * @param $sid
 *   A numeric (optional). The survey ID reference. Default is NULL
 *
 * @return
 *   An html string displaying helping text for configuring the LimeSurvey
 *   website for ls_survey module compatibility.
 */
function ls_survey_compatibilityhelp($sid = FALSE) {
  // Load file for the limesurvey_sync_link_to_ls() function.
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');

  if ($sid) {
    $ls_link = limesurvey_sync_link_to_ls(t('LimeSurvey admin interface'), $sid, 'editsurveysettings', 'presentation');
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    $survey_properties = limesurvey_sync_survey_properties($sid);
  }
  else {
    $ls_link = limesurvey_sync_link_to_ls(t('LimeSurvey admin interface'), NULL, 'listsurveys');
  }
  $type = 'your_answers_content_type_value';
  $url_backfromsurvey = url('backfromsurvey/' . $type, array('absolute' => TRUE));

  $output = '<p>' . t('Note that this module does not override the LimeSurvey site settings. In order to access to specific features, you need to set appropiate survey settings <em>on the !ls_link</em>', array('!ls_link' => $ls_link)) . '</p>';
  $output .= '<p>' . t('In a nutshell :') . '</p>';
  $list = array();
  $list[0]['data'] =  '<b>' . t('For a basic use : you only need to activate the survey.') . '</b>';
  $list[1]['data'] =  t('For the most advanced use, you need to :');
  $list[1]['children'][] = t('enable token-based response persistence,');
  $list[1]['children'][] = t("set 'allow editing answers after completion' to <em>yes</em>,");
  $list[1]['children'][] = t("create the invitation token table,");
  $list[1]['children'][] = t("and set on <em>each language</em> the 'end url' to %end_url.", array('%end_url' => $url_backfromsurvey));
  $list[2]['data'] = t('For a custom use : check below.');

  return $output . theme('item_list', array('items' => $list));
}

/**
 * Display a helper table for ls settings.
 *
 * @param $node
 *   The survey node object. Default is NULL (not provided).
 *
 * @return
 *   An html table with all datas.
 */
function ls_survey_compatibility_table($node = NULL) {
  // Load file for displaying boolean images.
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
  $cells = $cell = $header = array();
  $center = array('align' => 'center');
  $empty_cell = '';
  $type = 'your_answers_content_type_value';
  if (!empty($node)) {
    if ($answ_type = ls_survey_find_answ_type($node->ls_orig_type)) {
      $type = $answ_type->type;
    }
  }
  $url_backfromsurvey = url('backfromsurvey/' . $type, array('absolute' => TRUE));
  $url_backfromsurvey_frames = $url_backfromsurvey . '<b>/back</b>';

  // Add css file for displaying the compatibility table borders.
  // Edit : The ls_survey.css is load into the ls_survey_sid() function.

  $sub_header = array('header' => TRUE);
  $no_striping = array('no_striping' => TRUE);
  $first_col = array('class' => 'ls_first_col');
  $header = array();
  $header[] = array(
    'data' => t('Settings on the Limesurvey admin interface'),
    'colspan' => 2,
    'class' => 'ls_bd_top_left',
  );

  $header[] = array_merge(array('data' => t('Activate survey')), array('class' => 'ls_bd_top'));
  $header[] = array_merge(array('data' => t('Available')), array('class' => 'ls_bd_top_right'));
  $header[] = array_merge(array('data' => t('Allow editing answers after completion')));
  $header[] = array_merge(array('data' => t('Enable token-based response persistence')));
  $header[] = array_merge(array('data' => t('Use token invitations')));
  $header[] = array_merge(array('data' => t('End URL')));
  $header[] = array_merge(array('data' => t('Date stamp')));
  $header[] = array_merge(array('data' => t('Anonymized responses')));

  $cell[] = array_merge(array('data' => ''), array('header' => TRUE, 'class' => 'ls_bd_left'));
  $cell[] = array_merge(array('data' => t('Set to :'), 'align' => 'right'), $first_col);
  $cell[] = array_merge(array('data' => t('Currently active')), $center);
  $cell[] = array_merge(array('data' => t('Current time is between the survey start date and expiry date (default setting)')), $center, array('class' => 'ls_bd_right'));
  $cell[] = array_merge(array('data' => t('Yes')), $center);
  $cell[] = array_merge(array('data' => t('Yes')), $center);
  $cell[] = array_merge(array('data' => t("Yes (also called : '<em>switch to closed-access mode</em>', or '<em>initialize token</em>', or '<em>create tokens table</em>'")), $center);
  $end_url_text = $url_backfromsurvey;
  $cell[] = array_merge(array('data' => $end_url_text), $center);
  $cell[] = array_merge(array('data' => t('Yes')), $center);
  $cell[] = array_merge(array('data' => t('No')), $center);
  $cells[] = array_merge(array('data' => $cell, 'class' => 'ls_first_row'), $no_striping);

  $cell = array();
  $cell[] = array_merge(array('data' => '<strong>' . t('Basic features') . '</strong>', 'rowspan' => 2), $sub_header, $center, array('class' => 'ls_bd_bottom_left'));
  $cell[] = array_merge(array('data' => t('Create survey')), $first_col);
  $cell[] = limesurvey_sync_display_boolean_img(TRUE);
  $cell[] = array(
    'data' => $empty_cell,
    'class' => 'ls_bd_right',
  );
  $cell = array_merge($cell, array_fill(4, 6, $empty_cell));
  $cells[] = $cell;

  $cell = array();
  $cell[] = array_merge(array('data' => t('Create answers'), 'class' => 'ls_bd_intersect'), $no_striping);
  $cell[] = array(
    'data' => $empty_cell,
    'class' => 'ls_bd_bottom',
  );
  $cell[] = array(
    'data' => limesurvey_sync_display_boolean_img(TRUE),
    'class' => 'ls_bd_bottom_right',
  );
  $cell = array_merge($cell, array_fill(3, 6, $empty_cell));
  $cells[] = $cell;

  $cell = array();
  $cell[] = array_merge(array(
    'data' => '<strong>' . t('Advanced features') . '</strong>',
    'rowspan' => 7,
  ), $sub_header);
  $cell[] = array_merge(array('data' => t('Edit answers')), $first_col);
  $cell = array_merge($cell, array_fill(2, 2, $empty_cell));
  $cell = array_merge($cell, array_fill(4, 3, limesurvey_sync_display_boolean_img(TRUE)));
  $cell = array_merge($cell, array_fill(6, 3, $empty_cell));
  $cells[] = $cell;

  $cell = array();
  $cell[] =  array_merge(array('data' => t('Import drupal users informations into the LimeSurvey results')), $first_col);
  $cell = array_merge($cell, array_fill(1, 3, $empty_cell));
  $cell = array_merge($cell, array_fill(4, 2, limesurvey_sync_display_boolean_img(TRUE)));
  $cell = array_merge($cell, array_fill(6, 3, $empty_cell));
  $cells[] = $cell;

  $cell = array();
  $cell[] = array_merge(array('data' => t('Display answer form into an iframe')), $first_col);
  $cell = array_merge($cell, array_fill(1, 5, $empty_cell));
  $cell[] = $url_backfromsurvey_frames;
  $cell = array_merge($cell, array_fill(7, 2, $empty_cell));
  $cells[] = $cell;

  $cell = array();
  $cell[] = array_merge(array('data' => t('Redirection to a drupal specific page after completion')), $first_col);
  $cell = array_merge($cell, array_fill(1, 5, $empty_cell));
  $cell[] = limesurvey_sync_display_boolean_img(TRUE) . t("Or directly set it as the end url if you don't use neither the auto import answers or the iframe features.");
  $cell = array_merge($cell, array_fill(7, 2, $empty_cell));
  $cells[] = $cell;

  $cell = array();
  $cell[] = array_merge(array('data' => t('Import all answers manually')), $first_col);
  $cell = array_merge($cell, array_fill(1, 6, $empty_cell));
  $cell[] = array_merge(array('data' => t('If set to %yes_no : after survey expiry date only', array('%yes_no' => t('No')))), $center);
  $cell[] = array_merge(array('data' => t('If set to %yes_no : after survey expiry date only', array('%yes_no' => t('Yes')))), $center);
  $cells[] = $cell;

  $cell = array();
  $cell[] = array_merge(array('data' => t('Automatic import answers') . '<br />' . t('and User view own page after completion')), $first_col);
  $cell = array_merge($cell, array_fill(1, 5, $empty_cell));
  $cell = array_merge($cell, array_fill(6, 3, limesurvey_sync_display_boolean_img(TRUE)));
  $cells[] = $cell;

  $cell = array();
  $cell[] = array_merge(array('data' => t('Generate numeric answer titles')), $first_col);
  $cell = array_merge($cell, array_fill(1, 7, $empty_cell));
  $cell[] = limesurvey_sync_display_boolean_img(TRUE);
  $cells[] = $cell;

  return theme('table', array('header' => $header, 'rows' => $cells));
}

/**
 * Display on error or a simple message depending on a condition.
 *
 * This allow a node to be still editable (on revision, published status for
 * example) displaying warning message about desynchronization whereas error
 * messages would break node general edition.
 *
 * @param $error
 *   A Boolean. If TRUE, an error message is displayed. If FALSE, a warning
 *   message is displayed.
 * @param $field
 *   A string. The corresponding form field.
 * @param $message
 *   A string. The message to be displayed.
 * @param $type
 *   A string (optional). The type of the message. One of the following values
 *   are possible: 'status', 'warning', 'error'. Default to 'warning'.
 */
function ls_survey_error_or_message($error, $field, $message, $type = 'warning') {
  if (!empty($message)) {
    if ($error) {
      form_set_error($field, filter_xss($message));
    }
    else {
      drupal_set_message(filter_xss($message), $type);
    }
  }
}

/**
 * Provide human readable value for the sync values.
 *
 * Used for dysplaying synchronization datas values, for example in the views
 * module.
 *
 * @param $field
 *   A string. The node answer synchronization field name.
 * @param $value
 *   The node answer synchronization correponding value.
 * @param $langcode
 *   A string, the language code to use to translate the values.
 *   Default is NULL, corresponding to the Drupal default language.
 *
 * @return
 *   A string. The formated value.
 */
function ls_survey_display_sync_fields($field, $value, $langcode = NULL) {
  switch ($field) {

    case 'ls_sid':
      // Display nothing for non ls_survey module nodes, '45216' become :
      // '45 216'.
      // @TODO : Use a unbreakable space (http://groups.drupal.org/node/8007).
      $return = (empty($value)) ? '' : number_format($value, 0, '.', ' ');
      break;

    case 'ls_orig_type':
      // Don't provide the orig_type but the actual type.
      $answer_type = ls_survey_find_answ_type($value);
      $return = (!$answer_type) ? '' : $answer_type->type;
      break;

    case 'ls_lang':
      // Explicit language ('en' changed to 'english').
      module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
      $drupal_lang = limesurvey_sync_lang_convert($value);
      $return = ls_survey_language_name($drupal_lang, $langcode);
      break;

    default:
      $return = (!empty($value)) ? $value : '';
  }
  return $return;
}

/**
 * Provide human readable value for the sync fields.
 *
 * Used for dysplaying synchronization datas fields name, for example in the
 * views module.
 *
 * @param $col
 *   A string. The node answer synchronization field name.
 *
 * @return
 *   A string. The formated field value.
 */
function ls_survey_display_sync_headers($col) {
  switch ($col) {

    case 'ls_sid':
      // Translated into the ls_survey module.
      $return = t('Default survey ID');
      break;

    case 'ls_lang':
      $return = t('Default survey language');
      break;

    case 'ls_orig_type':
      $return = t('Answers content type');
      break;
    default:
      $return = '';
  }
  return $return;
}

/**
 * Returns the language name from a LimeSurvey language ID.
 *
 * This function acts as locale_language_name() will do, except it returns
 * the name even if the corresponding LimeSurvey language id is not enabled on
 * the Drupal site or if the locale module is not enabled.
 *
 * @param $dp_lang
 *   A string, the Drupal language id.
 * @param $langcode
 *   A string, the language code to use to translate the language names.
 *   Default is NULL, corresponding to the Drupal default language.
 *
 * @return
 *   A string. The language name.
 */
function ls_survey_language_name($dp_lang, $langcode = NULL) {
  static $list = NULL;
  if (empty($langcode)) {
    global $language;
    $langcode = isset($language->language) ? $language->language : 'en';
  }
  if (!isset($list[$langcode])) {
    // Set the previous db connection name.
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
    $previous_db = limesurvey_sync_active_db_name();
    // Switch to Drupal connection for the locale_language_list() function.
    limesurvey_sync_db_close();
    $t_arg = array();
    $t_lang = array('langcode' => $langcode);

    // Note : use _locale_prepare_predefined_list(), for returning all existing
    // languages.
    include_once DRUPAL_ROOT . '/includes/iso.inc';
    $predefined = _locale_get_predefined_list();
    // Check if the Locale module is not enabled in order to avoid an issue
    // (http://drupal.org/node/1815974#comment-6752530).
    $defined = array();
    if (module_exists('locale')) {
      module_load_include('module', 'locale');
      // Load all edited languages (not only enabled one).
      $defined = locale_language_list('name', TRUE);
    }
    foreach ($predefined as $key => $value) {
      // The predefined language name may be overwrited, update them.
      if (isset($defined[$key])) {
        $list[$langcode][$key] = t($defined[$key], $t_arg, $t_lang);
      }
      else {
        // Include native name in output, if possible
        if (count($value) > 1) {
          $tname = t($value[0], $t_arg, $t_lang);
          $list[$langcode][$key] = ($tname == $value[1]) ? $tname : "$tname ($value[1])";
        }
        else {
          $list[$langcode][$key] = t($value[0], $t_arg, $t_lang);
        }
      }
    }
    asort($list[$langcode]);
    // Switch back to the previous connection.
    limesurvey_sync_db_close($previous_db);
  }
  return ($dp_lang && isset($list[$langcode][$dp_lang])) ? $list[$langcode][$dp_lang] : t('All', array(), $t_lang);
}
