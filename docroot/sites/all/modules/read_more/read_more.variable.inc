<?php
/**
 * @file Variable definitions
 */

/**
 * Implements hook_variable_info().
 */
function read_more_variable_info($options) {
  $variables = array();

  $variables['read_more_placement'] = array(
    'title' => t('Link placement', array(), $options),
    'group' => 'read_more_behavior',
    'type' => 'select',
    'default' => READ_MORE_PLACEMENT_DEFAULT,
    'options' => array(
      'inline'  => t('Inline: Try to add the Read More link after the last word of the teaser. If this fails, add the link on a new line after the teaser.'),
      'after'   => t('On a new line: Add the Read More link on a new line after the teaser.'),
      'disable' => t('Disable the link: Do not add a Read More link to the teaser.'),
    ),
    'description' => t('The inline option will attempt to add the Read More link after the last word of the teaser and before any CCK fields. The HTML elements into which the Read More link may be inserted can be chosen in the "Advanced options for inline placement" interface below.', array(), $options),
  );

  $variables['read_more_elements'] = array(
    'title' => t('Elements eligible for inline placement', array(), $options),
    'type' => 'options',
    'description' => t('Select the elements into which the Read More link may be inserted. The "Inline" placement option must be selected above.', array(), $options),
    'options' => array(
      'address'    => check_plain('<address>'),
      'blockquote' => check_plain('<blockquote>'),
      'cite'       => check_plain('<cite>'),
      'div'        => check_plain('<div>'),
      'h1'         => check_plain('<h1>'),
      'h2'         => check_plain('<h2>'),
      'h3'         => check_plain('<h3>'),
      'h4'         => check_plain('<h4>'),
      'h5'         => check_plain('<h5>'),
      'h6'         => check_plain('<h6>'),
      'p'          => check_plain('<p>'),
      'span'       => check_plain('<span>'),
    ),
    'default' => array('p'),
  );

  $variables['read_more_remove'] = array(
    'title' => t('Remove Read More link from links section', array(), $options),
    'group' => 'read_more_behavior',
    'type' => 'boolean',
    'description' => t('Enabling this option will remove Drupal\'s default Read More link from the node links.', array(), $options),
    'default' => TRUE,
  );

  $variables['read_more_rss'] = array(
    'title' => t('Replace Read More link in RSS feeds', array(), $options),
    'group' => 'read_more_behavior',
    'type' => 'boolean',
    'default' => TRUE,
  );

  $variables['read_more_require_body_field'] = array(
    'title' => t('Do not display if the body text is empty', array(), $options),
    'group' => 'read_more_behavior',
    'type' => 'boolean',
    'default' => FALSE,
  );

  $variables['read_more_anchor'] = array(
    'title' => t('Skip to unread content (SEE WARNING BELOW)', array(), $options),
    'group' => 'read_more_behavior',
    'type' => 'boolean',
    'description' => t('Enabling this option will add an anchor to the destination page so that the user skips past the content they already saw in the teaser. WARNING: This feature is still being tested! Please report any problems you experience in the <a href="@link">Read More issue queue</a>.', array('@link' => url('http://drupal.org/project/issues/read_more')), $options),
    'default' => FALSE,
  );

  $variables['read_more_text'] = array(
    'title' => t('Read More Wrapper Text', array(), $options),
    'group' => 'read_more_formatting',
    'type' => 'string',
    'description' => t('The full "Read More" text.  Defaults to [node:read-more:link]'),
    'default' => '[node:read-more:link]',
    'localize' => TRUE,
  );

  $variables['read_more_link_text'] = array(
    'title' => t('Link Text', array(), $options),
    'group' => 'read_more_formatting',
    'type' => 'string',
    'localize' => TRUE,
    'description' => t('Enter the text you wish to display in the Read More link. Special characters should be encoded (like <code>&amp;raquo;</code> or <code>&amp;amp;</code>). Allowed HTML is listed below.', array(), $options),
    'default' => READ_MORE_LINK_TEXT_DEFAULT,
  );

  $variables['read_more_title'] = array(
    'title' => t('Link title attribute', array(), $options),
    'group' => 'read_more_formatting',
    'type' => 'string',
    'description' => t('Enter the text you wish to be used as the title for the Read More link (the value of the <code>title=""</code> attribute). The link title is used for accessibility and search engine optimization and appears as a tooltip in some browsers.'),
    'default' => '',
  );

  $variables['read_more_nofollow'] = array(
    'title' => t('Make link nofollow', array(), $options),
    'group' => 'read_more_formatting',
    'type' => 'boolean',
    'default' => FALSE,
    'description' => t('Adds <code>rel="nofollow"</code> to the link\'s attributes. Often used for search engine optimization.<br/><strong>CAUTION: If there are two links to the same URL, one with "nofollow", the other without, some search engines will treat BOTH as "nofollow" links.</strong>', array(), $options),
  );

  $variables['read_more_newwindow'] = array(
    'title' => t('Make link open in a new window', array(), $options),
    'group' => 'read_more_formatting',
    'type' => 'boolean',
    'default' => FALSE,
    'description' => t('Adds <code>target="_blank"</code> to the link\'s attributes.', array(), $options),
  );

  $variables['read_more_[node_type]_view_modes'] = array(
    'type' => 'multiple',
    'group' => 'read_more_attachment',
    'title' => t('Enable read more link per content type', array(), $options),
    'repeat' => array(
      'type' => 'options',
      'default' => array(),
      'options callback' => 'read_more_view_modes_options_list',
    ),
  );

  return $variables;
}

function read_more_variable_group_info() {
  $groups = array();

  $groups['read_more_behavior'] = array(
    'title' => t('Link behavior'),
    'access' => 'administer site configuration',
  );

  $groups['read_more_formatting'] = array(
    'title' => t('Link text and formatting'),
    'description' => t('Here you can specify the wording of the Read More link, change its appearance by wrapping it in markup, and use tokens to display information about the node.'),
    'access' => 'administer site configuration',
  );

  $groups['read_more_attachment'] = array(
    'title' => t('Content types'),
    'description' => t('Select the content types and the associated view modes this should apply to.'),
    'access' => 'administer site configuration',
  );

  return $groups;
}
