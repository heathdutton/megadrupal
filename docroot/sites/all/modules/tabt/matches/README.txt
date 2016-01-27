CONTENTS OF THIS FILE
---------------------

 * Summary
 * Installation
 * Configuration
 * Theming
 * PHP Functions
 
 SUMMARY
 --------------
 This Submodule provides all functionality related to matches.
 It provides a Calendar and Rankinglist per team and Matchpages for each match
 played by your club.
 
 REQUIREMENTS
 --------------
 
   * TabT Module
   
 INSTALLATION
 --------------
 
   * Install as usual. See https://drupal.org/node/895232 for further
   instructions.
 
 CONFIGURATION
 --------------
   1. Go to admin/config/services/tabt/matches and edit the settings:
   
     * TabT Calendarpage Link: The path to the calendarpage of a team. % will
       be replaced by the teamletter. (default: championship/calendar/%)
     * TabT Calenderpage Title: The pagetitle of the calendar of a team.
       !club will be replace by the clubname, !team by the teamletter and
       !division by the divisionname. (default: Calendar !club !team)
     * TabT Rankingpage Link: The path to the rankingpage of a team. % will be
       replaced by the teamletter. (default: championship/ranking/%)
     * TabT Rankingpage Title: The pagetitle of the ranking of a team. The same
       replacements as with the calendarpage can be used.
     * TabT Matchpage Link: The path to a matchpage. % will be replaced by the
       Matchnumber (e.g. 201937). (default: championship/match/%)
     * TabT Matchpage Title: The pagetitle of a matchpage. !matchid will be
       replaced by the Match-ID (e.g. VlB01/234), !hometeam and !awayteam by
       the 2 teams, !score by the score and !week by the weekname.
       (default: Match !matchid: !hometeam - !awayteam)
     * Reportslist Viewname: If you make reports about your matches, you can
       make a View with a block that shows a list of reports for a match.
       The first argument should be the match-number (e.g. 201937).
       You can enter the viewname here if you want it to be included on
       matchpages.
     * Load match details: You can choose to load details about the match
       (the involved players, the matches, etc.). Note that this can increase
       loadtimes. (default: disabled)
     * Show setscores: The details of some matches include the scores for each
       set. Enable this if you want to show these on the matchpage.
       (default: disabled)
       Note: Load match details needs to be enabled for this.
       
   2. Clear the cache tables for these changes to be taken into effect.
   
   3. Go to admin/structure/menu and add links to calendars and rankings.
   
   4. Go to admin/structure/blocks and choose which blocks to use.
   
 THEMING
 --------------
   You can theme the Calendars, Rankings and Matchpages with theme functions.
   
   1. Calendars:
     * Theme: tabt_calendar
     * Variable: matches: array of TabTMatch-objects
     Each TabTMatch-object has the following properties:
       id: The Matchnumber (e.g. 201937) 
       err: If there was an error trying to load the matchdetails.
       isHome: True if the hometeam is from this club.
       isAway: True if the awayteam is from this club.
       WeekName: The Weekname (A, 01, 02, ...).
       Date*: The Date of the match.
       Time*: The Time of the match.
       MatchId*: The Match-ID (e.g. VlB01/234).
       MatchUniqueId*: The Matchnumber (e.g. 201937) .
       Venue*: The number of the homeclub-location the match is played at.
       HomeClub: The ID of the homeclub (e.g. Vl-B123).
       HomeTeam: The name of the hometeam (e.g. City A).
       AwayClub: The ID of the awayclub.
       AwayTeam: The name of the awayteam.
       Score: The score (e.g. 07 - 09). If there was a withdrawal, w (or the
       equivalent in the users' language, will be appended.
       HomeScore: The matches the hometeam won (e.g. 07).
       AwayScore: The matches the awayteam won (e.g. 09).
       Withdrawal: Which team withdrew (None: -1, Home: 0, Away: 1, Both: 2).
       Link: The link to the local matchpage.
       VTTLLink: The link to the matchpage on the TabT Results-site.
       Results: Whether or not matchdetails were loaded. (default: false)
       Season: The seasonnumber of the match.
     Note: All of these properties will be set. Some of them can however be
     NULL.
     Note: Data in this object is not sanitized. For more info, see
       https://drupal.org/writing-secure-code
			
       *: These properties can be NULL if the team does not have to play that
          week.
     * Variable: team: The teamletter
     * Variable: teaminfo: a TeamEntry-object
       See http://api.frenoy.net/tabtapi-doc/classTeamEntry.html for more
       details.
       Note: Data in this object is not sanitized. For more info, see
       https://drupal.org/writing-secure-code
     * Variable: season: The number of the season being viewed.
			
   2. Rankingpage
     * Theme: tabt_ranking
     * Variable: ranking: array of RankingEntry-objects.
       See http://api.frenoy.net/tabtapi-doc/classRankingEntry.html for more
       details.
       Note: Data in these objects is not sanitized. For more info, see
       https://drupal.org/writing-secure-code
     * Variable: team: The teamletter
     * Variable: teaminfo: a TeamEntry-object
       See http://api.frenoy.net/tabtapi-doc/classTeamEntry.html for more
       details.
       Note: Data in this object is not sanitized. For more info, see
       https://drupal.org/writing-secure-code
     * Variable: season: The number of the season being viewed.
            
   3. Matchpage
     * Theme: tabt_matchpage
     * Variable: match: TabTMatch-object.
       The TabTMatch-object has the same properties as on Calendarpages, with
       the addition of several other properties containing details (if
       enabled). Note that the available properties depend on the available
       details at the TabT Results-site and should not be used without first
       checking if the property exists.
       Use the Result-property to see if any matchdetails were loaded.
       Note: Data in this object is not sanitized. For more info, see
       https://drupal.org/writing-secure-code
   
   