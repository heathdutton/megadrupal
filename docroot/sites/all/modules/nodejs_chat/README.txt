
CONTENTS OF THIS FILE
---------------------

 * INTRODUCTION
 * INSTALLATION
 * USAGE
 * ROADMAP


INTRODUCTION
------------

Current Maintainer: Salvador Molina <salvador.molinamoreno@codeenigma.com>

The Node JS Chat module provides a simple chat block that allows the users of a
site to talk with each other through a realtime chat that relies on the Node JS
server provided by the Node.js module <http://drupal.org/project/nodejs>.

Each Chat block placed within a page of the site gets an individual ID, and all
users visiting that page are automatically added to the chat channel. This means
that if the block is placed in, say, all the nodes for a content type "X", every
node for that content type will have its own chat channel.

INSTALLATION
------------

To install the NodeJS Chat module:

 1. Place its entire folder into the "sites/all/modules/contrib" folder of your
    drupal installation.

 2. In your Drupal site, navigate to "admin/modules", search the "NodeJS Chat"
    module, and enable it by clicking on the checkbox located next to it.

 3. Click on "Save configuration".

 4. If you're a site builder and just want to use the module's default features,
    you need to enable the "nodejs_chat.module.js" extension that is located
    under the "extensions" folder of the module in your Node JS server. Make
    sure that at least either the property "clientsCanWriteToChannels" or the
    property "clientsCanWriteToClients" is set to true in the "nodejs.config.js"
    file of your Node JS Server.

 5. Enjoy.

USAGE
-----

After installing the module:

  1. Navigate to "admin/structure/block", and place the "Node JS Chat Block"
     block in a theme region of your choice. It's recommended to place it in
     a reasonably wide region, given the actual layout and styling of the chat.

  2. Alter any of the visibility settings as you would usually do with any other
     block. The are no actual permissions to set up in order to let users use
     the module, althought it won't be displayed for anonymous users in its
     current state. This is likely to change in the future.

  3. Chat away!


ROADMAP
-------

TBC
