/**
 * @file
 * Supporting JS for the menu parent item form element.
 */

(function($) {
  Drupal.behaviors.menu_select = {
    attach: function(context, settings) {
      $('.form-item-menu-parent', context).once(function() {
        // Hide the menu parent and show the new selection fieldset.
        $(this).hide();
        $('#edit-menu-menu-select').removeClass('element-invisible');

        // Initial hierarchy build and selection if applicable.
        menu_select_build_menu_hierarchy($('#edit-menu-menu-select-select-menu-link-position-parent-menu').val());
        select_by_mkey($('#edit-menu-parent').val());

        // Build the menu hierarchy belonging to the menu selected.
        // Select the current parent in the hierarchy if it belongs to the menu selected.
        $('#edit-menu-menu-select-select-menu-link-position-parent-menu').change(function(e) {
          menu_select_build_menu_hierarchy($(this).val());
          select_by_mkey($('#edit-menu-parent').val(), true);
        });

        // Select the menu item parent from the autocomplete.
        $('#edit-menu-menu-select-select-menu-link-position-parent-menu-item-search').bind('autocompleteSelect', function() {
          select_by_mkey($(this).val());
        });
      });
    }
  }

  /**
   * Reveal any menu item in any menu given an mkey
   */
  function select_by_mkey(mkey, disable_menu_switch) {
    var mkey_parts = mkey.split(':');
    var root_parts = $('#edit-menu-menu-select-select-menu-link-position-parent-menu').val().split(':');

    if (disable_menu_switch) {
      if (mkey_parts[0] !== root_parts[0]) {
        return false;
      }
    }
    else {
      if (mkey_parts[0] !== root_parts[0]) {
        $('#edit-menu-menu-select-select-menu-link-position-parent-menu').val(mkey_parts[0] + ':0').change();
      }
    }

    // Select the parent.
    var selected = $('[data-mkey="' + mkey + '"]');
    selected.children('a').click();
    
    // Reveal all levels necessary.
    var parents = selected.parents('.has-children');
    
    for (var i = 0; i < parents.length; i++) {
      menu_select_toggle_level({}, $(parents[i]).children('span.toggle'), true);
    }
  }

  /**
   * Function to show/hide children.
   */
  function menu_select_toggle_level(e, level, reveal_only) {
    if (typeof(level) === 'undefined') {
      level = this;
    }

    // Reveal level if not already revealed, or else close it.
    if ($(level).children('.icon-caret-right').length > 0) {
      $(level).siblings('ul').show();
      $(level).children().removeClass().addClass('icon-caret-down-two');
    }
    else if (!reveal_only) {
      $(level).siblings('ul').hide();
      $(level).children().removeClass().addClass('icon-caret-right');
    }
  }

  /**
   * Build the menu hierarchy.
   */
  function menu_select_build_menu_hierarchy(root) {
    var map = Drupal.settings.menu_select.map;
    var menu = Drupal.settings.menu_select.menu[root];
    var output = _menu_select_hierarchy_recurse(menu, map);

    $('.menu-hierarchy').html('<div class="root" data-mkey="' + root + '"><i class="icon-caret-down-two"></i><a href="#" class="menu-parent-selection">' + Drupal.settings.menu_select.map[root] + '</a></div>' + output);
    
    if (Drupal.settings.menu_select.cut_off > -1 && $('.menu-hierarchy li').length > Drupal.settings.menu_select.cut_off) {
      $('.menu-hierarchy > ul').addClass('start-collapsed');
      $('.menu-hierarchy ul.start-collapsed li.has-children > .toggle i').removeClass().addClass('icon-caret-right');
    }

    $('.menu-parent-selection').click(function(e) {
      e.preventDefault();

      var label = $(this).text();
      var mkey = $(this).parent().attr('data-mkey');

      $('.form-item-menu-parent select').val(mkey);

      menu_select_build_position_preview(mkey);

      $('.menu-hierarchy .selected').removeClass('selected');
      $(this).addClass('selected');

      if ($(this).siblings('span.toggle').children('.icon-caret-right').length > 0) {
        $(this).siblings('span.toggle').click();
      }

      return false;
    });

    $('.has-children > .toggle').click(menu_select_toggle_level);
  }

  /**
   * Prepare the menu output.
   */
  function _menu_select_hierarchy_recurse(level, map) {
    var output = '';
    for (var mkey in level) {
      var has_children = (level[mkey].length !== 0);
      var classes = '';

      if (has_children) {
        classes += ' has-children';
      }

      output += '<li data-mkey="' + mkey + '" class="' + classes + '"><span class="toggle"><i class="icon-caret-down-two"></i></span><span class="item"><i class="icon-circle"></i></span><a href="#" class="menu-parent-selection">' + map[mkey] + '</a>';

      if (has_children) {
        output += _menu_select_hierarchy_recurse(level[mkey], map);
      }

      output += '</li>';
    }

    return '<ul>' + output + '</ul>';
  }

  /**
   * Build the menu position preview.
   */
  function menu_select_build_position_preview(mkey) {
    var map = Drupal.settings.menu_select.map;
    var menu_element = $('li[data-mkey="' + mkey + '"]');
    var parents = $(menu_element).parents('li');

    $('.parent-menu').text(Drupal.settings.menu_select.map[$('#edit-menu-menu-select-select-menu-link-position-parent-menu').val()]);

    $('.position-preview').html('');

    for (var i = parents.length - 1; i >= 0; i--) {
      $('.position-preview').append('<span>' + $(parents[i]).children('a').text() + '</span> / ');
    }

    if (menu_element.length === 1) {
      $('.position-preview').append('<span>' + menu_element.children('a').text() + '</span> / ');
    }
  }
})(jQuery);
