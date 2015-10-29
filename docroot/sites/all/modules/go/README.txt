Features

1. Autoloader — @see go_autoload()

  - To disable this feature, in settings.php, define GO_DISABLE_AUTOLOAD constant.
  - Run faster with APC extension enabled.

2. go_cache()

  Without go_cache()

    function your_data_provider($reset = FALSE) {
      $cache_id = '…';
      $bin = 'bin';
      $expire = strtotime('+ 15 minutes');

      if (!$reset && $cache = cache_get($cache_id, $bin)) {
        return $cache->data;
      }

      $data = your_logic();

      cache_set($data, $cache_id, $bin, $expire);

      return $data;
    }

  With go_cache(), your logic becomes cleaner:

    function your_data_provider() {
      return your_logic();
    }

    $data = go_cache(array('cache_id' => '…'), 'your_data_provider');

3. Useful drush commands:

  # Download libraries
  drush godl ckeditor
  drush godl jquery.cycle
  drush godl jquery.cycle.2
  drush godl colorbox
  drush godl maxmind.geolite.country
  drush godl maxmind.geolite.country ipv6
  drush godl maxmind.geolite.city
  drush godl maxmind.geolite.city ipv6

  drush godev
    Shortcut for quick dev enviroments.

  drush golive --cache=1 --js=1 --update=1

    This is wrapper command for useful auto configuration for live site:
      - Enable page/block caching
      - Enable js/css aggregation
      - Disable UI modules (context, views, rules, …)
      - Enable update.module
      - drush help golive for more informations.

  Send message to Hipchat room:

    $ drush go-hipchat room_id 'Message to be sent…'

  Send e-mail:

    $ drush gomail \
        --body="message content" \
        --subject="Subject for this mail" \
        --to="to@mail.com" \
        --from="mailfrom@mail.com"

4. Back ported some Components from Drupal 8

  - \Drupal\Component\Uuid
  - \Drupal\Core\KeyValueStore

5. Simple Google Analytics integration

  In settings.php configure your Google Analytics code by adding this line:

    define('GO_GOOGLE_ANALYTICS', 'UA-****');

6. Simple 403/404 handler:

  - On 403, redirect user to login page.
  - On 404, redirect user to search page.

  To enable this feature. Just go to your settings.php add these lines:

    define('GO_403', 1);
    define('GO_404', 1);
    // or
    define('GO_404', 'site-content');

7. Created new golive command

   Used command: drush golive --cache=1 --js=1 --update=1
   Please run command drush golive --help for help

8. /node => /<front>

  Drupal site usually does not have designed for /node page, this look ugly.
  Instead render this unwanted page, go.module redirect user to front page.

  To disable this feature, in your settings.php, add this line:

    define('GO_SKIP_NODE_TO_FRONT', 1);

9. Better format similar feature in simple implementation.

  Read sample configure in go.config.sample.php

10. No current password

  Drupal 7 added a new feature: If a user changes their email or password, they
  are required to enter their current password. (see the 5+ year old
  issue: http://drupal.org/node/86711)

  However, the implementation causes problems for certain edge cases. (For
  example, http://drupal.org/node/889772) Or, you may just not like the design
  decision. Either way, this module makes it optional.

  To disable this default bahavior, in settings.php add this line:

    define('GO_NO_CURRENT_PASSWORD', TRUE);

11. Simple slider/slideshow

  People usually using views + views slideshow + views slideshow cycle (even more)
  modules to just render the modules. But with help many jquery libraries out
  there, rendering a slideshow is not hard like that. Read more at go.config.sample.php

12. Provide linnks to run specific cron job at /admin/config/system/cron.
    There's also drush for this:
      drush go-cron %module

13. drush go-require

  Drush make is the best way to make our Drupal code base. But if we would like
  make our code base on an existing Drupal core, Drush make can not help, that's
  why we wrote this command.

  On your dev site, use drush make generate to generate the .go_require file

    $ # change directory to your Drupal dir, where you place settings.php
    $ cd /path/to/your-drupal/
    $ drush make-generate > .go_require
    $ git add .go_require
    $ git commit .go_require -m "Define make file for go-require command"

  On your production site, if .go_required are already there, just run simple
  command:

    $ drush go-require

No more needed modules:

  - login_redirect
  - google_analytics
  - better format
  - nocurrent_pass

Comming features:

  - Simple spam protection, similar to hidden captcha, but more powerful.
  - lazy_routing, in Drupal you have to define menu_item for each route, this
    comming feature will help you to do routing in Drupal just faster.
  - node_edit_protection — A void user to exit editing node by accident.
  - hipchat action for Rules.module
  - drush go-require — Similar to drush make but it just downloads the missing
    dependencies, no make the full Drupal code base.
  - Backport KeyValue classes of Drupal 8.
  - Integrate http://phpdebugbar.com/
  - Better Cache Flushing - It's readlly hard to flush cache on high Drupal traffic
    websites, this feature provide something new…
  - go_content_group() — which solve this issue:
    — Quicktabs module provide API to render contents as tabs. It does not
      support render div/accordion/fieldset/slideshow/…
    - Field Group module can render contents in to groups in div/accordion/
      fieldset/slideshow/… but it is tied to Form, no API for us to render the
      normal content.
