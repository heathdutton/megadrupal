<?php

/**
 * @file
 * English Account activated template.
 *
 * @ingroup ces_message_template
 */

$subject = '[[exchange:shortname]] Account activated';

$body = 'Dear [account:user:fullname],

Welcome to [exchange:name] ([exchange:code]), your local exchange of the Integral Community Exchange System platform.

Below are the details you need to access your account on the [exchange:code] network.

To get to the web site go to [site:url]. This will take you to the login page.

Enter the following information:

Username: [account:user:name]
Password: *

* Your password. If you do not already have one, you will receive another email with a link to generate it.

The web interface is available in several languages. If you wish to select another language, click on the your name at the top right corner. In your personal profile select the language of your preference. Be sure also that your personal details are correct and complete.

After looking around please enter your \'offers\' and \'wants\' by clicking on the My offers and My wants buttons at the left menu. If you provided offers with your application you may add as many additional offerings as you like.

You can also upload a photograph of yourself so that other users can see what you look like.

Feel free to contact any advertiser in the \'Offers\' list to purchase any goods and services offered. Please don\'t wait for someone to buy from you before you use the system. You don\'t need to be in credit before making a purchase; going into debit is part of the system and simply represents your commitment to the community.

If you require any assistance please contact me. I will be glad to help.

Happy trading!

--
[exchange:shortname] administrator:
[exchange:admin:fullname]
[exchange:admin:mail]
[exchange:admin:mainphone]
';
