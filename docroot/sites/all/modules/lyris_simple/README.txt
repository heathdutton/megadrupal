Lyris Simple Forms
==================

This module provides simple subcribe and unsubscribe forms for a Lyris mailing
list.  In effect, it mimics the custom Lyris web forms you can create through
your Lyris admin UI at /utilities/webforms.

Two blocks are provided that you can drop into any region or panel layout, one
for subscribing and one for unsubscribing.

All the settings can be found at /admin/config/services/lyris-simple

Block specific settings can also be changed from within the block configuration.


Technical notes
--------------------------------------------------------------------------------

Subscribe Form Params
---------------------
# Dynamic data sent to Lyris
- email (member's email)
- name (member's name) (optional)
- list (Lyris list name)
- confirm (Type of confirmation email to send)

# Fixed False
- showconfirm (Show the user a confirmation page)
- appendsubinfotourl (Appends data to return URL)

# Handled by us, do not send to Lyris
- url (Return URL)

Unsubscribe Form Params
-----------------------
# Dynamic data sent to Lyris
- email (member's email)
- lists (Lyris list names)
- email_notification (Send an email notification)

# Fixed False
- showconfirm (Show the user a confirmation page after processing)
- confirm_first (Would take the user to a confirmation prompt page)
- appendsubinfotourl (Appends data to return URL)

# Handled by us, do not send to Lyris
- url (Return URL)
