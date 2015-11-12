<?php
/**
 * @file
 * Simple currency converter configuration.
 */

module_load_include('inc', 'simple_currency_converter', 'includes/simple_currency_converter.currency');

/**
 * Settings form.
 */
function simple_currency_converter_admin_form($form, &$form_state) {

  $feeds = simple_currency_converter_return_submodules();

  $form['scc_feeds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Available Feeds'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if ($feeds) {
    $form['scc_feeds']['scc_feed_primary'] = array(
      '#type' => 'select',
      '#title' => 'Choose the primary feed to use',
      '#options' => array(0 => 'None') + $feeds,
      '#default_value' => variable_get('scc_feed_primary', ''),
    );
  }
  else {
    $form['scc_feeds']['error_message'] = array(
      '#markup' => t('You must enable at least one module that provides a feed on the !link. <br /><em>Westpac</em> and <em>European Central Bank</em> are availble within this module.', array('!link' => l(t('modules page'), 'admin/modules'))),
    );
  }

  if (count($feeds) >= 2) {
    $form['scc_feeds']['scc_feed_secondary'] = array(
      '#type' => 'select',
      '#title' => 'Choose the secondary feed to use',
      '#description' => 'This will be used if the primary feed is unavailable',
      '#options' => array(0 => 'None') + $feeds,
      '#default_value' => variable_get('scc_feed_secondary', ''),
    );
  }
  else {
    $form['scc_feeds']['message'] = array(
      '#markup' => t('You can enable a secondary feed to be used when the primary feed is unavailable on the !link.', array('!link' => l(t('modules page'), 'admin/modules'))),
    );
  }

  if ($feeds) {

    $codes = simple_currency_converter_supply_country_info();

    foreach ($feeds as $key => $feed) {
      $form['scc_feeds']['feed_' . $key] = array(
        '#type' => 'fieldset',
        '#title' => $feed,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $func = $key . '_return_codes';
      $this_feeds_codes = $func();

      $values = '';
      foreach ($this_feeds_codes as $code) {
        $values .= '<li>' . $code . ' => ' . $codes[$code]['name'] . '</li>';
      }

      $values = t('The following currencies are provided:') . '<ul>' . $values . '</ul>';

      $form['scc_feeds']['feed_' . $key]['markup'] = array(
        '#markup' => $values,
      );
    }
  }

  $form['scc_config_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration variables'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $currencies = array_merge(array('none' => t('-- None --')), simple_currency_converter_supply_country_codes_options());
  $form['scc_config_defaults']['scc_default_conversion_currency'] = array(
    '#type' => 'select',
    '#options' => $currencies,
    '#title' => t('Default conversion currency'),
    '#description' => t('Choose a default currency to use for conversion. <br />Note: This is the currency that is initially used on the page.'),
    '#default_value' => variable_get('scc_default_conversion_currency', 'none'),
  );

  $form['scc_config_defaults']['scc_currency_rate_cookie_expiration'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency rate cookie expiration'),
    '#description' => t('How many seconds should the currency conversion value last for, before refreshing?'),
    '#default_value' => variable_get('scc_currency_rate_cookie_expiration', 1800),
  );

  $form['scc_config_defaults']['scc_user_cookie_expiration'] = array(
    '#type' => 'textfield',
    '#title' => t('Users prefered currency cookie expiration'),
    '#description' => t('How many days should the user preference last for?'),
    '#default_value' => variable_get('scc_user_cookie_expiration', 14),
  );

  $form['scc_config_defaults']['scc_modal_window_trigger'] = array(
    '#type' => 'textfield',
    '#title' => t('Modal Window Activation Selector'),
    '#description' => t('Enter a css selector to use to make the converter modal window scroll down and up.'),
    '#default_value' => variable_get('scc_modal_window_trigger', '.scc_modal_window_trigger'),
  );

  $form['scc_config_defaults']['scc_modal_window_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Form holder element ID'),
    '#description' => t('Enter a css ID to use for placement of the converter dropdown, where users can choose a currency'),
    '#default_value' => variable_get('scc_modal_window_id', 'scc_modal_window_id'),
  );

  $form['scc_config_defaults']['scc_modal_window_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Modal Window Title'),
    '#description' => t('Clicking on element with this ID will close the form'),
    '#default_value' => variable_get('scc_modal_window_title', 'Select your currency'),
  );

  $form['scc_config_defaults']['scc_element_to_convert'] = array(
    '#type' => 'textfield',
    '#title' => t('Price elements to convert'),
    '#description' => t('A css selector for the currency to convert.'),
    '#default_value' => variable_get('scc_element_to_convert', '.scc_price'),
  );

  $disclaimer = <<<HTML
    <h5>Pricing terms</h5>
    <p>The price is based on current exchange rates but is only an approximation. Please contact us for a final price</p>
HTML;

  $form['scc_config_defaults']['scc_disclaimer_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Disclaimer'),
    '#description' => t('Disclaimer to display in the modal window'),
    '#default_value' => variable_get('scc_disclaimer_text', $disclaimer),
  );

  return system_settings_form($form);
}

/**
 * A list of module that extends this module.
 */
function simple_currency_converter_return_submodules() {
  return module_invoke_all('simple_currency_converter_feed', array());
}
