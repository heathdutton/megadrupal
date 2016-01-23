/**
 * @file
 * Custom autocomplete implementation.
 *
 * We use Drupal's autocomplete functionality for suggesting terms, but Drupal's
 * implementation only allows us to pass a search string, whereas we need to
 * pass filter information as well. Re-implement the autocomplete component,
 * extending the existing Drupal code.
 */

(function ($) {

DsbPortalAutocomplete = {};

/**
 * An AutoComplete object. We extend the core one.
 *
 * The constructor is a verbatim copy of the core one.
 */
DsbPortalAutocomplete.jsAC = function ($input, db) {
  var ac = this;
  this.input = $input[0];
  this.ariaLive = $('#' + this.input.id + '-autocomplete-aria-live');
  this.db = db;

  $input
    .keydown(function (event) { return ac.onkeydown(this, event); })
    .keyup(function (event) { ac.onkeyup(this, event); })
    .blur(function () { ac.hidePopup(); ac.db.cancel(); });
};

DsbPortalAutocomplete.jsAC.prototype = $.extend(true, {}, Drupal.jsAC.prototype);

/**
 * @{inheritDoc}
 *
 * We override the populate method, because we want to pass more information
 * than just the search string. Most of the code is a verbatim copy of
 * Drupal.jsAC.prototype.populatePopup().
 */
DsbPortalAutocomplete.jsAC.prototype.populatePopup = function () {
  var $input = $(this.input);
  var position = $input.position();
  // Show popup.
  if (this.popup) {
    $(this.popup).remove();
  }
  this.selected = false;
  this.popup = $('<div id="autocomplete"></div>')[0];
  this.popup.owner = this;
  $(this.popup).css({
    top: parseInt(position.top + this.input.offsetHeight, 10) + 'px',
    left: parseInt(position.left, 10) + 'px',
    width: $input.innerWidth() + 'px',
    display: 'none'
  });
  $input.before(this.popup);

  // Fetch any active filters.
  var filters = {},
      $input = $(this.input);

  // Does our input have a specific target for the "parent" form, which
  // contains the filters? If so, use it. If not, simply use the form in which
  // this input is defined.
  var $form = $input.attr('dsb-portal-autocomplete-form') ?
    $($input.attr('dsb-portal-autocomplete-form')) :
    $input.parents('form');

  $.each($form.serializeArray(), function(_, item) {
    // Only treat items that start with the name "filter".
    if (/^filter/.test(item.name)) {
      if (filters.hasOwnProperty(item.name)) {
        filters[item.name] = $.makeArray(filters[item.name]);
        filters[item.name].push(item.value);
      }
      else {
        filters[item.name] = item.value;
      }
    }
  });

  // Do search.
  this.db.owner = this;
  this.db.search(this.input.value, filters);
};

/**
 * An AutoComplete DataBase object. We extend the core one.
 *
 * The constructor is a verbatim copy of the core one.
 */
DsbPortalAutocomplete.ACDB = function (uri) {
  this.uri = uri;
  this.delay = 300;
  this.cache = {};
};

DsbPortalAutocomplete.ACDB.prototype = $.extend(true, {}, Drupal.ACDB.prototype);

/**
 * @{inheritDoc}
 *
 * We override the search method, because we want to pass more information
 * than just the search string. Most of the code is a verbatim copy of
 * Drupal.ACDB.prototype.search().
 */
DsbPortalAutocomplete.ACDB.prototype.search = function (searchString, filters) {
  var db = this;
  this.searchString = searchString;

  // See if this string needs to be searched for anyway.
  searchString = searchString.replace(/^\s+|\s+$/, '');
  if (searchString.length <= 0 ||
    searchString.charAt(searchString.length - 1) == ',') {
    return;
  }

  // See if this key has been searched for before.
  if (this.cache[searchString]) {
    return this.owner.found(this.cache[searchString]);
  }

  // Initiate delayed search.
  if (this.timer) {
    clearTimeout(this.timer);
  }
  this.timer = setTimeout(function () {
    db.owner.setStatus('begin');

    // Ajax GET request for autocompletion. We use Drupal.encodePath instead of
    // encodeURIComponent to allow autocomplete search terms to contain slashes.
    $.ajax({
      type: 'GET',
      url: db.uri + '/' + Drupal.encodePath(searchString),
      data: filters,
      dataType: 'json',
      success: function (matches) {
        if (typeof matches.status == 'undefined' || matches.status != 0) {
          db.cache[searchString] = matches;
          // Verify if these are still the matches the user wants to see.
          if (db.searchString == searchString) {
            db.owner.found(matches);
          }
          db.owner.setStatus('found');
        }
      },
      error: function (xmlhttp) {
        alert(Drupal.ajaxError(xmlhttp, db.uri));
      }
    });
  }, this.delay);
};

})(jQuery);
