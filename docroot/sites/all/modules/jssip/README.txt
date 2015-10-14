Unpack the libraries API into your Drupal 7 modules directory

Unpack this jssip module into your Drupal 7 modules directory

Relative to the Drupal home,
  mkdir -p sites/all/libraries/jssip

Download JsSIP.min.js from http://jssip.net/download into
  sites/all/libraries/jssip

Create the VERSION file:

echo "0.3.7" > sites/all/libraries/jssip/VERSION

Now log in to Drupal, go to the Modules administration page and
enable the modules:

  * libraries
  * jssip

Finally, install and enable any other modules that want to use JsSIP
