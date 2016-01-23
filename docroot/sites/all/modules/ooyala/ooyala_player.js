
/**
 * @file
 * Ooyala JavaScript API implementation.
 *
 * The Ooyala JavaScript API allows you to register a callback function that
 * Ooyala will use to notify your script(s) of video player events. Since it is
 * only possible to specify a single callback function per player and we want
 * to make sure that any number of modules have access to the API we implement
 * a simple system that allows modules to register a listener which will be
 * notified whenever an event notification is received by the callback
 * function.
 */

/**
 * Intialize the Drupal.ooyala object if it hasn't already been initialized.
 *
 * Modules wishing to use the Drupal.ooyala API should include this line at the
 * top of their JavaScript files as it is possible for the module's JavaScript
 * to be included before this file.
 */
Drupal.ooyala = Drupal.ooyala || {'listeners': {}, 'onCreateHandlers': {}, 'players': {} };

/**
 * Dispatch Ooyala player callback events to all registered listeners.
 *
 * This method is DEPRECATED. It only works with the v2 player. For the v3
 * player API, see Drupal.ooyala.onCreate().
 *
 * Listeners are registered in the Drupal.ooyala.listeners object as follows:
 * @code
 *    Drupal.ooyala.listeners.myModule = function(player, eventName, p) {
 *      ...
 *    };
 * @endcode
 *
 * All listeners will receive the following arguments.
 * - player: The DOM object representing the Ooyala player that is receiving
 *   the event notification.
 * - eventName: The event name as sent by Ooyala.
 * - p: Extra notification parameters.
 *
 * Read the Ooyala JavaScript API documentation for a complete list of events
 * and their possible extra notification parameters.
 * @see http://www.ooyala.com/support/docs/player_api#javascript
 */
Drupal.ooyala.notifyListeners = function(playerId, eventName, p) {
  var player = document.getElementById(playerId);
  jQuery.each(Drupal.ooyala.listeners, function() {
    this(player, eventName, p);
  });
};

/**
 * Allow modules to respond to the creation of a new player.
 *
 * This callback is used on sites using the v3 player. Typically a module
 * providing an onCreate handler will register a "message bus" using the Ooyala
 * API.

 * OnCreate handlers are registered in the Drupal.ooyala.onCreateHandlers object
 * as follows:
 * @code
 *    Drupal.ooyala.onCreateHandlers.myModule = function(player) {
 *      player.mb.subscribe(OO.EVENTS.PLAYING, 'example',
 *        function(eventName) {
 *          alert("Player is now playing!!!");
 *        }
 *      );
 *    };
 * @endcode
 *
 * All listeners will receive the following arguments.
 * - player: The Ooyala player object. Events may be bound to the player.mb
 *   (message bus) property.
 *
 * @see http://support.ooyala.com/developers/documentation/reference/player_v3_dev_listenevent.html
 */
Drupal.ooyala.onCreate = function(player) {
  Drupal.ooyala.players[player.elementId] = player;
  jQuery.each(Drupal.ooyala.onCreateHandlers, function() {
    this(player);
  });
};

(function ($) {
  Drupal.ooyala.listeners.ooyala = function(player, eventName, p) {
    var playerId = $(player).attr('id');

    switch(eventName) {
      case 'embedCodeChanged':
        $('#title-' + playerId ).empty().append(p.title);
        $('#description-' + playerId ).empty().append(p.description);
        break;
      case 'loadComplete':
        var description = document.getElementById(Drupal.settings.ooyalaSharedPlayer).getCurrentItem();
        $('#title-' + playerId ).empty().append(description.title);
        $('#description-' + playerId ).empty().append(description.description);
        break;
    }
    return;
  }
})(jQuery);

/**
 * Callback function for Ooyala JavaScript API. Receives events for the player
 * and then dispatches them so that other modules can interact with the player.
 *
 * Use of this callback function is DEPRECATED. It only works with the Ooyala
 * V2 player. If your site is using the V3 player, see
 * Drupal.ooyala.onCreate().
 *
 * @see Drupal.ooyala.notifyListeners()
 */
function receiveOoyalaEvent(playerId, eventName, p) {
  Drupal.ooyala.notifyListeners(playerId, eventName, p);
  return;
}
