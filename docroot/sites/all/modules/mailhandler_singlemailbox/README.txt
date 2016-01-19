http://drupal.org/project/mailhandler_singlemailbox

LICENCE: See LICENCE.txt
INSTALLATION: See INSTALL.txt
CONFIGURATION: See INSTALL.txt

ABOUT MAILHANDLER SINGLE BOX MODULE and MAIL MEDIA MODULE

Mailhanlder and Feeds can be used together to treat email mailboxes as feeds
'sources'. Together, they allow the creation of nodes from email messages.

If you wanted different user accounts to have their own mailbox to send content
to a site, it would require configuring multiple mailhandler mailboxes
for each account as well as multiple feeds to process each mailbox. The
provisioning of mailboxes on a mail server and configuration of so many
mailhandler mailboxes and the associated feeds could be automated, but it would
be pretty gnarly.

The solution we came up with was to have a single mailbox on the mail server,
and have a single mailhandler and single feed processor for the a site.
Instead of each user getting their own inbox, we have a single inbox that
collects any number of addresses. This can be done by either configuring the
mail server to have a single ‘catch all’ address or, if available, a plus
addressing scheme (think GMail).

The module provides an user interface that allows users with the appropriate
permissions to ‘generate’ a new address for themselves. The addresses generated
are in the format DRUPALUSERNAME-RANDOMSTRING@mailbox.example.com in the case of
catch-all addresses or
MAILBOXUSER+DRUPALUSERNAME-RANDOMSTRING@mailbox.example.com in the case of plus
addressing.

These two default address generators are implemented as CTools plugins.
This allows developers to write their own plugins that generate unique addresses
if the formats provided don’t meet their needs. (or someone could provide
patches to the default generators to allow for configuration options).

The module exposes a method to retrieve all generated email addresses and their
associated user ids to itself and other modules. This is how the
‘mailhandler_sendto_auth’ module 1) authorizes each email and 2) matches each
message to a site user.

MAIL MEDIA MODULE

For convenience we have provided the Mail Media Module as part of Mailhandler
Sinble Mailbox Module. It is a glue/feature module that provides and example
configuration for importing emails with photo or video link attachments. It does
the work of creating a node type and defining a configuration for a feeds
importer making use of Mailhandler. It pre-configures the a feed fetcher, parser
and processor.

Use it as is, or learn from it and come up with your own configuration.

MORE INFORMATION

See: Mailhandler Module
See: Feeds Module
See: Mailhandler Send To Auth Module
See: Mailhanlder Media Links Command Module