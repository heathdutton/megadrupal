<?php
/**
 * @file
 *  wildfire_broadcasts.admin.inc
 *
 * Forms for the Wildfire Broadcasts module
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * FAPI form to confirm the deletion of a broadcast.
 */
function wildfire_broadcasts_delete_form($form, &$form_state, $mid) {

  // Sanity check. Redirect to broadcast list if a stupid broadcast ID was
  // provided.
  $broadcast = new WildfireBroadcast($mid);

  if (!isset($broadcast->mid)) {
    $message = t(
      'Cannot delete broadcast !mid, as it is not a valid broadcast ID',
      array(
        '!mid' => $mid
      )
    );
    drupal_set_message(check_plain($message), 'error');
    watchdog('wildfire_messages', check_plain($message));

    drupal_goto('admin/wildfire/broadcasts');
  }

  $form = confirm_form(
    array(
      'mid' => array(
        '#type' => 'hidden',
        '#value' => arg(3),
      )
    ),
    t('Please confirm'),
    'admin/wildfire/jobs',
    t(
      'Are you sure you want to delete the broadcast %broadcast? There is no way to undo this.',
      array(
        '%broadcast' => $broadcast->name)
    ),
    t('Yes'),
    t('No')
  );

  return $form;
}

/**
 * Submission handler for the broadcast deletion form
 *
 * @param array $form
 *  A Drupal form array
 * @param array $form_state
 *  The form state
 *
 * @return NULL
 *
 */
function wildfire_broadcasts_delete_form_submit($form, &$form_state) {
  $broadcast = new WildfireBroadcast($form_state['values']['mid']);

  if ($broadcast->delete()) {
    drupal_set_message(t('Broadcast %name has been deleted.', array('%name' => $broadcast->name)));
  }
  else {
    drupal_set_message(t('The broadcast with ID %id was not found and could not be deleted.', array('%id' => $form_state['values']['mid'])), 'warning');
  }

  // The broadcast is gone or invalid, to redirect back to the list page
  drupal_goto('admin/wildfire/broadcasts');
}

/**
 * FAPI form to add or edit a broadcast.
 *
 * If the $mid parameter is present, this form is an edit form. Otherwise, this
 * is an add form.
 *
 * @param array $form
 * @param array $form_state
 * @param int $mid
 *
 * @return array
 */
function wildfire_broadcasts_form($form, &$form_state, $mid = 0) {

  global $base_url;

  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.admin');
  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');

  drupal_add_js(drupal_get_path('module', 'wildfire_messages') . '/js/wildfire.template_selector.js');

  // Don't preprocess. This will be used very infrequently.
  drupal_add_css(drupal_get_path('module', 'wildfire_messages') . '/css/wildfire.send.css', array('preprocess' => FALSE));

  $form = array();

  $broadcast = NULL;

  // Stop non-numeric values being added to the database as Broadcast IDs.
  if (!is_numeric($mid)) {
    $mid = 0;
  }

  $row = array();

  if ($mid) {
    $broadcast = new WildfireBroadcast($mid);
  }

  $form['mid'] = array(
    '#type' => 'hidden',
    '#value' => $mid,
  );

  /**
   * Broadcast name.
   */
  $form['broadcast_name_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Name'),
    '#collapsible' => FALSE,
  );
  $form['broadcast_name_fieldset']['broadcast_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Broadcast name'),
    '#description' => t('Enter a name for your broadcast.'),
    '#required' => TRUE,
    '#maxlength' => 128,
  );

  /**
   * Template.
   */
  $form['template_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Template'),
    '#collapsible' => FALSE,
  );
  $form['template_fieldset']['template'] = array(
    '#type' => 'hidden',
    '#attributes' => array(
      'id' => 'edit-template'
    ),
  );
  $form['template_fieldset']['template_select'] = array(
    '#markup' => theme(
      'wildfire_template_selector',
      array(
        'templates' => wildfire_templates_get(),
        'current_template' => isset($broadcast->template) ? $broadcast->template : '',
      )
    ),
  );

  /**
   * Details fieldset for from and reply addresses.
   */
  $form['details_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Addresses'),
    '#collapsible' => FALSE,
  );
  $form['details_fieldset']['from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#description' => t('The name that you wish the broadcast to be sent as, for example, "Wildfire".'),
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['details_fieldset']['from_email'] = array(
    '#type' => 'textfield',
    '#title' => t('From email'),
    '#description' => t(
      'The email that you wish the broadcast to be sent as, for example, "!site_mail".',
      array(
        '!site_mail' => variable_get('site_mail', 'webmaster@' . parse_url($base_url, PHP_URL_HOST)),
      )
    ),
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['details_fieldset']['reply_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply name'),
    '#description' => t('The name of the person to whom replies should be sent. If you leave this out, the "from" name will be used.'),
    '#maxlength' => 128,
    '#required' => FALSE,

  );
  $form['details_fieldset']['reply_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply email'),
    '#description' => t('The email address of the person to whom replies should be sent. If you leave this out, the "from" email will be used.'),
    '#maxlength' => 128,
    '#required' => FALSE,
  );

  // If $broadcast contains a WildfireBroadcast object, fill in the field
  // default values
  if (is_object($broadcast)) {
    $form['broadcast_name_fieldset']['broadcast_name']['#default_value'] = $broadcast->name;
    $form['template_fieldset']['template']['#default_value'] = $broadcast->template;
    $form['details_fieldset']['from_name']['#default_value'] = $broadcast->from_name;
    $form['details_fieldset']['from_email']['#default_value'] = $broadcast->from_email;
    $form['details_fieldset']['reply_name']['#default_value'] = $broadcast->reply_name;
    $form['details_fieldset']['reply_email']['#default_value'] = $broadcast->reply_email;
  }

  /**
   * Submit.
   */

  // Change the label on the submit button depending on whether we're adding or
  // editing an alert.
  $submit_label = ($mid == 0) ? t('Add') : t('Update');

  $form['send'] = array(
    '#type' => 'submit',
    '#value' => $submit_label,
  );

  return $form;
}

