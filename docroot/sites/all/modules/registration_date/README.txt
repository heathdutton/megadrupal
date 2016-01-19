Registration Date
dpi@d.o - http://danielph.in

Provides actions based on Date fields attached entities with registrations.

## Dependencies:

* Registration
* Date API

## Features

Depending on the value of a date field attached to a host entity, the following
actions may take place:

  ### Open or Close New Registrations

  Entity Registrations are closed or opened automatically, 

  ### Send Reminder

  A reminder will be sent on a date 

An option to disable date open and close functionality included with 
entity registration module.

Copyright (C) 2012 Daniel Phin

## License

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

## Instructions

1. Attach a `date` field to an entity
2. Attach a `registration` field to an entity.
3. In field settings for a Registration field, select which functionality you
   would like to apply when a date fields value has been reached.

## Development Notes

Entities can have more than one Registration field attached; registration_date
will automatically use the first Registration field found on the entity.