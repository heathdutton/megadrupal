(function ($) {
  Drupal.behaviors.TaxonomyCopier = {
    attach: function () {
      $(document).ready(function(){
        $('input[name*="-nodes"][value="custom"]').attr('disabled', true);
        $('input[name*="-node-type-"][value="custom"]').attr('disabled', true);
      });
      $('input[type="checkbox"]').change(function(){
        var tid = $(this).attr('name').split('-')[0];
        if ($(this).attr('checked') == true) {
          var parent = $('input[name="' + tid + '-parent"]').val();
          if (parent > 0) {
            // Check parent terms.
            if ($('input[type="checkbox"][name="' + parent + '-option"]').attr('checked') == false) {
              $('input[type="checkbox"][name="' + parent + '-option"]').trigger('click');
            }
          }
        }
        else {
          if ($('div[name="' + tid + '-children"]').length > 0) {
            // Uncheck child terms.
            $('div[name="' + tid + '-children"] input[name="tid"]').each(function(){
              var child = $(this).val();
              if ($('input[type="checkbox"][name="' + child + '-option"]').attr('checked') == true) {
                $('input[type="checkbox"][name="' + child + '-option"]').trigger('click');
              }
            });
          }
        }
      });
      $('input[type="radio"]').change(function(){
        var option_type = $(this).attr('name').split('-');
        var value = $(this).val();
        var tid = option_type[0];
        // Changing option for all nodes.
        if (option_type[1] == 'nodes') {
          if ($('div[name="' + tid + '-nodes-list"] table tr').length > 0 && value.search('custom') < 0) {
            // Disable "Custom" button.
            if ($('input[name*="' + tid + '-nodes"][value="custom"]').attr('disabled') == false) {
              $('input[name*="' + tid + '-nodes"][value="custom"]').attr('disabled', true);
            }
            // Set the same value for all node types.
            $('input[name*="' + tid + '-node-type"][value="' + value + '"]').trigger('click');
          }
        }
        else {
          var type;
          var custom;
          // Changing option for node type.
          if (option_type[2] == 'type') {
            type = option_type[3];
            if (value.search('custom') < 0) {
              // Disable "Custom" button.
              if ($('input[name*="' + tid + '-node-type-' + type + '-option"][value="custom"]').attr('disabled') == false) {
                $('input[name*="' + tid + '-node-type-' + type + '-option"][value="custom"]').attr('disabled', true);
              }
              // Set value for all nodes of selected type.
              $('input.' + type + '[name*="' + tid + '-node"][value="' + value + '"]').each(function(){
                $(this).trigger('click');
              })
              // Set all nodes value.
              if ($('input[name*="' + tid + '-node-type"][value="' + value + '"]:not([name*="' + type + '"])').length > 0) {
                custom = false;
                $('input[name*="' + tid + '-node-type"][value="' + value + '"]:not([name*="' + type + '"])').each(function(){
                  if ($(this).attr('checked') == false) {
                    custom = true;
                  }
                });
                if (custom) {
                  $('input[name*="' + tid + '-nodes"][value="custom"]').attr('disabled', false);
                  $('input[name*="' + tid + '-nodes"][value="custom"]').trigger('click');
                }
                else {
                  $('input[name*="' + tid + '-nodes"][value="' + value + '"]').trigger('click');
                }
              }
              else {
                $('input[name*="' + tid + '-nodes"][value="' + value + '"]').trigger('click');
              }
            }
            else {
              $('input[name*="' + tid + '-nodes"][value="custom"]').attr('disabled', false);
              $('input[name*="' + tid + '-nodes"][value="custom"]').trigger('click');
            }
          }
          // Changing option for single node.
          else {
            type = option_type[3];
            var nid = option_type[2];
            custom = false;
            $('input.' + type + '[name*="' + tid + '-node"][value="' + value + '"]:not([name*="node-' + nid + '"])').each(function(){
              if ($(this).attr('checked') == false) {
                custom = true;
              }
            });
            // Set value for node type.
            if (custom) {
              $('input[name="' + tid + '-node-type-' + type + '-option"][value="custom"]').attr('disabled', false);
              $('input[name="' + tid + '-node-type-' + type + '-option"][value="custom"]').trigger('click');
            }
            else {
              $('input[name="' + tid + '-node-type-' + type + '-option"][value="' + value + '"]').trigger('click');
            }
          }
        }
      });
    }
  }
})(jQuery);
