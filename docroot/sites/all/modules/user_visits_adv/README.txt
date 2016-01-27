-- SUMMARY --

The User Visits Advanced module is a contrib module for the User Visits module.
It is extremely useful for social networking sites who wish to provide their
users with statistics about the number of times a the users profile page is
being viewed and by which other users. See the provided screen shot how the
block of this module can look like.

This module comes with its own database table to store statistics per user
profile (uid). At cron time, the data from the user_visits table is collected,
aggregated and stored in the user_visits_adv table. It stores

  - the total number of visits per uid
  - the number of visits of the X past days per uid
  - the most recent visitors uids of the past Y hours


-- REQUIREMENTS --

User Visits (user_visits) module must be enabled.


-- INSTALLATION --

1) Enable the user_visits_adv module.
2) Go to admin/user/user_visits and the advanced fieldset.
3) You can choose to enable the two blocks: My recent visitors adv &
My visitors history adv.
4) Configure each block for the amount of data which should be handed over to
the theming functions.
