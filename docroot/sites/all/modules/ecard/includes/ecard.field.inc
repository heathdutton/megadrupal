<?php

/**
 * @file
 * Field for the ecard module to display the input form or an ecard output.
 */

/**
 * Implements hook_field_info().
 */
function ecard_field_info() {
  return array(
    'ecard' => array(
      'label' => t('Ecard Form'),
      'description' => t('Create an Ecard Form for this content.'),
      'default_widget' => 'ecard_widget',
      'default_formatter' => 'ecard_formatter_html',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function ecard_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function ecard_field_widget_info() {
  return array(
    'ecard_widget' => array(
      'label' => t('Default'),
      'field types' => array('ecard'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function ecard_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $element += array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $element['id'] = array(
    '#title' => t('Id'),
    '#type' => 'textfield',
    '#description' => 'CSS id for the ecard div.',
    '#default_value' => isset($items[0]['id']) ? $items[0]['id'] : '',
  );
  $element['class'] = array(
    '#title' => t('Class'),
    '#type' => 'textfield',
    '#description' => 'CSS class for the ecard div.',
    '#default_value' => isset($items[0]['class']) ? $items[0]['class'] : 'ecard-message',
  );
  $element['top'] = array(
    '#title' => t('Top'),
    '#type' => 'textfield',
    '#description' => 'Top offset for the text in pixel.',
    '#default_value' => isset($items[0]['top']) ? $items[0]['top'] : 0,
  );
  $element['left'] = array(
    '#title' => t('Left'),
    '#type' => 'textfield',
    '#description' => 'Left offset for the text in pixel.',
    '#default_value' => isset($items[0]['left']) ? $items[0]['left'] : 0,
  );
  $element['width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#description' => 'Width for the text in pixel.',
    '#default_value' => isset($items[0]['width']) ? $items[0]['width'] : 300,
  );
  $element['color'] = array(
    '#title' => t('Color'),
    '#type' => 'textfield',
    '#description' => 'Color of the ecard message to be displayed in hexadecimal. Don\'t use "#"',
    '#default_value' => isset($items[0]['color']) ? $items[0]['color'] : '000000',
  );
  $element['font'] = array(
    '#title' => t('Font.'),
    '#type' => 'textfield',
    '#description' => 'Path to a TTF font file.',
    '#default_value' => isset($items[0]['font']) ? $items[0]['font'] : '',
  );
  $element['font_size'] = array(
    '#title' => t('Font size'),
    '#type' => 'textfield',
    '#description' => 'Font size for the text in pixel.',
    '#default_value' => isset($items[0]['font_size']) ? $items[0]['font_size'] : 12,
  );
  return $element;
}

/**
 * Implements hook_form_field_ui_field_edit_form_alter().
 * Remove unwanted multi instances field. api.drupal.org says this shiuld be
 * done in hook_field_widget_info() but this won't work. Got this from the
 * fivestar module.
 *
 * @param array $form
 * @param array $form_state
 */
function ecard_form_field_ui_field_edit_form_alter(&$form, $form_state) {
  $field = $form['#field'];
  if ($field['type'] == 'ecard') {
    // Multiple values is not supported with ecard.
    $form['field']['cardinality']['#access'] = FALSE;
    $form['field']['cardinality']['#value'] = 1;
  }
}

/**
 * Settings for the field instance.
 *
 * @param type $field
 * @param type $instance
 * @return type
 */
function ecard_field_instance_settings_form($field, $instance) {
  // Base config.
  $form['ecard_settings'] = array(
    '#title' => t('Configuration'),
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE
  );


  /**
   * @todo Add cron support. Maybe with rules.
  $form['ecard_settings']['ecard_cron'] = array(
  '#type' => 'textfield',
  '#title' => t('Days to keep E-Cards'),
  '#size' => '3',
  '#default_value' => isset($instance['settings']['ecard_settings']['ecard_cron']) ? $instance['settings']['ecard_settings']['ecard_cron'] : 0,
  '#description' => t('Ecards older than the value entered here will automatically be deleted when cron runs. To prevent ecard deletion, set this value to 0.'),
  );
   */
  /**
   * @todo Make this work when the "send multiple e-mails" option is ready.
  $form['ecard_settings']['ecard_max_count'] = array(
  '#type' => 'textfield',
  '#title' => 'Maximum number of e-mails allowed to send at a time',
  '#size' => '3',
  '#default_value' => isset($instance['settings']['ecard_settings']['ecard_max_count']) ? $instance['settings']['ecard_settings']['ecard_max_count'] : 10,
  '#description' => 'The default value is 10. It is recommended to use a low number so mails sent from your server are not treated as spam. If "time out" errors or "white screens" occur then reduce the number.',
  );
   *
   */
  // @todo: use tokens!
  $form['ecard_settings']['ecard_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirection after the ecard is sent.'),
    '#default_value' => isset($instance['settings']['ecard_settings']['ecard_redirect']) ? $instance['settings']['ecard_settings']['ecard_redirect'] : '',
    '#description' => t('Enter the path to where users should be redirected to after sending an ecard.  For example <em>node/123</em>. Leave blank for redirection to the submitted ecard.'),
  );
  $form['ecard_settings']['ecard_require_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require the sender to provide their name.'),
    '#default_value' => isset($instance['settings']['ecard_settings']['ecard_require_name']) ? $instance['settings']['ecard_settings']['ecard_require_name'] : 0,
  );
  $form['ecard_settings']['ecard_require_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require the sender to enter a personal message.'),
    '#default_value' => isset($instance['settings']['ecard_settings']['ecard_require_message']) ? $instance['settings']['ecard_settings']['ecard_require_message'] : 0,
  );
  $form['ecard_settings']['ecard_fill_in_name_e-mail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically fill in sender name and e-mail for logged in users.'),
    '#default_value' => isset($instance['settings']['ecard_settings']['ecard_fill_in_name_e-mail']) ? $instance['settings']['ecard_settings']['ecard_fill_in_name_e-mail'] : 0,
  );
  $form['ecard_settings']['ecard_enable_scheduling'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow the sender to choose the date when the ecard should be sent.'),
    '#default_value' => isset($instance['settings']['ecard_settings']['ecard_enable_scheduling']) ? $instance['settings']['ecard_settings']['ecard_enable_scheduling'] : 0,
  );
  $form['ecard_settings']['ecard_base_path'] = array(
    '#type' => 'select',
    '#title' => t('Ecard url base path'),
    '#options' => array(
      'entity' => t('Entity path'),
      'ecard' => t('Ecard path')
    ),
    '#default_value' => isset($instance['settings']['ecard_settings']['ecard_base_path']) ? $instance['settings']['ecard_settings']['ecard_base_path'] : 'entity',
  );

  // Ecard Delivery message.
  $form['letter'] = array(
    '#title' => t('Delivery message'),
    '#type' => 'fieldset',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE
  );
  $form['letter']['ecard_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#size' => 70,
    '#maxlength' => 70,
    '#default_value' => isset($instance['settings']['letter']['ecard_subject']) ? $instance['settings']['letter']['ecard_subject'] : 'Ecard from [site:name]',
    '#description' => t('Customize the subject for ecard'),
  );
  $form['letter']['ecard_letter'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#cols' => 70,
    '#rows' => 5,
    '#default_value' => isset($instance['settings']['letter']['ecard_letter']) ? $instance['settings']['letter']['ecard_letter'] : _ecard_letter(),
    '#description' => t('This text is the body of the e-mail that the ecard recipient will see. These are the variables you may use: %site = your site name, %site_url = your site URL, %site_mail = your site e-mail address, %card_url = the URL for the ecard, %sender = sender name, %sender_e-mail = sender e-mail, %recipient = recipient e-mail'),
  );

  // Ecard copy notification.
  $form['copy'] = array(
    '#title' => t("Sender's copy message"),
    '#type' => 'fieldset',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE
  );
  $form['copy']['ecard_copy_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send a copy of ecards to sender'),
    '#default_value' => isset($instance['settings']['copy']['ecard_copy_enabled']) ? $instance['settings']['copy']['ecard_copy_enabled'] : TRUE,
  );
  $form['copy']['ecard_copy_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#size' => 70,
    '#maxlength' => 70,
    '#default_value' => isset($instance['settings']['copy']['ecard_copy_subject']) ? $instance['settings']['copy']['ecard_copy_subject'] : 'A copy of your ecard',
    '#description' => t('Customise the e-mail sent to the sender as copy of the ecard.'),
  );
  $form['copy']['ecard_copy'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#cols' => 70,
    '#rows' => 5,
    '#default_value' => isset($instance['settings']['copy']['ecard_copy']) ? $instance['settings']['copy']['ecard_copy'] : _ecard_copy(),
    '#description' => t('This text is the body of the e-mail to notice the sender that his ecard has been picked up. These are the variables you may use: %site = your site name, %site_url = your site URL, %site_mail = your site e-mail address, %card_url = the URL for the ecard'),
  );

  // Notification settings.
  $form['notice'] = array(
    '#title' => t('Collection notification message'),
    '#type' => 'fieldset',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE
  );
  $form['notice']['ecard_notice_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable option for E-Card collection notification e-mails'),
    '#default_value' => isset($instance['settings']['notice']['ecard_notice_enabled']) ? $instance['settings']['notice']['ecard_notice_enabled'] : TRUE,
  );
  $form['notice']['ecard_notice_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#size' => 70,
    '#maxlength' => 70,
    '#default_value' => isset($instance['settings']['notice']['ecard_notice_subject']) ? $instance['settings']['notice']['ecard_notice_subject'] : 'Your ecard has been just picked up',
    '#description' => t('Customize the subject for ecard'),
  );
  $form['notice']['ecard_notice'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#cols' => 70,
    '#rows' => 5,
    '#default_value' => isset($instance['settings']['notice']['ecard_notice']) ? $instance['settings']['notice']['ecard_notice'] : _ecard_notice(),
    '#description' => t('This text is the body of the e-mail to notice the sender that the ecard has been picked up. These are the variables you may use: %site = your site name, %site_url = your site URL, %site_mail = your site e-mail address, %card_url = the URL for the ecard, %sender = sender name, %recipient = recipient e-mail'),
  );

  return $form;
}

