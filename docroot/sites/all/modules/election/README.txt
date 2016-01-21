Election
========
Election is a module for Drupal 7 providing online voting.


Dependencies
------------
* Entity API (http://drupal.org/project/entity)
* Views (http://drupal.org/project/views)
* Date (http://drupal.org/project/date)
* Libraries API (http://drupal.org/project/libraries)


How to use this module
----------------------
1. Enable at least one of the 'election type' modules (election_stv or
   election_referendum), and all dependencies.
2. Visit /elections to view the list of elections. You can add/edit/delete
   elections from here.
3. Visit /admin/people/permissions to configure permissions.
4. Visit /admin/config/election to configure fields and display settings.

There are several optional sub-modules you can also enable, providing further
functionality, including Election Condition, Election Statistics, Election
Results, and Election Export.


Note on terminology
-------------------
'Post' is a generic name for a single element of an election.

For referendums, the post is a *question*, e.g. "Should Britain join the Euro?",
which voters answer. For STV elections, the post is an electoral *position*
(e.g. "Prime Minister"), for which candidates stand, and for which voters cast
ranked-choice votes.

Posts are Drupal entities, bundled by election type. For example, an STV
election position is an 'election_post' entity with the bundle name 'stv_post'.


Copyright
---------
Copyright 2012 UCLU (University College London Union)
25 Gordon St, London WC1H 0AY
http://uclu.org/


Author
-----
Patrick Dawkins (drupal.org username: pjcdawkins)
