********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: BrightTalk Field Module
Author: Robert Castelo <www.codepositive.com>
Project:  http://drupal.org/project/brighttalk_field
Drupal: 7.x
********************************************************************
DESCRIPTION:

Provides fields that can display a BrightTalk webcast or channel.

BrightTALK embeds are fully functional and allow users to register
and view content both live and on demand.

For a channel field the user needs to enter a channel ID.

For a webcast field the user can either paste in the embed code or
input the channel and webcast ID.

For more information about available BrightTalk content:

https://www.brighttalk.com


********************************************************************
INSTALLATION:

Note: It is assumed that you have Drupal up and running.  Be sure to
check the Drupal web site if you need assistance.

1. Place the entire directory into your Drupal directory:
   sites/all/modules/


2. Enable the module by navigating to:

   administration > modules

  Click the 'Save configuration' button at the bottom to commit your
  changes.


********************************************************************
USAGE

  Add BrightTalk channel or webcast fields to the content type of your
  choice on the Manage Fields page of the content type.

  * BrightTalk Channel
  - Channel ID Option
  Choose field type 'BrightTalk Channel' and then widget 'Channel ID'.
  You will then be able to add channels to nodes by adding a channel ID
  to the field, e.g. '43'

  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  * BrightTalk Webcast
  - Embed Code Option
  Choose field type 'BrightTalk Webcast' and then widget 'Embed Code'.
  You will then be able to copy embed code from the BrightTalk site and
  paste it into the field to display a webcast.

  The embed code will look like this:

  <script type="text/javascript"
          src="https://www.brighttalk.com/clients/js/embed/embed.js"></script>
  <object class="BrightTALKEmbed" width="705" height="660">
    <param name="player" value="channel_player"/>
    <param name="domain" value="https://www.brighttalk.com"/>
    <param name="channelid" value="43"/>
    <param name="communicationid" value="77677"/>
    <param name="autoStart" value="false"/>
    <param name="theme" value=""/>
  </object>

  * BrightTalk Webcast
  - Channel & Webcast ID Option
  Choose field type 'BrightTalk Webcast' and then widget 'Channel & Webcast ID'.
  You will then be able to add webcasts to nodes by adding a channel ID
  to the field, e.g. '43', and a webcast ID, e.g. '77677'.