/**
 * Implements hook_field_formatter_info().
 */
function ecard_field_formatter_info() {
  return array(
    'ecard_formatter_html' => array(
      'label' => t('View'),
      'field types' => array('ecard'),
      'settings' => array(
        'show_ecard' => 1,
        'show_form' => 0,
        'form_position' => 'below'
      ),
    ),
    'ecard_formatter_image' => array(
      'label' => t('Ecard image'),
      'field types' => array('image'),
      'settings' => array('image_style' => '',),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function ecard_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

// Settings for ecard field formater.
  if ($field['type'] === 'ecard') {
    $element['show_ecard'] = array(
      '#title' => t('Show ecard text'),
      '#type' => 'checkbox',
      '#description' => t('Show the ecard text if the ecard is available. You can also show the ecard by an image field.'),
      '#default_value' => $settings['show_ecard'],
    );
    $element['show_form'] = array(
      '#title' => t('Show form'),
      '#type' => 'checkbox',
      '#description' => t('Show the ecard form if an ecard is available. If no ecard is available the form is always been shown.'),
      '#default_value' => $settings['show_form'],
    );
    $element['form_position'] = array(
      '#title' => t('Form position'),
      '#type' => 'select',
      '#description' => 'If the form is shown, where shall it apear.',
      '#options' => array(
        'above' => t('Above the ecard text'),
        'below' => t('Below the ecard text')
      ),
      '#default_value' => $settings['form_position'],
    );
  }

  // Settings for image field formatter. Copy from image.field.inc
  if ($field['type'] === 'image') {
    $image_styles = image_style_options(FALSE);
    $element['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#default_value' => $settings['image_style'],
      '#empty_option' => t('None (original image)'),
      '#options' => $image_styles,
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function ecard_field_formatter_settings_summary($field, $instance, $view_mode) {

  $settings = $instance['display'][$view_mode]['settings'];
  $summary = array();

  // Summary for ecard field.
  if ($field['type'] === 'ecard') {
    $summary[] = t('Show ecard: ') . $settings['show_ecard'];
    $summary[] = t('Show form: ') . $settings['show_form'];
    $summary[] = t('Form position: ') . $settings['form_position'];
  }

  // Summary for image field. Copy from image.field.inc
  if ($field['type'] === 'image') {
    $image_styles = image_style_options(FALSE);
    // Unset possible 'No defined styles' option.
    unset($image_styles['']);
    // Styles could be lost because of enabled/disabled modules that defines
    // their styles in code.
    if (isset($image_styles[$settings['image_style']])) {
      $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['image_style']]));
    }
    else {
      $summary[] = t('Original image');
    }
  }
  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 *
 * @todo
 *  Better switching of formatters.
 */
function ecard_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $options = $display['settings'];

  $args = ecard_get_args($entity_type, $entity, $field, $instance);

  $element = array();

  $pick_up = 0;
  $hash = ecard_get_hash();

  $display_settings = ECARD_NO_HASH_SET;

  if ($hash) {
    $display_settings = ECARD_HASH_SET;
    if (empty($_GET['preview'])) {
      $pick_up = time();
    }
  }


  if ($field['type'] === 'ecard') {
    $display_settings = $display_settings | ECARD_ECARD_FIELD;
  }
  if ($field['type'] === 'image') {
    $display_settings = $display_settings | ECARD_IMAGE_FIELD;
  }


  switch ($display_settings) {

    case ECARD_ECARD_FIELD | ECARD_NO_HASH_SET:
      switch ($display['type']) {
        case 'ecard_formatter_html':
          $args['mail'] = array(
            'letter' => $instance['settings']['letter'],
            'copy' => $instance['settings']['copy'],
            'notice' => $instance['settings']['notice'],
          );
          $args['settings'] = $instance['settings']['ecard_settings'];
          $element[0] = drupal_get_form('ecard_form_create_ecard', $args);
          break;
      }
      break;

    case ECARD_IMAGE_FIELD | ECARD_NO_HASH_SET:
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'image_formatter',
          '#item' => $item,
          '#image_style' => $display['settings']['image_style'],
        );
      }
      break;

    case ECARD_ECARD_FIELD | ECARD_HASH_SET:
      $element[0]['#markup'] = ecard_render_ecard($hash, $args, $options);
      break;

    case ECARD_IMAGE_FIELD | ECARD_HASH_SET:
      foreach ($items as $delta => $item) {

        $element[$delta] = array(
          '#theme' => 'ecard_image_formatter',
          '#item' => $item,
          '#image_style' => $display['settings']['image_style'],
          '#hash' => $hash,
          '#args' => $args,
          '#ecard_settings' => $entity->field_ecard['und'][0],
        );
      }
      break;
  }

  if ($pick_up && ($display['type'] == 'ecard_formatter_html' || $display['type'] == 'ecard_formatter_image')) {
    $ecards = ecard_read($hash);
    foreach ($ecards as $ecard) {
      // Don't trash original pick up time if they're viewing it a second time.
      if (empty($ecard->pick_up)) {
        $ecard->pick_up = $pick_up;
        ecard_update($ecard);

        // Send notice email if necessary.
        $notice = $instance['settings']['notice'];
        if ($notice['ecard_notice_enabled'] && $ecard->notify) {
          $ecard->base_path = $args['entity_path'];
          if ($instance['settings']['ecard_settings']['ecard_base_path'] == 'ecard') {
            $ecard->base_path = 'ecard';
          }
          $ecard->ecard_url = url($ecard->base_path, array(
            'query' => array('ecard' => $ecard->hash),
            'absolute' => TRUE
          ));
          $ecard->site_name = variable_get('site_name', 'Default site name');
          $ecard->mail = array(
            'letter' => $instance['settings']['letter'],
            'copy' => $instance['settings']['copy'],
            'notice' => $instance['settings']['notice'],
          );
          ecard_mail_send_ecard($ecard, 'ecard_notice');
        }
      }
    }
  }
  return $element;
}

