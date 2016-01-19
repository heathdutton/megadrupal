<?php
/**
 * @file
 * fonctions for the answers content type edit page.
 */

/**
 * Provide default values for the answer node type options.
 *
 * @return
 *   An array of the each option default value.
 */
function ls_survey_answ_default_options() {
  $compatible_formats = ls_survey_type_compatible_filters();
  $default_ls_filter = (array_key_exists(0, $compatible_formats)) ? $compatible_formats[0] : '';
  return array(
    'ls_redirect' => 'redirect',
    'ls_overwrite_endurl' => 0,
    'iframe_dim' => 'auto',
    'iframe_dim_width' => '100%',
    'iframe_dim_height' => 700,
    'ls_validuntil' => 7200,
    'ls_end' => 'page',
    'ls_end_url' => '',
    'ls_hide_lang' => 0,
    'ls_prefix' => 'Response #',
    'ls_suffix_method' => 'numeric',
    'ls_suffix_min' => 1000,
    'ls_suffix_max' => 9999,
    'ls_disabled_title' => 0,
    'ls_import_lang' => '',
    'ls_only_answered' => 0,
    'ls_merge' => 1,
    'ls_ignore_x' => 0,
    'ls_fields' => array(),
    'ls_format' => $default_ls_filter,
    'ls_auto_import' => 0,
    'ls_author' => '',
    'ls_standing_by_status' => 0,
    'ls_export_token' => 0,
    'ls_keep_revisions' => 0,
    'ls_revision' => 0,
    'ls_recycling' => 0,
    'ls_options_last_import' => REQUEST_TIME,
  );
}

/**
 * Display the answers content type edit page.
 *
 * Display the answers content type edit page on the options tab of the
 * associated survey node.
 */
function ls_survey_type($node) {
  module_load_include('inc', 'ls_survey', 'ls_survey');
  $type = ls_survey_find_answ_type($node->ls_orig_type);
  return drupal_get_form('node_type_form', $type);
}

/**
 * Form builder for the answers content type options.
 *
 * Set options answers content type on a specific fieldset
 *
 * @see ls_survey_answ_options_validate()
 * @see ls_survey_node_type_validate()
 * @see ls_survey_answ_options_submit()
 * @see ls_survey_answ_options_redirect()
 * @ingroup forms
 */
