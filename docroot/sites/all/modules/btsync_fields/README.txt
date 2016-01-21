The BitTorrent Sync (BTSync) fields module adds Field types and widgets allowing
various methods for syncing files to entities.

An example would be the basic File widget, which allows for a BTSync secret to
be attached to a File field so that when new files are synced they will be
automatically attached to the field.

BitTorrent Sync fields was created and is maintained by Stuart Clark:
- http://stuar.rc/lark
- http://twitter.com/Decipher



Features
--------------------------------------------------------------------------------

- Field widgets:

  - BitTorrent Sync (basic) for File and Image fields:

    A Basic BTSync field widget that allows the user to supply or generate a
    BTSync secret which will be used as the source of the Field items.

    When Cron is run it will check whether the Files in the sync have changed
    and if so it will re-attach the items to the field.



Required modules
--------------------------------------------------------------------------------

* BitTorrent Sync API - https://drupal.org/project/btsync