/**
 * Validate handler for wildfire_broadcasts_form().
 */
function wildfire_broadcasts_form_validate($form_id, &$form_state) {

  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');
  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.admin');

  // Check that we have all the bits we need in order to set up a broadcast.
  $mid = isset($form_state['values']['mid'])
    ? $form_state['values']['mid']
    : FALSE;
  $broadcast_name = $form_state['values']['broadcast_name'];
  $template = $form_state['values']['template'];

  // If we're updating a broadcast, make sure that the broadcast actually exists.
  $broadcast = new WildfireBroadcast($mid);
  if (!$broadcast) {
    form_set_error(
      'broadcast_name',
      t(
        'The broadcast being updated no longer exists. Please return to the !link and try again.',
        array(
          '!link' => l(t('broadcasts list'), 'admin/wildfire/broadcasts'),
        )
      )
    );
  }

  // If filled in, make sure the from and reply addresses are valid.
  if (drupal_strlen($form_state['values']['from_email']) > 0 &&
      !wildfire_valid_email_address($form_state['values']['from_email'])) {
    form_set_error('from_email', t('Please enter a valid "from" email address.'));
  }
  if (drupal_strlen($form_state['values']['reply_email']) > 0 &&
      !wildfire_valid_email_address($form_state['values']['reply_email'])) {
    form_set_error('reply_email', t('Please enter a valid "reply" email address.'));
  }

  // Make sure that the template is valid.
  $valid_template = wildfire_template_get($template);
  if ($valid_template === FALSE) {
    form_set_error(
      'template',
      t(
        'The template !template was not found.',
        array(
          '!template' => $template
        )
      )
    );
  }
  else {
    // Make sure that the template is enabled.
    if ($valid_template['status'] !== '1') {
      form_set_error(
        'template',
        t(
          'The selected template for this broadcast (!template) is disabled; please choose a different template for this broadcast.',
          array(
            '!template' => check_plain($valid_template['title']),
          )
        )
      );
    }
  }

}

/**
 * Submit handler for wildfire_broadcasts_form().
 */
function wildfire_broadcasts_form_submit($form_id, &$form_state) {

  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  $broadcast = array(
    'mid' => $form_state['values']['mid'],
    'name' => $form_state['values']['broadcast_name'],
    'template' => $form_state['values']['template'],
    'from_name' => $form_state['values']['from_name'],
    'from_email' => $form_state['values']['from_email'],
    'reply_name' => $form_state['values']['reply_name'],
    'reply_email' => $form_state['values']['reply_email'],
  );

  if ($broadcast['mid']) {
    if (wildfire_broadcasts_update($broadcast)) {
      drupal_set_message(t('Broadcast %name has been updated.', array('%name' => $broadcast['name'])));
    }
    else {
      drupal_set_message(t('Broadcast %name could not be updated.', array('%name' => $broadcast['name'])), 'error');
    }
  }
  else {
    $broadcast['created'] = REQUEST_TIME;
    if (wildfire_broadcasts_add($broadcast)) {
      drupal_set_message(t('Broadcast %name has been created.', array('%name' => $broadcast['name'])));
    }
    else {
      drupal_set_message(t('Broadcast %name could not be created.', array('%name' => $broadcast['name'])), 'error');
    }
  }

  // Redirect to broadcast list on submit finish
  $form_state['redirect'] = 'admin/wildfire/broadcasts';

}

/**
 * FAPI form to list all broadcasts.
 *
 * @return array
 */
function wildfire_broadcasts_list_form($form, &$form_state) {

  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.api');
  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.template.admin');
  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  $form = array();
  $broadcasts = wildfire_broadcasts_get();
  $templates = wildfire_templates_get();

  foreach ($broadcasts as $mid => $broadcast) {
    $form['mid'][$mid] = array('#markup' => $mid);
    $form['name'][$mid] = array('#markup' => $broadcast['name']);
    $form['created'][$mid] = array('#markup' => $broadcast['created'] == 0 ? '…' : format_date($broadcast['created'], 'custom', WILDFIRE_DATETIME));
    $form['lastsent'][$mid] = array('#markup' => $broadcast['lastsent'] == 0 ? '…' : format_date($broadcast['lastsent'], 'custom', WILDFIRE_DATETIME));
    $form['template'][$mid] = array('#markup' => $templates[$broadcast['template']]['title']);
  }

  return $form;
}

/**
 * A list of broadcasts suitable for use in a FAPI form.
 */
