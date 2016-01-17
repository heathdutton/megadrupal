Bot Rules (botrules) module

SUMMARY

This module provides Rules events, conditions and actions to integrate an IRC
bot with Rules.

PREREQUISITES

A working IRC bot using Bot module.

CURRENTLY IMPLEMENTED FEATURES

Events:
  - Received a channel message
  - Received a private message
  - User joined channel
Conditions:
  - Bot is on a channel
  - Nickname is on a channel
  - Nickname is on any of bot's channels
Actions:
  - Get user count of a channel
  - Log a highlight (requires bot_log module)
  - Make bot change its nickname
  - Make bot join a channel
  - Make bot part (leave) a channel
  - Modify Botagotchi greeting responses (requires bot_agotchi module)
  - Modify Botagotchi feeding responses (requires bot_agotchi module)
  - Queue a message to a nickname until they show signs of activity
  - Send a message to a nickname/channel
  - Send an action to a nickname/channel

EXAMPLES

The module includes one default rule (initially disabled) that demonstrates
sending new comment notifications to IRC channel #test.

MISCELLANEOUS NOTES (in no particular order)

'Log a highlight' action:
This writes a log entry with a freely configurable 'category' instead of
channel name. On the module configuration page, you can pick categories to be
displayed on the logs page, below real channels. This can be used, for example,
to record mentions of some specific word, keep track of bot_karma messages,
etc.
Technically, you can write to real channels' logs, too (enter the channel name
without '#'), but if you choose to do so, perhaps you should be extremely
careful not to confuse your users.

Error logging:
Using variables in 'send a message'/'send an action' actions may result in an
attempt to send an invalid message/action, where either the target or body is
an empty string. Such messages/actions will not be sent to the IRC server, and
a notice will be logged to dblog. If you are consciously taking advantage of
this (e.g., using data transformation actions to parse data), you can turn off
logging of invalid messages.

'Make bot change its nickname' action:
Note that the nickname is changed permanently, in Bot settings as well. (If we
did not do so, Bot module would change it back on the next five-minute cron
run.)
This action should be used with some caution as it does not check whether the
nickname is available; attempting to assume a name which is already taken can
result in a sort of nick flood, so it is reasonable to stick to nicknames you
have registered.

User data (counts and online status):
In initial synchronization upon joining a channel, the bot somehow fails to
'notice' some users (5 to 7 out of ~500 in #drupal). We work around this by
issuing a /WHO once in a while (specifically, in the five-minute cron), so
normally user counts should be accurate five minutes after the bot joining a
channel. If you have a use case that requires more aggressive synchronization,
let us know.

DEVELOPMENT STATUS

Upgrade path exists since 7.x-1.0-alpha2.
Feature requests are welcome.

CREDITS

This module was commissioned by Sliced Bread Labs.

AUTHOR/MAINTAINER

Drave Robber
drave at right-here-tavern dot info