function ls_survey_answ_options($form) {
  $node_type = $form['#node_type'];

  // Load appropriate variable, depending on the type
  $variable_name = ($node_type->module == 'ls_survey') ? 'ls_survey_options' : 'ls_answ_options_' . $node_type->type;
  $answer_options = variable_get($variable_name, FALSE);
  if (!$answer_options) {
    $answer_options = variable_get('ls_survey_options', ls_survey_answ_default_options());
  }

  $sid = $answ_type = FALSE;
  $lang = '';
  if ($node_type->module != 'ls_survey' && module_exists('ls_answ')) {
    // Load file for the ls_answ_find_survey() function.
    module_load_include('inc', 'ls_answ', 'ls_answ');
    $node_survey = ls_answ_find_survey($node_type->type);
    // Load survey datas.
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    $survey_properties = limesurvey_sync_survey_properties($node_survey->ls_sid);
    $sid = $node_survey->ls_sid;
    $lang = $node_survey->ls_lang;
    $answ_type = $node_type->type;
    $answ_type_name = $node_type->name;
  }
  elseif ($node_type->module == 'ls_survey') {
    $answ_type_name = 'the_answers_content_type';
  }

  $form['ls_answers_options'] = array(
    '#type' => 'fieldset',
    '#title' => ($node_type->module == 'ls_survey') ? t('Default options for responses content types') : t('Options for this responses content type'),
    '#description' => ($node_type->module == 'ls_survey') ? t('Choose default display options for responses content types') : t('Choose display options for this responses content type'),
    '#weight' => -1,
    // Put the validate function into the hook_form_alter instead to make the
    // #node_type variable available for the validate function.
    //'#element_validate' => array('ls_survey_answ_options_validate'),
  );

  $redirect_method = array(
    'link' => t('print a simple hypertext link'),
    'form' => t('print a form button'),
    'redirect' => t('automatic and immediate redirection'),
    'iframe' => t('open into an iframe'),
    'colorbox' => t("open into a lightbox from the '<a href=\"http://drupal.org/project/colorbox\">colorbox</a>' module (dimensions can be set using percents or pixels)"),
    'lightbox2' => t("open into a lightbox from the '<a href=\"http://drupal.org/project/lightbox2\">lightbox2</a>' module (dimensions can be set using pixels only)"),
    // The modalframe is Drupal6 only (D7 port: http://drupal.org/node/491224).
    //'modalframe' => t("open into a Modalframe frame from the '<a href=\"http://drupal.org/project/modalframe\">Modal Frame API</a>' module"),
  );

  $form['ls_answers_options']['answ_redirect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options for the response completing form'),
    '#description' => t('Users submits their answers <em>on</em> the LimeSurvey website. Choose options about the completing form.'),
    '#collapsible' => 1,
    '#collapsed' => 0,
  );
  $message = t('In order to automatically close the frame, you should :');
  $current_type = ($answ_type && !empty($form_state['values']['type']) && trim($form_state['values']['type']) != $type) ? $form_state['values']['type'] : $answ_type;
  $form['ls_answers_options']['answ_redirect']['ls_redirect'] = array(
    '#type' => 'radios',
    '#title' => t('Choose the way to redirect the user to the LimeSurvey site, in order to fill the survey'),
    '#default_value' => $answer_options['ls_redirect'],
    '#options' => $redirect_method,
    '#description' => $message . ls_survey_type_help_iframe($sid, $current_type, TRUE, $lang),
  );
  $form['ls_answers_options']['answ_redirect']['ls_overwrite_endurl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically overwrite the LimeSurvey end url into the LimeSurvey database, in order to reflect the above settings.'),
    '#default_value' => $answer_options['ls_overwrite_endurl'],
    '#return_value' => 1,
    '#description' => t('Or edit it manually from the LimeSurvey admin interface.'),
  );
  // Create a 'hidden' variable : it will be rewrited into the validate process
  // if the end url must be rewrited, storing the correct end url.
  $form['ls_answers_options']['answ_redirect']['ls_overwrite_endurl_url'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  $form['ls_answers_options']['answ_redirect']['iframe_dim'] = array(
    '#type' => 'item',
    '#title' => t('Choose the iframe/lightbox dimensions'),
    '#description' => t("Set '0' to automatically fit the frame on one dimension."),
    '#states' => array('visible' => array(':input[name="ls_redirect"]' => array(
          array('value' => 'iframe'),
          array('value' => 'colorbox'),
          array('value' => 'lightbox2'),
        ))),
  );
  $form['ls_answers_options']['answ_redirect']['iframe_dim']['auto'] = array(
    '#type' => 'radio',
    '#title' => t('automatically fit to the user window'),
    '#default_value' => $answer_options['iframe_dim'],
    '#return_value' => 'auto',
    '#parents' => array('iframe_dim'),
    // The first radio button needs to make the style match a normal radios
    // group.
    '#prefix' => '<div class="form-radios">',
  );
  $form['ls_answers_options']['answ_redirect']['iframe_dim']['invisible'] = array(
    '#type' => 'radio',
    '#title' => t('Invisible iframe ! No vertical scrolling bar into on the iframe') . ' (' . t('beta') . ')',
    '#default_value' => $answer_options['iframe_dim'],
    '#return_value' => 'invisible',
    '#parents' => array('iframe_dim'),
  );
  $form['ls_answers_options']['answ_redirect']['iframe_dim']['iframe_dim_group'] = array(
    '#prefix' => '<div class="container-inline form-item">',
    '#suffix' => '</div>',
  );
  $form['ls_answers_options']['answ_redirect']['iframe_dim']['iframe_dim_group']['cutom'] = array(
    '#type' => 'radio',
    '#title' => t('Use thoses dimensions :'),
    '#default_value' => $answer_options['iframe_dim'],
    '#return_value' => 'custom',
    '#parents' => array('iframe_dim'),
  );
  $form['ls_answers_options']['answ_redirect']['iframe_dim']['iframe_dim_group']['iframe_dim_width'] = array(
    '#type' => 'textfield',
    '#default_value' => $answer_options['iframe_dim_width'],
    '#required' => 0,
    '#size' => 8,
    '#title' => t('width'),
    '#suffix' => t('px or %') . '; ',
  );
  $form['ls_answers_options']['answ_redirect']['iframe_dim']['iframe_dim_group']['iframe_dim_height'] = array(
    '#type' => 'textfield',
    '#default_value' => $answer_options['iframe_dim_height'],
    '#required' => 0,
    '#size' => 8,
    '#title' => t('height'),
    '#suffix' => t('px or %') . '</div>',
  );

  $form['ls_answers_options']['answ_redirect']['ls_validuntil'] = array(
    '#type' => 'select',
    '#title' => t('Choose expiration period for completing the survey'),
    '#default_value' => $answer_options['ls_validuntil'],
    '#description' => t('Since printing the survey to its final submission'),
    '#options' => array(
      600 => t('10 min'),
      3600 => t('1 hour'),
      7200 => t('2 hours'),
      86400 => t('1 day'),
      0 => t('Unlimited (not recommanded)'),
    ),
  );
  $end_url = array(
    'own' => t('its answer view page'),
    'page' => t('this specific page :'),
  );
  // In order to use the core sentence, remove double em tag.
  $right = filter_xss(t('%type_name: View own content', array('%type_name' => $answ_type_name)), array());
  $url_perm = url('admin/people/permissions');
  $form['ls_answers_options']['answ_redirect']['ls_end'] = array(
    '#type' => 'item',
    '#title' => t('Once the form is submitted, redirect the user to'),
    '#description' => t('In order to allow a user to view its submission datas, you need to <b>set the appropiate rights</b> (%right) on the <a href="@url">permission setting page</a>', array('%right' => $right, '@url' => $url_perm)),
  );
  $form['ls_answers_options']['answ_redirect']['ls_end']['own'] = array(
    '#type' => 'radio',
    '#title' => t('its answer view page'),
    '#default_value' => $answer_options['ls_end'], // If there is a default value, this should also be specified for each radio button.
    '#return_value' => 'own',
    '#parents' => array('ls_end'), // You must specify this for each radio button for them to act as a group.
    '#prefix' => '<div class="form-radios">', // The first radio button needs to make the style match a normal radios group.
  );
  $form['ls_answers_options']['answ_redirect']['ls_end']['page_group'] = array(
    '#prefix' => '<div class="container-inline form-item">',
    '#suffix' => '</div>',
  );
  $form['ls_answers_options']['answ_redirect']['ls_end']['page_group']['page'] = array(
    '#type' => 'radio',
    '#title' => t('this specific page :'),
    '#default_value' => $answer_options['ls_end'],
    '#return_value' => 'page',
    '#parents' => array('ls_end'),
  );
  $end_url_length = drupal_strlen($answer_options['ls_end_url']);
  $form['ls_answers_options']['answ_redirect']['ls_end']['page_group']['ls_end_url'] = array(
    '#type' => 'textfield',
    '#default_value' => $answer_options['ls_end_url'],
    '#description' => 'For example : node/458, leave it blank for the home page.',
    '#required' => 0,
    '#size' => ($end_url_length == 0) ? 20 : ($end_url_length + 10),
    '#suffix' => '</div>',
  );
  $form['ls_answers_options']['answ_redirect']['ls_hide_lang'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the languages dropdown on the Drupal page preceding the LimeSurvey survey form page.'),
    '#default_value' => $answer_options['ls_hide_lang'],
    '#return_value' => 1,
    '#description' => t("If the 'Locale' core module is enabled. The default language is set below."),
  );

  $form['ls_answers_options']['answ_title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options for the respond title'),
    '#description' => t('Choose how the title associated to each response will be created'),
    '#collapsible' => 1,
    '#collapsed' => 1,
  );
  $ls_prefix = (($node_type->module != 'ls_survey') ? $answer_options['ls_prefix'] : t('the_answers_content_type #'));
  $form['ls_answers_options']['answ_title']['ls_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix for the response title'),
    '#default_value' => $ls_prefix,
    '#description' => 'Delete the value for an empty title',
    '#required' => 0,
    '#size' => drupal_strlen($ls_prefix) + 10,
  );
  $form['ls_answers_options']['answ_title']['suffix'] = array(
    '#type' => 'item',
    '#title' => t('Suffix, to identify a single response'),
  );
  $default_random_method = (!empty($sid) && $survey_properties['anonymized']) ? 'random' : $answer_options['ls_suffix_method'];
  $form['ls_answers_options']['answ_title']['suffix']['numeric'] = array(
    '#type' => 'radio',
    '#title' => t('An increment number'),
    // The default value should also be specified for each radio button.
    '#default_value' => $default_random_method,
    '#return_value' => 'numeric',
    // You must specify this for each radio button for them to act as a group.
    '#parents' => array('ls_suffix_method'),
    // The first radio button needs to make the style match a normal radios group.
    '#prefix' => '<div class="form-radios">',
  );
  $form['ls_answers_options']['answ_title']['suffix']['random_group'] = array(
    '#prefix' => '<div class="container-inline form-item">',
    '#suffix' => '</div>',
  );
  $form['ls_answers_options']['answ_title']['suffix']['random_group']['random'] = array(
    '#type' => 'radio',
    '#title' => t('A random number between :'),
    '#default_value' => $default_random_method,
    '#return_value' => 'random',
    '#parents' => array('ls_suffix_method'),
  );
  $form['ls_answers_options']['answ_title']['suffix']['random_group']['ls_suffix_min'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimal number'),
    '#default_value' => $answer_options['ls_suffix_min'],
    '#size' => '5',
    '#maxlength' => '10',
  );
  module_load_include('inc', 'ls_answ', 'ls_answ'); // pour ls_answ_create_title()
  $form['ls_answers_options']['answ_title']['suffix']['random_group']['ls_suffix_max'] = array(
    '#type' => 'textfield',
    '#prefix' => t('and'),
    '#title' => t('Maximal number'),
    '#default_value' => $answer_options['ls_suffix_max'],
    '#size' => '5',
    '#maxlength' => '10',
    '#suffix' => '</div>', // End of the "form-radios" style and the div container.
  );
  // Answer title example
  if (module_exists('ls_answ')) {
    $answ_type_title = ($node_type->module != 'ls_survey') ? $answ_type : $answ_type_name;
    $form['ls_answers_options']['answ_title']['suffix']['ls_suffix_example'] = array(
      '#type' => 'markup',
      '#markup' => t('For example : %title', array('%title' => ls_answ_create_title($answ_type_title))),
    );
  }
  $form['ls_answers_options']['answ_title']['ls_disabled_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('The responses title can not be changed'),
    '#default_value' => (!empty($sid) && $survey_properties['anonymized']) ? 1 : $answer_options['ls_disabled_title'],
    '#return_value' => 1,
  );

  $form['ls_answers_options']['answ_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options for the response display page'),
    '#description' => t('Choose how datas will be displayed on the response page'),
    '#collapsible' => 1,
    '#collapsed' => 1,
  );
  $ls_import_lang_select = array();
  if (!empty($sid)) {
    module_load_include('inc', 'ls_survey', 'ls_survey');
    foreach (array_keys($survey_properties['#lang']) as $ls_id_lang) {
      $ls_import_lang_select[limesurvey_sync_lang_convert($ls_id_lang)] = t('All in @lang (translate existing answers)', array('@lang' => ls_survey_display_sync_fields('ls_lang', $ls_id_lang)));
    }
    asort($ls_import_lang_select);
    $ls_default_lang_option = t("The survey language : @lang (recommanded)", array('@lang' => ls_survey_display_sync_fields('ls_lang', $lang)));
  }
  else {
    $ls_default_lang_option = t('The LimeSurvey survey language');
  }
  // Add 1 line at the top :
  $ls_import_lang_select = array_merge(array('' => $ls_default_lang_option), $ls_import_lang_select);
  $import_lang_description = t('As an example, you can provide a japonese survey for your japonese end users and read their responds in english on the Drupal site.');
  $import_lang_description .= '<br />' . t('For such feature, you need to :');
  $import_lang_list = array();
  $import_lang_list[] = t('!enable the Locale core module,', array('!enable' => (module_exists('locale')) ? l(t('enable'), 'admin/modules') : t('enable')));
  $import_lang_list[] = t("on the current page ('Publishing options' tab) set 'Multilingual support' to 'enabled',");
  $import_lang_list[] = t('!install and enable the appropriate languages,', array('!install' => (module_exists('locale')) ? l(t('install'), 'admin/config/regional/language') : t('install')));
  $import_lang_list[] = t('!download translations,', array('!download' => (module_exists('locale')) ? l(t('download'), 'http://drupal.org/project/l10n_update') : t('download')));
  $import_lang_list[] = t('!translate the LimeSurvey Sync module for the few missing strings (if any).', array('!translate' => (module_exists('locale')) ? l(t('translate'), 'admin/config/regional/translate/translate') : t('translate')));
  $import_lang_description .= theme('item_list', array('items' => $import_lang_list, 'type' => 'ol'));
  $form['ls_answers_options']['answ_options']['ls_import_lang'] = array(
    '#type' => 'select',
    '#title' => t('Choose the display language for answers'),
    '#description' => $import_lang_description,
    '#default_value' => $answer_options['ls_import_lang'],
    '#options' => $ls_import_lang_select,
  );
  $form['ls_answers_options']['answ_options']['ls_only_answered'] = array(
    '#type' => 'checkbox',
    '#title' => t('Print only responded questions'),
    '#description' => t('Usefull for large surveys'),
    '#default_value' => $answer_options['ls_only_answered'],
    '#return_value' => 1,
  );
  $form['ls_answers_options']['answ_options']['ls_merge'] = array(
    '#type' => 'checkbox',
    '#title' => t('Merge question groups, questions and subquestion for greater readability'),
    '#default_value' => $answer_options['ls_merge'],
    '#return_value' => 1,
  );
  $form['ls_answers_options']['answ_options']['ls_ignore_x'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore "Text display" question type, also called "boilerplate questions"'),
    '#default_value' => $answer_options['ls_ignore_x'],
    '#return_value' => 1,
  );
  $ls_fields = array(
    'id' => t('Response ID'),
    'submitdate' => t('Date submitted'),
    'lastpage' => t('Last page'),
    'startlanguage' => t('Start language'),
    'token' => t('Token'),
  );

  if ($node_type->module != 'ls_survey' && module_exists('ls_answ')) {
    $ls_fields_date = $ls_fields_ipaddr = $ls_fields_refurl = array();
    if ($survey_properties['date_answer']) {
      $ls_fields_date = array(
        'datestamp' => t('Date last action'),
        'startdate' => t('Date started'),
      );
    }
    if ($survey_properties['ipaddr']) {
      $ls_fields_ipaddr = array(
        'ipaddr' => t('IP address'),
      );
    }
    if ($survey_properties['refurl']) {
      $ls_fields_refurl = array(
        'refurl' => t('Referrer URL'),
      );
    }
    $ls_fields = array_merge($ls_fields, $ls_fields_date, $ls_fields_ipaddr, $ls_fields_refurl);
  }

  $form['ls_answers_options']['answ_options']['ls_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('LimeSurvey fields to be displayed'),
    '#default_value' => $answer_options['ls_fields'],
    '#options' => $ls_fields,
  );

  $title_filter = t('Choose default format filter');
  if (user_access('administer filters')) {
    module_load_include('module', 'filter');
    // Overview of all formats.
    $options_filter = array();
    $compatible_formats = ls_survey_type_compatible_filters();
    global $user;
    $formats = filter_formats($user);
    foreach ($formats as $id => $format) {
      $args = array(
        '@format_name' => $format->name,
        '@format_code' => $id,
        '@url' => url('admin/config/content/formats/' . $id),
      );
      if (!in_array($id, $compatible_formats)) {
        $options_filter[$id] = t('@format_name (<a href="@url">@format_code</a>) <em>[Not recommanded]</em>', $args);
      }
      else {
        $options_filter[$id] = t('@format_name (<a href="@url">@format_code</a>)', $args);
      }
    }
    $form['ls_answers_options']['answ_options']['ls_format'] = array(
      '#type' => 'radios',
      '#title' => $title_filter,
      '#default_value' => $answer_options['ls_format'],
      '#description' => t('The <a href="@url">input format</a> should display HTML text or at least the following tags : @tags.', array('@url' => url('admin/config/content/formats'), '@tags' => ls_survey_type_compatible_tags(TRUE))),
      '#options' => $options_filter,
    );
  }
  else {
    $form['ls_answers_options']['answ_options']['ls_format'] = array(
      '#type' => 'value',
      '#value' => $answer_options['ls_format'],
    );
    $form['ls_answers_options']['answ_options']['ls_format_display'] = array(
      '#type' => 'markup',
      '#prefix' => '<p>',
      '#markup' => '<strong>' . $title_filter . '</strong><br />--' . t('This option needs administrator access rights.') . '--',
      '#suffix' => '</p>',
    );
  }

  $form['ls_answers_options']['sync_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options for the response synchronization'),
    '#description' => t('The synchronization process imports the LimeSurvey responses into Drupal nodes'),
    '#collapsible' => 1,
    '#collapsed' => 1,
  );
  $auto_import_description = ($node_type->module != 'ls_survey') ? t('Or import answers manually on the <a href="@url">survey synchronization page</a>.', array('@url' => url('node/' . $node_survey->nid . '/ls-import'))) : t('Or import answers manually on the survey synchronization page.');
  $form['ls_answers_options']['sync_options']['ls_auto_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic import answer after completion'),
    '#description' => $auto_import_description,
    '#default_value' => $answer_options['ls_auto_import'],
    '#return_value' => 1,
  );
  $form['ls_answers_options']['sync_options']['ls_author'] = array(
    '#type' => 'textfield',
    '#title' => t('Set a specific user as author for the manually imported answers'),
    '#maxlength' => 60,
    '#size' => 30,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $answer_options['ls_author'],
    '#description' => t('Leave it blank for the current user, or 0 for Anonymous user.'),
  );
  $form['ls_answers_options']['sync_options']['ls_standing_by_status'] = array(
    '#type' => 'checkbox',
    '#title' => t("Adapt the response status depending on the answer is completed or is being edited"),
    '#description' => t("Automatic switch to the 'unpublished' status during the edition process and switch back to the 'published' status when the response has been completed."),
    '#default_value' => $answer_options['ls_standing_by_status'],
    '#return_value' => 1,
  );
  $form['ls_answers_options']['sync_options']['ls_export_token'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fill the token table of the LimeSurvey database with corresponding usernames and emails'),
    '#description' => t('Users names and emails from this site will be available into the LimeSurvey results.'),
    '#default_value' => $answer_options['ls_export_token'],
    '#return_value' => 1,
  );
  $form['ls_answers_options']['sync_options']['ls_keep_revisions'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't delete the answer revision entries store into the LimeSurvey site database"),
    '#description' => t('Keeping the answer revision entries allow you to switch back to this revision and being able to edit it again. They will be mark as "uncompleted reponses" into the LimeSurvey site user interface. you will need so to select "include completed answers only" to ignore them into the statitics.'),
    '#default_value' => $answer_options['ls_keep_revisions'],
    '#return_value' => 1,
  );
  $form['ls_answers_options']['sync_options']['ls_revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always create revision on synchronization process'),
    '#default_value' => $answer_options['ls_revision'],
    '#return_value' => 1,
  );
  $form['ls_answers_options']['sync_options']['ls_recycling'] = array(
    '#type' => 'checkbox',
    '#title' => t('Recycling virgin responses for the same user'),
    '#description' => t('Virgin answers may be generated when the LimeSurvey form is displayed but the user did not submit the first page.'),
    '#default_value' => $answer_options['ls_recycling'],
    '#return_value' => 1,
  );

  $form['ls_answers_options']['ls_options_last_import'] = array(
    '#type' => 'value',
    '#value' => $answer_options['ls_options_last_import'],
  );

  // The 'ls_survey_answ_options_submit()' function will be moved by the hook
  // form alter in order to be executed before the node_type_form_submit()
  // function.
  $form['#submit'][] = 'ls_survey_answ_options_submit';
  if ($node_type->module == 'ls_answ') {
    // Add specific redirection on the survey node.
    $form['#submit'][] = 'ls_survey_answ_options_redirect';
  }

  // The 'reset to default' button has been removed from D6 to D7 into the
  // node_type_form() function, we add it here.
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 50,
  );

  return $form;
}

