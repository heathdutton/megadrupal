<?php
/**
 *
 * @file : jquery_twitter_search_admin.inc
 * 
 * This file contains administration functions 
 * and functionality implementation.
 * 
 */

function jquery_twitter_search_admin_page() {
  $header = array();
  $header[] = array('data' => t('Block title'), 'field' => 'title', 'sort' => 'asc');
  $header[] = array('data' => t('Operations'));
  
  $query = db_select('jquery_twitter_search')->extend('PagerDefault')->extend('TableSort');
  $result = $query->fields('jquery_twitter_search')->orderByHeader($header)->limit(50)->execute();
  
  $rows = array();
  
  foreach ($result as $data) {
    
    $row = array();
    $row['data']['title'] = check_plain($data->title);
    
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => "admin/config/services/jquery-twitter-search/edit/" . $data->delta,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => "admin/config/services/jquery-twitter-search/del/" . $data->delta,
    );
    
    $row['data']['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );
    
    $rows[] = $row;
  }
  
  $build['path_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No Twitter search block available. <a href="@link">Add block</a>.', array('@link' => url('admin/config/services/jquery-twitter-search/edit'))),
  );
  $build['path_pager'] = array('#theme' => 'pager');

  return $build;
  
  
  return $output;
}

/**
 * jquery twitter search block creation form.
 * 
 * @param $delta
 *   The id of the block.
 * 
 * @return the form array structre.
 */
