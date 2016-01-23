<?php

/**
 * @file
 * A class representing Club Members
 */

/**
 * A class representing Club Members
 */
class TabTClubMember {

  public $Position;
  public $RankingIndex;
  public $UniqueIndex;
  public $Name;
  public $FirstName;
  public $LastName;
  public $Ranking;
  public $Link;
  public $VTTLLink;
  public $Category;
  public $Status;
  public $Gender;
  public $BirthDate;
  public $UserID;
  public $ProfilePic;
  public $DefaultPic;
  public $Results;
  public $Season;
  public $ResultsLoaded = FALSE;

  /**
   * Copy the values from a MemberEntry-object into this object.
   *
   * @param memberentry
   *   A MemberEntry-object to copy from.
   * @param season
   *   The season to look at.
   */
  public function copy_from_tabt_entry($member_entry, $season = NULL) {
    // Copy everything into our own object. Some properties may not be set,
    // depending on if we requested extended information or not.
    $season = tabt_process_seasonnumber($season);
    $this->Season = $season;
    $this->Position = $member_entry->Position;
    $this->RankingIndex = $member_entry->RankingIndex;
    $this->UniqueIndex = $member_entry->UniqueIndex;
    if (tabt_is_current_season($season)) {
      $this->VTTLLink = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/' . $member_entry->UniqueIndex;
    }
    else {
      $this->VTTLLink = 'http://' . variable_get('tabt_site', 'competitie.vttl.be') . '/index.php?result=1&index=' . $member_entry->UniqueIndex . '&category=1&season=' . $season;
    }
    $this->Ranking = $member_entry->Ranking;
    $this->FirstName = $member_entry->FirstName;
    $this->LastName = $member_entry->LastName;
    $this->UserID = tabt_members_get_user($this->UniqueIndex);
    $this->Name = tabt_members_get_name($this->UniqueIndex, $this);
    $this->Link = tabt_members_get_link($this->UniqueIndex, $season);
    if (isset($member_entry->Status)) {
      $this->Status = $member_entry->Status;
    }
    if (isset($member_entry->Category)) {
      $this->Category = $member_entry->Category;
    }
    if (isset($member_entry->Gender)) {
      $this->Gender = $member_entry->Gender;
    }
    if (isset($member_entry->BirthDate)) {
      $this->BirthDate = $member_entry->BirthDate;
    }
    // See what profile picture we need to use.
    $this->set_profile_pic();
  }

  /**
   * Set what Profile Picture to use.
   */
  protected function set_Profile_Pic() {
    // We already have one, so we don't need to set another one.
    if ($this->ProfilePic) {
      return;
    }
    // Let's see if we've set one on the adminpage.
    $query = db_select('tabt_members', 't')
    ->fields('t')
    ->condition('id', $this->UniqueIndex)
    ->execute()
    ->fetchObject();
    if ($query) {
      $result = $query->pic;
      if ($result) {
        // If it's an external image, we can't check if it exists, so we'll
        // assume it's ok.
        if (strpos($result, 'http://') !== FALSE) {
          $this->ProfilePic = $result;
          return;
        }
        // If it's a local image, only use it if it actually exists.
        if (file_exists($result)) {
          $this->ProfilePic = $result;
          return;
        }
      }
    }
    // We haven't gotten one yet, so let's see if there was one in the results.
    if (isset($this->Results->Pic)) {
      if (strpos($this->Results->Pic, 'No_Photo') === FALSE) {
        $this->ProfilePic = $this->Results->Pic;
        return;
      }
    }
    // We'll have to set a default picture.
    if (variable_get('user_picture_default')) {
      // If the member is female, we'll first try to get a female picture.
      // Only try this if the default picture ends on _m
      if ($this->Gender == 'F' && strpos(variable_get('user_picture_default'), '_m.') !== FALSE) {
        // If the default picture is external, we'll assume there is also a
        // female picture.
        if (strpos(variable_get('user_picture_default'), 'http://') !== FALSE) {
          $this->DefaultPic = str_replace('_m.', '_f.', variable_get('user_picture_default'));
          return;
        }
        // If it's a local picture, we'll only set it if there is a female
        // picture.
        if (file_exists(str_replace('_m.', '_f.', variable_get('user_picture_default')))) {
          $this->DefaultPic = str_replace('_m.', '_f.', variable_get('user_picture_default'));
          return;
        }
      }
      // We'll be using the default picture.
      // If it's external, we'll assume it exists.
      if (strpos(variable_get('user_picture_default'), 'http://') !== FALSE) {
        $this->DefaultPic = variable_get('user_picture_default');
        return;
      }
      // If it's local, we'll only use it if we can actually find it.
      if (file_exists(variable_get('user_picture_default'))) {
        $this->DefaultPic = variable_get('user_picture_default');
        return;
      }
    }
  }

  /**
   * Get and process individual results.
   */
  public function get_results() {
    // We can't get results if we don't have a playerID.
    if (!$this->UniqueIndex || $this->UniqueIndex == '') {
      return;
    }
    // Load the results.
    $this->load_results();
    // We may have gotten a new profile picture.
    $this->set_profile_pic();
    // Process the matches and change the links.
    $this->process_matches();
    if ($this->Gender == 'F') {
      $this->load_results(TRUE);
      $this->process_matches(TRUE);
    }
  }

  /**
   * Process all matches.
   *
   * The oponents' name and link will be updated.
   * All links to championshipmatches will be changed to local links if the
   * tabt_matches-module is loaded.
   */
  protected function process_matches($women = FALSE) {
    // If we don't have any matches, there's no point in doing this.
    if ($women) {
      if (!isset($this->WomenResults->Matches)) {
        return;
      }
      else {
        $match_list = &$this->WomenResults->Matches;
      }
    }
    else {
      if (!isset($this->Results->Matches)) {
        return;
      }
      else {
        $match_list = &$this->Results->Matches;
      }
    }
    // Get all members by last and first name.
    $members = tabt_members_get_members_by_lastfirst_name($this->Season);

    // Loop through each match.
    foreach ($match_list as &$match) {
      // We can only check this if we have a Type and a MatchLink and if the
      // Matches-module is loaded.
      if (module_exists('tabt_matches') && isset($match->Type) && isset($match->MatchLink)) {
        // Only change the matchlink for championship matches.
        if (($match->Type == 'Champ.' || $match->Type == 'Men' || $match->Type == 'Women') && preg_match('/\?sel=([0-9]*)\&/', $match->MatchLink, $matches)) {
          if (tabt_is_current_season($this->Season)) {
            $match->MatchLink = variable_get('tabt_matches_matchpage', 'championship/match') . '/' . $matches[1];
          }
          else {
            $match->MatchLink = variable_get('tabt_matches_matchpage', 'championship/match') . '/' . $matches[1] . '/' . tabt_get_seasonname($this->Season);
          }
        }
      }
      // If the opponent is also a member, try changing his name and link.
      if (isset($match->Opp)) {
        if (isset($members[$match->Opp])) {
          if (isset($members[$match->Opp]->Link)) {
            $match->OppLink = $members[$match->Opp]->Link;
          }
          if (isset($members[$match->Opp]->Name)) {
            $match->Opp = $members[$match->Opp]->Name;
          }
        }
        else {
          $match->Opp = ucwords($match->Opp);
        }
      }
    }
  }

  /**
   * Load the individual results from the results-site.
   */
  protected function load_results($women = FALSE) {
    if (!module_load_include('inc', 'tabt_members', 'includes/tabt_members.load_results')) {
      return;
    }
    _tabt_members_load_results($this, $women);
  }
}
