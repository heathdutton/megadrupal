<?php

/**
 * @file
 * Administration forms for the Read More module.
 */

/**
 * Displays the settings form.
 */
function read_more_admin_settings() {
  $form = array();

  /* Per content type + view mode support */
  $content_types = node_type_get_types();
  $view_modes = read_more_view_modes_options_list();

  $elements = array(
    'address'    => '<address>',
    'blockquote' => '<blockquote>',
    'cite'       => '<cite>',
    'div'        => '<div>',
    'h1'         => '<h1>',
    'h2'         => '<h2>',
    'h3'         => '<h3>',
    'h4'         => '<h4>',
    'h5'         => '<h5>',
    'h6'         => '<h6>',
    'p'          => '<p>',
    'span'       => '<span>',
  );

  $form['read_more_behavior'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link behavior'),
    '#collapsible' => FALSE,
  );
    $form['read_more_behavior']['read_more_placement'] = array(
      '#type' => 'radios',
      '#title' => t('Link placement'),
      '#options' => array(
        'inline'  => t('Inline: Try to add the Read More link after the last word of the teaser. If this fails, add the link on a new line after the teaser.'),
        'after'   => t('On a new line: Add the Read More link on a new line after the teaser.'),
        'disable' => t('Disable the link: Do not add a Read More link to the teaser.'),
      ),
      '#default_value' => variable_get('read_more_placement', READ_MORE_PLACEMENT_DEFAULT),
      '#description' => t('The inline option will attempt to add the Read More link after the last word of the teaser and before any CCK fields. The HTML elements into which the Read More link may be inserted can be chosen in the "Advanced options for inline placement" interface below.'),
    );
    $form['read_more_behavior']['read_more_placement_advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced options for inline placement'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
      $form['read_more_behavior']['read_more_placement_advanced']['read_more_elements'] = array(
        '#type' => 'select',
        '#title' => t('Elements eligible for inline placement'),
        '#description' => t('Select the elements into which the Read More link may be inserted. The "Inline" placement option must be selected above.'),
        '#multiple' => TRUE,
        '#options' => $elements,
        '#default_value' => variable_get('read_more_elements', array('p')),
        '#size' => 10,
      );
    $form['read_more_behavior']['read_more_remove'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove Read More link from links section'),
      '#default_value' => variable_get('read_more_remove', TRUE),
      '#description' => t('Enabling this option will remove Drupal\'s default Read More link from the node links.'),
    );
    $form['read_more_behavior']['read_more_rss'] = array(
      '#type' => 'checkbox',
      '#title' => t('Replace Read More link in RSS feeds'),
      '#default_value' => variable_get('read_more_rss', TRUE),
    );
    $form['read_more_behavior']['read_more_require_body_field'] = array(
      '#type' => 'checkbox',
      '#title' => t('Do not display if the body text is empty'),
      '#default_value' => variable_get('read_more_require_body_field', FALSE),
    );
    $form['read_more_behavior']['read_more_anchor'] = array(
      '#type' => 'checkbox',
      '#title' => t('Skip to unread content (SEE WARNING BELOW)'),
      '#default_value' => variable_get('read_more_anchor', FALSE),
      '#description' => t('Enabling this option will add an anchor to the destination page so that the user skips past the content they already saw in the teaser. WARNING: This feature is still being tested! Please report any problems you experience in the <a href="@link">Read More issue queue</a>.', array('@link' => url('http://drupal.org/project/issues/read_more'))),
    );

  $form['read_more_formatting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link text and formatting'),
    '#description' => t('Here you can specify the wording of the Read More link, change its appearance by wrapping it in markup, and use tokens to display information about the node.'),
    '#collapsible' => FALSE,
  );
    $form['read_more_formatting']['read_more_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Read More Wrapper Text'),
      '#default_value' => variable_get('read_more_text', '[node:read-more:link]'),
      '#description' => t('The full "Read More" text.  Defaults to [node:read-more:link]'),
      '#required' => TRUE,
    );
    $form['read_more_formatting']['read_more_link_text'] = array(
      '#type' => 'textfield',
      '#title' =>  t('Link text'),
      '#default_value' => variable_get('read_more_link_text', READ_MORE_LINK_TEXT_DEFAULT),
      '#description' => t('Enter the text you wish to display in the Read More link. Special characters should be encoded (like <code>&amp;raquo;</code> or <code>&amp;amp;</code>). Allowed HTML is listed below.'),
      '#required' => TRUE,
    );
    $form['read_more_formatting']['allowed_html'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Allowed HTML'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
      '#value'       => t('The following HTML is allowed in the link text field above:') . ' <code>abbr</code>, <code>acronym</code>, <code>b</code>, <code>big</code>, <code>cite</code>, <code>code</code>, <code>del</code>, <code>em</code>, <code>i</code>, <code>img</code>, <code>ins</code>, <code>small</code>, <code>span</code>, <code>strong</code>, <code>sub</code>, <code>sup</code>',
    );
    $form['read_more_formatting']['read_more_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Link title attribute'),
      '#default_value' => variable_get('read_more_title', ''),
      '#description' => t('Enter the text you wish to be used as the title for the Read More link (the value of the <code>title=""</code> attribute). The link title is used for accessibility and search engine optimization and appears as a tooltip in some browsers.'),
      '#required' => FALSE,
    );
    $form['read_more_formatting']['read_more_nofollow'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make link nofollow'),
      '#default_value' => variable_get('read_more_nofollow', TRUE),
      '#description' => t('Adds <code>rel="nofollow"</code> to the link\'s attributes. Often used for search engine optimization.<br/><strong>CAUTION: If there are two links to the same URL, one with "nofollow", the other without, some search engines will treat BOTH as "nofollow" links.</strong>'),
    );
    $form['read_more_formatting']['read_more_newwindow'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make link open in a new window'),
      '#default_value' => variable_get('read_more_newwindow', FALSE),
      '#description' => t('Adds <code>target="_blank"</code> to the link\'s attributes.'),
    );
    $form['read_more_formatting']['read_more_absolute_link_modes'] = array(
      '#type' => 'checkboxes',
      '#options' => $view_modes,
      '#title' => t('Absolute Links'),
      '#default_value' => variable_get('read_more_absolute_link_modes', array()),
      '#description' => t('Which display modes should use absolute links?'),
    );

  $form['read_more_attachment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Types'),
    '#description' => t('Select the content types and the associated view modes this should apply to.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  foreach ($content_types as $type) {
    $form['read_more_attachment']['read_more_' . $type->type] = array(
      '#type' => 'fieldset',
      '#title' => $type->name,
      '#description' => $type->description,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['read_more_attachment']['read_more_' . $type->type]['read_more_' . $type->type . '_view_modes'] = array(
      '#type' => 'checkboxes',
      '#options' => $view_modes,
      '#title' => t('View Modes'),
      '#default_value' => variable_get('read_more_' . $type->type . '_view_modes', array()),
    );
  }

  // Add help text for tokens and list of tokens if the Tokens module is enabled.
  $token_description = t('Use the <code>[node:title]</code> token to insert the node title.');
  if (module_exists('token')) {
    $token_description = t('Other available <a href="@tokens">tokens</a> are listed below.', array('@tokens' => url('http://drupal.org/project/token')));
    $form['read_more_tokens'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Tokens'),
      '#description' => t('The following tokens can be used in the Read More link text and title attribute.'),
    );
    $form['read_more_tokens']['list'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
    );
  }
  $form['read_more_formatting']['read_more_text']['#description'] .= ' ' . $token_description;
  $form['read_more_formatting']['read_more_title']['#description'] .= ' ' . $token_description;

  $form['#validate'][] ='read_more_admin_settings_validate';

  return system_settings_form($form);
}

function read_more_admin_settings_validate($form, &$form_state) {
  $no_modes = TRUE;
  $content_types = node_type_get_types();

  foreach ($content_types as $type) {
    $enabled_set = array_filter($form_state['values']['read_more_' . $type->type . '_view_modes']);
    if (!empty($enabled_set)) {
      $no_modes = FALSE;
    }
  }

  // Generate a warning if the admin hasn't selected any view modes of any content types.
  if ($no_modes) {
    drupal_set_message(t('No view modes were enabled on any content types.'), 'warning');
  }
}
