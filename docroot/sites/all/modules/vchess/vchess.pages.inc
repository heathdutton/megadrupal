<?php

/**
 * @file
 * User page callbacks for the vchess module.
 */

/**
 * page callback vchess_main_page to display main vchess window
 */
function vchess_main_page() {
  global $user;

  $out = "";

  if (user_is_logged_in()) {
    $gamefolder =  variable_get('vchess_game_files_folder', '/vchess-data');
    $res_games = $gamefolder;

    if (!$user->name) {
      $txt = t('Please, register to play chess');
      return $txt;
    }

    $player = new Player($user->uid);
    $out .= "<p>My current rating: <b>" . $player->rating() . "</b></p>"; 
        
    $out .= vchess_users_current_games($user->name);
    $out .= vchess_challenges();

    $out .= vchess_player_stats_table($user->uid);
  }
  else {
    $out = "Please log in to access this page";
  }

  return $out;
}

/**
 * Get the stats for a particular player
 */
function vchess_player_stats_table($uid) {
  $gamer_stats = gamer_player_stats_array($uid);
  
  if ($gamer_stats['current'] <> vchess_stats_games_current($uid)) {
    watchdog("vchess", "Stats for current games from gamer was :gamer_current but vchess calculates :vchess",
        array(
          ':gamer_current' => $gamer_stats['current'], 
          ':vchess' => vchess_stats_games_current($uid)
        ),
        WATCHDOG_ERROR
    );
    
    $player = new Player($uid);
    
    $player->set_current(vchess_stats_games_current($uid));
    $player->save_stats();
  }
  
  //    $out .= vchess_wld_stats($user);
  $html = gamer_player_stats_table($uid);

  return $html;
}

/**
 * menu callback to display current games
 */
function vchess_my_current_games() {
  global $user;

  $out = "";

  $out .= vchess_users_current_games($user->name);
  $out .= vchess_new_game_links();

  return $out;
}

/**
 * menu callback vchess_opponent_game_page to display new game form
 */
function vchess_opponent_game_form() {
  $colors = array('w' => t('white'), 'b' => t('black'));

  $form['colorfield'] = array(
      '#type' => 'fieldset',
      '#title' => t('Choose your color'),
  );

  $form['colorfield']['color'] = array(
      '#type' => 'radios',
      '#default_value' => 'w',
      '#options' => $colors,
  );

  $form['opponent'] = array(
      '#type' => 'textfield',
      '#title' => t('opponent'),
      '#description' => t('Type opponent\'s name. Opponent must be registered on this site.'),
      '#required' => TRUE,
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create Game'),
  );

  return $form;
}

/**
 * menu callback vchess_random_game_form to display new game form
 */
function vchess_random_game_form() {
  $form['description'] = array(
      '#type' => 'item',
      '#title' => t('Simply click on the button below and we will create
          a game for you against a random opponent.'),
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create Game'),
  );

  return $form;
}

/**
 * menu callback vchess_create_challenge_form to display new game form
 */
function vchess_create_challenge_form() {
  $form = array();
  
  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Simply click on the button below and we will create
      a game for you against a random opponent.'),
  );

  $form['time_per_move'] = array(
    '#type' => 'select', 
    '#title' => t('Time per move'),
    '#options' => array(
       '1' => t('1 day'),
       '2' => t('2 days'), 
       '3' => t('3 days'),
       '5' => t('5 days'),
     ),
    '#default_value' => "3", // added default value.
  );
  
//   pos_initialise_table();
  $positions = pos_get_positions();
  $form['position'] = array(
      '#type' => 'select',
      '#title' => t('Starting position'),
      '#options' => $positions,
      '#description' => t('Select the game starting position.'),
  );
  
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create Challenge'),
  );
  
//  return drupal_get_form('vchess_create_challenge_form', $form);
  
  return $form;
}

