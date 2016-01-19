_______________________________________________________________________________

                                -INTRODUCTION -
_______________________________________________________________________________

This module allows you to display the number of the users currently on any particular page. That's it!

_______________________________________________________________________________

                               - CONFIGURATION -
_______________________________________________________________________________

On the configuration page of the module (admin/config/pagewatchers), the following can be set:

- 'Count for anonymous users': As the name suggests, set whether to count anonymous users.

- 'Cache Lifetime': Define how old do cached numbers have to be to be considered too old and be discarded. Set to 0 to disable. (Disabling caching might prove useful while still in development stages)

- 'AJAX Update Intervals': Define the intervals between the AJAX updates by the JS.

_______________________________________________________________________________

                                - HOW TO USE -
_______________________________________________________________________________


OPTION 1 (EASY):
----------------
The module defines a block "Online Users On Page". Just use it as you would use any regular block!


OPTION 2 (ALSO EASY!):
----------------------
The JS in the module will automatically detect for any HTML element with class "pagewatchers--live-count". Simply having such element(s) on the page will have the script automatically update it with the number of online users.

NOTE: Be cautious regarding which pages display this counter. Displaying it on too many pages on a busy website might result in performance issues.