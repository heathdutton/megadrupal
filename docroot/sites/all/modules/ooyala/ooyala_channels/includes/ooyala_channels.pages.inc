<?php

/**
 * @file
 * Menu callbacks and page-specific functions for the Ooyala Channels module.
 */

/**
 * hook_node_alter() helper function to alter the node edit form for channel-
 * enabled node types.
 */
function ooyala_channels_alter_node_form(&$form, &$form_state, $form_id) {
  if (isset($form_state['action'])) {
   $form['#action'] = $form_state['action'];
  }

  ooyala_channels_pop_table($form['#node']);

  // Determine if the user submitted an updated channel lineup.
  if (isset($form_state['new_lineup'])) {
    $new_lineup = &$form_state['new_lineup'];
  }
 
  // We need to handle three cases here.
  //   1. Creating a new node. In this case, $form['#node'] is reloaded from
  //      the default state every time the form is loaded.
  //   2. Updating the lineup for a new node. We can't use $form['#node'] to
  //      stash the existing lineup, so we store it in a form value.
  //   3. Editing an existing node. In this case, we should use $form['#node']
  //      to modify the existing node for hook_nodeapi().
  if (!isset($form['#node']->ooyala_channel['lineup']) && !isset($form_state['new_lineup'])) {
    // New node / no lineup.
    $form['current_lineup'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
    $current_lineup = &$form['current_lineup']['#value'];
  }
  elseif (!isset($form['#node']->ooyala_channel['lineup']) && isset($form_state['values']['current_lineup'])) {
    // New node / existing lineup.
    $current_lineup = &$form_state['values']['current_lineup'];
    $form['current_lineup'] = array(
      '#type' => 'value',
      '#value' => &$current_lineup,
    );
  }
  else {
    // Editing an existing node.
    $current_lineup = &$form['#node']->ooyala_channel['lineup'];
    $form['current_lineup'] = array(
      '#type' => 'value',
      '#value' => &$current_lineup,
    );
  }

  // If we are previewing the node, we need to load the full video details.
  // We can't rely on node_load() because the node might not be saved yet so
  // Ooyala might not have a channel created yet.
  if (isset($form['#node']->in_preview) && $form['#node']->in_preview == TRUE) {
    foreach ($current_lineup as $index => $item) {
      module_load_include("inc", "ooyala", "includes/ooyala.api");
      $current_lineup[$index] = ooyala_api_video_load($item['embedCode']);
    }
  }

  $form['ooyala_channel'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Channel lineup'),
    '#collapsible' => TRUE,
    '#weight' => field_info_max_weight('node', $form['#node']->type, 'form') + 1,
  );

  // Add wrapper
  $form['ooyala_channel']['lineup_add_wrapper'] = array(
    '#markup' => '<div id="ooyala-channels-add-wrapper">',
  );

  // Handle video addition.
  if (isset($form_state['new_video'])) {
    ooyala_channels_lineup_item_add($current_lineup, $form_state['new_video']);
  }

  // Handle video removal. Loop through our lineup, and pull out rows that have been removed.
  if (isset($form_state['remove_embed_codes'])) {
    foreach ($current_lineup as $delta => $data) {
      if (in_array($data['embedCode'], $form_state['remove_embed_codes'])) {
        unset($current_lineup[$delta]);
      }
    }
  }

  // Add the wrapper div where we are going to be rendering a table.
  // This is the target of the ajax_command_replace in the callback
  $form['ooyala_channel']['lineup_remove_wrapper'] = array(
    '#markup' => '<div id="ooyala-channels-remove-wrapper">',
  );

  $form['ooyala_channel']['table'] = array();
  $form['ooyala_channel']['table']['lineup'] = ooyala_channels_lineup_list($current_lineup, isset($new_lineup) ? $new_lineup : array(), TRUE);

  // Only show the remove button if the current channel lineup isn't empty.
  if (!empty($current_lineup)) {
    // There is an additional closing div here to handle the remove wrapper div
    // that was opened earlier
    $form['ooyala_channel']['lineup_remove_wrapper_closing'] = array(
      '#markup' => '</div>',
    );

    $form['ooyala_channel']['remove_items'] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="clear-block">',
      '#suffix' => '</div>',
      '#value' => t('Remove selected videos'),
      '#submit' => array('ooyala_channels_remove_videos'),
      '#ajax' => array(
        'callback' => 'ooyala_channels_remove_videos_callback',
        'wrapper' => 'ooyaala-channels-remove-wrapper',
      ),
    );

    // Handle the items depending on the case, as above. 
    // Existing node.
    if (isset($form['#node']->ooyala_channel)) {
      $form['ooyala_channel']['table']['thumbnail'] = ooyala_channels_thumbnail_element($form['#node']->ooyala_channel);
    }
    else if (isset($form_state['values']['ooyala_channel']['table'])) {
      // Use the form state.
      $form['ooyala_channel']['table']['thumbnail'] = ooyala_channels_thumbnail_element(array('lineup' => $current_lineup, 'thumbnail' => $form_state['values']['ooyala_channel']['table']['thumbnail']));
    }
    else {
      // This is a brand new lineup on a new node.
      $form['ooyala_channel']['table']['thumbnail'] = ooyala_channels_thumbnail_element(array('lineup' => $current_lineup));
    }
    $form['ooyala_channel']['table']['#theme'] = 'ooyala_channels_lineup';
  }

  // closing div for ooyala-channels-remove-wrapper so our add button doesn't disappear when the form is rerendered.
  $form['ooyala_channel']['remove_wrapper_closer'] = array(
    '#markup' => '</div>',
  );

  // The form for adding a new video to the lineup.
  $form['ooyala_channel']['add_item'] = ooyala_channels_lineup_item_add_form();

  $form['ooyala_channel']['#theme'] = 'ooyala_channels_lineup_tabledrag';
}

