HiringThing is online software that helps companies post jobs online,
manage applicants, and hire great employees.

This module provides a Drupal block with the HiringThing embeddable
jobs widget that allows companies to add a list of jobs to their
website.

## Configuration

 1. Enable the *HiringThing* module.

 2. Go to block configuration (Structure >> Blocks)

 3. Find the "HiringThing jobs widget" in the list of disabled blocks
    at the end.

 4. Click the "configure" link next to it.

 5. If you'd like you can override the blocks title (by default it's
    "We're hiring!")

 6. Enter your HiringThing account URL. If you don't yet have a
    HiringThing account, visit http://www.hiringthing.com for a 30-day
    free trial.

 7. Select which region you'd like the block to appear (probably one
    of the sidebars!)

 8. Click "Save block"

 9. That's it!

Now your job listings on HiringThing.com should appear in a block in
the given region. Check it out!

## Accessing the HiringThing API

This module also provides optional integration with the HiringThing
Read API.

To use it:

 1. Enable the *HiringThing* module.

 2. Download and enable the
    [Libraries](http://drupal.org/project/libraries) module.

 3. Download the
    [HiringThing PHP library](https://github.com/hiringthing/hiringthing_api_php)
    and extract it into `sites/all/libraries/hiringthing` (this directory should contain hiringthing.inc)

 4. Configure your API key and password (Configuration >> Web services
    >> HiringThing)

 5. In your code, do: `$hiringthing = hiringthing_api_object();` to get
    an API object!

See the [HiringThing PHP documentation](https://github.com/hiringthing/hiringthing_api_php)
for more information on what you can do with that object.