/**
 * Process the vchess_create_challenge form.
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function vchess_create_challenge_form_submit($form, &$form_state) {
  global $user;
  

  $pending = 0;
  $games = vchess_load_challenges();
  foreach ($games as $game) {
    // Check if there is a matching challenge
    if ($game->challenger()->uid() != $user->uid 
    && $game->time_per_move() == $form_state['values']['time_per_move']) {
      vchess_accept_challenge($game->gid());
    }
    if ($game->challenger()->uid() == $user->uid) {
      $pending++;
    }
  }
  
  // Check that user does not already have too many challenges pending
  if ($pending < VCHESS_PENDING_LIMIT) {
    $game = new Game();
  
    $gid = $game->gid();
    $game->set_player($user->uid);
    $game->set_time_per_move( $form_state['values']['time_per_move']);
    $game->setup_position($form_state['values']['position']);
    $game->save();
  
    drupal_set_message(t('Challenge has been created.'));
  }
  else {
    drupal_set_message(t('You already have the allowed maxiumum of') . " " . VCHESS_PENDING_LIMIT . 
        " " . t('challenges pending.'));
  }
  
  drupal_goto("vchess/challenges");
}

/**
 * Menu callback to create new default challenge
 * 
 * @todo use drupal_form_submit() to make this function call the vchess_create_challenge_form_submit() function
 */
function vchess_create_default_challenge() {
  global $user;

  // Check that user does not already have a challenge pending
  $pending = 0;
  $games = vchess_load_challenges();
  foreach ($games as $game) {
    if ($game->challenger()->uid() == $user->uid) {
      $pending++;
    }  
  }
  
  if ($pending <= VCHESS_PENDING_LIMIT) {
    $game = new Game();
  
    $gid = $game->gid();
    $game->set_player($user->uid);
  
    drupal_set_message(t('Challenge has been created.'));
  
    drupal_goto("vchess/game/$gid");
  }
  else {
    drupal_set_message(t("You already have the maximum of") . VCHESS_PENDING_LIMIT . t("challenges pending."));
    
    drupal_goto("vchess/challenges");
  }
}



/**
 * menu callback vchess_opponent_game_page to display new game form
 */
function vchess_reset_games_form() {
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Reset ALL games!?!'),
  );

  return $form;
}

/**
 * Process the Reset Games form.
 *
 * All tables for vchess (games, statistics, moves) will be emptied!
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function vchess_reset_games_form_submit($form, &$form_state) {
  $result = db_truncate('vchess_games')->execute();
  $result = db_truncate('vchess_moves')->execute();

  $result = gamer_reset();

  drupal_set_message(t('ALL games have been reset!'));
}


/**
 * Process the form to start a new game against a named opponent
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function vchess_opponent_game_form_submit($form, &$form_state) {
  global $user;

  $form_values = $form_state['values'];

  // Get the uid and name of the opponent
  $sql = "SELECT uid, name FROM {users} WHERE name = '" . $form_values['opponent'] . "'";
  $result = db_query($sql);

  $record = $result->fetchAssoc();
  $opponent_uid = $record['uid'];
  $opponent_name = $record['name'];
  //  $opponent_name = $result->fetchField();  // Hugh - new D7 way

  if ($form_values['color']=='w') {
    // user plays white
    $white_uid = $user->uid;

    // opponent plays black
    $black_uid = $opponent_uid;
  }
  else {
    // user plays black
    $black_uid = $user->uid;

    // opponent plays white
    $white_uid = $opponent_uid;
  }

  $game = new Game();
  $game->set_players($white_uid, $black_uid);
  $gid = $game->gid();

  drupal_set_message(t('Game has been created.'));
  $form_state['redirect'] = 'vchess/game/' . $gid;
}

/**
 * Check the form for a new game.  The only bit of validation is to check
 * that the selected opponent exists on the site.
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function vchess_opponent_game_form_validate($form, &$form_state) {
  global $user;

  $form_values = $form_state['values'];

//   $sql = "SELECT name FROM {users} WHERE name = '" . $form_values['opponent'] . "'";
  
  $res = db_select('users')
    ->fields('users', array('name'))
    ->condition('name', $form_values['opponent'],'=')
    ->execute();

  if ($res->rowCount() == 0) {
    form_set_error('', t('Opponent does not exist on this site'));
  }
}

/**
 * Accept a challenge to play a particular game
 */
