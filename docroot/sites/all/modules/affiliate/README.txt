Affiliate module

PREREQUISITES

- (Optional) If the User Referral module is installed, affiliates can receive credit for
  referring other users.
  Project: https://drupal.org/project/referral

- (Optional) If the User Points module is installed, affiliates can acquire user points
  when someone clicks on one of their affiliate links.
  Project: https://drupal.org/project/userpoints

OVERVIEW

This module allows site owners to issue affiliate links to their partners. When an
affiliate is logged in and viewing a node, they will see the option to "display the
affiliate link" towards the bottom. When anonymous users follow these links, provided they
have been granted the proper permissions, the clicks are counted by affiliate, day, and
referrer.  When referred users sign up for an account, and if your site uses the referral
module, the referred account is associated with that affiliate.

This revision of the Affiliate module is based on a combination of the old version of the
Affiliate module and the current version of the Affiliates (plural) module, with many
improvements added in between.

INSTALLATION

- Activate the module as usual
- Grant permission(s) to each role as is appropriate for your setup

USAGE

- For a user to become an affiliate, they must have the permission
  to "opt in or out as an affiliate", at which point they can visit their account page
  and check the option to opt in.

- Affiliate links are any site path that have the following structure:

    www.example.com/affiliate/AFF_ID/OPTIONAL_DESTINATION_PATH/OPTIONAL_TRACKER_ID

  ... where AFF_ID is the affiliate's user ID, and OPTIONAL_DESTINATION_PATH is any
  eligible path, as set on the affiliate config page, with one exception. If the path
  specified is a node path, for example, node/4, or any other path with a slash in it,
  the slash needs to be replaced with a pipe character (a pipe character is a | symbol).
  If OPTIONAL_DESTINATION_PATH is not present, the person who clicked will be redirected
  to the front page and no credit will be issued to the affiliate.
  
  So, say the desired destination is node/867, and the affiliate's user ID is 5309. The
  link for them to get credit would look something like this:
  
    www.example.com/affiliate/5309/node|867

  The OPTIONAL_TRACKER_ID is currently not taken into account, but it will be soon. Right
  now it can be present or not, and should not have any effect on whether the affiliate
  receives credit.
  
  Even though the displayed URL is node|867, the user who clicked will be redirected to
  the corresponding path alias, if one exists, upon clicking.

- Payout amounts can be configured and customized for each click and/or referral an
  affiliate gets credit for. For instance, if you want each affiliate to receive a credit 
  of 0.01 for each click, and 5.00 for each referral, you can configure such on the
  affiliate payouts config page. You can also configure the way your payouts are displayed
  based on your locale. So if your locale is in France, you can set the payouts to be
  displayed with the monetary amount first, followed by a euro sign (e.g. 20 Û). Likewise,
  if your locale is in the US, you can configure the currency symbol to be displayed
  first, followed by the amount (e.g. $20).

- Payouts, clicks, and optionally referrals and user points are all updated when cron runs

- Several measures are in place that attempt to prevent an affiliate from "cheating", some
  of which include checking to make sure the user who clicked is not the user that's
  currently logged in, checking that the click came from another website (and was not
  arbitrarily entered into the a URL box of the person's browser), checking to make sure
  the same IP address has not accessed multiple affiliate links within a given period (if
  configured to do so on the affiliate admin page), etc. Keep in mind though that people
  with the right knowledge will always be able to find a way to slip through the cracks,
  so keep an eye on your logs.
  

TO DO

- Integrate tracker ID for each affiliate to be able to associate a click with a tracker

CREDITS

This module is derived from the Affiliate module for Drupal 4.5, written by Moshe Weitzman
and sponsored by Share New York, an innovative online music sharing web site -
http://sharenewyork.com. Overhaul completed by Jeremy Trojan and sponsored by the Telsev
Corporation (NSFW, so don't even bother to look). ;)

AUTHORS

Jeremy Trojan
jer at jerdiggity dawt com

Barry Jaspan
barry at jaspan dot org

Maintainer 

Thierry Guégan (thierry_gd)