function wildfire_broadcasts_options() {

  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  $broadcasts = wildfire_broadcasts_get();
  $options = array();

  foreach ($broadcasts as $broadcast) {
    $options[$broadcast['mid']] = $broadcast['name'];
  }

  return $options;
}

/**
 * Provides default values for the broadcast checkboxes in a FAPI form.
 *
 * Checks a particular node ID to see which broadcasts the node is in, and
 * returns an array of broadcast IDs.
 *
 * @param int $nid
 *    The node ID to test.
 * @return array
 *    An array containing the broadcast IDs of the broadcasts that the node is
 *    in.
 */
function wildfire_broadcasts_options_defaults($nid = 0) {

  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  $broadcasts = wildfire_broadcasts_get();
  $mids = array();

  // Go through each broadcast.
  foreach ($broadcasts as $broadcast) {
    // For each broadcast, go through the broadcast's nodes and see if the nid
    // we're looking for is attached to the broadcast. If we find the nid,
    // break out of the loop because there's no chance the same nid is in there
    // twice.
    foreach ($broadcast['nodes'] as $node) {
      if ($nid == $node['nid']) {
        $mids[] = $broadcast['mid'];
        break;
      }
    }
  }

  return $mids;
}

/**
 * A list of repeaters suitable for use in a FAPI form.
 * @param int $mid
 *    If defined, filters selection to only those repeaters valid for
 *    the given message
 */
function wildfire_broadcast_repeater_options($mid = 0) {

  // Load the Broadcasts API
  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  // Broadcasts uses Repeaters which are implemented by the parent module,
  // Wildfire messages. So load that too.
  module_load_include('inc', 'wildfire_messages', 'wildfire_messages.regions');

  $repeaters = array();

  if (!empty($mid) && $mid > 0) {
    // Get for just this message
    $broadcast = wildfire_broadcast_get($mid);
    if ($broadcast !== FALSE) {
      $repeaters = wildfire_repeaters_get($broadcast['template']);
    }
  }
  else {
    // Get for everything
    $repeaters = wildfire_repeaters_get();
  }

  $options = array();

  foreach ($repeaters as $repeater_name => $repeater) {
    $options[$repeater_name] = $repeater['title'];
  }

  return $options;
}

/**
 * Creates a form to allow a node to be added to a broadcasts repeater.
 *
 */
function wildfire_broadcasts_add_node_to_repeater_form($form, &$form_state, $nid = 0) {
  /**
   * 2011-08-26, craig: This needs some AJAX magic to change the listed
   * repeaters depending on the broadcast selected.
   */
  drupal_add_js(drupal_get_path('module', 'wildfire_messages') . '/js/wildfire.add_n_to_r_form.js');

  // Add the base path as a Drupal.* setting in JavaScript so we can use it to
  // construct paths correctly.
  drupal_add_js(
    array(
    'base_path' => base_path(),
  ), array('type' => 'setting'
  , 'scope' => JS_DEFAULT));

  $form = array();
  $node = NULL;

  // Make sure the node could be loaded.
  if (($node = node_load($nid)) === FALSE) {
    drupal_set_message(t('The node with ID %nid could not be found.', array('%nid' => $nid)), 'error');
    drupal_goto('<front>');
  }

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $nid,
    '#attributes' => array(
      'id' => 'edit-nid'
    ),
  );

  $form['broadcast_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add \'%title\' to broadcast', array('%title' => $node->title)),
  );

  $options = wildfire_broadcasts_options();

  if (count($options) > 0) {

    $form['broadcast_fieldset']['broadcast'] = array(
      '#type' => 'select',
      '#title' => t('Broadcast'),
      '#description' => t(
        'Choose the broadcast in which this node should appear. Please note you can also add, remove and re-order broadcast content from the !broadcastlink.',
        array('!broadcastlink' => l(
          t('broadcast list'),
          'admin/wildfire/broadcasts'
        ))
      ),
      '#options' => $options,
    );

    /**
     * 2011-08-26, craig: Nodes can only be added to repeater regions in the
     * present implementation, though later down the line they may also be valid
     * for other region types.
     *
     * We show all valid repeaters on the form, then disable the ones not relevant
     * to the selected broadcast via JQuery. This means if Javascript is disabled
     * users can still add nodes to repeaters, though the results may be
     * unexpected if they add a repeater to a broadcast that doesn't define it.
     */
    $form['broadcast_fieldset']['repeater'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Repeaters'),
      '#description' => t(
        'Choose the repeater(s) to place the node into'
      ),
      '#options' => wildfire_broadcast_repeater_options(),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

  }
  else {

    $form['broadcast_fieldset']['no_broadcasts'] = array(
      '#type' => 'markup',
      '#markup' => t(
        'You currently have no broadcasts to add content to. !add_broadcast_link',
        array(
          '!add_broadcast_link' => l(
            t('Add a new broadcast'),
            'admin/wildfire/broadcasts/add'
          )
        )
      )
    );

  }

  return $form;

}

/**
 * Validation handler for wildfire_broadcasts_add_node_to_repeater_form().
 */
