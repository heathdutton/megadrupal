<?php
/**
 * @file
 *  wildfire_messages.regions.inc
 *
 * Functions related to regions (repeaters, snippets).
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 *
 * TODO: Untangle FAPI forms code and Regions API functions into separate
 * include files.
 */

/**
 * Add the specified node to the given repeater on the given message.
 *
 * @param string $type
 *    The type of message.
 * @param int $mid
 *    The message ID.
 * @param string $repeater
 *    The name of the repeater.
 * @param int $nid
 *    The node ID to add.
 *
 * @return bool
 *  TRUE if the node was added successfully, else FALSE
 */
function wildfire_node_add_to_repeater($type, $mid, $repeater, $nid) {
  $message = wildfire_message_get($type, $mid);

  // Make sure the message is valid.
  if ($message === FALSE) {
    wildfire_error(
      'wildfire_node_add_to_repeater: failed to load message with type "type" and ID "@mid"',
      array('@type' => $type, '@mid' => $mid)
    );
    return FALSE;
  }

  // Make sure the message has the specified repeater on it.
  if (!wildfire_region_valid($repeater, 'repeater', $message['template'])) {
    wildfire_error(
      'Cannot add node %nid to the %repeater repeater on the %title @message: this message does not have this repeater',
      array(
      '%nid' => $nid,
      '%repeater' => $repeater,
      '%title' => $message['title'],
      '@message' => $type,
    )
    );
    return FALSE;
  }

  // Check that the node's not already on the repeater.
  $query = db_select('wildfire_repeaters_nodes', 'crn')
    ->fields('crn', array(
      'delta',
    ));
  $query->condition('mid', $mid, '=');
  $query->condition('type', $type, '=');
  $query->condition('repeater', $repeater, '=');
  $query->condition('nid', $nid, '=');
  $result = $query->execute();

  $result = $result->fetchField();

  if ($result) {
    wildfire_error(
      'Cannot add node %nid to the %repeater repeater on the %title @message: node is already on the repeater',
      array(
      '%nid' => $nid,
      '%repeater' => $repeater,
      '%title' => $message['title'],
      '@message' => $type,
    )
    );
  }

  // Add the node.
  $query = db_select('wildfire_repeaters_nodes', 'crn')
    ->fields('crn', array(
      'delta'
    ));
  $query->condition('mid', $mid, '=');
  $query->condition('type', $type, '=');
  $query->condition('repeater', $repeater, '=');
  $query->orderBy('delta', 'DESC');
  $query->range(0, 1);
  $result = $query->execute();

  $max = $result->fetchField();

  // If there are no existing deltas, use '0' as the new delta. Otherwise, add
  // one to the highest existing delta.
  $delta = $max === FALSE ? 0 : $max + 1;

  // Insert into the database.
  $record = array(
    'mid' => $mid,
    'type' => $type,
    'repeater' => $repeater,
    'nid' => $nid,
    'delta' => $delta,
  );
  drupal_write_record('wildfire_repeaters_nodes', $record);

  return TRUE;
}

/**
 * Display an overview of the content in a specific broadcast.
 */
function wildfire_content_overview_form($form, &$form_state, $type = '', $mid = 0) {

  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  // Parameter checking.
  if (!wildfire_broadcast_valid($mid)) {
    wildfire_error(
      'Invalid broadcast ID of @mid when trying to show the content overview page',
      array('@mid' => $mid),
      array('@mid' => $mid),
      'admin/wildfire/broadcasts'
    );
  }

  $form = array();
  $descriptions = wildfire_region_descriptions();

  $message = wildfire_message_get($type, $mid);
  $message = module_invoke_all('wildfire_message_populate', $message);

  foreach ($descriptions as $region_type => $description) {

    $form[$region_type . '_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => check_plain(drupal_ucfirst($region_type) . 's'),
      '#description' => check_plain($descriptions[$region_type]),
      '#collapsible' => TRUE,
      '#attributes' => array('id' => 'edit-' . $region_type . '-fieldset'),
    );

    // Find all the regions of this type and render the content.
    if (!empty($message['regions'][$region_type . 's'])
      && is_array($message['regions'][$region_type . 's'])) {

      $sub_content = wildfire_region_preview($region_type, $mid, $type);
      /**
       * wildfire_region_preview() can return either a renderable array
       * or plain HTML, depending on the region type
       */
      if (is_array($sub_content)) {
        $form[$region_type . '_fieldset'][$region_type] = $sub_content;
      }
      else {
        $form[$region_type . '_fieldset'][$region_type] = array(
          '#markup' => $sub_content,
        );
      }
    }
    else {

      $form[$region_type . '_fieldset'][$region_type] = array(
        '#markup' => t(
          'No !region_types are defined in this broadcasts template.',
          array(
            '!region_type' => $region_type
          )
        ),
      );

    }
  }

  return $form;
}

