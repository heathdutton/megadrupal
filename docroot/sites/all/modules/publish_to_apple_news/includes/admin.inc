<?php
/**
 * @file
 * Administrative forms and functions for Publish to Apple News.
 */

/**
 * Implements hook_node_insert().
 */
function publish_to_apple_news_node_insert($node) {
  entity_get_controller('node')->resetCache(array($node->nid));
  
  if (!empty($node->publish_to_apple_news_publish)) {
    if ($node->publish_to_apple_news_publish) {
      publish_to_apple_news_save_sections_preview($node);
      publish_to_apple_news_post_article($node->nid);
    } else {
      // Fire our hook_node_delete() implementation
      publish_to_apple_news_node_delete($node);
    }
  }
}


/**
 * Implements hook_node_update().
 */
function publish_to_apple_news_node_update($node) {
  // Clear the static loading cache: need the changes from the just-updated node.
  entity_get_controller('node')->resetCache(array($node->nid));
  
  if (!empty($node->publish_to_apple_news_publish)) {
    if ($node->publish_to_apple_news_publish) {
      publish_to_apple_news_save_sections_preview($node);
      publish_to_apple_news_post_article($node->nid);
    } else {
      publish_to_apple_news_node_delete($node);
    }
  } else {
    publish_to_apple_news_node_delete($node);
  }
}


/**
 * Implements hook_node_delete().
 * Delete the article from Apple News.
 */
function publish_to_apple_news_node_delete($node) {
  $result = db_select('publish_to_apple_news_posted_articles', 'a')
    ->fields('a')
    ->condition('entity_type', 'node')
    ->condition('entity_id', $node->nid)
    ->execute()
    ->fetchAssoc();

  if (!empty($result) && !empty($result['article_id'])) {
    module_load_include('php', 'publish_to_apple_news', 'library/AppleNews/AppleNews');
    $public_key = variable_get('publish_to_apple_news_api_key', '');
    $private_key = variable_get('publish_to_apple_news_shared_secret', '');

    try {
      $apple_news = new AppleNews($public_key, $private_key);
      $apple_news->deleteArticle($result['article_id']);

      db_delete('publish_to_apple_news_posted_articles')
        ->condition('entity_type', 'node')
        ->condition('entity_id', $node->nid)
        ->execute();

      drupal_set_message(t('The Apple News article for @title has been deleted.', array('@title' => $node->title)));
      
      return TRUE;
    } catch (Exception $e) {
      drupal_set_message(t('Error deleting article @title: @error', array('@title' => $node->title, '@error' => $e->getMessage())), 'error');
      
      return FALSE;
    }
  }
}

/**
 * Implements hook_form_alter().
 */
