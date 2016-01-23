(function ($) {

    Drupal.behaviors.justified = {
        attach: function (context, settings) {
            'use strict';
            var instances = settings.justified.instances;

            var instance_id;
            for (instance_id in instances) {
                var instance = instances[instance_id];

                $('#' + instance.id).justifiedImages({
                    images : instance.images,
                    rowHeight: instance.row_height,
                    maxRowHeight: instance.max_row_height,
                    thumbnailPath: function(image, width, height){
                        return image.path;
                    },
                    getSize: function(image){
                        return {width: image.width, height: image.height};
                    },
                    margin: instance.margin
                });

            }

        }
    }

})(jQuery);
