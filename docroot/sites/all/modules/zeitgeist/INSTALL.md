# Zeitgeist (ZG) module for Drupal

    @copyright (C) 2005-2013 Ouest Systèmes Informatiques SARL (OSInet) 
    @author Frederic G. MARAND
    @license Licensed under the General Public License version 2 or later

## 1. Prerequisites

- Drupal 7.x
- a MySQL or PostgreSQL version supported by the version of Drupal
    - Alternative databases (eg Microsoft SQL) might work...
    - ... but Zeitgeist includes engine-specific calls to improve performance on 
      these specific servers.

##2. Installing


a. Copy the `zeitgeist` directory to  
   `<drupal>/sites/all/modules/zeitgeist`
b. Enable the module at  
   `http://www.example.com/admin/modules`
c. The module is ready. Optional subsequent steps are:
d. configure the Zeitgeist settings at:  
   `http://www.example.com/admin/config/search/settings/zeitgeist`   
e. Configure the ZG/Latest and ZG/Top blocks at :
   `http://www.example.com/admin/structure/block/manage/zeitgeist/latest/configure`  
   `http://www.example.com/admin/structure/block/manage/zeitgeist/top/configure`


## 3. Upgrading

You can upgrade from already installed older versions of Zeitgeist by going to  
`http://www.example.com/update.php`

If you are upgrading from a previous version, it is usually wise to empty the
zeitgeist table or, at least, hand-check it as some erroneous entries may have 
been introduced by a previous version. This is not done automatically because 
the values may be actually usable.


## 4. Uninstalling

Drupal now supports uninstalling and Zeitgeist implements it, so the procedure
is just:

a. disable the module at `admin/modules`
b. uninstall it at `admin/modules/uninstall`

This will remove all variables, data, and cache entries. Be sure to save your
Zeitgeist table elsewhere if you want to keep it for use without the module.


## 5. Reading The Fine Documentation

Most of the module functionality is actually only visible at the PHP level, 
not in the Drupal UI. Specifically, it includes human-readable date spans, for 
which you can find an example in function `zeitgeist_page_results_simple()` 
and`zeitgeist_page_results_multicolmun()`.

It can be leveraged in your site's customs module(s) or just in custom blocks,
so you really should look at the module code, or you'll be missing most of the 
features of the module.

- On Drupal.org:   
  <http://drupal.org/project/zeitgeist>
- Or on the Audean Wiki:  
  <http://wiki.audean.com/zg/zeitgeist?s=zeitgeist>
