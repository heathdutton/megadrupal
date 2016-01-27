
CONTENTS OF THIS FILE
-----------------------------------------------------------------------------------------
 * Introduction
 * Installation
 * Usage

  
INTRODUCTION
-----------------------------------------------------------------------------------------
Before you install the video chat module, please read and agree
to the conditions found at http://www.camamba.com/free_chat_applet.php


INSTALLATION
-----------------------------------------------------------------------------------------
1. Copy the entire videochat module directory into your normal directory for modules, 
   usually sites/all/modules.
   
2. Enable the Video Chat in the modules administration.

3. Edit the modules permissions to open it for registered or all users.

4. Enable the Video Chat block in the region of your choice in the Blocks module configuration.

5. Customize your chat appearance in the User Interface section of the configuration menu.


USAGE
-----------------------------------------------------------------------------------------
 * This module will provide a block on your site with an age and gender input form and an "Enter Video Chat" button.
   Access to this block is restricted by default to user #1 (the admin user).

 * Administrators will see a different form with an optional moderator password field and a setup link. (Users get a sponsored link and no password field)

 * To allow authenticated and/or anonymous users to see the block, set the permissions accordingly at admin/user/permissions.

 * If the current user is authenticated (registered), clicking on the button will automatically populate the "user name" in the URL with the user's registered username ($user->name);
   if the user is an anonymous (non-registered) user, the "user name" part will instead be filled with a "Guest ###" name.
   Either way, the "room id" will automatically be populated with "example.com" (the "www." will be stripped from the URL of your site).

 * Moderator access is available through the Camamba website. (Setup: http://www.camamba.com/admin_create.php)

 * The enhanced video quality of the chat is automatically available to all Camamba premium users, but they have to log into the Camamba website first.

 * This module was written and published with the express written consent of the provider (camamba.com) with such consent being granted to the module's author on July 13, 2009.

 * Behave.  :)