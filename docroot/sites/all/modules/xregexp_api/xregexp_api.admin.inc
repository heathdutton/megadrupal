<?php
function xregexp_api_settings_form($form, $form_state) {
  $lib = libraries_detect('xregexp');
  if ($lib && $lib['installed']) {
    $form['xregexp_api_include'] = array(
      '#title' => t('Include Library'),
      '#description' => t('You can choose not to include the XRegExp library
        (and only include the plugins) when it\'s loaded. This can be useful when the library is
        included by an other module (For example: The XRegExp library is baked
        into the syntaxhighlighter plugin).'),
      '#type' => 'radios',
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
      '#default_value' => variable_get('xregexp_api_include', 1),
    );
    $form['xregexp_api_plugins'] = array(
      '#type' => 'checkboxes',
      '#title' => t('We found the following addons, check the ones you want to use'),
      '#options' => _xregexp_api_plugins(),
      '#default_value' => variable_get('xregexp_api_plugins', array()),
    );
    $form = system_settings_form($form);
  }
  else {
    $form['not_installed'] = array(
      '#type' => 'item',
      '#title' => t('Not installed'),
      '#markup' => t('XRegExp is not installed, please download it and extract it to sites/all/libraries/xregexp.'),
    );

    $links[] = array(
      'title' => 'XRegExp v2.0.0 without plugins',
      'href' => 'http://xregexp.com/xregexp.js',
    );
    $links[] = array(
      'title' => 'minified XRegExp v2.0.0 without plugins',
      'href' => 'http://xregexp.com/xregexp-min.js',
    );
    $links[] = array(
      'title' => 'XRegExp v2.0.0 with all plugins in a single file',
      'href' => 'http://xregexp.com/xregexp-all.js',
    );
    $links[] = array(
      'title' => 'minified XRegExp v2.0.0 all plugins in a single file',
      'href' => 'http://xregexp.com/xregexp-all-min.js',
    );
    $links[] = array(
      'title' => 'XRegExp v2.0.0 including all plugins a seperate files',
      'href' => 'https://github.com/slevithan/XRegExp/zipball/v2.0.0',
    );
    $links[] = array(
      'title' => 'XRegExp v1.5.1 without plugins',
      'href' => 'http://xregexp.com/old/xregexp_1.5.1.js',
    );
    // @see https://github.com/slevithan/XRegExp/issues/19#issuecomment-5868136
    $links[] = array(
      'title' => 'XRegExp v1.5.1 including all plugins a seperate files',
      'href' => 'https://github.com/slevithan/XRegExp/zipball/v1.5.1',
    );

    $form['downloads'] = array(
      '#type' => 'item',
      '#title' => t('Possible downloads'),
      '#markup' => theme('links', array('links' => $links)),
    );
  }
  return $form;
}
