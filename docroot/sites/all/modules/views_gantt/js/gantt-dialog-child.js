(function ($) {
  /**
   * Attach the child dialog behavior to new content.
   */
  Drupal.behaviors.GanttDialogChild = {
    attach: function(context, settings) {
      // Get the entity id and title from the settings provided by the views display.
      var entity_id = parseInt(settings.GanttDialog.entity_id);
      var entity_type = settings.GanttDialog.entity_type;
      var title = settings.GanttDialog.title;
      if (entity_id != null && entity_id != 0) {
        // Close the dialog by communicating with the parent.
        parent.Drupal.GanttDialog.close(entity_type, entity_id, title);
      }
    }
  }
})(jQuery);
