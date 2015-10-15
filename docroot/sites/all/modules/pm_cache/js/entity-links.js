(function ($) {

  Drupal.behaviors.pmCacheEntityLinks = {
    attach: function (context, settings) {
      // Links are rendered on the server side for anonymous users.
      if (settings.pmCache.isAnonymous) {
        return;
      }
      var data = {};
      var new_data = false;
      $('.entity-links:not(.processed)', context).each(function() {
        var entity_type = $(this).attr('data-entity-type');
        var entity_id = parseInt($(this).attr('data-entity-id'));
        var view_mode = $(this).attr('data-view-mode');
        if (typeof data[entity_type] === 'undefined') {
          data[entity_type] = {};
        }
        if (typeof data[entity_type][entity_id] === 'undefined') {
          data[entity_type][entity_id] = [];
        }
        data[entity_type][entity_id].push(view_mode);
        new_data = true;
      });
      if (new_data) {
        $.ajax({
          type: 'POST',
          url: settings.pathPrefix + '/pm-cache/entity-links',
          data: data,
          dataType: 'json',
          success: function(data) {
            $('.entity-links:not(.processed)', context).each(function() {
              var entity_type = $(this).attr('data-entity-type');
              var entity_id = parseInt($(this).attr('data-entity-id'));
              var view_mode = $(this).attr('data-view-mode');
              if (typeof data[entity_type][entity_id][view_mode] !== 'undefined') {
                var output = data[entity_type][entity_id][view_mode];
                $(this).html(output).addClass('processed');
                Drupal.attachBehaviors(this, settings);
              }
            });
          }
        });
      }
    }
  };

})(jQuery);
