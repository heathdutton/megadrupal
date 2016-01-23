Japanese Mobile Theme
---------------------

Introduction
--------------------
Japanese Mobile is a theme aimed at making a functional Drupal site for Japanese mobile phones(DoCoMo, au, Softbank).  Since Drupal doesn't work with Japanese mobile phones by default, the theme uses a number of overrides in order to make Drupal compatible with them.

Features
--------------------
-Use of user agent detection, which dynamically loads different headers depending on a carrier.
-Inline CSS since one of the carriers allow neither external CSS sheet nor header CSS.
-No javascript because it doesn't work on Japanese mobile phones.
-Character encoding conversion of HTML output fom UTF-8 to Shift_JIS,
since some mobile phones cannot display UTF-8
-Character encoding conversion of user input ($_POST) from Shift_JIS to
UTF-8 suitable for internal processing by Drupal.
-Session handling by URL-rewriting instead of cookies, since one of the
carriers doesn't allow use of cookies.
-As a standard practice for a mobile website, blocks are positioned
above and below main content, instead of left and right.
-Liquid layout for various screen sizes.
-Since some mobile phones cannot display CSS border property, care has
been taken to ensure different regions are distinguishable from one
another, by use of different color for each region and use of <hr />
tags in some cases.
-Use of GIF image instead of PNG because Japanese mobile phones cannot
display PNG.

Installation
-----------------
I. Modify settings.php
Since one of Japanese carriers(DoCoMo) cannot handle cookies, it is necessary to use url rewriting for session handling.
Change settings.php as follows:

ini_set('session.use_cookies', 0);
ini_set('session.use_only_cookies', 0);
ini_set('session.use_trans_sid',    1);
ini_set('url_rewriter.tags',        '');

The following php settings are optional but recommended.
ini_set('session.cache_expire',     3600);
ini_set('session.cache_limiter',    'none');
ini_set('session.cookie_lifetime',  3600);
ini_set('session.gc_maxlifetime',   3600);
ini_set('session.gc_probability',   20);

Define variable override as follows.

$conf = array(
  'session_inc' => './sites/all/themes/jp_mobile/session.inc'
);


II. Install jp_mobile_helper module
Copy the folder named jp_mobile_helper to 'sites/all/modules' and enable the jp_mobile_helper module.  The module converts $_POST encoding from Shift JIS to UTF-8 for internal processing by Drupal and rewrites urls to include a session ID.