function jquery_twitter_search_admin_form($delta = NULL) {
  $consumer_key = variable_get('consumer_key', '');
  if (empty($consumer_key)) {
    drupal_set_message(t('Please configure your Twitter App OAuth keys <a href="@link">OAuth Configration</a>', array('@link' => base_path() . 'admin/config/services/jquery-twitter-search/oauth-configration')), 'error');
  }

  $form = array();

  if (arg(5)) {
    $block = jquery_twitter_search_get_block_info(arg(5));
    $options = unserialize($block->options);
    //print_r($options);die;
  }
  
  $form['jqueryts_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Block Title'),
    '#description' => t('Block title.'),
    '#required' => TRUE,
    '#default_value' => (isset($block->title)) ? $block->title : '' ,
  );
  
  $form['jqueryts_term'] = array(
    '#type' => 'textfield',
    '#title' => t('Term'),
    '#required' => TRUE,
    '#description' => t('Term to search for.'),
    '#default_value' => (isset($options['jqueryts_term'])) ? $options['jqueryts_term'] : '' ,
    '#maxlength' => 100,
  );

  $form['advanced_option'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced option'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['advanced_option']['jqueryts_term_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Term title'),
    '#description' => t('title text to display when frame option is true (default = "term" text) if left empty the title will not appear.'),
    '#default_value' => (isset($options['jqueryts_term_title'])) ? $options['jqueryts_term_title'] : '' ,
    '#maxlength' => 100,
  );

  $form['advanced_option']['jqueryts_term_title_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Term title link'),
    '#description' => t('URL for title link.'),
    '#default_value' => (isset($options['jqueryts_term_title_link'])) ? $options['jqueryts_term_title_link'] : 'null' ,
  );
  
  $form['advanced_option']['jqueryts_anchors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Anchors'),
    '#return_value' => 1,
    '#description' => t('Enable embedded links in tweets.'),
    '#default_value' => (isset($options['jqueryts_anchors'])) ? $options['jqueryts_anchors'] : 1 ,
  );
  
  $form['advanced_option']['jqueryts_anim_out_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('animation out speed'),
    '#description' => t('Speed of animation for top tweet when removed.'),
    '#default_value' => (isset($options['jqueryts_anim_out_speed'])) ? $options['jqueryts_anim_out_speed'] : 2000 ,
    '#maxlength' => 4,
  );

  $form['advanced_option']['jqueryts_nim_in_speed'] = array(
    '#type' => 'textfield',
    '#title' => t('animation in speed'),
    '#description' => t('Speed of scroll animation for moving tweets up.'),
    '#default_value' => (isset($options['jqueryts_nim_in_speed'])) ? $options['jqueryts_nim_in_speed'] : 1000 ,
    '#maxlength' => 4,
  );

  $form['advanced_option']['jqueryts_apply_styles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Anchors'),
    '#return_value' => 1,
    '#description' => t('Apply default css styling or not.'),
    '#default_value' => (isset($options['jqueryts_apply_styles'])) ? $options['jqueryts_apply_styles'] : 0 ,
  );

  $form['advanced_option']['jqueryts_avatar'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use avatar'),
    '#description' => t('Show or hide twitter profile images.'),
    '#return_value' => 1,
    '#default_value' => (isset($options['jqueryts_avatar'])) ? $options['jqueryts_avatar'] : 1 ,
  );

  $form['advanced_option']['jqueryts_bird'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Bird image'),
    '#return_value' => 1,
    '#description' => t('To show or hide twitter bird image.'),
    '#default_value' => (isset($options['jqueryts_bird'])) ? $options['jqueryts_bird'] : 1 ,
  );

  $form['advanced_option']['jqueryts_bird_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Use Bird image link to'),
    '#description' => t('URL that twitter bird image should link to.'),
    '#default_value' => (isset($options['jqueryts_bird_link'])) ? $options['jqueryts_bird_link'] : '' ,
  );

  $form['advanced_option']['jqueryts_bird_src'] = array(
    '#type' => 'textfield',
    '#title' => t('Use Bird image URL'),
    '#description' => t('twitter bird image.'),
    '#default_value' => (isset($options['jqueryts_bird_src'])) ? $options['jqueryts_bird_src'] : 'http://cloud.github.com/downloads/malsup/twitter/tweet.gif' ,
  );

  $form['advanced_option']['jqueryts_colorexterior'] = array(
    '#type' => 'textfield',
    '#title' => t('Color exterior'),
    '#description' => t('CSS override of frame border-color and title background-color (#000000).'),
    '#default_value' => (isset($options['jqueryts_colorexterior'])) ? $options['jqueryts_colorexterior'] : '#DDD' ,
    '#maxlength' => 20,
  );
  
  $form['advanced_option']['jqueryts_colorinterior'] = array(
    '#type' => 'textfield',
    '#title' => t('Color interior'),
    '#description' => t('CSS override of container background-color (#000000).'),
    '#default_value' => (isset($options['jqueryts_colorinterior'])) ? $options['jqueryts_colorinterior'] : 'white' ,
    '#maxlength' => 20,
  );

  $form['advanced_option']['jqueryts_filter'] = array(
    '#type' => 'textfield',
    '#title' => t('Filter'),
    '#description' => t('Callback fn to filter tweets:  fn(tweetJson) { /* return false to skip tweet */ }.'),
    '#default_value' => (isset($options['jqueryts_filter'])) ? $options['jqueryts_filter'] : 'null' ,
    '#maxlength' => 120,
  );

  $form['advanced_option']['jqueryts_pause'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause'),
    '#return_value' => 1,
    '#description' => t('Pause on hover.'),
    '#default_value' => (isset($options['jqueryts_pause'])) ? $options['jqueryts_pause'] : 1 ,
  );

  $form['advanced_option']['jqueryts_refresh_seconds'] = array(
    '#type' => 'textfield',
    '#title' => t('Refresh seconds'),
    '#description' => t('Number of seconds to wait before polling for newer tweets'),
    '#default_value' => (isset($options['jqueryts_refresh_seconds'])) ? $options['jqueryts_refresh_seconds'] : 0 ,
    '#maxlength' => 2,
  );
  
  $form['advanced_option']['jqueryts_time'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the tweet time'),
    '#return_value' => 1,
    '#description' => t('Show or hide the time that the tweet was sent.'),
    '#default_value' => (isset($options['jqueryts_time'])) ? $options['jqueryts_time'] : 1,
  );
  
  $form['advanced_option']['jqueryts_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#description' => t('Delay betweet tweet scroll in milesecond'),
    '#default_value' => (isset($options['jqueryts_timeout'])) ? $options['jqueryts_timeout'] : 4000 ,
    '#maxlength' => 4,
  );

  $form['advanced_option']['block_delta'] = array(
    '#type' => 'hidden',
    '#value' => (isset($block->delta)) ? $block->delta : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  return $form;
}