/**
 * @return
 *   The valid hash of an ecard. If no valid hash is available it returns FALSE.
 */
function ecard_get_hash() {
  if (!empty($_GET['ecard'])) {
    $hash = $_GET['ecard'];
    if (preg_match('~^[0-9a-f]{32}$~', $hash)) {
      return $hash;
    }
  }
  return FALSE;
}

/**
 * Returns an array of arguments for an ecard. This arguments are necessary
 * to save or view an ecard with a formatter.
 * @param $entity
 *   The entity, where the ecard is saved in.
 * @param $field
 *   The field array, for the ecard or the image field.
 * @param $instance
 *   The instance array of the $field field.
 *
 * @return
 *   An array of arguments for ecards.
 */
function ecard_get_args($entity_type, $entity, $field, $instance) {

  $entity_ids = entity_extract_ids($instance['entity_type'], $entity);
  $entity_uri = entity_uri($entity_type, $entity);

  $args = array(
    'entity_type' => $instance['entity_type'],
    'bundle' => $instance['bundle'],
    'entity_id' => $entity_ids[0],
    'entity_path' => $entity_uri['path'],
    'field_name' => $field['field_name'],
  );

  if (!empty($instance['settings']['ecard_settings']['ecard_redirect'])) {
    $args['custom_path'] = $instance['settings']['ecard_settings']['ecard_redirect'];
  }
  return $args;
}

