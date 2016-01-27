<?php

/**
 * @file
 * The main logic for the administrative page of the ext_link_page
 * module
 */

/**
 * Admin Settings Page for ext_link_page
 */
function ext_link_page_admin_settings() {
  $options = ext_link_page_get_options();

  // put together form elements
  $form['ext_link_page_direct_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Direct Delay'),
    '#default_value' => $options['ext_link_page_direct_delay'],
    '#maxlength' => 4,
    '#description' => t('The delay (in seconds) before the user will be redirected to the new site. If the delay is 0, then the user will simply be redirected.'),
  );
  $form['ext_link_page_direct_disable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable automatic redirection'),
    '#default_value' => $options['ext_link_page_direct_disable'],
    '#description' => t('Check this to disable automatic redirection from the exit page.'),
  );
  $form['ext_link_page_link_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Link Page'),
    '#default_value' => $options['ext_link_page_link_page'],
    '#maxlength' => 256,
    '#description' => t('The page that external links will be directed to. Example: !link',
      array('!link' => url($options['ext_link_page_link_page'], array('query' => array('url' => 'http://example.com'))))
    ),
  );
  $form['ext_link_page_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title'),
    '#maxlength' => 256,
    '#default_value' => $options['ext_link_page_page_title'],
    '#description' => t('The title of the external link page.'),
  );
  $form['ext_link_page_page_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Page Message'),
    '#default_value' => $options['ext_link_page_page_message'],
    '#description' => t('The message that will be displayed on the Link Page. You may use the following variables: %var.', array('%var' => '[site_name], [url], [delay], [link]')),
  );
  $form['ext_link_page_new_window'] = array(
    '#type' => 'checkbox',
    '#title' => t('New Window'),
    '#default_value' => $options['ext_link_page_new_window'],
    '#description' => t('If this option is checked, the link that handles external pages will be given: %code, which will cause the link to open in a new page.', array('%code' => 'target="_blank"')),
  );
  $form['ext_link_page_nofollow'] = array(
    '#type' => 'checkbox',
    '#title' => t('No Follow'),
    '#default_value' => $options['ext_link_page_nofollow'],
    '#description' => t('If this option is checked, the link that handles external pages will be given: %code, which will suggest to search engines not to index the link.', array('%code' => 'rel="nofollow"')),
  );
  $form['ext_link_page_use_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('If available, use JavaScript to display a modal dialog window'),
    '#default_value' => $options['ext_link_page_use_js'],
    '#description' => t('If this option is checked, then visitors with a browser that supports JavaScript will see the external link notification as an on-page alert rather than being redirected to another page.'),
  );
  $form['ext_link_page_js'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for the JavaScript modal dialog interface'),
    '#description' => t('With the exception of the checkbox to disable automatic redirection, the following settings if left blank will fall back on the corresponding settings above for the exit page.'),
  );
  $form['ext_link_page_js']['ext_link_page_js_direct_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Direct Delay'),
    '#default_value' => $options['ext_link_page_js_direct_delay'],
    '#maxlength' => 4,
    '#description' => t('The delay (in seconds) before the user will be redirected to the new site. If the delay is 0, then the user will simply be redirected.'),
  );
  $form['ext_link_page_js']['ext_link_page_js_direct_disable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable automatic redirection'),
    '#default_value' => $options['ext_link_page_js_direct_disable'],
    '#description' => t('Check this to disable automatic redirection on the modal from the exit page.'),
  );
  $form['ext_link_page_js']['ext_link_page_js_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Modal Title'),
    '#maxlength' => 256,
    '#default_value' => $options['ext_link_page_js_title'],
    '#description' => t('The title of the modal window.'),
  );
  $form['ext_link_page_js']['ext_link_page_js_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Modal Message'),
    '#default_value' => $options['ext_link_page_js_message'],
    '#description' => t('The message that will be displayed on the modal window. You may use the following variables: %var.', array('%var' => '[site_name], [url], [delay], [link]')),
  );
  $form['ext_link_page_js']['ext_link_page_link_class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS class for external links (requires JavaScript)'),
    '#default_value' => $options['ext_link_page_link_class'],
    '#description' => t('Enter any CSS classes that should be added to external links. This only works for visitors with a browser that supports JavaScript.'),
  );
  $form['ext_link_page_patterns_exclude'] = array(
    '#type' => 'textfield',
    '#title' => t('Exclude external links for matching host names'),
    '#default_value' => $options['ext_link_page_patterns_exclude'],
    '#description' => t('Enter a string or regular expression that if found in the URL host name will exclude it from being handled with an external link page.'),
  );

  return system_settings_form($form);
}