/**
 * Validate the answers content type options.
 */
function ls_survey_answ_options_validate($form, &$form_state) {

  $node_type = $form['#node_type'];
  $frames = array('iframe', 'colorbox', 'lightbox2', 'modalframe');
  $frames_set = in_array(trim($form_state['values']['ls_redirect']), $frames);
  $sid = $answ_type = FALSE;
  $variable_name = ($node_type->module == 'ls_survey') ? 'ls_survey_options' : 'ls_answ_options_' . check_plain($form_state['values']['old_type']);
  $old_settings = variable_get($variable_name, array());
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
  // @TODO : I'm not sure the ls_survey.inc file is needed here.
  module_load_include('inc', 'ls_survey', 'ls_survey');

  if ($node_type->module != 'ls_survey' && module_exists('ls_answ')) {
    // Find the survey ID, answers content type and the correct url.

    // Load file for the ls_answ_find_survey() function.
    module_load_include('inc', 'ls_answ', 'ls_answ');
    $node_survey = ls_answ_find_survey($node_type->type);
    // Load survey datas.
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    $survey_properties = limesurvey_sync_survey_properties($node_survey->ls_sid);
    $sid = $node_survey->ls_sid;
    $answ_type = $node_type->type;

    // Create the correct url : for iframe, no_iframe and actual.
    // Adapt if the type has changed.
    $current_type = trim($form_state['values']['type']);
    $path = 'backfromsurvey/' . $current_type;
    $correct_back_url_no_iframe = url($path, array('absolute' => TRUE));
    $correct_back_url_iframe = $correct_back_url_no_iframe . '/back';
    $correct_back_url = ($frames_set) ? $correct_back_url_iframe : $correct_back_url_no_iframe;

    // Format the actual back url.
    $ls_settings = variable_get('ls_settings', array('ls_path' => ''));
    $url_ls = limesurvey_sync_rel2abs($survey_properties['#lang'][$node_survey->ls_lang]['back_url'], limesurvey_sync_rel2abs($ls_settings['ls_path'] . '/'));
    // Handle back url from LS, comparing to the drupal url() version.
    $actual_back_url = limesurvey_sync_drupal_formated_url($url_ls);

    if ($answ_type != $current_type) {
      // Rebuild the form to update the end URL on the 'answ_redirect'
      // field description. The form will be rebuild only if no error occurs.
      $form_state['rebuild'] = TRUE;
    }

  }

  // Validate all fields one by one.
  $keys = ls_survey_answ_default_options();
  foreach ($form_state['values'] as $key => $value) {
    if (array_key_exists($key, $keys)) {
      $tested_value = (!is_array($value)) ? trim($value) : $value;
      $condition = TRUE;
      $datas = $form;
      $single_radio = FALSE;

      // Find fieldset :
      switch ($key) {
        case 'ls_options_last_import':
          $datas = $form['ls_answers_options'];
          break;
        case 'iframe_dim':
          // Values can be 'custom' or 'auto' or 'invisible'.
          $datas = ($tested_value == 'auto' || $tested_value == 'invisible') ?  $form['ls_answers_options']['answ_redirect']['iframe_dim'][$tested_value] : $form['ls_answers_options']['answ_redirect']['iframe_dim']['iframe_dim_group']['cutom'];
          $single_radio = TRUE;
          break;
        case 'iframe_dim_width':
        case 'iframe_dim_height':
          $datas = $form['ls_answers_options']['answ_redirect']['iframe_dim']['iframe_dim_group'];
          break;
        case 'ls_redirect':
        case 'ls_overwrite_endurl':
        case 'ls_hide_lang':
        case 'ls_validuntil':
          $datas = $form['ls_answers_options']['answ_redirect'];
          break;
        case 'ls_end':
          // Values can be 'own' or 'page'.
          $datas = ($tested_value == 'page') ? $form['ls_answers_options']['answ_redirect']['ls_end']['page_group']['page'] : $form['ls_answers_options']['answ_redirect']['ls_end']['own'];
          $single_radio = TRUE;
          break;
        case 'ls_end_url':
          $datas = $form['ls_answers_options']['answ_redirect']['ls_end']['page_group'];
          break;
        case 'ls_prefix':
        case 'ls_disabled_title':
          $datas = $form['ls_answers_options']['answ_title'];
          break;
        case 'ls_suffix_method':
          // Values can be 'numeric' or 'random'.
          $datas = ($tested_value == 'random') ? $form['ls_answers_options']['answ_title']['suffix']['random_group']['random'] : $form['ls_answers_options']['answ_title']['suffix']['numeric'];
          $single_radio = TRUE;
          break;
        case 'ls_suffix_min':
        case 'ls_suffix_max':
          $datas = $form['ls_answers_options']['answ_title']['suffix']['random_group'];
          break;
        case 'ls_import_lang':
        case 'ls_only_answered':
        case 'ls_merge':
        case 'ls_ignore_x':
        case 'ls_fields':
        case 'ls_format':
          $datas = $form['ls_answers_options']['answ_options'];
          break;
        case 'ls_auto_import':
        case 'ls_author':
        case 'ls_standing_by_status':
        case 'ls_export_token':
        case 'ls_keep_revisions':
        case 'ls_revision':
        case 'ls_recycling':
          $datas = $form['ls_answers_options']['sync_options'];
          break;
      }

      $element = (!$single_radio) ? $datas[$key] : $datas;

      // Check if datas are valid.
      switch ($element['#type']) {
        case 'checkbox':
          $condition = in_array($tested_value, array('', 0, 1));
          if ($key == 'ls_disabled_title' && $tested_value && empty($form_state['values']['ls_prefix'])) {
            form_set_error($key, t('You can not disable an empty title.'));
          }
          elseif ($key == 'ls_disabled_title' && !$tested_value && !empty($sid) && $survey_properties['anonymized']) {
            form_set_error($key, t('For privacy raison, you can not change the answers title on anonymized surveys.'));
          }
          break;
        case 'select':
        case 'radios':
          $condition = array_key_exists($tested_value, $element['#options']);
          break;
        case 'checkboxes':
          if (!is_array($tested_value)) {
            $condition = FALSE;
          }
          else {
            $condition = (array_diff_key($tested_value, $element['#options']) == array());
          }
          break;
        case 'radio':
          switch ($key) {
            case 'iframe_dim':
              $availables = array('custom', 'auto', 'invisible');
              break;
            case 'ls_end':
              $availables = array('own', 'page');
              break;
            case 'ls_suffix_method':
              $availables = array('random', 'numeric');
              break;
            default:
              $availables = array();
          }
          if (!in_array($tested_value, $availables)) {
            $condition = FALSE;
          }
          break;
      }

      // Specific validations :
      if ($key == 'ls_options_last_import') {
        $condition = (empty($tested_value) || is_numeric($tested_value));
      }

      // Handle the frame dimension validation.
      if ($key == 'iframe_dim' && $tested_value == 'invisible' && $form_state['values']['ls_redirect'] != 'iframe') {
        // @TODO : The key paramater does not hightlight if error.
        form_set_error($key, t("The invisible iframe feature is only available for opening the form into an iframe."));
      }
      elseif ($key == 'iframe_dim_width' || $key == 'iframe_dim_height') {
        if (!($form_state['values']['iframe_dim'] != 'custom' && empty($tested_value))) {
          $pattern = '#(^[0-9]{1,4}\.?[0-9]{0,1})[ ]?(%|px)?$#';
          if (!preg_match($pattern, drupal_strtolower($tested_value), $match)) {
            $condition = FALSE;
          }
          else {
            // rewrite the provided datas:
            $num = (float) $match[1];
            $unit = (!empty($match[2])) ? $match[2] : 'px';
            $type_dim = ($key == 'iframe_dim_width') ?  'width' :  'height';
            // Define limit values :
            $dim = array(
              'width' => array(
                '%' => array(
                  'min' => 0,
                  'max' => 100,
                ),
                'px' => array(
                  'min' => 0,
                  'max' => 3000,
                ),
              ),
              'height' => array(
                '%' => array(
                  'min' => 0,
                  'max' => 100,
                ),
                'px' => array(
                  'min' => 0,
                  'max' => 6000,
                ),
              ),
            );
            if ($unit == '%' && ($form_state['values']['ls_redirect'] == 'modalframe' || $form_state['values']['ls_redirect'] == 'lightbox2') && $form_state['values']['iframe_dim'] == 'custom') {
              form_set_error($key, t("The frame module does not support percent parameter, tip a pixel value or '0' for automatically fit it."));
            }
            elseif ($num < $dim[$type_dim][$unit]['min'] || $num > $dim[$type_dim][$unit]['max']) {
              form_set_error($key, t('The frame %dim must be between @dim_min@dim_unit and @dim_max@dim_unit', array(
                '%dim' => $type_dim,
                '@dim_min' => $dim[$type_dim][$unit]['min'],
                '@dim_max' => $dim[$type_dim][$unit]['max'],
                '@dim_unit' => $unit,
              )));
            }
            // Rewrite the provided datas:
            // Store width and height as '88.8%' or '600px'.
            // Keep it as '0' for automatic fit.
            if ($tested_value != '0') {
              $form_state['values'][$key] = ((string) $num) . $unit;
            }
          }
        }
      }

      if ($key == 'ls_suffix_max' || $key == 'ls_suffix_min') {
        if (!($form_state['values']['ls_suffix_method'] != 'random' && empty($tested_value))) {
          if (!is_numeric($tested_value)) {
            $condition = FALSE;
          }
          elseif ($key == 'ls_suffix_min' && $tested_value < 0) {
            $condition = FALSE;
          }
          elseif ($key == 'ls_suffix_max' && is_numeric($form_state['values']['ls_suffix_min']) && $tested_value <= $form_state['values']['ls_suffix_min']) {
            $condition = FALSE;
          }
        }
      }

      if ($key == 'ls_redirect' && $frames_set) {
        if ($tested_value == 'colorbox' && !module_exists('colorbox')) {
          form_set_error($key, t("The '%module' module needs to be installed", array('%module' => 'colorbox')));
        }
        elseif ($tested_value == 'lightbox2' && !module_exists('lightbox2')) {
          form_set_error($key, t("The '%module' module needs to be installed", array('%module' => 'lightbox2')));
        }
        elseif ($tested_value == 'modalframe' && !module_exists('modalframe')) {
          form_set_error($key, t("The '%module' module needs to be installed", array('%module' => 'Modal Frame API')));
        }
      }

      if ($key == 'ls_author' && !empty($tested_value)) {
        $ls_author = db_query("SELECT 1 FROM {users} WHERE name = :name", array(':name' => $tested_value))->fetchField();
        if (!$ls_author) {
          form_set_error($key, t('Invalid user specified'));
        }
      }

      if ($key == 'ls_format') {
        $compatible_formats = ls_survey_type_compatible_filters();
        if (!$tested_value) {
          form_set_error($key, t('You have to choose a filter format'));
        }
        elseif ($tested_value != $old_settings['ls_format'] && !user_access('administer filters')) {
          form_set_error($key, t('You are not not allowed to modify the filter format'));
        }
        if (!in_array($tested_value, $compatible_formats)) {
          // Don't break the validation but display a warning message.
          drupal_set_message(t("The choosen format filter is not recommanded, you should choose another one."), 'warning');
        }
      }

      if ($key == 'ls_hide_lang' && $tested_value && !module_exists('locale')) {
        form_set_error($key, t("The '%module' module needs to be installed", array('%module' => 'Locale')));
      }

      if ($key == 'ls_import_lang' && !empty($tested_value)) {
        if (module_exists('locale')) {
          module_load_include('module', 'locale');
          $language_list = locale_language_list();
          if (!isset($language_list[$tested_value])) {
            // The asked language is not enabled.
            // Don't break the validation but display a warning message.
            drupal_set_message(t("The %lang language is not available on your Drupal site. For best results, pay attention on the help when choosing the 'display language for answers' option.", array('%lang' => ls_survey_language_name($tested_value))), 'warning');
          }
        }
        else {
          // Don't break the validation but display a warning message.
          drupal_set_message(t("For best results, you should enable the Locale core module"), 'warning');
        }
      }

      // Survey properties specific validations.
      if ($sid) {

        if ($key == 'ls_redirect') {
          if ($actual_back_url != $correct_back_url_iframe && $form_state['values']['ls_overwrite_endurl']) {
            // The end url will be overwritten into the submit process.
          }
          elseif ($actual_back_url != $correct_back_url_iframe) {
            if ($frames_set) {
              form_set_error($key, t('In order to automatically close the frame, you should :') . format_string('!do_this', array('!do_this' => ls_survey_type_help_iframe($sid, $current_type, TRUE, $node_survey->ls_lang))));
            }
            else {
              // Don't throw an error message but a warning message:
              drupal_set_message(t('In order to access to advanced features (automatically import answers, redirect the user on survey completion), you should :') . format_string('!do_this', array('!do_this' => ls_survey_type_help_iframe($sid, $current_type, FALSE, $node_survey->ls_lang))), 'warning');
            }
          }
        }

        if ($key == 'ls_suffix_method' && $tested_value == 'numeric' && $survey_properties['anonymized']) {
          form_set_error($key, t('For privacy raison, you can choose only random title method on anonymized surveys.'));
        }

        if ($key == 'ls_auto_import' && $tested_value) {
          if ($survey_properties['anonymized'] || !$survey_properties['date_answer']) {
            form_set_error($key, t('For privacy raison, anonymized or undated surveys can not automatically import answers'));
          }
          elseif ($actual_back_url != $correct_back_url) {
            if (!$form_state['values']['ls_overwrite_endurl']) {
              form_set_error($key, t('In order to automatically import answers, you should :') . format_string('!do_this', array('!do_this' => ls_survey_type_help_iframe($sid, $current_type, $frames_set, $node_survey->ls_lang))));
            }
          }
          if ($survey_properties['tokenanswerspersistence'] && !$survey_properties['tokens_table']) {
            form_set_error($key, t('You need to !create the token table on the LimeSurvey admin interface in order to use the token persistence features', array('!create' => limesurvey_sync_link_to_ls(t('create'), $node_survey->ls_sid, 'editsurveysettings', 'tokens'))));
          }

        }

        if ($key == 'ls_export_token' && $tested_value && !$survey_properties['tokenanswerspersistence']) {
          form_set_error($key, t('Only surveys with token-based response persistence enabled can export users informations'));
        }

        if ($key == 'ls_keep_revisions' && $tested_value) {

          if (!$survey_properties['can_edit_answer']) {
            form_set_error($key, t('Only surveys that can edit answers may generate revisions'));
          }
          elseif (empty($form_state['values']['node_options']['revision'])) {
            form_set_error('node_options', t('You must enable revsionning in order to store the revisions entries'));
          }
        }

        if ($key == 'ls_recycling' && $tested_value && (!$survey_properties['tokenanswerspersistence'] || !$survey_properties['alloweditaftercompletion'])) {
          form_set_error($key, t('Only surveys with token-based response persistence enabled may generate virgin answers'));
        }

      }
      else {
        // This is the survey content type default page : don't allow to store
        // changes which can not be checked, because the default answer content
        // type variable is generated from this one.
        if ($key == 'ls_redirect' && in_array($tested_value, $frames)) {
          form_set_error($key, t('You can not edit the default redirection'));
        }

        if ($key == 'ls_import_lang' && $tested_value) {
          form_set_error($key, t('You can not choose the default import language'));
        }

        if ($key == 'ls_auto_import' && $tested_value) {
          form_set_error($key, t('You can not choose the automatic import feature by default'));
        }

        if ($key == 'ls_export_token' && $tested_value) {
          form_set_error($key, t('You can not choose to export user informations by default'));
        }

        if ($key == 'ls_recycling' && $tested_value) {
          form_set_error($key, t('You can not choose to recycle virgin answers by default'));
        }

        if ($key == 'ls_keep_revisions' && $tested_value) {
          form_set_error($key, t('You can not choose to keep answers revisions by default'));
        }
      }

      if ($key == 'ls_end' && $tested_value == 'own' && !$form_state['values']['ls_auto_import']) {
        form_set_error($key, t("In order to be redirected to the answer view page, you need to enable the 'automatically import answers' feature"));
      }
      if ($key == 'ls_recycling' && $tested_value && !$form_state['values']['ls_validuntil']) {
        form_set_error('ls_validuntil', t('You can not ask for recycling answers with an unlimited expiration time'));
      }

      // Generate unspecific errors if needed.
      if (!$condition) {
        $option = $element['#title'];
        form_set_error($key, t("The '%setting' setting is not valid.", array('%setting' => $option)));
      }
    }
  }
  // Validate non LimeSurvey datas.
  if ($node_type->module != 'ls_survey' && module_exists('ls_answ')) {
    if (module_exists('locale') && isset($form_state['values']['language_content_type']) && $form_state['values']['language_content_type'] == 0) {
      // Throw a warning message if the multilingue support is not enabled.
      $survey_default_lang = (empty($form_state['values']['ls_import_lang'])) ? limesurvey_sync_lang_convert($node_survey->ls_lang) : $form_state['values']['ls_import_lang'];
      $site_default_lang = language_default('language');
      // Make sure this language is enabled.
      if ($survey_default_lang != $site_default_lang) {
        // Suggest to add/enable this language.
        module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
        drupal_set_message(t('Your LimeSurvey answers default display language is %ls_lang but your Drupal default language is %dr_lang.<br />You would probably want to <em>enable multilingual support</em> for that answers content type, in order to automatically set news answers as %ls_lang.',
          array(
          '%ls_lang' => ls_survey_language_name($survey_default_lang),
          '%dr_lang' => language_default('name'),
        )),
          'warning'
        );
      }
    }
  }
}

