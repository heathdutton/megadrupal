CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Configuration


INTRODUCTION
------------

Current Maintainer: Jyotisankar Pradhan <jyotisankar.pradhan@gmail.com>

After registration if you want your user to pay via stripe payment 
gateway this is the suitable module.

php5-curl should be enabled in your server. For ubuntu you can use
"sudo apt-get install php5-curl" to install it on server.
After installation you need to restart apache server.
To restart you can use "sudo service apache2 restart"

To create the stripe API keys(Secret Key and Publishable key) you can refer
https://stripe.com/docs/tutorials/dashboard
To access the link first you need to create an account in stripe.

INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

Download the code zip from git-hub URL: https://github.com/seanvoss/striper
Extract your code and navigate to striper-master/lib/stripe-php/
Now create directory called stripe inside sites/all/libraries. Copy the
contents of lib to stripe folder that you have created in sites/all/libraries.

Now your folder structure must look like sites/all/libraries/stripe/{contents
of lib folder}.

CONFIGURATION
-------------
Site admins can set the Secret Key, Publishable Key and
Registration Amount via admin panel
For adding Secret Key, Publishable Key and Registration Amount navigate to 
<yourdomain.com>/admin/register_user_with_stripe_payment_config
 
Site admins can view the list of payment made by the users.
To see the list navigate to the url given below
<yourdomain.com>/admin/register_user_with_stripe_payment/paid_users_with_stripe

In the back end go to <yourdomain.com>/admin/config/people/accounts,
Now in "Who can register accounts?" section check "Visitors" radio button and
un check "Require e-mail verification when a visitor creates an account" 
