<?php
// $Id: userpoints_nodeaccess.theme.inc,v 1.2 2010/11/23 14:08:56 berdir Exp $

/**
 * @file
 * Theme functions for userpoints_nodeaccess.module
 */

function theme_userpoints_nodeaccess_node_points_information($variables) {
  $args = array(
    '@category' => $variables['category'],
    '@points' => $variables['points'],
    '@price' => $variables['price'],
  );

  if ($variables['price'] <= $variables['points']) {
    return '<div class="userpoints-nodeaccess userpoints-nodeaccess-tradein" >' . t('Requires @price !points in the @category category. You have @points !points available.', array_merge(userpoints_translation(), $args)) . '</div>';
  }
  else {
    return '<div class="userpoints-nodeaccess userpoints-nodeaccess-needmore" >' . t('Requires @price !points in the @category category. You only have @points !points available.', array_merge(userpoints_translation(), $args)) . '</div>';
  }
}
