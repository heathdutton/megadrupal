Please read this file. Contain answers to many common questions.
If you are upgrading, check the CHANGELOG.txt for major changes.

**Description:
Pay with a tweet implements tweet payment feature. This allows the website to
make downloads available to users with a tweet as payment.

Pay with a tweet provides a token and unlimited blocks to show the downloads.

**Benefits:
Your visitors will get content without having to pay money and the website 
benefit from this viral marketing system based on the social network Twitter

**Installation AND Upgrades:
1. Install the oAuth module (https://drupal.org/project/oauth)
2. Unpack the pay_with_a_tweet folder and contents in the appropriate modules
directory of your Drupal installation.  This is probably sites/all/modules/
3. Enable the pay_with_a_tweet module in the administration tools.
4. If you're not using Drupal's default administrative account, make
sure "administer pay_with_a_tweet" is enabled through access control 
administration.
5. Visit the pay_with_a_tweet settings page and make appropriate configurations


**Blocks

You can configure that each button has a block.

You can configure the number of extra blocks you can use. In each extra block
can indicate which button to use. So you can have the same download in
different blocks areas.

**Token
Pay with a Tweet has a token [pay_with_a_tweet:pid:?]. This way you can add
buttons anywhere on your website. For inclusion in text areas without token
support you must to allow the use of PHP Filter and include the token follows:

<?php print (token_replace('[pay_with_a_tweet:pid:?]')); ?>

Note: You must replace "?" for the pay_with_a_tweet entity pid

**Credits:
The original module is writen by Rolando Caldas Sanchez http://rolandocaldas.com
(rolando.caldas@gmail.com) and use two third-party classes:

OAuth class (oAuth module: https://drupal.org/project/oauth)

twitteroath class by Abraham Williams (abraham@abrah.am) http://abrah.am

Current maintainers: 
  Rolando Caldas Sánchez - http://rolandocaldas.com

**Changes:
See the CHANGELOG.txt file.
