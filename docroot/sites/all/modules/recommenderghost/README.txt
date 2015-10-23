ABOUT
**************************************
This module provides integration with the RecommenderGhost service.

RecommenderGhost is a free hosted Recommender System that makes integration into 
websites as simple as integrating Google Analytics. There is no need to setup, 
run and maintain a Java based server - all complex computations are done on the 
infrastructure of RecommenderGhost. While tools for recommendations running on 
your own server, like Recommender API, are great, they lack the sophistication 
and ease of use of hosted recommendation system providers like RecommenderGhost.

RecommenderGhost provides the following personalization services:

* unpersonalized recommendations of the form "other users also bought/viewed..."
* personalized recommendation depending on individual preferences
* rankings such as "most bought items", "most viewed..."
* manual clustering of the items (e.g. XMAS SALE 2012)
* using item types (e.g. BOOK, DVD, MP3) for in type-specific recommendations


This package contains 2 modules:

* RecommenderGhost - Core: Tracking page views and providing 2 ready-to-use 
                           blocks for recommendations like "Users also viewed.."
* RecommenderGhost - Commerce: Tracking purchases and providing 2 ready-to-use 
                           blocks for recommendations like "Users also bought.."

This module will be extended by Rating/Voting recommendations in the future


HOW TO SETUP
**************************************

see this tutorial online with screenshots
http://www.recommenderghost.com/getting-started-drupal-module

Step 1) Create account (it's free)
If you have not yet an account with http://www.recommenderghost.com you can 
create one now. Basic registration is free (and will stay free) and no credit 
card or personal information other than your email address is required.
After you have filled-in the form and clicked on "Create new account" you will 
receive an email with a link to confirm your registration. Only after you have 
clicked on the link in the email (or copied and pasted it in your browser) will 
you be able to fully make use of the services. This is done to avoid spam and 
misuse.

Step 2) Register website in RecommenderGhost
As soon as you have confirmed your email address you can create new websites 
that should be tracked. On the RecommenderGhost website click "My websites" - 
"Create a new website". You are asked to enter a short name (eg "my shop"), 
a description and the URL of the website to track (this should be the URL you 
created you use your Drupal installation in). Confirm by clicking "Save".
When you click on "How to integrate your tracking code" you will see an API key 
and a website ID. Copy them as you will need both of them for the next step.

Step 3) Configure tracking
Great - you are now ready to track your website. Only a little bit of 
configuration is needed. Go back to your Drupal site and go to the 
RecommenderGhost administration section (Administration » System » 
RecommenderGhost). Now, enter your API key and Website ID. Enter the token 
representing any images of your nodes into the field "Pattern for the image". 
For Drupal Commerce Kickstart this would be: 
[node:field-product:1:field_images] (including brackets).

That's it. Your RecommenderGhost module is now ready to use. Click save.

Step 4) Browse your site/shop and validate configuration
To validate your configuration, visit your new shop/site and click a few 
products/nodes. If you go to your RecommenderGhost website profile you will be 
able to see all trackings in real-time. 
Note: After your registration of the website in RecommenderGhost, you need to 
wait at least 24 hours to get recommendations. This is for 2 reasons: 
1) we make sure that we have actually some data available to calculate 
   recommendations and 
2) the calculation of recommendations happens once a day. If you need a more 
   frequent update, please contact us.

Step 5) Enable blocks for "Users also viewed / bought"
Go to block administration (Administration - Structure - Blocks). Move the 
blocks ("RecommenderGhost: Users also viewed (picture view") into the content 
area below and click "Save blocks". If you review your node/product detail page 
you will see your first product recommendations.
