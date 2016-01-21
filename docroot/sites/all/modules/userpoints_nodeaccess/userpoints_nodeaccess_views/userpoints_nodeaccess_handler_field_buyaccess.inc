<?php
// $Id$

/**
 * @file
 * Views handler: Display access status of the node and if possible, a get
 * access form.
 */

/**
 * Display access status of the node and if possible, a get access form.
 */
class userpoints_nodeaccess_handler_field_buyaccess extends views_handler_field {
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    $this->additional_fields['type'] = array('table' => 'node', 'field' => 'type');
    $this->additional_fields['uid'] = array('table' => 'node', 'field' => 'uid');
    $this->additional_fields['category'] = array('table' => 'userpoints_nodeaccess_points', 'field' => 'tid');
    $this->additional_fields['points'] = array('table' => 'userpoints_nodeaccess_points', 'field' => 'points');
  }

  function element_type() {
    if (isset($this->definition['element type'])) {
      return $this->definition['element type'];
    }
    return 'div';
  }

  function render($values) {
    global $user;

    $nid = $values->{$this->aliases['nid']};
    $type = $values->{$this->aliases['type']};
    $uid = $values->{$this->aliases['uid']};
    $category = $values->{$this->aliases['category']};
    $points = $values->{$this->aliases['points']};

    // Check if node type is enabled and node as points assigned.
    if (empty($points) || !variable_get('userpoints_nodeaccess_enabled_' . $type, variable_get('userpoints_nodeaccess_enabled', TRUE))) {
      return '<span class="userpoints-nodeaccess userpoints-nodeaccess-unlimited">' . t('Unlimited Access') . '</span>';
    }

    // Don't limit access to users with bypass permission, the author or if the
    // user already has access.
    if (user_access('bypass node access') || $uid == $user->uid || userpoints_nodeaccess_has_access($nid, $user->uid)) {
      return '<span class="userpoints-nodeaccess userpoints-nodeaccess-granted">' . t('Granted') . '</span>';
    }

    // Check if user is allowed to trade points.
    if (!user_access('trade userpoints for node access')) {
      if ($user->uid > 0) {
        return '<span class="userpoints-nodeaccess userpoints-nodeaccess-denied">' . t('Denied') . '</span>';
      }
      else {
        return l(t('Login to view status'), 'user/login', array('attributes' => array('class' => array('userpoints-nodeaccess', 'userpoints-nodeaccess-denied')), 'query' => drupal_get_destination()));
      }
    }

    $current_points = userpoints_get_current_points($user->uid, $category);
    $categories = userpoints_get_categories();
    $category = $categories[$category];

    // Check if user has enough points.
    $difference = $points - $current_points;
    if ($difference > 0) {
      return '<span class="userpoints-nodeaccess userpoints-nodeaccess-needmore">' . t('Need @points more !points', array_merge(userpoints_translation(), array('@points' => $difference)));
    }

    return drupal_render(drupal_get_form('userpoints_nodeaccess_trade_access_form', $nid, $type));
  }
}