/**
 * Helper function to build the list of channel items.
 *
 * @param $lineup
 *   The channel lineup being edited.
 * @param $new_lineup
 *   Optional array of the the new lineup submitted by the user. These items
 *   only contain an embedCode and weight key.
 * @param $operations
 *   Optional boolean to add an operations column to the form.
 *
 * @return
 *   A form API array.
 */
function ooyala_channels_lineup_list($current_lineup = array(), $new_lineup = array(), $operations = FALSE) {
  $form = array();

  if (empty($current_lineup)) {
    return $form;
  }

  // Build each row of the form. $line_index is used to keep track of the
  // highest weight of the table so a new video is always displayed at the
  // bottom.
  $line_index = 0;
  foreach ($current_lineup as $item) {
    // If we've submitted new weights for the table, apply them.
    foreach ($new_lineup as $submitted_item) {
      if ($submitted_item['embedCode'] == $item['embedCode']) {
        // Only one embed code can ever match.
        $item['weight'] = $submitted_item['weight'];
        break;
      }
    }

    // Insert the item with the specified weight, or generate weights with the
    // existing order if needed.
    if (isset($item['weight'])) {
      $form[] = ooyala_channels_lineup_item_edit($item, $item['weight'], $operations);

      // When we are inserting a new item, we want it to always be at the end
      // of the table. We use max() since the array order might not match the
      // weight order.
      $line_index = max($item['weight'], $line_index);
      $line_index++;
    }
    else {
      $form[] = ooyala_channels_lineup_item_edit($item, $line_index++, $operations);
    }
  }

  $form['#theme'] = 'ooyala_channels_lineup_items_edit';

  return $form;
}

/**
 * Return a single field for an item in a channel lineup.
 *
 * @param $item
 *   The channel item to display.
 * @param $weight
 *   The weight of the item.
 * @param $operations
 *   Optional boolean to add operations for the item.
 *
 * @return
 *   A Form API array.
 */
