
NODE ANNOUNCE
========================================

Node Announce is designed to solve a problem I had. Most of my
community drupal sites are basically event listings. Clubs that have
one or two events going on every month. These organizations also have
discussion lists that are on some other site (yahoo, or a straight
mailman list). What I wanted was a way to automatically and regularly
sent out announcements for the events on our site.

Thus became Node Announce.

REQUIREMENTS
----------------------------------------

Required Modules:
- date
- date_api
- token

Suggested Modules:
- htmlmail

If you have htmlmail installed, emails will be sent as HTML mail by
default. If not, the emails will be down converted to plain text using
drupal internal features.

USAGE
----------------------------------------

Once you have installed Node Announce you will need to create a new
announcement (admin/config/system/node_announce/add).

You will then be creating an announcement for a Content Type, based on
a specific Date field that it has. The days before field is used to
send out the email announce aproximately that many days before (as set
by the user). This is aproximately as it runs as a drupal cron
task. Because of possible timing issues Node Announce will actually
send out an email sometime within the timeblock that is 24 hours
before and 24 hours prior to the ideal date/time.

Drupal Cron *must* be run at least daily for Node Announce to be reliable.

The announcement contains the from and to email address, as well as
subject and message fields that are tokenized. You can use any of the
standard tokens in either the subject or body.

Example: TBD

A record of the announcement being sent out for the event is made, so
that an announcement will only be sent out once for each combo of
Announcement + Node.

EXPECTED USE
----------------------------------------

My site has a content type called meeting, with a Date field
meeting_date. I create one Annoucement for 7 days before any Meeting /
Meeting Date event, and one Announcement for 2 days before any Meeting
/ Meeting Date event. Both Announcements have my email address as
from, and the group mailing list as the to address.

With these 2 announcements in place all future meetings automatically
get emailed out prior to their meeting time.

As this is still early, you may very well want to create a 3rd
Announcement that has the same content but sends to a private email
address. This will ensure that you get to see what the email looks
like prior to going to hundreds of people.

KNOWN LIMITATIONS
----------------------------------------

Repeating dates are dicey. Use with caution.

GET INVOLVED
----------------------------------------

Planning for the future of this module happens over on a trello board
- https://trello.com/board/node-announce/4e7734b88a99f20bf70d80e3 all
are welcome to participate
