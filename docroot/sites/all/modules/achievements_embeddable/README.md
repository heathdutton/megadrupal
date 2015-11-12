# Achievements Embeddable

### Description
Provides services for Achievements.
This module provides REST services for the Achievements module. It will expose 3
methods or resources:

1. get_all_achievements: Retrieves a list of all achievements available on the
site.
2. get_user_achievements: Retrieves a list of all achievements that the
specified user has unlocked.
3. achievement_trigger: Notifies the Achievement module that the specified user
has earned credit towards an achievement, and logs a user as having unlocked an
achievement, if it applies.

This module also creates a hook called hook_achievements_embeddable_callbacks()
that gets executed when the achievement_trigger resource is fired, so that other
modules can react, adding their own "achievement_trigger" functionality whenever
they use this resource.

#### Requirements
1. Services API 3.x
2. REST Server
2. Achievements

#### Installation
1. Install the Achievements Embeddable module as you would normally do with any
other Drupal module.

#### Usage
This module will create a Services endpoint called "achievements_embeddable"
with the "services/achievements" path. It will also check the three methods or
resources explained above, using a REST server and JSON for request and response.

You can create your own Services endpoint, with the configuration that you like.

Once you have the Services endpoint, you should be able to implement the
resources you've selected.

Examples for the achievements_embeddable Services endpoint:

1. GET <http://my.site.com/services/achievements/get_all_achievements>
Retrieves a list of all available achievements on the site.
2. GET <http://my.site.com/services/achievements/get_user_achievements/2>
Retrieves a list of all achievements that user 2 has unlocked. Expects 1
parameter. The User ID (uid).
3. GET <http://my.site.com/services/achievements/achievement_trigger/2/first-contribution>

The last example Expects 2 parameters:

1. The user id (uid)
2. The Achievement ID. This is an string that identifies the achievement you
defined implementing _hook_achievements_info( ).
Notifies the Achievement module that user [2] has earned credit towards the
["first-contribution"] Achievement.

If you want to add your own achievement_trigger functionality, simply implement
the hook_achievements_embeddable_callbacks() hook defining a callback function,
which then you will create to add your custom code. For example:

/**
 * Implements hook_achievements_embeddable_callbacks()
 */
function your_module_achievements_embeddable_callbacks() {
  $callbacks = array();
  $callbacks['your_module'] = array(
    'title' => t('My Custom Trigger'),
    'description' => t('My custom trigger description.'),
    'callback' => '_my_custom_trigger',
  );
  return $callbacks;
}

/**
 * [_my_custom_trigger description]
 * @param  {[type]} $achievement [description]
 * @param  {[type]} $uid         [description]
 * @return {[type]}              [description]
 */
function _my_custom_trigger($achievement, $uid) {
  $results = array();
  //
  // Your trigger code goes here!
  //
  $results[] = 'Successful execution of my custom trigger';
  return $results;
}
