/**
 * @file
 * Entity Complete Field Groups - Provides entity complete support for field groups - javascript.
 */

//*
(function($) {

Drupal.behaviors.fc_field_groups  = {
  attach : function(context, settings) {
    for (var id in settings.fc_field_groups) {
      $('#' + id, context)
      .once('fc-field-groups', function(){
        var form = $(this);
        var formSettings = settings.fc_field_groups[id];
        for (var g in formSettings) {
          var vtab = formSettings[g];
          form.find('.vertical-tabs-list a:contains(' + vtab.text + ')')
            .addClass('fc-field-groups')
            .addClass('fc-field-groups-' + (vtab.state?'complete':'incomplete'));
        }
      });
    }
  }
};
  
})(jQuery);

