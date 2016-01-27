<?php

/**
 * @file
 * A class representing matches.
 */

/**
 * A class representing matches.
 */
class TabTMatch {
  public $id; // Matchnumber
  public $err; // Error
  public $isHome = FALSE; // Club is hometeam
  public $isAway = FALSE; // Club is awayteam
  public $WeekName;
  public $Date;
  public $Time;
  public $MatchId; // MatchID
  public $MatchUniqueId; // Matchnumber
  public $Venue; // Location
  public $HomeClub;
  public $HomeTeam;
  public $AwayClub;
  public $AwayTeam;
  public $Score;
  public $HomeScore;
  public $AwayScore;
  public $Link;
  public $VTTLLink;
  public $Withdrawal = -1;
  public $Results = FALSE;
  public $Cache = FALSE;
  public $Season;

  /**
   * Copy the data from a MatchEntry into this object.
   *
   * @param matchentry
   *   The MatchEntry to get the data from.
   */
  public function copy_from_tabt_entry($matchentry, $season = NULL) {
    $season = tabt_process_seasonnumber($season);
    $this->Season = $season;
    // We'll copy everything in our own object.
    // Since the MatchEntry does not always have all properties, we
    // have to check some first if they exist.
    $this->WeekName = $matchentry->WeekName;
    if (isset($matchentry->Date)) {
      $this->Date = $matchentry->Date;
    }
    if (isset($matchentry->Time)) {
      $this->Time = $matchentry->Time;
    }
    $this->MatchId = $matchentry->MatchId;
    if (isset($matchentry->Venue)) {
      $this->Venue = $matchentry->Venue;
    }
    $this->HomeClub = $matchentry->HomeClub;
    if (strpos($this->HomeClub, variable_get('tabt_club_id', 'Vl-B000')) !== FALSE) {
      $this->isHome = TRUE;
    }
    $this->HomeTeam = $matchentry->HomeTeam;
    $this->AwayClub = $matchentry->AwayClub;
    if (strpos($this->AwayClub, variable_get('tabt_club_id', 'Vl-B000')) !== FALSE) {
      $this->isAway = TRUE;
    }
    $this->AwayTeam = $matchentry->AwayTeam;
    if (isset($matchentry->Score)) {
      $this->Score = $matchentry->Score;
      $score = explode('-', $matchentry->Score);
      $this->HomeScore = $score[0];
      $this->AwayScore = $score[1];
      $score2 = explode(' ', $score[1]);
      if (isset($score2[1]) && $score2[1] != '') {
        $this->AwayScore = $score2[0];
        if ($this->AwayScore == 0) {
          $this->Withdrawal = 1;
        }
        else {
          $this->Withdrawal = 0;
        }
      }
    }
    if (isset($matchentry->MatchUniqueId)) {
      $this->id = $matchentry->MatchUniqueId;
      $this->MatchUniqueId = $matchentry->MatchUniqueId;
      if (!tabt_is_current_season($season)) {
        $this->Link = variable_get('tabt_matches_matchpage', 'championship/match') . '/' . $this->MatchUniqueId . '/' . tabt_get_seasonname($season);
      }
      else {
        $this->Link = variable_get('tabt_matches_matchpage', 'championship/match') . '/' . $this->MatchUniqueId;
      }
      $this->VTTLLink = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php?menu=4&detail=1&list=1&sel=' . $this->MatchUniqueId;
    }
  }

  /**
   * Try creating the match with only the id given.
   */
  public function reconstruct() {
    $this->Venue = 1;
    if ($this->id) {
      $this->MatchUniqueId = $this->id;
      $this->Link = variable_get('tabt_matches_matchpage', 'championship/match') . '/' . $this->id;
      $this->VTTLLink = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php?menu=4&detail=1&list=1&sel=' . $this->id;
    }
  }

  /**
   * Get the results and process the involved players if the tabt_members
   * module is loaded.
   */
  public function get_results() {
    if (!$this->id || $this->id == '') {
      $this->err = TRUE;
      $this->errinfo = 'no id';
      return;
    }
    // Load the results
    $results = FALSE;

    $this->load_results();

    // If the TabT Members module is loaded, we can change the link of the
    // players.
    if (module_exists('tabt_members')) {
      $this->processPlayers();
    }
  }

  /**
   * Process the players.
   */
  protected function processPlayers() {
    // Make sure this is never called if the TabT Members-module isn't loaded
    if (!module_exists('tabt_members')) {
      return;
    }
    // Get an array of members where "<LastName> <Firstname>" is the key.
    // We need it in this format because the results lists players in this
    // format.
    $members = tabt_members_get_members_by_lastfirst_name($this->Season);
    // Process the hometeam
    if ($this->isHome && isset($this->HomeTeamMembers)) {
      $this->process_team($this->HomeTeamMembers, $members);
    }
    // Process the awayteam
    if ($this->isAway && isset($this->AwayTeamMembers)) {
      $this->process_team($this->AwayTeamMembers, $members);
    }
  }

  /**
   * Process the players of a team.
   *
   * @param &team
   *   A pointer to the team-array.
   * @param members
   *   The list of all clubmembers.
   */
  protected function process_team(&$team, $members) {
    // Check each player and change their Name and Link if they are one of the
    // members.
    foreach ($team as &$teammember) {
      if (isset($teammember->Name) && isset($members[drupal_strtoupper($teammember->Name)])) {
        if (isset($members[drupal_strtoupper($teammember->Name)]->Link)) {
          $teammember->Link = $members[drupal_strtoupper($teammember->Name)]->Link;
        }
        if (isset($members[drupal_strtoupper($teammember->Name)]->Name)) {
          $teammember->Name = $members[drupal_strtoupper($teammember->Name)]->Name;
        }
      }
    }
  }

  /**
   * Load the details of a match.
   */
  protected function load_results() {
    if (!module_load_include('inc', 'tabt_matches', 'includes/tabt_matches.load_results')) {
      return;
    }
    _tabt_matches_load_results($this);
  }

  /**
   * Copy all values from another TabTmatch-object into this object.
   *
   * @param new
   *   The object to copy.
   */
  public function copy($new) {
    foreach ($new as $key => $value)
      $this->$key = $value;
  }
}
