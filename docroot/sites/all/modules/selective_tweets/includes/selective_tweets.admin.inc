<?php
/**
 * @file
 * Provides the admin functionality for the Selective Tweets module.
 */

/**
 * Menu callback; Builds the admin form.
 *
 * @return
 *   Renderable array.
 */
function selective_tweets_admin_form($form, &$form_state) {
  $form = array();

  $form['selective_tweets_api_account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter API account'),
    '#description' => t('Fill in the Twitter API account details in order to connect to Twitter.'),
  );

  $form['selective_tweets_api_account']['selective_tweets_api_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter API version'),
    '#max_length' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('selective_tweets_api_version', SELECTIVE_TWEETS_API_VERSION),
    '#description' => t('Change this only if you know what you are doing.'),
  );

  $form['selective_tweets_api_account']['selective_tweets_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Key'),
    '#max_length' => 255,
    '#required' => TRUE,
    '#default_value' => variable_get('selective_tweets_consumer_key', ''),
  );

  $form['selective_tweets_api_account']['selective_tweets_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Secret'),
    '#max_length' => 255,
    '#required' => TRUE,
    '#default_value' => variable_get('selective_tweets_consumer_secret', ''),
  );

  $form['selective_tweets_api_account']['selective_tweets_oauth_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Oauth Token'),
    '#max_length' => 255,
    '#required' => TRUE,
    '#default_value' => variable_get('selective_tweets_oauth_token', ''),
  );

  $form['selective_tweets_api_account']['selective_tweets_oauth_token_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Oauth Token Secret'),
    '#max_length' => 255,
    '#required' => TRUE,
    '#default_value' => variable_get('selective_tweets_oauth_token_secret', ''),
  );

  $form['#submit'][] = 'selective_tweets_admin_form_cache_clear';

  return system_settings_form($form);
}

/**
 * Submit handler; Extra submit handler to clear cached data.
 */
function selective_tweets_admin_form_cache_clear($form, &$form_state) {
  cache_clear_all('selective_tweets', 'cache', TRUE);
  drupal_set_message(t('Cached data of Selective Tweets has been deleted.'));
}

/**
 * Form constructor for the Selective Tweets block configuration form.
 *
 * @param $edit
 *   (optional) An associative array of information retrieved by
 *   selective_tweets_block_load() if an existing block is being edited, or an
 *   empty array otherwise. Defaults to array().
 *
 * @ingroup forms
 */