/**
 * Get repeater content for a specific repeater.
 *
 * @param string $name
 *    The name of the repeater
 * @param int $mid
 *    The message ID of the message to which this repeater is attached.
 * @param string $type
 *    The type of message to which this repeater is attached.
 * @param boolean $raw
 *    Return body as exactly what was in the database & don't post-process it
 *    for link URL conversion, etc.
 * @return array
 *    Returns an array of fully loaded nodes, keyed by nid, in the correct order
 *    as set by the repeater's weight.
 */
function wildfire_repeater_get_content($name, $mid, $type, $raw = FALSE) {

  /**
   * 2011-09-29, craig: TODO: It is assumed that the 'nid' read from
   * wildfire_repeaters_nodes refers to a Node entity ID. This is a
   * design limitation at present, as the Regions Repeaters system inherited
   * from CM7 also makes this assumption.
   */
  $entity_type = 'node';

  $result = db_select('wildfire_repeaters_nodes', 'crn')
    ->fields('crn', array(
      'nid',
      'delta',
    ))
    ->condition('repeater', $name, '=')
    ->condition('mid', $mid, '=')
    ->condition('type', $type, '=')
    ->orderBy('delta', 'ASC')
    ->execute();

  $content = array();

  while ($row = $result->fetchAssoc()) {

    $entity = entity_load($entity_type, array($row['nid']));
    $content[$row['delta']] = $entity[$row['nid']];

    if (!is_object($content[$row['delta']])) {
      // Add a 'fake' pseudo-node object
      $content[$row['delta']] = new stdClass;
      $content[$row['delta']]->nid = $row['nid'];
      $content[$row['delta']]->status = 0;
      $content[$row['delta']]->title = t(
          '(Unknown/deleted node #!nid)',
          array(
        '!nid' => $row['nid'],
      )
      );
    }

    if (!$raw) {
    /**
     * 2011-09-29, craig: The D7 version does not provide prerendered
     * content in the returned $content array like the D6 version does,
     * due to the complexities of dealing with correct rendering of language
     * and display views for the fields which the template author may want
     * direct control over; therefore there's nothing to post-process at this
     * stage.
     *
     * Link conversion is now performed at the field rendering level in the
     * D7 port -> see theme_wildfire_field_render();
     */
    }

  }

  return $content;
}

/**
 * Get a preview of the specified repeater's content.
 *
 * @param string $repeater
 *    The name of the repeater.
 * @param int $mid
 *    The message ID.
 * @param string $message_type
 *    The type of the message.
 *
 * @return array
 *    Renderable array containing the preview
 */
