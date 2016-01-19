
Unpack the libraries API into your Drupal 7 modules directory

Unpack this arbiterjs module into your Drupal 7 modules directory

Relative to the Drupal home,
  mkdir -p sites/all/libraries/arbiterjs

Download Arbiter.js from http://arbiterjs.com/ into
  sites/all/libraries/arbiterjs

Create the VERSION file:

echo "1.0" > sites/all/libraries/arbiterjs/VERSION

Now log in to Drupal, go to the Modules administration page and
enable the modules:

  * libraries
  * arbiterjs

Finally, install and enable any other modules that want to use ArbiterJS

