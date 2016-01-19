<?php

/**
 * @file
 * provides admin pages for Comment Easy Reply module.
 */

/**
 * The module's settings form.
 */
function comment_easy_reply_settings_form($form, &$form_state) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment Easy Reply settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['settings'] += _comment_easy_reply_settings_form($form_state);
  $form['#validate'][] = 'comment_easy_reply_settings_form_validate';
  $form['#submit'][] = 'comment_easy_reply_settings_form_submit';
  $active_types = _comment_easy_reply_get_active_node_types();
  if (empty($active_types)) {
    drupal_set_message(t('Comment Easy Reply is currently not enabled for any node type. Control node type status on !status_page.',
    array('!status_page' => l(t('status page'), 'admin/config/content/comment-easy-reply/status'))), 'warning');
  }
  return system_settings_form($form);
}

/**
 * Generates the settings form.
 *
 * @param array $form_state
 *   The form values.
 * @param string $node_type
 *   The node type. It is used in node settings form, while in general settings
 *   page, the value is NULL.
 */
function _comment_easy_reply_settings_form(&$form_state, $node_type = NULL) {
  $form = array();
  $suffix = '';
  if (!is_null($node_type) && !empty($node_type)) {
    $suffix = '_' . $node_type;
  }
  $form['comment_easy_reply_node_type'] = array(
    '#type' => 'hidden',
    '#value' => $node_type,
  );
  $form['behaviours'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['tooltip'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['fragment'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['comment'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['behaviours']['comment_easy_reply_reply_from_numlink' . $suffix] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to reply to a comment by clicking on comment #NUM link.'),
    '#description' => t('If this option is not selected, the comment #NUM link will be a permalink.'),
    '#default_value' => variable_get('comment_easy_reply_reply_from_numlink' . $suffix, COMMENT_EASY_REPLY_DEFAULT_REPLY_FROM_NUMLINK),
  );
  $modes = _comment_easy_reply_get_tag_form_options();
  $form['behaviours']['comment_easy_reply_link_mode' . $suffix] = array(
    '#type' => 'radios',
    '#title' => t('Link mode'),
    '#options' => $modes,
    '#description' => t('Select the mode the reference to a comment will be shown into comment body'),
    '#default_value' => variable_get('comment_easy_reply_link_mode' . $suffix, COMMENT_EASY_REPLY_DEFAULT_LINK_MODE),
  );
  $form['tooltip']['comment_easy_reply_replytip_activated' . $suffix] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate reply tooltip.'),
    '#description' => t('This checkbox controls the tooltip that appears passing the mouse over comment #NUM link.'),
    '#default_value' => variable_get('comment_easy_reply_replytip_activated' . $suffix, COMMENT_EASY_REPLY_DEFAULT_REPLYTIP_ACTIVATED),
  );
  $form['tooltip']['comment_easy_reply_replytip_text' . $suffix] = array(
    '#type' => 'textfield',
    '#title' => t('Text inside comment reply tooltip'),
    '#description' => t('This text will be used as link inside any comment reply tooltip.'),
    '#default_value' => variable_get('comment_easy_reply_replytip_text' . $suffix, COMMENT_EASY_REPLY_DEFAULT_REPLYTIP_TEXT),
  );
  $form['tooltip']['comment_easy_reply_referrertip_text' . $suffix] = array(
    '#type' => 'textarea',
    '#title' => t('Text inside comment referrer tooltip'),
    '#description' => t('This text will be used to compose referrer comment
    text inside any comment tooltip.'), '#default_value' =>
    variable_get('comment_easy_reply_referrertip_text' . $suffix, COMMENT_EASY_REPLY_DEFAULT_REFERRERTIP_TEXT),
  );
  $form['fragment']['comment_easy_reply_fragment_activated' . $suffix] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate the custom fragment.'),
    '#description' => t("This checkbox allows to change the default fragment in comment's permalink, title and links from #comment-CID to #reply-NUM."),
    '#default_value' => variable_get('comment_easy_reply_fragment_activated' . $suffix, COMMENT_EASY_REPLY_DEFAULT_FRAGMENT_ACTIVATED),
  );
  $form['fragment']['comment_easy_reply_fragment_text' . $suffix] = array(
    '#type' => 'textfield',
    '#title' => t('Text of the fragment'),
    '#description' => t('This text will be used as link inside any comment reply tooltip.'),
    '#default_value' => variable_get('comment_easy_reply_fragment_text' . $suffix, COMMENT_EASY_REPLY_DEFAULT_FRAGMENT_TEXT),
  );
  $form['comment']['comment_easy_reply_comment_title' . $suffix] = array(
    '#type' => 'textfield',
    '#title' => t('Predefined comment reply title'),
    '#description' => t('This text will appear inside comment reply subject form to let user use a custom reply subject.'),
    '#default_value' => variable_get('comment_easy_reply_comment_title' . $suffix, COMMENT_EASY_REPLY_DEFAULT_REPLY_TITLE),
  );
  if (module_exists('token')) {
    $form['tokens']['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('comment', 'comment_easy_reply'),
    );
  }
  else {
    $form['tokens']['tokens'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view all the available tokens.',
      array(
        '@drupal-token' => 'http://drupal.org/project/token')
      ) . '</p>',
    );
  }
  $filters = array('' => 'None');
  $filter_info = filter_formats();
  foreach ($filter_info as $name => $filter) {
    $filters[$name] = $filter->name;
  }
  if (!empty($filters)) {
    $description = t('Apply a filter to comment text contained inside tooltips.');
    $form['tooltip']['comment_easy_reply_filter' . $suffix] = array(
      '#title' => 'Tooltip filter',
      '#description' => $description,
      '#type' => 'select',
      '#options' => $filters,
      '#default_value' => variable_get('comment_easy_reply_filter' . $suffix, ''),
    );
  }
  $tooltips = _comment_easy_reply_get_tooltips();
  $options = array();
  foreach ($tooltips as $name => $values) {
    if (!isset($values['display_settings']) || $values['display_settings']) {
      $options[$name] = $values['name'];
    }
  }
  $form['tooltip']['comment_easy_reply_tooltip_name' . $suffix] = array(
    '#type' => 'radios',
    '#title' => t('Tooltip style'),
    '#description' => t("Select a style to be applied to comment tooltips."),
    '#default_value' => variable_get('comment_easy_reply_tooltip_name' .
      $suffix, COMMENT_EASY_REPLY_DEFAULT_TOOLTIP_MODULE),
    '#options' => $options,
  );
  if (module_exists('quote')) {
    $form['quote'] = array(
      '#type' => 'fieldset',
      '#title' => t('Quote module integration'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['quote']['comment_easy_reply_quote_tooltip' . $suffix] = array(
      '#type' => 'checkbox',
      '#title' => t('Quote reply inside tooltip.'),
      '#description' => t("If enabled, comment quote will be rendered inside a tooltip as the same way of a comment reply."),
      '#default_value' => variable_get('comment_easy_reply_quote_tooltip' .
        $suffix, COMMENT_EASY_REPLY_DEFAULT_FRAGMENT_ACTIVATED),
    );
  }
  return $form;
}

/**
 * Validate callback function for module's settings form.
 */
function comment_easy_reply_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $suffix = '';
  if (isset($values['comment_easy_reply_node_type']) && !empty($values['comment_easy_reply_node_type'])) {
    $suffix = '_' . $values['comment_easy_reply_node_type'];
  }
  $fragment_active = $values['comment_easy_reply_fragment_activated' . $suffix];
  if ($fragment_active) {
    $text = $values['comment_easy_reply_fragment_text' . $suffix];
    if (empty($text)) {
      form_set_error('comment_easy_reply_fragment_text' . $suffix, t('Fragment text cannot be empty if activated.'));
    }
    else {
      $error = FALSE;
      $messages = _comment_easy_reply_validate_fragment_text($text, $error);
      if ($error) {
        foreach ($messages as $msg) {
          form_set_error('comment_easy_reply_fragment_text' . $suffix, $msg);
        }
      }
    }
  }
}

/**
 * Returns an array of options to populate the module settings form.
 *
 * @return array
 *   The array of options.
 */
function _comment_easy_reply_get_tag_form_options() {
  $options = array();
  $tags = _comment_easy_reply_get_tags();
  foreach ($tags as $module => $values) {
    if (is_array($values)) {
      foreach ($values as $mode => $mode_values) {
        $options[$module . '|' . $mode] = $mode_values['label'];
      }
    }
  }
  return $options;
}

/**
 * Submit callback function for module's settings form.
 */
function comment_easy_reply_settings_form_submit($form, &$form_state) {
  cache_clear_all('field:comment:', 'cache_field', TRUE);
}

/**
 * Page callback for Comment Easy Reply status report.
 *
 * @return string
 *   The HTML of the page.
 */
function comment_easy_reply_status_report_page() {
  $variables = array();
  $node_types = node_type_get_names();
  if (isset($_GET['enable'])) {
    $type_enable = check_plain($_GET['enable']);
    if ($type_enable && in_array($type_enable, array_keys($node_types))) {
      $actives = _comment_easy_reply_get_active_node_types();
      if (!in_array($type_enable, $actives)) {
        $actives[] = $type_enable;
        _comment_easy_reply_set_active_node_types($actives);
      }
      drupal_set_message(t('Comment Easy Reply has been enabled for node type %type',
      array('%type' => $node_types[$type_enable])));
    }
  }
  if (isset($_GET['disable'])) {
    $type_disable = check_plain($_GET['disable']);
    if ($type_disable && in_array($type_disable, array_keys($node_types))) {
      $actives = _comment_easy_reply_get_active_node_types();
      foreach ($actives as $pos => $type) {
        if ($type == $type_disable) {
          unset($actives[$pos]);
        }
      }
      _comment_easy_reply_set_active_node_types($actives);
      drupal_set_message(t('Comment Easy Reply has been disabled for node type %type',
      array('%type' => $node_types[$type_disable])));
    }
  }
  $variables['comment_easy_reply_node_types'] = variable_get('comment_easy_reply_node_types', array());
  return theme('comment_easy_reply_status_report_page', $variables);
}
