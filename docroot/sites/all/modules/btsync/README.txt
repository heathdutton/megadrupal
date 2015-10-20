The BitTorrent Sync API module allows you to communicate with a BitTorrent Sync
API enabled server, allowing you to manage syncs, and much, much more.

The BitTorrent Sync API is currently in beta, more details at
http://www.bittorrent.com/sync/developers

BitTorrent Sync API was written by Stuart Clark and is maintained by
Stuart Clark (deciphered) and Brian Gilbert (realityloop) of Realityloop Pty Ltd




Features
--------------------------------------------------------------------------------

- Full support for the BitTorrent Sync API as defined at
  http://www.bittorrent.com/sync/developers/api
- Rules actions and data for all methods.



How to use
--------------------------------------------------------------------------------

1. Install/Enable as per https://drupal.org/node/895232

2. Configure the API at "admin/config/services/btsync".

3. Use one of the following methods:


   - Rules:

     All BitTorrent Sync API methods are available as Rules actions and provide
     the relevant data as defined in the BitTorrent Sync API documentation.



   - Direct module integration:

     The "btsync_method_callback()" can be used directly in your module to call
     all available methods. The function takes two arguments:

     - $method; The name of the method, as per the BitTorrent Sync API
       documentation and the filename (excluding '.inc') in this modules
       "methods" directory.

     - $data; A keyed array of the required arguments required for the chosen
       method.


     An example, creating a new sync folder, can be seen below:


        function mymodule_myfunction() {
          btsync_method_callback('add_folder', array(
            'dir' => 'public://mydirectory',
          ));
        }



Recommended modules
--------------------------------------------------------------------------------

* Rules                - https://drupal.org/project/rules
* File Rules (sandbox) - https://drupal.org/sandbox/deciphered/2116397
