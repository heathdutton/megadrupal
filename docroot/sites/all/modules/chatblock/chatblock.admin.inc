<?php

/**
 * @file
 * Administration form templates for chatblock
 */

/**
 * Small form for the manual message log clean-up task.
 *
 * @see chatblock.module
 * @see _chatblock_settings_validate_cleanup()
 */
function chatblock_cleanup_form($form, &$form_state) {
  $form['chatblock_cleanup_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manual logs clean-up'),
    '#description' => t('Remove all messages from the chat log older than the given time period.'),
    'chatblock_cron_table' => array(
      '#theme' => 'chatblock_cron_form',
      'chatblock_autodelete_value' => array(
        '#type' => 'textfield',
        '#default_value' => variable_get('chatblock_autodelete_value', 0),
        '#size' => 2,
        '#maxlength' => 2,
      ),
      'chatblock_autodelete_unit' => array(
        '#type' => 'select',
        '#options' => array(
          t('minute(s)'),
          t('hour(s)'),
          t('day(s)'),
          t('week(s)'),
          t('month(s)'),
          t('year(s)'),
        ),
        '#default_value' => variable_get('chatblock_autodelete_unit', 0) ,
      ),
    ),
    'chatblock_clean_logs' => array(
      '#type' => 'submit',
      '#value' => t('delete'),
    ),
  );
  $form['#submit'] = array('chatblock_cleanup_manual');
  $form['#validate'][] = '_chatblock_settings_validate_cleanup';
  return $form;
}

/**
 * The main settings form for the admin section.
 *
 * @see chatblock.module
 * @see _chatblock_settings_validate()
 * @see _chatblock_settings_validate_cleanup()
 */