function vchess_accept_challenge($gid) {
  global $user;
  
  $game = new Game();
  $game->load($gid);
  
  watchdog(__FUNCTION__, "Player uid=%uid (%uname) has accepted challenge for game %gid. " .
      "white_uid=%white_uid, black_uid=%black_uid. ",
      array('%uid' => $user->uid,
          '%uname' => $user->name,
          '%gid' => $gid,
          '%white_uid' => $game->white_uid(),
          '%black_uid' => $game->black_uid()));

  // Check that the game has not already got players (should never happen!)
  if ($game->white_uid() <> 0 || $game->black_uid() <> 0) {
    $game->set_player($user->uid);
  
    gamer_start_game($game->white_uid(), $game->black_uid());
    
    $extra = "";
    $its_your_move = "";
    if ($game->player_color($user->uid) == 'w') {
      $opponent_name = $game->black_player()->name();
      $opponent_user = user_load($game->black_uid());
      $its_your_move = "Now, it's your move!";
    }
    else {
      $opponent_name = $game->white_player()->name();
      $opponent_user = user_load($game->white_uid());
      $extra = "Since you are playing black, you will have to wait for $opponent_name to move.<br />";
    }
    $msg = "Congratulations, you have started a game against <b>" . $opponent_name . "</b>.<br />";
    $msg .= $extra;
    $msg .= "You can keep an eye on the status of this game and all your games on your <a href='" . url("vchess/my_current_games") .
    "'>current games page</a>.<br />";
    $msg .= $its_your_move;
    
    rules_invoke_event('vchess_challenge_accepted', $opponent_user, $gid);
    
    drupal_set_message($msg);
    
    drupal_goto("vchess/game/" . $gid);
  }
  else {
    watchdog(__FUNCTION__, "Players are already assigned so challenge cannot be fulfilled. " . 
        "Player uid=%uid (%uname) accepted challenge for game %gid. " .
        "white_uid=%white_uid, black_uid=%black_uid. ",
        array('%uid' => $user->uid,
            '%uname' => $user->name,
            '%gid' => $gid,
            '%white_uid' => $game->white_uid(),
            '%black_uid' => $game->black_uid()
            ),
        WATCHDOG_ERROR);
  }
  
  return vchess_game($gid);
}

/**
 * Display the game (board, scoresheet) for the given gid
 *
 * @param $gid: Game id
 */
function vchess_game($gid) {
  global $user;
  global $base_path; // e.g. "/chess_drupal-7.14/"
  global $base_url; // e.g. "http://localhost/chess_drupal-7.14"

  $out = "";
  
  if (user_is_logged_in()) {
    $module_path = drupal_get_path('module', 'vchess');
  
    drupal_add_css($module_path . '/vchessstyle.css');
    drupal_add_js($module_path . '/board.js');
  
    $game = new Game();
    $game->load($gid);
  
    // Find the player color
    $player_color = $game->player_color($user->uid);
    
    // Find out if the player has the move or not
    if ($game->turn() == $player_color  && $game->status() == STATUS_IN_PROGRESS) {
      $player_may_move = TRUE;
    }
    else {
      $player_may_move = FALSE;
    }
  
    $out .= "<div id='container'>";
    
    // Display game heading, e.g. "white: admin - black: hugh"
    $out .= '<div style="text-align:center;">white: <b>' .
        $game->white_player()->name() . '</b> -   black: <b>' . $game->black_player()->name() . '</b>';
    $out .= '</div>';
    
    $out .= "<div id='board'>";
    $out .= vchess_render_game($gid, $player_color, $player_may_move);
    $out .= "</div>";
  
    $out .= "<div id='board_commands'>";
    $out .= vchess_render_command_form($game);
    $out .= "</div>";
    
    // Prepare the scoresheet
    $out .= "<div id='scoresheet'>";
    $scoresheet = new Scoresheet();
    $scoresheet->load($gid);
    $out .= $scoresheet->get_table();
    $out .= "</div>";
    
    $out .= "</div>";
  
   
    $out .= '';
    $out .= '<script language="Javascript">module_path=\'' . $module_path . '\';';
    $out .= "sub_path='" . $base_path . "/" . drupal_get_path('module', 'vchess') . "';";
    $out .= "full_path='" . $base_url . "/" . drupal_get_path('module', 'vchess') . '\';';
    $out .= 'checkMoveButton();';
    $out .= 'highlightMove(window.document.getElementById("vchess-command-form").move.value);';
    //      $out .= '<input type=\'button\' onclick=\'confirm_resign()\' value=\'Show alert box\' />;';
    $out .= '</script>';
  }
  else {
    $out = t("Please log in to access this page");
  }

  return $out;
}