function wildfire_repeater_get_preview($mid, $message_type) {
  $message = wildfire_message_get($message_type, $mid);
  $message = wildfire_broadcast_populate($message);

  $form = array();

  if (is_array($message['regions']['repeaters'])) {
    foreach ($message['regions']['repeaters'] as $rname => $repeater) {

      $form[$rname] = array(
        '#type' => 'fieldset',
        '#title' => drupal_ucfirst(check_plain($rname)),
        '#collapsible' => TRUE,
        '#attributes' => array('id' => 'edit-' . $rname),
      );

      $rows = array();

      foreach ($repeater['content'] as $content) {

        $row = array();
        $row[] = l(
          $content->title,
          'node/' . $content->nid
        );
        $rows[] = $row;

      }

      if (!empty($rows)) {

        $form[$rname]['table'] = array(
          '#markup' => theme(
            'table',
            array(
              'headers' => array(),
              'rows' => $rows,
              'attributes' => array('id' => 'wildfire-repeater-' . $rname . '-preview'),
            )
          )
        );

      }
      else {
        $form[$rname]['no_content'] = array(
          '#markup' => t('(No content)'),
        );
        $form[$rname]['#collapsed'] = TRUE;
      }
    }
  }

  // If there are no repeaters, say so.
  if (count($message['regions']['repeaters']) == 0) {

    $form['no_repeaters'] = array(
      '#title' => t('No repeaters'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#markup' => t('There are no repeaters'),
    );

  }

// This ensures that the returned value is nested correctly
  $form = array($form);

  return $form;
}

/**
 * Get all the repeaters on the system.
 *
 * Statically cached.
 *
 * @param string $template
 *    The template to get repeaters for. If omitted, will get repeaters for all
 *    templates.
 * @param bool $include_disabled
 *    If TRUE, gets all repeaters even if they're not enabled.
 * @param bool $reset
 *    If TRUE, regenerates the static repeater cache.
 *
 * @return array
 *  An associative array of all of the repeaters requested
 *
 * TODO: Remove the intertwining with template-related code
 */
function wildfire_repeaters_get($template = NULL, $include_disabled = FALSE, $reset = FALSE) {
  if ($template === NULL) {
    static $wildfire_repeaters;

    // We are going to get repeaters for all templates.
    if (!isset($wildfire_repeaters) || $reset) {

      $wildfire_repeaters = array();

      $query = db_select('wildfire_repeaters', 'cr')
        ->fields('cr', array(
          'name',
          'title',
          'status'
        ))
        ->orderBy('title', 'ASC');

      if (!$include_disabled) {
        $query->condition('status', 1, '=');
      }

      $result = $query->execute();

      while ($row = $result->fetchAssoc()) {
        $wildfire_repeaters[$row['name']] = array(
          'title' => $row['title'],
          'status' => $row['status'],
        );
      }
    }

    if (!empty($wildfire_repeaters)) {
      return $wildfire_repeaters;
    }
    else {
      return FALSE;
    }
  }
  else {
    // We only want repeaters for a specific template
    static $wildfire_template_repeaters;

    if (!isset($wildfire_template_repeaters[$template]) || $reset) {

      $query = db_select('wildfire_templates_regions', 'ctr')
        ->fields('cr', array(
          'name',
          'title',
          'status'
        ));
      $query->join('wildfire_repeaters', 'cr', 'ctr.name = cr.name');
      $query->condition('ctr.template', $template, '=');
      $query->condition('ctr.type', 'repeater', '=');
      $query->orderBy('cr.title', 'ASC');

      if (!$include_disabled) {
        $query->condition('status', 1, '=');
      }

      $result = $query->execute();

      while ($row = $result->fetchAssoc()) {
        $wildfire_template_repeaters[$template][$row['name']] = array(
          'title' => $row['title'],
          'status' => $row['status'],
        );
      }
    }

    if (!empty($wildfire_template_repeaters[$template])) {
      return $wildfire_template_repeaters[$template];
    }
    else {
      return FALSE;
    }
  }
}

/**
 * Get repeater content for a specific snippet.
 *
 * @param string $name
 *    The name of the snippet.
 * @param int $mid
 *    The message ID of the message to which this snippet is attached.
 * @param string $type
 *    The type of message to which this snippet is attached.
 * @param boolean $raw
 *    Return body as exactly what was in the database & don't post-process it
 *    for link URL conversion, etc.
 * @return string
 *    Returns the content of the snippet, or FALSE if not found.
 */
function wildfire_snippet_get_content($name, $mid, $type, $raw = FALSE) {

  global $language;

  $query = db_select('wildfire_snippets_content', 'sc')
    ->fields('sc', array(
    'content'
  ));
  $query->condition('snippet', $name, '=');
  $query->condition('mid', $mid, '=');
  $query->condition('type', $type, '=');
  $result = $query->execute();

  $result = $result->fetchField();

  $result = check_markup(
    $result,
    variable_get('wildfire_snippet_format_id', filter_default_format()),
    isset($language->language) ? $language->language : '',
    TRUE
  );

  if (!$raw) {
    $result = wildfire_convert_relative_links($result);
  }

  return $result;

}

/**
 * Get a preview of the specified snippet's content.
 *
 * @param int $mid
 *    The message ID.
 * @param string $message_type
 *    The type of the message.
 *
 * @return array
 *    Renderable array containing the preview
 */
function wildfire_snippet_get_preview($mid, $message_type) {
  $message = wildfire_message_get($message_type, $mid);
  $message = module_invoke_all('wildfire_message_populate', $message);

  $form = array();

  if (is_array($message['regions']['snippets'])) {
    foreach ($message['regions']['snippets'] as $sname => $snippet) {

      // Convert IDs to 'good' IDs when defined on an element
      $sname = str_replace('_', '-', $sname);

      /**
       * Snippets only contain a single child element. We trim this to
       * 100 characters (or less) for the content preview.
       */
      if (!empty($snippet['content'][0])) {
        $content = $snippet['content'][0];
        if (drupal_strlen($content) > 100) {
          $content = drupal_substr($content, 0, 99) . '…';
        }
      }
      else {
        $content = t('(No content)');
      }

      $form[$sname] = array(
        '#type' => 'fieldset',
        '#title' => drupal_ucfirst(check_plain($snippet['title'])),
        '#collapsible' => TRUE,
        '#collapsed' => empty($snippet['content'][0]) ? TRUE : FALSE,
        '#attributes' => array('id' => 'edit-' . $sname),
      );

      // We don't want formatting tags in the preview in case it makes the
      // overview page layout go awry
      $form[$sname]['preview'] = array(
        '#markup' => strip_tags($content),
      );

    }
  }

  // If there are no repeaters, say so.
  if (count($message['regions']['snippets']) == 0) {

    $form['no_snippets'] = array(
      '#title' => t('No snippets'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#markup' => t('There are no snippets'),
    );

  }

  // This ensures that the returned value is nested correctly
  $form = array($form);

  return $form;
}

/**
 * Get all the snippets on the system.
 *
 * Statically cached.
 *
 * @param string $template
 *    The template for which to get the snippets. If omitted, will get snippets
 *    for all templates.
 * @param bool $include_disabled
 *    If TRUE, gets all snippets even if they're not enabled.
 * @param bool $reset
 *    If TRUE, regenerates the static snippet cache.
 *
 * @return array
 *  An associative array of all of the repeaters requested
 *
 * TODO: Remove the intertwining with template-related code
 */
function wildfire_snippets_get($template = NULL, $include_disabled = FALSE, $reset = FALSE) {
  if ($template === NULL) {
    static $wildfire_snippets;

    if (!isset($wildfire_snippets) || $reset) {

      $wildfire_snippets = array();

      $query = db_select('wildfire_snippets', 'cs')
        ->fields('cs', array(
          'name',
          'title',
          'status'
        ))
        ->orderBy('title', 'ASC');

      if (!$include_disabled) {
        $query->condition('status', 1, '=');
      }

      $result = $query->execute();

      while ($row = $result->fetchAssoc()) {
        $wildfire_snippets[$row['name']] = array(
          'title' => $row['title'],
          'status' => $row['status'],
        );
      }
    }

    if (!empty($wildfire_snippets)) {
      return $wildfire_snippets;
    }
    else {
      return FALSE;
    }
  }
  else {
    // We only want repeaters for a specific template
    static $wildfire_template_snippets;

    if (!isset($wildfire_template_snippets[$template]) || $reset) {

      $query = db_select('wildfire_templates_regions', 'ctr')
        ->fields('cs', array(
          'name',
          'title',
          'status'
        ));
      $query->join('wildfire_snippets', 'cs', 'ctr.name = cs.name');
      $query->condition('ctr.template', $template, '=');
      $query->condition('ctr.type', 'snippet', '=');
      $query->orderBy('cs.title', 'ASC');

      if (!$include_disabled) {
        $query->condition('status', 1, '=');
      }

      $result = $query->execute();

      while ($row = $result->fetchAssoc()) {
        $wildfire_template_snippets[$template][$row['name']] = array(
          'title' => $row['title'],
          'status' => $row['status'],
        );
      }
    }

    if (!empty($wildfire_template_snippets[$template])) {
      return $wildfire_template_snippets[$template];
    }
    else {
      return FALSE;
    }
  }
}

/**
 * Get the content for the specified template region.
 *
 * @param string $region
 *    The name of the repeater or snippet.
 * @param string $region_type
 *    The type of this region such as repeater or snippet.
 * @param int $mid
 *    The message ID being sent.
 * @param string $message_type
 *    The type of the message being sent.
 *
 * @return array
 *  An associative array of all of the region content requested
 */
function wildfire_region_content($region, $region_type, $mid, $message_type) {
  return module_invoke_all('wildfire_region_content', $region, $region_type, $mid, $message_type);
}

/**
 * Gets descriptions for each of the regions implemented.
 */
function wildfire_region_descriptions() {
  return module_invoke_all('wildfire_region_descriptions');
}

/**
 * Generate a preview of the given region type when viewing a message's content.
 *
 * @param string $region_type
 *    The type of this region such as repeater or snippet.
 * @param int $mid
 *    The message ID being sent.
 * @param string $message_type
 *    The type of the message being sent.
 *
 * @return array
 *  An associative array of all of the previews requested
 */
function wildfire_region_preview($region_type, $mid, $message_type) {
  return array_pop(module_invoke_all('wildfire_region_preview', $region_type, $mid, $message_type));
}

/**
 * Save a region such as a snippet or repeater.
 *
 * @param string $region_type
 *    The type of this region such as repeater or snippet.
 * @param string $region
 *    The name of this region.
 * @param int $mid
 *    The message ID.
 * @param string $message_type
 *    The type of message.
 * @param mixed $content
 *    The actual content to be saved.
 *
 *  @return NULL
 */
function wildfire_region_save($region_type, $region, $mid, $message_type, $content) {
  return module_invoke_all('wildfire_region_save', $region_type, $region, $mid, $message_type, $content);
}

/**
 * Repeater add/edit form for a specific message.
 *
 * @param array $form
 *    The FAPI form.
 * @param string $type
 *    The message type.
 * @param int $mid
 *    The message ID.
 *
 * @return array
 *  A valid Drupal FAPI array
 */
function wildfire_repeater_edit_form($form, &$form_state, $type, $mid) {

  /**
   * Check the messages template is OK. If it's invalid or disabled,
   * redirect the user to the message edit page, as our repeaters/snippets
   * are bound by whats defined in the template i.e. User has to change
   * template, but the template they are changing to may not define the same
   * regions.
   */
  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');
  if (!wildfire_broadcast_valid_template($mid, TRUE)) {
    return FALSE;
  }

  $message = wildfire_message_get($type, $mid);

  if (empty($message['regions']['repeaters']) || !is_array($message['regions']['repeaters'])) {
    wildfire_error(
      'Cannot edit the repeaters on the "@name" @type: no repeaters exist',
      array('@name' => $message['name'], '@type' => $message['type']),
      'admin/wildfire/broadcasts/' . $mid . '/content'
    );
  }

  $message = module_invoke_all('wildfire_message_populate', $message);
  $form = array();

  // Make the list of repeaters into a list suitable for use in a drop-down
  // selector for the form.
  $options_repeaters = array();

  foreach ($message['regions']['repeaters'] as $rname => $repeater) {
    $options_repeaters[$rname] = $repeater['title'];
  }

  /**
   * "Add a node" fieldset.
   */
  $form['add_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add content'),
  );
  $form['add_fieldset']['node'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Begin typing the name of the content you\'d like to add, and choose it from the list of suggestions'),
    '#autocomplete_path' => 'wildfire/autocomplete/node',
  );
  $form['add_fieldset']['repeater'] = array(
    '#type' => 'select',
    '#title' => t('Repeater'),
    '#description' => t('If added, the chosen content will go into the repeater you select here, at the bottom of the existing content'),
    '#options' => $options_repeaters,
  );
  $form['add_fieldset']['add_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  /**
   * One fieldset for each repeater, with the draggable table of nodes.
   */
  foreach ($message['regions']['repeaters'] as $rname => $repeater) {
    $form[$rname . '_repeater_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($repeater['title']),
      '#collapsible' => TRUE,
    );

    // Add a form element for each node in the repeater.
    foreach ($repeater['content'] as $delta => $node) {
      $id = $node->nid;

      $viewlink = l(
        $node->title,
        'node/' . $node->nid
      );

      $editlink = l(
        t('edit'),
        'node/' . $node->nid . '/edit'
      );

      $removelink = l(
        t('remove'),
        'admin/wildfire/broadcasts/' . $mid . '/content/repeaters/remove/' . $rname . '/' . $node->nid
      );

      $form[$rname . '_repeater_fieldset']['title'][$id] = array(
        '#markup'  => $viewlink . ' (' . $editlink . ' | ' . $removelink . ')',
      );
      $form[$rname . '_repeater_fieldset']['weight']['weight-' . $rname . '-' . $id] = array(
        '#type' => 'textfield',
        '#size' => 5,
        '#default_value' => $delta,
        '#attributes' => array('class' => array('node-order-weight')),
      );
    }
  }

  /**
   * Save order fieldset.
   */
  $form['save_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Save changes'),
  );
  $form['save_fieldset']['save_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save order'),
  );

  /**
   * Hidden stuff to track the message type and ID.
   */
  $form['mid'] = array(
    '#type' => 'hidden',
    '#value' => $mid,
  );
  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );

  return $form;
}