function selective_tweets_custom_block_form($edit = array()) {
  $edit += array(
    'info' => '',
  );
  $edit += _selective_tweets_block_properties();

  $form_states_embed_mode = array(
    'visible' => array(
      ':input[name="render_mode"]' => array('value' => SELECTIVE_TWEETS_EMBED),
    ),
  );

  $form_states_raw_mode = array(
    'visible' => array(
      ':input[name="render_mode"]' => array('value' => SELECTIVE_TWEETS_RAW),
    ),
  );

  $form['info'] = array(
    '#type' => 'textfield',
    '#title' => t('Block description'),
    '#default_value' => $edit['info'],
    '#maxlength' => 64,
    '#description' => t('A brief description of your block. Used on the <a href="@overview">Blocks administration page</a>.', array('@overview' => url('admin/structure/block'))),
    '#element_validate' => array('element_validate_unique_block_name'),
    '#required' => TRUE,
  );

  $form['render_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Render mode'),
    '#options' => array(
      SELECTIVE_TWEETS_EMBED => t('Render embedded Tweet widgets'),
      SELECTIVE_TWEETS_RAW => t('Render raw HTML, free to theme'),
    ),
    '#default_value' => $edit['render_mode'],
    '#description' => t('Depending on this selection, the settings below will adjust. Go through them carefully. Embedded Tweets are widgets, styled by Twitter. Choose raw HTML to style the Tweets yourself. You can override the raw tweet template file to suit your needs.'),
  );

  $form['selective_tweets_selection_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Selection settings'),
    '#description' => t('Determine which Tweets should be displayed.'),
  );

  $form['selective_tweets_selection_settings']['display_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Twitter feed selection mode'),
    '#options' => array(
      ST_SELECTION_MODE_LIVE => t('Populate feed by most recent Tweets'),
      ST_SELECTION_MODE_FAVORITES => t('Populate feed by Tweets of any timeline favorited by the given Twitter account'),
      ST_SELECTION_MODE_SEARCH => t('Populate feed by Tweets filtered by'),
    ),
    '#default_value' => $edit['display_mode'],
    '#required' => TRUE,
  );

  $form['selective_tweets_selection_settings']['screen_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From Twitter account'),
    '#default_value' => $edit['screen_name'],
    '#description' => t('Twitter account name to load Tweets from.'),
    '#states' => array(
      'required' => array(
        ':input[name="display_mode"]' => array(
          array('value' => ST_SELECTION_MODE_LIVE),
          array('value' => ST_SELECTION_MODE_FAVORITES),
        ),
      ),
      'visible' => array(
        ':input[name="display_mode"]' => array(
          array('value' => ST_SELECTION_MODE_LIVE),
          array('value' => ST_SELECTION_MODE_FAVORITES),
        ),
      ),
    ),
  );

  $form['selective_tweets_selection_settings']['search_help_text'] = array(
    '#type' => 'item',
    '#markup' => t('Notice by Twitter:') . ' <em>' . t('"Please note that Twitter’s search service and, by extension, the Search API is not meant to be an exhaustive source of Tweets. Not all Tweets will be indexed or made available via the search interface."') . '</em><br/>' . t("If you are not getting the desired result, it's possible these Tweets are not indexed by the Twitter search index."),
    '#states' => array(
      'visible' => array(
        ':input[name="display_mode"]' => array('value' => ST_SELECTION_MODE_SEARCH),
      ),
    ),
    '#prefix' => '<div style="margin-left:30px;">',
    '#suffix' => '</div>',
  );

  $form['selective_tweets_selection_settings']['from_account'] = array(
    '#type' => 'textfield',
    '#title' => t('From Twitter accounts'),
    '#default_value' => $edit['from_account'],
    '#description' => t('Fill in accounts separated by spaces. This will gather all Tweets from the given users. Example: "drupal drupalcon"'),
    '#states' => array(
      'visible' => array(
        ':input[name="display_mode"]' => array('value' => ST_SELECTION_MODE_SEARCH),
      ),
    ),
    '#prefix' => '<div style="margin-left:30px;">',
    '#suffix' => '</div>',
  );

  $form['selective_tweets_selection_settings']['to_account'] = array(
    '#type' => 'textfield',
    '#title' => t('To Twitter accounts'),
    '#default_value' => $edit['to_account'],
    '#description' => t('Fill in accounts separated by OR or AND. Separating by AND will gather all Tweets addressed to all given users. Separating by OR will gather all Tweets addressed to one of the given users. Example: "drupal OR drupalcon"'),
    '#states' => array(
      'visible' => array(
        ':input[name="display_mode"]' => array('value' => ST_SELECTION_MODE_SEARCH),
      ),
    ),
    '#prefix' => '<div style="margin-left:30px;">',
    '#suffix' => '</div>',
  );

  $form['selective_tweets_selection_settings']['user_mentions'] = array(
    '#type' => 'textfield',
    '#title' => t('Containing user mentions'),
    '#default_value' => $edit['user_mentions'],
    '#description' => t('Fill in user names separated by OR or AND. Separating by AND will gather all Tweets containing all user mentions. Separating by OR will gather all Tweets containing one of the user mentions. Example: "drupal OR drupalcon"'),
    '#states' => array(
      'visible' => array(
        ':input[name="display_mode"]' => array('value' => ST_SELECTION_MODE_SEARCH),
      ),
    ),
    '#prefix' => '<div style="margin-left:30px;">',
    '#suffix' => '</div>',
  );

  $form['selective_tweets_selection_settings']['hashtags'] = array(
    '#type' => 'textfield',
    '#title' => t('Containing hashtags'),
    '#default_value' => $edit['hashtags'],
    '#description' => t('Fill in hashtags separated by OR or AND. Separating by AND will gather all Tweets containing all hashtags. Separating by OR will gather all Tweets containing one of the hashtags. Example: "Superbowl AND Pizza"'),
    '#states' => array(
      'visible' => array(
        ':input[name="display_mode"]' => array('value' => ST_SELECTION_MODE_SEARCH),
      ),
    ),
    '#prefix' => '<div style="margin-left:30px;">',
    '#suffix' => '</div>',
  );

  $form['selective_tweets_selection_settings']['include_retweets'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include retweets'),
    '#default_value' => $edit['include_retweets'],
    '#description' => t('When checked, retweets will be displayed too.'),
  );

  $form['selective_tweets_visual_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visual settings'),
    '#description' => t('Determine how the Tweets should be displayed.'),
  );

  $form['selective_tweets_visual_settings']['initial_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial number of Tweets'),
    '#default_value' => $edit['initial_count'],
    '#element_validate' => array('element_validate_integer_positive'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['selective_tweets_visual_settings']['count'] = array(
    '#type' => 'textfield',
    '#title' => t('Tweets per loaded page'),
    '#default_value' => $edit['count'],
    '#description' => t('Set to zero to hide the load more link.'),
    '#element_validate' => array('element_validate_integer_zero_or_higher'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['selective_tweets_visual_settings']['insert_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Insert links'),
    '#default_value' => $edit['insert_links'],
    '#description' => t('When checked, hashtags, users and urls will be converted into clickable links.'),
    '#states' => array(
      'visible' => array(
        ':input[name="render_mode"]' => array('value' => SELECTIVE_TWEETS_RAW),
      ),
    ),
  );

  $form['selective_tweets_visual_settings']['theme'] = array(
    '#type' => 'select',
    '#title' => t('Widget color theme'),
    '#options' => array(
      SELECTIVE_TWEETS_LIGHT => t('Light'),
      SELECTIVE_TWEETS_DARK => t('Dark'),
    ),
    '#default_value' => $edit['theme'],
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="render_mode"]' => array('value' => SELECTIVE_TWEETS_EMBED),
      ),
    ),
  );

  $form['selective_tweets_visual_settings']['hide_media'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide media'),
    '#default_value' => $edit['hide_media'],
    '#description' => t('When checked, links in a Tweet are not expanded to photo, video, or link previews.'),
    '#states' => array(
      'visible' => array(
        ':input[name="render_mode"]' => array('value' => SELECTIVE_TWEETS_EMBED),
      ),
    ),
  );

  $form['selective_tweets_visual_settings']['hide_thread'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide thread'),
    '#default_value' => $edit['hide_thread'],
    '#description' => t('When checked, a collapsed version of the previous Tweet in a conversation thread will not be displayed when the Tweet is in reply to another Tweet.'),
    '#states' => array(
      'visible' => array(
        ':input[name="render_mode"]' => array('value' => SELECTIVE_TWEETS_EMBED),
      ),
    ),
  );

  $form['selective_tweets_visual_settings']['maxwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum width of a rendered Tweet'),
    '#field_suffix' => t('pixels'),
    '#default_value' => $edit['maxwidth'],
    '#description' => t('The maximum width of a rendered Tweet in whole pixels. This value must be between 220 and 550 inclusive. A supplied value under or over the allowed range will be returned as the minimum or maximum supported width respectively.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#size' => 10,
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="render_mode"]' => array('value' => SELECTIVE_TWEETS_EMBED),
      ),
    ),
  );

  $form['selective_tweets_visual_settings']['loading_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Loading text'),
    '#default_value' => $edit['loading_text'],
    '#description' => t('This text is displayed in the block while the Tweets are getting loaded. This value can be translated via <em>Translate Interface</em>.'),
    '#required' => TRUE,
  );

  $form['selective_tweets_performance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performance'),
    '#description' => t('Determine how long cached Tweets can be served to save on Twitter API requests.<br><strong>IMPORTANT:</strong> Twitter API calls are limited per 15 minutes. This makes it very important to set a caching period of at least a minute or more if possible.'),
  );

  $form['selective_tweets_performance']['pull_cached_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Time to cache pulled Tweets'),
    '#field_prefix' => t('Retrieve new Tweets from Twitter every'),
    '#field_suffix' => t('minute(s)') . '.',
    '#default_value' => $edit['pull_cached_time'],
    '#description' => t('This time equals the maximum time before a new Tweet may be displayed on this website.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#size' => 6,
    '#required' => TRUE,
  );

  $form['selective_tweets_performance']['stack_cached_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Time to drop Tweet stack'),
    '#field_prefix' => t('Drop cached Tweet stack every'),
    '#field_suffix' => t('minute(s)') . '.',
    '#default_value' => $edit['stack_cached_time'],
    '#description' => t('This time indicates after how many minutes the Tweets in cache are dropped at the moment new Tweets are pulled in (logically this setting would be a plurality of the setting above). This way deleted or unfavorited Tweets cannot be displayed unintentionally from cache. New Tweets are added to this stack as long as it does not get dropped. Set to zero to never drop the cached Tweet stack.'),
    '#element_validate' => array('element_validate_integer_zero_or_higher'),
    '#size' => 6,
    '#required' => TRUE,
  );

  $form['selective_tweets_performance']['tweet_stack_max'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum amount of Tweets in stack'),
    '#field_suffix' => t('Tweets'),
    '#default_value' => $edit['tweet_stack_max'],
    '#description' => t('The maximum amount of Tweets in the cache stack at any given time. When pulling in new Tweets from Twitter, Drupal will never save more Tweets than this amount. The oldest Tweets will be removed as new ones come in. This setting prevents the database from growing too large when adding new Tweets to the cached stack.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#size' => 10,
    '#required' => TRUE,
  );

  $form['selective_tweets_performance']['cache_rendered_tweet'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache rendered Tweets'),
    '#default_value' => $edit['cache_rendered_tweet'],
    '#description' => t('Turn this caching off if you are displaying the "time ago" value in the template file. Otherwise, leave it on for better performance. For the render mode "embedded Tweet widgets", this caching is always turned on automatically.'),
    '#states' => array(
      'visible' => array(
        ':input[name="render_mode"]' => array('value' => SELECTIVE_TWEETS_RAW),
      ),
    ),
  );

  $form['old_info'] = array(
    '#type' => 'hidden',
    '#value' => $edit['info'],
  );

  return $form;
}

