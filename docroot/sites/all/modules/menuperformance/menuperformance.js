(function ($) {

Drupal.behaviors.menuperformance = {
  attach: function () {
    $('.menuperformance_container').once('menuperformance')
      .delegate('.menuperformance_node_menu_parent_menu', 'change', function () {
        var original = $('.menuperformance_node_menu_value').val();
        var menu = original.substring(0, original.indexOf(':'));
        var new_menu = $(this).val();
        if(new_menu != '0') {
          $('.menuperformance_node_menu_value').val(new_menu + ':0');
          // Get menu root level elements
          var container = $(this).parent();
          container.append('<img src="' +
            Drupal.settings.menuperformance.base_url + '/' + Drupal.settings.menuperformance.module_path +
            '/ajax-loader.gif" width="16" height="16" />');
          $.get(Drupal.settings.menuperformance.base_url + '/' + "_menuperformance/ajax/get_link_children/" + new_menu + '/0', function(data){
            // If there were no children, don't show the select.
            if(!$.isEmptyObject(data)) {
              $('.menuperformance_node_menu_parent_link').parent().remove();
              // Create select, add options

              $('.menuperformance_container').append('<div class="form-item form-type-select"><select id="edit-menu-menuperformance-node-menu-parent-link-0" class="form-select menuperformance_node_menu_parent_link"></select></div>');

              // Add data about the select element's index
              $('#edit-menu-menuperformance-node-menu-parent-link-0').data('mpi', '0');
              $('#edit-menu-menuperformance-node-menu-parent-link-0').append('<option value="0">[select]</option>');
              for (var key in data) {
                if (data.hasOwnProperty(key)) {
                  var obj = data[key];
                  if(obj.mlid != Drupal.settings.menuperformance.current_mlid) {
                    $('#edit-menu-menuperformance-node-menu-parent-link-0').append('<option value="' + obj.mlid + '">' + obj.link_title + '</option>');
                  }
                }
              }
            }
            else {
              $('#edit-menu-menuperformance-node-menu-parent-link-0').parent().remove();
            }
            container.find('img').remove();
          }, "json");
        } // end if new_menu
        else {
          // Note that empty menu selection is only available on the taxonomy
          // vocabulary page.
          $('.menuperformance_node_menu_value').val('0');
          $('.menuperformance_node_menu_parent_link').remove();
        }
      })
      .delegate('.menuperformance_node_menu_parent_link', 'change', function () {
        var new_link = parseInt($(this).val());
        var my_i = $(this).data('mpi');

        // Remove link selects for levels deeper than this one:
        $('.menuperformance_node_menu_parent_link').each(function(){
          var mpi = $(this).data('mpi');
          if(mpi > my_i) {
            $(this).parent().remove();
          }
        });

        // If empty value was selected, don't get children.
        if(new_link) {
          var original = $('.menuperformance_node_menu_value').val();
          var menu = original.substring(0, original.indexOf(':'));
          $('.menuperformance_node_menu_value').val(menu + ':' + new_link);

          // Get next level elements
          var container = $(this).parent();
          container.append('<img src="' +
            Drupal.settings.menuperformance.base_url + '/' + Drupal.settings.menuperformance.module_path +
            '/ajax-loader.gif" width="16" height="16" />');

          $.get(Drupal.settings.menuperformance.base_url + '/' + "_menuperformance/ajax/get_link_children/" + menu + '/' + new_link, function(data){
            // If there were no children, don't show the select.
            if(!$.isEmptyObject(data)) {
              var new_i = 1 + parseInt(my_i);

              // Create select, add options
              $('.menuperformance_container').append('<div class="form-item form-type-select"><select id="edit-menu-menuperformance-node-menu-parent-link-' + new_i + '" class="form-select menuperformance_node_menu_parent_link"></select></div>');

              $('#edit-menu-menuperformance-node-menu-parent-link-' + new_i).append('<option value="0">[select]</option>');

              // Add data about the select element's index
              $('#edit-menu-menuperformance-node-menu-parent-link-' + new_i).data('mpi', new_i);

              for (var key in data) {
                if (data.hasOwnProperty(key)) {
                  var obj = data[key];
                  if(obj.mlid != Drupal.settings.menuperformance.current_mlid) {
                    $('#edit-menu-menuperformance-node-menu-parent-link-' + new_i).append('<option value="' + obj.mlid + '">' + obj.link_title + '</option>');
                  }
                }
              }
            }
            container.find('img').remove();
          }, "json");
          // end get
        }
        // end if empty selected
        else {
          // Handle the case where the user selects the empty value.
          if(my_i == '0') {
            $('.menuperformance_node_menu_parent_menu').change();
          }
          else {
            var selector = '#edit-menu-menuperformance-node-menu-parent-link-' + (my_i - 1);
            $(selector).change();
          }
        }
      });
      // end delegate

      $('.menuperformance_node_menu_parent_link form-select').each(function(){
        $(this).data('mpi', $(this).attr('data-mpi'));
      });
    } // end attach
  }; // end behaviors
}(jQuery));