/**
 * Validation handler for wildfire_repeater_edit_form().
 */
function wildfire_repeater_edit_form_validate(&$form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op == t('Add')) {
    // We're trying to add a new node here. Make sure a node with the specified
    // name actually exists.
    if ($form_state['values']['node'] == '') {
      // There was no node title specified.
      form_set_error('node', t('You must enter the name of a piece of content to be added'));
    }
    else {
      // There was a node title, so make sure it's in the database.
      $query = db_select('node', 'n')
        ->fields('n', array(
          'nid'
        ));
      $query->condition('title', $form_state['values']['node'], '=');
      $result = $query->execute();
      $result = $result->fetchField();

      if ($result === FALSE) {
        form_set_error('node', t('No content by this name exists'));
      }
      else {
        $form_state['values']['nid'] = $result;

        // Make sure the node is not already in the repeater.
        $query = db_select('wildfire_repeaters_nodes', 'crn')
          ->fields('crn', array(
            'delta'
          ));
        $query->condition('mid', $form_state['values']['mid'], '=');
        $query->condition('type', $form_state['values']['type'], '=');
        $query->condition('repeater', $form_state['values']['repeater'], '=');
        $query->condition('nid', $form_state['values']['nid'], '=');
        $result = $query->execute();

        $result = $result->fetchField();

        if ($result !== FALSE) {
          form_set_error(
            'node',
            t('The %title content is already on the selected repeater.', array('%title' => $form_state['values']['node']))
          );
        }
      }
    }
  }
}

