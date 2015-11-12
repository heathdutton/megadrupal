(function($) {

Drupal.tribune_local = {};

Drupal.behaviors.tribune_local = {
  attach: function(context) {
    for (var i in Drupal.settings.tribune) {
      if (undefined != Drupal.settings.tribune[i].activeusers && !Drupal.settings.tribune[i].activeuserstimer) {
        var nid = i.replace(/tribune-/, '');
        var timeout = 1000 * 180; // 3 minutes
        var element = $('#tribune-active-users-' + nid);
        if (element.length > 0) {
          Drupal.settings.tribune[i].activeuserstimer = setTimeout(function() {
            Drupal.tribune_local.reloadActiveUsers(element, Drupal.settings.tribune[i].activeusers, timeout, nid);
          }, timeout);
        }
      }
    }
  }
};

Drupal.tribune_local.reloadActiveUsers = function(element, url, timeout, nid) {
  var request = {
    url: url,
    dataType: 'html',
    cache: false,
    type: 'GET',
    success: function(data, textStatus, jqXHR) {
      element.html(data);
    },
    complete: function() {
      setTimeout(function() {Drupal.tribune_local.reloadActiveUsers(element, url, timeout)}, timeout);
    }
  };

  if ($('div[data-nid=' + nid + ']').length > 0) {
    // the user is viewing this tribune, let's tell the server that he's there
    request.type = 'POST';
    request.data = {'active': true};
  }

  $.ajax(request);
}

})(jQuery);
