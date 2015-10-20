<?php

/**
 * Administration form for keynumbers.
 */
function doccheck_admin_keynumbers_form() {
  $form = array();

  module_load_include('data.inc', 'doccheck');

  $doccheck_languages = doccheck_get_languages();
  $domains = variable_get('doccheck_domains', array());
  $languages = variable_get('doccheck_language', array());
  foreach ($domains as $domain) {
    if (is_array($languages)) {
      $languages = array_filter($languages);
      if (count($languages)) {
        foreach ($languages as $language) {
          $var = "doccheck_keynumber_{$language}_" . drupal_html_class($domain);
          $form[$var] = array(
            '#type' => 'textfield',
            '#title' => t('Keynumber for %lang language on %domain', array(
              '%lang' => $doccheck_languages[$language],
              '%domain' => $domain,
            )),
            '#default_value' => variable_get($var, NULL),
          );
        }

      }
    }
  }

  if ($form) {
    return system_settings_form($form);
  }

  $form['nolang'] = array(
    '#type' => 'markup',
    '#markup' => t('No languages are selected. Go to !doccheck-admin-link and select some.', array(
      '!doccheck-admin-link' => l(t('DocCheck admin page'),
        'admin/config/people/doccheck'),
    )),
  );

  return $form;
}