/**
 * Submit handler for wildfire_repeater_edit_form().
 */
function wildfire_repeater_edit_form_submit(&$form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op == t('Add')) {
    wildfire_node_add_to_repeater(
      $form_state['values']['type'],
      $form_state['values']['mid'],
      $form_state['values']['repeater'],
      $form_state['values']['nid']
    );
    drupal_set_message(t('The %title content was added to the selected repeater.', array('%title' => $form_state['values']['node'])));
  }
  elseif ($op == t('Save order')) {
    foreach ($form_state['values'] as $key => $delta) {
      // Find all the form values that refer to a weight in a draggable table.
      if (drupal_substr($key, 0, 7) == 'weight-') {
        // Extract the repeater name, and the node ID, from the name of the form
        // element. They're in the format "weight-REPEATERNAME-NID". Note this
        // means we can't have repeater names that contain hyphens.
        $pieces = explode('-', $key);
        $rname = $pieces[1];
        $nid = $pieces[2];

        db_update('wildfire_repeaters_nodes')
          ->fields(array(
            'delta' => $delta,
          ))
          ->condition('mid', $form_state['values']['mid'])
          ->condition('type', $form_state['values']['type'])
          ->condition('repeater', $rname)
          ->condition('nid', $nid)
          ->execute();
      }
    }

    drupal_set_message(t('Changes to the order of items in the repeaters have been saved.'));
  }
}

