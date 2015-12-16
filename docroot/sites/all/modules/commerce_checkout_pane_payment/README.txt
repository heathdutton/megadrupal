Seperate checkout step If you want a seperate 'choose payment method' in your 
checkout flow install the module Commerce Checkout Pages. This module allows 
you to create a new checkout-out step. Go to Store > Configuration > Checkout 
settings and move both Payment and Payment Method panes to this new 
checkout-out step. 

Installation 1. Enable module 2. Go to Store > Configuration > Checkout 
settings and move both Payment and Payment Method panes to Checkout page 
(thanks @essbee). 

History This module is fully derived from this issue-cue: payment method on 
order review page (http://drupal.org/node/1302232) 

Special thanks to @Forward Media and @Sixelats for contributing code and 
writing instructions. 

Known issues, feature requests Q: AngryWookie on September 5, 2012 - If 
possible it would be nice if on the payment review screen it could show 
something like "Card ending in 1234" instead of just the payment type I 
selected A: rszrama on September 25, 2012 > You can't, because that 
information is sent immediately to the payment gateway and not stored server 
side for security reasons. nielsdefeyter > This is not possible, because this 
information is not saved in the Drupal database and therefore this module 
doesn't have any reference other then the selected payment method. 

Q: robinvdvleuten on October 3, 2012 - You've written the module's description 
that the module is created because of an issue in the module's queue. Can you 
give a brief explanation why the module you've created should be a seperate 
module and not a patch for the Drupal Commerce module? A: nielsdefeyter on 
April 8, 2013 > At the end, I think the functionality of this module belongs 
there. Currently this module is an addon / helper like several others that 
extend checkout configuration options in Drupal Commerce. My suggestion would 
be to promote this module and meanwhile I post a feature request in the Drupal 
Commerce issue queue. That feature request would be broader than the 
functionality of this module. And could probably only realized in a 2.x 
version (not supersure about that) or in a combined 'commerce checkout 
extras'. 