function ooyala_channels_lineup_item_edit($item, $weight, $operations = FALSE) {
  $line = array(
    '#weight' => $weight,
  );

  $line['embedCode'] = array(
    '#type' => 'value',
    '#value' => $item['embed_code'],
  );

  $line['weight'] = array(
    '#type' => 'weight',
    '#default_value' => $weight,
    '#attributes' => array('class' => array('ooyala-channels-lineup-edit-weight')),
  );

  $line['thumbnail'] = array(
    '#markup' => theme('ooyala_thumbnail', array('embed_code' => $item['embed_code'], 'image_attributes' => array('width' => 100), 'get_size' => FALSE)),
  );

  // If the item is in a local node, link the video title to the node. In the
  // event of multiple matches we link to the first node.
  $nodes = ooyala_load_nodes($item['embed_code']);

  $line['title'] = array(
    '#markup' => (!empty($nodes) ? l($item['name'], 'node/' . reset(array_keys($nodes))) : check_plain($item['name'])),
  );

  $line['status'] = array(
    '#markup' => theme('ooyala_video_data', array('data_type' => 'status', 'data' => $item['status'])),
  );
  $line['length'] = array(
    '#markup' => theme('ooyala_video_data', array('data_type' => 'length', 'data' => $item['duration'])),
  );

  if ($operations) {
    $line['remove'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove'),
    );
  }

  return $line;
}

/**
 * Return a form for adding an item to a channel lineup.
 *
 * @return
 *   A Form API array.
 */
function ooyala_channels_lineup_item_add_form() {
  $form = array();

  $form['add_form_open'] = array(
    '#markup' => '<div id="ooyala-channels-video-add-form">',
  );

  $form['video_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search for a video'),
    '#size' => 64,
    '#autocomplete_path' => 'ooyala-channels/autocomplete-video',
  );

  // We add a div around the button to prevent the #ajax throbber from shifting
  // the content of the page.
  $form['add_ooyala_video'] = array(
    '#type' => 'submit',
    '#value' => t('Add video'),
    '#submit' => array('ooyala_channels_add_video'),
    '#ajax' => array(
      'callback' => 'ooyala_channels_add_video_callback',
      'wrapper' => 'ooyala-channels-remove-wrapper',
    ),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );

  $form['add_form_close'] = array(
    '#markup' => '</div>',
  );
  return $form;
}

/**
 * Add the item referenced by an embed code to a channel lineup. If the
 * embed code exists in the lineup, the lineup is left unchanged.
 *
 * @param &$lineup
 *   The lineup array to add the item to.
 * @param $embed_code
 *   The embed code of the item to add.
 */
function ooyala_channels_lineup_item_add(&$lineup, $embed_code) {
  if (!empty($lineup)) {
    // If the video is in the current list, don't re-add it.
    foreach ($lineup as $item) {
      if ($embed_code == $item['embedCode']) {
        drupal_set_message(t("%title was not added as it is already assigned to this channel.", array('%title' => $item['title'])), 'warning');
        return;
      }
    }
  }

  // Load the video details from Ooyala.
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');
  $video = ooyala_api_video_load($embed_code);
  // Add the new video to the end of the current lineup.
  $lineup[] = $video;
}

/**
 * Return the form used to add a specific video to a channel from the video
 * node.
 *
 * @param $form_state
 *   The current state of the form.
 * @param $video_node
 *   The video node to add to a channel.
 *
 * @return
 *   A form API array.
 */
