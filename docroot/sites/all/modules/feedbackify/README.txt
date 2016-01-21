Feedbackify
-----------------
This module integrates the Feedbackify service with Drupal. You need a Feedbackify account in order to use this module.

Installation
---------------
1. Sign up for a Feedbackify account (http://www.feedbackify.com)
2. Grab the form ID from your feedback form (can get this from the URL at http://www.feedbackify.com/app/edit/[form ID] or http://www.feedbackify.com/app/deploy/[form ID])
3. Enable this module
4. Go into Admin > Settings > Feedbackify and paste your copied form ID in the required box

If you don't like the default hover and active state colours on the Feedbackify form you can always override them like this:
/**
 * Feedbackify
 */
div#feedbackify .select-on td a, div#feedbackify .select-on td a:hover, div#feedbackify .select td a:hover, div#feedbackify .txt1 {
  color: #e61166 !important;
}

div#feedbackify .select-on {
  background-color: #e61166 !important;
}

div#feedbackify .cat-arrow img {
  display: none;
}

Sponsors
--------------
Initially this module was kindly sponsored by NoJoShmo.com and is maintained by Ivan Breet (ivanbreet). This module is based on the Get Satisfaction module.

* Please note that Feedbackify is a paid service, but they do offer a 15 Day trial.