/**
 *  block creation form handler.
 */
function jquery_twitter_search_admin_form_submit($form, &$form_state) {

  $settings = array();

  foreach ($form_state['values'] as $key => $value) {
    if (preg_match('/jqueryts_/', $key) && $key != 'jqueryts_title') {
      $settings[$key] = $value;
    }
  }
  
  $fields = array(
    'title' => $form_state['values']['jqueryts_title'],
    'options' => serialize($settings),
  );
  
  if ($form_state['values']['block_delta']) {
    db_update('jquery_twitter_search')->fields($fields)->condition('delta', $form_state['values']['block_delta'])->execute();
  } 
  else {
    db_insert('jquery_twitter_search')->fields($fields)->execute();
  }
  
  drupal_set_message(t('Block :block_title have been saved.', array(':block_title' => $form_state['values']['jqueryts_term'])));
  drupal_goto('admin/config/services/jquery-twitter-search/settings'); 
}


/**
 * Page callback for jquery twitter search block deletions.
 */
function jquery_twitter_search_confirm_delete_page($delta = NULL) {
  if ($jqueryts_block = jquery_twitter_search_get_block_info($delta)) {
    return drupal_get_form('jquery_twitter_search_confirm_delete', $jqueryts_block);
  }
  return MENU_NOT_FOUND;
}

/**
 * Form builder; Builds the confirmation form for deleting a single 
 * jquery twitter search block.
 *
 * @ingroup forms
 * @see jquery_twitter_search_confirm_delete_submit()
 */
function jquery_twitter_search_confirm_delete($form, &$form_state, $jqueryts_block) {
  $form['#jqueryts_block'] = $jqueryts_block;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['delta'] = array('#type' => 'value', '#value' => $jqueryts_block->delta);
  return confirm_form(
    $form,
    t('Are you sure you want to delete this block %title?', array('%title' => $jqueryts_block->title)),
    'node/' . $jqueryts_block->delta,
    t('All the configurations of this block will be lost. This action cannot be undone.'),
    t('Delete'),
    t('Cancel'),
    'jquery_twitter_search_confirm_delete');
}

/**
 * Process jquery_twitter_search_confirm_delete form submissions.
 */
function jquery_twitter_search_confirm_delete_submit($form, &$form_state) {
  $jqueryts_block = $form['#jqueryts_block'];
  $jqueryts_block = block_load('jquery_twitter_search', $jqueryts_block->delta);
  $deleted = db_delete('jquery_twitter_search')->condition('delta', $jqueryts_block->delta)->execute();

  drupal_set_message(t('The block and all its configurations have been deleted.'));
  $form_state['redirect'] = "admin/config/services/jquery-twitter-search";
}

function jquery_twitter_search_oauth_admin_form() {
  $form = array();
  
  $form['consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer key'),
    '#required' => TRUE,
    '#default_value' => variable_get('consumer_key', ''),
    '#size' => 70
  );
  $form['consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('consumer_secret', ''),
    '#size' => 70
  );
  $form['access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token'),
    '#required' => TRUE,
    '#default_value' => variable_get('access_token', ''),
    '#size' => 70
  );
  $form['access_token_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('access_token_secret', ''),
    '#size' => 70
  );

  $form['notice'] = array(
    '#markup' => '<div class="description">' . t('Please refer to ' . l(t('https://dev.twitter.com/apps/'), 'https://dev.twitter.com/apps/') . 'to get your aceess keys.') . '</div>',
    '#description' => '',
  );

  return system_settings_form($form);
}


