Introduction
============
The social aggregator API provides a framework for normalising the rendered 
output of social media posts. 

The module provides three submodules for integrating facebook, twitter and 
instagram. Each of these 3 submodules have their own dependencies but will 
allow you to use the atoms on any content type as a social media field. 

The associated functions can be used in custom modules to use the social
atoms in things such as blocks. The module provides a core field type 
implementations where you can attach these fields to any fieldable entity.

It is up to you to theme them how you would like. A tpl is supplied for the 
basic structure.

INSTALLATION
----------------------------
REGULAR
download and install
social_media_aggregator
social_media_aggregator_facebook requires facebook_pull
social_media_aggregator_instagram requires drupagram
social_media_aggregator_twitter requires twitter_pull, twitter

http://www.drupal.org/project/facebook_pull
http://www.drupal.org/project/drupagram
http://www.drupal.org/project/twitter_pull
http://www.drupal.org/project/twitter

enable

DRUSH
drush en social_media_aggregator social_media_aggregator_facebook 
social_media_aggregator_instagram social_media_aggregator_twitter -y
drush cc all

NB:
- update twitter to latest dev version

CONFIGURATION:
----------------------------

ACCOUNT SETTINGS
CONFIGURATION > WEB SERVICES

FACEBOOK PULL
admin/config/services/facebook-pull

Steps
1 - Go to https://developers.facebook.com (in a new tab)
2 - Login with page admin account
3 - Menu > Apps > Register as Developer
4 - Menu > Apps > Create a New App
5 - Refresh the Captcha 10 times
6 - Turn of All Social Blockers like Ghostery
7 - Dashboard > Copy App ID & App Secret to Facebook pull settings
8 - Go to http://findmyfacebookid.com and enter page URL
9 - Copy Numeric ID to Facebook pull “Graph ID”
10 - Enter object type to be pulled in. Select from link, status, photo, video 
(for a full list: https://developers.facebook.com/docs/graph-api/reference/v2.0)
11 - Enter the number of items to fetch
12 - Save

Feed:

1 -

///////////////

TWITTER
admin/config/services/twitter

Steps
1 - Go to admin/config/services/twitter/settings
2 - Go to https://dev.twitter.com/apps/new (in a new tab)
3 - Login
4 - Complete form, OAuth redirect_uri can be your domain name 
(don’t forget http://)
5 - Agree
6 - Click “Create your Twitter Application”
7 - Click Test OAuth
8 - Copy Consumer key to OAuth Consumer key at 
admin/config/services/twitter/settings
9 - Copy Consumer secret to OAuth Consumer secret at 
admin/config/services/twitter/settings
10 - Save configuration
11 - Go to admin/config/services/twitter
12 - Click “Go to Twitter to add an authenticated account”
13 - Click Authorize App (make sure you’re logged into the correct 
Twitter account).

Feed:

1 - A feed is added when you add your default account.
2 - Additional non-authenticated accounts can be added.

///////////////

INSTAGRAM (DRUPAGRAM)
admin/config/services/drupagram

Steps
1 - Go to http://instagram.com/developer/register/ (in a new tab)
2 - Login
3 - Complete the form and hit “sign up”
4 - Click “Register your application”
5 - Click “Register a New Client”
6 - Complete form, Reditect URI is provided in: admin/config/services/drupagram
as “Callback URL”. For a testing server the website and redirect uri 
should be the uri provided here.
7 - Copy and Paste the info into the relevant fields at 
admin/config/services/drupagram
8 - Save configuration

Feed:
1 - Go to user/1/edit/drupagram (to setup main instagram feed)
2 - Click Add account
3 - Enter credentials and save
