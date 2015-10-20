(function ($) {

  Drupal.behaviors.jcrop_fapi_crop = {
    attach: function (context, settings) {

      $(".cropbox").one('load', function() {
        attachJcrop(context);
      }).each(function() {
        // Process cached images.
        if (this.complete) $(this).load();
      });

      function update_crop_fields(widget, c) {
        // Crop image even if user has left image untouched.
        widget = $(widget);
        widget.siblings('.preview-existing').css({display: 'none'});
        widget.siblings(".edit-image-crop-x").val(c.x);
        widget.siblings(".edit-image-crop-y").val(c.y);
        if (c.w) widget.siblings(".edit-image-crop-width").val(c.w);
        if (c.h) widget.siblings(".edit-image-crop-height").val(c.h);
        widget.siblings(".edit-image-crop-changed").val(1);
      }

      function attachJcrop(context) {
        if ($('.cropbox', context).length == 0) {
          return;
        }
        // add Jcrop exactly once to each cropbox
        $('.cropbox', context).each(function() {

          var self = $(this);

          self.once(function() {
            var self_id = self.attr('id');
            var id = self_id.substring(0, self_id.indexOf('-cropbox'));
            var widget = self.parent();

            // Create Jcrop object.
            $(this).Jcrop({
              onChange: function(c) {
                // Crop image even if user has left image untouched.
                update_crop_fields(widget, c);
              },
              onSelect: function(c) {
                update_crop_fields(widget, c);
              },
              aspectRatio: settings.jcrop_fapi_crop[id].box.ratio,
              boxWidth: settings.jcrop_fapi_crop[id].box.box_width,
              boxHeight: settings.jcrop_fapi_crop[id].box.box_height,
              minSize: [settings.jcrop_fapi_crop[id].minimum.width, settings.jcrop_fapi_crop[id].minimum.height],
              trueSize: [
                settings.jcrop_fapi_crop[id].width,
                settings.jcrop_fapi_crop[id].height,
              ],
              allowSelect: false,
              setSelect: [
                parseInt(widget.siblings(".edit-image-crop-x").val()),
                parseInt(widget.siblings(".edit-image-crop-y").val()),
                parseInt(widget.siblings(".edit-image-crop-width").val()) + parseInt($(widget).siblings(".edit-image-crop-x").val()),
                parseInt(widget.siblings(".edit-image-crop-height").val()) + parseInt($(widget).siblings(".edit-image-crop-y").val())
              ]
            });
          });
        });
      };
    }
  };

})(jQuery);