function chatblock_settings_form() {
  $custom_info_page_content = variable_get(
    'chatblock_infopage_custom_content',
    array(
      'value' => '',
      'format' => NULL,
    )
  );
  $form = array(
    'chatblock_basic_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Basic settings'),
      'chatblock_default_title' => array(
        '#default_value' => variable_get('chatblock_default_title', t('Chat')),
        '#title' => t('Chat title'),
        '#description' => t('A short name for your chat. Will be used as the default block title and also for the log pages.'),
        '#type' => 'textfield',
        '#size' => 20,
        '#maxlength' => 20,
        '#required' => TRUE,
      ),
      'chatblock_chat_height' => array(
        '#default_value' => variable_get('chatblock_chat_height', ''),
        '#title' => t('Chat window height'),
        '#description' => t('Override all stylesheet definitions for the chat window height. Use any valid CSS height value, e.g. <em>200px</em> or <em>20em</em>. Use no spaces or quotes and no trailing semicolon. Leave empty to not override CSS.'),
        '#type' => 'textfield',
        '#size' => 6,
        '#maxlength' => 7,
      ),
      'chatblock_number_messages' => array(
        '#type' => 'textfield',
        '#title' => t('Startup message count'),
        '#size' => 4,
        '#maxlength' => 3,
        '#description' => t(
          "The maximum number of messages to be displayed at block's initial load. Leavy empty to start with all available messages (not recommended). <strong>The effictive message count may be additionally limited by the value below.</strong>"
        ),
        '#default_value' => variable_get(
          'chatblock_number_messages',
          CHATBLOCK_STARTUP_MESSAGES_DEFAULT
        ),
      ),
      'chatblock_maximum_retrospect' => array(
        '#type' => 'textfield',
        '#title' => t('Maximum retrospect time'),
        '#description' => t('Limit the time range for the initial display of recent messages in the chat window. Leave empty for no limitation (not recommended). <strong>The effective results may be additionally limited by the value above.</strong>'),
        '#size' => 4,
        '#maxlength' => 4,
        '#field_suffix' => t('minutes'),
        '#default_value' => variable_get(
          'chatblock_maximum_retrospect',
          CHATBLOCK_RETROSPECT_DEFAULT
        ),
      ),
      'chatblock_input_label' => array(
        '#type' => 'textfield',
        '#title' => t('Input field label'),
        '#description' => t('Optional prefix for the chat form input field, e.g. "Your message:"'),
        '#size' => 30,
        '#maxlength' => 128,
        '#default_value' => variable_get('chatblock_input_label', t('Enter your message')),
      ),
      'chatblock_minimize_input_form' => array(
        '#default_value' => variable_get('chatblock_minimize_input_form', 1),
        '#title' => t('Minimize input form'),
        '#description' => t('Will turn the submit button invisible and move the input field label into the empty input field when Javascript is enabled.'),
        '#type' => 'checkbox',
      ),
      'chatblock_permission_hint' => array(
        '#type' => 'textfield',
        '#title' => t('Hint on missing permissions'),
        '#description' => t('Enter an optional message that informs users about how to obtain permissions to join the chat unless they already own them.'),
        '#size' => 60,
        '#maxlength' => 255,
        '#default_value' => variable_get('chatblock_permission_hint', ''),
      ),
      'function_links' => array(
        '#type' => 'fieldset',
        'chatblock_show_function_links' => array(
          '#type' => 'radios',
          '#default_value' => variable_get('chatblock_show_function_links', CHATBLOCK_LINKS_SHOW_CONDITIONAL),
          '#title' => t('Show function links'),
          '#description' => t('Display links to chat block functions (format infos, logs) in the chatblock if the user has permission to see them.'),
          '#options' => array(
            CHATBLOCK_LINKS_SHOW_NEVER => t('never'),
            CHATBLOCK_LINKS_SHOW_CONDITIONAL => t('as contextual links if possible'),
            CHATBLOCK_LINKS_SHOW_ALWAYS => t('always below chat window'),
          ),
        ),
      ),
      'chatblock_guest_prefix' => array(
        '#type' => 'textfield',
        '#title' => t('Guest name prefix'),
        '#description' => t('If you allow anonymous users to join your chat, their display name will consist of this prefix and a snippet from their Drupal session ID. <strong>Note:</strong> Changing this prefix will not affect existing messages.'),
        '#size' => 60,
        '#maxlength' => 255,
        '#default_value' => variable_get('chatblock_guest_prefix', t('Guest') . '-'),
      ),
    ),
    'chatblock_system_messages' => array(
      '#type' => 'fieldset',
      '#title' => t('System messages'),
      '#description' => ('Activate or deactivate inline chat messages and information.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'chatblock_show_timestamps' => array(
        '#type' => 'checkbox',
        '#title' => t('Add timestamp'),
        '#description' => t('Adds a timestamp to all chat messages and events.'),
        '#default_value' => variable_get(
          'chatblock_show_timestamps', 1
        ),
      ),
      'chatblock_timestamp_tooltip' => array(
        '#title' => t('Timestamps as tooltips'),
        '#description' => t('Show timestamps as tooltips while hovering each message instead of displaying them permanently. Recommended if chatblock resides in a narrow side bar.'),
        '#type' => 'checkbox',
        '#default_value' => variable_get('chatblock_timestamp_tooltip', 0),
      ),
      'chatblock_timestamp_format' => array(
        '#type' => 'textfield',
        '#title' => t('Timestamp format'),
        '#description' => t(
          'Configure the timestamp format. Use placeholders as described in <a href="@php_date_url">PHP date()</a>, but restrict to the following tokens (otherwise, you will receive odd results for some reasons): <em>Y, y, m, n, d, j, H, G, i, s</em>.',
          array(
            '@php_date_url' => 'http://php.net/manual/function.date.php'
          )
        ),
        '#size' => 30,
        '#maxlength' => 45,
        '#default_value' => variable_get(
          'chatblock_timestamp_format',
          'G:i:s'
        ),
      ),
      'chatblock_show_logins' => array(
        '#type' => 'checkbox',
        '#title' => t('Enable login messages'),
        '#description' => t('Display a system message when a user has logged in.')
          . t('Consider privacy impacts and the fact, that, due to persistant session cookies, this information is not very reliable anyway.')
        ,
        '#default_value' => variable_get('chatblock_show_logins', 0)
      ),
      'chatblock_show_logouts' => array(
        '#type' => 'checkbox',
        '#title' => t('Enable logout messages'),
        '#description' => t('Display a system message when a user has logged out.')
          . t('Consider privacy impacts and the fact, that, due to persistant session cookies, this information is not very reliable anyway.')
        ,
        '#default_value' => variable_get('chatblock_show_logouts', 0)
      ),
    ),
    'chatblock_log_info_options' => array(
      '#type' => 'fieldset',
      '#title' => t('Log and info page options'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'chatblock_infopage_custom_content' => array(
        '#type' => 'text_format',
        '#title' => t('Custom info page content'),
        '#weight' => 98,
        '#default_value' => $custom_info_page_content['value'],
        '#rows' => 6,
        '#format' => $custom_info_page_content['format'],
        '#description' => t(
          'Optional markup that can be displayed at the top of the <a href="@infopage_url">chat info page</a>.',
          array('@infopage_url' => url('chatblock/info'))
        ),
      ),
      'chatblock_log_messages_per_page' => array(
        '#default_value' => variable_get('chatblock_log_messages_per_page', 72),
        '#title' => t('Messages per page'),
        '#description' => t('Maximum number of messages displayed per log page. Set to "0" to have all messages on one page (are you sure?!).'),
        '#type' => 'textfield',
        '#size' => 4,
        '#maxlength' => 4,
      ),
      'chatblock_timestamp_format_logs' => array(
        '#type' => 'textfield',
        '#title' => t('Timestamp format for logs'),
        '#description' => t(
          'Configure the timestamp format. Use placeholders as described in <a href="@php_date_url">PHP date()</a>, but restrict to the following tokens (otherwise, you will receive odd results for some reasons): <em>Y, y, m, n, d, j, H, G, i, s</em>.',
          array(
            '@php_date_url' => 'http://php.net/manual/function.date.php'
          )
        ),
        '#size' => 30,
        '#maxlength' => 45,
        '#default_value' => variable_get(
          'chatblock_timestamp_format_logs',
          'm/d/y H:i:s'
        ),
      ),
      'chatblock_cron_settings' => array(
        '#type' => 'fieldset',
        '#title' => t('Clean up chat logs'),
        '#description' => t('Removes messages from the chat log after the given time period. Note that you need to configure a cronjob for this task to work. Set the value to 0 to disable automatic clean-ups.'),
        'chatblock_cron_table' => array(
          '#theme' => 'chatblock_cron_form',
          'chatblock_autodelete_value' => array(
            '#type' => 'textfield',
            '#default_value' => variable_get('chatblock_autodelete_value', 0),
            '#size' => 2,
            '#maxlength' => 2,
            '#required' => TRUE,
          ),
          'chatblock_autodelete_unit' => array(
            '#type' => 'select',
            '#options' => array(
              t('minute(s)'),
              t('hour(s)'),
              t('day(s)'),
              t('week(s)'),
              t('month(s)'),
              t('year(s)'),
            ),
            '#default_value' => variable_get('chatblock_autodelete_unit', 0),
          ),
        ),
      ),
    ),
    'chatblock_performance_relevant' => array(
      '#type' => 'fieldset',
      '#title' => t('Additional/performance options'),
      '#description' => t('Each of these settings may enhance your chat but does this at the cost of performance. Keep this in mind when configuring.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      'chatblock_poll' => array(
        '#type' => 'fieldset',
        '#title' => t('Poll rate'),
        'chatblock_poll_rate_default' => array(
          '#type' => 'textfield',
          '#size' => 4,
          '#maxlength' => 3,
          '#title' => t('Default poll interval'),
          '#field_suffix' => t('seconds'),
          '#description' => t('The default time before a client will update its chat window. Shorter intervals mean more server activity, 0 means the chat will not be updated by javascript at all.'),
          '#default_value' => variable_get(
            'chatblock_poll_rate_default',
            CHATBLOCK_POLL_RATE_DEFAULT
          ),
          '#required' => TRUE,
        ),
        'chatblock_smart_poll_stepwidth' => array(
          '#title' => t('Smart poll stepwidth'),
          '#description' => ('Smart polling constantly increases the poll interval as long as there is no chat activity (and restores it as soon as life returns to the chat). The stepwidth is the time before the default poll interval is added to the actual poll interval. If e.g. you set a step width of 2 minutes and a default poll interval of 5 seconds, the actual intervall will be 10 seconds after 2 minutes idle time and 150 seconds after one hour. Setting the value to 0 will disable smart polling and constantly use the default poll interval instead.'),
          '#type' => 'textfield',
          '#size' => 4,
          '#maxlength' => 4,
          '#field_suffix' => t('minutes'),
          '#default_value' => variable_get(
            'chatblock_smart_poll_stepwidth',
            2
          ),
          '#required' => TRUE,
        ),
        'chatblock_poll_rate_max' => array(
          '#default_value' => variable_get(
            'chatblock_poll_rate_max',
            CHATBLOCK_POLL_RATE_MAX_DEFAULT
          ),
          '#title' => t('Maximum poll interval (minutes)'),
          '#description' => t('Limits smart polling to a maximum interval. The value will always be adjusted not to be smaller than your default poll interval (see above).'),
          '#options' => array(t('Please select'), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 20),
          '#type' => 'select',
          '#required' => TRUE,
        ),
      ),
      'chatblock_link_users' => array(
        '#type' => 'checkbox',
        '#title' => t('Link user profiles'),
        '#description' => t("Adds a profile link to each registered user's name."),
        '#default_value' => variable_get('chatblock_link_users', 1)
      ),
      'chatblock_add_smileys' => array(
        '#type' => 'checkbox',
        '#title' => t('Smileys'),
        '#description' => t("Transforms emoticon codes into graphical smileys."),
        '#default_value' => variable_get('chatblock_add_smileys', 1)
      ),
      'chatblock_detect_urls' => array(
        '#type' => 'checkbox',
        '#title' => t('Detect URLs'),
        '#description' => t("Tries to detect URLs and email addresses and transforms them into links."),
        '#default_value' => variable_get('chatblock_detect_urls', 1)
      ),
    ),
  );
  $form['#validate'][] = '_chatblock_settings_validate';
  $form['#validate'][] = '_chatblock_settings_validate_cleanup';
  $form['#submit'][] = '_chatblock_settings_check_max_poll';

  return system_settings_form($form);
}