function publish_to_apple_news_form_alter(&$form, &$form_state, $form_id) {
  // Alter node forms whose content type is attached to an apple article type
  if (!empty($form['type']['#value']) && $form['type']['#value'] . '_node_form' == $form_id) {
    $node_type = $form['type']['#value'];

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'publish_to_apple_news_article_type')
      ->propertyCondition('content_type', $node_type)
      ->addMetaData('account', user_load(1));

    $result = $query->execute();

    // Check if this node type is attached to an apple article type
    if (!empty($result['publish_to_apple_news_article_type'])) {
      // If so, add a vertical-tab section to the form
      $form['publish_to_apple_news'] = array(
        '#type' => 'fieldset',
        '#access' => TRUE,
        '#title' => t('Apple News Settings'),
        '#weight' => 100,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#group' => 'additional_settings',
      );

      $available_sections = variable_get('publish_to_apple_news_available_sections', array());
      $default_section_label = "";
      $default_section_id = variable_get('publish_to_apple_news_default_section_id', '');

      if (!empty($available_sections[$default_section_id])) {
        $default_section_label = " (" . $available_sections[$default_section_id] . ")";
      }

      // Get saved section data
      $selected_sections = array();
      if (!empty($form['nid']['#value'])) {
        $node_article_data = publish_to_apple_news_get_article_data('node', $form['nid']['#value']);
        if (!empty($node_article_data['sections'])) {
          $selected_sections = explode(",", $node_article_data['sections']);
        }
      }
      
      // Add a checkbox to let user choose whether to publish to apple news
      $already_published = !empty($node_article_data['article_id']) ? TRUE : FALSE;
      $form['publish_to_apple_news']['publish_to_apple_news_publish'] = array(
        '#type' => 'checkbox',
        '#title' => t('Publish to Apple News?'),
        '#default_value' => $already_published ? TRUE : FALSE,
      );
      
      if ($already_published) {
        $form['publish_to_apple_news']['publish_to_apple_news_publish']['#description'] = t('Note: unchecking this box will un-publish the node from Apple News');
      }
      
      $form['publish_to_apple_news']['is_preview'] = array(
        '#type' => 'checkbox',
        '#title' => t('Is preview?'),
        '#description' => t('Check this box if the article should only be considered a preview.'),
        '#default_value' => !empty($node_article_data['preview']) ? TRUE : FALSE,
      );
      
      if (isset($node_article_data['preview']) && !$node_article_data['preview']) {
        $form['publish_to_apple_news']['is_preview']['#disabled'] = TRUE;
      }

      $form['publish_to_apple_news']['publish_to_apple_news_publish_sections'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Publish to section:'),
        '#options' => $available_sections,
        '#default_value' => $selected_sections,
        '#description' => t("If no sections are selected, content will be published to the default section!section.", array('!section' => $default_section_label)),
      );

      //helper text if sections have not been imported
      if (count($available_sections) === 0 && user_access('administer site configuration')) {
        $form['publish_to_apple_news']['publish_to_apple_news_publish_sections']['#description'] = t("Visit the !url to fetch available sections.", array('!url' => l(t('Apple News Global Settings'), "admin/config/services/publish-to-apple-news")));
      }
    }
  }
}

/**
 * Implements hook_node_operations().
 */
function publish_to_apple_news_node_operations() {
  return array(
    'publish_apple_news' => array(
      'label' => t('Publish selected content to Apple News'),
      'callback' => 'publish_to_apple_news_mass_publish',
    ),
    'delete_apple_news' => array(
      'label' => t('Remove selected content from Apple News'),
      'callback' => 'publish_to_apple_news_mass_delete',
    ),
  );
}

function publish_to_apple_news_mass_publish($nodes) {
  $batch = array(
    'operations' => array(
      array('_publish_to_apple_news_mass_publish_batch_process', array($nodes))
    ),
    'finished' => '_publish_to_apple_news_mass_batch_finished',
    'title' => t('Processing'),
    'progress_message' => '',
    'error_message' => t('The update has encountered an error.'),
  );
  batch_set($batch);
}

function publish_to_apple_news_mass_delete($nodes) {
  $batch = array(
    'operations' => array(
      array('_publish_to_apple_news_mass_delete_batch_process', array($nodes))
    ),
    'finished' => '_publish_to_apple_news_mass_batch_finished',
    'title' => t('Processing'),
    'progress_message' => '',
    'error_message' => t('The update has encountered an error.'),
  );
  batch_set($batch);
}

function _publish_to_apple_news_mass_publish_batch_process($nodes, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($nodes);
    $context['sandbox']['nodes'] = $nodes;
  }

  $nid = array_shift($context['sandbox']['nodes']);
  $node = node_load($nid);
  $node_link = l($node->title, 'node/' . $node->nid);
  if (publish_to_apple_news_post_article($nid)) {
    $context['success'][] = $node_link;
  } else {
    $context['error'][] = $node_link;
  }

  // Update our progress information.
  $context['sandbox']['progress']++;

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function _publish_to_apple_news_mass_delete_batch_process($nodes, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($nodes);
    $context['sandbox']['nodes'] = $nodes;
  }
  
  $nid = array_shift($context['sandbox']['nodes']);
  $node = node_load($nid);
  $node_link = l($node->title, 'node/' . $node->nid);
  if (publish_to_apple_news_node_delete($node)) {
    $context['success'][] = $node_link;
  } else {
    $context['error'][] = $node_link;
  }
  
  $context['sandbox']['progress']++;
  
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}


