/**
 * @file
 * JavaScript Code for using mediaCheck plug-in.
 */

(function($) {
    Drupal.behaviors.responsive_blocks = {
        attach: function(context, settings) {
            /*
             *  Setup Relationship between blocks. Create array to work with mediaCheck library
             */
            var responsive_blocks = convert(settings.responsive_blocks.responsive_blocks);
			//console.log(responsive_blocks);
            var device_width = convert(settings.responsive_blocks.device_width);
            var desktop = device_width['responsive_blocks_desktop'] + 'px';
            var ipad = device_width['responsive_blocks_ipad'] + 'px';
            var mobile = device_width['responsive_blocks_mobile'] + 'px';
            var block_name;
            var desktop_blocks = [];
            var ipad_blocks = [];
            var mobile_blocks = [];
			var desktop_min = parseInt(device_width['responsive_blocks_ipad']) + 1;
			var ipad_min = parseInt(device_width['responsive_blocks_mobile']) + 1;;
			desktop_min = desktop_min + 'px';
			ipad_min = ipad_min + 'px';
            for (var key in responsive_blocks) {
                block_name = key.split("-");
                var block = block_name[block_name.length - 1];
                block = key.replace("-" + block, '');
                if (responsive_blocks[key] == 'desktop') {
                    desktop_blocks.push(block);
                }
                if (responsive_blocks[key] == 'ipad') {
                    ipad_blocks.push(block);
                }
                if (responsive_blocks[key] == 'mobile') {
                    mobile_blocks.push(block);
                }
                $('#' + block).hide();
            }

            /*
             *  mediaCheck : For Desktop resolution
             */
            mediaCheck({
                media: '(min-width: ' + desktop_min + ') and (max-width: ' + desktop + ')',
                entry: function() {
                    for (var key in desktop_blocks) {
                        $('#' + desktop_blocks[key]).show();
                    }
                },
                exit: function() {
                    for (var key in desktop_blocks) {
                        $('#' + desktop_blocks[key]).hide();
                    }
                }
            });

            /*
             *  mediaCheck : For IPAD resolution
             */
            mediaCheck({
                media: '(min-width: ' + ipad_min + ') and (max-width: ' + ipad + ')',
                entry: function() {
                    for (var key in ipad_blocks) {
                        $('#' + ipad_blocks[key]).show();
                    }
                },
                exit: function() {
                    for (var key in ipad_blocks) {
                        $('#' + ipad_blocks[key]).hide();
                    }
                }
            });

            /*
             *  mediaCheck : For Mobile resolution
             */
            mediaCheck({
                media: '(max-width: ' + mobile + ')',
                entry: function() {
                    for (var key in mobile_blocks) {
                        $('#' + mobile_blocks[key]).show();
                    }
                },
                exit: function() {
                    for (var key in mobile_blocks) {
                        $('#' + mobile_blocks[key]).hide();
                    }
                }
            });

        }
    };
})(jQuery);

/*
 * Convert Object to array : for easy accessibility.
 *
 */
function convert(obj) {
  var vals = [];

  for (var key in obj) {
    if (obj.hasOwnProperty(key)) {
      vals[key] = obj[key];
    }
  }

  return vals;
}
