CONTENTS

1. Credits & Contact
2. Installation Instructions
3. Removal of "Theme by Avioso Designs" in Footer
4. Removal of "Menu" title for Navigation Block
5. 1024px version

--------------------------------------------------------------------------------

1. CREDITS & CONTACT

  Theme created by Avioso Designs
  www.avioso.com

  Please submit any problems to http://drupal.org/project/issues/252622

--------------------------------------------------------------------------------

2. INSTALLATION INSTRUCTIONS

  To install this theme simply place the entire Forest_Floor folder in your
  themes directory, located at your_drupal_site/themes .  Then log into your
  drupal site with any account that has sufficient privileges to administrate
  themes and head to the theme configuration page by Administer->Site Building->
  Themes. Tick the "Enabled" check box next to Forest Floor to enable it and if
  you wish to make it your default theme, select the "Default" radio box in line
  with Forest Floor as well.

--------------------------------------------------------------------------------

3. REMOVAL OF "THEME BY AVIOSO DESIGNS" IN FOOTER

  To remove the "Theme by Avioso Designs" in your footer, open page.tpl in a 
  text editor and remove the following code:
   
    <!-- Avioso Designs Credit, delete this section to remove -->
    <div style="color:#AAAAAA;">Theme by <a href="http://www.avioso.com" style="color:#AAAAAA;">Avioso Designs</a>.</div>
    <!-- End Avioso Designs Credit-->

--------------------------------------------------------------------------------

4. REMOVAL OF "MENU" TITLE FOR NAVIGATION BLOCK

Please note that if you simply want to translate this title into a different
language you may do so through use of the Internationalisation module.
You may, however, also delete block-user-1.tpl.php and then set the title in
Administer -> Blocks.

--------------------------------------------------------------------------------

5. 1024PX VERSION

To enable the 1024px version, you must install the Theme Settings module and its
requirements:

http://drupal.org/project/themesettings
http://drupal.org/project/themesettingsapi
