CONTENTS OF THIS FILE
---------------------

 * Summary
 * Installation
 * Configuration
 * Usage
 * Theming
 
 SUMMARY
 --------------
 This Submodule provides all functionality related to Players.
 It provides a Playerlist and player profiles with individual results
 (optional).
 If there is a Drupal User with the same playerid, the individual results will
 be printed on the userprofile.
 The memberlist will get a menuitem that is in the Navigation Menu by default.
 
 REQUIREMENTS
 --------------
 
   * TabT Module
   
 INSTALLATION
 --------------
 
   * Install as usual. See https://drupal.org/node/895232 for further
   instructions.
 
 CONFIGURATION
 --------------
   1. Go to admin/config/services/tabt/members and edit the settings:
   
     * TabT Memberlist path: This is the path of the memberlist and the
       playerprofiles. (default: members).
       When viewing playerprofiles, the player-ID will be appended.
     * TabT Memberlist title: This is the pagetitle of the memberlist.
       (default: Members)
            Note: The Menutitle will be Members or it's available translation.
     * ID Profile Field: If you want to link Drupal Userprofiles to
       playerprofiles, you can create a profilefield where users can enter
       their playerID. Enter the systemname (e.g. profile_playerid) here.
     * First Name Profile Field: If you have a profilefield for firstnames, you
       can enter it's systemname here. If you have also entered a lastname-
       field, their names in the memberlist will be replaced by the name
       they've entered. (See Usage)
     * Last Name Profile Field: If you have a profilefield for lastnames, you
       can enter it's systemname here. If you have also entered a firstname-
       field, their names in the memberlist will be replaced by the name
       they've entered. (See Usage)
     * NodeId ProfilePictures Page: If you have created an (admin-only) page
       where you can upload profile pictures for members that are not
       registered, fill in the node ID here. This will be used on admin-pages
       for linking in helptext only.
     * Load individual results: This module can also load individual results to
       be shown on playerprofiles. (default: disabled)
       Note: This can increase loadtimes.
            
   2. Clear all caches for these changes to be taken into effect.
   
   3. Go to admin/config/services/tabt/members/manage
      Here you can change the displayed name of your (unregistered) members.
      You can also change the profilepicture that is shown. (See Usage)
      Upon saving this page, the tabt-cache will be emptied automatically.
     
   4. Go to admin/structure/menu and change the menu in which the Memberslist
      should be shown. (default: Navigation)
      
 USAGE
 --------------
 
   1. Playernames
      Order of precedence:
        1. Name in user profilefields (First Name Profile Field &
           Last Name Profile Field)
        2. Name on TabT Site.
 
   2. Playerprofiles
   
     * Link
       Order of precedence:
         1. Player has account: user/<UID> (requires ID Profile Field in
            configuration)
         2. Player has no account: member/<PLAYERID> (can be changed at
            TabT Playerprofile path)
         
     * Profilepicture
       Order of precedence:
         1. Player has account and uploaded a User Picture: User Picture.
         2. Profile Picture in admin/config/members/members
         3. Profile Picture at TabT-Site, if not default-picture (Requires
            Individual Results)
         4. Default Drupal Profile Picture. If local picture and filename ends
            on _m, it will try to use _f for female members.
       
     * Individual Results
       If you have enabled Individual Results, the playerprofile will contain
       all sorts of details, like an overview of wins/losses per rank, all
       matches, etc.
       If you have disabled Individual Results, it will only contain the rank,
       age-category, profile picture and a link to the playerprofile on the
       TabT Results-site.
       
     * Matchlist
       If the TabT Matches-module is enabled, the link to Championship matches
       will be changed to a local matchpage-link.
       
 THEMING
 --------------
  You can theme both the memberlist and the player-profile with theme-functions.
     
   1. Memberlist
     * Theme: tabt_member
     * Variable: $members: array of TabTMember-objects
     Each TabTMember-object has the following properties:
       Position: The number of the player on the memberlist.
       RankingIndex: The Index-number of the player in the club.
       UniqueIndex: The Player-ID.
       Name: The user's displayed name (see Usage)
       FirstName: The user's First Name on the TabT-Site.
       LastName: The user's Last Name on the TabT-Site.
       Ranking*: The rank of the user. E6, E4, ...
       Link: The link to the local playersprofile.
       VTTLLink: The link to the playersprofile on the TabT-Site.
       Category*: The players age category. SEN, VET, MIN, ... 
       Status: The status of the user. A = Active, R = Recreatif, V = Reserve
       Gender: The user's gender. M = Male, F = Female
       BirthDate: The BirthDate of the player. Note: you need an admin-account
       on the TabT-Site to get this information.
       UserID: The Drupal UserID if this user has an account.
       ProfilePic: The link to a profilepicture if there is one set for this 
       user.
       DefaultPic: The defaultpicture to use if there is no profilepicture.
       Results: This will be NULL in the memberlist, unless it's still cached 
       from loading the profilepage.
       WomenResults: This will be NULL in the memberlist, unless it's still
       cached from loading the profilepage.
     Note: All of these properties will be set. Some of them can however be
     NULL.
     Note: To get Status, Gender, Category and BirthDate, you need to have
     entered valid Credentials at admin/config/tabt/credentials.
     Note: All data in this object is sanitized.
 
       *: This property depends on Language-settings for the TabT-account used.
     * Variable: season: The number of the season being viewed.
				        
  2. Memberpage
     * Theme: tabt_memberpage (also formats the data on User Profiles)
     * Variable: member: TabTMember-object
       This has the same properties as on the Memberlist. The Result-property
       will however have more if Individual Results are enabled. If enabled,
       the Result-property will be a standard Object. It's properties will
       depend on the data available at the TabT-Site and should not be used
       without first checking if the property exists.
       The WomenResult-property will contain Individual Results in Women-
       Divisions for female members. It's structure will be the same as that of
       the Result-property.
     * Variable: season: The number of the season being viewed.
       
     * Theme: tabt_memberpage_results (themes individual results)
     * Variable: results: The Results-property of a TabTMember-object.
         