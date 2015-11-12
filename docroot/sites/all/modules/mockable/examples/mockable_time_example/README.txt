Mockable Time Demo
==================

This example module is meant to demonstrate how to use Mockable to develop and test a module which does something special at a given time. Specifically, this module prints "We're closed; please come back later" between 1 a.m. and 2 a.m. Although simple, this is hard to test without using some form of mock data.

This module is meant to provide an example of the simplest possible use case of the Mockable module.

Normally your module might call:

    if (in_array(format_date(REQUEST_TIME, 'custom', 'H'), range(1, 2))) {
      // ... do something
    }

This is hard to test manually because we are rarely at work between 1 and 2
a.m.; it is also hard to test automatically because we don't want to limit our
testbot to work between 1 and 2 a.m.

Enter mock objects. Using [Mockable](https://drupal.org/project/mockable), your code might look something like

    function mymodule_mockable_time() {
      if (module_exists('mockable')) {
        return mockable('mymodule_time');
      }
      else {
        return mymodule_time();
      }
    }
    ...
    function mymodule_time() {
      return REQUEST_TIME;
    }
    ...
    if (in_array(format_date(mymodule_mockable_time(), 'custom', 'H'), range(1,
    2))) {
      // ... do something
    }

For now you are simply passing your function in mockable() if Mockable exists.
Now if you want to test Mockable, you can add [underscore]mock to your function
name, and create a new function with that name, which returns dummy (mock)
data:

    function mymodule_time_mock() {
      // works only if 'mymodule_time' or '*' has been set to mock
      return variable_get('mymodule_mockable_time', strtotime("Wed Nov 09 2011
      1:30:00"));
    }

Now whenever you mock mymodule_time, you will get the mock time (always 1:30
a.m. eastern) instead of the real time. Of course, your mock function can be as complex as you wish. Let's say you are developing, you might want to set the mockable object with one of the following techniques (if you wish to work on getting a GUI for this module, see https://drupal.org/node/2068405):

    drush mockable-set mymodule_time

And, when you are done,

    drush mockable-unset

If you are running tests and are running many different mock objects across your
site, you can call:

    drush mockable-set

which is the equivalent of:

    drush mockable-set *

And, when you are done,

    drush mockable-unset

The equivalent PHP functions are:

    mockable_set($pattern = '*');
    mockable_unset();

Within your simpletests, you can add Mockable as a soft dependency to your
module's info file (I suggest not adding it as a hard dependency, because you might not want it enabled in the production site):

    test_dependencies[] = mockable

This tells Drupal.org's Test Bot to add mockable to your test suite. In your
simpletest:

    setUp() {
      parent::setUp('mymodule', 'mockable');
      mockable_set();
    }