/**
 * Submit the answers content type options.
 *
 * This function creates or resets the associated answers content type and
 * saves options values into a drupal variable.
 */
function ls_survey_answ_options_submit($form, &$form_state) {

  if (is_numeric($form_state['values']['ls_author']) && $form_state['values']['ls_author'] == 0) {
    $author_uid = 0;
  }
  elseif (!empty($form_state['values']['ls_author'])) {
    $author_uid = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $form_state['values']['ls_author']))->fetchField();
  }
  else {
    $author_uid = '';
  }

  // Save options values and clean the $form_state['values'] array.
  // Initialisation.
  $new_settings = array();
  $default_options = ls_survey_answ_default_options();
  foreach ($form_state['values'] as $key => $value) {
    // Save only options values :
    if (array_key_exists($key, $default_options)) {
      if (!is_array($value)) {
        if ($key == 'ls_author') {
          $value = $author_uid;
        }
        $new_settings[$key] = check_plain($value);
      }
      else {
        $array_data = array();
        foreach ($value as $array_key => $array_value) {
          if (!empty($array_value)) {
            $array_data[] = check_plain($array_key);
          }
        }
        $new_settings[$key] = $array_data;
      }
      // Then delete values from the $form_state['values'] variable to prevent
      // those datas to be saved by the node_type_form_submit() function.
      unset($form_state['values'][$key]);
    }
  }

  // Manage the options variable before updating the node type :
  $node_type = $form['#node_type'];
  // Load appropriate variable, depending on the type.
  // Use "old_type" instead of "type" for the variable name : the new variable
  // name will be updated during ls_survey_node_type() function if needed.
  $variable_name = ($node_type->module == 'ls_survey') ? 'ls_survey_options' : 'ls_answ_options_' . check_plain($form_state['values']['old_type']);
  $old_settings = variable_get($variable_name, array());
  // If "reset to default" is requested, need to overwrite the new_settings.
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Reset to defaults')) {
    $new_settings = $default_options;
    // Import 'ls_options_last_import' into the default value, to make sure
    // the comparison between old and new values will only involve all other
    // properties.
    if (isset($old_settings['ls_options_last_import'])) {
      $new_settings['ls_options_last_import'] = $old_settings['ls_options_last_import'];
    }
  }
  // The array_diff_assoc() function can not manage multidimensional array, so
  // we manage the ls_fields array separatly.
  $new_settings_strings = $new_settings;
  $old_settings_strings = $old_settings;
  unset($new_settings_strings['ls_fields'], $old_settings_strings['ls_fields']);
  $diff_settings = array_diff_assoc($new_settings_strings, $old_settings_strings);
  // Reintroduce the ls_field.
  if ($new_settings['ls_fields'] != $old_settings['ls_fields']) {
    $diff_settings['ls_fields'] = TRUE;
  }

  module_load_include('inc', 'ls_survey', 'ls_survey');
  if ($node_type->module == 'ls_answ') {
    $node_survey = ls_survey_find_survey($node_type->orig_type);
  }

  if (!empty($diff_settings)) {
    // Options settings have changed, they need to be updated.


    // Search for changes imposing to resynchronize all answers.
    // Load the options changes that impose resynchronized answers.
    $options_needing_resync = array(
      'ls_import_lang',
      'ls_only_answered',
      'ls_merge',
      'ls_ignore_x',
      'ls_format',
      'ls_fields',
    );

    if (array_intersect_key($diff_settings, array_flip($options_needing_resync)) != array()) {
      $time = REQUEST_TIME;
      $new_settings['ls_options_last_import'] = $time;
      if ($node_type->module == 'ls_answ') {
        // All answers need to be resynchronized.
        drupal_set_message(t('You should <a href="@url">resynchronize all existing answers</a> in order to apply changes concerning the display page options', array('@url' => url('node/' . $node_survey->nid . '/ls-import'))), 'warning');
      }
    }

    // Save the new settings
    variable_set($variable_name, $new_settings);
    // Watchdog the new settings :
    module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
    limesurvey_sync_custom_watchdog($variable_name, $old_settings, $new_settings, 'ls_survey');

    // And delete answer entries on the LimeSurvey site if needed.
    if ($node_type->module == 'ls_answ') {
      if ($old_settings['ls_keep_revisions'] && !$new_settings['ls_keep_revisions'] && module_exists('ls_answ')) {
        drupal_set_message(t('You must <a href="@url">resynchronize all existing answers</a> in order to delete the obsolet revisions entries on your LimeSurvey site', array('@url' => url('node/' . $node_survey->nid . '/ls-import'))), 'warning');
      }

      // And delete exported user entries on the LimeSurvey site if needed.
      if ($old_settings['ls_export_token'] && !$new_settings['ls_export_token'] && module_exists('ls_answ')) {
        drupal_set_message(t('You must <a href="@url">resynchronize all existing answers</a> in order to delete the exported users informations entries on your LimeSurvey site', array('@url' => url('node/' . $node_survey->nid . '/ls-import'))), 'warning');
        // Processed to the test and updates.
        module_load_include('inc', 'ls_answ', 'ls_answ');
        $on = array("n.type = '%s'" => $form_state['values']['old_type']);
        $token_users_deleted = ls_answ_delete_exported_users($on);
        if ($token_users_deleted) {
          // Because we have used the access bypass, we don't explicit how many
          // answer entries have been updated.
          drupal_set_message(t('Exported user informations have been deleted on the LimeSurvey database.'));
        }
      }
    }
  }

  // Overwrite the LimeSurvey end url.
  if ($node_type->module == 'ls_answ' && !empty($new_settings['ls_overwrite_endurl'])) {
    module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
    // Reset the end url according to the iframe url.
    $survey_properties = limesurvey_sync_survey_properties($node_survey->ls_sid, TRUE);
    limesurvey_sync_save_endurl($node_survey, $survey_properties['#lang'][$node_survey->ls_lang]['correct_back_url']);
  }

  // Deal if reset to default requested.
  if ($op == t('Reset to defaults')) {

    if ($node_type->module == 'ls_answ' && $node_type->modified) {

      module_load_include('inc', 'ls_survey', 'ls_survey');
      $node_survey = ls_survey_find_survey($node_type->orig_type);
      if ($node_survey) {
        $default_type = ls_survey_default_node_type_answ($node_survey);
        // Replace the actual machine name by the orig type.
        // Because the node_type_save() function
        // uses the sql update : where type = ... we declare $info->old_type
        $default_type->old_type = check_plain($form_state['values']['old_type']);

        // We keep the orig_type by the old_type because the
        // node_type_reset() function need it.
        $default_type->orig_type = $default_type->type;

        // Delete "is_new" data for performance raisons. node_type_save
        // function will be call later via node_type_submit() function.
        unset($default_type->is_new);

        // The node_type_reset() function loads the reseted values from the
        // ls_answ_node_info() function. As the ls_answ_node_info() function
        // draws datas from the {node_type} table, we update it now.
        node_type_save($default_type);

        // Refresh the static variable node type
        node_types_clear();
        node_type_get_types();

        // Update $form_state['values']['old_type']
        $form_state['values']['old_type'] = $default_type->type;
      }
    }
  }
}

