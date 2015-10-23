##Summary
A Drupal module to provide the jstorage script as a library.  Using this module means you do not have to include the jstorage file more than once in any Drupal installation. See <https://drupal.org/node/1342220> for more info on libraries.


##Installation
1. Download and unzip this module into your modules directory.
1. Download jStorage <http://www.jstorage.info/> to `sites/all/libraries/jstorage` so that the js file is located here: `sites/all/libraries/jstorage/jstorage.min.js`
2. Goto Administer > Site Building > Modules and enable this module.
2. Goto Reports > Status reports and look for the line that starts with jStorage to make sure the library is detected correctly.


##Usage
When you want to use jstorage on a page you must include this line of code:

    libraries_load('jstorage');

For info on how to implement jstorage refer to <http://www.jstorage.info/#usage>.

##Contact
* **In the Loft Studios**
* Aaron Klump - Developer
* PO Box 29294 Bellingham, WA 98228-1294
* _aim_: theloft101
* _skype_: intheloftstudios
* _d.o_: aklump
* <http://www.InTheLoftStudios.com>
