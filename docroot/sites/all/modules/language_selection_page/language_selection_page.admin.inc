<?php

/**
 * @file
 * The admin page of the language selection page module.
 */

/**
 * The admin page form.
 *
 * @return array
 */
function language_selection_page_admin() {
  module_load_include('inc', 'language_selection_page', 'includes/language_selection_page.helpers');
  module_load_include('install', 'language_selection_page', 'language_selection_page');
  require_once(DRUPAL_ROOT.'/includes/install.inc');

  $requirements = language_selection_page_requirements('');

  foreach($requirements as $requirement) {
    if ($requirement['severity'] == REQUIREMENT_WARNING) {
      drupal_set_message($requirement['value'], 'warning');
    }
    if ($requirement['severity'] == REQUIREMENT_ERROR) {
      drupal_set_message($requirement['value'], 'error');
    }
  }


  $options = array(
    LANGUAGE_SELECTION_PAGE_TEMPLATE_ONLY => 'Template only',
    LANGUAGE_SELECTION_PAGE_TEMPLATE_IN_THEME => t('Template in theme'),
    LANGUAGE_SELECTION_PAGE_BLOCK => 'In a Drupal\'s block',
  );

  $language_none_object = new StdClass();
  $language_none_object->language = LANGUAGE_NONE;

  $form['language_selection_page_path'] = array(
    '#title' => t('Select the path of the Language Selection Page'),
    '#type' => 'textfield',
    '#default_value' => variable_get('language_selection_page_path', 'language_selection'),
    '#description' => t('The path of the page displaying the Language Selection Page'),
    '#required' => TRUE,
    '#size' => 40,
    '#field_prefix' => url(NULL, array('absolute' => TRUE, 'language' => $language_none_object)) . (variable_get('clean_url', 0) ? '' : '?q=')
  );

  $form['language_selection_page_redirect_type'] = array(
    '#title' => t('Select the way the Selection Page should work'),
    '#type' => 'select',
    '#multiple' => FALSE,
    '#default_value' => variable_get('language_selection_page_redirect_type', LANGUAGE_SELECTION_PAGE_TEMPLATE_ONLY),
    '#options' => $options,
    '#description' => t(
      '<ul>
         <li><b>Template only</b>: Display the Language Selection Page template only.</li>
         <li><b>Template in theme</b>: Insert the Language Selection Page body as <i>$content</i> in the current theme.</li>
         <li><b>In a Drupal\'s block</b>: Insert the Language Selection Page in a block <em>Language Selection Block</em>.</li>
       </ul>'
    )
  );

  $form['language_selection_page_blacklisted_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths blacklist'),
    '#default_value' => variable_get('language_selection_page_blacklisted_paths', ''),
    '#size' => 10,
    '#description' => t('Specify on which paths the language selection pages should be circumvented.') . '<br />'
      . t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  if (module_exists('language_cookie')) {
    $form['language_selection_page_js_cookie_redirect'] = array(
      '#title' => t('Enable Javascript-based language cookie redirect.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('language_selection_page_js_cookie_redirect', FALSE),
      '#description' => t('Redirect requests to the language selection page using the language saved in the language cookie of the visitor. This may be useful if you use page caching systems such as Boost or Varnish.'),
    );
  }

  $language_selection_page_template = drupal_html_class(drupal_clean_css_identifier('html--' . variable_get('language_selection_page_path', 'language_selection'))).'.tpl.php';

  $form['language_selection_page_templates'] = array(
    '#title' => t('Templating system'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['language_selection_page_templates']['template'] = array(
    '#title' => 'Template only: ' . $language_selection_page_template,
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['language_selection_page_templates']['template']['content'] = array(
    '#type' => 'textarea',
    '#value' => file_get_contents(drupal_get_path('module', 'system') . '/html.tpl.php'),
    '#description' => t("Copy the content of this file into your theme's template directory. Name it <em>@language_selection_page_template</em>, the variable <em>\$language_selection_page</em> is also available. Read the documentation in the <em>language-selection-page-body.tpl.php</em> file.", array('@language_selection_page_template' => $language_selection_page_template))
  );

  $form['language_selection_page_templates']['theme'] = array(
    '#title' => 'Template in theme: language-selection-page-body.tpl.php or ',
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['language_selection_page_templates']['theme']['content'] = array(
    '#type' => 'textarea',
    '#value' => file_get_contents(drupal_get_path('module', 'language_selection_page') . '/themes/language-selection-page-body.tpl.php'),
    '#description' => 'Copy the content of this file into your theme\'s template directory. Name it <em>language-selection-page-body.tpl.php</em> and be sure to read the documentation in it.'
  );

  $form['language_selection_page_templates']['block'] = array(
    '#title' => 'In a Drupal\'s block: block--language-selection-page--block.tpl.php',
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['language_selection_page_templates']['block']['content'] = array(
    '#type' => 'textarea',
    '#value' => file_get_contents(drupal_get_path('module', 'block') . '/block.tpl.php'),
    '#description' => 'Copy the content of this file into your theme\'s template directory. Name it <em>block--language-selection-page--block.tpl.php</em>. The variable <em>$language_selection_page</em> is also available. Read the documentation in the <em>language-selection-page-body.tpl.php</em> file.'
  );

  drupal_flush_all_caches();

  $form['#validate'][] = 'language_selection_page_admin_validate';

  return system_settings_form($form);
}

function language_selection_page_admin_validate($form, &$form_state) {

  $values = $form_state['values'];

  if (drupal_strlen($values['language_selection_page_path']) > 32) {
    form_set_error('language_selection_page_path', 'The path is invalid. Please use a path with 32 characters or less.');
  }

  $candidate_path = drupal_html_class(drupal_clean_css_identifier($values['language_selection_page_path']));
  $candidate_path = preg_replace('/[^a-z0-9_-]+/', '', $candidate_path);

  // validate the string
  if (preg_match('/[^\w.-]/', $values['language_selection_page_path'])){
    form_set_error('language_selection_page_path', 'The path is invalid. Please use a valid path such this example: ' . $candidate_path);
  }
}
