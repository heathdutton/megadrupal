<?php

/**
 * @file
 * Pjirc module page  display
 */


/**
 * Menu callback; displays a Drupal page PJIRC chatroom.
 */
function pjirc_page() {
  $pjirc_allow_nick_selection = variable_get('pjirc_allow_nick_selection', 1);
  $pjirc_allow_nick_selection_registered = variable_get('pjirc_allow_nick_selection_registered', 0);
  $pjirc_user_is_guest = _pjirc_user_is_guest();

  // Only display the chat page if configuration allows it, else, show nick selector form
  if ((($pjirc_user_is_guest && $pjirc_allow_nick_selection) || (!$pjirc_user_is_guest && $pjirc_allow_nick_selection_registered)) && !isset($_POST['pjirc_nick'])) {
    return drupal_get_form('_pjirc_page_nick_selection');
  }
  else {
    return _pjirc_page_pjirc();
  }
}

/**
 * Displays a form to select nick before start chatting
 */
function _pjirc_page_nick_selection($form, &$form_state) {
  global $user;
  $nick = !_pjirc_user_is_guest() ? $user->name : variable_get('pjirc_nick', 'Guest');
  $form['pjirc_nick'] = array(
    '#type' => 'textfield',
    '#title' => 'Nickname',
    '#default_value' => "$nick",
    '#size' => 30,
    '#maxlength' => 30,
    '#required' => TRUE,
  );
  $form['submit_nick'] = array(
    '#type' => 'submit',
    '#value' => t('Start chatting!'),
    '#executes_submit_callback' => FALSE,
  );
  return $form;
}

/**
 * Displays PJIRC chat applet
 */