/**
 * Display the page for a given player
 *
 * @param $uid
 */
function vchess_player($name) {
  $player = user_load_by_name($name);

  $html = "";

//  $html .= vchess_wld_stats($player);
  $html .= gamer_player_stats_table($player->uid);
  $html .= vchess_users_current_games($name);

  return $html;
}

/**
 * page callback to display the table of players
 */
function vchess_players() {
  vchess_check_for_lost_on_time_games();

  return gamer_players();
}

/**
 * Get simple won/lost/drawn stats
 */
function vchess_wld_stats($stats_user) {
  global $user;

  $html = "";

  $player = new Player($stats_user->uid);
  //   $stats = gamer_load_user_stats($stats_user->uid);

  if ($stats_user->uid == $user->uid) {
    $html .= "Here are your basic statistics<br />";
  }
  else {
    $html .= "Here are the basic statistics for <b>" . $stats_user->name . "</b><br />";
  }

  $html .= '<div style="text-align:center;">';
  $html .= t('won:') . " " . $player->won() . " " . t('lost:') . " " . $player->lost() . " " .
      t('drawn:') . " " . $player->drawn() . '</div>';
  $html .= "<br />";

  return $html;
}

/**
 * menu callback to display all active games
 */
function vchess_all_current_games_page() {
  global $user;

  if (user_is_logged_in()) {
    $gamefolder =  variable_get('vchess_game_files_folder', '/vchess-data');
    $res_games = $gamefolder;

    if (!$user->name) {
      $txt = t('Please, register to play chess');
      return $txt;
    }

    $out = vchess_all_current_games();
    $out .= vchess_new_game_links();
  }
  else {
    $out = "Please log in to access this page";
  }

  return $out;
}

/**
 * Get the link to the page for creating a new game
 */
function vchess_new_game_links() {
  $links = "";

  $links .= "<p>";
  $links .= "[ <a href=" . url("vchess/create_challenge") . ">Create challenge</a> ] ";
//   $links .= "[ <a href=" . url("vchess/random_game_form") . ">New random game</a> ] ";
//   $links .= "[ <a href=" . url("vchess/opponent_game_form") . ">New opponent game</a> ]";
  $links .= "</p>";

  return $links;
}

/**
 * Get table of all current games
 */
function vchess_all_current_games() {
  global $user;

  $rows = array();

  $html = "";

  $uid = $user->uid;

  // Get the list of possible games to view
  $games = vchess_load_all_current_games();

  $html = vchess_build_current_games_table($games, $user);

  return $html;
}

/**
 * Get page of all current challenges
 */
function vchess_challenges() {
  global $user;

  $rows = array();

  $html = "";

  // Get the list of possible games to view
  $games = vchess_load_challenges();

  $html = vchess_build_challenges_table($games, $user);
  $html .= vchess_new_game_links();

  return $html;
}

/**
 * Get table of current games for the current player
 *
 * @param $name
 *   Name of player for whom list of games is required
 *
 * @return
 *   Table of current games as HTML
 */
function vchess_users_current_games($name) {
  $named_user = user_load_by_name($name);

  // Get the list of possible games to view
  $games = vchess_load_users_current_games($named_user->uid);

  $html = vchess_build_current_games_table($games, $named_user);

  return $html;
}

/**
 * Construct a current games table
 */
