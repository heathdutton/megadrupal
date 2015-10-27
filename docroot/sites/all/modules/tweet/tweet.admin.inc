<?php

/**
 * @file
 *   Builds links to post pages to twitter.
 */

/**
 * Settings page.
 */
function tweet_admin($form, &$form_state) {
  $form['tweet_node'] = array(
    '#type' => 'select',
    '#title' => t('Type of link to show on nodes'),
    '#default_value' => variable_get('tweet_node', 'icon'),
    '#options' => array('icon' => 'icon', 'icon_text' => 'icon_text', 'text' => 'text', 'none' => 'none'),
  );
  $form['tweet_teaser'] = array(
    '#type' => 'select',
    '#title' => t('Type of link to show on teasers'),
    '#default_value' => variable_get('tweet_teaser', 'none'),
    '#options' => array('icon' => 'icon', 'icon_text' => 'icon_text', 'text' => 'text', 'none' => 'none'),
  );
  $form['tweet_new_window'] = array(
    '#type' => 'radios',
    '#title' => t('Open Twitter'),
    '#default_value' => variable_get('tweet_new_window', 'target'),
    '#options' => array(0 => t('In same window'), 'target' => t('In new window with target="_blank" (not XHTML 1.0 Strict compliant)'), 'js' => t('In new window with JavaScript')),
  );
  $node_types = variable_get('tweet_types', array());
  //If all types are selected, un-select them, because the system will still save the result as all selected and it looks better.
  if ($node_types == _tweet_node_types()) {
    $node_types = array();
  }
  $form['tweet_types'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Node types on which to display link'),
    '#description' => t('If no types are selected, the link will appear on all types.  To stop links from appearing on all nodes, choose "none" in the teaser and node display options above.'),
    '#default_value' => $node_types,
    '#options' => _tweet_node_types(),
  );
  if (module_exists('taxonomy')) {
    $options = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $key => $vocabulary) {
      $options[$key] = check_plain($vocabulary->name);
    }
    $form['tweet_node_vocabs'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('Include taxonomy terms from these vocabularies as #hashtags in tweets when used in the current node'),
      '#default_value' => variable_get('tweet_node_vocabs', array()),
      '#options' => $options,
    );
  }
  $options = array();
  foreach (tweet_sites() as $site => $data) {
    $options[$site] = $site;
  }
  if (count($options) > 1) {
    $form['tweet_sites'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#required' => TRUE,
      '#title' => t('Sites'),
      '#description' => t('Display links to these sites from the Twitter API family.'),
      '#default_value' => variable_get('tweet_sites', array('Twitter' => 'Twitter')),
      '#options' => $options,
    );
  }
  $path = drupal_get_path('module', 'tweet');
  $form['tweet_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter image'),
    '#description' => t('The location of the icon to use with the "Post to Twitter" link, relative to your Drupal installation. Ex.: @location', array('@location' => $path . '/twitter.png')),
    '#default_value' => variable_get('tweet_image', $path . '/twitter.png'),
  );
  $image_location = drupal_get_path('module', 'tweet') . '/identica.png';
  $form['tweet_identica_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Identica image'),
    '#description' => t('The location of the icon to use with the "Post to Identi.ca" link, relative to your Drupal installation. Ex.: @location', array('@location' => $path . '/identica.png')),
    '#default_value' => variable_get('tweet_identica_image', $path . '/identica.png'),
  );
  $form['tweet_exclude'] = array(
    '#type' => 'textfield',
    '#title' => t('Exclude nodes'),
    '#description' => t('Enter the NIDs of nodes which should not have Tweet links, separated by commas.'),
    '#default_value' => variable_get('tweet_exclude', ''),
  );
  $form['tweet_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Format'),
    '#description' => t('Manipulate the elements of the tweet by changing their order, removing them, or adding them (like hashtags).') . ' ' .
      t('You can use the case-sensitive tokens [url], [title], [node-tags], and [node-teaser].') . ' ' .
      t('The [node-tags] and [node-teaser] tokens only take effect on nodes.') . ' ' .
      t('Note that some token values may be truncated or left out to fit the tweet into 140 characters.'),
    '#maxlength' => 140,
    '#default_value' => variable_get('tweet_format', '[url] [title] [node-tags]'),
  );
  $form['tweet_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text of link'),
    '#description' => t('The token [site] will be replaced with the name of the site for which the link is generated. This is only relevant if you picked a display format that uses text.'),
    '#default_value' => variable_get('tweet_text', t('Post to [site]')),
  );
  return system_settings_form($form);
}

/**
 * Submit handler for tweet_admin().
 */
function tweet_admin_submit($form, &$form_state) {
  //If no types are selected, assign all types.
  if ($form_state['values']['tweet_types'] == array()) {
    $form_state['values']['tweet_types'] = _tweet_node_types();
  }
  variable_set('tweet_types', $form_state['values']['tweet_types']);
  //Clear the general cache because changed settings may mean that different URLs should be used.
  cache_clear_all('*', 'cache', TRUE);
  drupal_set_message(t('The configuration options have been saved.'));
}