/**
 * Saves a user-created Selective Tweets block in the database.
 *
 * @param $edit
 *   Associative array of fields to save.
 * @param $delta
 *   Block ID of the block to save.
 *
 * @return
 *   Always returns TRUE.
 */
function selective_tweets_custom_block_save($edit, $delta) {
  // The serialized 'data' column contains the timeline settings.
  $data = array();
  foreach (_selective_tweets_block_properties() as $property => $value) {
    $data[$property] = $edit[$property];
  }

  $bid = $delta;

  // Save the block configuration.
  $delta = db_update('selective_tweets_block')
    ->fields(array(
      'info' => $edit['info'],
      'data' => serialize($data),
      'pull_cache_expire' => REQUEST_TIME + ($edit['pull_cached_time'] * 60),
      'stack_cache_expire' => REQUEST_TIME + ($edit['stack_cached_time'] * 60),
    ))
    ->condition('bid', $delta)
    ->execute();

  cache_clear_all('selective_tweets::bid_' . $bid . '_tweet_stack', 'cache', FALSE);
  cache_clear_all('selective_tweets::bid_' . $bid . '_rendered_tweets', 'cache', TRUE);
  cache_clear_all('selective_tweets::bid_' . $bid . '_raw_data_tweets', 'cache', TRUE);
  drupal_set_message(t('The cached Tweets of the block %block_name are deleted. This means the loading time will be higher when loading the Tweets for the first time again.', array(
    '%block_name' => $edit['info'],
  )));
}

