Drupal mailadmin
================

Drupal module that provides an administration interface for a e-mail
server configuration database.

Currently, the only supported database type is PostgreSQL, although the
database should be usable (although with less security and elegance) on
all Drupal's supported platforms.

Setup/installation
------------------

Here we assume that you have Drupal and this module installed correctly.

#### Permissions ####

The most secure way to configure this module is to create a separate
database user for the mail server, with access to only the tables and
functions it needs. In this example, that user is called `mailreader`
and the database is called `mailadmin` (as is the user that owns it).

Grant access to the database and required functions:

    GRANT CONNECT ON DATABASE mailadmin to mailreader;
    GRANT EXECUTE ON FUNCTION mailadmin_account_maildir(TEXT, TEXT, TEXT) TO mailreader;
    GRANT EXECUTE ON FUNCTION mailadmin_alias_destinations(TEXT, TEXT) TO mailreader;
    GRANT EXECUTE ON FUNCTION mailadmin_get_mailbox(TEXT, TEXT) TO mailreader;
    GRANT EXECUTE ON FUNCTION mailadmin_is_local_domain(TEXT) TO mailreader;
    GRANT EXECUTE ON FUNCTION mailadmin_is_local_user(TEXT, TEXT) TO mailreader;

