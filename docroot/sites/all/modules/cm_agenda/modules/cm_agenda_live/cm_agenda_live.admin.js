(function($) {
  var cm_agenda_live_admin_position;
  Drupal.behaviors.cm_agenda_live_admin = {
    attach : function(context, settings) {
      cm_agenda_live_admin_position = cm_agenda_live_admin_position || 0;
      Drupal.ajax['edit-submit-top'].options.beforeSubmit = function(form_values, element, options) {
        cm_agenda_live_admin_position = $(".cm-agenda-chapter-container")[0].scrollTop;
        return true;
      };
      $(".cm-agenda-chapter-container")[0].scrollTop = cm_agenda_live_admin_position;
    }
  };
})(jQuery);