function vchess_build_challenges_table($games, $named_user) {
  global $user;

  $rows = array();

  $theme = 'default';

  $html = "";

  $table['empty'] = "";
  if (count($games) > 0) {
    foreach ($games as $game) {
      $challenger = $game->challenger();
      if ($challenger->uid() <> $user->uid || MAY_PLAY_SELF) {
        $accept_link = "<a href='" . url("vchess/accept_challenge/" . $game->gid()) . "'>" . t('Accept') . "</a>"; 
      }
      else {
        $accept_link = "Pending";        
      }
      $rows[] = array(
        'challenger' => "<a href='" . url("vchess/player/" . $challenger->name()) . "'>" . $challenger->name() . "</a>",
        'rating' => $challenger->rating(),
        'speed' => $game->speed(),
        'gid' => $accept_link,
      );
  
    }
  }
  else {
    $table['empty'] = "There are currently no waiting challenges. You can " . "<a href='" .
        url("vchess/create_challenge") . "'>create a new challenge here.</a>";
  }

  $header = array(
      array('data' => t('Challenger'), 'field' => 'challenger'),
      array('data' => t('Rating'), 'field' => 'rating'),
      array('data' => t('Time limit per move'), 'field' => 'speed'),
      t('Accept'));

  // getting the current sort and order parameters from the url
  // e.g. q=vchess/my_current_games&sort=asc&order=White
  $sort = tablesort_get_sort($header);
  $order = tablesort_get_order($header);

  if (count($rows) > 1) {
    // sort the table data accordingly
    $rows = tsort_nonsql_sort($rows, $sort, $order['sql']);
  }

  $table['header'] = $header;
  $table['attributes'] = array();
  $table['caption'] = t("Challenges");
  $table['colgroups'] = array();
  $table['sticky'] = "";
  $table['rows'] = $rows;

  $html .= theme_table($table);

  return $html;
}

/**
 * Construct a current games table
 */
function vchess_build_current_games_table($games, $named_user) {
  global $user;
  global $base_url;

  $rows = array();

//   $module_path = drupal_get_path('module', 'vchess');  // e.g. sites/all/modules/vchess
//   //  $view_path = "chess_drupal_7/vchess";
//   $view_path = $GLOBALS['base_root'] . url("vchess");
//   //    $full_path = "http://localhost/chess_drupal-7.12/" . drupal_get_path('module', 'vchess');
//   $full_path = HOME_ROOT . BASE_DIR . drupal_get_path('module', 'vchess');

  $theme = 'default';

  $html = "";

  $table['empty'] = "";
  if (count($games) > 0) {
    foreach ($games as $game) {
      // We need to check first if the game has recently been lost on time, in
      // which case it is no longer a current game.  If it has, then this is the
      // first time it has been noticed (since this game was in "In progress")
      // and so we need to update the gamer statistics
      if ($game->is_lost_on_time()) {
        vchess_update_player_stats($game);
      }
      else {
        if ($game->is_user_playing($user->uid)) {
          if ($game->is_players_move($user->uid)) {
            $mark = 'greenmark.gif';
            $alt = '1.green'; // alt text is used so sort order is green, red, grey
          }
          else {
            $mark = 'redmark.gif';
            $alt = '2.red';
          }
        }
        else {
          $mark = 'greymark.gif';
          $alt = '3.grey';
        }

        if ($game->turn() == 'w') {
          $player_to_move = $game->white_player();
        }
        else {
          $player_to_move = $game->black_player();
        }
        
        $long_time = sprintf("%07d", $game->time_left());
        $rows[] = array(
            'move' => "<img alt='$alt' src='" . $base_url . "/" . drupal_get_path('module', 'vchess') . '/images/' . $theme . '/' . $mark . "'>",
            'white' => "<a href='" . url("vchess/player/" . $game->white_player()->name()) . "'>" . $game->white_player()->name() . "</a>",
            'black' => "<a href='" . url("vchess/player/" . $game->black_player()->name()) . "'>" . $game->black_player()->name() . "</a>",
            'move_no' => $game->move_no(),
            // We use div id in secs to ensure sort works correctly
             'time_left' => "<div id='" . $long_time . "'>" . 
                 vchess_user_friendly_time($game->time_left()) . "</div>",
            'speed' => $game->speed(),
            'turn' => "<a href='" . url("/vchess/player/" . $player_to_move->name()) . "'>" . $player_to_move->name() . "</a>",
            'gid' => "<a href='" . url("vchess/game/" . $game->gid()) . "'>" . t('View') . "</a>",
        );
      }
    }
  }
  else {
    if ($user->uid == $named_user->uid) {
      $table['empty'] = t("You currently do not have any games.<br />" . 
          "<br />" .
          "You can find a <a href='" . url("vchess/challenges") . "'>list of open challenges here</a>, and you can " . 
          "<a href='" . url("vchess/create_challenge") . "'>create a new challenge here.</a>");
    }
    else {
      $table['empty'] = $named_user->name . " " . t("does not have any current games.");
    }
  }

  $header = array(
      array('data' => t('Your move?'), 'field' => 'move'),
      array('data' => t('White'), 'field' => 'white'),
      array('data' => t('Black'), 'field' => 'black'),
      array('data' => t('Move #'), 'field' => 'move_no'),
      array('data' => t('Time left'), 'field' => 'time_left'),
      array('data' => t('Time per move'), 'field' => 'speed'),
      array('data' => t('Turn'), 'field' => 'turn'),
      t('View'));

  // getting the current sort and order parameters from the url
  // e.g. q=vchess/my_current_games&sort=asc&order=White
  $sort = tablesort_get_sort($header);
  $order = tablesort_get_order($header);

  if (count($rows) > 1) {
    // sort the table data accordingly
    $rows = tsort_nonsql_sort($rows, $sort, $order['sql']);
  }

  $table['header'] = $header;
  $table['attributes'] = array();
  $table['caption'] = t("Current Games");
  $table['colgroups'] = array();
  $table['sticky'] = "";
  $table['rows'] = $rows;

  $html .= theme_table($table);

  return $html;
}

