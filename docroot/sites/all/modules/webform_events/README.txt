README Webform Events
--------------------
11.02.2013 Initial entry to README.txt. Module added to review queue.

Module functionality:
Provides event content type that can be used to create events with possible slots, administration tools and functionality on account pages.

Complete list of features.

Webform events is a module that provides the following:

- Event content type
- Administrative tools to manage an event
- Event page with current participants, and possible queue to an event
- Users can opt to: Signup, queue (If event is full) and cancel their signups
- Ending date support: After set date event is closed
- Participant limit support: After certain number of signups, event turns on queue and prevents users from signing up.
- Queue protection: Queue is protected while administrator is editing it
- Event overflow protection: Event can't overflow under situations many users are filling the form
- Re-opening event if space is freed up (Queue is protected, signups are enabled only after queue is sorted)
- Easy updating from queue
- Handling of cancelled users (Will be improved later on)
- Events page(view) also administrative tool for quick searching (ajax)
- Themeable admin page via template
- Slots based registration
- Overbooking functionality
- Block for user event activity
- Toggle to show event activity on user profile
- Admin page for defaults and options
- Start date functionality
- Creates webform base on node creation
- Toggle for single slot signups
- Overbooking toggle
- Automatic activity notification for email address via node form
-Toggle for automatic updating of node titles according to event status

Setup:
Enable module (Preferred way: drush en webform_event)

When creating event, you wont notice much difference. You will see event with due date for signups and participant limit. If you want to keep your event this way, feel free to do it but to unlock the power of the module create a webform component and view your module. Your event gets signups, option to queue to full event and cancellations. Participation can be managed easily from Manage event -tab.

Dependencies:
- Views
- Webform
- Date