function wildfire_broadcasts_add_node_to_repeater_form_validate(&$form, &$form_state) {

  $op = $form_state['values']['op'];

  if ($op == t('Save')) {
    foreach ($form_state['values']['repeater'] as $key => $repeater) {
      // Get the nodes existing status on the selected repeaters
      $query = db_select('wildfire_repeaters_nodes', 'crn')
        ->fields('crn', array(
          'delta'
        ));
      $query->condition('mid', $form_state['values']['broadcast'], '=');
      $query->condition('type', 'broadcast', '=');
      $query->condition('repeater', $key, '=');
      $query->condition('nid', $form_state['values']['nid'], '=');
      $result = $query->execute();

      $result = $result->fetchField();

      if (empty($repeater)) {
        // It's a delete - if the nodes not there, don't bother deleting
        if ($result === FALSE) {
          unset($form_state['values']['repeater'][$key]);
        }
      }
      else {
        // It's an add - if the nodes there, don't bother adding
        if ($result !== FALSE) {
          unset($form_state['values']['repeater'][$key]);
        }
      }
    }
  }

}

/**
 * Submit handler for wildfire_broadcasts_add_node_to_repeater_form().
 */
function wildfire_broadcasts_add_node_to_repeater_form_submit(&$form, &$form_state) {

  $op = $form_state['values']['op'];

  if ($op == t('Save')) {

    foreach ($form_state['values']['repeater'] as $key => $value) {
      // Skip if the check box wasn't actually checked.
      if (empty($value)) {
        // The check box was unticked. Delete the existing record.
        if (wildfire_repeater_delete_node(
          'broadcast',
          $form_state['values']['broadcast'],
          $key,
          $form_state['values']['nid'],
          FALSE
        )) {
          drupal_set_message(
            t(
              'Node with ID "!nid" was removed from the "!repeater" repeater',
              array(
            '!nid' => $form_state['values']['nid'],
            '!repeater' => $key,
          )
            )
          );
        }
        else {
          drupal_set_message(
            t(
              'Node with ID "!nid" could not be removed from the "!repeater" repeater',
              array(
            '!nid' => $form_state['values']['nid'],
            '!repeater' => $key,
          )
            ),
            'error'
          );
        }
      }
      else {
        // The check box was ticked. Add a new entry
        if (wildfire_node_add_to_repeater(
          'broadcast',
          $form_state['values']['broadcast'],
          $key,
          $form_state['values']['nid']
        )) {
          drupal_set_message(
            t(
              'Node with ID "!nid" was added to the "!repeater" repeater',
              array(
            '!nid' => $form_state['values']['nid'],
            '!repeater' => $key,
          )
            )
          );
        }
        else {
          drupal_set_message(
            t(
              'Node with ID "!nid" could not be added to the "!repeater" repeater',
              array(
            '!nid' => $form_state['values']['nid'],
            '!repeater' => $key,
          )
            ),
            'error'
          );
        }
      }
    }

    $form_state['redirect'] = 'node/' . $form_state['values']['nid'];

  }

}

/**
 * AJAX callback to obtain the repeaters for a given broadcast
 * @param int $mid
 *  Broadcast message ID
 * @param int $nid
 *  If a node ID is specified, repeaters will indicate if the node is present
 *  there.
 */
function wildfire_broadcast_get_repeaters($mid, $nid) {

  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  $repeaters = array();
  $message = wildfire_broadcast_get($mid);
  $message = wildfire_broadcast_populate($message);

  if (!empty($message['regions']['repeaters'])) {
    foreach ($message['regions']['repeaters'] as $rname => $repeater) {

      $node_in_repeater = FALSE;
      if (!empty($nid)) {
        foreach ($repeater['content'] as $node) {
          if ($node->nid == $nid) {
            $node_in_repeater = TRUE;
            break;
          }
        }
      }

      $repeaters[$rname] = array(
        'selected' => $node_in_repeater ? 1 : 0,
      );
    }
  }

  print drupal_json_encode(array(
    'repeaters' => $repeaters,
    'data' => '',
  ));
  exit;
}

/**
 * Form to display the broadcast send page.
 */
