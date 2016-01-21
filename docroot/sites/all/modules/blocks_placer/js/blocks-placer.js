
/**
 * @file
 * Javascript requirements for the blocks_placer module.
 */

(function($) {
  Drupal.behaviors.blocks_placer = {
    attach: function(context, settings) {

      // Run once.
      if ($('#edit-blocks-placer-blocks').is('.blocks-placer-processed')) return;
      $('#edit-blocks-placer-blocks').addClass('blocks-placer-processed');

      var text_replace_1 = Drupal.t('showing all');
      var text_replace_2 = Drupal.t('showing selected');
      var text_region_1  = Drupal.t('visible');
      var text_region_2  = Drupal.t('hidden');
      var input_type     = 'hidden'; // 'hidden'. Set to 'text' for debug.

      var $select   = $('#edit-blocks-placer-blocks .form-select', context);

      // Enable multiselect.
      $select.multiselect({
        dividerLocation: .5,
        width: '100%',
        height: 400,
        includeAddAll: 0
      });

      $selected = $('#edit-blocks-placer-blocks ul.connected-list.selected', context);
      $form = $select.closest('form');

      if (input_type == 'text') $select.show(); // Debug helper.

      // Get settings.
      var regions = Drupal.settings.blocks_placer.regions;
      var blocks  = Drupal.settings.blocks_placer.blocks;
      var hidden  = Drupal.settings.blocks_placer.hidden;

      // Add regions to the UI list.
      var regions_key;
      var link_replace = '<a title="Click to change visibility of blocks or region." class="replace" href="#">'+ text_replace_1 +'</a>';

      for (regions_key in regions) {
        if (regions.hasOwnProperty(regions_key)) {
          var $li = $('<li>')
              .addClass('ui-state-default ui-state-disabled')
              .attr('data-region', regions_key)
              .attr('data-replace', '0')
              .html(regions[regions_key] + link_replace);
          $selected.append($li);
        }
      }

      // Add region settings placeholder field.
      $regions = $('<input>').attr('type', input_type).attr('id', 'blocks_placer-regions').attr('name', 'blocks_placer-regions').addClass('form-text').appendTo($form).val(hidden);

      //--- Handle replace/action links.
      var $actionlink = $selected.find('a.replace');
      
      try {
        $actionlink.on('click', function(e) {
          e.stopPropagation(); e.preventDefault();
          updateActionLink.apply(this);
        });
      }
      catch(e) { // Use .live() as < 1.7 fallback.
        $actionlink.live('click', function(e) {
          e.stopPropagation(); e.preventDefault();
          updateActionLink.apply(this);
        });
      }

      function updateActionLink() {
        var val;
        var text = $(this).text();
        var region = $(this).closest('li').attr('data-region');

        switch(text) {
          case text_replace_1:
            text = text_replace_2;
            val = 1;
            break;
          case text_region_1:
            text = text_region_2;
            val = 1;
            break;
          case text_replace_2:
            text = text_replace_1;
            val = 0;
            break;
          case text_region_2:
            text = text_region_1;
            val = 0;
            break;
          default:
          // none
        }

        $(this).text(text);

        if (val) {
          $regions.val($regions.val() + '|' + region + '|');
        } else {
          $regions.val($regions.val().replace('|' + region + '|', ''));
        }

        $select.trigger('change');
      }


      // Handle region settings for block UI.
      $select.change(function() {
        var $field;
        var region = regions_key;
        var value, newval, selected_li_text, field_id, replace;

        // Wait for DOM changes.
        setTimeout(function() {

          // Iterate ul list.
          $selected.children().each(function() {

            // Check if this is a region.
            if ($(this).is('.ui-state-disabled')) {
              region  = $(this).data('region');
              replace = $regions.val().indexOf('|' + region + '|');
              $link   = $(this).find('a.replace');
              var text = '';

              // Check if this region contain blocks.
              if ($(this).next().is('.ui-element')) {
                if (replace >= 0) {
                  text = text_replace_2;
                } else {
                  text = text_replace_1;
                }
              }
              // No blocks in this region.
              else {
                if (replace >= 0) {
                  text = text_region_2;
                } else {
                  text = text_region_1;
                }
              }

              $link.text(text); // Set link text.

            }
            // Check if this is a selected option.
            else if ($(this).is('.ui-state-default')) {
              // Match item by text (block title).
              selected_li_text = htmlEscape($(this).attr('title'));

              var $option;
              $select.children().each(function() {
                if ($(this).text() == selected_li_text) {
                  $option = $(this);
                  return false;
                }
              });

              if ($option !== undefined) {
                value  = $option.attr('value');
                newval = value +','+ region;

                // Add hidden field.
                field_id = 'blocks_placer-block-'+ value;
                $field = $('#'+ field_id);

                // Update or create field.
                if ($field.length) {
                  $field.attr('value', newval);
                }
                else {
                  $('<input>').attr('type', input_type).attr('id', field_id).attr('name', field_id).addClass('form-text').appendTo($form).val(newval);
                }

              }

            }

          });
        }, 250);
      });


      // Handle current selected blocks (post change function).
      // Order is reversed so .after() inserts in correct order.
      $select.find('option:selected').reverse().each(function() {
        var this_id  = $(this).attr('value');
        var this_txt = $(this).text();
        var region   = '';

        if (typeof blocks[this_id] != 'undefined') {
          region = blocks[this_id]['region'];
        }

        // Iterate selected elements and
        // put the matching li in the correct region.
        $selected.find('li.ui-element').each(function() {
          if (htmlEscape($(this).text()) == this_txt) {
            $selected.find('[data-region="' + region + '"]').after($(this));
            return false;
          }
        });

      });

      // Trigger initial change behavior.
      $select.trigger('change');


      // Make regions non-draggable.
      $selected.sortable({
        cancel: '.ui-state-disabled'
      });

    }
  }
})(jQuery);


/**
 * Tiny jQuery plugin for .reverse().
 */
jQuery.fn.reverse = function() {
  return this.pushStack(this.get().reverse(), arguments);
};


/**
 * HTML escapes a string.
 *
 * @param str str
 * @returns An HTML escaped string.
 */
function htmlEscape(str) {
  return String(str)
    .replace(/&/g, '&amp;')
    .replace(/"/g, '&quot;')
    .replace(/'/g, '&#39;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
  ;
}
