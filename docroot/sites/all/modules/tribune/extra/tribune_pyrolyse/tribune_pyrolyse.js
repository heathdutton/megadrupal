// vim:et:sw=2
(function($) {

Drupal.tribune_pyrolyse = {};

Drupal.behaviors.tribune_pyrolyse = {
  attach: function(context) {
    $('div.tribune-wrapper.tribune-local', context).each(function(index, element) {
      var tribune = $(this);

      tribune.data('pyrolyse', {queue: {ids: [], posts: {}}});

      tribune.bind('tribunereloading', function() {
        return false;
      });

      var socket = io.connect(tribune.data('tribunesettings').pyrolyse.socket_server);
      socket.on('connect', (function(tribune) {return function(data) {
        Drupal.tribune_pyrolyse.tribune_auth(tribune, socket.socket.sessionid);

        socket.on('tribune-message', function(data) {
          Drupal.tribune_pyrolyse.tribune_new_post(tribune, data);
        });

        socket.on('tribune-delete-message', function(data) {
          Drupal.tribune_pyrolyse.tribune_delete_post(tribune, data.post_id);
        });
      }})(tribune));
    })
  }
};

Drupal.tribune_pyrolyse.tribune_auth = function(tribune, socket_id) {
  $.post(tribune.data('tribunesettings').pyrolyse.auth_url, {'socket_id': socket_id});
};

Drupal.tribune_pyrolyse.tribune_new_post = function(tribune, message) {
  if (tribune.data('tribune-pyrolyse-timer')) {
    clearTimeout(tribune.data('tribune-pyrolyse-timer'));
  }

  tribune.data('pyrolyse').queue.ids.push(message.data.post_id);
  tribune.data('pyrolyse').queue.posts[message.data.post_id] = message.data.post_html;

  tribune.data('tribune-pyrolyse-timer',
    setTimeout(
      (function(tribune) {return function() {
        Drupal.tribune_pyrolyse.insertQueue(tribune);
      };})(tribune)
      , 250
    )
  );
};

Drupal.tribune_pyrolyse.tribune_delete_post = function(tribune, post_id) {
  Drupal.tribune.postDelete(tribune, post_id);
};

Drupal.tribune_pyrolyse.insertQueue = function(tribune) {
  var posts = {};

  var ids = tribune.data('pyrolyse').queue.ids;
  var queued_posts = tribune.data('pyrolyse').queue.posts;
  tribune.data('pyrolyse').queue = {ids: [], posts: {}};

  ids.sort();

  for (var i in ids) {
    var id = ids[i];
    posts[id] = queued_posts[id];
  }

  Drupal.tribune.addNewPosts(tribune, posts);
};

})(jQuery);
