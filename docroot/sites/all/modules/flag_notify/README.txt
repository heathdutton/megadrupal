FLAG NOTIFY DRUPAL MODULE

A simple but flexible module that uses the great flag module for email
notifications of site activity and optionally integrates with organic groups.

It uses a hierarchical notification system organised around three levels:
groups,nodes,comments. Any of these "levels" are optional and if the og module
is not installed the group level is just hidden.
Users are always notified only once for the same event, with preference to the
most relevant for them (i.e. a reply to own comment is notified as such rather
than "there is a comment in your group").

Flag Notify purposely do not build the flags in code (it just ask for existing
flags to use), nor it creates itself the view of elements that have been
flagged (subscribed).
While this approach requires little bit extra work from the users, it increases
the flexibility and avoid potential problems if the og module is
enabled/disabled after this module.
If necessary, I can provide an export of flags and/or views to be used in
conjunction with this module.

Default notification for users are managed trough an option field in the user
profiles so they can can choose the default notification settings when
submitting a comment, a node or join a group.

You can "intercept" the notification and change the event data or "break" it
using the hooks defined by the module (see flag_notify.api.php).

Currently the notification email text is fixed (with a few customisation as
footer and salutation). A token-based email may be developed in the next
versions if there is enought request for that.

This project is sponsored by Associazione Alessandro Bartola and Università
Politecnica delle Marche that pay for my time :-)

For installation instructions please read the INSTALL.txt file.