/**
 * Redirect after submitting the answers content type editing page.
 *
 * Redirect to the associated survey view after submitting the answers content
 * type editing page.
 */
function ls_survey_answ_options_redirect($form, &$form_state) {
  $node_type = $form['#node_type'];
  module_load_include('inc', 'ls_survey', 'ls_survey');
  $node_survey = ls_survey_find_survey($node_type->orig_type);
  if ($node_survey) {
    $form_state['redirect'] = 'node/' . $node_survey->nid;
  }

  // If asked for reset settings, overwrite the 'node_option_type' variable,
  // to delete the "promote" option. Executing here because the
  // node_type_form_submit() function has been executed already. That why we
  // use the type value and not the old_type value.
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Reset to defaults')) {
    variable_set('node_options_' . $form_state['values']['type'], array("status"));
  }
}

/**
 * Display an html help text for setting the end url.
 *
 * @param $sid
 *   A numeric (optional). The survey ID reference. Default is NULL
 * @param $type
 *   A string (optional). The answer content type machine name. Default is
 *   FALSE corresponding to the text : 'your_answers_content_type_value'.
 * @param $iframes
 *   A Boolean (optional). If set to TRUE, display the end url for iframes
 *   feature (add '/back' to the end url). Default is FALSE.
 * @param $lang
 *   A string (optional). The language survey. Default is empty. Use to target
 *   the correct language tab of the LimeSurvey admin Local setting page.
 * @param $new_window
 *   A Boolean (optional). If set to TRUE, open links into a new window.
 *   Default is TRUE.
 *
 * @return
 *   An html string displaying helping text for configuring the end url.
 */
