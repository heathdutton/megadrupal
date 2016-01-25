<?php

/**
 *  Get a ticket:
 *    http://.../mailman.php?a=new
 *    response:
 *      { error: false, ticket: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx } 
 *      
 *  Send a mail:
 *    http://.../mailman.php?a=send
 *    w/ POST params:   
 *             n = FROM_NAME
 *             e = FROM_EMAIL
 *             s = SUBJECT
 *             m = MAIL_TEXT
 *             t = TICKET
 *    response:
 *      { error: false, sent: true }
 */


switch ($_GET['a']) {
  case 'new' :
    print action('new');
    break;
  case 'send' :
    print action('send');
    break;
  default:
    print '{ "error" : true }';  
    break;
}
exit;



function action($a) {
  //require_once('dbconn.php');
  //db_connect();
  
  switch ($a) {
    case 'new' :
      return getTicket();
    case 'send' :
      return sendMail();
    default:
      return '{ "error" : true }';  
  }
}

/**
 *  create a new ticket.
 *  @return JSON
 *   { error: false, ticket: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx }   
 */ 
function getTicket() {
  $site = substr($_SERVER['SERVER_NAME'], 0, strrpos($_SERVER['SERVER_NAME'], '.'));  
  $ticket = md5(time() + rand());
  $sql = "insert into mail (ticket, issued, site) values ('$ticket', '" . time() . "', '$site')";
  //$success = mysql_query($sql);
  if (true || $success) {
    return '{ "error" : false, "ticket" : "' . $ticket . '" }';
  }
  return '{ "error" : true }';  
}



/**
 * Send a mail:
 *    http://.../mailman.php?a=send
 *    w/ POST params:   
 *             n = FROM_NAME
 *             e = FROM_EMAIL
 *             s = SUBJECT
 *             m = MAIL_TEXT
 *             t = TICKET
 *  
 * @return JSON
 *      { error: false, sent: true }
 */
function sendMail() {
  $site = substr($_SERVER['SERVER_NAME'], 0, strrpos($_SERVER['SERVER_NAME'], '.'));  
  
  $name = mysql_real_escape_string($_POST['n']);
  $email = mysql_real_escape_string($_POST['e']);
  $subj = mysql_real_escape_string($_POST['s']);
  $mssg = mysql_real_escape_string($_POST['m']);
  $ticket = mysql_real_escape_string($_POST['t']);

//  $sql = "select id from mail where ticket='$ticket' and site='$site' and used='0'";
//  
//  $result = mysql_query($sql);
//  if ( (!result) || (mysql_num_rows($result) != 1 ) ) {
//    return '{ "error" : true }';  
//  }
//
//  $row = mysql_fetch_object($result);
//
//  $sql = "update mail 
//          set used = '" . time() . "', user_ip = '" . $_SERVER['REMOTE_ADDR'] . "'
//          where id='" . $row->id . "'";
//  $result = mysql_query($sql);
//  
//  require_once($site . '/info.php');
//
//  $sent = mail($info->email, "[$site]: $subj", "FROM: $name\nEMAIL: $email\n\n$mssg");

   if ($sent) {
     return '{ "error" : false, "sent" : true }';
   }
   return '{ "error" : true }';
}