function jquery_twitter_search_twitter_search_form() {
  $form = array();
  //@TODO : clean up this miss.
  $form['notice'] = array(
    '#markup' => '<br />' . t('For more information about these parameter refer to ' . l(t('https://dev.twitter.com/docs/api/1.1/get/search/tweets'), 'https://dev.twitter.com/docs/api/1.1/get/search/tweets') . ' to get your aceess keys.') . '<br />',
  );
  $form['twitter_search_geocode'] = array(
    '#type' => 'textfield',
    '#title' => t('geocode'),
    '#default_value' => variable_get('twitter_search_geocode', ''),
    '#size' => 70,
    '#description' => t('Optional. Returns tweets by users located within a given radius of the given latitude/longitude. The location is preferentially taking from the Geotagging API, but will fall back to their Twitter profile. The parameter value is specified by "latitude,longitude,radius", where radius units must be specified as either "mi" (miles) or "km" (kilometers). Note that you cannot use the near operator via the API to geocode arbitrary locations; however you can use this geocode parameter to search near geocodes directly. A maximum of 1,000 distinct "sub-regions" will be considered when using the radius modifier.<br /><strong>Example Values: </strong>37.781157,-122.398720,1mi'),
  );
  $form['twitter_search_lang'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#default_value' => variable_get('twitter_search_lang', ''),
    '#size' => 70,
    '#description' => t('Optional. Restricts tweets to the given language, given by an ISO 639-1 code. Language detection is best-effort.<br /><strong>Example Values: </strong>eu'),
  );
  $form['twitter_search_locale'] = array(
    '#type' => 'textfield',
    '#title' => t('Local'),
    '#default_value' => variable_get('twitter_search_locale', ''),
    '#size' => 70,
    '#description' => t('Optional. Specify the language of the query you are sending (only ja is currently effective). This is intended for language-specific consumers and the default should work in the majority of cases.<br/><strong>Example Values: </strong>ja'),
  );
  $form['twitter_search_result_type'] = array(
    '#type' => 'radios',
    '#title' => t('Result type'),
    '#options' => array('mixed', 'recent', 'popular'),
    '#default_value' => variable_get('twitter_search_result_type', 'recent'),
    '#size' => 70,
    '#description' => t('Optional. Optional. Specifies what type of search results you would prefer to receive. The current default is "mixed." Valid values include:<ul>
      <li>mixed: Include both popular and real time results in the response.</li>
  <li>recent: return only the most recent results in the response</li>
  <li>popular: return only the most popular results in the response.</li></ul><br />'),
  );
  $form['twitter_search_result_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Result count'),
    '#default_value' => variable_get('twitter_search_result_count', 15),
    '#size' => 70,
    '#description' => t('The number of tweets to return per page, up to a maximum of 100. Defaults to 15. This was formerly the "rpp" parameter in the old Search API.<br /><strong>Example Values: </strong>100'),
  );
  $form['twitter_search_result_until'] = array(
    '#type' => 'textfield',
    '#title' => t('Until'),
    '#default_value' => variable_get('twitter_search_result_until', ''),
    '#size' => 70,
    '#description' => t('Returns tweets generated before the given date. Date should be formatted as YYYY-MM-DD. Keep in mind that the search index may not go back as far as the date you specify here.<br /><strong>Example Values: </strong>2012-09-01'),
  );
  $form['twitter_search_result_since_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Since id'),
    '#default_value' => variable_get('twitter_search_result_since_id', ''),
    '#size' => 70,
    '#description' => t('Returns results with an ID greater than (that is, more recent than) the specified ID. There are limits to the number of Tweets which can be accessed through the API. If the limit of Tweets has occured since the since_id, the since_id will be forced to the oldest ID available.<br /><strong>Example Values: </strong>12345'),
  );
  $form['twitter_search_result_max_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Max id'),
    '#default_value' => variable_get('twitter_search_result_max_id', ''),
    '#size' => 70,
    '#description' => t('Returns results with an ID less than (that is, older than) or equal to the specified ID.<br /><strong>Example Values: </strong>54321'),
  );
  return system_settings_form($form);
}
