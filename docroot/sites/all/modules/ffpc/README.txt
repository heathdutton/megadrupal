
INTRODUCTION
------------
The FileField Podcaster enables Views to generate Podcast feeds using filefield
to store the media files. This module will work with any file that can be
uploaded and managed as a filefield on a node and that can be read by getid3.
Getid3 can read a file and retrieve its metadata (such as runnin time). See
http://getid3.org for more details.

Podcast feeds are those as defined by the specification released by Apple and
available at http://www.apple.com/itunes/store/podcaststechspecs.html.

INSTALLATION
------------
1. Download and unzip the FileField Podcaster module into your modules directory.

2. Goto Administer > Site Building > Modules and enable FileField Podcaster.

3. Goto Administer > Site Building > Views and edit or add a view.

4. In the Style options choose Podcast Feed.

5. In the Row Style options choose Podcast Episode.

6. Add a Filefield as a field.

AUTHORS
-------
- Maintained by Matt Farina (http://drupal.org/user/25701)
- Originally written by suydam (http://drupal.org/user/50195)