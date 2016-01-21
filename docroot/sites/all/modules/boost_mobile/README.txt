
BOOST MOBILE MODULE FOR DRUPAL 7.x
---------------------------

CONTENTS OF THIS README
-----------------------

   * Description
   * Requirements
   * Installation

DESCRIPTION
-----------

Basically the Boost Mobile module adds mobile support to the Boost module.

The Boost module provides static page caching for Drupal. It is designed to 
serve cached pages per domain.
So what if you want two versions of your website (let's say a desktop and mobile
 version) on the same domain name but still cached with boost ?

This is what the Boost mobile module is for. All Drupal users who already use 
the Boost module and want to have a mobile version of their website on the same
domain name will be happy with this module.

Users will just have to choose a different Drupal theme for the mobile version.

Technically, we use the same logic as boost and generate some Apache rewrite 
rules to paste in .htaccess file. It's where we do all of the detection of user 
agent, cookies management and check of $_GET parameters.

Practically you just have to choose a Drupal theme to be associated to the 
mobile version of the site.

You also have a switcher block to switch between desktop and mobile version.
To use it, Just add this block in the region you want in desktop/mobile theme.


REQUIREMENTS
------------

Boost module must be installed and working properly.


INSTALLATION
------------

The installation steps are easy:

1) Download and enable the module like any other modules.

2) In the settings.php file, you must add the two following lines:
$conf['boost_normal_dir'] = $_SERVER['boostpath'];
$conf['boost_gzip_dir'] = $_SERVER['boostpath'];

3) Check the settings in admin/config/system/boost/mobile. This is also where
you choose which theme is the mobile theme.

4) Copy paste the new .htaccess code generated in
admin/config/system/boost/mobile/htaccess to the .htaccess file of your Drupal
installation.
It must replace the original boost rules.
