// Wrapper for jQuery
(function ($) {
mkdru.FormHandler = function () {};

// update mkdru_form theme's ui to match state
mkdru.FormHandler.prototype.fromState = function () {
  for (var key in mkdru.state) {
    switch(key) {
    case 'query':
      $('.mkdru-search input:text').attr('value', mkdru.state[key]);
      break;
    case 'perpage':
      $('.mkdru-perpage').attr('value', mkdru.state[key]);
      break;
    case 'sort':
      $('.mkdru-sort').attr('value', mkdru.state[key]);
      break;
    }
  }
};

mkdru.FormHandler.prototype.updateCriteria = function () {
  mkdru.state.perpage = $('.mkdru-perpage').attr('value');
  mkdru.state.sort = $('.mkdru-sort').attr('value');
};

mkdru.FormHandler.prototype.updateState = function () {
  mkdru.state.query = $('.mkdru-search input:text').attr('value');
  mkdru.form.updateCriteria();
};

mkdru.FormHandler.prototype.autocomplete = function(request, response) {
  var URL = mkdru.settings.autocomplete.alt_url || mkdru.settings.pz2_path;
  URL += '?command=autocomplete&query=';
  $.get(URL + request.term, function (data) {
    var suggestions = [];
    $(data).find('item').each(function() {
      suggestions.push($(this).attr('name'));
    });
    response(suggestions);
  });
};

// called when the DOM is ready
mkdru.FormHandler.prototype.init = function () {
  $(window).bind( 'hashchange', mkdru.hashChange);
  $('.mkdru-search').bind('submit', mkdru.submitQuery);
  $('.mkdru-search input:text').attr('value', '');
  // sort and results per page are criteria of pz2.show(),
  // changing these does not need to trigger a whole search
  $('.mkdru-perpage').bind('change', mkdru.submitCriteria);
  $('.mkdru-sort').bind('change', mkdru.submitCriteria);
  // pazpar2 doesn't support autocomplete but you can use a different service
  if (mkdru.settings.autocomplete &&
    mkdru.settings.autocomplete.use_autocomplete &&
    (mkdru.settings.is_service_proxy || mkdru.settings.autocomplete.alt_url)) {
    $('.mkdru-autocomplete').autocomplete({source: this.autocomplete});
  }
};

mkdru.form = new mkdru.FormHandler();
})(jQuery);
