<?php

/**
 * @file
 *
 * Settings callbacks
 */


/**
 * Generate the administration form.
 *
 * This function generates the administration form for the Discuss This! module.
 * The form includes entries of interest:
 *  - Selection of who should be the author of newly-created forum discussions.
 *      The author can be a specific person, the author of the node being
 *      discussed or the currently logged in user. All 3 cases make sense, yet 
 *      each website may want to use one or the other user.
 *      WARNING:  Note that the user assigned to such posts should NOT have more
 *                rights in regard to Input formats, otherwise XSS attacks are
 *                possible. Although the module attempts to prevent such
 *                problems, it is your responsibility to choose who has the
 *                right to create posts in your forum and from what data.
 *
 *  - The set of node types to which Discuss This! links are added
 *      That selection should probably be moved to each Content type instead of
 *      being in this settings form. It let the user select the nodes where a
 *      link appear based on a node type.
 *      TODO:   We could also look into adding a table to allow the user to make
 *              such a selection on a per node basis.
 *
 *  - For each selected node type, a default forum in which to create discussion
 *    topics
 *
 *  - Whether to display some of the comments at the bottom of the original node
 *
 *  - Default content
 *
 *  - Authorize users to log in right from the Discuss This! link
 *
 * @return array
 *            discussthis settings edit form.
 */
