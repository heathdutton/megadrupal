<?php

// $Id: $
/**
 * Form builder; Build the User Warn settings form.
 */
function search_api_context_settings_form($form, &$form_state) {
 $form['search_api_context']['html'] = array(
   '#markup' => t('Context blocks are powerful add-ons to the Search API. They allow to display views based on active facets. They also can be used to set the breadcrumb and the page title to give a more navigation-like experience.')
 );
 $form['search_api_context']['search_api_context_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of blocks to provide'),
    '#default_value' => variable_get('search_api_context_num', 2),
    '#description' => t('The number of blocks to provide. Each context block can have its own settings. To enable and configure go to the <a href="!block">block configuration</a>.',
    array("!block", url('admin/structure/block'))),
    '#size' => 4,
  );

//   $form['#submit'][] = 'search_api_facets_paths_settings_form_submit';
  return system_settings_form($form);
}
/*function search_api_facets_paths_settings_form_submit($form, &$form_state) {
  foreach($form_state['values']['search_api_facets_paths'] as $key => $v) {
    $path = trim($v['path']);
    if (strlen($path) == 0 ) {
      unset($form_state['values']['search_api_facets_paths'][$key]);
    }
  }
}
function search_api_facets_paths_search_api_facets_paths_pathtypes() {
  return array(
    "voc-slash-name" => t("vocabulary-name/term-name"),
    "id-name" => t("vid-term-name (vocabulary ID, name)"),
  );
}*/