function _publish_to_apple_news_mass_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed.'));
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
    $message = format_plural(count($results['success']), '1 item successfully processed:', '@count items successfully processed:');
    $message .= theme('item_list', array('items' => $results['success']));
    drupal_set_message($message);
    
    $error_count = count($results['error']);
    if ($error_count) {
      $message = format_plural(count($results['error']), '1 item returned an error:', '@count items returned an error:');
      $message .= theme('item_list', array('items' => $results['error']));
    }
  }
}

/**
 * Global configuration form.
 */
function publish_to_apple_news_settings_form($form, &$form_state) {
  $api_key = variable_get('publish_to_apple_news_api_key', '');
  $default_section = variable_get('publish_to_apple_news_default_section_id', '');
  $available_sections = variable_get('publish_to_apple_news_available_sections', array());
  if (!$api_key) {
    drupal_set_message(t('Fill out the required fields in the form below and click "Save".'));
  } elseif ($api_key && !$default_section && !$available_sections) {
    drupal_set_message(t('Last step: click "Refresh sections list", select a section, and click "Save".'), 'warning');
  }
  
  $live_base_url = variable_get('publish_to_apple_news_live_base_url', '');
  if (!$live_base_url) {
    // First setup, populate w/ $base_url if we're on a non-local domain
    global $base_url;
    if (valid_url($base_url, TRUE) && strpos($base_url, '.') !== FALSE) {
      $live_base_url = $base_url;
      drupal_set_message(t('We\'ve detected that your Drupal base URL is a valid URL that Apple News will accept. Make sure to click "Save".'));
    } else {
      drupal_set_message(t('We\'ve detected that your Drupal base URL is invalid and Apple News will reject it. Enter a valid URL with a TLD and click "Save".'), 'warning');
    }
  }
  $form['publish_to_apple_news_live_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Live base URL'),
    '#description' => t('Ex. "http://mylivesite.com". Apple News requires all URLs to be valid. http://localhost:8888/, for example, is considered invalid. This value will be used to replace any relative links in your content, as well as your article\'s canonicalURL.'),
    '#required' => TRUE,
    '#default_value' => $live_base_url,
  );
  $form['publish_to_apple_news_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Apple News API key'),
    '#default_value' => $api_key,
    '#required' => TRUE,
  );
  $shared_secret = variable_get('publish_to_apple_news_shared_secret', '');
  $form['publish_to_apple_news_shared_secret'] = array(
    '#type' => 'password',
    '#title' => t('Apple News shared secret'),
    '#default_value' => $shared_secret,
  );
  if (!$shared_secret) {
    $form['publish_to_apple_news_shared_secret']['#required'] = TRUE;
  }

  $secret = variable_get('publish_to_apple_news_shared_secret', '');
  if (!empty($secret)) {
    $form['publish_to_apple_news_shared_secret']['#description'] = t('Secret stored previously. Leave blank to not change.');
  }
  $default_channel = variable_get('publish_to_apple_news_default_channel_id', '');
  $form['publish_to_apple_news_default_channel_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Default channel ID'),
    '#default_value' => $default_channel,
    '#required' => TRUE,
  );
  
  $refresh_button_label = t("Refresh sections list");

  $form['publish_to_apple_news_default_section_id'] = array(
    '#type' => 'select',
    '#title' => t('Default section ID'),
    '#default_value' => $default_section,
    '#options' => $available_sections,
    '#prefix' => '<div id="sections-list-div">',
    '#suffix' => '</div>',
  );

  if (count($available_sections) === 0) {
    $form['publish_to_apple_news_default_section_id']['#options'] = array(t("No sections found"));
    $form['publish_to_apple_news_default_section_id']['#disabled'] = TRUE;
    $form['publish_to_apple_news_default_section_id']['#description'] = t('Click the') . ' "' . $refresh_button_label . '" ' . t("button to get available sections.");
  }

  $form['publish_to_apple_news_default_section_ids_refresh'] = array(
    '#type' => 'submit',
    '#value' => $refresh_button_label,
    '#prefix' => '<small>',
    '#suffix' => '</small>',
    '#ajax' => array(
      'callback' => 'publish_to_apple_news_get_sections_options',
      'wrapper' => 'sections-list-div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('publish_to_apple_news_refresh_sections_submit'),
  );
  
  if (!$api_key) {
    $form['publish_to_apple_news_default_section_id']['#description'] = t('You must click the "Save" button before you can refresh and select a default section ID.');
    $form['publish_to_apple_news_default_section_ids_refresh']['#disabled'] = TRUE;
  } else {
    $form['publish_to_apple_news_default_section_id']['#required'] = TRUE;
  }
  
  $form['publish_to_apple_news_article_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Article identifier'),
    '#default_value' => variable_get('publish_to_apple_news_article_identifier', '[site:name]--[node:nid]'),
    '#description' => t('All nodes published to Apple News will use this identifier. You may use the replacement patterns below.'),
    '#required' => TRUE,
  );
  $form['token_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#description' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#value' => theme('token_tree', array('token_types' => array('node'))),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );

  return $form;
}


