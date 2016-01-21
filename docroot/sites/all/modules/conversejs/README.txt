ABOUT
==================

This module is a fully featured bridged between drupal and converse.js library,
an advanced XMPP chat client for your website

Converse.js is an open source webchat client, that runs in the browser
and can be integrated into any website.
It's similar to Facebook chat, but also supports multi-user chatrooms.
Converse.js can connect to any accessible XMPP/Jabber server, either from a
public provider such as jabber.org, or to one you have set up yourself.

It's possible to enable single-site-login, whereby users already authenticated
in your website will also be automatically logged in on the chat server,
but you will have to pre-authenticate them on your server.
This needs some configuration at server.
we provide commercial support for advanced use cases.


QUICK START
===================

Since this module depends on a few third-party softwares, setting up this
module can be a little (or very) complex, depending on your situation.

If you want to quickly get up and running with this module, plese read the
QUICKSTART.txt file (HIGHLY RECOMMENDED).

That file contains minimal instructions for quickly setup this
module and get a working chat room in your site.

For additional informations, read below.


WHY CONVERSEJS?
===================

conversejs.module is very fast and efficient because it's based on
XMPP and BOSH.

BOSH stands for "Bidirectional-streams Over Synchronous HTTP",
A technology for two-way communication over the
Hypertext Transfer Protocol (HTTP).
You can read more at: http://xmpp.org/about-xmpp/technology-overview/bosh/

Unlike other chatroom modules like https://drupal.org/project/drupalchat,
conversejs does not bootstrap drupal on every ajax request.
So it's much faster and consumes less resources.
It's very fast at delivering messages and presence events.
It's virtually a realtime instant messaging system because it depends
on BOSH which is very fast.

Since conversejs connects to an XMPP server, its capabilities are not limited.
You can connect it to Google Talk (now also known as Google Hangouts)
or jabber.org .
You can use your favorite jabber clients in windows/linux/mac/android/ios/etc..
to chat with your site's visitors.
It can also be connected to facebook chat.


FEATURES
===================

The original converse.js library includes the following features
(according to its official docs):
    - Single-user chat
    - Multi-user chat in chatrooms (XEP-0045)
    - vCard support (XEP-0054)
    - Service discovery (XEP-0030)
    - Contact rosters
    - Manually or automically subscribe to other contacts
    - Accept or decline contact requests
    - Roster item exchange (XEP-0144)
    - Chat statuses (online, busy, away, offline)
    - Custom status messages
    - Typing notifications
    - Third person messages (/me )
    - Translated into multiple languages (af, de, en, es, fr, he, hu,
      id, it, ja, nl, pt_BR, ru)
    - Off-the-record encryption (via OTR.js)

This module provides all of the above features and all other
options which converse.js provides and are not listed above.
This module also provides some advanced
extrea features and capabilities:
    - Splitted and separated javascript libraries for better
      integration with Drupal
    - Pre-Authenticate users into chat room on page load via ajax
    - Built-in proxy (to overcome Same-Origin-Policy restrictions)
        It seems that this module works fine even without enabling the
        Built-in proxy, however built-in proxy can be used
        for some advanced tasks and can provide better integration
        between Drupal and converse.js
        Built-in proxy needs some configuration before it can be used.
        please read docs for this.
    - Integrate with tokens.
    - Integrate with rules (NOT AVAILABLE YET)


REQUIREMENTS
===================

- jQuery:
    Converse.js requires at least jQuery 1.7 but jQuery 1.10 is recommended.
    After installing jquery_update module, go to its settings and select
    the proper jQuery version.

- token module (and entity tokens)
    You should install token module in order to use tokens in configuration
    fields.
    It's also recommended to install entity and entity_token modules.
    (entity_token module is packaged with the entity module)
    Installing entity_token module provides more tokens for use.

- converse.js library
    Download converse.js from this location:
    https://github.com/hejazee/conversejs_drupal
    Direct link:
    https://github.com/hejazee/conversejs_drupal/archive/master.zip
    Then extract the archive in lib/ folder so the converse.js file should be
    located at lib/converse.js/converse.js
    NOTE:
      you can also build your own converse.js. for instructions go to
      https://github.com/jcbrand/converse.js
      If you want to use your own library, you should configure
      library placements in module's administration page.

- xmpp-prebind-php library
    Download xmpp-prebind-php library from this location:
    https://github.com/hejazee/xmpp-prebind-php
    And extract is in lib/ folder so the XmppPrebind.php file should be located
    at lib/xmpp-prebind-php/lib/XmppPrebind.php

- A working XMPP server and BOSH server
    Of course, you need a working BOSH server and XMPP server.
    In fact, this module only connects to a BOSH server and does not interact
    directly with XMPP.
    If you don't have your own BOSH server, you can use this URL:
    https://bind.opkode.im
    But be aware that this is a security risk. You should always use a trusted
    server, because all user credentials are transferred via the BOSH server.

    If you want to setup your own BOSH server, you can set it up with your
    XMPP server (recommended).
    Several severs now come with built in BOSH support:
    ejabberd, Tigase, Openfire, and Jabber XCP.
    These can be enabled in the server configuration and allow
    BOSH clients to make connections.

    You can also use a standalone BOSH server.
    There are a few implementations of BOSH that are not tied to a
    particular XMPP server implementation: Punjab, Araneo, JabberHTTPBind


INSTALLATION
===================

First make sure you have downloaded and configured all dependencies.
see REQUIREMENTS section.
Then download and install this module as usual.
After installation, go to settings page at admin/config/services/conversejs
and configure the required fields. And also configure module's permissions at
admin/people/permissions


COMPATIBILITY NOTES
===================

The original converse.js library is packaged with jQuery and a lot of
other packages. and this may cause conflicts and errors in you site.
So I have created a customized repository and you should download converse.js
from that repository.
However if your site includes one or more of the libraries which are included
in converse.js, you should take care.
If you experience such issues, you can go to Library placement page at
admin/config/services/conversejs/libraries
and resolve issues.


AUTHOR/MAINTAINER
===================

This module is created and maintained by Ahmad Hejazee
http://www.hejazee.ir/


COMMERCIAL SERVICES
===================

If you need commercial support and services, please contact me via my blog at:
http://www.hejazee.ir/contact
Or use this contact form (You need to have an account in drupal.org):
https://drupal.org/user/911168/contact


REFERENCES
===================

- Converse.js library and docs:
    https://conversejs.org/
- Author site:
    http://www.hejazee.ir/
