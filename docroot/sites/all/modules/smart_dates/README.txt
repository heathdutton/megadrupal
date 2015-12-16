Smart Dates is a module about user interface, and making your Drupal
site more magical.

If you've ever had to create a lot of content with Dates included,
you'll have run into a standard frustrating user experience. From Date
and To Date are populated with some defaults, and after changing the
From Date, you have to go and also change the To Date. It's
aggravating, as we've come to expect our date fields to be smart, and
do expected smart things.

Smart Dates solves that, using datejs. It links From and To, Dates and
Times together with javascript. Maintaining intervals between them.

Algorithm

For the Time field, the logic is as follows: if From and To started
off the same (set by some default) make the To date 2hrs after the
From date (a setting for this default will be added later). After that
it does interval math between From and To, so if From is 6:30PM and To
is 8:00PM, and you change From to 7:00PM, To will now be 8:30PM.

For the Date field, intervals are always what's used. If you move From
ahead by a week, To date will shift the same amount.

This should work regardless of what input formatting you use for dates
and times, as the module translates between the date format you have
specified for the date field into the javascript formatter.

Installation and Configuration

Install the module. There is no configuration.

The module works by using hook_elements(), and hooking onto the
existing date fields. It will work in conjunction with Date Popup with
no issues.

Known Limitations

datejs is culture specific on how it parses dates (m/d/y vs. d/m/y,
for instance), and for now only the en-US version has been
integrated. If there is interest in other cultures being put in,
that's possible, but I'll wait until there is an open request so I've
got testers in other locales to make sure it *really* works.

Time fields aren't linked to Date fields. To Time will roll over past
midnight correctly, but the To Date doesn't shift forward when that
happens. This may be fixed in the future if people are actually
running into this on a regular basis.

Please file a tickets

This works in my setups extremely well, however everyone's setups are
different, so I really want folks to report issues in if they run into
them. I'll be happy to fix them.
