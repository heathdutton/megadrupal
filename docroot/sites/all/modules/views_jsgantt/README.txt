Readme file for the jsGantt View module.
---------------------------------------------

About jsGantt View:

Provides a Views style plugin that displays a gantt chart using jsGantt.

Manual installation:
Install and enable like any other Drupal module. Create a View that exposes Date
fields. Set the View style to "jsGantt", and configure the style to select the
Date fields that correspond to start and end dates for tasks. Also, if tasks 
have a "parent" (OA Cases have parent Projects, for instance), you can add that
relation as well. For example, for OA Cases and Projects, you would need to add
a field of type "Node: Title" and add the relationship "Case Tracker: Project"
to this field (this will expose the title of the parent project to jsGantt).
Then, configure the jsGantt style so that "Case Tracker: Project" is the "Parent
ID Field", and the last "Node: Title" is the "Parent Title Field". You will need
to sort the view by the start date field. Also, if you want to use a multi-value
field (for instance, display assignees to a task from a multi-value
userreference field), you need to make sure to check the option to group
multiple values!

Dependencies:
Views
