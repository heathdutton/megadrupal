<?php
// $Id: theme.inc,v 1.1 2010/12/12 16:22:35 manuelbs Exp $

// @file this file containes all theming functioins for the module bs_uniquenode

/**
 * Theme the message of a non unique node, displayed on submitform of a node
 */
function theme_matching_node_message($variables) {
  $nid = $variables['nid'];
  $value = $variables['value'];

  $link = 'node/' . $nid;

  $options = array(
    'attributes' => array(
        'class' => array(
            'new_window',
        ),
        'target=' => '_blank',
    ),
  );
  return l($value, $link, $options);
}

/**
 * Make the messages in adminsettings themeable
 */
function theme_uniquity_check_field_explanation($variables) {
  $message = $variables['message'];
  return '<div class="uniquity_check_field_explanation">' . $message . '</div>';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_uniquity_check_field($variables) {
  $fieldlink = $variables['fieldlink'];
  $fieldname = $variables['fieldname'];
  $checktype = $variables['checktype'];

  return '<div class="uniquity_check_field">' .
          l($fieldname . '(' . $checktype . ')', $fieldlink). '</div>';
}