/**
 * Remove the specified node from the specified repeater on the given message.
 *
 * @param string $type
 *    The message type.
 * @param int $mid
 *    The message ID.
 * @param string $repeater
 *    The name of the repeater.
 * @param int $nid
 *    The node ID to be removed.
 * @param bool $redirect
 *    Whether to redirect to the repeaters page for the broadcast on completion.
 *    Defaults to TRUE
 *
 * @return bool
 *  TRUE if the node was removed from the repeater, else FALSE
 */
function wildfire_repeater_delete_node($type, $mid, $repeater = '', $nid = 0, $redirect = TRUE) {

  // Grab the node title so we can use it in the message to confirm to the user.
  $query = db_select('node', 'n')
    ->fields('n', array(
      'title'
    ));
  $query->condition('nid', $nid, '=');
  $result = $query->execute();
  $title = $result->fetchField();

  db_delete('wildfire_repeaters_nodes')
    ->condition('type', $type)
    ->condition('mid', $mid)
    ->condition('repeater', $repeater)
    ->condition('nid', $nid)
    ->execute();

  if ($redirect) {
    drupal_set_message(t('The %title content has been removed from the selected repeater.', array('%title' => $title)));
    drupal_goto('admin/wildfire/broadcasts/' . $mid . '/content/repeaters');
  }

  return TRUE;
}

