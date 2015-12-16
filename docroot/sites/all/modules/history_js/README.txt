The History.js Module is a Drupal 7 Library Implementation of
Benjamin Lupton's History.js

History.js enables a clean stable implementation of html5 History.popStates and
gracefully degrades to hash changes where the html5 popstates aren't supported.

Installation:

  1. Download the latest version of History.js
     (from https://github.com/balupton/History.js)

  2. Extract the contents of the archive to:
     <Drupal>/sites/<example.com | all>/libraries/balupton-history.js

     Such that the bundled directory exists at:
     <...>/libraries/balupton-history.js/scripts/bundled


  3. Verify You have the Libraries API installed

  4. Disable the Overlay Module

  5. Enable Libraries and History.js

  6. Visit admin/settings/admin/config/history_js to chose your configuration

  6a. ...

  7. Profit
