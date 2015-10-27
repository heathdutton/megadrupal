/**
 * @file
 * Handles the operation of the template remove button on the node edit form.
 */

(function($) {
  Drupal.behaviors.template_field = {
    /**
     * Bind additional events to the remove template buttons.
     * @param context
     * @param settings
     */
    attach: function (context, settings) {
      // Find all of the remove template buttons.
      remove_buttons = $('td.template-field-operations input.template-field-remove');

      // Prevents multiple bindings of this event handler
      // to the elements after an AJAX call.
      remove_buttons.unbind('mousedown.template_field');

      // Bind a new mousedown handler to each button and make it the first
      // handler to be processed.
      // We bind to mousedown because the Drupal AJAX framework also binds
      // to mousedown and disables the click event.
      remove_buttons.each(function() {
        $(this).bind('mousedown.template_field', Drupal.behaviors.template_field.remove_template);
        Drupal.behaviors.template_field.reorder_events(this, 'mousedown');
      });
    },

    /**
     * Handle the remove template event trigger.
     * @param event
     */
    remove_template: function (event) {
      if (event.isDefaultPrevented()) {
        return false;
      }
      var row = $(this).parents('tr');

      var template_title_obj = row.find('fieldset a.fieldset-title').clone();
      template_title_obj.find('.fieldset-legend-prefix').remove();
      var template_title = template_title_obj.text();

      var params = {
        '!name' : template_title
      };

      var confirm_message = 'Are you sure you wish to remove template "!name" ?';
      confirm_message += '\n\nClick OK to remove template.';
      confirm_message += '\nClick Cancel to go back.';
      confirm_message = Drupal.t(confirm_message, params);
      var ret = confirm(confirm_message);

      if (ret) {
        // OK Button was pressed, allow the AJAX callback.
        row.find('.template-field-removed-flag input').val(1);
      }
      else {
        // Cancel button was pressed, stop all further event handling.
        event.stopImmediatePropagation();
      }

      event.preventDefault();
    },

    /**
     * Moves our event handler from the end of the handlers array
     * to the beginning so that we can display the confirmation prompt
     * before the ajax request starts.
     *
     * This routine is based the article by Alexander Shchekoldin at
     * http://shchekoldin.com/2010/11/18/reorder-jquery-event-handlers/#comment-2045
     */
    reorder_events: function(obj, event) {
      elementData = $.data(obj);
      if (elementData.events) {
        // For jQuery 1.4.x
        events = elementData.events;
      }
      else {
        // For jQuery 1.5.x, the data() element returns a container named
        // "jQuery" + a number (ex: jQuery15204337786921098282)
        for (key in elementData) {
          events = elementData[key]['events'];
          break;
        }
      }
      handlers = events[event];

      if (handlers.length == 1) {
        return;
      }
      else {
        handlers.splice(0, 0, handlers.pop());
      }
    }
  }
})(jQuery);
