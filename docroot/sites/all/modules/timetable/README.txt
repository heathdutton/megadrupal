Timetable
=========

The Timetable module allows you to create schedules of events, and display them
in a table format.

Set-up
------

This module comes with several Feature that help reduce the amount of set-up
needed:
  - Timetable Timeslot: This Feature is required. It provides the timeslot
    entity type, one bundle on it, and a timefield on that bundle.
  - Timetable Schedule: This provides a schedule and a session node type, and
    the timeslot reference fields on them. You can enable this to get going
    immediately, and expand on what it provides later on.
  - Timetable Rooms: This expands on the Timetable Schedule feature to provide
    rooms for sessions.

Both the Schedule and the Rooms Features come with a sample timetable view that
you can clone and adapt to your requirements. Use of EVA module is recommended
to place a timetable on your schedule entities.

For details of how to set up custom session and schedule types, see the Advanced
set-up section further on.

Content creation
----------------

To create a timetable of events:

1. Create session entities. The timeslot field may be left blank, thus these can
  be created before there is a schedule or timeslots. Indeed, for a conference
  site where participants will be submitting sessions, you may want to add field
  permissions to the timeslot field.
2. Create one or more schedules. In each schedule's timeslot field, use the
  inline timeslot form to create the timeslots for that schedule. Each schedule
  entity represents a day; create as many as required.
3. (optional) If using rooms, create the room entities.
4. Use the admin link on the schedule to edit the timetable for that schedule.
  All sessions that are not yet placed in a schedule are made available for
  selecting for a timeslot.

Advanced set-up
---------------

A timetable system is made up of three or four kinds of entity that work together:

  - the timeslot entity: each timeslot is a specific time interval at which an
    event can take place.
  - the schedule entity: a schedule holds a number of timeslots for a complete
    day.
  - the session entity: a session is an event that takes place at a particular
    time.
  - the room entity (optional): a room is a location where a session takes
    place.

The timeslots and schedules must each be represented by only a single entity type and bundle. There is no restriction on sessions though: any number of different bundles can be sessions (and indeed different entity types though this is untested and not entirely recommended).

To create a timetable system:

1. Ensure you have a timeslot bundle. This module defines one initially. Each new timetable set needs its own timeslot bundle.
2. Add the timefield to this timeslot bundle. (The initial timeslot bundle has this already.)
3. Create an entity type and bundle to be your schedules. For example, add a new node type.
4. Add an entityreference field to your schedule bundle:
  - This must point to the timeslot bundle, and have the 'Timeslot: for
    referencing from a schedule entity' behaviour enabled on it.
  - Use the inline entity form widget to allow quick creation of timeslots.
5. Create an entity type and bundle to be your sessions. For example, add a new node type.
6. Add an entityreference field to your session bundle. This must point to the timeslot bundle, and have the 'Timeslot: for referencing from a session entity' behaviour enabled on it.
7. (optional) For additional session entity types and bundles, add an instance of the same field.

To additionally use rooms:

8. Create an entity type and bundle to be your rooms. Currently only a single bundle of room is supported.
