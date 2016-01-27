(function ($) {

Drupal.behaviors.relatedlinks = {
  attach: function() {

  var autocomplete_path = Drupal.settings.relatedlinks_autocomplete[0];
  // Strip relatedlinks/autocomplete from the path.
  var path_prefix = autocomplete_path.substring(0, autocomplete_path.length - 25);

  // Trigger Drupal's autocomplete.
  Drupal.behaviors.autocomplete;

  var ta = $('#edit-relatedlinks-fieldset-relatedlinks');
  $(ta).parents('div.form-textarea-wrapper').css({'display' : 'none'});
  var top = $(ta).parents('fieldset');

  // Parse textarea for links and store them in a global array.
  var l = $(ta).val().split("\n");
  var r = /([^\s]+)\s+(.*)$/i;
  links = new Array();
  for (var i = 0; i < l.length; i++) {
    m = l[i].match(r);
    if (m) {
      link_set(m[1], m[2], i);
    }
  }

  // Insert URL and title textfields.
  var fields = $('<div class="relatedlinks clear-block">' +
    '<div id="relatedlinks-preview"></div>' +
    '<div class="relatedlinks-title">' +
      '<div class="form-item">' +
        '<label for="field-relatedlinks-title">Title:</label>' +
        '<input id="field-relatedlinks-title" class="form-text form-autocomplete" type="text" value="" size="60" name="relatedlinks-title" maxlength="128" />' +
        '<input class="autocomplete" type="hidden" id="field-relatedlinks-title-autocomplete" value="' + autocomplete_path + '" disabled="disabled" />' +
      '</div>' +
    '</div>' +
    '<div class="relatedlinks-url">' +
      '<div class="form-item">' +
        '<label for="field-relatedlinks-url">URL:</label>' +
        '<input id="field-relatedlinks-url" class="form-text" type="text" value="" size="60" name="relatedlinks-url" maxlength="128" />' +
      '</div>' +
    '</div></div>')
    .appendTo(top);

  var preview = $('#relatedlinks-preview', fields);
  links_preview();

  // -1 signifies that no link is being edited at the moment.
  var current = -1;

  $('<a id="relatedlinks-add" href="#">' + Drupal.t('Add / Update') + '</a>')
    .appendTo(fields)
    .click(function() {
      url = $('#field-relatedlinks-url', fields).val();
      if (url.length) {
        if (current == -1) {
          link_set(url, $('#field-relatedlinks-title', fields).val(), links.length);
        }
        else {
          link_set(url, $('#field-relatedlinks-title', fields).val(), current);
        }
        $('#field-relatedlinks-title', fields).val('');
        $('#field-relatedlinks-url', fields).val('');
      }
      links_preview();
      return false;
    }
  );

  $('<a id="relatedlinks-clear" href="#">' + Drupal.t('Clear') + '</a>')
    .appendTo(fields)
    .click(function() {
      current = -1;
      $('#field-relatedlinks-url', fields).val('');
      $('#field-relatedlinks-title', fields).val('');
      return false;
    }
  );

  function links_preview() {
    // Remove the preview list. This function is called after every event and
    // the list is just recreated every time based on the textarea input.
    $('table', preview).remove();
    table = $('<table><tbody><th>ID</th><th>' + Drupal.t('Link') + '</th><th colspan="4">' + Drupal.t('Operations') + '</th></tbody></table>').appendTo(preview);

    l = '';
    for (var i = 0; i < links.length; i++) {
      // Create textarea input.
      l += links[i].url + ' ' + links[i].title + "\n";

      var url = url_sanitise(links[i].url);
      // Trim and check if title is available; else use the URL as the title.
      links[i].title = $.trim(links[i].title);
      title = links[i].title.length ? links[i].title : url;

      // Add edit button.
      actions = '<td class="relatedlinks-action relatedlinks-edit"><a title="' + Drupal.t('Edit this link') + '">' + Drupal.t('Edit') + '</a></td>';
      // Add delete button.
      actions += '<td class="relatedlinks-action relatedlinks-delete"><a title="' + Drupal.t('Delete this link') + '">' + Drupal.t('Delete') + '</a></td>';

      // If available, add the up button.
      if (i > 0) {
        actions += '<td class="relatedlinks-action relatedlinks-up"><a title="' + Drupal.t('Move this link one row up') + '">' + Drupal.t('Up') + '</a></td>';
      }
      else {
        actions += '<td class="relatedlinks-action relatedlinks-up"></td>';
      }
      // If available, add the down button.
      if (i < links.length - 1) {
        actions += '<td class="relatedlinks-action relatedlinks-down"><a title="' + Drupal.t('Move this link one row down') + '">' + Drupal.t('Down') + '</a></td>';
      }
      else {
        actions += '<td class="relatedlinks-action relatedlinks-down"></td>';
      }

      // Insert action buttons.
      $('<tr><td>' + (i + 1) + '</td><td class="relatedlinks-link"><a href="' + url + '">' + title + '</a>' + actions + '</td></tr>')
        .appendTo(table);
    }

    // Handle edit button clicks: populate URL and title fields with clicked
    // link value.
    $('.relatedlinks-edit', table).each(function(k) {
      $(this).click(function() {
        // Track the currently edited link ID.
        current = k;
        $('#field-relatedlinks-url', fields).val(links[current].url);
        $('#field-relatedlinks-title', fields).val(links[current].title);
        return false;
      });
    });

    // Handle delete button clicks. Remove clicked link from the relatedlinks
    // textfield.
    $('.relatedlinks-delete', table).each(function(k) {
      $(this).click(function() {
        links_delete(k);
        links_preview();
        return false;
      });
    });

    // Handle up clicks. Move clicked link up one row.
    $('.relatedlinks-up', table).each(function(k) {
      $('a', this).click(function() {
        links_swap(k, k - 1);
        links_preview();
        return false;
      });
    });

    // Handle down clicks. Move clicked link down one row.
    $('.relatedlinks-down', table).each(function(k) {
      $('a', this).click(function() {
        links_swap(k, k + 1);
        links_preview();
        return false;
      });
    });

    // Update relatedlinks textarea.
    $(ta).val(l);
  }

  // Insert/update link input into global links array.
  function link_set(link, text, i) {
    links[i] = {url: $.trim(link), title: $.trim(text)};
  }

  // Delete link from global links array.
  function links_delete(k) {
    for (var i = k; i < links.length - 1; i++) {
      links[i] = links[i + 1];
    }
    links.pop();
    current = -1;
  }

  // Swap link orders.
  function links_swap(j, k) {
    if (k >= 0 && k < links.length) {
      var t = links[j];
      links[j] = links[k];
      links[k] = t;
    }
  }

  function url_sanitise(url) {
    url = $.trim(url);
    var r = new RegExp("^\\w*://");
    var matches = url.match(r);
    if (!matches) {
      url = path_prefix + url;
    } 

    return url;
  }

  /**
   * "Override" Drupal.jsAC.prototype.select from autocomplete.js to allow
   * other fields to be updated.
   */
  Drupal.jsAC.prototype.select = function (node) {
    if (this.input.id != 'field-relatedlinks-title') {
      this.input.value = $(node).data('autocompleteValue');
    }
    else {
      $('#field-relatedlinks-url').val('node/' + $(node).data('autocompleteValue'));
      // Title field is set in .hidePopup().
    }
  };

  /**
   * Hides the autocomplete suggestions.
   */
  Drupal.jsAC.prototype.hidePopup = function (keycode) {
    // Select item if the right key or mousebutton was pressed
    if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
      if (this.input.id == 'field-relatedlinks-title') {
        var title = this.db.cache[this.input.value][$(this.selected).data('autocompleteValue')];
        $('#field-relatedlinks-url').val('node/' + $(this.selected).data('autocompleteValue'));
        // Replace with title instead of nid.
        this.selected.autocompleteValue = title;
      }

      this.input.value = this.selected.autocompleteValue;
    }
    // Hide popup
    var popup = this.popup;
    if (popup) {
      this.popup = null;
      $(popup).fadeOut('fast', function() { $(popup).remove(); });
    }
    this.selected = false;
  };
}
};
}(jQuery));