/**
 * The ecard creation form. Called by drupal_get_form
 *
 * @return
 *  The ecard creation form.
 */
function ecard_form_create_ecard($form, &$form_state, $settings = array()) {
  global $user;
  $auto_fill = ($user->uid && isset($settings['settings']['ecard_fill_in_name_e-mail'])) ? $settings['settings']['ecard_fill_in_name_e-mail'] : FALSE;

  // Sender's name.
  $form['name_from'] = array(
    '#title' => t('Sender name'),
    '#type' => 'textfield',
    '#default_value' => $auto_fill ? $user->name : '',
    '#required' => isset($settings['settings']['ecard_require_name']) ? $settings['settings']['ecard_require_name'] : FALSE,
    //'#tree' => FALSE,
  );

  // Sender's e-mail.
  $form['mail_from'] = array(
    '#title' => t('Sender e-mail'),
    '#type' => 'textfield',
    '#default_value' => $auto_fill ? $user->mail : '',
    '#required' => TRUE,
    //'#tree' => FALSE,
  );

  // Sender's name.
  $form['name_to'] = array(
    '#title' => t('Recipients name'),
    '#type' => 'textfield',
    //'#tree' => FALSE,
  );

  // Sender's e-mail.
  $form['mail_to'] = array(
    '#title' => t('Recipients e-mail'),
    '#type' => 'textfield',
    '#required' => TRUE,
    //'#tree' => FALSE,
  );

  /**
   * @todo Implement a switch between single and multiple recipients.
   *
  // Display box to type recipients e-mails.
  $form['mail_to'] = array(
  '#title' => t('E-mail(s) of recipient(s)'),
  '#type' => 'textarea',
  '#rows' => 3,
  '#default_value' => '',
  '#description' => t('You may enter multiple e-mails separated by commas or line breaks'),
  '#required' => TRUE,
  '#tree' => FALSE,
  );
   */
  // Display textarea to type message.
  $form['text'] = array(
    '#title' => t('Your message'),
    '#type' => 'textarea',
    '#description' => t('Whatever you type here will be displayed with the ecard'),
    '#required' => isset($settings['settings']['ecard_require_message']) ? $settings['settings']['ecard_require_message'] : FALSE,
    //'#tree' => FALSE,
  );

  if ($settings['settings']['ecard_enable_scheduling']) {
    $form['ecard_scheduled_date'] = array(
      '#title' => t('Schedule your ecard for sending'),
      '#type' => 'date',
      '#description' => t("Enter today's date or leave blank to send the card immediately"),
    );
  }

  if ($settings['mail']['notice']['ecard_notice_enabled']) {
    $form['ecard_send_notice'] = array(
      '#title' => t('Notify me when the e-card is picked up'),
      '#type' => 'checkbox',
      '#default_value' => 0,
    );
  }

  // Filter format for the text.
  // @see http://drupal.org/update/modules/6/7#text_format
  //$form['filter'] = filter_form();

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send this e-card'),
    '#weight' => 100
  );

  return $form;
}