function wildfire_broadcasts_send_form($form, &$form_state, $mid) {

  module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

  /**
   * Check the messages template is OK. If it's invalid or disabled,
   * redirect the user to the message edit page.
   */
  if (!wildfire_broadcast_valid_template($mid, TRUE)) {
    return FALSE;
  }

  // Confirmation screen if necessary.
  if (isset($form_state['storage']['confirm'])) {

    // Save the subject line to the broadcast
    $broadcast = array(
      'mid' => $mid,
      'subject' => $form_state['values']['cmsubject'],
    );

    // If subject is empty, provide a default one
    if (empty($broadcast['subject'])) {
      $broadcast['subject'] = variable_get('site_name', 'Website') . ' broadcast';
    }

    if ($broadcast['mid']) {
      $message = new WildfireBroadcast($broadcast['mid']);
      foreach ($broadcast as $key => $value) {
        $message->{$key} = $value;
      }

      if ($message->save() === FALSE) {
        form_set_error('cmsubject', t('Subject for broadcast #%mid could not be updated.', array('%mid' => $mid)));
      }
    }

    switch ($form_state['values']['recipient_type']) {
      case 'list':
        // Sending to a list...

        $count = wildfire_list_get_count($form_state['storage']['values']['list'], TRUE);

        return confirm_form(NULL,
          t('Please confirm'),
          'admin/wildfire/broadcasts/' . $mid . '/send',
          t('Are you sure you want to send the "@title" broadcast to the "@list" list, which contains @users ?',
            array(
              '@title' => $form_state['storage']['values']['preview_name'],
              '@list' => wildfire_list_get_name($form_state['storage']['values']['list']),
              '@users' => format_plural($count, '1 user', '@count users'),
            )
          ),
          'Send',
          'Cancel'
        );

      break;
      case 'test':
        // Sending a test...

        $email_addresses = preg_split(
          '/[\s,]/',
          $form_state['values']['recipient_addresses'],
          NULL,
          PREG_SPLIT_NO_EMPTY
        );

        return confirm_form(NULL,
          t('Please confirm'),
          'admin/wildfire/broadcasts/' . $mid . '/send',
          t('Are you sure you want to send the broadcast to the test list, which contains @users ?',
            array(
              '@users' => format_plural(count($email_addresses), '1 user', '@count users'),
            )
          ),
          'Send',
          'Cancel'
        );

      break;
    }

  }

  $form = array();

  /**
   * Get the region content from the broadcast so that we can check that they
   * are not empty. We should only allow the send to proceed if there is
   * actually something to send
   */
  $message = new WildfireBroadcast($mid);

  // Get the region content attached to this broadcast object
  $region_content = $message->getRegionContent();

  $broadcast_has_content = FALSE;

  /**
   * Regions.
   */
  $descriptions = wildfire_region_descriptions();

  foreach ($descriptions as $region_type => $description) {
    // Find all the regions of this type and check the content.
    if (!empty($region_content[$region_type . 's'])
      && is_array($region_content[$region_type . 's'])) {

      foreach ($region_content[$region_type . 's'] as $region) {
        /**
         * Check for empty on the first entries content. This caters for
         * snippets where the area is defined but it just contains no text,
         * but of course this works for checking repeaters too.
         */
        if (!empty($region['content'][0])) {
          $broadcast_has_content = TRUE;
          break;
        }
      }

    }
  }

  if (!$broadcast_has_content) {
    drupal_set_message(
      t('Broadcast "!broadcast" has no content defined. To successfully send at least one repeater must contain nodes, or at least one snippet must contain text',
      array(
      '!broadcast' => $message->name,
    )),
      'warning'
    );
  }

  /**
   * Subject
   */
  $form['details_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subject'),
    '#collapsible' => FALSE,
  );
  $form['details_fieldset']['cmsubject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('This will be the subject line of the email. If you are conducting a test send, the word [TEST] will be prepended to this subject. If you are inclined to change this subject, you should avoid words or terms that might trigger a junk mail filter, like "sale" or "pharmaceuticals"'),
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => $message->subject,
  );

  /**
   * List.
   */
  $form['list_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recipients'),
    '#collapsible' => FALSE,
  );

  $form['list_fieldset']['recipient_type'] = array(
    '#type' => 'radios',
    '#title' => 'Recipients',
    '#options' => array(
      'list' => 'Send to a list',
      'test' => 'Send to test recipients',
    ),
    '#default_value' => 'list',
  );

  // LIVE SECTION

  $form['list_fieldset']['list_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'list-wrapper'
    ),
  );

