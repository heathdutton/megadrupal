-- contents of this file --
* motivation
* Features
* Planned Features
* Dependencies
* Installation
* Similar Modules
* Community Support

-- Motivation --
I am annoyed by messages on my page when I don't need them anymore.
So my goal was to hide messages after I have read them.
I got inspired by Timed Notifications with CSS Animations
http://tympanus.net/codrops/2012/06/25/timed-notifications-with-css-animations/
But I soon figured there is need for more functionality that can't be done with
css only.

-- Features --
works with all messages from drupal_set_message()
enhanced css for drupal messages
doesn't move messages, so they stay where you want them, even when using panels
hides status messages after 5s, warnings after 10s and errors after 15s
animated progress bar, to indicate when the message will be hidden
on hover the timer and progress bar is paused, to keep the message for further
reading after timer has finished the corresponding messages will be hidden
after hiding a message, a link to show the message is shown
a close button to hide messages

-- Planned Features --
admin page to test the messages
form to edit the timers for each message on admin page move message
for missing jQuery pause to admin page, so even without jq pause its usable

-- Themes --
I have tested timed messages on the following themes. If you test in on other themes and it it working, let me know, so I can add the theme to this list. If you find a theme it's not working on but you would like it to, post an issue

Seven
Adminimal
AT Admin, AT Core, AT Panels Everywhere
Bartik
FontFolio
Garland
Skeleton
Stark
Aurora
Partially supported: Omega
Not supported at the moment: Arctica, Bootstrap, Ohm



-- Dependencies --
Libraries module: http://drupal.org/project/libraries
jQuery Pause: http://tobia.github.com/Pause/

-- Installation --
make sure Libraries module ist activated
download jQuery Pause and place in libraries folder
(e.g. sites/all/libraries/pause/jquery.pause.min.js)
activate timed messages and you are ready to go

-- Similar Modules --
An awesome list to help you find what you need is found on
http://groups.drupal.org/node/51088.
I found none of them hides the message without extra clicking by the user.

-- Commutity support --
This is my first contributed module, so I am happy about advice from
the community. If you find this page needs better documentation, the
code quality can be improved, you find bugs or you have ideas for more
features, please let me know.
