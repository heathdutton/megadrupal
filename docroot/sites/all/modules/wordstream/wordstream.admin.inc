<?php

/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function wordstream_admin_settings($form, &$form_state) {
  $wordstream = wordstream_include_api_class();
  $account = 0;
  if ($wordstream) {
	  $ret = $wordstream->getAPICredits();
	  if ($ret->credits_per_month) {
	    $account = 1;
	  }  
  }
  if(!$account) {
  	// remove error message from wordstream_include_api_class()
  	if(isset($_SESSION['messages']['error'])) {
  		unset($_SESSION['messages']['error']);
  	}
    drupal_set_message(t('You are unable to log in to WordStream. Please enter a valid WordStream API username and password.'), 'error');
  }
  //dsm($ret);

  $form['wordstream_username'] = array(
    '#type' => 'textfield',
    '#title' => t('WordStream API username'),
    '#description' => t(''),
    '#default_value' => variable_get('wordstream_username', ''),
    '#description' => t('In order to enable the API tools you will need a WordStream account login (username & password). !wordstream_link.', 
      array(
      '!wordstream_link' => l(t('Get your API account here'), WORDSTREAM_LINK_API_ACCOUNT, array('attributes' => array('target', 'wordstream'))),
    )
    ),
  );
  $form['wordstream_password'] = array(
    '#type' => 'password',
    '#title' => t('WordStream API password'),
    '#description' => t(''),
    '#default_value' => variable_get('wordstream_password', ''),
  );

  $form['wordstream_cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache time'),
    '#description' => t('API results are cached. Use this value to set in seconds the time before checking if the cache data is out of date.'),
    '#default_value' => variable_get('wordstream_cache_time', WORDSTREAM_CACHE_TIME),
  );

  if ($account) {
    $form['wordstream_account_status'] = array(
      '#type' => 'markup',
      '#value' => '<h3>' . t('Account Status') . '</h3>',
    );
    $form['wordstream_remaining_monthly_credits'] = array(
      '#type' => 'item',
      '#title' => t('Remaining montly credits'),
      '#markup' => $ret->remaining_monthly_credits,
    );
    $form['wordstream_credits_per_month'] = array(
      '#type' => 'item',
      '#title' => t('Credits per month'),
      '#markup' => $ret->credits_per_month,
    );
  }

  $form['wordstream_stats_report_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Stats report options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['wordstream_stats_report_options'] = array_merge($form['wordstream_stats_report_options'], wordstream_stats_report_form_options());

  return system_settings_form($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function wordstream_free_keyword_tool_page($keyword = NULL) {
  if (!$embed_id = variable_get('wordstream_free_embed_id', '')) {
    drupal_set_message(
      t('You will need a embed id before you can use the free keyword tool. !admin_link', 
        array(
      '!admin_link' => l(t('Configure setting here'), 'admin/config/wordstream'),
    )
      ), 'error');
    $output = '';
  }
  else {
    $output = '
      <iframe width="673" height="557" scrolling="no" frameborder=0 src="http://freekeywordtool.wordstream.com/themes/keyword_tool_widget_big.html?name=Test&color_b=#01679a&color_t=#ffffff&embed_id=' . $embed_id . '"></iframe>
      <br/><a href="http://www.wordstream.com">Powered by WordStream Internet Marketing Software</a>
    ';
  }
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function wordstream_free_keyword_niche_page($keyword = NULL) {
  if (!$embed_id = variable_get('wordstream_free_embed_id', '')) {
    drupal_set_message(
      t('You will need a embed id before you can use the free keyword tool. !admin_link', 
        array(
      '!admin_link' => l(t('Configure setting here'), 'admin/config/wordstream'),
    )
      ), 'error');
    $output = '';
  }
  else {
    $output = '
    <iframe width="673" height="557" scrolling="no" frameborder=0 src="http://nichefinder.wordstream.com/themes/niche_finder_widget_big.html?name=Test&color_b=#01679a&color_t=#ffffff&embed_id=' . $embed_id . '"></iframe>
    <br/><a href="http://www.wordstream.com">Powered by WordStream Internet Marketing Software</a>
    ';
  }
  return $output;
}