// Get all of the available lists
  $lists = wildfire_lists_get();

  // Get the top X most recently used lists
  $top_lists = wildfire_lists_get('recent', TRUE, WILDFIRE_MAX_TOP_LISTS);

  if (count($lists) > WILDFIRE_MAX_TOP_LISTS) {
    $top_lists['-1'] = t('Select from all available lists');
  }

  $form['list_fieldset']['list_wrapper']['recently_used_lists'] = array(
    '#type' => 'radios',
    '#title' => 'Send to a list',
    '#options' => $top_lists,
  );

  if (count($lists) > WILDFIRE_MAX_TOP_LISTS) {
    // Only needed if there's more than $top_list_limit lists in the system

    $all_lists = array();
    $all_lists[0] = t('- Select a List -');
    foreach ($lists as $list => $name) {
      $totals = wildfire_list_get_totals($list);
      if (drupal_strlen($name) > 30) {
        $name = drupal_substr($name, 0, 29) . '…';
      }
      $all_lists[$list] = t(
        '!name (!count)',
        array(
          '!name' => $name,
          '!count' => $totals[WILDFIRE_STATUS_SUBSCRIBED],
        )
      );
    }

    $form['list_fieldset']['list_wrapper']['all_lists_wrapper'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'all-lists-wrapper',
      )
    );

    $form['list_fieldset']['list_wrapper']['all_lists_wrapper']['all_lists'] = array(
      '#type' => 'select',
      '#title' => 'All available lists',
      '#options' => $all_lists,
      '#default_value' => 0,
    );

  }

  // TEST SECTION
  $form['list_fieldset']['test_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'test-wrapper'
    ),
  );

  $test_lists = wildfire_lists_get_test_lists();

  $recipient_addresses = '';
  $tlkeys = array();

  // Use the last used list as the default for the recipients list.
  if (count($test_lists) > 0) {
    $tlkeys = array_keys($test_lists);
    $test_list_users = wildfire_list_get($tlkeys[0], TRUE);
    if (!empty($test_list_users)) {
      foreach ($test_list_users as $item) {
        $recipient_addresses[] = $item['mail'];
      }
      $recipient_addresses = implode(', ', $recipient_addresses);
    }
  }

  $form['list_fieldset']['test_wrapper']['recipient_addresses'] = array(
    '#type' => 'textarea',
    '#title' => t('Send to test recipients'),
    '#description' => t('To send to more than one recipient, separate email addresses with newlines or commas'),
    '#rows' => 3,
    '#cols' => 60,
    '#default_value' => $recipient_addresses
  );

  if (count($test_lists) > 0) {

    $form['list_fieldset']['test_wrapper']['recently_used_tests'] = array(
      '#type' => 'radios',
      '#title' => 'Recent test email address sets',
      '#options' => $test_lists,
      '#default_value' => $tlkeys[0],
      '#ajax' => array(
        'callback' => 'wildfire_lists_test_list_callback',
        'wrapper' => 'test-wrapper',
      ),
    );

  }
  else {

    $form['list_fieldset']['test_wrapper']['recently_used_tests'] = array(
      '#type' => 'container',
      '#markup' => t('You have no existing test lists')
    );

  }

  /**
   * Preview & send fieldset.
   */
  $form['send_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview & Send'),
    '#collapsible' => FALSE,
  );
  $form['send_fieldset']['send_when'] = array(
    '#type' => 'radios',
    '#options' => array(
      'now' => t('Send this broadcast immediately'),
      'later' => t('Send this broadcast later'),
    ),
    '#default_value' => isset($form_state['values']['send_when']) ? $form_state['values']['send_when'] : 'now',
  );

  // If the Date API and Date Popup modules are available, use a date_popup
  // field so the user has a nice calendar to pick from. Otherwise, use a single
  // standard textfield.
  $form['send_fieldset']['date_wrapper'] = array(
    '#type'           => 'container',
    '#attributes' => array(
      'id' => 'edit-date-wrapper',
    )
  );

  $form['send_fieldset']['date_wrapper']['date'] = array(
    '#title' => t('Date and time'),
    '#description' => t('Choose the date and time when you\'d like to perform this send. Please note that the system cannot send at exactly this time. Your send might begin up to ten minutes after this time, and, if you have thousands of emails in your list, might take several hours to complete.'),
    // Default to +10 minutes
    '#default_value' => isset($form_state['values']['date']) ?
      $form_state['values']['date'] :
      format_date(REQUEST_TIME + 10 * 60, 'custom', 'j M Y H:i'),
  );

  if (module_exists('date_popup')) {
    $form['send_fieldset']['date_wrapper']['date']['#type'] = 'date_popup';
    $form['send_fieldset']['date_wrapper']['date']['#date_format'] = 'j M Y H:i';
  }
  else {
    $form['send_fieldset']['date_wrapper']['date']['#type'] = 'textfield';
  }

  $form['send_fieldset']['preview_name'] = array(
    '#type' => 'hidden',
    '#value' => $message->name,
    '#attributes' => array(
      'id' => 'wildfire-preview-name'
    ),
  );
  $form['send_fieldset']['preview_subject'] = array(
    '#type' => 'hidden',
    '#value' => $message->subject,
    '#attributes' => array(
      'id' => 'wildfire-preview-subject'
    ),
  );

  /**
   * Render the current quota block at the bottom of the broadcast send page
   * for reference, as suggested by Paul Lomax
   */
  $form['send_quota'] = array(
    '#type' => 'fieldset',
    '#title' => t('Quota'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $defaults = array(
    'total' => 0,
    'top-up' => 0,
    'monthly' => 0,
    'last_checked' => strtotime('-1 hour', REQUEST_TIME),
  );

  $wildfire_quota = new WildfireQuota;
  $quota['total']        = $wildfire_quota->getTotal();
  $quota['last_checked'] = $wildfire_quota->getLastChecked();
  $quota['top-up']       = $wildfire_quota->getTopup();
  $quota['monthly']      = $wildfire_quota->getMonthly();

  $quota_block = array(
    '#theme' => 'wildfire_quota_block',
    '#quota' => $quota,
  );

  $form['send_quota']['quota'] = array(
    '#type' => 'markup',
    '#markup' => drupal_render($quota_block),
  );

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'edit-actions',
      'class' => array('form-actions'),
    )
  );

  $form['actions']['send'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
    '#attributes' => array(
      'class' => array('button-yes'),
    )
  );

  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for wildfire_broadcasts_send_form().
 */
