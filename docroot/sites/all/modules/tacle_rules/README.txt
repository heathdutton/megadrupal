***********************
* Sponsors
***********************

  Version 6.x-1.x-dev by Made to Order Software Corp.
  http://www.m2osw.com/drupal

  Version 6.x-2.x and 7.x-1.x by Coda
  http://www.codamoda.com


***********************
* Dependencies
***********************

  Rules: http://drupal.org/project/rules
  TAC Lite: http://drupal.org/project/tac_lite

***********************
* Upgrade
***********************

  There is no migration path from 6.x-1.x-dev. All rules using this version will
  have to be rebuilt after upgrade. Database update not required after upgrade.

  Please delete the 'sites/all/modules/tacle_rules' before following the
  installation instructions below.


***********************
* Installation
***********************

  Extract this module into your Drupal 'sites/all/modules' folder. Browse to
  'admin/build/modules', select the 'TACLe Rules' check-box (found inside the
  Rules package), and click the 'Save configuration' button at the bottom of the
  page.


***********************
* Usage
***********************

  Once installed this module adds functionality in the Rules module. Add a new
  rule action or condition. You may need to 'load a term' and 'load a user'
  before an action becomes visible.


***********************
* Disable & un-install
***********************

  Please note that disabling this module will cause all rules that use it to
  stop functioning.

  Go to 'admin/build/modules' and un-check the check-box by 'TACLe Rules' then
  click the 'Save configuration' button at the bottom of the page. Go to
  'admin/rules' and edit/disable all rules that use this module. This module
  does not create any database tables so there is no un-install.