/**
 * Global configuration form callback: get available sections
 */
function publish_to_apple_news_get_sections_options($form, $form_state) {
  $sections = publish_to_apple_news_refresh_sections();
  
  reset($sections);
  $first_section_id = key($sections);
  variable_set('publish_to_apple_news_default_section_id', $first_section_id);

  if (count($sections) === 0) {
    $form['publish_to_apple_news_default_section_id']['#options'] = array(t("No sections found"));
    $form['publish_to_apple_news_default_section_id']['#disabled'] = TRUE;
    $form['publish_to_apple_news_default_section_id']['#description'] = t('No sections appear to be available for your default channel.');
  } else {
    $form['publish_to_apple_news_default_section_id']['#options'] = $sections;
    unset($form['publish_to_apple_news_default_section_id']['#disabled']);
    unset($form['publish_to_apple_news_default_section_id']['#description']);
    unset($form['publish_to_apple_news_default_section_id']['#attributes']['disabled']);
  }
  
  // Suppress any messages sent from main form
  drupal_get_messages();
  
  return $form['publish_to_apple_news_default_section_id'];
}

/**
 * Submit handler for refreshing sections
 */
function publish_to_apple_news_refresh_sections_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Configuration form validation.
 */
function publish_to_apple_news_settings_form_validate($form, &$form_state) {
  $live_base_url = $form_state['values']['publish_to_apple_news_live_base_url'];
  if (!valid_url($live_base_url, TRUE) || strpos($live_base_url, '.') === FALSE) {
    form_set_error('publish_to_apple_news_live_base_url', 'Enter a valid live base URL. Ex: http://mylivesite.com');
  }
}

/**
 * Configuration form submit handler.
 */
function publish_to_apple_news_settings_form_submit($form, &$form_state) {
  // Fields that will be saved from $form_state['values']
  $variables = array(
    'publish_to_apple_news_live_base_url',
    'publish_to_apple_news_api_key',
    'publish_to_apple_news_shared_secret',
    'publish_to_apple_news_default_channel_id',
    'publish_to_apple_news_default_section_id',
    'publish_to_apple_news_article_identifier',
  );

  foreach ($variables as $var) {
    // Don't save secret if left empty & already stored
    if ($var == 'publish_to_apple_news_shared_secret' && empty($form_state['values'][$var])) {
      continue;
    }
    if ($var == 'publish_to_apple_news_default_section_id' && empty($form_state['values']['publish_to_apple_news_default_section_id'])) {
      continue;
    }
    variable_set($var, $form_state['values'][$var]);
  }

  drupal_set_message(t('The configuration options have been saved.'));
}