/*
 * Removes all entries in repeaters_nodes that are linked to the given node
 * Primarily used to "clean up" wildfire tables upon node deletion
 *
 * @param int $nid
 *    Node ID to remove references to.
 * @return bool
 *    TRUE on success, FALSE if the removal failed for any reason.
 */
function wildfire_repeater_remove_node_all($nid) {

  if (db_delete('wildfire_repeaters_nodes')
    ->condition('nid', $nid)
    ->execute()
  !== FALSE) {
    return TRUE;
  }

  return FALSE;

}

/**
 * Saves a repeater's content for a particular message.
 *
 * Note that this will erase any existing repeater content for that same
 * message.
 *
 * @param string $repeater
 *    The name of the repeater.
 * @param int $mid
 *    The message ID.
 * @param string $type
 *    The type of the message.
 * @param array $content
 *    A content array where the keys are the intended delta values (position of
 *    the nodes in the repeater) and the values are the node objects themselves,
 *    or numeric node IDs.
 * @return bool
 *    TRUE on success (which will happen even if the $content array was empty).
 *    FALSE otherwise.
 */
function wildfire_repeater_save($repeater, $mid, $type, $content) {
  $message = wildfire_message_get($type, $mid);

  if (!wildfire_region_valid($repeater, 'repeater', $message['template'])) {
    wildfire_error(
      'wildfire_repeater_save: failed because the %repeater repeater is not valid for the %title @type.',
      array('%repeater' => $repeater, '%title' => $message['title'], '@type' => $type)
    );
    return FALSE;
  }

  // Wipe all existing repeater content for this message.
  db_delete('wildfire_repeaters_nodes')
    ->condition('mid',  $mid)
    ->condition('type',  $type)
    ->condition('repeater',  $repeater)
    ->execute();

  // Save all nodes for the new repeater.
  if (is_array($content)) {
    $record = array(
      'mid' => $mid,
      'type' => $type,
      'repeater' => $repeater,
    );

    foreach ($content as $delta => $node) {
      // Delta must be a number or we can't use it.
      if (!is_numeric($delta)) {
        continue;
      }

      $record['nid'] = is_numeric($node) ? $node : $node->nid;
      $record['delta'] = $delta;
      drupal_write_record('wildfire_repeaters_nodes', $record);
    }
  }

  return TRUE;
}

/**
 * Save a snippet's content.
 *
 * @param string $snippet
 *    The name of the snippet to be saved.
 * @param int $mid
 *    The message ID.
 * @param string $type
 *    The type of the message such as broadcast.
 * @param string $content
 *    The HTML content that should go in this snippet.
 * @return bool
 *    FALSE on failure or TRUE otherwise.
 */
