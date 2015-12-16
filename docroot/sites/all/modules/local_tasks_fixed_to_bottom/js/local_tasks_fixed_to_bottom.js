/**
 * @file
 * Append the html from variable to child of body tag.
 */

(function ($) {
  $(Drupal.settings.localTasksFixedToBottom.tasksHTML).appendTo('body');
})(jQuery);
