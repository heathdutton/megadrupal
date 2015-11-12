The module allows simple redirection based on Country (Geo Ip).

The module uses the Telize API from http://www.telize.com/.

The redirection is handled on the front-end, rather than on the backend. 
So you don't have to worry about caching or breaking your Varnish. 
Works with boost as well.

How to configure:
1) Navigate to admin/config/system/telize-redirect
2) Enter the redirection rules in the format of 
[<strong>2-letter-country-code</strong>]|[Redirection-Url]
3) That's it! 

Configuration Screen: https://www.drupal.org/files/telizeconfig.jpg

For the 2 letter country codes, refer to 
http://en.wikipedia.org/wiki/ISO_3166-1_alpha-2

Beware
You might lock yourself out if you add your country rule to redirection. 
The module tries its best to prevent it. Ex: Does not redirect if the user is 
already logged in. If you still happen to lock yourself out, you can still get 
back by various techniques like turning off JS on your browser, or 
disabling the module from command line etc.