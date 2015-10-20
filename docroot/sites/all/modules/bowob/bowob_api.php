<?php

/**
 * BoWoB API
 *
 * @version 1.0
 */

/**
 * Generates synchronization data that BoWoB server needs.
 * @param int $id Record indentificator.
 * @param string $auth Record auth.
 * @param boolean $get_name Add user name to response.
 * @param boolean $get_avatar Add user avatar to response.
 * @param boolean $get_friends Add user friends to response.
 * @return string The synchronization data.
 */
function bowob_api_get_sync_data($id, $auth, $get_name, $get_avatar, $get_friends)
{
  if($id < 0 || $auth == '')
  {
    return '';
  }
  
  $rs = bowob_extract_sync($id, $auth, time() - 300);

  if(!sizeof($rs))
  {
    return '';
  }

  $output = 'bowob_id: ' . $rs['user_id'] . "\n";
  $output .= 'bowob_user: '. $rs['user_nick'] . "\n";
  if($get_name)
  {
    $output .= 'bowob_name: ' . $rs['user_name'] . "\n";
  }
  if($get_avatar)
  {
    $output .= 'bowob_avatar: ' . $rs['user_avatar'] . "\n";
  }
  $output .= 'bowob_type: ' . $rs['user_type'] . "\n";
  if($get_friends)
  {
    $output .= 'bowob_friends_start' . "\n";
    $user_friends = bowob_get_user_friends($rs['user_id'], "\n");
    $output .= $user_friends;
    $output .= 'bowob_friends_end' . "\n";
  }
  
  return $output;
}

/**
 * Creates a synchronization record.
 * @param string $rq_nick Requested user nick.
 * @param boolean $get_name Add user name to the record.
 * @param boolean $get_avatar Add user avatar to the record.
 * @return string The record identifiers.
 */
function bowob_api_new_sync($rq_nick, $get_name, $get_avatar)
{
  $pattern = '1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

  if($rq_nick == '')
  {
    return '';
  }

  $user_id = bowob_get_user_id();
  if(bowob_is_user_logued())
  {
    $user_nick = bowob_get_user_nick();
    $user_name = $get_name ? bowob_get_user_name() : '';
    $user_type = 0;
  }
  else
  {
    $user_nick = $rq_nick;
    $user_name = '';
    $user_type = 1;
  }
  $user_avatar = $get_avatar ? bowob_get_user_avatar() : '';
  
  $auth = '';
  for($i = 0; $i < 50; $i ++)
  {
    $auth .= $pattern[mt_rand(0, 61)];
  }

  $id = bowob_store_sync($auth, time(), $user_id, $user_nick, $user_name, $user_avatar, $user_type);
  
  return '{id: "' . $id . '", auth: "' . $auth . '"}';
}

/**
 * Creates the HTML code for show the chat.
 * @param string $app_id BoWoB App id.
 * @param string $server_address BoWoB server address.
 * @return string The HTML code.
 */
function bowob_api_get_code($app_id, $server_address)
{
  if($app_id == '' || $server_address == '')
  {
    return '';
  }

  $output = '<div id="bowob"></div>' . "\n";
  $output .= '<script type="text/javascript">';
    $output .= 'bowob_user_nick = "' . bowob_get_user_nick() . '";';
  $output .= '</script>' . "\n";
  $output .= '<script type="text/javascript" src="' . $server_address . $app_id . '/bl_2/loader.js"></script>' . "\n";

  return $output;
}

?>