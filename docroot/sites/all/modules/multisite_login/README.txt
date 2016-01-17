
** OVERVIEW **

Multisite Login allows a user to login to all sites in a multisite configuration, even though the
multisites do not share domain names.  

For example:
  politicker.com
  politickernj.com
  politickerny.com
  etc.

NOTE: if you are using several sites on the same domain (ex. politicker.com, nj.politicker.com, 
ny.politicker.com, etc.), then you do not need this module.  The multisite functionality in Drupal
core has the ability to share logins for sites like these without additional modules. 

This module was designed to be an alternative to the singlesignon module
but using a technique that does not block search engines from accessing your websites.



** REQUIREMENTS **

You must be:
  - using a multisite installation
  - using a separate database for each site (no table prefixing, though the code could be 
    modified to allow for this, patches welcome). 
  - sharing the users table across all sites (If this is not you, what are you hoping that this module
    will do for you?)



** INSTALLATION **

1)  Install and setup multisite_api before using this module.

2)  Adjust the settings in multisite_api to control which sites are logged in to.  Multisite_login
    will login to all sites that match the current site's visibility and server. 

3)  Install and enable this module and multisite_api on all of your sites.

4)  Make sure that you are sharing the user and multisite_login tables on all of your sites.  The
    $db_prefix in the settings.php files of your secondary sites will look something like this:

      Master website

      $databases = array (
      'default' => 
      array (
        'default' => 
        array (
          'driver' => 'mysql',
          'database' => 'master_db',
          'username' => 'root',
          'password' => 'root',
          'host' => 'localhost',
          'port' => '',
          'prefix' => array(
               'default'   => '',
               'users'     => 'master_db.',
               'multisite_login'  => 'master_db.',
             ),
            ),
          ),
        );

      other website

      $databases = array (
      'default' => 
      array (
        'default' => 
        array (
          'driver' => 'mysql',
          'database' => 'other_database',
          'username' => 'root',
          'password' => 'root',
          'host' => 'localhost',
          'port' => '',
          'prefix' => array(
               'default'   => '',
               'users'     => 'master_db.',
               'multisite_login'  => 'master_db.',
             ),
            ),
          ),
        );

    Note that if you are sharing the sessions table between your sites, then you should also share
    the multisite_login_sessions table.
    
    Note drupal_hash_salt must be same for all websites. Copy form master website hash_salt to other websites.

4)  Login to one of the sites

5)  If you are an administrator, you will see a string of single-pixel images at the bottom of the
    page, one for each partner site (If you are not an admin, the images are hidden via CSS techniques). A black image indicates a successfull login, red means failure.  Check admin/logs/watchdog on the partner site to see why a login failed.



** HAVING PROBLEMS? **

1)  I get "user/pass does not match" errors.

    Make sure you are sharing the multisite_login table, Review the installation steps above.

2)  I get "The host does not match" errors.

    In rare circumstances, usually only in a development environment, IPs from the same browser
    session may not in fact match.  Go to admin/multisite/multisite_login and disable host matching.

3)  I just copied my database from staging to production and now I can't get one or more sites to
    work.

    Clear the multisite_api cache on those sites by visiting:
    admin/multisite/multisite_api
    And saving the form.

4)  It just doesn't work.  My sites are in a subdirectory of the main domain, or are accessed on a
    special port.

    When Multisite Login creates the URLs that point to the images on the other sites it has to do a
    bit of guesswork.  Normally it will use the name of site directory as the domain of the other
    site. However this doesn't work if the other site is in a sub-directory, or if the other site
    uses a non-standard port.  In these cases you must set $base_url in your settings.php files, and then go to the multisite api settings page and save the form (this clears the cache).

5)  It works in some browsers / for some of  my users, but not others.

    Make sure that the browser is not blocking 3rd party cookies.



** TECHNIQUE **

When a user logs into a site, the returned html will include several image tags, one for each partner
site. The url of the image will include a query string which provides enough information to login the
user on the partner site. A cookie belonging to the partner site is then stored on the user's browser.
Partner sites return 1x1 images.

* Originating Site *

On hook_user $op == login
- query each partner site via image tag
- img src has a query string that includes: double-munged password, uid, and the session id encoded
  in a reversable hash

* Partner Site *

Via a menu callback
- Check if uid/password match
- Check if IP matches what is stored in the session table for the given session id
- Check that the timestamp is recent
- Check that this sid hasn't been tried before
- log the user in
- store the sid so it can't be used again for a multisite login
- return image
- exit(0);

* Considerations *
We are passing a double MD5ed password, though not theoretically impossible to reverse engineer,
if someone really wants to break into your site they would use a method that requires less work.
If the query string _were_ to be somehow obtained by a third party, it could not be used by the
third party to masquerade as the original user since logins are only allowed once per session id.



** LOGGING OUT **

Multisite Login does not synchronize logging out accross sites. Consider this scenario:  A user 
logs in to your network of sites.  Chances are high that they will spend most of their time on just 
one site, they might not even vist other sites in the network.  Depending on how you have PHP 
configured, their session will expire on those other sites and they will be automatically logged 
out, even though they remain logged in on the one site.  

Multiste Login cannot control this situation and so it doesn't make any attempt to do anything when
a user explicitly logs out.

However in theory, if uids were stored in {multisite_login_sessions}, a combination of
hook_user $op == 'logout' and hook_cron could be used to log out the user on all sites, patches
welcome.

It is also because of this reason that a user who vists several sites in your network, but at 
inconsistent frequencies may find that they have to re-login to some sites.  



** CREDITS **

This module was developed by

Lalit kumar
lalit774@gmail.com