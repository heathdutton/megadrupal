INTRODUCTION
------------
The PeerJS Chat module adds the ability to chat using webRTC peer connection
and the PeerJS javascript library.

REQUIREMENTS
------------
 * Chrome 26+, Firefox 23+
 * PeerJS - https://github.com/nttcom/peerjs
 * PeerServer - https://github.com/peers/peerjs-server

INSTALLATION
------------
1. Install a peer-server instance. Please see the installation directions at
   https://github.com/peers/peerjs-server. The easiest way to get an instance
   up and running is to use the Heroku link on the peerjs-server github page.

2. Install angularjs, views, and restws, and peerjs module as per:
   https://drupal.org/documentation/install/modules-themes/modules-7

3. Install the peer javascript library. The easiest way to do this is to use
   the drush command: 'drush pdl'. To install manually, download the library
   from https://github.com/nttcom/peerjs and install into
   sites/<site>/libraries/peerjs. The peer.min.js file should reside at
   sites/<site>/libraries/peerjs/dist/peer.min.js.

4. Configure angularjs. It's recommended to use the latest 1.3.x or 1.4.x branch.

5. Configure peerjs using the server address of the peer-server that was set up
   in step 1.

CREDITS
-------
Current maintainers are:
 * Norman Kerr (kenianbei) - https://drupal.org/user/778980

This project has been sponsored by:
 * Yamada Language Center - https://babel.uoregon.edu
