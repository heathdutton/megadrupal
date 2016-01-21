Full version of this readme exists at:
https://docs.google.com/document/d/1BufsyQSEAWghg8ABPlIoaKb03eI1MUB-tymFvSvoi9c/edit

Installation:

1) Navigate to “Modules” [admin/modules] and enable Alter Ego module.
Click “Save” at the bottom of the page.  Once the page has refreshed, enable “WoW Guild” and click “Save” again.
Note: There is a known bug that does not load dependencies in the proper order and will cause an error if wowguild is enabled alone.

2) Navigating to "Add World of Warcraft Toon" [admin/content/avatars/add/wowtoon]
Enter your main character’s name and realm.

3) Navigate to "Configuration > WoW Guild"  [admin/config/system/wowguild] and enter your guild's name and realm.
This should be autoloaded if you have added a character to your account.
(optional) review your guild rank names, cron options, language to use with wow armory and API key if you are requesting large number of characters.

4) Enable authenticated users (or whatever role you choose) to add World of Warcraft Toons to their account.
Navigate to "People > Permissions" [admin/people/permissions] and check the Authenticated User box under "World of Warcraft Toon: Create Avatar".
Check the Anonymous User (if desired) and Authenticated User next to “World of Warcraft Toon: View any avatar”.
  
5) If the "Roster" main menu item is not displayed, clear the cache to reset the views item.

6) If WoW Guild cron is enabled (in settings) then each cron run will update the characters in your guild.  You can run multiple subsequent crons to speed this process.