/**
 * Form constructor for the add block form.
 *
 * @see selective_tweets_add_block_form_validate()
 * @see selective_tweets_add_block_form_submit()
 * @ingroup forms
 */
function selective_tweets_add_block_form($form, &$form_state) {
  module_load_include('inc', 'block', 'block.admin');
  $form = block_admin_configure($form, $form_state, 'selective_tweets', NULL);

  // Other modules should be able to use hook_form_block_add_block_form_alter()
  // to modify this form, so add a base form ID.
  $form_state['build_info']['base_form_id'] = 'block_add_block_form';

  // Prevent block_add_block_form_validate/submit() from being automatically
  // added because of the base form ID by providing these handlers manually.
  $form['#validate'] = array('selective_tweets_add_block_form_validate');
  $form['#submit'] = array('selective_tweets_add_block_form_submit');

  return $form;
}

/**
 * Form validation handler for selective_tweets_add_block_form().
 *
 * @see selective_tweets_add_block_form()
 * @see selective_tweets_add_block_form_submit()
 */
function selective_tweets_add_block_form_validate($form, &$form_state) {
  $unique_description = (bool) db_query_range('SELECT 1 FROM {selective_tweets_block} WHERE info = :info', 0, 1, array(':info' => $form_state['values']['info']))->fetchField();

  if (empty($form_state['values']['info']) || $unique_description) {
    form_set_error('info', t('Ensure that each block description is unique.'));
  }
}

/**
 * Form submission handler for selective_tweets_add_block_form().
 *
 * Saves the new custom Selective Tweets block.
 *
 * @see selective_tweets_add_block_form()
 * @see selective_tweets_add_block_form_validate()
 */
