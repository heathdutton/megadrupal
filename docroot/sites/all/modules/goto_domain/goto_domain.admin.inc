<?php
/**
 * @file
 * Goto Domain Module Adming config form
 */

/**
 * Settings page.
 */
function goto_domain_settings() {
  $form['goto_domain_mapping'] = array(
    '#type' => 'textarea',
    '#title' => t('Goto Domains Mapping'),
    '#default_value' => variable_get('goto_domain_mapping', ''),
    '#description' => t('Original Base Url|Alternate Base URL for redirection <br/>'
        . 'For Example, if you want redirection to the path \'abc\' to happen to http://myCDNdomain.com/abc  <br/>'
        . 'instead of http://myORIGINdomain.com/abc , give  http://myORIGINdomain.com|http://myCDNdomain.com <br/>'
        . 'NO Trailing slashes. Start with http. If you have https as well, make separate entries one for each of http and https'),
  );

  return system_settings_form($form);
}
