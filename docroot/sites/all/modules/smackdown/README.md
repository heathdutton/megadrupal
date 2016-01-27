```
  _________                      __       .___
 /   _____/ _____ _____    ____ |  | __ __| _/______  _  ______
 \_____  \ /     \\__  \ _/ ___\|  |/ // __ |/  _ \ \/ \/ /    \
 /        \  Y Y  \/ __ \\  \___|    </ /_/ (  <_> )     /   |  \
/_______  /__|_|  (____  /\___  >__|_ \____ |\____/ \/\_/|___|  /
        \/      \/     \/     \/     \/    \/                 \/
```

Provides a way to compare two pieces of content to vote upon.

This module is great for websites that value high page views as it allows users to view many pages as they keep clicking that vote button.

# Installation Instructions

1. Download the module & place into your typical contrib module directory.
 * Alternatively: `drush dl smackdown`
1. Enable the module at /admin/modules
 * Alternatively: `drush en smackdown`
 * Note: if you don't already have the references and votingapi modules, this will download those for you. Otherwise you will need to get the dependency modules listed within the .info file.
 * Also note that node_reference is contained within the references module.
1. Create a new content type to be your 'smackdown' content type & check the 'Smackdown node' checkbox at the bottom of your edit screen.
1. Create two fields of type 'Node Reference'.
 * Note: you will need one or two other content types with which to use as your references.

Once these step have been completed, you should be able to create a new smackdown, reference two other nodes during creation, and upon viewing the newly created smackdown node you should be able to click on a node title in order to vote for one or the other. By default this will then redirect you to the voting results tab of your smackdown node.