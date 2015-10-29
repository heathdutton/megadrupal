<?php
/*
Custom "away" command, written by permutations (redisplay method). Toggle:
(1) Broadcasts the person is away and appends " (away)" to the nick.
(2) Broadcasts the person is back and strips " (away)" from the nick.
*/

// Assumes the "away" command is in the custom command directory for Drupal.
require_once DRUPAL_ROOT . '/'. drupal_get_path('module', 'phpfreechat') . '/phpfreechat/src/pfccommand.class.php';

// Code for "away" command.
class pfcCommand_away extends pfcCommand {
  /**
   * Command name (lowercase)
   */
  var $name = 'away';

  /**
   * Contains the command syntax (how to use the command)
   */
  var $usage = "/away";

  /**
   * Virtual method which must be implemented by concrete commands.
   * It is called by the phpFreeChat::HandleRequest function to execute the wanted command.
   */
  function run(&$xml_reponse, $p) {
    $clientid = $p["clientid"];
    $param = $p["param"];
    $sender = $p["sender"];
    $recipient = $p["recipient"];
    $recipientid = $p["recipientid"];

    $c = &pfcGlobalConfig::Instance();
    $u = &pfcUserConfig::Instance();
    $ct = &pfcContainer::Instance();

    // toggle away-status.
    $is_away = $ct->getUserMeta($u->nickid, 'away');
    // If it's on, turn it off.
    if ($is_away == 'yes') {
      $ct->setUserMeta($u->nickid, 'away', 'no');
      $is_away = 'no';
      $pfc_msg = "$u->nick is back";
      $away_img = $c->getFileUrlFromTheme('images/away-off.gif');
    }
    // If it's off, turn it on.
    else {
      $ct->setUserMeta($u->nickid, 'away', 'yes');
      $is_away = 'yes';
      $pfc_msg = "$u->nick is away";
      $away_img = $c->getFileUrlFromTheme('images/away-on.gif');
    }

    // refresh the nick list and nick box with away-status.
    $u->saveInCache();
    $this->forceWhoisReload($u->nickid);

    // notify all the joined channels/privmsg.
    $cmdp = $p;
    //	$cmdp["param"] = _pfc($pfc_msg);
    $cmdp["param"] = $pfc_msg;
    $cmdp["flag"] = 1;
    $cmd = &pfcCommand::Factory("notice");
    foreach ($u->channels as $id => $chan) {
      $cmdp["recipient"] = $chan["recipient"];
      $cmdp["recipientid"] = $id;
      $cmd->run($xml_reponse, $cmdp);
    }
    foreach ($u->privmsg as $id => $pv) {
      $cmdp["recipient"] = $pv["recipient"];
      $cmdp["recipientid"] = $id;
      $cmd->run($xml_reponse, $cmdp);
    }

    // refresh the away button.
    $xml_reponse->script("pfc.handleResponse('" . $this->name . "', '" . $is_away . "', '" . $away_img . "');");
    return;
  }
}

?>
