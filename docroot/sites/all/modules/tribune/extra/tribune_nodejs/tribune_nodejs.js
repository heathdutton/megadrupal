(function($) {

Drupal.tribune_nodejs = {};

Drupal.behaviors.tribune_nodejs = {
  attach: function(context) {
    var push_tribunes = {};

    $('div.tribune-wrapper.tribune-local', context).each(function(index, element) {
      var tribune = $(this);

      tribune.bind('tribunereloading', function() {
        return false;
      });

      var settings = Drupal.settings.tribune['tribune-' + $(this).data('nid')];
      settings.nodejs = {};
      settings.nodejs.queue = {ids: [], posts: {}};
      settings.nodejs.timer = 0;
    })
  }
};

Drupal.Nodejs.callbacks.tribune_new_post = {
  callback: function(message) {
    var settings = Drupal.settings.tribune['tribune-' + message.data.tribune];
    if (settings.nodejs.timer) {
      clearTimeout(settings.nodejs.timer);
    }

    settings.nodejs.queue.ids.push(message.data.post_id);
    settings.nodejs.queue.posts[message.data.post_id] = message.data.post_html;

    settings.nodejs.timer = setTimeout(
      (function(nid) {
        return function() {
          Drupal.tribune_nodejs.insertQueue(nid);
        };
      })(message.data.tribune)
      , 250
    );
  }
};

Drupal.Nodejs.callbacks.tribune_delete_post = {
  callback: function(message) {
    Drupal.tribune.postDelete($('.tribune-wrapper[data-nid=' + message.data.tribune + ']'), message.data.post_id);
  }
};

Drupal.tribune_nodejs.insertQueue = function(nid) {
  var settings = Drupal.settings.tribune['tribune-' + nid];

  var posts = {};

  var ids = settings.nodejs.queue.ids;
  var queued_posts = settings.nodejs.queue.posts;
  settings.nodejs.queue = {ids: [], posts: {}};

  ids.sort();

  for (var i in ids) {
    var id = ids[i];
    posts[id] = queued_posts[id];
  }

  $('.tribune-wrapper[data-nid=' + nid + ']').each(function() {
    Drupal.tribune.addNewPosts($(this), posts);
  });
};

})(jQuery);