function ls_survey_type_help_iframe($sid = FALSE, $type = FALSE, $iframes = FALSE, $lang = '', $new_window = TRUE) {
  $type_value = ($type) ? $type : 'your_answers_content_type_value';
  $path = 'backfromsurvey/' . $type_value . (($iframes) ? '/back' : '');
  $url_backfromsurvey = url($path, array('absolute' => TRUE));

  // Load file for the limesurvey_sync_link_to_ls() function.
  module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
  $load_url_tx = t('automatically load URL');
  $end_url_tx = t('End URL');
  $link_settings_autoredirect = (!empty($sid)) ? limesurvey_sync_link_to_ls($load_url_tx, $sid, 'editsurveysettings', 'presentation', TRUE, $new_window) : $load_url_tx;
  $link_settings_end_url = (!empty($sid)) ? limesurvey_sync_link_to_ls($end_url_tx, $sid, 'editsurveylocalesettings', limesurvey_sync_localsettings_anchor($sid, $lang), TRUE, $new_window) : $end_url_tx;

  $li = $lii = array();
  $li[] = t('simply check the checkbox : Automatically overwrites the LimeSurvey end url');
  $lii[] = t("'!link' to '%end_url' (or an equivalent relative url),", array('!link' => $link_settings_end_url, '%end_url' => $url_backfromsurvey));
  if ($iframes) {
    $show_autoredirect = TRUE;
    if ($sid) {
      module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_survey');
      $survey_properties = limesurvey_sync_survey_properties($sid);
      if (isset($survey_properties['autoredirect']) && $survey_properties['autoredirect']) {
        $show_autoredirect = FALSE;
      }
    }
    if ($show_autoredirect) {
      $lii[] = t("and '!link when survey complete ?' to <em>Yes</em> (optional).", array('!link' => $link_settings_autoredirect));
    }

  }
  $li[] = array(
    'data' => t("or manually set on your LimeSurvey admin user interface :"),
    'children' => $lii,
  );
  return theme('item_list', array('items' => $li));
}