/**
 * Update the player statistics
 *
 * @param $game
 *   The game which has just finished
 */
function vchess_update_player_stats(Game $game) {
  switch ($game->status()) {
    case STATUS_WHITE_WIN:
      $score = GAMER_WHITE_WIN;
      break;
    case STATUS_BLACK_WIN:
      $score = GAMER_BLACK_WIN;
      break;
    case STATUS_DRAW:
      $score = GAMER_DRAW;
      break;
  }

  gamer_update_stats($game->white_uid(), $game->black_uid(), $score);
}


/**
 * Process the form to start a new randomgame
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function vchess_random_game_form_submit($form, &$form_state) {
  global $user;

  $opponent_uid = vchess_random_uid();

  // Get the uid and name of a random opponent
  $sql = "SELECT uid, name FROM {users} WHERE uid = '" . $opponent_uid . "'";
  $result = db_query($sql);

  $record = $result->fetchAssoc();
  $opponent_name = $record['name'];

  if (rand(0, 1)) {
    // user plays white
    $white_uid = $user->uid;

    // opponent plays black
    $black_uid = $opponent_uid;
  }
  else {
    // user plays black
    $black_uid = $user->uid;

    // opponent plays white
    $white_uid = $opponent_uid;
  }

  $game = new Game();
  $game->set_players($white_uid, $black_uid);
  $gid = $game->gid();
  gamer_start_game($white_uid, $black_uid);

  drupal_set_message(t('Game has been created.'));
  $form_state['redirect'] = 'vchess/game/' . $gid;
}

/**
 * A single test
 * 
 * This function is for running individual tests normally found
 * within the vchess.test file, but without all the heavy setup
 * and teardown time which those functions need. 
 */
function vchess_test() {
  $html = "";
  
  gamer_start_game(1, 1);

  $player = new Player(1);
  $player->set_current(-25);
  
  $html .= "time() is: " . date("Y-m-d H:i:s", time()) . "<br />";
  $html .= "SERVER REQUEST_TIME is: " . date("Y-m-d H:i:s", $_SERVER['REQUEST_TIME']) . "<br />";
  $html .= "gmdate() is: " . gmdate("Y-m-d H:i:s");
  
  return $html;
}
