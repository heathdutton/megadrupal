CONTENTS OF THIS FILE
---------------------

 * Summary
 * Supported time strings


INTRODUCTION
------------

This module aims to provide an extra display formatter for 
Date fields from the Date module. As the supplied timestamp gets further away, 
less accurate information will be shown. Future dates and end dates
are also supported.


USAGE EXAMPLE
-------------
This module is intended for developers who need a timestamp
as currently being featured on Facebook. Likewise the module
could be used in conjuction with activity stream modules
such as Message and Heartbeat.


SUPPORTED TIME STRINGS
----------------------

These formats will be shown, depending on the situation:

    # second(s) ago / in # second(s)
    # minute(s) ago / in # minute(s)
    # hour(s) ago / in # hour(s)
    yesterday / tomorrow
    # day(s) ago / in # day(s)
    # month(s) ago / in # month(s)
    # year(s) ago / in # year(s)

If an end date has been provided, the format will be:

    from ### ago until ### ago
    from ### ago until ### from now
    from in ### until ### from now
    + some slight alterations for the cases yesterday / tomorrow
