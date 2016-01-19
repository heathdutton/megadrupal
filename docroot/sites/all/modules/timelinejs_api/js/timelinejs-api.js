/**
 * Timeline JS API.
 */

(function ($) {

  window.timelinejs_api = {};

  Drupal.behaviors.timelineJsApi = {

    attach: function(context, settings) {
      var timeline_data_objects = Drupal.settings.timelinejs_api;

      for (var key in timeline_data_objects) {
        if (timeline_data_objects.hasOwnProperty(key)) {
          var timeline_data = timeline_data_objects[key];

          // Rather than pass the data directly to the TL.Timeline instance
          // (which works fine). The data is loaded here first so we can check
          // whether there are events or not. As initializing a timeline with no
          // events will just show a timeline-sized error message, with no real
          // good way to stop it. This behaviour only occurs if timelineJS is
          // AJAX loading the data. If data is passed in directly and is empty
          // (Not [events => []], but []) it will just not render.
          if (typeof timeline_data.data == 'string') {
            $.getJSON(timeline_data.data, function (data) {
              if (data.events.length > 0) {
                Drupal.behaviors.timelineJsApi.addTimeline(timeline_data.id, data);
              }
            });
          }
          // Otherwise, just using data from the settings directly is OK.
          else {
            Drupal.behaviors.timelineJsApi.addTimeline(timeline_data.id, timeline_data.data);
          }
        }
      }

      setTimeout(function() {
        // Trigger a resize event so timeline height calculations are correct.
        // This needs to run after everything else.
        window.dispatchEvent(new Event('resize'));
      }, 0);
    },

    /**
     * Add a timeline instance to the window object.
     *
     * @param id
     * @param data
     */
    addTimeline: function(id, data) {
      // Set the height of the timeline element before the timeline is
      // initialized. Otherwise the navbar and pagination controls are not
      // calculated correctly and are invisible.
      document.getElementById(id).style.height = '500px';

      var timeline_options = {
        scale_factor: 1
      };

      // Create a new timeline object instance.
      window.timelinejs_api[id] = new TL.Timeline(id, data, timeline_options);

      window.onresize = function(event) {
        window.timelinejs_api[id].updateDisplay();
      }
    }
  }

})(jQuery);