function wildfire_broadcasts_send_form_submit($form_id, &$form_state) {

  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  // Get the current state of the broadcast
  $broadcast = wildfire_broadcast_get(arg(3));

  // If the 'Save' button is clicked, store the subject's current
  // state but do nothing else
  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#value'] == 'Save') {

      // Save the subject line to the broadcast
      $broadcast['subject'] = $form_state['values']['cmsubject'];

      // If subject is empty, provide a default one
      if (empty($broadcast['subject'])) {
        $broadcast['subject'] = variable_get('site_name', 'Website') . ' broadcast';
      }

      $message = new WildfireBroadcast($broadcast['mid']);
      foreach ($broadcast as $key => $value) {
        $message->{$key} = $value;
      }

      if ($message->save() === FALSE) {
        form_set_error('cmsubject', t('Subject for broadcast #%mid could not be updated.', array('%mid' => $broadcast['mid'])));
      }

      return;

    }
  }

  if (!isset($form_state['storage']['confirm'])) {
    $form_state['storage']['confirm'] = TRUE; // This will cause the form to be
    // rebuilt, entering the confirm
    // part of the form...
    $form_state['rebuild'] = TRUE; // ...along with this.
    $form_state['storage']['values'] = $form_state['values'];
  }
  else {

    module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');
    module_load_include('inc', 'wildfire_jobs', 'wildfire_jobs.send');
    module_load_include('inc', 'wildfire_jobs', 'wildfire_jobs.api');
    module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

    $lid = 0;
    $mode = WILDFIRE_JOB_MODE_NORMAL;

    switch ($form_state['storage']['values']['recipient_type']) {
      case 'list':
        /**
         * Sending to a live list. All we need to do is set the mode
         */
        $lid = $form_state['storage']['values']['list'];
      break;
      case 'test':
        /**
         * Sending to a test list. Store the test list ready to pass it
         * through the standard API.
         *
         * The list of addresses passed into wildfire_lists_save_test_list()
         * will be validated on their way through, so no need to do it again
         * here.
         */
        $lid = wildfire_lists_save_test_list($form_state['storage']['values']['recipient_addresses']);
        if ($lid == FALSE) {
          drupal_set_message(
            t('An error occurred when saving the list of test recipients'),
            'error'
          );
          return;
        }
        $mode = WILDFIRE_JOB_MODE_TEST;
      break;
    }

    /**
     * If the list is currently locked, another job is probably using it
     * so prevent two attempts to process the same list at the same time
     */
    if (wildfire_list_is_locked($lid)) {
      drupal_set_message(
        t('Could not send to the selected list: The selected list is'
         . ' currently locked by another job build. Please wait for the'
         . ' other job to be built and try again.'),
        'error'
      );
      return;
    }

    $list_count = wildfire_list_get_count($lid);

    // Make sure we have at least one person to send to.
    if (empty($list_count)) {
      drupal_set_message(
        t(
          'Could not send to the selected list: either the list is empty'
          . ' or all list members have opted out of receiving this broadcast.'
        ),
        'error'
      );
      return;
    }

    // Create a job.
    $job = array();
    $job['template'] = $broadcast['template'];
    $job['lid'] = $lid;
    $job['type'] = 'broadcast';
    $job['extra'] = $broadcast['mid'];
    if ($mode == WILDFIRE_JOB_MODE_NORMAL) {
      // If a deferred send time was given, store it
      $job['sendtime'] = $form_state['storage']['values']['send_when'] == 'now' ?
        REQUEST_TIME :
        strtotime($form_state['storage']['values']['date']);
    }
    elseif ($mode == WILDFIRE_JOB_MODE_TEST) {
      // Test sends can't be deferred
      $job['sendtime'] = REQUEST_TIME;
    }
    $job['mode'] = $mode;

    if (wildfire_job_add($job, WILDFIRE_JOB_CREATE_NORMAL)) {

      drupal_set_message(t('The job has been created successfully.'));

      // Save the broadcasts last sent date.
      $broadcast['lastsent'] = REQUEST_TIME;
      if (!wildfire_broadcasts_update($broadcast)) {
        drupal_set_message(
          t('Updating the last sent date on the broadcast failed'),
          'error'
        );
      }

    }
    else {

      drupal_set_message(t('There was an error creating the job'), 'error');

    }

    // All done; redirect the user to the jobs list.
    $form_state['redirect'] = 'admin/wildfire/jobs';
    unset($form_state['storage']);

  }
}

/**
 * Validation handler for wildfire_broadcasts_send_form().
 */