/**
 * Returns answers node compatible format filters.
 *
 * In order to correctly display an answer node, the node format filter should
 * allow html tags including the <table> tag. This function returns the format
 * filters machine name that match this condition. On a fresh Drupal install,
 * the 'full_htlm' format filter is the only one that match.
 *
 * @return
 *   An array, the machine names compatible filter formats.
 */
function ls_survey_type_compatible_filters() {
  $ls = array();
  // Note : only enabled formats are explored.
  $formats = filter_formats();
  $expected_tags = ls_survey_type_compatible_tags();
  foreach ($formats as $format => $filter) {
    $properties = filter_list_format($format);
    if (isset($properties['filter_html_escape']) && !$properties['filter_html_escape']->status && isset($properties['filter_html'])) {
      // 'Display any HTML as plain text' is unchecked.
      if (!$properties['filter_html']->status) {
        // 'Limit allowed HTML tags' is unchecked.
        $ls[] = $format;
      }
      else {
        // 'Limit allowed HTML tags' is checked.
        $allowed_tags = (isset($properties['filter_html']) && isset($properties['filter_html']->settings) && array_key_exists('allowed_html', $properties['filter_html']->settings)) ? array_map('drupal_strtolower', preg_split('#\s?<|>\s?#', $properties['filter_html']->settings['allowed_html'], -1, PREG_SPLIT_NO_EMPTY)) : array();
        if (array_intersect($expected_tags, $allowed_tags) == $expected_tags) {
          // But the needed tags are allowed.
          $ls[] = $format;
        }
      }
    }
  }
  return $ls;
}

/**
 * Returns answers node compatible html tags.
 *
 * Define the tags the answer node should be allowed to display into the body
 * field in order to correctly display the answer view page.
 *
 * @param $display
 *   A boolean. If FALSE : returns an array; if TRUE : returns a string for
 *   display, to use with t() and @variable. Default is FALSE.
 *
 * @return
 *   An array, the necessary tags or a string for display.
 */
function ls_survey_type_compatible_tags($display = FALSE) {
  $expected_tags = array(
    'table',
    'thead',
    'th',
    'td',
    'tr',
  );
  if (!$display) {
    return $expected_tags;
  }
  $formated_tags = array();
  foreach ($expected_tags as $tag) {
    $formated_tags[] = '<' . $tag . '>';
  }
  return implode(', ', $formated_tags);
}
