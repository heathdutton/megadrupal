## Overview ##
All state of New York websites are required to have the state Universal
 Navigation bar at the top and bottom of the site, surrounding any other
 content.  The content of the bars are iFrames, this module makes it
 easy to integrate them on a Drupal site.

## Installation and Configuration ##
- Install as usual, see [http://drupal.org/node/70151](http://drupal.org/node/70151)
  for further information.
- Enable the module.
- Go to Configuration >> User Interface >> NYS Universal Navigation
  (/admin/config/user-interface/nys-unav) to configure the module.
  You can also reach the configuration page from the Configure link on the
  module page.
  - The configuration page has two options:
		- Whether to automatically insert the header/footer
		- Whether to use the interactive or static uNav header
  - *Note that if you don't configure the module, it will default to
  automatic insertion and the interactive version of the header.*
- Set the *Administer NYS Universal Navigation* permission for those roles that
  should be able to administer the configuration of this module at People >>
  Permissions (/admin/people/permissions#module-nys_unav).
  You can also reach the permission from the Permissions link on the
  module page.
- The module will automatically insert the Universal Navigation at the top
  and the Universal footer at the bottom of your website's page;
  outside of any page HTML.


Alternatively the module adds two blocks, NYS uNav Header and NYS uNav Footer,
 which can be placed in your theme and it exposes two functions which can be
 used in theme templates.

## Restrictions ##
This Drupal module was developed for use by New York State agencies and
 entities for official New York State websites to be compliant per ITS mandate
 policy NYS-S05-001 ([https://www.its.ny.gov/document/state-common-web-banner](https://www.its.ny.gov/document/state-common-web-banner)).
 For use on other sites, please contact New York State Office of Information
 Services WebNY team at webnysupport@its.ny.gov for guidance and authorization
 for use. The static html `NY State Universal Navigation`, that is integrated
 using this module, can be found at [https://github.com/ny/universal-navigation](https://github.com/ny/universal-navigation).

## Credits ##
This project was sponsored by the [New York State Office of Information
 Technology Services WebNY department](https://www.drupal.org/webny-new-york-state-office-of-information-technology-services).

## Use ##
Enabling the module on your site will, by default, insert the Universal
 Navigation at the top and the Universal footer at the bottom of your website's
  page; outside of any page HTML.

The Universal Navigation will **not** display on any site administration
 pages, for most sites administration pages use the Seven or other contributed
 administration theme.

For other applications, the module adds two blocks and defines two functions,
 either can be used depending on your theme.

Ideally if you enable the module after your other modules it will adapt to
 most situations.

### Non-automatic option ###

If you find the automatic option doesn't work on your particular site, or you
 want to use the additional flexibility offered via either blocks you can
 position in your theme or functions you can use in a template, you can
 disable the automatic addition of the header and footer via an addition
 to your site's settings.php file.  In that file, about line 330, is a block
 of comments that starts with "Variable overrides:".  After the block comment,
 there are several lines that are commented out with a # character of the
 form `$conf['site_name'] = 'My Drupal site';`.  After the commented out lines,
 add the line:

    $conf['nys_unav_auto'] = FALSE;

A gentle reminder -- make sure you have the semicolon (;) at the end of the
 line, otherwise your site might not load.

Another use for this feature would be to disable the Universal Navigation on
 your development environment by putting the setting above in your
 local.settings.php file or other environment specific configuration area
 (some sites keep that configuration in the settings.php inside conditional
 statements).

Once you have disabled the automatic addition of the Universal Navigation,
 you have two options:  use blocks in your theme or functions in your theme
 templates.

### Blocks ###

If your theme has header and footer regions (they aren't necessarily named
 that way) that are full page width, and don't have anything above
 them (header) or below them (footer), using the module is as simple as
 enabling it and using your block placement method
 (the structure >> blocks page in Drupal or using context, panels, etc if
 you are using one of those contributed modules.).

If your theme doesn't have appropriate regions, you could add new regions
 in your theme, using the information
 at [https://www.drupal.org/node/171224](https://www.drupal.org/node/171224)
 or following the tutorial
 at [https://www.ostraining.com/blog/drupal/block-region-drupal-theme/](https://www.ostraining.com/blog/drupal/block-region-drupal-theme/),
 then place the blocks as described.  Adding new regions will requiring
 modifying your page.tpl.php file, if you also have custom page templates
 (e.g. page--front.tpl.php, etc), remember to modify them also.

### Templates ###

Alternatively, you can modify your theme template(s) to insert the uNav HTML
 during output. The easiest template to modify is the core system html.tlp.php
 file which is used to wrap the page with the html doctype, output the styles
 and scripts and add the accessibility skip link.

You can make a single change to that template.  Remember **Don't hack core**.
  If your theme, or base theme if you have created a subtheme, doesn't have
 the html.tpl.php file in it's template directory, you can copy the default
 Drupal html.tpl.php from the core /modules/system directory into your theme's
 template directory (if your theme doesn't have a template directory, you can
 create one at the same level as the theme .info file).  *Remember to clear
 caches after copying the template file to cause Drupal to rebuild the theme
 registry and "discover" the new file!*

Then, in the theme html.tpl.php file add the following code right after the
 block comment documenting the variables and before the HTML that starts
 <!DOCTYPE....

    <?php
    if (module_exists('nys_unav')) {
      $page_top = nys_unav_header() . $page_top;
      $page_bottom = $page_bottom . nys_unav_footer();
    }
    ?>

Since this code is inside a template, remember to use both the <?php and ?>
 closing tags.  The order of the variables in the assignment statements are
 important.  The first ensures the uNav header is before any module generated
 HTML, the second ensures the uNav foot is after any module generated HTML.

## Restrictions ##
This Drupal module was developed for use by New York State agencies and entities
  for official New York State websites to be compliant per ITS mandate policy
  NYS-S05-001 ([https://www.its.ny.gov/document/state-common-web-banner](https://www.its.ny.gov/document/state-common-web-banner)).
  For use on other sites, please contact New York State Office of Information
  Services WebNY team at webnysupport@its.ny.gov for guidance and authorization
  for use. The CSS, JavaScript and fonts that make up the library used for this
  module can be found at [https://github.com/ny/global-navigation](https://github.com/ny/global-navigation).
  Because that repository contains fonts that are licensed only for use on
  New York State websites, it is not public.  If you need access to the
  repository, contact the WebNY team.

## Updating uNav Embed Code ##
The uNav embed code(s) are contained in the two template files
 nys-unav-header.tpl.php and nys-unav-footer.tpl.php.  Should the State of
 New York require changes to the embed code, update the module to the latest
 version or edit those template files.

## Adapting This Module for Non-NYS Sites ##
Since the uNav embed codes are simple HTML iFrames with some in-line Javascript,
 this module could be used on any site to insert an HTML header and/or footer.
 One possible use might be to insert a banner/footer that is enabled or
 disabled by enabling/disabling the module.  Simply copy the same two template
 files, nys-unav-header.tpl.php and nys-unav-footer.tpl.php, into your custom
 theme and replace the contents of the template(s) with your own
 HTML, in-line Javascript and/or in-line CSS.  If you don't need either the
 header or footer, simply have an empty template file.