function _pjirc_page_pjirc() {
  if (isset($_POST['pjirc_nick']) && !empty($_POST['pjirc_nick'])) {
    $pjirc_nick = check_plain($_POST['pjirc_nick']);
  }
  else {
    global $user;
    $pjirc_nick = !_pjirc_user_is_guest() ? $user->name : variable_get('pjirc_nick', 'Guest');
  }
  $pjirc_nick = _pjirc_sanitize_nick($pjirc_nick);
  $pjirc_realname = variable_get('pjirc_realname', 'Java User');
  $pjirc_server = variable_get('pjirc_server', 'irc.freenode.org');
  $pjirc_serverport = variable_get('pjirc_serverport', '6667');
  $pjirc_quitmessage = variable_get('pjirc_quitmessage', 'Java User');
  $pjirc_room = variable_get('pjirc_room', '');
  $pjirc_room_width = variable_get('pjirc_room_width', '500');
  $pjirc_room_height = variable_get('pjirc_room_height', '400');
  $pjirc_font_color = variable_get('pjirc_font_color', '');
  $pjirc_font_type = variable_get('pjirc_font_type', '');
  $pjirc_enablesmileys = variable_get('pjirc_enablesmileys', 'true');
  $pjirc_nickfield = variable_get('pjirc_nickfield', 'true');

  $output  = "<div id=\"pjircdiv\" align=\"center\">\n";
  $output .= "<applet codebase=" . base_path() . drupal_get_path('module', 'pjirc') . "/pjirc/ code=IRCApplet.class archive=irc.jar,pixx.jar width=" . $pjirc_room_width . " height=" . $pjirc_room_height . ">\n";
  $output .= "<param name=\"CABINETS\" value=\"irc.cab,securedirc.cab,pixx.cab\">\n";
  $output .= "<param name=\"nick\" value=\"" . $pjirc_nick . "\">\n";
  $output .= "<param name=\"alternatenick\" value=\"" . $pjirc_nick . "???\">\n";
  $output .= "<param name=\"name\" value=\"" . $pjirc_realname . "\">\n";
  $output .= "<param name=\"gui\" value=\"pixx\">\n";
  $output .= "<param name=\"host\" value=\"" . $pjirc_server . "\">\n";
  $output .= "<param name=\"port\" value=\"" . $pjirc_serverport . "\">\n";
  $output .= "<param name=\"quitmessage\" value=\"" . $pjirc_quitmessage . "\">\n";
  if (!empty($pjirc_room)) {
    $output .= "<param name=\"command1\" value=\"/join " . $pjirc_room . "\">\n";
  }
  $output .= "<param name=\"pixx:language\" value=\"pixx-english\">\n";
  $output .= "<param name=\"language\" value=\"english\">\n";
  $output .= "<param name=\"style:bitmapsmileys\" value=\"" . $pjirc_enablesmileys . "\">\n";
  if ($pjirc_enablesmileys === 'true') {
    $output .= "<param name=\"style:smiley1\" value=\":) img/sourire.gif\">\n";
    $output .= "<param name=\"style:smiley2\" value=\":-) img/sourire.gif\">\n";
    $output .= "<param name=\"style:smiley3\" value=\":-D img/content.gif\">\n";
    $output .= "<param name=\"style:smiley4\" value=\":d img/content.gif\">\n";
    $output .= "<param name=\"style:smiley5\" value=\":-O img/OH-2.gif\">\n";
    $output .= "<param name=\"style:smiley6\" value=\":o img/OH-1.gif\">\n";
    $output .= "<param name=\"style:smiley7\" value=\":-P img/langue.gif\">\n";
    $output .= "<param name=\"style:smiley8\" value=\":p img/langue.gif\">\n";
    $output .= "<param name=\"style:smiley9\" value=\";-) img/clin-oeuil.gif\">\n";
    $output .= "<param name=\"style:smiley10\" value=\";) img/clin-oeuil.gif\">\n";
    $output .= "<param name=\"style:smiley11\" value=\":-( img/triste.gif\">\n";
    $output .= "<param name=\"style:smiley12\" value=\":( img/triste.gif\">\n";
    $output .= "<param name=\"style:smiley13\" value=\":-| img/OH-3.gif\">\n";
    $output .= "<param name=\"style:smiley14\" value=\":| img/OH-3.gif\">\n";
    $output .= "<param name=\"style:smiley15\" value=\":'( img/pleure.gif\">\n";
    $output .= "<param name=\"style:smiley16\" value=\":$ img/rouge.gif\">\n";
    $output .= "<param name=\"style:smiley17\" value=\":-$ img/rouge.gif\">\n";
    $output .= "<param name=\"style:smiley18\" value=\"(H) img/cool.gif\">\n";
    $output .= "<param name=\"style:smiley19\" value=\"(h) img/cool.gif\">\n";
    $output .= "<param name=\"style:smiley20\" value=\":-@ img/enerve1.gif\">\n";
    $output .= "<param name=\"style:smiley21\" value=\":@ img/enerve2.gif\">\n";
    $output .= "<param name=\"style:smiley22\" value=\":-S img/roll-eyes.gif\">\n";
    $output .= "<param name=\"style:smiley23\" value=\":s img/roll-eyes.gif\">\n";
  }
  $output .= _pjirc_output_colors_params();
  $output .= "<param name=\"style:backgroundimage\" value=\"true\">\n";
  $output .= "<param name=\"style:backgroundimage1\" value=\"all all 0 background.gif\">\n";
  $output .= "<param name=\"style:sourcefontrule1\" value=\"all all Serif 12\">\n";
  $output .= "<param name=\"style:floatingasl\" value=\"true\">\n";
  $output .= "<param name=\"pixx:timestamp\" value=\"true\">\n";
  $output .= "<param name=\"pixx:highlight\" value=\"true\">\n";
  $output .= "<param name=\"pixx:highlightnick\" value=\"true\">\n";
  $output .= "<param name=\"pixx:nickfield\" value=\"" . $pjirc_nickfield . "\">\n";
  $output .= "<param name=\"pixx:styleselector\" value=\"" . $pjirc_font_color . "\">\n";
  $output .= "<param name=\"pixx:setfontonstyle\" value=\"" . $pjirc_font_type . "\">\n";
  $output .= "</applet>\n";
  $output .= "<br /><br />";
  $output .= t("<a href=\"@javaurl\" target=\"_blank\">If the applet does not display then click here to download a Java client.</a>", array('@javaurl' => 'http://www.java.com/en/download/')) . "\n";
  $output .= "<br /><br />";
  $output .= t("You can also connect using any IRC client (such as <a href=\"@mircurl\">MIRC</a>) using the following:", array('@mircurl' => 'http://www.mirc.com/')) . "<br />\n";
  $output .= t("HOST") . ": " . $pjirc_server . "\n";
  $output .= t("CHANNEL") . ": " . $pjirc_room . "\n";
  $output .= "</div>\n";

  return $output;
}

function _pjirc_sanitize_nick($username) {
  $unallowed_chars = array(' ');
  $username = str_replace($unallowed_chars, '', $username);

  return $username;
}

function _pjirc_filter_conf_array_callback($var) {
  return preg_match('/^pjirc_color(\d)+$/', $var);
}

function _pjirc_output_colors_params() {
  global $conf;

  require_once drupal_get_path('module', 'pjirc') . '/' . 'pjirc.functions.inc.php';

  $output = '';
  // obtain default colors
  $colorsdef = _pjirc_get_colorsdef();

  // obtain the keys in conf that are about pjirc color, then convert values to
  // keys
  $colorkeys = array_flip(array_filter(array_keys($conf), '_pjirc_filter_conf_array_callback'));
  //obtain elements of $conf[] that are for pjirc colors 
  $colorsset = array_intersect_key($conf, $colorkeys);
  // check if we have found color variables in conf[]
  if (!empty($colorsset) && count($colorsset)) {
    // check if there are difference between $conf color variables and default
    // colors,  and print only params for non-default colors
    foreach ($colorsset as $colorset => $colorsetvalue) {
      $colorname = str_replace('pjirc_', '', $colorset);
      if ($colorsdef[$colorname]['defvalue'] != $colorsetvalue) {
        $colorsetvalue = str_replace('#', '', $colorsetvalue);
        $output .= "<param name=\"pixx:$colorname\" value=\"$colorsetvalue\">\n";
      }
    }
  }

  return $output;
}

function _pjirc_user_is_guest() {
  global $user;
  return !$user->uid;
}
