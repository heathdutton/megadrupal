Simple Clean
------------

The main purpose of the Simple Clean Theme is for it to be simple to get a clean simple site up and running in no time. Just install and follow the step-by-step guide and your simple but still good looking clean website is running within the hour.

Simple Clean is a two column fixed design (optimized for width 960px). The content column have width 610px and right column (sidebar) have width 260px.


News in 7.x
-----------

* No more built in sub navigation. Use the module Menu Block instead (http://drupal.org/project/menu_block).
* Links instead of buttons for Read more and Add comments etc.
* New design for tags


Quick step-by-step
------------------

Setting up a Drupal site involves many steps. This is some of the steps I usually go through when setting up a Drupal site with Simple Clean Theme:

1. Install Drupal 7.x
2. Install Simple Clean Theme (Put simpleclean-folder in: sites/all/themes)
3. Enable and make Simple Clean default (admin/appearance)
4. Edit theme settings (admin/appearance/settings/simpleclean) - Add mission, slogan, etc.
5. Edit site information (admin/config/system/site-information)
6. Add Home to Main menu (admin/structure/menu/manage/main-menu/add)

If you want user pictures or signatures:

1. Enable user pictures and/or signatures (admin/config/people/accounts)
2. For Default picture use: /sites/all/themes/simpleclean/images/no-pic.png
3. Make user pictures in posts and/or user pictures in comments are enabled in theme settings (admin/appearance/settings/simpleclean)

If you want a search engine on your site:

1. Activate search module (admin/modules) or install the Search By Page module (http://drupal.org/project/search_by_page) which is a bit more sophisticated
2. Edit search settings (admin/config/search/settings)
3. Set permissions for search module (admin/people/permissions)
4. Run cron to index search and active cron at your host or install the modele Poor mans cron if you host dosn't support cron


Tips & tricks
-------------

If you install a WYSIWYG editor like CK Editor. Don't forget to edit the settings for comment text format at (admin/structure/types/manage/article/comment/fields/comment_body)


Author's Information
--------------------

Simple Clean is designed and developed by Mattias Axelsson (drupalname: acke) at Happiness Web Agency (www.happiness.se) in the beautiful city Stockholm, Sweden.

Follow me at http://twitter.com/mattiasaxelsson

Enjoy!
