Giant Bomb
dpi@d.o - http://danielph.in

Automatically import data from Giant Bomb and compatible endpoints.

## External Dependencies:

* Entity API
* Date API

## Notes

### Types

Giant Bomb has a listing of types that can be retrieved. It is an important
first step (after entering your API key) that you 'Reload types' in the 
'Types' tab.

Giant Bomb types will be listed on the 'Types' tab. Each type maps to an
entity/bundle pair (example: node/game, node/franchise). You need to create
your bundle (for nodes: admin/structure/types/add), then go to the
'entity mapping' page for the Giant Bomb type, and select your new bundle.

Fields for your type must be configured, four fields must be attached to your
bundle, otherwise the module will not know where Giant Bomb objects are located
on your Drupal site:

  * 'Endpoint' maps to a 'text' field.
  * 'Type' maps to a 'text' field.
  * 'Object ID' maps to an 'integer' field.
  * 'Next Update' maps to a 'datetime' field.

Another useful field to configure is 'Entity > Label' to 'Title'.

### Jobs

Begin by creating a job. A job can be a `list`, or a singular `object`. A list 
job contains *all* objects of a type available on Giant Bomb. Lists can queue
the objects contained within it, and then queue objects related to each object
in the list. Object jobs queue objects, related to a single object.

An example of an object job:

Use the search function to locate the 'Battlefield' franchise. The
'Battlefield franchise' is an object on Giant Bomb. Create a job based off
this object. When select the 'Game' option in 'Create related objects'. This
job will create a 'Battlefield' entity locally, and will then create all
'Games' in the 'Battlefield franchise'.

When the cron task for your Drupal site runs, it will check to see if any
jobs are ready to be refreshed. Cron will also process any objects that have
been queued for update.

After a job has been refreshed, objects associated with the job are queued for
update. Object are never updated straight away. It is important that cron is 
set up to run on a frequent basis. Typically, 20 object will be processed
during a single cron run.

Each Giant Bomb object can map to one entity on your local Drupal site.

### Relationships

Relationships between objects can be created automatically using
[Entity Reference](http://drupal.org/project/entityreference) or 
[Relation](http://drupal.org/project/entityreference) (recommended) modules.

Relationships can be exposed via Views, for example a list of related objects
to the object currently being viewed.

For relationships to be created, you must configure how relationships are
stored in Drupal. You may use Entity Reference fields and/or Relations at the
same time. Options to automatically configure relationships between types 
are provided. 

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