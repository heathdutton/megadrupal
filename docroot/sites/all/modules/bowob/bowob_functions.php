<?php
// $Id$

/**
 * @file bowob_functions.php
 *
 * Implements main functions and calls the BoWoB API.
 */

//define('BOWOB_UR_RELATION_TYPE_ID', 0); // Relation used to the friends list. Undefined: all relations.

/**
 * Prints synchronization data to be readed by BoWoB server.
 * @return void.
 */
function bowob_server_sync()
{
  header('Cache-Control: no-cache, must-revalidate');
  header('Content-Type: text/plain; charset=utf-8');

  module_load_include('php', 'bowob', 'bowob_api');

  echo bowob_api_get_sync_data(
    is_numeric(@$_GET['id']) ? intval($_GET['id']) : -1,
    is_string(@$_GET['sync']) ? $_GET['sync'] : '',
    @$_GET['name'] == '1',
    @$_GET['avatar'] == '1',
    @$_GET['friends'] == '1'
  );
}

/**
 * Creates a synchronization record and prints record identifiers to be readed by BoWoB client.
 * @return void.
 */
function bowob_client_sync()
{
  header('Cache-Control: no-cache, must-revalidate');
  header('Content-Type: text/plain');

  if(!user_access('bowob chat'))
  {
    return;
  }

  module_load_include('php', 'bowob', 'bowob_api');

  echo bowob_api_new_sync(
    is_string(@$_GET['nick']) ? $_GET['nick'] : '',
    @$_GET['name'] == '1',
    @$_GET['avatar'] == '1'
  );
}

/**
 * Redirects to login page.
 * @return void.
 */
function bowob_redirect_login()
{
  drupal_goto('user');
}

/**
 * Redirects to user profile page.
 * @return void.
 */
function bowob_redirect_profile()
{
  $userid = is_numeric(@$_GET['id']) ? intval($_GET['id']) : -1;

  if($userid > 0)
  {
    drupal_goto('user/' . $userid);
  }
  else
  {
    drupal_goto('');
  }
}

/**
 * Gets BoWoB HTML code for show the chat.
 * @return string The HTML code.
 */
function bowob_code()
{
  if(!user_access('bowob chat'))
  {
    return '';
  }

  module_load_include('php', 'bowob', 'bowob_api');

  return bowob_api_get_code(
    variable_get('bowob_app_id', ''),
    variable_get('bowob_server_address', '')
  );
}

/**
 * Checks if current user is logued.
 * @return boolean User is logued.
 */
function bowob_is_user_logued()
{
  global $user;

  if($user->uid > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Gets current user id.
 * @return int User id.
 */
function bowob_get_user_id()
{
  global $user;

  return $user->uid;
}

/**
 * Gets current user nick.
 * @return string User nick.
 */
function bowob_get_user_nick()
{
  global $user;

  if($user->uid > 0)
  {
    return $user->name;
  }
  else
  {
    return '';
  }
}

/**
 * Gets current user name.
 * @return string User name.
 */
function bowob_get_user_name()
{
  return '';
}

/**
 * Gets current user avatar url.
 * @return string User avatar.
 */
function bowob_get_user_avatar()
{
  global $user;

  if($user->uid <= 0 || !$user->picture)
  {
    return '';
  }

  $user_load = user_load($user->uid);

  if(!$user_load->picture || empty($user_load->picture->uri))
  {
    return '';
  }

  return file_create_url($user_load->picture->uri);
}

/**
 * Gets current user friends.
 * @param int $id User id.
 * @param string $separator Separator between nicks.
 * @return string User friends.
 */
function bowob_get_user_friends($id, $separator)
{
  $output = '';

  if(function_exists('user_relationships_load'))
  {
    $friends = array();
    $args = array(
      'user' => $id,
      'approved' => true,
    );
    if(defined('BOWOB_UR_RELATION_TYPE_ID'))
    {
      $args['rtid'] = BOWOB_UR_RELATION_TYPE_ID;
    }
    $query_opts = array('include_user_info' => true);

    $relationships = user_relationships_load($args, $query_opts);
    if($relationships)
    {
      foreach($relationships as $rtid => $relationship)
      {
        if($id == $relationship->requester_id)
        {
          $relatee = $relationship->requestee;
        }
        else
        {
          $relatee = $relationship->requester;
        }

        $friends[] = $relatee->name;
      }
    }

    $friends = array_unique($friends);

    $count = sizeof($friends);
    for($i = 0; $i < $count; $i ++)
    {
      $output .= $friends[$i] . $separator;
    }
  }

  return $output;
}

/**
 * Stores a synchronization record in database.
 * @param string $auth Record auth.
 * @param int $creation Record creation time.
 * @param int $user_id Record user id.
 * @param string $user_nick Record user nick.
 * @param string $user_name Record user name.
 * @param string $user_avatar Record user avatar.
 * @param int $user_type Record user type.
 * @return int Record id.
 */
function bowob_store_sync($auth, $creation, $user_id, $user_nick, $user_name, $user_avatar, $user_type)
{
  return db_insert('bowob')
    ->fields(array(
      'auth' => $auth,
      'creation' => $creation,
      'user_id' => $user_id,
      'user_nick' => $user_nick,
      'user_name' => $user_name,
      'user_avatar' => $user_avatar,
      'user_type' => $user_type,
    ))
    ->execute()
  ;
}

/**
 * Extracts a synchronization record from database.
 * @param int $id Record id.
 * @param string $auth Record auth.
 * @param int $expiration Record expiration time.
 * @return array Record values.
 */
function bowob_extract_sync($id, $auth, $expiration)
{
  db_delete('bowob')
    ->condition('creation', $expiration, '<')
    ->execute()
  ;

  $rs = db_query('SELECT auth, user_id, user_nick, user_name, user_avatar, user_type FROM {bowob} WHERE id = :id', array(':id' => $id));
  $rc = $rs->fetchObject();

  if(!$rc || $rc->auth != $auth)
  {
    return array();
  }
  else
  {
    db_delete('bowob')
      ->condition('id', $id)
      ->execute()
    ;

    return array(
      'user_id' => $rc->user_id,
      'user_nick' => $rc->user_nick,
      'user_name' => $rc->user_name,
      'user_avatar' => $rc->user_avatar,
      'user_type' => $rc->user_type,
    );
  }
}

?>