function discussthis_admin_settings() {

  // Retrieve existing content-types and discussion configuration for those.
  $node_types = node_type_get_names();
  
  // Prevent Discuss This! in forums.
  unset($node_types['forum']);
  
  // ###################################
  // CONTENT TYPE and FORUM SELECTION
  
  // the forum/node type table
  $form['discussthis_forums'] = array(
      '#type' => 'fieldset',
      '#title' => t('Content Type &amp; Forum Selection'),
      '#description' => t('Defines which content type to activate discussion for. Also for each content type, associate the forum attached and the input format of discussion entries. Leave a node type unchecked if you do not want Discuss This! on these nodes.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
  );
  
  // create a parent element and use our custom theme
  $form['discussthis_forums']['discussthis_types_config'] = array(
      '#prefix' => '<div id="discussthis_types_config">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
      '#theme' => 'discussthis_admin_settings_theming'
  );
  
  // Retrieve a list of available forum containers.
  $discussthis_forums = _discussthis_build_forums_selection_list();
  
  // Retrive a list of available input format
  $discussthis_formats = _discussthis_build_formats_selection_list();
  
  // Retrieve previously submitted value
  $discussthis_types_config = variable_get('discussthis_types_config', array());
  
  // Build a selection row for each node types : 
  //    - enable
  //    - choose forum container
  //    - choose input format
  foreach ($node_types as $type => $name) {

    $form['discussthis_forums']['discussthis_types_config'][$type][$type . '_enable'] = array(
      '#type' => 'checkbox',
      '#title' => $name,
      '#default_value' => isset($discussthis_types_config[$type][$type . '_enable']) ? $discussthis_types_config[$type][$type . '_enable'] : 0,
    );
    $form['discussthis_forums']['discussthis_types_config'][$type][$type . '_forum'] = array(
        '#type' => 'select',
        '#default_value' => isset($discussthis_types_config[$type][$type . '_forum']) ? $discussthis_types_config[$type][$type . '_forum'] : 0,
        '#options' => $discussthis_forums,
    );
    $form['discussthis_forums']['discussthis_types_config'][$type][$type . '_format'] = array(
        '#type' => 'select',
        '#default_value' => isset($discussthis_types_config[$type][$type . '_format']) ? $discussthis_types_config[$type][$type . '_format'] : 0,
        '#options' => $discussthis_formats,
    );
  }
  
  // ###################################
  // LINKS and COMMENTS
  
  // Links and Comments (node being discussed) :
  // This conf let you choose which content-type to allow discussion.
  $form['discussthis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for nodes being discussed'),
    '#description' => t('Setup the forum message that is created automatically when a new discussion is started.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
//   $form['discussthis']['discussthis_autocomplete_contains'] = array(
//     '#type' => 'checkbox',
//     '#title' => t('Auto-complete in nodes searches posts that contain the entered string'),
//     '#description' => t('WARNING: on systems with a large number of nodes in forums, this feature can be very slow. If unsure, leave unchecked.'),
//     '#default_value' => variable_get('discussthis_autocomplete_contains', 0),
//   );

  $form['discussthis']['discussthis_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Discuss This! link'),
    '#description' => t('Enter the message used as the Discuss This! link when no comments were posted yet. Leave empty to use the default.'),
    '#default_value' => variable_get('discussthis_link', ''), // no default, so we can use the default when empty
  );

  $form['discussthis']['discussthis_participate'] = array(
    '#type' => 'textfield',
    '#title' => t('Participate link'),
    '#description' => t('Enter the message shown in the Discuss This! link once comments were already posted. Leave empty to use the default.'),
    '#default_value' => variable_get('discussthis_participate', ''), // no default, so we can use the default when empty
  );

  $form['discussthis']['discussthis_new_user'] = array(
    '#type' => 'textfield',
    '#title' => t('New user message'),
    '#description' => t('Enter the message shown in the Discuss This! link in regard to having the user register a new account. In that case, the user will have to come back to the node on his own in order to post his comment...'),
    '#default_value' => variable_get('discussthis_new_user', ''), // no default, so we can use the default when empty
  );

  $form['discussthis']['discussthis_showcounts'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show comment counts'),
    '#description' => t('When checked, show the number of comments in the discussion (new/total.)'),
    '#default_value' => variable_get('discussthis_showcounts', 1),
  );

  $form['discussthis']['discussthis_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Login/Register link for Anonymous users'),
    '#description' => t('Check this box to offer anonymous users to login/register when on a page with the Discuss this feature. Note: this will have no effect if anonymous users do not have the "access discuss this links" permission. If your registration process is complicated, you may want to turn this feature off.'),
    '#default_value' => variable_get('discussthis_login', 1),
  );

//   $form['discussthis']['discussthis_comments'] = array(
//     '#type' => 'textfield',
//     '#title' => t('Number of comments'),
//     '#description' => t('Select the number of forum comments to display at the bottom of the node being discussed. This feature is ignored in teaser view. Use 0 to disable this feature.'),
//     '#default_value' => variable_get('discussthis_comments', 0),
//     '#size' => 10,
//   );

  // author, template, post being created
  $form['discussthis_post'] = array(
    '#type' => 'fieldset',
    '#title' => t('Post settings for new topics'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['discussthis_post']['discussthis_new_post_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Discuss This! create a new discussion topic title.'),
    '#description' => t('Enter the title of the Discuss This! form used to create a new topic : happen when the user is the first to comment. Leave empty to use the default.'),
    '#default_value' => variable_get('discussthis_new_post_title', t('Create the first post in this discussion')),
  );

  $form['discussthis_post']['discussthis_author'] = array(
    '#type' => 'textfield',
    '#title' => t('Author of newly created Forum discussions'),
    '#description' => t('You can enter one of the following as the Forum discussion author:'
      . 'WARNING: this has to be a UID. Use autocompletion or token if unsure,'
      . 'Note that if the author doesn\'t have permissions to edit their own topics,'
      . ' they won\'t be able to update the topic being posted, and if they don\'t have'
      . ' permissions to post comments, they will not be able to actually submit a'
      . ' comment on the newly-created forum topic.'),
    '#size' => 30,
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => variable_get('discussthis_author', '[current-user:uid]'),
  );

  $form['discussthis_post']['discussthis_newsubject'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for newly created Forum discussions'),
    '#size' => 30,
    '#maxlength' => 60,
    '#default_value' => variable_get('discussthis_newsubject', t('Discussion on [node:content-type]: [node:title]')),
  );      
  $default_body  = DISCUSSTHIS_DEFAULT_NODE_MESSAGE;
  // TODO Please change this theme call to use an associative array for the $variables parameter.
  $form['discussthis_post']['discussthis_newtemplate'] = array(
    '#type' => 'textarea',
    '#title' => t('Template for auto-created Forum discussions'),
    '#description' => theme('token_help', array('discussthis', 'user')),
    '#default_value' => variable_get('discussthis_newtemplate', $default_body),
    '#format' => 'full_html',
  );
  $form['discussthis_post']['discussthis_tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
  );
  // Now process the form
  return system_settings_form($form);
}

/**
 * Validate the discussthis selections.
 *
 * Make sure that every node-type selected also has a default Forum topic.
 */
function discussthis_admin_settings_validate($form, &$form_state) {
  // Check if forum container is choosen for each enabled content types.
  foreach ($form_state['values']['discussthis_types_config'] as $type => $config) {
    if ($config[$type . '_enable'] && !$config[$type . '_forum']) {
      form_set_error('forum', t('Please select a default forum container for content type "@type".',
        array('@type' => $type)));
    }
  }
}

// -------------------------------------------------------------------------.
// FORM THEMING

/**
 * Themer function for the admin setting page.
 * 
 * This method renders an array to configure the node type discussion selection.
 */
function theme_discussthis_admin_settings_theming($vars) {
  
  // Elements to render.
  $element = $vars['element'];
   
  // Build headers.
  $header = array(
    'Content Type', 
    'Forum Container',
    'Input Format',
  );
  
  $rows = array();
  
  // Build rows.
  foreach (element_children($element) as $key) {

    // Create the row.
    $row = array();
    $row['data'] = array();
    
    // Add the row elements.
    foreach ($element[$key] as $fieldname => $value) {
      // Do not render property elements.
      if (substr($fieldname, 0, 1 ) === "#") {
        continue;
      }
      $row['data'][] = drupal_render($element[$key][$fieldname]);
    }
    $rows[] = $row;
  }
  
  // Render the table.
  return theme('table', array(
    'header' => $header, 
    'rows' => $rows,
    'attributes' => array('id' => 'discussthis_types_config'),
  ));
}

// -------------------------------------------------------------------------.
// HELPER METHODS

/**
 * Build a selection list of available forums containers
 * 
 * @return array 
 *            list of available forums containers
 */
function _discussthis_build_forums_selection_list() {
  // Retrieve existing forums
  $forums = forum_forum_load();
  // Build the selection array
  $discussthis_forums = array('0' => '-- select --');
  // Add each forum containers
  foreach ($forums->forums as $tid => $forum) {
    if (empty($forum->container)) {
      $discussthis_forums[$tid] = str_repeat('-', $forum->depth) . $forum->name;
    }
  }
  return $discussthis_forums;
}

/**
 * Build a selection list of available forums
 *
 * @return array 
 *          list of available formats
 */
function _discussthis_build_formats_selection_list() {
  $formats = filter_formats();
  $discussthis_formats = array('0' => 'Default');
  foreach ($formats as $fid => $format) {
    $discussthis_formats[$fid] = $format->name;
  }
  return $discussthis_formats;
}