function ooyala_channels_add_video_form($form, $form_state, $video_node) {
  module_load_include("inc", "ooyala", "includes/ooyala.api");
  $form = array(
    '#tree' => TRUE,
  );
  $channel_options = array(
    '' => t('- Select a channel -'),
  );

  $content_types = ooyala_channels_content_types();
  $query = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('type', $content_types, 'IN')
    ->addTag('node_access');
  $result = $query->execute()->fetchAll();

  foreach ($result as $id => $data) {
    $channel_options[$data->nid] = check_plain($data->title);
  }

  // Fetch all of the videos embedded in this node. This isn't to handle field 
  // deltas, but instead to handle multiple video fields attached to the same
  // content type.
  $videos = ooyala_node_embed_codes($video_node);
  $video_options = array();
  foreach ($videos as $embed_code) {
    $video = ooyala_api_video_load($embed_code);
    $video_options[$video['embed_code']] = $video['name'];
  }

  $form['video'] = array(
    '#type' => 'select',
    '#title' => t('Video'),
    '#options' => $video_options,
    '#ajax' => array(
      'callback' => 'ooyala_channels_add_to_channel',
      'wrapper' => 'ooyala-channels-add-to-channel-list',
    ),
  );

  $form['channel'] = array(
    '#type' => 'select',
    '#title' => t('Channel'),
    '#multiple' => FALSE,
    '#options' => $channel_options,
    '#ajax' => array(
      'callback' => 'ooyala_channels_add_to_channel',
      'wrapper' => 'ooyala-channels-add-to-channel-list',
    ),
    '#suffix' => '<div class="clear-block"></div>',
  );

  $form['lineup_wrapper'] = array(
    '#markup' => '<div id="ooyala-channels-add-to-channel-list"></div>',
  );

  if (!empty($form_state['values']['channel'])) {
    $channel_node = node_load($form_state['values']['channel']);

    // Add the current video to the lineup.
    ooyala_channels_lineup_item_add($channel_node->ooyala_channel['lineup'], $form_state['values']['video']);

    $form['ooyala_channel']['table']['lineup'] = ooyala_channels_lineup_list($channel_node->ooyala_channel['lineup'], isset($new_lineup) ? $new_lineup : array(), TRUE);

    // We don't want users to be able to remove the video they are adding.
    foreach(element_children($form['ooyala_channel']['table']['lineup']) as $child) {
      $item = &$form['ooyala_channel']['table']['lineup'][$child];
      if ($item['embedCode']['#value'] == $form_state['values']['video']) {
        unset($item['remove']);
      }
    }

    $form['ooyala_channel']['table']['thumbnail'] = ooyala_channels_thumbnail_element($channel_node->ooyala_channel);

    // Only show the save channel button if the user has selected a channel.
    $form['ooyala_channel']['save_channel'] = array(
      '#type' => 'submit',
      '#value' => t('Save channel'),
    );
  }

  $form['lineup_closer'] = array(
    '#markup' => '</div>',
  );

  $form['ooyala_channel']['table']['#theme'] = 'ooyala_channels_lineup';

  $form['ooyala_channel']['#theme'] = 'ooyala_channels_lineup_tabledrag';

  return $form;
}

/**
 * Form API submit handler for the "Add to channel" form.
 */
function ooyala_channels_add_video_form_submit($form, &$form_state) {
  // Ignore #ajax submissions.
  if (isset($form_state['ajax_submission'])) {
    return;
  }

  $channel = node_load($form_state['values']['channel']);
  foreach ($form_state['values']['ooyala_channel']['table']['lineup'] as $index => $item) {
    if (!empty($item['remove'])) {
      unset($form_state['values']['ooyala_channel']['table']['lineup'][$index]);
    }
  }
  $channel->ooyala_channel['lineup'] = $form_state['values']['ooyala_channel']['table']['lineup'];
  $channel->ooyala_channel['thumbnail'] = $form_state['values']['ooyala_channel']['table']['thumbnail'];
  node_save($channel);

  if ($channel->nid) {
    // Text is from node_form_submit(). Hopefully this code got refactored for
    // D7.
    $node_link = l(t('view'), 'node/' . $channel->nid);
    $watchdog_args = array(
      '@type' => $channel->type,
      '%title' => $channel->title,
    );
    $node_types = node_type_get_types();
    $t_args = array(
      '@type' => $node_types[$channel->type]->type,
      '%title' => $channel->title,
    );
    watchdog('content', '@type: updated %title.', $watchdog_args, WATCHDOG_NOTICE, $node_link);
    drupal_set_message(t('@type %title has been updated.', $t_args));
  }
  else {
    drupal_set_message(t('The post could not be saved.'), 'error');
  }
}

/**
 * Return the form API element for a channel thumbnail.
 *
 * @param $channel
 *  The channel with 'lineup' and 'thumbnail' keys.
 *
 * @return
 *  A Form API radios element.
 */
function ooyala_channels_thumbnail_element($channel) {
  // The options for the channel thumbnail
  $thumbnail_options = array();
  foreach ($channel['lineup'] as $item) {
    $thumbnail_options[$item['embed_code']] = t('Channel thumbnail');
  }

  if (isset($channel['thumbnail']) && isset($thumbnail_options[$channel['thumbnail']])) {
    $default_thumbnail = $channel['thumbnail'];
  }
  else {
    $default_thumbnail = reset(array_keys($thumbnail_options));
  }

  return array(
    '#type' => 'radios',
    '#title' => t('Channel thumbnail'),
    '#options' => $thumbnail_options,
    '#default_value' => $default_thumbnail,
  );
}

