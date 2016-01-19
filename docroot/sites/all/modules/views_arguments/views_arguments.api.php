<?php

/**
 * Implements hook_views_arguments_any_field__VIEW__DISPLAY__FIELD().
 *
 * Depending on FIELD, you know what kind of entity $entity is (node, user etc).
 *
 * Valid function names could be:
 * - hook_views_arguments_any_field__admin_node_content__default__any_node
 * - hook_views_arguments_any_field__fav_content__page__any_user_1  (2nd 'any_user' field)
 * - etc. 'FIELD' will always have a format like 'any_ENTITY_TYPE[_N]'
 *
 * $context contains
 * - 'handler' - the views field handler, which contains ->view, ->query etc
 * - 'values' - the db record for this row, maybe with 'nid' or 'uid' or something
 */
function hook_views_arguments_any_field__VIEW__DISPLAY__FIELD($entity, $context) {
  // Any $entity could do this:
  $magic = new MyMagic($entity);
  $magic->calculateStuff(menu_get_object());
  $html = $magic->renderStuff();
  return $html;

  // A $node could do this to show its enabled domains:
  $dids = $node->domains;
  $domains = domain_load_multiple($dids); // I wish
  $domain_names = array_map(function($domain) {
    return $domain['name'];
  }, $domains);
  return check_plain(implode(', ', $domain_names));

  // A $user could show the IP address of the hostname of its init mail:
  $components = explode('@', $user->init);
  $hostname = $components[1];
  $ip = gethostbyname($hostname);
  return $ip;
}



/**
 * Implements views_arguments_argument_callback_info().
 */
function hook_views_arguments_argument_callback_info() {
  $callbacks['user_organization'] = array(
    // Mandatory title. Translate it if you want.
    'title' => "Curent user's organization",

    // Mandatory callback, can be Closure, array(Class, Method), String etc.
    'callback' => 'YOURMODULE_views_argument__user_organization',
    'callback' => function($langcode) {
      global $user;
      return $user->field_organization[$langcode][0]['target_id'];
    },

    // Optional arguments.
    'arguments' => array('und'),
  );
  return $callbacks;
}

/**
 * Implements views_arguments_argument_callback_info_alter().
 */
function hook_views_arguments_argument_callback_info_alter(&$callbacks) {
  // Read organization from another field with this callback.
  $callbacks['user_organization']['callback'] = 'MYMODULE_views_argument__user_organization';

  // Read from 'en' field instead.
  $callbacks['user_organization']['arguments'][0] = 'en';
}

/**
 * Views Arguments argument callback for 'user_organization'.
 *
 * The first argument will always be the Views argument handler. The rest is up to the
 * callback definition.
 */
function MYMODULE_views_argument__user_organization($handler, $langcode) {
  global $user;
  return $user->field_other_organization_field[$langcode][0]['target_id'];
}



/**
 * Implements views_arguments_access_callback_info().
 */
function hook_views_arguments_access_callback_info() {
  $callbacks['rand'] = array(
    // Mandatory title. Translate it if you want.
    'title' => 'Random, with $spread argument',

    // Mandatory callback, can be Closure, array(Class, Method), String etc.
    // N.B. If you're using this for a page callback, it MUST be a String.
    'callback' => 'YOURMODULE_views_access__rand',
    'callback' => function($spread) {
      return (bool) rand(0, $spread-1);
    },

    // Optional # arguments. Default min = 0. Default max = 3.
    'min arguments' => 1,
    'max arguments' => 1,
  );
  return $callbacks;
}

/**
 * Implements views_arguments_access_callback_info_alter().
 */
function hook_views_arguments_access_callback_info_alter(&$callbacks) {
  // Better randomness with this callback.
  $callbacks['rand']['callback'] = 'MYMODULE_views_access__rand';

  // Doesn't need a mandatory first argument.
  $callbacks['rand']['min arguments'] = 0;
}

/**
 * Views Arguments access callback for 'rand'.
 */
function MYMODULE_views_access__rand($spread = 2) {
  return (bool) rand(0, $spread-1);
}