function wildfire_snippet_save($snippet, $mid, $type, $content) {
  $message = wildfire_message_get($type, $mid);
  $snippets = wildfire_snippets_get($message['template']);

  // Make sure we can legitimately save this snippet for this message.
  if (!isset($snippets[$snippet])) {
    wildfire_error(
      'Could not save the "@sname" snippet for the "@name" @type',
      array(
      '@sname' => $snippet,
      '@name' => $message['name'],
      '@type' => $message['type'],
    )
    );
    return FALSE;
  }

  db_merge('wildfire_snippets_content')
    ->key(array(
      'mid' => $mid,
      'type' => $type,
      'snippet' => $snippet,
    ))
    ->fields(array(
      'mid' => $mid,
      'type' => $type,
      'snippet' => $snippet,
      'content' => $content,
    ))
    ->updateFields(array(
      'content' => $content,
    ))
    ->execute();

  return TRUE;
}

/**
 * FAPI form for editing the snippets on a message.
 */
function wildfire_snippet_edit_form($form, &$form_state, $type, $mid) {

  /**
   * Check the messages template is OK. If it's invalid or disabled,
   * redirect the user to the message edit page, as our repeaters/snippets
   * are bound by whats defined in the template i.e. User has to change
   * template, but the template they are changing to may not define the same
   * regions.
   */
  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');
  if (!wildfire_broadcast_valid_template($mid, TRUE)) {
    return FALSE;
  }

  $message = wildfire_message_get($type, $mid);

  if (empty($message['regions']['snippets']) || !is_array($message['regions']['snippets'])) {
    wildfire_error(
      'Cannot edit the snippets on the "@name" @type: no snippets exist',
      array('@name' => $message['name'], '@type' => $message['type']),
      'admin/wildfire/broadcasts/' . $mid . '/content'
    );
  }

  // We don't need the whole messages contents, only the snippet content
  // for this page. Therefore, only load the snippets into the message.

  /**
   * TODO: Snippet content can't be manipulated by hooks in other modules
   */

  foreach ($message['regions']['snippets'] as $region_name => &$region) {
    $region = array(
      'title' => $region['title'],
      'status' => 1,
      'content' => array(
        wildfire_snippet_get_content(
          $region_name,
          $mid,
          'broadcast',
          TRUE      /* We want RAW content from the database */
        )
      )
    );
  }

  $form = array();

  foreach ($message['regions']['snippets'] as $sname => $snippet) {
    $form['snippet_' . $sname] = array(
      '#type'           => 'text_format',
      '#base_type'      => 'textarea',
      '#title'          => check_plain($snippet['title']),
      '#default_value'  => check_markup(
          $snippet['content'][0],
          variable_get('wildfire_snippet_format_id', filter_default_format()),
          FALSE
      ),
    );
  }

  // Hidden fields to store mid and type.
  $form['mid'] = array(
    '#type'   => 'hidden',
    '#value'  => $mid,
  );
  $form['type'] = array(
    '#type'   => 'hidden',
    '#value'  => $type,
  );
  $form['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for wildfire_snippet_edit_form().
 */
function wildfire_snippet_edit_form_submit(&$form, &$form_state) {
  $c = 0;

  // Go through each form value and if it's a snippet, try to save it.
  foreach ($form_state['values'] as $key => $value) {
    if (drupal_substr($key, 0, 8) == 'snippet_') {
      $sname = drupal_substr($key, 8);
      wildfire_region_save(
        'snippet',
        $sname,
        $form_state['values']['mid'],
        $form_state['values']['type'],
        $value['value']
      );

      $c++;
    }
  }

  drupal_set_message(t(
    '@snippet updated',
    array('@snippet' => format_plural($c, 'Snippet', 'Snippets'))
  ));
}

/**
 * Works out whether the specified region is valid.
 *
 * Disabled regions will not be counted as valid.
 *
 * @param string $region
 *    The name of the region.
 * @param string $type
 *    The region type, such as repeater, or snippet.
 * @param string $template
 *    Optional template name. If included, only checks the regions for the
 *    specified template.
 * @return bool
 *    TRUE if valid. FALSE otherwise.
 */
function wildfire_region_valid($region, $type, $template = NULL) {
  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');

  $regions = $template ?
    wildfire_template_get_regions($template) :
    wildfire_template_get_regions();

  return $regions[$type . 's'][$region]['status'] == 1 ? TRUE : FALSE;
}
