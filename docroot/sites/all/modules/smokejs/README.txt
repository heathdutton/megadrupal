Provide [smoke.js](http://smoke-js.com) library --
framework-agnostic styled alert system for javascript.

## Feature ##
* Replace drupal ajax error alert with `smoke.alert`.
* Replace drupal ajax command alert with `smoke.alert`.
* New ajax command `ajax_command_smoke_signal` to provide `smoke.signal` in ajax
  callback.

## Requirements ##
* [Libraries API](http://dgo.to/libraries)

## Installation ##
1. Get the smoke.js Library
  * Via Git

      ```
$ cd sites/SITENAME/libraries
$ git clone https://github.com/hxgf/smoke.js.git
      ```
  * Download
      * Download smoke.js library from
        https://github.com/hxgf/smoke.js/archive/master.zip.
      * Extract compressed smoke.js library.
      * Rename `smoke.js-master` directory to `smoke.js`.
      * Move `smoke.js` directory to your `sites/SITENAME/libraries`
        directory.

2. Enable Smoke.js Module
   ```
   $ drush en smokejs
   ```

   Or enable via administration page `admin/modules`.

3. Configure Smoke.js Module
   Visit configuration page `admin/config/user-interface/smokejs`.