function selective_tweets_add_block_form_submit($form, &$form_state) {
  // The serialized 'data' column contains the timeline settings.
  $data = array();
  foreach (_selective_tweets_block_properties() as $property => $value) {
    $data[$property] = $form_state['values'][$property];
  }

  // Save the block configuration.
  $delta = db_insert('selective_tweets_block')
    ->fields(array(
      'info' => $form_state['values']['info'],
      'data' => serialize($data),
      'pull_cache_expire' => REQUEST_TIME + ($data['pull_cached_time'] * 60),
      'stack_cache_expire' => REQUEST_TIME + ($data['stack_cached_time'] * 60),
    ))
    ->execute();

  // Store block delta to allow other modules to work with new block.
  $form_state['values']['delta'] = $delta;

  // Run the normal new block submission
  // (borrowed from block_add_block_form_submit).
  $query = db_insert('block')->fields(array('visibility', 'pages', 'custom', 'title', 'module', 'theme', 'status', 'weight', 'delta', 'cache'));
  foreach (list_themes() as $key => $theme) {
    if ($theme->status) {
      $query->values(array(
        'visibility' => (int) $form_state['values']['visibility'],
        'pages' => trim($form_state['values']['pages']),
        'custom' => (int) $form_state['values']['custom'],
        'title' => $form_state['values']['title'],
        'module' => $form_state['values']['module'],
        'theme' => $theme->name,
        'status' => 0,
        'weight' => 0,
        'delta' => $delta,
        'cache' => DRUPAL_NO_CACHE,
      ));
    }
  }
  $query->execute();

  $query = db_insert('block_role')->fields(array('rid', 'module', 'delta'));
  foreach (array_filter($form_state['values']['roles']) as $rid) {
    $query->values(array(
      'rid' => $rid,
      'module' => $form_state['values']['module'],
      'delta' => $delta,
    ));
  }
  $query->execute();

  // Store regions per theme for this block.
  foreach ($form_state['values']['regions'] as $theme => $region) {
    db_merge('block')
      ->key(array('theme' => $theme, 'delta' => $delta, 'module' => $form_state['values']['module']))
      ->fields(array(
        'region' => ($region == BLOCK_REGION_NONE ? '' : $region),
        'pages' => trim($form_state['values']['pages']),
        'status' => (int) ($region != BLOCK_REGION_NONE),
      ))
      ->execute();
  }

  drupal_set_message(t('The block has been created.'));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Form constructor for the custom Selective Tweets block deletion form.
 *
 * @param $delta
 *   The unique ID of the block within the context of $module.
 *
 * @see selective_tweets_block_delete_submit()
 */
function selective_tweets_block_delete($form, &$form_state, $delta) {
  $block = block_load('selective_tweets', $delta);
  $selective_tweets_block = selective_tweets_block_load($block->delta);
  $form['info'] = array('#type' => 'hidden', '#value' => $selective_tweets_block['info'] ? $selective_tweets_block['info'] : $selective_tweets_block['title']);
  $form['bid'] = array('#type' => 'hidden', '#value' => $block->delta);

  return confirm_form($form, t('Are you sure you want to delete the block %name?', array('%name' => $selective_tweets_block['info'])), 'admin/structure/block', '', t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for selective_tweets_block_delete().
 *
 * @see selective_tweets_block_delete()
 */
function selective_tweets_block_delete_submit($form, &$form_state) {
  db_delete('selective_tweets_block')
    ->condition('bid', $form_state['values']['bid'])
    ->execute();
  db_delete('block')
    ->condition('module', 'selective_tweets')
    ->condition('delta', $form_state['values']['bid'])
    ->execute();
  db_delete('block_role')
    ->condition('module', 'selective_tweets')
    ->condition('delta', $form_state['values']['bid'])
    ->execute();
  drupal_set_message(t('The block %name has been removed.', array('%name' => $form_state['values']['info'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/block';
  return;
}

/**
 * Helper function to validate integer: 0 or higher.
 */
function element_validate_integer_zero_or_higher($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value || $value < 0)) {
    form_error($element, t('%name must be 0 or higher.', array('%name' => $element['#title'])));
  }
}

/**
 * Helper function to validate block name to be unique.
 */
function element_validate_unique_block_name($element, &$form_state) {
  $old_block_name = $form_state['values']['old_info'];
  $new_block_name = $element['#value'];
  $unique_description = (bool) db_query_range('SELECT 1 FROM {selective_tweets_block} WHERE info = :info', 0, 1, array(':info' => $new_block_name))->fetchField();

  if ($unique_description && $old_block_name != $new_block_name) {
    form_set_error('info', t('Ensure that each block description is unique.'));
  }
}
