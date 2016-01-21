
General Information
-------------------

This module implements a punctuation pre-processor that provides special
handling for punctuation characters when they are encountered within word
strings during searching and indexing.

Though the core Drupal search module provides some basic punctuation processing,
some characters (such as hyphens, periods and underscores) are never treated as
word boundaries. This means that strings like "word-power" are only matched in a
search for the whole string ("word-power") but NOT the individual fragments
("word" or "power").

The punctuation pre-processor attempts to overcome this limitation by analyzing
all punctuated word strings during searching and indexing (before the core
search applies its own processing), and enforcing word boundaries when
appropriate. It also ensures that all combinations of punctuated words are
recorded and searchable (e.g., "word-power" will be matched with a search for
"word", "power", "word-power" and "wordpower").


Installation
------------

1. Unzip the files, and upload them as a subdirectory of the sites/all/modules
directory of your Drupal installation (or the location you normally use for
contributed modules).

2. Go to 'Modules', and enable the Punctuation Pre-Processor module.

3. Once you enable the module, your site will automatically initiate the content
re-index process. At this point you'll want to ensure that cron has run at least
once, to build the search index. On larger sites, it may take several cron runs
to complete the search index. You can check progress on the Search Settings
page, and you can run cron manually by visiting 'Reports > Status report', and
clicking on "Run cron manually".


Configuration
-------------

All configuration options are available within the search administration page at
'Configuration > Search and Metadata > Search Settings'.

* The "Bypass URLs" option will ensure that punctuation pre-processing rules are
not applied to URLs or email addresses, regardless of the punctuation that they
contain (e.g., "http://drupal.org/some-path" should NOT be matched when
searching for just "drupal", "org", "some" or "path").

* You can specify the exact punctuation to be analyzed by the pre-processor in
the "advanced" options. By default the hyphen, period, underscore and apostrophe
are processed, but any group of punctuation characters can be specified. Note
that the core search module already treats a wide variety of punctuation as word
boundaries, so the list of characters to be pre-processed does not need to be
exhaustive. For most sites the defaults are acceptable.


Additional Notes and Testing
------------------------------------

* The use of this module will increase the size of your search index slightly
due to the way all combinations of punctuated words are recorded (e.g.,
"one-two-three" will be indexed as "one two onetwo three twothree onetwothree").
This increase should be negligible for most sites.

* This module does not overcome all limitations of phrased searching when
strings that contain punctuation are involved. When running a phrased search
(placing your content in quotes), stings that contain punctuation will still
only be matched if searching for the exact, and complete, string contained in
your content.