/**
 * Validation of the form ecard_form_create_ecard.
 */
function ecard_form_create_ecard_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['mail_from'])) {
    form_set_error('', t('You must enter a valid e-mail adress.'));
  }
  if (!valid_email_address($form_state['values']['mail_to'])) {
    form_set_error('', t('You must enter a valid e-mail adress.'));
  }
  if (!empty($form_state['values']['ecard_scheduled_date'])) {
    $date = $form_state['values']['ecard_scheduled_date'];
    // Check date is not yesterday or before, but still allow today.
    $formatted_date = mktime(23, 59, 59, $date['month'], $date['day'], $date['year']);
    if ($formatted_date < time()) {
      form_set_error('ecard_scheduled_date', t('Please enter a date in the future to send the ecards on.'));
    }
    else {
      // Default time to midnight.
      $new_date = mktime(0, 0, 0, $date['month'], $date['day'], $date['year']);
      form_set_value(array('#parents' => array('ecard_scheduled_timestamp')), $new_date, $form_state);
    }
  }
}

/**
 * Submit of the form ecard_form_create_ecard.
 * @todo tree = false?
 */
function ecard_form_create_ecard_submit($form, &$form_state) {
  $ecard['entity_type'] = $form_state['build_info']['args'][0]['entity_type'];
  $ecard['bundle'] = $form_state['build_info']['args'][0]['bundle'];
  $ecard['entity_id'] = $form_state['build_info']['args'][0]['entity_id'];
  $ecard['field_name'] = $form_state['build_info']['args'][0]['field_name'];
  $ecard['uid'] = $GLOBALS['user']->uid;

  // Remove any HTML and other unwanted chars that might destroy the senders name.
  $ecard['name_from'] = ecard_clean_name($form['name_from']['#value']);
  $ecard['mail_from'] = $form_state['values']['mail_from'];
  $ecard['name_to'] = ecard_clean_name($form['name_to']['#value']);
  $ecard['mail_to'] = $form_state['values']['mail_to'];
  $ecard['notify'] = $form_state['values']['ecard_send_notice'];

  // Make sure nothing bad can happen.
  // @todo Implement input filter system.
  $ecard['text'] = filter_xss_admin($form_state['values']['text']);

  // Handle delayed / scheduled sending.
  $send_now = TRUE;
  $ecard['scheduled_time'] = 0;
  if ($form_state['build_info']['args'][0]['settings']['ecard_enable_scheduling'] && !empty($form_state['values']['ecard_scheduled_timestamp'])) {
    $ecard['scheduled_time'] = $form_state['values']['ecard_scheduled_timestamp'];
    if ($ecard['scheduled_time'] > time()) {
      $send_now = FALSE;
    }
  }

  // Create the ecard and save to the database.
  $ecard = ecard_create($ecard);

  // Configure some useful variables used when sending the ecard and its copy.
  $ecard->mail = $form_state['build_info']['args'][0]['mail'];
  $ecard->base_path = $form_state['build_info']['args'][0]['entity_path'];
  if ($form_state['build_info']['args'][0]['settings']['ecard_base_path'] === 'ecard') {
    $ecard->base_path = 'ecard';
  }
  $ecard->ecard_url = url($ecard->base_path, array(
    'query' => array('ecard' => $ecard->hash),
    'absolute' => TRUE
  ));
  $ecard->ecard_copy_url = url($ecard->base_path, array(
    'query' => array(
      'ecard' => $ecard->hash,
      'preview' => 1
    ),
    'absolute' => TRUE
  ));

  $ecard->site_name = variable_get('site_name', 'Default site name');

  // Send ecard if no delayed sending.
  if ($send_now) {
    ecard_mail_send_ecard($ecard, 'ecard_ecard');
    drupal_set_message(t('Your ecard was sent.'));
  }
  else {
    drupal_goto($ecard->base_path, array(
      'query' => array(
        'ecard' => $ecard->hash,
        'preview' => 1
      )
    ));
    drupal_set_message(t('Your e-card has been scheduled for delivery on %date', array('%date' => format_date($ecard->scheduled_time, 'custom', 'jS F Y'))));
  }

  // Send the ecard copy immediately.
  if ($ecard->mail['copy']['ecard_copy_enabled']) {
    ecard_mail_send_ecard($ecard, 'ecard_copy');
    drupal_set_message(t('A copy of your ecard was sent to you.'));
  }

  // Redirect the sender to the specified custom path.
  if (!empty($form_state['build_info']['args'][0]['custom_path'])) {
    drupal_goto($form_state['build_info']['args'][0]['custom_path']);
  }
  // Otherwise redirect to ecard preview.
  else {
    drupal_goto($ecard->base_path, array(
      'query' => array(
        'ecard' => $ecard->hash,
        'preview' => 1
      )
    ));
  }
}

