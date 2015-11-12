Holding page
============

The Holding page module lets you show visitors a static holding page while you work on setting the site or making changes. This gives you more flexibility than Drupal's site maintenance mode, as you can show your own HTML page or even a small site made up of a set of pages.

Installation
============
Enable the module as normal.

Setup
============
Go to admin/settings/holding to set the module up.

The settings are:
- Holding sites:
  These are the site paths that should show the holding site. These should be the same as the folder names in your /sites folder, so: 'default', 'example.com', 'subdomain.example.com'.
- Path to holding page:
  The path to the file that Drupal should return instead of your site. This is relative to Drupal root.
  You need to give the full filename, not just a path as there is no directory indexing involved.
  Examples: 'holding/index.html', 'sites/all/themes/YOURTHEME/holding/index.html'.
- Parse the holding page for tokens
  If token module is installed, you can optionally parse the contents of the holding page file for global tokens such as '[site-name]'.

Usage
============
All paths that begin with /user and /admin are allowed through to the Drupal site: this is to prevent locking yourself out.
All logged-in users are allowed through.

Examples
============
Single site:
  - Set holding to 'default'. Anonymous users who wish to see the real site can use an IP to reach it.
Multisites:
  - Set up a second domain for your site, such as dev.example.com. Make Drupal treat this the same as
    example.com by making a symbolic link to it in sites:
      $ ln -s example.com dev.example.com
     You can then set holding to 'example.com' and work at dev.example.com.

Notes
============
Any URLs in your holding pagess should be relative to Drupal root:
  <imc src="holding/image.jpg">
  <a href="holding/page2.html">

It's also possible to do this with .htaccess: see http://drupal.org/node/336370
This method has the advantage that it works even when Drupal is down.
Also it may be more suitable if your holding site itself runs on a script.