function wildfire_broadcasts_send_form_validate($form_id, &$form_state) {

  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  // If the 'Save' button is clicked, skip validation as we
  // aren't performing a send right now.
  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#value'] == 'Save') {
      return;
    }
  }

  // For all methods...
  if (!isset($form_state['storage']['confirm'])) {

    // Check that the date is valid, only if the user has chosen not to send
    // immediately.
    if (
      $form_state['values']['send_when'] == 'later' &&
      strtotime($form_state['values']['date']) === FALSE
    ) {
      form_set_error('date', t('Please enter a valid date.'));
    }

    // Make sure the date is in the future.
    if (
      $form_state['values']['send_when'] == 'later' &&
      strtotime($form_state['values']['date']) < REQUEST_TIME
    ) {
      form_set_error('date', t('Please enter a date in the future.'));
    }

    // Check all the links in the broadcast (we'll use the preview) do not exceed
    // 333 characters.
    $broadcast = wildfire_broadcast_get(arg(3));
    $preview = wildfire_preview(
      'broadcast',
      $broadcast['mid'],
      $broadcast['template'],
      TRUE
    );

    $untrackable = wildfire_message_check_link_lengths($preview);

    if ($untrackable === FALSE) {
      form_set_error(
        'field_name',
        t('An error occured whilst trying to check link compatibility for tracking.')
      );
    }

    if (is_array($untrackable)) {
      $count = count($untrackable);

      for ($i = 0; $i < $count; $i++) {
        $untrackable[$i] = l(drupal_substr($untrackable[$i], 0, 80) . '...', $untrackable[$i]);
      }
      $untrackable = implode('<br />', $untrackable);
      drupal_set_message(t(
          'The following links exceed the 333 character limit on URL length, and will not be tracked: !links',
          array('!links' => '<br />' . $untrackable)
      ), 'warning');
    }

    switch ($form_state['values']['recipient_type']) {
      case 'list':

        // Load the list API, as we use its function for extra validation.
        module_load_include('inc', 'wildfire_lists', 'wildfire_lists.api');

        /**
         * If the 'all lists' value is greater than 0, use it, else
         * use the value from recent lists
         */
        if (!empty($form_state['values']['all_lists'])) {
          // All lists
          $form_state['values']['list'] = $form_state['values']['all_lists'];
          unset($form_state['values']['all_lists']);
        }
        else {
          // Recent lists
          $form_state['values']['list'] = $form_state['values']['recently_used_lists'];
          unset($form_state['values']['recently_used_lists']);
        }

        // Make sure a list has been selected
        if (empty($form_state['values']['list'])) {
          form_set_error('list', t('Please select a list for this send.'));
        }

        /**
         * If the selected list is currently locked, another job is probably
         * using it so prevent two attempts to process the same list at the
         * same time.
         */
        if (wildfire_list_is_locked($form_state['values']['list'])) {
          form_set_error(
            'list',
            t('The selected list is'
            . ' currently locked by another job build. Please wait for the'
            . ' other job to be built and try again.')
          );
        }

      break;
      case 'test':
        // Sending a test

        if (empty($form_state['values']['recipient_addresses'])) {
          form_set_error(
            'recipient_addresses',
            t(
              'Test email address list is empty'
            )
          );
          break;
        }

        // Get the email addresses the user put in.
        $email_addresses = preg_split(
          '/[\s,]/',
          $form_state['values']['recipient_addresses'],
          NULL,
          PREG_SPLIT_NO_EMPTY
        );

        // Filter out duplicates...
        $email_addresses = array_unique($email_addresses);

        /**
         * Realistically, it's not a test send if it's going to more than 25
         * addresses. Thus cap the test send limit to prevent huge test sends.
         */
        $test_recipients_cap = 25;

        if (count($email_addresses) > $test_recipients_cap) {
          form_set_error(
            'recipient_addresses',
            t(
              'Test email address lists can only contain !cap or fewer addresses.',
              array(
                '!cap' => $test_recipients_cap,
              )
            )
          );
        }

        foreach ($email_addresses as $email_address) {

          // Check it's a valid email address.
          if (!wildfire_valid_email_address($email_address)) {
            form_set_error(
              'recipient_addresses',
              t(
                'Test email address list contains errors: %bogus_address is not a valid email address.',
                array(
                  '%bogus_address' => $email_address,
                )
              )
            );
            break;
          }

          /**
           * If a user exists for the given address, check whether it's
           * globally opted out. If it is, the message won't be sent to that
           * recipient. Therefore, provide a warning message about it.
           */
          $account = user_load_by_mail($email_address);
          if ($account !== FALSE && !empty($account->wildfire_optouts[0])) {
            form_set_error(
              'recipient_addresses',
              t(
                'A test email cannot be sent to %email as the user has globally opted-out. !edit_link',
                array(
                  '%email' => $email_address,
                  '!edit_link' => l(
                    'Edit user',
                    'user/' . $account->uid . '/edit'
                  )
                )
              )
            );
          }
        }

      break;
      default:
        // Sending a test
        form_set_error(
          'recipient_type',
          t('Unsupported recipient type selected')
        );
      break;
    }

  }
}

/**
 * Broadcasts send form
 */
function wildfire_broadcasts_send_page($form_id, $mid) {

  module_load_include('inc', 'wildfire_broadcasts', 'wildfire_broadcasts.api');

  // Parameter checking.
  if (!wildfire_broadcast_valid($mid)) {
    wildfire_error(
      'Invalid broadcast ID of @mid when trying to show the broadcast send page',
      array('@mid' => $mid),
      'admin/wildfire/broadcasts'
    );
  }

  /**
   * We have a valid broadcast, so now check that the broadcasts contents
   * are actually valid.
   */
  if (!wildfire_broadcast_valid_template($mid, TRUE)) {
    wildfire_error(
      'Invalid template for broadcast ID @mid when trying to show the broadcast send page',
      array('@mid' => $mid),
      'admin/wildfire/broadcasts'
    );
  }

  $broadcast = wildfire_broadcast_get($mid);

  if (!$broadcast) {
    wildfire_error(
      'Broadcast with ID @mid could not be loaded when trying to show the broadcast send page',
      array('@mid' => $mid),
      'admin/wildfire/broadcasts'
    );
  }

  drupal_add_js(drupal_get_path('module', 'wildfire_lists') . '/js/wildfire.list_selector.js');
  drupal_add_js(drupal_get_path('module', 'wildfire_messages') . '/js/wildfire.send.js');

  // Don't preprocess. This will be used very infrequently.
  drupal_add_css(drupal_get_path('module', 'wildfire_messages') . '/css/wildfire.send.css', array('preprocess' => FALSE));

  return theme(
    'wildfire_broadcasts_send_page',
    array(
      'form' => drupal_get_form($form_id, $mid),
      'preview' => array(
        'type' => $broadcast['type'],
        'mid' => $mid,
        'template' => $broadcast['template']
      )
    )
  );

}
